/* E04 — balayage EXHAUSTIF de transpositions, statistique INVARIANTE par substitution (aucun carré à résoudre).
 * Statistique : R = rep3 + 2·rep4 + 3·rep5, rep_n = Σ (occurrences − 1) des n-grammes de symboles répétés.
 * Familles (déchiffrement appliqué à la suite S de N symboles) :
 *   E : colonnaire standard (clair écrit par lignes de W, colonnes lues dans l'ordre de la clé ; grille incomplète
 *       permise : les N mod W premières colonnes ont une case de plus) ;
 *   I : inverse (S écrit par lignes de W, colonnes lues dans l'ordre de la clé) ;
 *   D : double colonnaire standard (déchiffre W2 puis W1).
 *   e04_scan cipher.txt real|null|control ...   (voir main)
 * Env : GEO (0 : 196 ; 1 : colonne 14 retirée, 182), FAM (E|I|D), WMIN, WMAX (E/I), W1MAX, W2MAX (D), TOP, SEED
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int N, S0[196];
static uint64_t rs=88172645463325252ULL;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}

static char FAM;
/* ---- statistique invariante ---- */
static uint32_t *st3,*st4,*st5,gen=0; static int A3=1,A4=2,A5=3;
static inline int stat(const int*x){
  gen++; int r3=0,r4=0,r5=0;
  for(int i=0;i+2<N;i++){int k=(x[i]*25+x[i+1])*25+x[i+2]; if(st3[k]==gen)r3++; else st3[k]=gen;
    if(i+3<N){int k4=k*25+x[i+3]; if(st4[k4]==gen)r4++; else st4[k4]=gen;
      if(i+4<N){int k5=k4*25+x[i+4]; if(st5[k5]==gen)r5++; else st5[k5]=gen;}}}
  return A3*r3+A4*r4+A5*r5; }

/* ---- transpositions (déchiffrement) ---- */
/* E : in = colonnes du clair concaténées dans l'ordre de lecture ; P[j] = colonne lue en j-ième */
static void dec_E(const int*in,int*out,int W,const int*P){
  int H=N/W, ex=N%W, off=0;
  for(int j=0;j<W;j++){ int c=P[j], len=H+(c<ex); for(int r=0;r<len;r++) out[r*W+c]=in[off+r]; off+=len; } }
/* I : in écrit par lignes de W ; out = colonnes lues dans l'ordre P */
static void dec_I(const int*in,int*out,int W,const int*P){
  int H=N/W, ex=N%W, k=0;
  for(int j=0;j<W;j++){ int c=P[j], len=H+(c<ex); for(int r=0;r<len;r++) out[k++]=in[r*W+c]; } }
/* chiffrement E (pour les contrôles) : clair par lignes, colonnes lues dans l'ordre P */
static void enc_E(const int*pl,int*out,int W,const int*P){ int k=0,H=N/W,ex=N%W;
  for(int j=0;j<W;j++){int c=P[j],len=H+(c<ex); for(int r=0;r<len;r++) out[k++]=pl[r*W+c];} }
/* chiffrement I : clair réparti en colonnes (ordre P, longueurs de grille incomplète), lu par lignes */
static void enc_I(const int*pl,int*out,int W,const int*P){ int H=N/W,ex=N%W,k=0;
  for(int j=0;j<W;j++){int c=P[j],len=H+(c<ex); for(int r=0;r<len;r++) out[r*W+c]=pl[k++];} }

/* ---- famille P (E06) : lecture VERTICALE des colonnes imprimées (NC = N/14 colonnes), lignes paires (0-indexées)
 * dans l'ordre sE (7!), lignes impaires dans l'ordre sO (7!). PMODE 0 : toutes les colonnes en lignes paires, puis
 * toutes en lignes impaires (généralise la largeur 7 d'E04) ; 1 : colonne par colonne, paires puis impaires ;
 * 2 : colonne par colonne, impaires puis paires. */
static int PMODE=0, PBEST=0;
static void idx_P(int*idx,const int*sE,const int*sO){ int NC=N/14,k=0;
  if(PMODE==0){ for(int b=0;b<2;b++) for(int c=0;c<NC;c++) for(int j=0;j<7;j++) idx[k++]=(2*(b?sO[j]:sE[j])+b)*NC+c; }
  else for(int c=0;c<NC;c++) for(int q=0;q<2;q++){ int b=(PMODE==1)?q:1-q; for(int j=0;j<7;j++) idx[k++]=(2*(b?sO[j]:sE[j])+b)*NC+c; } }
