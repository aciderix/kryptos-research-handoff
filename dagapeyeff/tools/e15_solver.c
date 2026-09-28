/* E15 — hypothèse « 13 classes » : chaque symbole vaut 2 lettres (un symbole : 1 lettre), + transposition colonnaire.
 * Inconnues : affectation lettres -> symboles (12 paires + 1 singleton), clé de transposition (famille E ou I,
 * largeur W). Score : meilleure lecture en lettres (Viterbi, 8 états, quadgrammes joints) du texte détransposé.
 * Recherche : recuit joint (échanges de lettres entre symboles ; échange/insertion/inversion de colonnes).
 *   e15_solver qg.bin cipher.txt control N corpus | null N | real
 * Env : FAM (E|I|N), WMIN, WMAX, RESTARTS, ITERS, SEED
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
static float *QG; static uint64_t rs=88172645463325252ULL;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (rnd()>>11)*(1.0/9007199254740992.0);}
#define N 182
static int SYM[N], NS=13;           /* chiffré (colonnes 1-13), symboles 0..12 */
static char FAM='E'; static int W, RESTARTS=8, ITERS=400000;
static const char AL[]="ABCDEFGHIKLMNOPQRSTUVWXYZ";   /* 25 lettres, I = J */
/* ---- transposition : pos[k] = indice dans le chiffré de la k-ième lettre du clair ---- */
static void build_pos(const int*P,int*pos){ int H=N/W,ex=N%W;
  if(FAM=='N'){ for(int k=0;k<N;k++) pos[k]=k; return; }
  int start[16],off=0; for(int j=0;j<W;j++){ int c=P[j],len=H+(c<ex); start[c]=off; off+=len; }
  if(FAM=='E'){ for(int k=0;k<N;k++){ int r=k/W,c=k%W; pos[k]=start[c]+r; } }
  else { /* I : chiffré écrit par lignes de W, clair = colonnes lues dans l'ordre P */ int k=0; for(int j=0;j<W;j++){ int c=P[j],len=H+(c<ex); for(int r=0;r<len;r++) pos[k++]=r*W+c; } } }
/* ---- Viterbi : lettres possibles par symbole let[s][0..cnt[s]-1] ---- */
static int LET[13][2], CNT[13];
static double viterbi(const int*y,int*best){
  double dp[8],nd[8]; int bp[N][8];
  for(int s=0;s<8;s++) dp[s]=0;
  for(int k=0;k<N;k++){ int c=CNT[y[k]];
    for(int s=0;s<8;s++) nd[s]=-1e30;
    for(int s=0;s<8;s++){ if(dp[s]<-1e29) continue; int b1=(s>>2)&1,b2=(s>>1)&1,b3=s&1; /* choix aux positions k-3,k-2,k-1 */
      for(int b=0;b<c;b++){ int ns=((s<<1)&7)|b; double v=dp[s];
        if(k>=3){ int l0=LET[y[k-3]][b1],l1=LET[y[k-2]][b2],l2=LET[y[k-1]][b3],l3=LET[y[k]][b]; v+=QG[((l0*26+l1)*26+l2)*26+l3]; }
        if(v>nd[ns]){nd[ns]=v; bp[k][ns]=s;} } }
    /* états invalides pour les positions à une seule lettre : déjà exclus (b<c) ; positions k<3 : bits hors portée neutres */
    memcpy(dp,nd,sizeof dp); }
  int bs=0; for(int s=1;s<8;s++) if(dp[s]>dp[bs]) bs=s;
  if(best){ int s=bs; for(int k=N-1;k>=0;k--){ best[k]=LET[y[k]][s&1]; s=bp[k][s]; } }
  return dp[bs]; }
static int L26(int i){ return AL[i]-'A'; }   /* index 0..24 -> lettre 0..25 (QG sur 26) */
static double score_key(const int*P,int*best){ int pos[N],y[N]; build_pos(P,pos); for(int k=0;k<N;k++) y[k]=SYM[pos[k]]; return viterbi(y,best); }
/* affectation initiale : symboles par effectif décroissant <-> classes équilibrées (E seul ; fréquente + rare) */
static const char ORDER_EN[]="ETAOINSHRDLCUMWFGYPBVKXQZ";
static void init_assign(int random_init){
  int cnt[13]={0}; for(int i=0;i<N;i++) cnt[SYM[i]]++; int ord[13]; for(int i=0;i<13;i++) ord[i]=i;
  for(int i=0;i<13;i++)for(int j=i+1;j<13;j++) if(cnt[ord[j]]>cnt[ord[i]]){int t=ord[i];ord[i]=ord[j];ord[j]=t;}
  int L[25]; for(int i=0;i<25;i++) L[i]=ORDER_EN[i]-'A';
  if(random_init){ for(int i=24;i>0;i--){int j=rnd()%(i+1);int t=L[i];L[i]=L[j];L[j]=t;} }
  /* classe 0 : L[0] seul ; classe k : L[k] + L[25-k] */
  CNT[ord[0]]=1; LET[ord[0]][0]=L[0]; LET[ord[0]][1]=L[0];
  for(int k=1;k<13;k++){ CNT[ord[k]]=2; LET[ord[k]][0]=L[k]; LET[ord[k]][1]=L[25-k]; } }
