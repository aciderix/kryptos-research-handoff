/* e01_solver.c — E01 (voir experiments/E01_routes14/PREREGISTRATION.md).
 * Polybe 5×5 inconnu (substitution simple symbole→lettre) + route de transposition sur grille H×W.
 * Géométrie A : 14×14 (196). Géométrie B : colonne 14 retirée → 14×13 (182).
 * 64 routes × 2 sens ; substitution résolue par recuit à score quadgramme incrémental (qg_big).
 *
 * Usage : e01_solver qg_big.bin cipher.txt control [n]   -> contrôles positifs (A et B)
 *         e01_solver qg_big.bin cipher.txt null [n]      -> distribution nulle (paires réelles mélangées)
 *         e01_solver qg_big.bin cipher.txt real          -> vrai chiffré
 *         e01_solver qg_big.bin cipher.txt control_corpus corpus.txt [n] (textes anglais réels)
 * Compile : cc -O2 -o e01_solver e01_solver.c -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define MAXN 196
static float *QG;
static uint64_t rs=88172645463325252ULL;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (rnd()>>11)*(1.0/9007199254740992.0);}

/* ---------------- routes ---------------- */
#define MAXR 80
static int NR; static int ROUTE[MAXR][MAXN]; static char RNAME[MAXR][32];
static void addroute(const int*o,int n,const char*nm){memcpy(ROUTE[NR],o,sizeof(int)*n);snprintf(RNAME[NR],32,"%s",nm);NR++;}
static void build_routes(int H,int W){
  NR=0; int n=H*W; int o[MAXN]; char nm[32];
  /* 8 diédrales × lignes/colonnes × simple/boustrophédon */
  for(int d=0;d<8;d++)for(int colread=0;colread<2;colread++)for(int bou=0;bou<2;bou++){
    int sw=(d==1||d==3||d==6||d==7); int RR=sw?W:H, WW=sw?H:W; int k=0;
    int outer=colread?WW:RR, inner=colread?RR:WW;
    for(int a=0;a<outer;a++)for(int t=0;t<inner;t++){int b=(bou&&(a&1))?inner-1-t:t;
      int r2=colread?b:a, c2=colread?a:b, r,c;
      switch(d){case 0:r=r2;c=c2;break;case 1:r=H-1-c2;c=r2;break;case 2:r=H-1-r2;c=W-1-c2;break;case 3:r=c2;c=W-1-r2;break;
        case 4:r=r2;c=W-1-c2;break;case 5:r=H-1-r2;c=c2;break;case 6:r=c2;c=r2;break;default:r=H-1-c2;c=W-1-r2;}
      o[k++]=r*W+c;}
    snprintf(nm,32,"d%d-%s%s",d,colread?"col":"row",bou?"-bou":"");addroute(o,n,nm);}
  /* diagonales : 4 coins × simple/zigzag × anti(r+c)/main(r-c) */
  for(int corner=0;corner<4;corner++)for(int zz=0;zz<2;zz++)for(int typ=0;typ<2;typ++){
    int k=0; int kmin=typ?-(W-1):0, kmax=typ?(H-1):(H+W-2);
    for(int key=kmin,di=0;key<=kmax;key++,di++){
      int cells[MAXN],m=0;
      for(int r=0;r<H;r++){int c=typ?(r-key):(key-r); if(c<0||c>=W)continue; cells[m++]=r*W+c;}
      if(zz&&(di&1)) for(int i=m-1;i>=0;i--) o[k++]=cells[i]; else for(int i=0;i<m;i++) o[k++]=cells[i];}
    /* réflexion de coin */
    for(int i=0;i<n;i++){int r=o[i]/W,c=o[i]%W; if(corner&1)c=W-1-c; if(corner&2)r=H-1-r; o[i]=r*W+c;}
    snprintf(nm,32,"diag-%s-c%d%s",typ?"main":"anti",corner,zz?"-zz":"");addroute(o,n,nm);}
  /* spirales : 1er mouvement droite/bas × 4 coins × vers intérieur/extérieur */
  for(int first=0;first<2;first++)for(int corner=0;corner<4;corner++)for(int out=0;out<2;out++){
    int top=0,bot=H-1,lef=0,rig=W-1,k=0;
    while(top<=bot&&lef<=rig){
      if(first==0){ for(int c=lef;c<=rig;c++)o[k++]=top*W+c; top++;
        for(int r=top;r<=bot;r++)o[k++]=r*W+rig; rig--;
        if(top<=bot){for(int c=rig;c>=lef;c--)o[k++]=bot*W+c; bot--;}
        if(lef<=rig){for(int r=bot;r>=top;r--)o[k++]=r*W+lef; lef++;} }
      else { for(int r=top;r<=bot;r++)o[k++]=r*W+lef; lef++;
        for(int c=lef;c<=rig;c++)o[k++]=bot*W+c; bot--;
        if(lef<=rig){for(int r=bot;r>=top;r--)o[k++]=r*W+rig; rig--;}
        if(top<=bot){for(int c=rig;c>=lef;c--)o[k++]=top*W+c; top++;} }
    }
    for(int i=0;i<n;i++){int r=o[i]/W,c=o[i]%W; if(corner&1)c=W-1-c; if(corner&2)r=H-1-r; o[i]=r*W+c;}
    if(out){int t[MAXN];for(int i=0;i<n;i++)t[i]=o[n-1-i];memcpy(o,t,sizeof(int)*n);}
    snprintf(nm,32,"spir-%s-c%d-%s",first?"down":"right",corner,out?"out":"in");addroute(o,n,nm);}
}
/* clair à partir du chiffré (ordre lignes) : dir0 clair[k]=S[R[k]] ; dir1 clair[R[k]]=S[k] */
static void apply_route(const int*S,int n,int r,int dir,int*out){
  if(dir==0) for(int k=0;k<n;k++) out[k]=S[ROUTE[r][k]];
  else for(int k=0;k<n;k++) out[ROUTE[r][k]]=S[k];
}