static void dec_P(const int*in,int*out,const int*sE,const int*sO){ int idx[196]; idx_P(idx,sE,sO); for(int k=0;k<N;k++) out[k]=in[idx[k]]; }
static void enc_P(const int*pl,int*out,const int*sE,const int*sO){ int idx[196]; idx_P(idx,sE,sO); for(int k=0;k<N;k++) out[idx[k]]=pl[k]; }
/* ---- énumération des permutations (algorithme de Heap, itératif) ---- */
typedef struct{int s; int W; int P[16]; int P2[16]; int W2;} Hit;
static int TOPN=10; static Hit *top; static int ntop=0; static int THR=1<<30; static long CNTABOVE=0;
static void push(int s,int W,const int*P,int W2,const int*P2){
  if(s>THR) CNTABOVE++;
  if(ntop<TOPN||s>top[ntop-1].s){ int i=ntop<TOPN?ntop++:TOPN-1; while(i>0&&top[i-1].s<s){top[i]=top[i-1];i--;}
    top[i].s=s; top[i].W=W; memcpy(top[i].P,P,sizeof(int)*W); top[i].W2=W2; if(W2) memcpy(top[i].P2,P2,sizeof(int)*W2); } }

static int scan_single(const int*S,char fam,int W,int record){
  int P[16],c[16]={0},buf[196],best=-1; for(int i=0;i<W;i++)P[i]=i;
  #define EVAL { if(fam=='E') dec_E(S,buf,W,P); else dec_I(S,buf,W,P); int s=stat(buf); if(s>best)best=s; if(record) push(s,W,P,0,NULL); }
  EVAL; int i=1;
  while(i<W){ if(c[i]<i){ if(i&1){int t=P[c[i]];P[c[i]]=P[i];P[i]=t;} else {int t=P[0];P[0]=P[i];P[i]=t;} EVAL; c[i]++; i=1; } else { c[i]=0; i++; } }
  #undef EVAL
  return best; }

/* double : pour chaque clé W2 (déchiffrée d'abord), balayage complet de W1 */
static int scan_double(const int*S,int W1,int W2,int record){
  int Q[16],cq[16]={0},mid[196],best=-1; for(int i=0;i<W2;i++)Q[i]=i;
  #define OUTER { dec_E(S,mid,W2,Q); int P[16],c[16]={0},buf[196]; for(int k=0;k<W1;k++)P[k]=k; \
      { dec_E(mid,buf,W1,P); int s=stat(buf); if(s>best)best=s; if(record)push(s,W1,P,W2,Q);} int i=1; \
      while(i<W1){ if(c[i]<i){ if(i&1){int t=P[c[i]];P[c[i]]=P[i];P[i]=t;} else {int t=P[0];P[0]=P[i];P[i]=t;} \
        dec_E(mid,buf,W1,P); int s=stat(buf); if(s>best)best=s; if(record)push(s,W1,P,W2,Q); c[i]++; i=1; } else { c[i]=0; i++; } } }
  OUTER; int i=1;
  while(i<W2){ if(cq[i]<i){ if(i&1){int t=Q[cq[i]];Q[cq[i]]=Q[i];Q[i]=t;} else {int t=Q[0];Q[0]=Q[i];Q[i]=t;} OUTER; cq[i]++; i=1; } else { cq[i]=0; i++; } }
  #undef OUTER
  return best; }