static int BESTP[16]; static int BESTLET[13][2],BESTCNT[13];
static double solve(void){
  double gbest=-1e30;
  for(int R=0;R<RESTARTS;R++){
    init_assign(R>0); int P[16]; for(int i=0;i<W;i++)P[i]=i; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int t=P[i];P[i]=P[j];P[j]=t;}
    double cur=score_key(P,NULL),best=cur; int bP[16],bL[13][2],bC[13]; memcpy(bP,P,sizeof P); memcpy(bL,LET,sizeof LET); memcpy(bC,CNT,sizeof CNT);
    for(int it=0;it<ITERS;it++){ double T=3.0*pow(0.05/3.0,(double)it/ITERS);
      if(FAM=='N'||urand()<0.6){ /* échange de deux lettres entre symboles différents */
        int s1=rnd()%13,s2=rnd()%13; if(s1==s2) continue; int a=rnd()%CNT[s1],b=rnd()%CNT[s2];
        int t=LET[s1][a]; LET[s1][a]=LET[s2][b]; LET[s2][b]=t; if(CNT[s1]==1) LET[s1][1]=LET[s1][0]; if(CNT[s2]==1) LET[s2][1]=LET[s2][0];
        double ns=score_key(P,NULL);
        if(ns>=cur||urand()<exp((ns-cur)/T)) cur=ns; else { t=LET[s1][a]; LET[s1][a]=LET[s2][b]; LET[s2][b]=t; if(CNT[s1]==1) LET[s1][1]=LET[s1][0]; if(CNT[s2]==1) LET[s2][1]=LET[s2][0]; }
      } else { int c1=rnd()%W,c2=rnd()%W; if(c1==c2) continue; if(c1>c2){int t=c1;c1=c2;c2=t;} int old[16]; memcpy(old,P,sizeof P); int op=rnd()%3;
        if(op==0){int t=P[c1];P[c1]=P[c2];P[c2]=t;} else if(op==1){int t=P[c1];for(int c=c1;c<c2;c++)P[c]=P[c+1];P[c2]=t;} else {for(int a=c1,b=c2;a<b;a++,b--){int t=P[a];P[a]=P[b];P[b]=t;}}
        double ns=score_key(P,NULL); if(ns>=cur||urand()<exp((ns-cur)/T)) cur=ns; else memcpy(P,old,sizeof P); }
      if(cur>best){best=cur; memcpy(bP,P,sizeof P); memcpy(bL,LET,sizeof LET); memcpy(bC,CNT,sizeof CNT);} }
    if(best>gbest){gbest=best; memcpy(BESTP,bP,sizeof bP); memcpy(BESTLET,bL,sizeof bL); memcpy(BESTCNT,bC,sizeof bC);} }
  memcpy(LET,BESTLET,sizeof LET); memcpy(CNT,BESTCNT,sizeof CNT);
  return gbest/(N-3); }
static int WMIN=5,WMAX=13;
/* meilleure largeur : balaye W, garde le meilleur score */
static double solve_all(int*bw,int*bestText){ double gb=-1e30; int keepP[16],keepL[13][2],keepC[13],kw=0;
  int w0=(FAM=='N')?1:WMIN, w1=(FAM=='N')?1:WMAX;
  for(W=w0;W<=w1;W++){ double q=solve(); if(q>gb){gb=q; kw=W; memcpy(keepP,BESTP,sizeof keepP); memcpy(keepL,LET,sizeof keepL); memcpy(keepC,CNT,sizeof keepC);} }
  W=kw; memcpy(BESTP,keepP,sizeof keepP); memcpy(LET,keepL,sizeof keepL); memcpy(CNT,keepC,sizeof keepC); *bw=kw; score_key(BESTP,bestText); return gb; }
static void load(const char*p){ FILE*f=fopen(p,"r"); char d[1000]; int n=0,ch; while((ch=fgetc(f))!=EOF) if(ch>='0'&&ch<='9') d[n++]=ch; fclose(f);
  int map[25],ns=0; for(int i=0;i<25;i++) map[i]=-1; int k=0;
  for(int i=0;i<196;i++){ if(i%14==13) continue; int a=d[2*i]-'0',b=d[2*i+1]-'0'; int c=((a==0)?4:a-6)*5+(b-1); if(map[c]<0) map[c]=ns++; SYM[k++]=map[c]; } NS=ns; }
