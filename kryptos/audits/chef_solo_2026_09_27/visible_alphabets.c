/* visible_alphabets.c
 * Genere une famille FINIE d'alphabets-permutations derives de l'ORDRE SPATIAL
 * du texte grave (panneau chiffre, 869 lettres, identites verifiees).
 * Motivation (base 15) : le verrou residuel de K4 est l'alphabet libre ; il doit
 * venir d'une feature VISIBLE. S1-S4 ont ferme les alphabets connus/affines/
 * structures, mais JAMAIS un alphabet auto-keye par la lecture spatiale du texte.
 * Un alphabet = ordre de PREMIERE (ou derniere) occurrence des 26 lettres A-Z
 * le long d'un parcours spatial du panneau (lecture, colonne, miroir, retourne,
 * boustrophedon, arc). Sortie : chaines de 26 lettres, dedupliquees, etiquetees.
 *
 * Compile: cc -O2 -o visible_alphabets visible_alphabets.c
 * Usage:   ./visible_alphabets data/master_cipher_letters.csv
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int gidx,row,col; char letter; double x,y,z,arc; int is_k4; } Cell;

static Cell C[1000];
static int N=0;

/* -- parcours : renvoient un ordre d'indices dans C[] -- */
static int idx[1000];

static int cmp_grid_rc(const void*a,const void*b){ const Cell*p=&C[*(int*)a],*q=&C[*(int*)b]; if(p->row!=q->row)return p->row-q->row; return p->col-q->col; }
static int cmp_col_major(const void*a,const void*b){ const Cell*p=&C[*(int*)a],*q=&C[*(int*)b]; if(p->col!=q->col)return p->col-q->col; return p->row-q->row; }
static int cmp_col_major_rev(const void*a,const void*b){ const Cell*p=&C[*(int*)a],*q=&C[*(int*)b]; if(p->col!=q->col)return q->col-p->col; return p->row-q->row; }
static int cmp_row_desc(const void*a,const void*b){ const Cell*p=&C[*(int*)a],*q=&C[*(int*)b]; if(p->row!=q->row)return q->row-p->row; return p->col-q->col; }
static int cmp_row_mirror(const void*a,const void*b){ const Cell*p=&C[*(int*)a],*q=&C[*(int*)b]; if(p->row!=q->row)return p->row-q->row; return q->col-p->col; }
static int cmp_arc(const void*a,const void*b){ const Cell*p=&C[*(int*)a],*q=&C[*(int*)b]; if(p->arc<q->arc)return -1; if(p->arc>q->arc)return 1; if(p->z>q->z)return -1; if(p->z<q->z)return 1; return 0; }

/* alphabet par ordre de premiere occurrence sur un ordre d'indices donne */
static void first_occ(int *order,int n,int reverse,int k4only,char out[27]){
    int seen[26]={0}; int m=0; char buf[27];
    for(int t=0;t<n;t++){ int i = reverse? order[n-1-t] : order[t];
        if(k4only && !C[i].is_k4) continue;
        char c=C[i].letter; if(c<'A'||c>'Z') continue;
        if(!seen[c-'A']){ seen[c-'A']=1; buf[m++]=c; if(m==26) break; } }
    buf[m]=0; if(m==26) strcpy(out,buf); else out[0]=0; /* incomplet -> vide */
}
static void last_occ(int *order,int n,int k4only,char out[27]){
    /* derniere occurrence = premiere occurrence sur l'ordre inverse, puis on garde
       l'ordre d'apparition des dernieres occurrences (scan avant, on remplace) */
    int pos[26]; for(int k=0;k<26;k++)pos[k]=-1;
    for(int t=0;t<n;t++){ int i=order[t]; if(k4only&&!C[i].is_k4)continue; char c=C[i].letter; if(c<'A'||c>'Z')continue; pos[c-'A']=t; }
    /* tri des lettres par pos croissant */
    int ord[26],ok=1; for(int k=0;k<26;k++){ord[k]=k; if(pos[k]<0)ok=0;}
    for(int a=0;a<26;a++)for(int b=a+1;b<26;b++) if(pos[ord[b]]<pos[ord[a]]){int t=ord[a];ord[a]=ord[b];ord[b]=t;}
    if(!ok){out[0]=0;return;} for(int k=0;k<26;k++)out[k]='A'+ord[k]; out[26]=0;
}