/* ---------------- substitution : recuit incrémental ---------------- */
static int L; static int SEQ[MAXN]; static int NS; /* symboles 0..NS-1 */
static int posl[25][MAXN],npos[25];
static int map[25],used[26]; static float wval[MAXN]; static int stamp[MAXN],stampc=0;
static inline float qw(int i){int a=map[SEQ[i]],b=map[SEQ[i+1]],c=map[SEQ[i+2]],d=map[SEQ[i+3]];return QG[((a*26+b)*26+c)*26+d];}
static double total(void){double s=0;for(int i=0;i+3<L;i++){wval[i]=qw(i);s+=wval[i];}return s;}
static const char*EFREQ="ETAOINSHRDLCUMWFGYPBVKJXQZ";
static double solve_sub(int restarts,int iters,int*bestmap){
  /* index des positions par symbole */
  for(int s=0;s<NS;s++)npos[s]=0; for(int i=0;i<L;i++){int s=SEQ[i];posl[s][npos[s]++]=i;}
  int present[25],np=0; for(int s=0;s<NS;s++) if(npos[s]) present[np++]=s;
  double gbest=-1e18;
  for(int R=0;R<restarts;R++){
    /* départ : rang de fréquence (R=0) sinon aléatoire */
    int ord[25];for(int i=0;i<np;i++)ord[i]=present[i];
    for(int a=0;a<np;a++)for(int b=a+1;b<np;b++) if(npos[ord[b]]>npos[ord[a]]){int t=ord[a];ord[a]=ord[b];ord[b]=t;}
    for(int l=0;l<26;l++)used[l]=-1; for(int s=0;s<25;s++)map[s]=0;
    int lets[26];for(int i=0;i<26;i++)lets[i]=EFREQ[i]-'A';
    if(R>0) for(int i=25;i>0;i--){int j=rnd()%(i+1);int t=lets[i];lets[i]=lets[j];lets[j]=t;}
    for(int i=0;i<np;i++){map[ord[i]]=lets[i];used[lets[i]]=ord[i];}
    double cur=total(),best=cur; int bm[25];memcpy(bm,map,sizeof bm);
    double T0=2.5,T1=0.05;
    for(int it=0;it<iters;it++){
      double T=T0*pow(T1/T0,(double)it/iters);
      int a=present[rnd()%np]; int newl=rnd()%26; int b=used[newl]; if(b==a) continue;
      int la=map[a];
      /* propose : a<-newl ; b (si existe) <- la */
      stampc++; int wins[4*2*MAXN],nw=0;
      for(int q=0;q<2;q++){int s=q?b:a; if(s<0)continue; for(int t=0;t<npos[s];t++){int p=posl[s][t];
        for(int i=p-3;i<=p;i++) if(i>=0&&i+3<L&&stamp[i]!=stampc){stamp[i]=stampc;wins[nw++]=i;}}}
      map[a]=newl; if(b>=0)map[b]=la;
      double delta=0; float nv[4*2*MAXN]; for(int w=0;w<nw;w++){nv[w]=qw(wins[w]);delta+=nv[w]-wval[wins[w]];}
      if(delta>=0||urand()<exp(delta/T)){ for(int w=0;w<nw;w++)wval[wins[w]]=nv[w]; cur+=delta;
        used[newl]=a; used[la]=b; if(cur>best){best=cur;memcpy(bm,map,sizeof bm);} }
      else { map[a]=la; if(b>=0)map[b]=newl; }
    }
    if(getenv("DBG")){ double chk=total(); fprintf(stderr,"  restart %d: start->best %.1f  (recalc cur=%.1f vs tracked %.1f) np=%d\n",R,best,chk,cur,np);}
    if(best>gbest){gbest=best;memcpy(bestmap,bm,sizeof(int)*25);}
  }
  return gbest/(L-3);
}

