/* E16 — hypothèse « 13 classes » (H13) : routes, nulles régulières, clés K1/K2 d'E03, colonnaire à clé libre W=13/14.
 * Voir experiments/E16_thirteen_classes_gaps/PREREGISTRATION.md.
 * Statistique R et solveur de paires repris d'E04/E15 (tools/e04_scan.c) sans changement d'algorithme.
 * Usage : e16_h13 qg_en.bin ciphertext.txt heldout_en.txt MODE CELL [n]
 *   MODE : real | null n | control n | s1ctrl n | s1null n   (s1* : étage 1 seul, cellule F)
 *   CELL : R | N | K | F        Env : SEED, PR_REST (6), PR_ITERS (150000), TOP (10), FREST (20), FITER (100000)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAXN 196
static int N, S0[MAXN], N0;
static uint64_t rs=88172645463325252ULL;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (double)(rnd()>>11)*(1.0/9007199254740992.0);}
static float *QG;

/* ---- statistique invariante (E04) ---- */
static uint32_t *st3,*st4,*st5,gen=0;
static int stat(const int*x){
  gen++; int r3=0,r4=0,r5=0;
  for(int i=0;i+2<N;i++){int k=(x[i]*25+x[i+1])*25+x[i+2]; if(st3[k]==gen)r3++; else st3[k]=gen;
    if(i+3<N){int k4=k*25+x[i+3]; if(st4[k4]==gen)r4++; else st4[k4]=gen;
      if(i+4<N){int k5=k4*25+x[i+4]; if(st5[k5]==gen)r5++; else st5[k5]=gen;}}}
  return r3+2*r4+3*r5; }

/* ---- solveur de paires (E15, identique) ---- */
static int PR_REST=6, PR_ITERS=150000;
static int PL_[13][2], PC_[13], CURLAB[25];
static double pviterbi(const int*y,int n,int*best){
  double dp[8],nd[8]; static int bp[MAXN][8];
  for(int s=0;s<8;s++) dp[s]=0;
  for(int k=0;k<n;k++){ int c=PC_[y[k]]; for(int s=0;s<8;s++) nd[s]=-1e30;
    for(int s=0;s<8;s++){ if(dp[s]<-1e29) continue; int b1=(s>>2)&1,b2=(s>>1)&1,b3=s&1;
      for(int b=0;b<c;b++){ int ns=((s<<1)&7)|b; double v=dp[s];
        if(k>=3) v+=QG[((PL_[y[k-3]][b1]*26+PL_[y[k-2]][b2])*26+PL_[y[k-1]][b3])*26+PL_[y[k]][b]];
        if(v>nd[ns]){nd[ns]=v; bp[k][ns]=s;} } }
    memcpy(dp,nd,sizeof dp); }
  int bs=0; for(int s=1;s<8;s++) if(dp[s]>dp[bs]) bs=s;
  if(best){ int s=bs; for(int k=n-1;k>=0;k--){ best[k]=PL_[y[k]][s&1]; s=bp[k][s]; } }
  return dp[bs]; }