int main(int argc,char**argv){
  setbuf(stdout,NULL); if(argc<4) return 1;
  FILE*f=fopen(argv[1],"rb"); QG=malloc(4*456976); if(fread(QG,4,456976,f)!=456976) return 2; fclose(f);
  if(getenv("FAM"))FAM=getenv("FAM")[0]; if(getenv("WMIN"))WMIN=atoi(getenv("WMIN")); if(getenv("WMAX"))WMAX=atoi(getenv("WMAX"));
  if(getenv("RESTARTS"))RESTARTS=atoi(getenv("RESTARTS")); if(getenv("ITERS"))ITERS=atoi(getenv("ITERS"));
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  load(argv[2]); printf("FAM=%c W=%d..%d R=%d I=%d ; symboles réels (col. 1-13) : %d\n",FAM,WMIN,WMAX,RESTARTS,ITERS,NS);
  int REAL[N]; memcpy(REAL,SYM,sizeof SYM); int bw,txt[N];
  if(!strcmp(argv[3],"real")){ double q=solve_all(&bw,txt); printf("MEILLEUR qoff=%.3f W=%d clé :",q,bw); for(int i=0;i<bw;i++)printf(" %d",BESTP[i]+1);
    printf("\npaires :"); for(int s=0;s<13;s++){ printf(" %c",'A'+LET[s][0]); if(CNT[s]==2) printf("%c",'A'+LET[s][1]); }
    printf("\nLECTURE="); for(int k=0;k<N;k++) putchar('A'+txt[k]); printf("\n"); return 0; }
  if(!strcmp(argv[3],"null")){ int nn=atoi(argv[4]); for(int t=0;t<nn;t++){ memcpy(SYM,REAL,sizeof SYM); for(int i=N-1;i>0;i--){int j=rnd()%(i+1);int x=SYM[i];SYM[i]=SYM[j];SYM[j]=x;}
      double q=solve_all(&bw,txt); printf("NULL #%d qoff=%.3f W=%d\n",t,q,bw); } return 0; }
  if(!strcmp(argv[3],"control")){ int nc=atoi(argv[4]); FILE*g=fopen(argv[5],"rb"); fseek(g,0,SEEK_END); long CL=ftell(g); fseek(g,0,SEEK_SET); char*C=malloc(CL); if(fread(C,1,CL,g)!=(size_t)CL) return 3; fclose(g);
    /* partition équilibrée anglaise fixe (hypothèse), symboles aléatoires */
    const char*cls[13]={"E","TZ","AQ","OX","IK","NV","SB","HP","RY","DG","LF","CW","UM"}; int ok=0;
    for(int t=0;t<nc;t++){ int perm[13]; for(int i=0;i<13;i++)perm[i]=i; for(int i=12;i>0;i--){int j=rnd()%(i+1);int x=perm[i];perm[i]=perm[j];perm[j]=x;}
      int clsof[26]; for(int s=0;s<13;s++) for(const char*q=cls[s];*q;q++) clsof[*q-'A']=perm[s];
      long off=rnd()%(CL-5000); int txt0[N],k=0; while(k<N){int ch=C[off++]; if(ch<'A'||ch>'Z')continue; if(ch=='J')ch='I'; txt0[k++]=ch-'A';}
      int Wt=(FAM=='N')?1:WMIN+rnd()%(WMAX-WMIN+1); W=Wt; int Pt[16]; for(int i=0;i<W;i++)Pt[i]=i; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int x=Pt[i];Pt[i]=Pt[j];Pt[j]=x;}
      int pos[N]; build_pos(Pt,pos); for(int kk=0;kk<N;kk++) SYM[pos[kk]]=clsof[txt0[kk]];
      double q=solve_all(&bw,txt); int acc=0; for(int sh=-16;sh<=16;sh++){ int a=0; for(int kk=0;kk<N;kk++){int j=kk+sh; if(j>=0&&j<N&&txt[kk]==txt0[j]) a++;} if(a>acc) acc=a; }
      printf("CTRL #%d W=%d trouvée W=%d qoff=%.3f lettres justes=%d/%d %s\n",t,Wt,bw,q,acc,N,acc>=0.8*N?"OK":"ÉCHEC"); ok+=(acc>=0.8*N);
      printf("   vrai : "); for(int kk=0;kk<60;kk++) putchar('A'+txt0[kk]); printf("\n   lu   : "); for(int kk=0;kk<60;kk++) putchar('A'+txt[kk]); printf("\n"); }
    printf("==> %d/%d\n",ok,nc); return 0; }
  return 1; }
