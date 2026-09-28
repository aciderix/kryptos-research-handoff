/* kcomposite.c — does the keystream K=C-P hide structure under a NON-STANDARD transform?
 * Targets Scheidt's "more than one step / composite" hint: a progressive/LFSR/grid or
 * periodic+autokey layering leaves a fingerprint under differencing / decimation / autokey-removal
 * even when the raw key looks random. We report IC (index of coincidence *26) for each transform;
 * random ~1.00 (IC 0.0385), structured/monoalpha-ish >1.3. Also functional-periodicity flag reused.
 * Usage: kcomposite CT PT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
static int md(int x){x%=26;return x<0?x+26:x;}
static double ic(const int*a,int n){
  int f[26];for(int i=0;i<26;i++)f[i]=0;for(int i=0;i<n;i++)f[a[i]]++;
  double s=0;for(int i=0;i<26;i++)s+=(double)f[i]*(f[i]-1);
  return n>1? s/((double)n*(n-1))*26.0 : 0;
}
int main(int argc,char**argv){
  const char*cs=argv[1],*ps=argv[2];
  int C[N],P[N],K[N];
  for(int i=0;i<N;i++){C[i]=cs[i]-'A';P[i]=ps[i]-'A';K[i]=md(C[i]-P[i]);}
  printf("raw K            IC*26=%.3f\n",ic(K,N));
  /* first & second difference */
  int D1[N],D2[N];
  for(int i=0;i+1<N;i++)D1[i]=md(K[i+1]-K[i]);
  for(int i=0;i+2<N;i++)D2[i]=md(D1[i+1]-D1[i]);
  printf("diff D1(K)       IC*26=%.3f\n",ic(D1,N-1));
  printf("diff D2(K)       IC*26=%.3f\n",ic(D2,N-2));
  /* decimation: take every step-th letter */
  for(int step=2;step<=12;step++){
    int t[N],n=0; for(int i=0;i<N;i+=step)t[n++]=K[i];
    printf("decimate step=%2d IC*26=%.3f (n=%d)\n",step,ic(t,n),n);
  }
  /* autokey-removal: R_i = K_i - K_{i-d} (removes a period-d self component) then IC */
  for(int d=1;d<=14;d++){
    int t[N],n=0; for(int i=d;i<N;i++)t[n++]=md(K[i]-K[i-d]);
    printf("autoremove d=%2d  IC*26=%.3f\n",d,ic(t,n));
  }
  /* plaintext-autokey removal: maybe K = periodic + f(P). test K_i - P_{i-d} */
  for(int d=0;d<=10;d++){
    int t[N],n=0; for(int i=d;i<N;i++)t[n++]=md(K[i]-P[i-d]);
    printf("Premove   d=%2d   IC*26=%.3f\n",d,ic(t,n));
  }
  /* ciphertext-autokey removal: K_i - C_{i-d} */
  for(int d=0;d<=10;d++){
    int t[N],n=0; for(int i=d;i<N;i++)t[n++]=md(K[i]-C[i-d]);
    printf("Cremove   d=%2d   IC*26=%.3f\n",d,ic(t,n));
  }
  return 0;
}
