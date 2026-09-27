/* transpose_quag.c — porte NON-1:1 via ORDRES DE LECTURE PHYSIQUES du modele 3D.
 * Idee : le clair a peut-etre ete pose dans un ORDRE PHYSIQUE (serpentin/colonnes/arc/spirale
 * sur la grille gravee reelle), puis substitue (Quagmire). On DE-transpose K4 par chaque ordre
 * physique R (y[k]=ct[R[k]] = chiffre en ordre-clair), puis Quagmire crib-solve + LIRE le clair.
 * NON couvert par T22 (ordre naturel) ni S4 (colonnaire 7/14/21 abstrait) : ici ordres 3D reels.
 * Entree : k4_geom.txt (gidx row col letter x y arc), 97 lignes ordre naturel.
 * Usage : transpose_quag k4_geom.txt qg.bin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 97
static int CT[N],ROW[N],COL[N]; static double ARC[N];
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static const char*KW[4]={"","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*BN[4]={"AZ","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*CN[3]={"VIG","BEAU","VAR"};
static void mkperm(int b,int*perm){int seen[26]={0},al[26],n=0;
  for(const char*p=KW[b];*p;p++){int L=*p-'A';if(!seen[L]){seen[L]=1;al[n++]=L;}}
  for(int L=0;L<26;L++)if(!seen[L])al[n++]=L; for(int i=0;i<26;i++)perm[al[i]]=i;}
static inline int fx(int c,int t,int g){if(c==0)return md(t-g);if(c==1)return md(g-t);return md(t+g);}
static float *QG;
static double qoff(const int*p){double q=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}}return no?q/no:0;}

/* Quagmire periodique sur y (ordre-clair) : conv, alphabet perm (sig=tau=perm, Quagmire III),
 * periode P ; key[r]=perm(cle) derive des cribs par majorite ; renvoie erreurs crib, remplit pt. */
static int quag(int conv,const int*y,const int*perm,int P,int*pt){
  int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i; int err=0;
  for(int r=0;r<P;r++){
    int req[64],nq=0;
    for(int i=r;i<N;i+=P) if(iscrib[i]){ int t=perm[y[i]]; int want=perm[cribL[i]];
      /* y_i chiffre du clair via cle k : t = fx(conv, want, k) ; VIG t=want-k?? on definit
         chiffrement Quagmire: cipherval = fx(conv, plainval, keyval). decrypt: plainval tel que
         fx(conv,plainval,k)=t. On derive k requis: */
      /* decrypt(conv,t,k): VIG t-k, BEAU k-t, VAR t+k. cle requise pour decrypt=want : */
      int k; if(conv==0)k=md(t-want); else if(conv==1)k=md(want+t); else k=md(want-t);
      req[nq++]=k; }
    int bk=0,bc=0;for(int a=0;a<nq;a++){int c=0;for(int b2=0;b2<nq;b2++)if(req[b2]==req[a])c++;if(c>bc){bc=c;bk=req[a];}}
    err+=nq-bc;
    for(int i=r;i<N;i+=P){int t=perm[y[i]]; int pv; if(conv==0)pv=md(t-bk); else if(conv==1)pv=md(bk-t); else pv=md(t+bk); pt[i]=inv[pv];}
  }
  return err;
}

/* construit les ordres de lecture R (R[k]=index naturel lu en k-ieme). retourne nb d'ordres. */
static int build_orders(int ords[][N],char names[][24]){
  int no=0;
  /* 0 identite */
  for(int i=0;i<N;i++)ords[no][i]=i; strcpy(names[no],"identity"); no++;
  /* 1 reverse */
  for(int i=0;i<N;i++)ords[no][i]=N-1-i; strcpy(names[no],"reverse"); no++;
  /* 2 colmajor (tri par col puis row) */
  {int idx[N];for(int i=0;i<N;i++)idx[i]=i;
   for(int a=0;a<N;a++)for(int b=a+1;b<N;b++){int u=idx[a],v=idx[b]; if(COL[u]>COL[v]||(COL[u]==COL[v]&&ROW[u]>ROW[v])){idx[a]=v;idx[b]=u;}}
   for(int i=0;i<N;i++)ords[no][i]=idx[i]; strcpy(names[no],"colmajor"); no++;}
  /* 3 colmajor reverse */
  for(int i=0;i<N;i++)ords[no][i]=ords[2][N-1-i]; strcpy(names[no],"colmajor_rev"); no++;
  /* 4 serpentin par rows (row croissant ; sens alterne selon parite du row) */
  {int idx[N],m=0; int rmin=1000,rmax=-1; for(int i=0;i<N;i++){if(ROW[i]<rmin)rmin=ROW[i];if(ROW[i]>rmax)rmax=ROW[i];}
   for(int r=rmin;r<=rmax;r++){int cells[64],nc=0; for(int i=0;i<N;i++)if(ROW[i]==r)cells[nc++]=i;
     for(int a=0;a<nc;a++)for(int b=a+1;b<nc;b++)if(COL[cells[a]]>COL[cells[b]]){int t=cells[a];cells[a]=cells[b];cells[b]=t;}
     if((r-rmin)&1) for(int a=nc-1;a>=0;a--)idx[m++]=cells[a]; else for(int a=0;a<nc;a++)idx[m++]=cells[a];}
   for(int i=0;i<N;i++)ords[no][i]=idx[i]; strcpy(names[no],"serpentine"); no++;}
  /* 5 serpentin reverse */
  for(int i=0;i<N;i++)ords[no][i]=ords[4][N-1-i]; strcpy(names[no],"serpentine_rev"); no++;
  /* 6 tri par arc_s (position sur la courbe depliee) */
  {int idx[N];for(int i=0;i<N;i++)idx[i]=i;
   for(int a=0;a<N;a++)for(int b=a+1;b<N;b++)if(ARC[idx[a]]>ARC[idx[b]]){int t=idx[a];idx[a]=idx[b];idx[b]=t;}
   for(int i=0;i<N;i++)ords[no][i]=idx[i]; strcpy(names[no],"arc_sorted"); no++;}
  /* 7 arc reverse */
  for(int i=0;i<N;i++)ords[no][i]=ords[6][N-1-i]; strcpy(names[no],"arc_rev"); no++;
  return no;
}

