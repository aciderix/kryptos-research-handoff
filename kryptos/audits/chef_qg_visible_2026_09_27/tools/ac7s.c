/* ac7s.c — 2-alphabets, autoclé écart-7, recherche STRUCTURÉE par équations de cribs.
 *
 * Levier : aux positions double-crib (p_i ET p_{i-7} connus), tau[c_i]=sig[p_i]+sig[p_{i-7}].
 * K4 : 8 lettres de tau sont DÉRIVÉES de sigma, + 2 contraintes de cohérence sur sigma.
 *   R:(T,E) N:(H,A) G:(E,S) K:(A,T) S:(S,N) Z:(L,B) F:(O,E) P:(C,R)
 *   cohérence: sig[S]+sig[N]==sig[T]+sig[O] ; sig[A]+sig[T]==sig[K]+sig[I]
 * => 10 cribs AUTO-satisfaites, tau réduit à 18 libres. Recuit sig(perm)+tau_libre(18)+kappa(7),
 *    score = qg_big + Wc*cribs - Pc*incohérence - Pt*(26-distinct(tau)).
 * Objectif validé (qg_big) ; ici on aide la NAVIGATION par la structure.
 *
 * Compile: cc -O2 -o ac7s ac7s.c -lm
 * Usage:   ac7s attack qg.bin CT iters R seed [Wc] [truePT]
 *          ac7s gen SEED < clair(97)   (contrôle positif 2-alph Vig autoclé écart-7)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define N 97
#define LAG 7
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N],ncrib;
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";ncrib=0;
 for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';ncrib++;}
 for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';ncrib++;}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static uint64_t rs;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (rnd()>>11)*(1.0/9007199254740992.0);}
static void shuffle(int*s,int n){for(int i=0;i<n;i++)s[i]=i;for(int i=n-1;i>0;i--){int j=rnd()%(i+1);int t=s[i];s[i]=s[j];s[j]=t;}}
static float*QG;
static int CT[N];

/* lettres de tau dérivées de sigma : pin[k]={ctletter, A, B} => tau[ct]=sig[A]+sig[B] */
static int PIN[8][3]; static int npin=8;
static void initpin(void){
  int k=0;
  #define ADD(c,a,b) {PIN[k][0]=c-'A';PIN[k][1]=a-'A';PIN[k][2]=b-'A';k++;}
  ADD('R','T','E') ADD('N','H','A') ADD('G','E','S') ADD('K','A','T')
  ADD('S','S','N') ADD('Z','L','B') ADD('F','O','E') ADD('P','C','R')
  #undef ADD
}
/* est-ce une lettre pinée ? */
static int ispin[26];
static void initispin(void){for(int i=0;i<26;i++)ispin[i]=0; for(int k=0;k<npin;k++)ispin[PIN[k][0]]=1;}

/* construit tau (26) : 8 dérivées de sig, 18 depuis freetau[] ; retourne #collisions(26-distinct) */
static int buildtau(const int*sig,const int*freetau,int*tau){
  for(int k=0;k<npin;k++){int c=PIN[k][0]; tau[c]=md(sig[PIN[k][1]]+sig[PIN[k][2]]);}
  int fi=0; for(int c=0;c<26;c++) if(!ispin[c]) tau[c]=freetau[fi++];
  int seen[26]={0},dis=0; for(int c=0;c<26;c++){if(!seen[tau[c]]){seen[tau[c]]=1;dis++;}}
  return 26-dis;
}
/* cohérence sigma (2 contraintes) : #violations */
static int consist(const int*sig){
  int v=0;
  if(md(sig['S'-'A']+sig['N'-'A'])!=md(sig['T'-'A']+sig['O'-'A'])) v++;
  if(md(sig['A'-'A']+sig['T'-'A'])!=md(sig['K'-'A']+sig['I'-'A'])) v++;
  return v;
}
static void decrypt(const int*sig,const int*siginv,const int*tau,const int*kappa,int*pt){
  int x[N];
  for(int i=0;i<N;i++){ int t=tau[CT[i]]; x[i]=md(t-((i<LAG)?kappa[i]:x[i-LAG])); pt[i]=siginv[x[i]]; }
}
static double qg_of(const int*pt){double s=0;for(int i=0;i+3<N;i++)s+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];return s;}