/* ---- étage 2 : carré résolu à clé fixée (recuit quadgrammes joints) ---- */
static float *QG; static int ITER2=15000, REST2=3; static double T0S=2.0,T1S=0.05;
static const char EFREQ[]="ETAOINSRHLDCUMFPGWYBVKXQJZ";
static double qsc(const int*pl,const int*m){ double s=0; for(int k=0;k+3<N;k++) s+=QG[((m[pl[k]]*26+m[pl[k+1]])*26+m[pl[k+2]])*26+m[pl[k+3]]]; return s; }
static double solve_sub(const int*pl,int*best){
  int cnt[25]={0}; for(int i=0;i<N;i++) cnt[pl[i]]++; int ord[25]; for(int i=0;i<25;i++)ord[i]=i;
  for(int i=0;i<25;i++)for(int j=i+1;j<25;j++) if(cnt[ord[j]]>cnt[ord[i]]){int t=ord[i];ord[i]=ord[j];ord[j]=t;}
  /* positions de chaque symbole -> fenêtres quadgrammes touchées (score incrémental) */
  static int posl[25][196]; int npos[25]={0}; for(int i=0;i<N;i++) posl[pl[i]][npos[pl[i]]++]=i;
  int pres[25],np=0; for(int x=0;x<25;x++) if(npos[x]) pres[np++]=x;
  static float wv[196]; static int stamp[196]; static int sc=0;
  double gb=-1e30;
  for(int R=0;R<REST2;R++){ int m[25],used[26]; int lt[26]; for(int i=0;i<26;i++) lt[i]=EFREQ[i]-'A';
    if(R>0) for(int i=25;i>0;i--){int j=rnd()%(i+1);int t=lt[i];lt[i]=lt[j];lt[j]=t;} /* départs indépendants */
    for(int l=0;l<26;l++)used[l]=-1; for(int i=0;i<25;i++){m[ord[i]]=lt[i]; used[lt[i]]=ord[i];}
    double cur=0; for(int k=0;k+3<N;k++){ wv[k]=QG[((m[pl[k]]*26+m[pl[k+1]])*26+m[pl[k+2]])*26+m[pl[k+3]]]; cur+=wv[k]; }
    double b=cur; int bm[25]; memcpy(bm,m,sizeof bm);
    for(int it=0;it<ITER2;it++){ double T=T0S*__builtin_pow(T1S/T0S,(double)it/ITER2);
      int a=pres[rnd()%np]; int nl=rnd()%26; int bb=used[nl]; if(bb==a) continue; int la=m[a];
      m[a]=nl; if(bb>=0)m[bb]=la;
      int wins[800],nw=0; sc++; for(int q=0;q<2;q++){ int x=q?bb:a; if(x<0) continue; for(int t=0;t<npos[x];t++){ int p0=posl[x][t];
        for(int w=p0-3;w<=p0;w++) if(w>=0&&w+3<N&&stamp[w]!=sc){stamp[w]=sc;wins[nw++]=w;} } }
      float nv[800]; double d=0; for(int w=0;w<nw;w++){int k=wins[w]; nv[w]=QG[((m[pl[k]]*26+m[pl[k+1]])*26+m[pl[k+2]])*26+m[pl[k+3]]]; d+=nv[w]-wv[k];}
      if(d>=0||(double)(rnd()>>11)*(1.0/9007199254740992.0)<__builtin_exp(d/T)){ for(int w=0;w<nw;w++) wv[wins[w]]=nv[w]; cur+=d; used[nl]=a; used[la]=bb; if(cur>b){b=cur;memcpy(bm,m,sizeof bm);} }
      else {m[a]=la; if(bb>=0)m[bb]=nl;} }
    if(getenv("CHK")){ double full=0; for(int k=0;k+3<N;k++) full+=QG[((m[pl[k]]*26+m[pl[k+1]])*26+m[pl[k+2]])*26+m[pl[k+3]]]; double fb=0; for(int k=0;k+3<N;k++) fb+=QG[((bm[pl[k]]*26+bm[pl[k+1]])*26+bm[pl[k+2]])*26+bm[pl[k+3]]]; fprintf(stderr,"  cur suivi=%.2f recalculé=%.2f | best suivi=%.2f recalculé=%.2f\n",cur,full,b,fb); }
    if(b>gb){gb=b;memcpy(best,bm,sizeof bm);} }
  return gb/(N-3); }
