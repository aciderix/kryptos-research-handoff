/* ktranspose.c — completeness: does a COLUMNAR de-transposition of the keystream K=C-P reveal
 * a hidden periodicity (i.e. K4 = periodic-then-transposed)? For width w=2..24, read K into w columns
 * both ways, then over the reordered stream compute functional-periodicity conflicts (min over L=2..12)
 * vs a shuffled null, and IC. Also base note: base change is a fixed bijection -> cannot create
 * structure our IC/period tests would newly detect (stated, not re-run). K ~uniform => all noise expected.
 * Usage: ktranspose CT PT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 97
static uint64_t rs=99991ULL;
static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static int md(int x){x%=26;return x<0?x+26:x;}
static double ic(const int*a,int n){int f[26]={0};for(int i=0;i<n;i++)f[a[i]]++;double s=0;for(int i=0;i<26;i++)s+=(double)f[i]*(f[i]-1);return n>1?s/((double)n*(n-1))*26.0:0;}
/* min functional conflicts over L=2..12 for a stream of length n paired with plaintext-agnostic:
 * here we test SELF-period of the reordered keystream: c_i vs c_{i+L} equal-run consistency is not a
 * cipher; instead we measure IC of each residue class (a periodic key => each class ~constant => IC high).
 * We report max over L of mean-class-IC. */
static double periodicity(const int*a,int n){
  double best=0;
  for(int L=2;L<=12;L++){
    double acc=0;int cnt=0;
    for(int j=0;j<L;j++){int cls[N],m=0;for(int i=j;i<n;i+=L)cls[m++]=a[i];if(m>2){acc+=ic(cls,m);cnt++;}}
    if(cnt){double v=acc/cnt;if(v>best)best=v;}
  }
  return best;/* ~1.0 random, >1.5 = a near-constant class = periodic signal */
}
static void transpose(const int*K,int w,int order,int*out){
  int rows=(N+w-1)/w; int idx=0;
  if(order==0){ /* write row-wise, read column-wise */
    for(int c=0;c<w;c++)for(int r=0;r<rows;r++){int p=r*w+c;if(p<N)out[idx++]=K[p];}
  } else { /* write column-wise, read row-wise */
    int grid[512]; for(int i=0;i<512;i++)grid[i]=-1;
    int p=0; for(int c=0;c<w;c++)for(int r=0;r<rows;r++){int gi=r*w+c; if(p<N && gi<512)grid[gi]=K[p++];}
    for(int i=0;i<512;i++)if(grid[i]>=0)out[idx++]=grid[i];
  }
}
int main(int argc,char**argv){
  const char*cs=argv[1],*ps=argv[2];int K[N];
  for(int i=0;i<N;i++)K[i]=md((cs[i]-'A')-(ps[i]-'A'));
  /* null baseline for periodicity metric on uniform-random length-97 */
  double nmax=0,nsum=0;int T=3000;
  for(int t=0;t<T;t++){int R[N];for(int i=0;i<N;i++)R[i]=rnd()%26;double v=periodicity(R,N);nsum+=v;if(v>nmax)nmax=v;}
  printf("null periodicity metric: mean=%.3f max(3000)=%.3f  (raw K IC*26=%.3f, periodicity=%.3f)\n",
         nsum/T,nmax,ic(K,N),periodicity(K,N));
  double worst=0;
  for(int w=2;w<=24;w++)for(int o=0;o<2;o++){
    int out[N];transpose(K,w,o,out);double per=periodicity(out,N);double icv=ic(out,N);
    if(per>worst)worst=per;
    if(per>nmax) printf("  w=%2d order=%d periodicity=%.3f IC*26=%.3f *** EXCEEDS null ***\n",w,o,per,icv);
  }
  printf("=> max transposed periodicity=%.3f vs null-max=%.3f : %s\n",worst,nmax,
         worst>nmax?"INVESTIGATE":"AUCUNE transposition-grille ne revele de periode => FERME");
  printf("Note base!=26 : un changement de base est une bijection fixe des symboles ; il ne cree aucune\n"
         "structure que IC/periode/complexite-lineaire detecteraient differemment (IC et LC invariants par\n"
         "relabel bijectif). Deja couvert par le verdict OTP-class.\n");
  return 0;
}