static double solve_pair(const int*pl,int*letters){
  int lab[25]; for(int i=0;i<25;i++) lab[i]=-1; int y[MAXN],k=0,cnt[13]={0};
  for(int i=0;i<N;i++){ if(lab[pl[i]]<0){ if(k>=13) return -1e30; lab[pl[i]]=k++; } y[i]=lab[pl[i]]; cnt[y[i]]++; }
  if(k!=13) return -1e30; memcpy(CURLAB,lab,sizeof lab);
  int ord[13]; for(int i=0;i<13;i++) ord[i]=i; for(int i=0;i<13;i++)for(int j=i+1;j<13;j++) if(cnt[ord[j]]>cnt[ord[i]]){int t=ord[i];ord[i]=ord[j];ord[j]=t;}
  static const char OE[]="ETAOINSHRDLCUMWFGYPBVKXQZ"; double gb=-1e30; int bL[13][2],bC[13];
  for(int R=0;R<PR_REST;R++){ int L[25]; for(int i=0;i<25;i++) L[i]=OE[i]-'A';
    if(R>0) for(int i=24;i>0;i--){int j=rnd()%(i+1);int t=L[i];L[i]=L[j];L[j]=t;}
    PC_[ord[0]]=1; PL_[ord[0]][0]=PL_[ord[0]][1]=L[0]; for(int q=1;q<13;q++){ PC_[ord[q]]=2; PL_[ord[q]][0]=L[q]; PL_[ord[q]][1]=L[25-q]; }
    double cur=pviterbi(y,N,NULL),best=cur; int cL[13][2],cC[13]; memcpy(cL,PL_,sizeof cL); memcpy(cC,PC_,sizeof cC);
    for(int it=0;it<PR_ITERS;it++){ double T=3.0*pow(0.05/3.0,(double)it/PR_ITERS); double u=urand();
      int s1=rnd()%13,s2=rnd()%13; if(s1==s2) continue;
      if(u<0.3){ if(PC_[s1]!=PC_[s2]) continue; int t0=PL_[s1][0],t1=PL_[s1][1]; PL_[s1][0]=PL_[s2][0];PL_[s1][1]=PL_[s2][1];PL_[s2][0]=t0;PL_[s2][1]=t1;
        double ns=pviterbi(y,N,NULL); if(ns>=cur||urand()<exp((ns-cur)/T)) cur=ns; else { t0=PL_[s1][0];t1=PL_[s1][1]; PL_[s1][0]=PL_[s2][0];PL_[s1][1]=PL_[s2][1];PL_[s2][0]=t0;PL_[s2][1]=t1; } }
      else { int a=rnd()%PC_[s1],b=rnd()%PC_[s2]; int t=PL_[s1][a]; PL_[s1][a]=PL_[s2][b]; PL_[s2][b]=t; if(PC_[s1]==1)PL_[s1][1]=PL_[s1][0]; if(PC_[s2]==1)PL_[s2][1]=PL_[s2][0];
        double ns=pviterbi(y,N,NULL); if(ns>=cur||urand()<exp((ns-cur)/T)) cur=ns;
        else { t=PL_[s1][a]; PL_[s1][a]=PL_[s2][b]; PL_[s2][b]=t; if(PC_[s1]==1)PL_[s1][1]=PL_[s1][0]; if(PC_[s2]==1)PL_[s2][1]=PL_[s2][0]; } }
      if(cur>best){best=cur; memcpy(cL,PL_,sizeof cL); memcpy(cC,PC_,sizeof cC);} }
    if(best>gb){gb=best; memcpy(bL,cL,sizeof bL); memcpy(bC,cC,sizeof bC);} }
  memcpy(PL_,bL,sizeof bL); memcpy(PC_,bC,sizeof bC); if(letters) pviterbi(y,N,letters); return gb/(N-3); }