int main(int argc,char**argv){
  setcribs(); initpin(); initispin();
  if(argc>=3&&!strcmp(argv[1],"gen")){
    rs=0x9E3779B97F4A7C15ULL*(atol(argv[2])+1);
    char buf[256]; if(!fgets(buf,sizeof buf,stdin))return 1; int pt[N],n=0;
    for(char*c=buf;*c&&n<N;c++){int u=*c;if(u>='a'&&u<='z')u-=32;if(u>='A'&&u<='Z')pt[n++]=u-'A';}
    if(n<N){fprintf(stderr,"short %d\n",n);return 1;}
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
    int sig[26],taui[26],tau[26],kap[LAG]; shuffle(sig,26); shuffle(tau,26); for(int i=0;i<26;i++)taui[tau[i]]=i;
    for(int i=0;i<LAG;i++)kap[i]=rnd()%26;
    int x[N],ct[N]; for(int i=0;i<N;i++){int k=(i<LAG)?kap[i]:x[i-LAG]; x[i]=sig[pt[i]]; ct[i]=taui[md(x[i]+k)];}
    for(int i=0;i<N;i++)putchar('A'+ct[i]); printf(" PT="); for(int i=0;i<N;i++)putchar('A'+pt[i]); putchar('\n'); return 0;
  }
  if(argc>=6&&!strcmp(argv[1],"attack")){
    FILE*f=fopen(argv[2],"rb");QG=malloc(sizeof(float)*456976);if(fread(QG,sizeof(float),456976,f)!=456976)return 2;fclose(f);
    const char*cts=argv[3];for(int i=0;i<N;i++)CT[i]=cts[i]-'A';
    long IT=atol(argv[4]);int R=atoi(argv[5]);rs=0x9E3779B97F4A7C15ULL*(atol(argv[6])+1);
    double Wc=argc>7?atof(argv[7]):8.0;
    int truePT[N],hasT=0; if(argc>8){const char*tp=argv[8];for(int i=0;i<N;i++)truePT[i]=tp[i]-'A';hasT=1;}
    double Pc=30.0, Pt=12.0, T0=6.0,T1=0.1;
    int nfree=26-npin;
    int bestsig[26],bestft[26],bestk[LAG];double best=-1e18;int bestm=0;
    for(int r=0;r<R;r++){
      int sig[26],siginv[26],freetau[26],kappa[LAG];
      shuffle(sig,26); for(int i=0;i<26;i++)siginv[sig[i]]=i;
      for(int i=0;i<nfree;i++)freetau[i]=rnd()%26; for(int i=0;i<LAG;i++)kappa[i]=rnd()%26;
      int tau[26]; int col=buildtau(sig,freetau,tau);
      int pt[N]; decrypt(sig,siginv,tau,kappa,pt);
      int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
      double cur=qg_of(pt)+Wc*m-Pc*consist(sig)-Pt*col;
      int bs[26],bf[26],bk[LAG];memcpy(bs,sig,104);memcpy(bf,freetau,104);memcpy(bk,kappa,4*LAG);double bsc=cur;int bm=m;
      for(long it=0;it<IT;it++){
        double T=T0*pow(T1/T0,(double)it/IT);
        int mv=rnd()%12; int su=0,sv=0,fi=-1,fo=0,ki=-1,ko=0;
        if(mv<7){ su=rnd()%26;sv=rnd()%26; if(su==sv)continue; int t=sig[su];sig[su]=sig[sv];sig[sv]=t; siginv[sig[su]]=su;siginv[sig[sv]]=sv; }
        else if(mv<10){ fi=rnd()%nfree; fo=freetau[fi]; freetau[fi]=rnd()%26; }
        else { ki=rnd()%LAG; ko=kappa[ki]; kappa[ki]=rnd()%26; }
        int tau2[26]; int col2=buildtau(sig,freetau,tau2);
        decrypt(sig,siginv,tau2,kappa,pt);
        int mm=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])mm++;
        double sc=qg_of(pt)+Wc*mm-Pc*consist(sig)-Pt*col2;
        if(sc>=cur||urand()<exp((sc-cur)/T)){ cur=sc; if(cur>bsc){bsc=cur;memcpy(bs,sig,104);memcpy(bf,freetau,104);memcpy(bk,kappa,4*LAG);bm=mm;} }
        else { /* revert */
          if(mv<7){int t=sig[su];sig[su]=sig[sv];sig[sv]=t;siginv[sig[su]]=su;siginv[sig[sv]]=sv;}
          else if(mv<10) freetau[fi]=fo; else kappa[ki]=ko;
        }
      }
      if(bsc>best){best=bsc;memcpy(bestsig,bs,104);memcpy(bestft,bf,104);memcpy(bestk,bk,4*LAG);bestm=bm;}
    }
    int siginv[26];for(int i=0;i<26;i++)siginv[bestsig[i]]=i; int tau[26];int col=buildtau(bestsig,bestft,tau);
    int pt[N];decrypt(bestsig,siginv,tau,bestk,pt);
    double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
    printf("BEST qg+=%.1f cribs=%d/%d taucol=%d consist=%d qoff=%.4f PT=",best,bestm,ncrib,col,consist(bestsig),no?qo/no:0);
    for(int i=0;i<N;i++)putchar('A'+pt[i]);
    if(hasT){int rec=0;for(int i=0;i<N;i++)if(pt[i]==truePT[i])rec++;printf(" recovered=%d/%d",rec,N);}
    putchar('\n'); return 0;
  }
  fprintf(stderr,"usage: gen SEED | attack qg CT iters R seed [Wc] [truePT]\n");return 1;
}