static void decode(const int*S,const Hit*h,int*out){ int mid[196]; if(FAM=='P'){ dec_P(S,out,h->P,h->P2); return; } if(h->W2){dec_E(S,mid,h->W2,h->P2); dec_E(mid,out,h->W,h->P);} else if(FAM=='E') dec_E(S,out,h->W,h->P); else dec_I(S,out,h->W,h->P); }
/* ---- étage 2 « paires » (E15) : 13 symboles, chacun = 2 lettres (un seul = 1 lettre) ; lecture par Viterbi ---- */
static int PAIRMODE=0, PR_REST=8, PR_ITERS=200000;
static int PL_[13][2], PC_[13]; static int BPL[13][2], BPC[13], BLAB[25], CURLAB[25];
static double pviterbi(const int*y,int n,int*best){
  double dp[8],nd[8]; static int bp[196][8];
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
  int lab[25]; for(int i=0;i<25;i++) lab[i]=-1; int y[196],k=0,cnt[13]={0};
  for(int i=0;i<N;i++){ if(lab[pl[i]]<0){ if(k>=13) return -1e30; lab[pl[i]]=k++; } y[i]=lab[pl[i]]; cnt[y[i]]++; }
  if(k!=13) return -1e30; memcpy(CURLAB,lab,sizeof lab);
  int ord[13]; for(int i=0;i<13;i++) ord[i]=i; for(int i=0;i<13;i++)for(int j=i+1;j<13;j++) if(cnt[ord[j]]>cnt[ord[i]]){int t=ord[i];ord[i]=ord[j];ord[j]=t;}
  static const char OE[]="ETAOINSHRDLCUMWFGYPBVKXQZ"; double gb=-1e30; int bL[13][2],bC[13];
  for(int R=0;R<PR_REST;R++){ int L[25]; for(int i=0;i<25;i++) L[i]=OE[i]-'A';
    if(R>0) for(int i=24;i>0;i--){int j=rnd()%(i+1);int t=L[i];L[i]=L[j];L[j]=t;}
    PC_[ord[0]]=1; PL_[ord[0]][0]=PL_[ord[0]][1]=L[0]; for(int q=1;q<13;q++){ PC_[ord[q]]=2; PL_[ord[q]][0]=L[q]; PL_[ord[q]][1]=L[25-q]; }
    double cur=pviterbi(y,N,NULL),best=cur; int cL[13][2],cC[13]; memcpy(cL,PL_,sizeof cL); memcpy(cC,PC_,sizeof cC);
    for(int it=0;it<PR_ITERS;it++){ double T=3.0*__builtin_pow(0.05/3.0,(double)it/PR_ITERS); double u=(double)(rnd()>>11)*(1.0/9007199254740992.0);
      int s1=rnd()%13,s2=rnd()%13; if(s1==s2) continue;
      if(u<0.3){ if(PC_[s1]!=PC_[s2]) continue; int t0=PL_[s1][0],t1=PL_[s1][1]; PL_[s1][0]=PL_[s2][0];PL_[s1][1]=PL_[s2][1];PL_[s2][0]=t0;PL_[s2][1]=t1;
        double ns=pviterbi(y,N,NULL); if(ns>=cur||(double)(rnd()>>11)*(1.0/9007199254740992.0)<__builtin_exp((ns-cur)/T)) cur=ns; else { t0=PL_[s1][0];t1=PL_[s1][1]; PL_[s1][0]=PL_[s2][0];PL_[s1][1]=PL_[s2][1];PL_[s2][0]=t0;PL_[s2][1]=t1; } }
      else { int a=rnd()%PC_[s1],b=rnd()%PC_[s2]; int t=PL_[s1][a]; PL_[s1][a]=PL_[s2][b]; PL_[s2][b]=t; if(PC_[s1]==1)PL_[s1][1]=PL_[s1][0]; if(PC_[s2]==1)PL_[s2][1]=PL_[s2][0];
        double ns=pviterbi(y,N,NULL); if(ns>=cur||(double)(rnd()>>11)*(1.0/9007199254740992.0)<__builtin_exp((ns-cur)/T)) cur=ns;
        else { t=PL_[s1][a]; PL_[s1][a]=PL_[s2][b]; PL_[s2][b]=t; if(PC_[s1]==1)PL_[s1][1]=PL_[s1][0]; if(PC_[s2]==1)PL_[s2][1]=PL_[s2][0]; } }
      if(cur>best){best=cur; memcpy(cL,PL_,sizeof cL); memcpy(cC,PC_,sizeof cC);} }
    if(best>gb){gb=best; memcpy(bL,cL,sizeof bL); memcpy(bC,cC,sizeof bC);} }
  memcpy(PL_,bL,sizeof bL); memcpy(PC_,bC,sizeof bC); if(letters) pviterbi(y,N,letters); return gb/(N-3); }