/* ---- routes (E01, identique) sur grille H×W ---- */
#define MAXR 80
static int NR; static int ROUTE[MAXR][MAXN]; static char RNAME[MAXR][32];
static void addroute(const int*o,int n,const char*nm){memcpy(ROUTE[NR],o,sizeof(int)*n);snprintf(RNAME[NR],32,"%s",nm);NR++;}
static void build_routes(int H,int W){
  NR=0; int n=H*W; int o[MAXN]; char nm[32];
  for(int d=0;d<8;d++)for(int colread=0;colread<2;colread++)for(int bou=0;bou<2;bou++){
    int sw=(d==1||d==3||d==6||d==7); int RR=sw?W:H, WW=sw?H:W; int k=0;
    int outer=colread?WW:RR, inner=colread?RR:WW;
    for(int a=0;a<outer;a++)for(int t=0;t<inner;t++){int b=(bou&&(a&1))?inner-1-t:t;
      int r2=colread?b:a, c2=colread?a:b, r,c;
      switch(d){case 0:r=r2;c=c2;break;case 1:r=H-1-c2;c=r2;break;case 2:r=H-1-r2;c=W-1-c2;break;case 3:r=c2;c=W-1-r2;break;
        case 4:r=r2;c=W-1-c2;break;case 5:r=H-1-r2;c=c2;break;case 6:r=c2;c=r2;break;default:r=H-1-c2;c=W-1-r2;}
      o[k++]=r*W+c;}
    snprintf(nm,32,"d%d-%s%s",d,colread?"col":"row",bou?"-bou":"");addroute(o,n,nm);}
  for(int corner=0;corner<4;corner++)for(int zz=0;zz<2;zz++)for(int typ=0;typ<2;typ++){
    int k=0; int kmin=typ?-(W-1):0, kmax=typ?(H-1):(H+W-2);
    for(int key=kmin,di=0;key<=kmax;key++,di++){
      int cells[MAXN],m=0;
      for(int r=0;r<H;r++){int c=typ?(r-key):(key-r); if(c<0||c>=W)continue; cells[m++]=r*W+c;}
      if(zz&&(di&1)) for(int i=m-1;i>=0;i--) o[k++]=cells[i]; else for(int i=0;i<m;i++) o[k++]=cells[i];}
    for(int i=0;i<n;i++){int r=o[i]/W,c=o[i]%W; if(corner&1)c=W-1-c; if(corner&2)r=H-1-r; o[i]=r*W+c;}
    snprintf(nm,32,"diag-%s-c%d%s",typ?"main":"anti",corner,zz?"-zz":"");addroute(o,n,nm);}
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

/* ---- candidats : clair[k] = S[idx[k]], k < n ---- */
#define MAXC 160
typedef struct{ int n; int idx[MAXN]; char name[48]; int R; } Cand;
static Cand C[MAXC]; static int NC;
static void cand_routes(void){ build_routes(14,13); NC=0;
  for(int r=0;r<NR;r++) for(int dir=0;dir<2;dir++){ Cand*c=&C[NC++]; c->n=182;
    if(dir==0) for(int k=0;k<182;k++) c->idx[k]=ROUTE[r][k]; else for(int k=0;k<182;k++) c->idx[ROUTE[r][k]]=k;
    snprintf(c->name,48,"%s/dir%d",RNAME[r],dir); } }
static void cand_nulls(void){ NC=0;
  for(int k=3;k<=5;k++) for(int ph=0;ph<k;ph++){ Cand*c=&C[NC++]; c->n=0; for(int i=0;i<182;i++) if(i%k!=ph) c->idx[c->n++]=i; snprintf(c->name,48,"nulles k=%d phase=%d",k,ph); } }
/* colonnaire E complète (convention E03) : clair(r,c) = S[pos(c)*H + r] ; pos = rang de lecture de la colonne c */
static void idx_colE(int*idx,int W,const int*pos){ int H=182/W; for(int r=0;r<H;r++) for(int c=0;c<W;c++) idx[r*W+c]=pos[c]*H+r; }
/* colonnaire I : S écrit par lignes de W, colonnes lues dans l'ordre P (P[j] = colonne lue en j-ième) */
static void idx_colI(int*idx,int W,const int*P){ int H=182/W,k=0; for(int j=0;j<W;j++) for(int r=0;r<H;r++) idx[k++]=r*W+P[j]; }
static void cand_K(void){ static const int K1[13]={5,3,7,13,8,6,1,4,9,2,11,12,10}, K2[13]={4,9,2,11,7,13,8,6,1,12,10,3,5}; NC=0;
  for(int q=0;q<2;q++) for(int inv=0;inv<2;inv++){ const int*K=q?K2:K1; int pos[13]; for(int c=0;c<13;c++) pos[c]=K[c]-1;
    if(inv){ int t[13]; for(int c=0;c<13;c++) t[pos[c]]=c; memcpy(pos,t,sizeof t); }
    Cand*c=&C[NC++]; c->n=182; idx_colE(c->idx,13,pos); snprintf(c->name,48,"K%d%s",q+1,inv?" (inverse)":""); } }

/* ---- F : recuit sur R dans l'espace des ordres de colonnes ---- */
static int FREST=20, FITER=100000, TOPN=10;
static void mkidx(int conv,int W,const int*P,int*idx){ if(conv==0) idx_colE(idx,W,P); else idx_colI(idx,W,P); }
static int Rof(const int*S,int conv,int W,const int*P){ int idx[MAXN],x[MAXN]; mkidx(conv,W,P,idx); for(int k=0;k<182;k++) x[k]=S[idx[k]]; return stat(x); }
static void cand_F(const int*S){ NC=0; N=182;
  for(int W=13;W<=14;W++) for(int conv=0;conv<2;conv++){
    int bestR[64]; int bestP[64][14]; int nb=0;
    for(int rs_=0;rs_<FREST;rs_++){ int P[14]; for(int i=0;i<W;i++)P[i]=i; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int t=P[i];P[i]=P[j];P[j]=t;}
      int cur=Rof(S,conv,W,P), best=cur; int bP[14]; memcpy(bP,P,sizeof bP);
      for(int it=0;it<FITER;it++){ double T=3.0*pow(0.1/3.0,(double)it/FITER); int Q[14]; memcpy(Q,P,sizeof Q);
        int a=rnd()%W,b=rnd()%W; if(a==b) continue;
        if(rnd()&1){int t=Q[a];Q[a]=Q[b];Q[b]=t;} else { int v=Q[a]; if(a<b){for(int i=a;i<b;i++)Q[i]=Q[i+1];} else {for(int i=a;i>b;i--)Q[i]=Q[i-1];} Q[b]=v; }
        int ns=Rof(S,conv,W,Q); if(ns>=cur||urand()<exp((ns-cur)/T)){cur=ns; memcpy(P,Q,sizeof Q); if(cur>best){best=cur;memcpy(bP,P,sizeof bP);}} }
      int dup=0; for(int i=0;i<nb;i++) if(!memcmp(bestP[i],bP,sizeof(int)*W)) dup=1;
      if(!dup){ bestR[nb]=best; memcpy(bestP[nb],bP,sizeof bP); nb++; } }
    for(int i=0;i<nb;i++){ Cand*c=&C[NC++]; c->n=182; mkidx(conv,W,bestP[i],c->idx); c->R=bestR[i];
      int o=snprintf(c->name,48,"%c W=%d :",conv?'I':'E',W); for(int k=0;k<W&&o<46;k++) o+=snprintf(c->name+o,48-o," %d",bestP[i][k]+1); } } }