/* ---------------- recherche sur toute la famille ---------------- */
typedef struct{double q; int geo,r,dir; int map[25]; int plain[MAXN]; int n;} Best;
static int RESTARTS=6, ITERS=30000;
static void search(const int*S196,Best*best,int verbose,int wantgeo){
  best->q=-1e9;
  for(int geo=0;geo<2;geo++){ if(wantgeo>=0&&geo!=wantgeo)continue;
    int H=14,W=geo?13:14,n=H*W; int S[MAXN];
    if(geo==0) memcpy(S,S196,sizeof(int)*196); else {int k=0;for(int r=0;r<14;r++)for(int c=0;c<13;c++)S[k++]=S196[r*14+c];}
    build_routes(H,W);
    for(int r=0;r<NR;r++)for(int dir=0;dir<2;dir++){
      int pl[MAXN]; apply_route(S,n,r,dir,pl);
      L=n; memcpy(SEQ,pl,sizeof(int)*n);
      int m[25]; double q=solve_sub(RESTARTS,ITERS,m);
      if(verbose>1) printf("  geo%c %-22s dir%d qoff=%.3f\n",geo?'B':'A',RNAME[r],dir,q);
      if(q>best->q){best->q=q;best->geo=geo;best->r=r;best->dir=dir;memcpy(best->map,m,sizeof m);
        best->n=n; for(int i=0;i<n;i++)best->plain[i]=m[pl[i]];}
    }
  }
}
static int CT[196];
static void load_cipher(const char*p){FILE*f=fopen(p,"r");char d[1000];int n=0,c;while((c=fgetc(f))!=EOF)if(c>='0'&&c<='9')d[n++]=c;fclose(f);
  int rows[10]={4,0,0,0,0,0,0,1,2,3}; /* 6->1,7->2,8->3,9->4,0->... */ (void)rows;
  for(int i=0;i<196;i++){int a=d[2*i]-'0',b=d[2*i+1]-'0'; int ri=(a==0)?4:a-6; CT[i]=ri*5+(b-1);} NS=25;}
static void print_plain(const Best*b){for(int i=0;i<b->n;i++)putchar('A'+b->plain[i]);putchar('\n');}