typedef struct{double q; Hit h; int map[25]; int pl[196]; int bpl[13][2], bpc[13], blab[25];} Final;
/* étage 2 sur la liste courante top[0..ntop) ; met à jour *fb */
static void stage2(const int*S,Final*fb){
  if(getenv("STAGE1ONLY")) return;
  for(int i=0;i<ntop;i++){ int pl[196],m[25]; decode(S,&top[i],pl); double q; if(PAIRMODE){ int let[196]; q=solve_pair(pl,let); memcpy(pl,let,sizeof let); for(int z=0;z<25;z++) m[z]=z; } else q=solve_sub(pl,m);
    if(getenv("VERB")&&i<2) printf("    top%d R=%d stat(décodé)=%d q=%.3f\n",i,top[i].s,stat(pl),q);
    if(q>fb->q){ fb->q=q; fb->h=top[i]; memcpy(fb->map,m,sizeof m); memcpy(fb->pl,pl,sizeof pl); if(PAIRMODE){ memcpy(fb->bpl,PL_,sizeof fb->bpl); memcpy(fb->bpc,PC_,sizeof fb->bpc); memcpy(fb->blab,CURLAB,sizeof fb->blab);} } } }
static int scan_P(const int*S,int record){
  int sE[7],cE[7]={0},best=-1,buf[196]; for(int i=0;i<7;i++)sE[i]=i;
  #define INNER { int sO[7],cO[7]={0}; for(int k=0;k<7;k++)sO[k]=k; \
    { dec_P(S,buf,sE,sO); int s=stat(buf); if(s>best)best=s; if(record)push(s,7,sE,7,sO);} int i2=1; \
    while(i2<7){ if(cO[i2]<i2){ if(i2&1){int t=sO[cO[i2]];sO[cO[i2]]=sO[i2];sO[i2]=t;} else {int t=sO[0];sO[0]=sO[i2];sO[i2]=t;} \
      dec_P(S,buf,sE,sO); int s=stat(buf); if(s>best)best=s; if(record)push(s,7,sE,7,sO); cO[i2]++; i2=1; } else { cO[i2]=0; i2++; } } }
  INNER; int i=1;
  while(i<7){ if(cE[i]<i){ if(i&1){int t=sE[cE[i]];sE[cE[i]]=sE[i];sE[i]=t;} else {int t=sE[0];sE[0]=sE[i];sE[i]=t;} INNER; cE[i]++; i=1; } else { cE[i]=0; i++; } }
  #undef INNER
  return best; }
static void load(const char*p,int geo){ FILE*f=fopen(p,"r"); char d[1000]; int n=0,ch; while((ch=fgetc(f))!=EOF) if(ch>='0'&&ch<='9') d[n++]=ch; fclose(f);
  N=0; for(int i=0;i<196;i++){ if(geo==1&&i%14==13) continue; int a=d[2*i]-'0',b=d[2*i+1]-'0'; S0[N++]=((a==0)?4:a-6)*5+(b-1);} }

/* balayage d'une famille complète ; renvoie le max par largeur dans mx[] */
static char FAM='E'; static int WMIN=2,WMAX=9,W1MAX=6,W2MAX=6;
/* pipeline complet sur une suite S : pour chaque largeur (ou couple), étage 1 (TOP clés par R) puis étage 2 ;
 * renvoie le meilleur global ; bw[] reçoit le meilleur qoff par largeur */