/* ---- pipeline ---- */
typedef struct{ double q; int ci; int letters[MAXN]; int pl[MAXN]; int n; int PL[13][2],PC[13],LAB[25]; int Rmax; } Best;
static int cmpR(const void*a,const void*b){ return ((const Cand*)b)->R-((const Cand*)a)->R; }
static char CELL;
static Best run_cell(const int*S,int stage1only){
  Best B; B.q=-1e30; B.ci=-1; B.Rmax=-1;
  if(CELL=='R') cand_routes(); else if(CELL=='N') cand_nulls(); else if(CELL=='K') cand_K(); else cand_F(S);
  for(int i=0;i<NC;i++){ int x[MAXN]; N=C[i].n; for(int k=0;k<N;k++) x[k]=S[C[i].idx[k]]; if(CELL!='F') C[i].R=stat(x); }
  qsort(C,NC,sizeof(Cand),cmpR); B.Rmax=C[0].R;
  if(stage1only) return B;
  int lim=(CELL=='R'||CELL=='F')?(NC<TOPN?NC:TOPN):NC;
  for(int i=0;i<lim;i++){ int x[MAXN],let[MAXN]; N=C[i].n; for(int k=0;k<N;k++) x[k]=S[C[i].idx[k]];
    double q=solve_pair(x,let);
    if(q>B.q){ B.q=q; B.ci=i; B.n=N; memcpy(B.letters,let,sizeof(int)*N); memcpy(B.pl,x,sizeof(int)*N); memcpy(B.PL,PL_,sizeof PL_); memcpy(B.PC,PC_,sizeof PC_); memcpy(B.LAB,CURLAB,sizeof CURLAB); } }
  return B; }