/* ---------------- contrôles ---------------- */
static char *CORPUS; static long CL;
static void mk_control(int geo,int*S196,int*truep,int*tn,int*tr,int*tdir){
  int H=14,W=geo?13:14,n=H*W;
  /* texte anglais du corpus, J->I, lettres seulement */
  long off=rnd()%(CL-5000); int k=0; int txt[MAXN];
  while(k<n){int ch=CORPUS[off++]; if(ch<'A'||ch>'Z')continue; if(ch=='J')ch='I'; txt[k++]=ch-'A';}
  /* carré de Polybe aléatoire (25 lettres sans J) */
  int letters[25],m=0; for(int l=0;l<26;l++) if(l!=9) letters[m++]=l;
  for(int i=24;i>0;i--){int j=rnd()%(i+1);int t=letters[i];letters[i]=letters[j];letters[j]=t;}
  int sym[26]; for(int i=0;i<25;i++) sym[letters[i]]=i;
  build_routes(H,W); int r=rnd()%NR, dir=rnd()%2;
  /* clair -> grille : inverse de apply_route */
  int S[MAXN];
  if(dir==0) for(int kk=0;kk<n;kk++) S[ROUTE[r][kk]]=sym[txt[kk]];
  else for(int kk=0;kk<n;kk++) S[kk]=sym[txt[ROUTE[r][kk]]];
  if(geo==0) memcpy(S196,S,sizeof(int)*196);
  else { int kk=0; for(int rr=0;rr<14;rr++){for(int c=0;c<13;c++)S196[rr*14+c]=S[kk++]; S196[rr*14+13]=rnd()%25;} }
  memcpy(truep,txt,sizeof(int)*n); *tn=n; *tr=r; *tdir=dir;
}

