/* gromark_solve.c — Gromark (Bean model) résolu au qg_big fort.
 * c_i = tau[(sig[p_i]+k_i)%26] ; k = primer(5) puis k_i=(k_{i-5}+k_{i-4})%10.
 * décrypt: p_i = siginv[(tv[c_i]-k_i)%26]. Recherche (sig,tv) SA scorée qg_big + cribs.
 * But: Bean a trouvé 39 amorces crib-compatibles mais « anglais non convaincant » (SCORING FAIBLE).
 * qg_big tranche: anglais réel (qoff~-2) ou charabia (~-3.3) ?
 * Usage: gromark_solve qg.bin CT primer iters R seed [Wc]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define N 97
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
static void shuffle(int*s){for(int i=0;i<26;i++)s[i]=i;for(int i=25;i>0;i--){int j=rnd()%(i+1);int t=s[i];s[i]=s[j];s[j]=t;}}
static float*QG;
static int K[N],CT[N];
static void genkey(const char*primer){int n=strlen(primer);for(int i=0;i<n;i++)K[i]=primer[i]-'0';
 for(int i=n;i<N;i++)K[i]=(K[i-n]+K[i-n+1])%10;}
static double qg(const int*p){double s=0;for(int i=0;i+3<N;i++)s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];return s;}
/* état: sig[letter]=val, siginv, tv[letter]=val (cipher alpha) */
static void decrypt(const int*siginv,const int*tv,int*pt){
 for(int i=0;i<N;i++){int v=md(tv[CT[i]]-K[i]);pt[i]=siginv[v];}}
int main(int argc,char**argv){
 setcribs();
 FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
 const char*cts=argv[2];for(int i=0;i<N;i++)CT[i]=cts[i]-'A';
 genkey(argv[3]);
 long IT=atol(argv[4]);int R=atoi(argv[5]);rs=0x9E3779B97F4A7C15ULL*(atol(argv[6])+1);
 double Wc=argc>7?atof(argv[7]):8.0;
 double T0=5.0,T1=0.1; int bsg[26],btv[26];double best=-1e18;int bm=0;
 for(int r=0;r<R;r++){
  int sig[26],siginv[26],tv[26];shuffle(sig);for(int i=0;i<26;i++)siginv[sig[i]]=i;shuffle(tv);
  int pt[N];decrypt(siginv,tv,pt);int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
  double cur=qg(pt)+Wc*m;int bs2[26],bt2[26];memcpy(bs2,sig,104);memcpy(bt2,tv,104);double bc=cur;int bmm=m;
  for(long it=0;it<IT;it++){
   double T=T0*pow(T1/T0,(double)it/IT);int mv=rnd()%10;int u=0,v=0,which=0;
   if(mv<5){u=rnd()%26;v=rnd()%26;if(u==v)continue;int t=sig[u];sig[u]=sig[v];sig[v]=t;siginv[sig[u]]=u;siginv[sig[v]]=v;which=0;}
   else{u=rnd()%26;v=rnd()%26;if(u==v)continue;int t=tv[u];tv[u]=tv[v];tv[v]=t;which=1;}
   decrypt(siginv,tv,pt);int mm=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])mm++;
   double sc=qg(pt)+Wc*mm;
   if(sc>=cur||urand()<exp((sc-cur)/T)){cur=sc;if(cur>bc){bc=cur;memcpy(bs2,sig,104);memcpy(bt2,tv,104);bmm=mm;}}
   else{if(which==0){int t=sig[u];sig[u]=sig[v];sig[v]=t;siginv[sig[u]]=u;siginv[sig[v]]=v;}else{int t=tv[u];tv[u]=tv[v];tv[v]=t;}}
  }
  if(bc>best){best=bc;memcpy(bsg,bs2,104);memcpy(btv,bt2,104);bm=bmm;}
 }
 int siginv[26];for(int i=0;i<26;i++)siginv[bsg[i]]=i;int pt[N];decrypt(siginv,btv,pt);
 double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
 printf("primer=%s cribs=%d/%d qoff=%.4f PT=",argv[3],bm,ncrib,no?qo/no:0);
 for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');
 return 0;}