int main(int argc,char**argv){
  setcribs();
  if(argc<3){fprintf(stderr,"usage: transpose_quag k4_geom.txt qg.bin\n");return 1;}
  FILE*f=fopen(argv[1],"r"); char line[256]; int n=0;
  while(fgets(line,sizeof line,f)){int g,r,c;char L;double x,y,arc;
    if(sscanf(line,"%d %d %d %c %lf %lf %lf",&g,&r,&c,&L,&x,&y,&arc)==7){CT[n]=L-'A';ROW[n]=r;COL[n]=c;ARC[n]=arc;n++;}}
  fclose(f);
  if(n!=N){fprintf(stderr,"got %d letters\n",n);return 2;}
  FILE*q=fopen(argv[2],"rb");QG=malloc(sizeof(float)*456976);
  if(fread(QG,sizeof(float),456976,q)!=456976){fprintf(stderr,"qg\n");return 2;}fclose(q);
  /* CONTROLE POSITIF : fabrique y=Vigenere(English+cribs, AZ, key P=5), ordre identite,
     quag doit rendre cribErr=0 et retrouver le clair. */
  {int perm[26];mkperm(0,perm); int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
   const char*eng="THEQUICKBROWNFOXJUMPSOVERTHELAZYDOGXXWORDFILLERTOREACHNINETYSEVENLETTERSFORTHISCONTROLTESTOKAYNOW";
   int pt0[N];for(int i=0;i<N;i++)pt0[i]=eng[i]-'A'; for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt0[p]=cribL[p];}
   int key[5]={3,14,7,21,4}; int y0[N];
   for(int i=0;i<N;i++){int pv=perm[pt0[i]]; int t=md(pv+key[i%5]); y0[i]=inv[t];} /* VIG chiffre: t=pv+k */
   int pt2[N];int e=quag(0,y0,perm,5,pt2); int rec=0;for(int i=0;i<N;i++)if(pt2[i]==pt0[i])rec++;
   printf("CONTROLE POSITIF (Vig AZ P=5, ordre identite): cribErr=%d recovered=%d/%d -> %s\n",e,rec,N,(e==0&&rec==N)?"PASS":"FAIL");}
  static int ords[16][N]; static char names[16][24]; int no=build_orders(ords,names);
  printf("cible solution: qoff~-4.3, cribErr 0-1. (ordres 3D x 4 alphabets x 3 conv x periode 1-14)\n");
  double bestq=-1e9; char bestlab[128]; int bestpt[N],besterr=99;
  for(int o=0;o<no;o++){
    int y[N]; for(int k=0;k<N;k++)y[k]=CT[ords[o][k]];
    for(int b=0;b<4;b++){int perm[26];mkperm(b,perm);
      for(int conv=0;conv<3;conv++)for(int P=1;P<=14;P++){
        int pt[N];int e=quag(conv,y,perm,P,pt); double qq=qoff(pt);
        /* on garde le meilleur par qoff parmi ceux a peu d'erreurs */
        if(e<=3 && qq>bestq){bestq=qq;besterr=e;memcpy(bestpt,pt,sizeof bestpt);
          snprintf(bestlab,sizeof bestlab,"order=%s alpha=%s %s P=%d",names[o],BN[b],CN[conv],P);}
      }}
  }
  printf("MEILLEUR (cribErr<=3, max qoff) : %s | cribErr=%d qoff=%.2f\nPT=",bestlab,besterr,bestq);
  for(int i=0;i<N;i++)putchar('A'+bestpt[i]);putchar('\n');
  /* aussi : le meilleur par cribErr minimal */
  int mine=99; char mlab[128]; int mpt[N]; double mq=0;
  for(int o=0;o<no;o++){int y[N];for(int k=0;k<N;k++)y[k]=CT[ords[o][k]];
    for(int b=0;b<4;b++){int perm[26];mkperm(b,perm);for(int conv=0;conv<3;conv++)for(int P=1;P<=14;P++){
      int pt[N];int e=quag(conv,y,perm,P,pt); if(e<mine){mine=e;mq=qoff(pt);memcpy(mpt,pt,sizeof mpt);snprintf(mlab,sizeof mlab,"order=%s alpha=%s %s P=%d",names[o],BN[b],CN[conv],P);}}}}
  printf("MIN cribErr : %s | cribErr=%d qoff=%.2f\nPT=",mlab,mine,mq);
  for(int i=0;i<N;i++)putchar('A'+mpt[i]);putchar('\n');
  return 0;
}