int main(int argc,char**argv){
  setbuf(stdout,NULL);
  if(argc<4){fprintf(stderr,"usage: e01_solver qg.bin cipher.txt control|null|real [n] [corpus]\n");return 1;}
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  load_cipher(argv[2]);
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  if(getenv("RESTARTS"))RESTARTS=atoi(getenv("RESTARTS")); if(getenv("ITERS"))ITERS=atoi(getenv("ITERS"));
  if(!strcmp(argv[3],"control")){
    int nc=argc>4?atoi(argv[4]):5; const char*cp=argc>5?argv[5]:NULL;
    FILE*g=fopen(cp,"rb");fseek(g,0,SEEK_END);CL=ftell(g);fseek(g,0,SEEK_SET);CORPUS=malloc(CL);if(fread(CORPUS,1,CL,g)!=(size_t)CL)return 3;fclose(g);
    for(int geo=0;geo<2;geo++){int ok=0;
      for(int t=0;t<nc;t++){int S196[196],tp[MAXN],tn,tr,tdir; mk_control(geo,S196,tp,&tn,&tr,&tdir);
        { /* diagnostic : recuit sur la VRAIE route seule */
          int H=14,W=geo?13:14,n=H*W,S[MAXN]; if(geo==0)memcpy(S,S196,sizeof(int)*196); else{int k=0;for(int r=0;r<14;r++)for(int c=0;c<13;c++)S[k++]=S196[r*14+c];}
          build_routes(H,W); int pl[MAXN]; apply_route(S,n,tr,tdir,pl); L=n; memcpy(SEQ,pl,sizeof(int)*n);
          int m[25]; double qt=solve_sub(RESTARTS,ITERS,m); int rr=0; for(int i=0;i<n;i++) if(m[pl[i]]==tp[i]) rr++;
          int mt[25]; for(int s=0;s<25;s++)mt[s]=0; for(int i=0;i<n;i++) mt[pl[i]]=tp[i];
          memcpy(map,mt,sizeof mt); double qtrue=total()/(n-3);
          int distinct=0,seen[25]={0}; for(int i=0;i<n;i++) if(!seen[pl[i]]){seen[pl[i]]=1;distinct++;}
          printf("   [diag] vraie route seule : qoff=%.3f récupéré=%d/%d | qoff de la VRAIE clé=%.3f | symboles=%d\n",qt,rr,n,qtrue,distinct); }
        Best b; search(S196,&b,0,geo);
        int rec=0; for(int i=0;i<tn;i++) if(b.plain[i]==tp[i]) rec++;
        build_routes(14,geo?13:14);
        printf("CTRL geo%c #%d vraie=%s/dir%d  trouvée=%s/dir%d  qoff=%.3f  récupéré=%d/%d %s\n",geo?'B':'A',t,RNAME[tr],tdir,RNAME[b.r],b.dir,b.q,rec,tn,rec>=0.9*tn?"OK":"ÉCHEC");
        if(rec>=0.9*tn)ok++;}
      printf("==> géométrie %c : %d/%d contrôles récupérés (≥90%%)\n",geo?'B':'A',ok,nc);}
    return 0;}
  if(!strcmp(argv[3],"null")){
    int nn=argc>4?atoi(argv[4]):10; double mx=-1e9;
    for(int t=0;t<nn;t++){int S[196];memcpy(S,CT,sizeof S);for(int i=195;i>0;i--){int j=rnd()%(i+1);int x=S[i];S[i]=S[j];S[j]=x;}
      Best b; search(S,&b,0,-1); if(b.q>mx)mx=b.q; printf("NULL #%d best qoff=%.3f (geo%c %d)\n",t,b.q,b.geo?'B':'A',b.r);}
    printf("==> max null = %.3f\n",mx); return 0;}
  if(!strcmp(argv[3],"real")){
    Best b; search(CT,&b,2,-1);
    build_routes(14,b.geo?13:14);
    printf("\nMEILLEURE CELLULE : geo%c route=%s dir%d qoff=%.3f\nCLAIR=",b.geo?'B':'A',RNAME[b.r],b.dir,b.q); print_plain(&b);
    if(b.geo==0){ /* prédiction colonne 14 : où finissent les cases de la colonne 13 ? */
      printf("positions dans le clair des cases de la colonne 14 : ");
      for(int k=0;k<196;k++){int cell=(b.dir==0)?ROUTE[b.r][k]:-1; if(b.dir==1){ /* clair[R[k]]=S[k] : case k du chiffré -> clair R[k] */ }
        (void)cell;}
      for(int rr=0;rr<14;rr++){int cell=rr*14+13; int ppos=-1;
        if(b.dir==0){for(int k=0;k<196;k++)if(ROUTE[b.r][k]==cell)ppos=k;} else ppos=ROUTE[b.r][cell];
        printf("%d ",ppos);}
      printf("\n");}
    { /* vérification directe : clair + carré + route -> 392 chiffres, comparés au fichier */
      int n=b.n,inv[26]; for(int l=0;l<26;l++)inv[l]=-1; int pres[25]={0}; for(int i=0;i<196;i++) if(b.geo==0||i%14!=13) pres[CT[i]]=1; for(int s=0;s<25;s++) if(pres[s]) inv[b.map[s]]=s; /* symboles présents seulement */
      int S[MAXN],S196[196]; memcpy(S196,CT,sizeof S196);
      if(b.dir==0) for(int k=0;k<n;k++) S[ROUTE[b.r][k]]=inv[b.plain[k]]; else for(int k=0;k<n;k++) S[k]=inv[b.plain[ROUTE[b.r][k]]];
      if(b.geo==0) memcpy(S196,S,sizeof S196); else {int k=0;for(int r=0;r<14;r++)for(int c=0;c<13;c++)S196[r*14+c]=S[k++];} /* (B) : nulles de la colonne 14 conservées */
      FILE*g=fopen(argv[2],"r");char d[1000];int nd=0,ch;while((ch=fgetc(g))!=EOF)if(ch>='0'&&ch<='9')d[nd++]=ch;fclose(g);
      const char rowd[5]={'6','7','8','9','0'}; int bad=0;
      for(int i=0;i<196;i++){ if(S196[i]<0){bad++;continue;} if(d[2*i]!=rowd[S196[i]/5]||d[2*i+1]!='1'+S196[i]%5) bad++; }
      printf("ré-enchiffrement : %d/196 paires différentes %s\n",bad,bad?"ÉCHEC":"(exact)"); }
    { /* stabilité : 8 recuits indépendants (1 redémarrage chacun) sur la cellule gagnante */
      int H=14,W=b.geo?13:14,n=H*W,S[MAXN],pl[MAXN]; if(b.geo==0)memcpy(S,CT,sizeof(int)*196); else{int k=0;for(int r=0;r<14;r++)for(int c=0;c<13;c++)S[k++]=CT[r*14+c];}
      apply_route(S,n,b.r,b.dir,pl); int same=0;
      for(int t=0;t<8;t++){ L=n; memcpy(SEQ,pl,sizeof(int)*n); int m[25]; double q=solve_sub(1,ITERS,m); int eq=0; for(int i=0;i<n;i++) if(m[pl[i]]==b.plain[i]) eq++;
        printf("  stabilité #%d qoff=%.3f identique=%d/%d\n",t,q,eq,n); if(eq>=0.95*n) same++; }
      printf("stabilité : %d/8 redémarrages redonnent le même clair (≥95%%)\n",same); }
    return 0;}
  return 1;
}