/* deduplication + sortie */
static char store[200][27]; static char label[200][64]; static int S=0;
static void emit(const char*lab,const char*alpha){
    if(!alpha||!alpha[0]||strlen(alpha)!=26) return;
    for(int i=0;i<S;i++) if(!strcmp(store[i],alpha)){ /* deja vu */
        char t[200]; snprintf(t,sizeof t,"%s | %s",label[i],lab);
        strncpy(label[i],t,63); label[i][63]=0; return; }
    strcpy(store[S],alpha); strncpy(label[S],lab,63); label[S][63]=0; S++;
}

int main(int argc,char**argv){
    if(argc<2){fprintf(stderr,"usage: %s master_cipher_letters.csv\n",argv[0]);return 1;}
    FILE*f=fopen(argv[1],"r"); if(!f){perror("open");return 1;}
    char line[512]; fgets(line,sizeof line,f); /* header */
    while(fgets(line,sizeof line,f)){
        Cell c; char sec[16]; char cr[8],q[8],conf[16];
        /* gidx,row,col,letter,section,is_crib,is_q,conf,x_m,y_m,z_m,arc_s */
        char lch=0;
        int nf=sscanf(line,"%d,%d,%d,%c,%15[^,],%7[^,],%7[^,],%15[^,],%lf,%lf,%lf,%lf",
            &c.gidx,&c.row,&c.col,&lch,sec,cr,q,conf,&c.x,&c.y,&c.z,&c.arc);
        if(nf<12) continue; c.letter=lch; c.is_k4 = (strncmp(sec,"K4",2)==0);
        C[N++]=c;
    }
    fclose(f);
    fprintf(stderr,"lu %d cellules\n",N);

    /* ordres de base */
    for(int i=0;i<N;i++)idx[i]=i;

    char a[27];
    /* 1 lecture (gidx) */
    qsort(idx,N,sizeof(int),cmp_grid_rc);
    first_occ(idx,N,0,0,a); emit("read_full_first",a);
    first_occ(idx,N,1,0,a); emit("read_full_first_REV",a);
    last_occ(idx,N,0,a);   emit("read_full_last",a);
    first_occ(idx,N,0,1,a); emit("read_K4_first",a);
    first_occ(idx,N,1,1,a); emit("read_K4_first_REV",a);
    last_occ(idx,N,1,a);   emit("read_K4_last",a);
    /* 2 colonne-major (haut->bas, colonnes g->d) */
    qsort(idx,N,sizeof(int),cmp_col_major);
    first_occ(idx,N,0,0,a); emit("colmajor_first",a);
    first_occ(idx,N,1,0,a); emit("colmajor_first_REV",a);
    /* 3 colonne-major miroir (colonnes d->g) = flip horizontal puis colonnes */
    qsort(idx,N,sizeof(int),cmp_col_major_rev);
    first_occ(idx,N,0,0,a); emit("colmajor_mirror_first",a);
    first_occ(idx,N,0,1,a); emit("colmajor_mirror_K4_first",a);
    /* 4 lignes retournees (upside-down) */
    qsort(idx,N,sizeof(int),cmp_row_desc);
    first_occ(idx,N,0,0,a); emit("rows_upsidedown_first",a);
    first_occ(idx,N,0,1,a); emit("rows_upsidedown_K4_first",a);
    /* 5 miroir de chaque ligne (read from behind) */
    qsort(idx,N,sizeof(int),cmp_row_mirror);
    first_occ(idx,N,0,0,a); emit("rows_mirror_first",a);
    first_occ(idx,N,0,1,a); emit("rows_mirror_K4_first",a);
    /* 6 par arc (position horizontale depliee) */
    qsort(idx,N,sizeof(int),cmp_arc);
    first_occ(idx,N,0,0,a); emit("arc_first",a);
    first_occ(idx,N,1,0,a); emit("arc_first_REV",a);

    /* rotation 180 = lignes desc + colonnes desc */
    {
        for(int i=0;i<N;i++)idx[i]=i;
        /* tri: row desc, col desc */
        for(int x=0;x<N;x++)for(int y=x+1;y<N;y++){int p=idx[x],q=idx[y]; int sw=0;
            if(C[p].row<C[q].row)sw=1; else if(C[p].row==C[q].row && C[p].col<C[q].col)sw=1;
            if(sw){idx[x]=q;idx[y]=p;}}
        first_occ(idx,N,0,0,a); emit("rot180_first",a);
        first_occ(idx,N,0,1,a); emit("rot180_K4_first",a);
    }
    /* transpose (col-major) puis reverse global = read-from-behind de la transposee */
    qsort(idx,N,sizeof(int),cmp_col_major);
    first_occ(idx,N,1,0,a); emit("colmajor_readbehind",a);
    /* lecture par diagonales r+c=const (montantes), motive par K3=decimation/diagonale */
    {
        int m=0; int maxs=0; for(int i=0;i<N;i++){int s=C[i].row+C[i].col; if(s>maxs)maxs=s;}
        for(int s=0;s<=maxs;s++) for(int i=0;i<N;i++) if(C[i].row+C[i].col==s) idx[m++]=i;
        first_occ(idx,m,0,0,a); emit("diag_sum_first",a);
        first_occ(idx,m,1,0,a); emit("diag_sum_first_REV",a);
    }
    /* lecture par anti-diagonales r-c=const */
    {
        int m=0; int mn=1<<30,mx=-(1<<30); for(int i=0;i<N;i++){int d=C[i].row-C[i].col; if(d<mn)mn=d; if(d>mx)mx=d;}
        for(int d=mn;d<=mx;d++) for(int i=0;i<N;i++) if(C[i].row-C[i].col==d) idx[m++]=i;
        first_occ(idx,m,0,0,a); emit("antidiag_first",a);
    }
    /* alphabet keye par FREQUENCE (masque enleve le biais : Scheidt) */
    {
        int cnt[26]={0}; for(int i=0;i<N;i++){char c=C[i].letter; if(c>='A'&&c<='Z')cnt[c-'A']++;}
        int ord[26]; for(int k=0;k<26;k++)ord[k]=k;
        /* desc (plus frequent d'abord) */
        for(int x=0;x<26;x++)for(int y=x+1;y<26;y++) if(cnt[ord[y]]>cnt[ord[x]]){int t=ord[x];ord[x]=ord[y];ord[y]=t;}
        for(int k=0;k<26;k++)a[k]='A'+ord[k]; a[26]=0; emit("freq_desc",a);
        for(int k=0;k<26;k++)a[k]='A'+ord[25-k]; a[26]=0; emit("freq_asc",a);
    }

    /* boustrophedon sur la grille */
    {
        int maxrow=0; for(int i=0;i<N;i++) if(C[i].row>maxrow)maxrow=C[i].row;
        int m=0;
        for(int r=0;r<=maxrow;r++){
            /* collecte lignes r */
            int tmp[64],tn=0; for(int i=0;i<N;i++) if(C[i].row==r) tmp[tn++]=i;
            /* tri col */
            for(int x=0;x<tn;x++)for(int y=x+1;y<tn;y++) if(C[tmp[y]].col<C[tmp[x]].col){int t=tmp[x];tmp[x]=tmp[y];tmp[y]=t;}
            if(r&1) for(int x=tn-1;x>=0;x--) idx[m++]=tmp[x];
            else    for(int x=0;x<tn;x++)     idx[m++]=tmp[x];
        }
        first_occ(idx,m,0,0,a); emit("boustrophedon_first",a);
    }

    /* sortie finale */
    printf("# alphabets candidats derives de l'ordre spatial du panneau chiffre\n");
    printf("# %d permutations distinctes\n",S);
    for(int i=0;i<S;i++) printf("%-2d %-40s %s\n",i,label[i],store[i]);
    return 0;
}