static Final pipeline(const int*S,double*bw,int verbose){
  Final fb; fb.q=-1e30; fb.h.s=-1; int mx; int s1=getenv("STAGE1ONLY")!=NULL;
  #define KEEP1 if(s1&&ntop>0&&top[0].s>fb.h.s) fb.h=top[0];
  if(FAM=='P'){ int m0=PMODE; for(int pm=(getenv("PMODE")?m0:0);pm<=(getenv("PMODE")?m0:2);pm++){ PMODE=pm; ntop=0; mx=scan_P(S,1); KEEP1 Final f; f.q=-1e30; stage2(S,&f); f.h.W2=7;
      bw[pm]=f.q; if(verbose) printf("  mode P%d : Rmax=%d ; meilleur qoff=%.3f\n",pm,mx,f.q); if(f.q>fb.q){fb=f; fb.h.s=fb.h.s; PBEST=pm;} } PMODE=PBEST; }
  else if(FAM=='D'){ for(int a=2;a<=W1MAX;a++) for(int b=2;b<=W2MAX;b++){ ntop=0; mx=scan_double(S,a,b,1); KEEP1 Final f; f.q=-1e30; stage2(S,&f);
      bw[a*16+b]=f.q; if(verbose) printf("  W1=%d W2=%d : Rmax=%d ; meilleur qoff=%.3f\n",a,b,mx,f.q); if(f.q>fb.q) fb=f; } }
  else for(int w=WMIN;w<=WMAX;w++){ ntop=0; mx=scan_single(S,FAM,w,1); KEEP1 Final f; f.q=-1e30; stage2(S,&f);
      bw[w]=f.q; if(verbose) printf("  W=%d : Rmax=%d ; meilleur qoff=%.3f\n",w,mx,f.q); if(f.q>fb.q) fb=f; }
  return fb; }
static void print_final(const Final*f){
  if(FAM=='P') printf("mode P%d ; ",PMODE);
  if(PAIRMODE){ memcpy(BPL,f->bpl,sizeof BPL); memcpy(BPC,f->bpc,sizeof BPC); memcpy(BLAB,f->blab,sizeof BLAB); printf("paires (symbole = lettres) :"); for(int c=0;c<25;c++) if(BLAB[c]>=0){ int q=BLAB[c]; printf(" %d%d=%c%s",c/5==4?0:c/5+6,c%5+1,'A'+BPL[q][0],BPC[q]==2?(char[]){'/', (char)('A'+BPL[q][1]),0}:""); } printf("\n"); }
  printf("MEILLEUR qoff=%.3f W=%d P=",f->q,f->h.W); for(int k=0;k<f->h.W;k++)printf("%d ",f->h.P[k]+1);
  if(f->h.W2){printf("| W2=%d Q=",f->h.W2); for(int k=0;k<f->h.W2;k++)printf("%d ",f->h.P2[k]+1);} printf(" (R=%d)\nCLAIR=",f->h.s);
  for(int i=0;i<N;i++) putchar('A'+(PAIRMODE?f->pl[i]:f->map[f->pl[i]])); printf("\n"); }