static void load(const char*p){ FILE*f=fopen(p,"r"); char d[1000]; int n=0,ch; while((ch=fgetc(f))!=EOF) if(ch>='0'&&ch<='9') d[n++]=ch; fclose(f);
  N0=0; for(int i=0;i<196;i++){ if(i%14==13) continue; int a=d[2*i]-'0',b=d[2*i+1]-'0'; S0[N0++]=((a==0)?4:a-6)*5+(b-1);} }

int main(int argc,char**argv){
  setbuf(stdout,NULL); if(argc<6) return 1;
  FILE*fq=fopen(argv[1],"rb"); QG=malloc(4*456976); if(fread(QG,4,456976,fq)!=456976) return 2; fclose(fq);
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  if(getenv("PR_REST"))PR_REST=atoi(getenv("PR_REST")); if(getenv("PR_ITERS"))PR_ITERS=atoi(getenv("PR_ITERS"));
  if(getenv("TOP"))TOPN=atoi(getenv("TOP")); if(getenv("FREST"))FREST=atoi(getenv("FREST")); if(getenv("FITER"))FITER=atoi(getenv("FITER"));
  st3=calloc(15625,4); st4=calloc(390625,4); st5=calloc(9765625,4);
  load(argv[2]); const char*mode=argv[4]; CELL=argv[5][0]; int cnt=argc>6?atoi(argv[6]):1;
  printf("E16 cellule %c mode %s PR_REST=%d PR_ITERS=%d TOP=%d FREST=%d FITER=%d\n",CELL,mode,PR_REST,PR_ITERS,TOPN,FREST,FITER);
  if(!strcmp(mode,"real")){ Best B=run_cell(S0,0); Cand*c=&C[B.ci];
    printf("MEILLEUR qoff=%.3f candidat=%s R=%d (Rmax étage 1=%d)\npaires :",B.q,c->name,c->R,B.Rmax);
    for(int s=0;s<25;s++) if(B.LAB[s]>=0){int q=B.LAB[s]; printf(" %d%d=%c",s/5==4?0:s/5+6,s%5+1,'A'+B.PL[q][0]); if(B.PC[q]==2) printf("/%c",'A'+B.PL[q][1]);}
    printf("\nCLAIR="); for(int i=0;i<B.n;i++) putchar('A'+B.letters[i]); printf("\n");
    /* vérification directe : chaque lettre appartient au symbole lu, et ré-application de la transposition */
    int bad=0; for(int i=0;i<B.n;i++){ int q=B.LAB[B.pl[i]]; if(q<0||(B.PL[q][0]!=B.letters[i]&&B.PL[q][1]!=B.letters[i])) bad++; if(S0[c->idx[i]]!=B.pl[i]) bad++; }
    printf("vérification directe : %d écarts %s\n",bad,bad?"ÉCHEC":"(exact)"); return 0; }
  if(!strcmp(mode,"null")||!strcmp(mode,"s1null")){ int s1=!strcmp(mode,"s1null");
    for(int t=0;t<cnt;t++){ int S[MAXN]; memcpy(S,S0,sizeof(int)*182); for(int i=181;i>0;i--){int j=rnd()%(i+1);int x=S[i];S[i]=S[j];S[j]=x;}
      Best B=run_cell(S,s1); if(s1) printf("NULL1 #%d Rmax=%d\n",t,B.Rmax); else printf("NULL #%d qoff=%.3f candidat=%s Rmax=%d\n",t,B.q,C[B.ci].name,B.Rmax); }
    return 0; }
  if(!strcmp(mode,"control")||!strcmp(mode,"s1ctrl")){ int s1=!strcmp(mode,"s1ctrl");
    FILE*g=fopen(argv[3],"rb"); fseek(g,0,SEEK_END); long CLn=ftell(g); fseek(g,0,SEEK_SET); char*T=malloc(CLn); if(fread(T,1,CLn,g)!=(size_t)CLn) return 3; fclose(g);
    const char*cls="E TZ AQ OX IK NV SB HP RY DG LF CW UM"; int rep[26]; for(int l=0;l<26;l++) rep[l]=l;
    for(const char*q=cls;*q;){ while(*q==' ')q++; if(!*q)break; int a=*q-'A'; q++; while(*q&&*q!=' '){ rep[*q-'A']=a; q++; } }
    int ok=0;
    for(int t=0;t<cnt;t++){
      int L[25],m=0; for(int l=0;l<26;l++) if(l!=9) L[m++]=l; for(int i=24;i>0;i--){int j=rnd()%(i+1);int x=L[i];L[i]=L[j];L[j]=x;} int sy[26]; for(int i=0;i<25;i++) sy[L[i]]=i;
      int S[MAXN], idx[MAXN], n=182; char tname[64]="";
      /* choix du mécanisme (déchiffrement : clair[k] = S[idx[k]]) */
      if(CELL=='R'){ build_routes(14,13); int r=rnd()%NR, dir=rnd()%2; if(dir==0) for(int k=0;k<182;k++) idx[k]=ROUTE[r][k]; else for(int k=0;k<182;k++) idx[ROUTE[r][k]]=k; snprintf(tname,64,"%s/dir%d",RNAME[r],dir); }
      else if(CELL=='N'){ int k=3+rnd()%3, ph=rnd()%k; n=0; for(int i=0;i<182;i++) if(i%k!=ph) idx[n++]=i; snprintf(tname,64,"k=%d phase=%d",k,ph); }
      else if(CELL=='F'){ int W=13+rnd()%2, conv=rnd()%2, P[14]; for(int i=0;i<W;i++)P[i]=i; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int x=P[i];P[i]=P[j];P[j]=x;} mkidx(conv,W,P,idx); snprintf(tname,64,"%c W=%d",conv?'I':'E',W); }
      else { printf("pas de contrôle pour K\n"); return 0; }
      long off=rnd()%(CLn-5000); int txt[MAXN],k2=0; while(k2<n){int ch=T[off++]; if(ch<'A'||ch>'Z')continue; if(ch=='J')ch='I'; txt[k2++]=ch-'A';}
      int used[13],nu=0,seen[25]={0};
      for(int i=0;i<182;i++) S[i]=-1;
      for(int k=0;k<n;k++){ int s=sy[rep[txt[k]]]; S[idx[k]]=s; if(!seen[s]){seen[s]=1; if(nu<13) used[nu++]=s;} }
      for(int i=0;i<182;i++) if(S[i]<0) S[i]=used[rnd()%nu]; /* nulles (cellule N) : tirées parmi les symboles utilisés */
      N=n; int x[MAXN]; for(int k=0;k<n;k++) x[k]=S[idx[k]]; int Rtrue=stat(x);
      Best B=run_cell(S,s1);
      if(s1){ printf("CTRL1 #%d %s R(vrai)=%d Rmax trouvé=%d %s\n",t,tname,Rtrue,B.Rmax,B.Rmax>=Rtrue?"(>= vrai)":""); continue; }
      int rec=0; for(int sh=-16;sh<=16;sh++){ int r=0; for(int i=0;i<B.n;i++){int j=i+sh; if(j>=0&&j<n&&B.letters[i]==txt[j]) r++;} if(r>rec) rec=r; }
      int succ=rec>=0.8*n; ok+=succ;
      printf("CTRL #%d vrai=%s R(vrai)=%d | trouvé=%s qoff=%.3f lues=%d/%d %s\n",t,tname,Rtrue,C[B.ci].name,B.q,rec,n,succ?"OK":"ÉCHEC"); }
    if(!s1) printf("==> %d/%d\n",ok,cnt); return 0; }
  return 1; }
