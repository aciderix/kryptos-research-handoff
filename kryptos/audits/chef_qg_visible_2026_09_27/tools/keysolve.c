/* keysolve.c — DECISIVE test: is K4's key an ENGLISH running-key through a keyed alphabet A?
 * Model (single keyed alphabet A, Vigenere family in A-index space):
 *   posA(c_i) = (posA(p_i) + posA(key_i)) mod 26           [variant 0: Vigenere]
 *   posA(c_i) = (posA(key_i) - posA(p_i)) mod 26           [variant 1: Beaufort]
 *   posA(key_i) = (posA(p_i) - posA(c_i)) mod 26           [variant 2: variant-Beaufort]
 * Recovered key letters: key_i = A[posA(key_i)]. We HILL-CLIMB/anneal over the permutation A
 * to MAXIMIZE qg_big(key-text). If some A yields English key-text (qoff ~ -2.0), the method is
 * "running-key via keyed alphabet A" => METHOD IDENTIFIED. If it plateaus at charabia over many
 * restarts (like the null on shuffled ciphertext), no keyed-alphabet running key exists => OTP-like.
 * Usage: keysolve qg.bin CT PT variant iters restarts seed
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define N 97
static float*QG;
static int C[N],P[N];
static uint64_t rs;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double ur(void){return (rnd()>>11)*(1.0/9007199254740992.0);}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static void shuffle(int*A){for(int i=0;i<26;i++)A[i]=i;for(int i=25;i>0;i--){int j=rnd()%(i+1);int t=A[i];A[i]=A[j];A[j]=t;}}
static double qg(const int*t){double s=0;for(int i=0;i+3<N;i++)s+=QG[((t[i]*26+t[i+1])*26+t[i+2])*26+t[i+3]];return s/(N-3);}
/* A[]=letter at index; pos[]=inverse (index of letter). keytext letters given pos & variant. */
static void keytext(const int*A,const int*pos,int var,int*kt){
  for(int i=0;i<N;i++){
    int pp=pos[P[i]], cc=pos[C[i]], ki;
    if(var==0) ki=md(cc-pp);
    else if(var==1) ki=md(cc+pp); /* Beaufort: cc = ki - pp => ki = cc+pp */
    else ki=md(pp-cc);
    kt[i]=A[ki];
  }
}
static double solve(int var,long IT,int R,int*bestA){
  double best=-1e18;
  for(int r=0;r<R;r++){
    int A[26],pos[26]; shuffle(A); for(int i=0;i<26;i++)pos[A[i]]=i;
    int kt[N]; keytext(A,pos,var,kt); double cur=qg(kt);
    double T0=1.0,T1=0.02;
    for(long it=0;it<IT;it++){
      double T=T0*pow(T1/T0,(double)it/IT);
      int u=rnd()%26,v=rnd()%26; if(u==v)continue;
      int t=A[u];A[u]=A[v];A[v]=t; pos[A[u]]=u;pos[A[v]]=v;
      keytext(A,pos,var,kt); double nc=qg(kt);
      if(nc>=cur||ur()<exp((nc-cur)/T)){cur=nc; if(cur>best){best=cur;memcpy(bestA,A,sizeof A);}}
      else{t=A[u];A[u]=A[v];A[v]=t;pos[A[u]]=u;pos[A[v]]=v;}
    }
  }
  return best;
}
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  const char*cs=argv[2],*ps=argv[3];for(int i=0;i<N;i++){C[i]=cs[i]-'A';P[i]=ps[i]-'A';}
  int var=atoi(argv[4]); long IT=atol(argv[5]);int R=atoi(argv[6]);rs=0x9E3779B97F4A7C15ULL*(atol(argv[7])+1);
  int bestA[26]; double best=solve(var,IT,R,bestA);
  int pos[26];for(int i=0;i<26;i++)pos[bestA[i]]=i; int kt[N];keytext(bestA,pos,var,kt);
  printf("var=%d best-qoff(keytext)=%.4f  A=",var,best);
  for(int i=0;i<26;i++)putchar('A'+bestA[i]); printf("\n  KEYTEXT=");
  for(int i=0;i<N;i++)putchar('A'+kt[i]); putchar('\n');
  /* NULL control: same solve on shuffled ciphertext (destroys any real key structure) */
  int Csave[N];memcpy(Csave,C,sizeof C);
  double nullbest=-1e18;
  for(int t=0;t<8;t++){
    for(int i=0;i<N;i++)C[i]=Csave[i];
    for(int i=N-1;i>0;i--){int k=rnd()%(i+1);int tmp=C[i];C[i]=C[k];C[k]=tmp;}
    int na[26];double nb=solve(var,IT/2,R,na); if(nb>nullbest)nullbest=nb;
  }
  memcpy(C,Csave,sizeof C);
  printf("  NULL(shuffled-CT) best-qoff over 8 = %.4f  => real %s null by %.3f\n",
         nullbest, best>nullbest?"BEATS":"~=", best-nullbest);
  return 0;
}