int main(int argc,char**argv){
  setbuf(stdout,NULL); if(argc<4) return 1;
  FILE*fq=fopen(argv[1],"rb"); QG=malloc(4*456976); if(fread(QG,4,456976,fq)!=456976) return 2; fclose(fq);
  int geo=getenv("GEO")?atoi(getenv("GEO")):1; if(getenv("FAM"))FAM=getenv("FAM")[0];
  if(getenv("WMIN"))WMIN=atoi(getenv("WMIN")); if(getenv("WMAX"))WMAX=atoi(getenv("WMAX"));
  if(getenv("W1MAX"))W1MAX=atoi(getenv("W1MAX")); if(getenv("W2MAX"))W2MAX=atoi(getenv("W2MAX")); if(getenv("TOP"))TOPN=atoi(getenv("TOP"));
  if(getenv("A3"))A3=atoi(getenv("A3")); if(getenv("A4"))A4=atoi(getenv("A4")); if(getenv("A5"))A5=atoi(getenv("A5"));
  if(getenv("T0S"))T0S=atof(getenv("T0S")); if(getenv("T1S"))T1S=atof(getenv("T1S"));
  if(getenv("PMODE"))PMODE=atoi(getenv("PMODE")); if(getenv("PAIR"))PAIRMODE=atoi(getenv("PAIR")); if(getenv("PR_REST"))PR_REST=atoi(getenv("PR_REST")); if(getenv("PR_ITERS"))PR_ITERS=atoi(getenv("PR_ITERS"));
  if(getenv("ITER2"))ITER2=atoi(getenv("ITER2")); if(getenv("REST2"))REST2=atoi(getenv("REST2"));
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  st3=calloc(15625,4); st4=calloc(390625,4); st5=calloc(9765625,4); top=calloc(TOPN,sizeof(Hit));
  load(argv[2],geo);
  printf("GEO=%d N=%d FAM=%c W=%d..%d (D: W1<=%d W2<=%d) TOP=%d ITER2=%d REST2=%d A=%d,%d,%d\n",geo,N,FAM,WMIN,WMAX,W1MAX,W2MAX,TOPN,ITER2,REST2,A3,A4,A5);
  double bw[256];
  if(!strcmp(argv[3],"real")&&PAIRMODE){ Final f=pipeline(S0,bw,1); print_final(&f);
    int dec[196],mid[196]; if(FAM=='D'){dec_E(S0,mid,f.h.W2,f.h.P2); dec_E(mid,dec,f.h.W,f.h.P);} else decode(S0,&f.h,dec);
    int bad=0; for(int i=0;i<N;i++){ int q=BLAB[dec[i]]; if(q<0||(BPL[q][0]!=f.pl[i]&&BPL[q][1]!=f.pl[i])) bad++; }
    printf("vérification directe (chaque lettre lue appartient au symbole observé) : %d/%d écarts %s\n",bad,N,bad?"ÉCHEC":"(exact)"); return 0; }
  if(!strcmp(argv[3],"real")){ Final f=pipeline(S0,bw,1); print_final(&f);
    { /* vérification directe : ré-enchiffrement */
      int inv[26]; for(int l=0;l<26;l++)inv[l]=-1; int pres[25]={0},coll=0; for(int i=0;i<N;i++)pres[S0[i]]=1;
      for(int x=0;x<25;x++) if(pres[x]){ if(inv[f.map[x]]>=0)coll++; inv[f.map[x]]=x; }
      int pl[196],ct[196],mid[196]; for(int i=0;i<N;i++) pl[i]=inv[f.map[f.pl[i]]];
      if(FAM=='P'){ enc_P(pl,ct,f.h.P,f.h.P2); (void)mid; } else if(f.h.W2){ enc_E(pl,mid,f.h.W,f.h.P); enc_E(mid,ct,f.h.W2,f.h.P2);} else if(FAM=='E') enc_E(pl,ct,f.h.W,f.h.P); else enc_I(pl,ct,f.h.W,f.h.P);
      int bad=0; for(int i=0;i<N;i++) if(ct[i]!=S0[i]) bad++;
      printf("ré-enchiffrement : %d/%d symboles différents, collisions=%d %s\n",bad,N,coll,(bad||coll)?"ÉCHEC":"(exact)"); }
    return 0; }
  if(!strcmp(argv[3],"null")){ int nn=atoi(argv[4]);
    int colnull=getenv("NULLKIND")&&!strcmp(getenv("NULLKIND"),"col"); /* col : mélange à l'intérieur de chaque colonne imprimée */
    for(int t=0;t<nn;t++){ int S[196]; memcpy(S,S0,sizeof(int)*N);
      if(colnull){ int NC=N/14; for(int c=0;c<NC;c++) for(int r=13;r>0;r--){int q=rnd()%(r+1);int x=S[r*NC+c];S[r*NC+c]=S[q*NC+c];S[q*NC+c]=x;} }
      else for(int i=N-1;i>0;i--){int j=rnd()%(i+1);int x=S[i];S[i]=S[j];S[j]=x;}
      Final f=pipeline(S,bw,1); printf("NULL #%d qoff=%.3f (W=%d W2=%d)\n",t,f.q,f.h.W,f.h.W2); }
    return 0; }
  if(!strcmp(argv[3],"control")){
    int nc=atoi(argv[4]); FILE*g=fopen(argv[5],"rb"); fseek(g,0,SEEK_END); long CL=ftell(g); fseek(g,0,SEEK_SET); char*C=malloc(CL); if(fread(C,1,CL,g)!=(size_t)CL) return 3; fclose(g);
    int ok=0;
    for(int t=0;t<nc;t++){ long off=rnd()%(CL-5000); int txt[196],k=0; while(k<N){int ch=C[off++]; if(ch<'A'||ch>'Z')continue; if(ch=='J')ch='I'; txt[k++]=ch-'A';}
      int L[25],m=0; for(int l=0;l<26;l++) if(l!=9) L[m++]=l; for(int i=24;i>0;i--){int j=rnd()%(i+1);int x=L[i];L[i]=L[j];L[j]=x;} int sy[26]; for(int i=0;i<25;i++) sy[L[i]]=i;
      int txtorig[196]; memcpy(txtorig,txt,sizeof txtorig);
      if(getenv("CLS13")){ /* E14 : fusion en 13 classes équilibrées (anglais), classe -> symbole de son représentant */
        const char*cls="E TZ AQ OX IK NV SB HP RY DG LF CW UM"; int rep[26]; for(int l=0;l<26;l++) rep[l]=l;
        for(const char*q=cls;*q;){ while(*q==' ')q++; if(!*q)break; int a=*q-'A'; q++; while(*q&&*q!=' '){ rep[*q-'A']=a; q++; } }
        for(int i=0;i<N;i++) txt[i]=rep[txt[i]]; }
      int pl[196]; for(int i=0;i<N;i++) pl[i]=sy[txt[i]];
      int S[196],W=0,W2=0,P[16],Q[16];
      if(FAM=='P'){ W=7; W2=7; if(!getenv("PMODE")) PMODE=rnd()%3; for(int i=0;i<7;i++){P[i]=i;Q[i]=i;} for(int i=6;i>0;i--){int j=rnd()%(i+1);int x=P[i];P[i]=P[j];P[j]=x; j=rnd()%(i+1); x=Q[i];Q[i]=Q[j];Q[j]=x;}
        enc_P(pl,S,P,Q); }
      else if(FAM=='D'){ W=2+rnd()%(W1MAX-1); W2=2+rnd()%(W2MAX-1); for(int i=0;i<W;i++)P[i]=i; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int x=P[i];P[i]=P[j];P[j]=x;}
        for(int i=0;i<W2;i++)Q[i]=i; for(int i=W2-1;i>0;i--){int j=rnd()%(i+1);int x=Q[i];Q[i]=Q[j];Q[j]=x;} int mid[196]; enc_E(pl,mid,W,P); enc_E(mid,S,W2,Q); }
      else { W=WMIN+rnd()%(WMAX-WMIN+1); for(int i=0;i<W;i++)P[i]=i; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int x=P[i];P[i]=P[j];P[j]=x;}
        if(FAM=='E') enc_E(pl,S,W,P); else enc_I(pl,S,W,P); }
      int strue=stat(pl); THR=strue; CNTABOVE=0;
      if(getenv("DIAG2")){ int mm[25]; for(int z=0;z<atoi(getenv("DIAG2"));z++){ double q0=solve_sub(pl,mm); printf("   appel %d : %.3f\n",z,q0);} double qq=solve_sub(pl,mm); int rr=0; for(int i=0;i<N;i++) if(mm[pl[i]]==txt[i]) rr++; printf("   [diag] étage 2 sur le vrai clair : qoff=%.3f récupéré=%d/%d\n",qq,rr,N); continue; }
      if(getenv("STAGE1ONLY")){ Final f1=pipeline(S,bw,0); printf("CTRL #%d W=%d W2=%d R(vrai)=%d R(meilleur)=%d (W=%d) rang vrai=%ld\n",t,W,W2,strue,f1.h.s,f1.h.W,CNTABOVE+1); continue; }
      Final f=pipeline(S,bw,getenv("VERB")!=NULL); int rec=0; for(int sh=-16;sh<=16;sh++){ int r=0; for(int i=0;i<N;i++){int j=i+sh; int lt=PAIRMODE?f.pl[i]:f.map[f.pl[i]]; if(j>=0&&j<N&&lt==(PAIRMODE?txtorig[j]:txt[j])) r++;} if(r>rec) rec=r; } /* tolère un décalage (clé tournée) */
      int mt[25]={0}; for(int i=0;i<N;i++) mt[pl[i]]=txt[i]; double qt=qsc(pl,mt)/(N-3);
      int sq=0; for(int i=0;i<N;i++) if(f.map[f.pl[i]]==L[f.pl[i]]) sq++; /* carré retrouvé (indépendant de l'ordre) */
      int succ=(rec>=0.9*N)||(FAM=='I'&&sq>=0.9*N);
      printf("CTRL #%d W=%d W2=%d R(vrai)=%d  qoff=%.3f (vrai %.3f)  récupéré=%d/%d carré=%d/%d %s\n",t,W,W2,strue,f.q,qt,rec,N,sq,N,succ?"OK":"ÉCHEC"); ok+=succ; }
    printf("==> %d/%d\n",ok,nc); return 0; }
  return 1; }
