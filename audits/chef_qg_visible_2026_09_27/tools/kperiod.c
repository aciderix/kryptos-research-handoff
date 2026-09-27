/* kperiod.c — DECISIVE known-plaintext periodicity test (alphabet-agnostic).
 * For a polyalphabetic cipher with ANY (even keyed/secret) alphabets and period L,
 * within each residue class j={i : i%L==j} the map p_i -> c_i must be a well-defined
 * INJECTIVE function (one fixed substitution alphabet per column). We test that
 * directly on the PUBLIC reconstructed plaintext, with NO assumption on the alphabet.
 *   conflicts(L) = # of (class j, plaintext letter x) where x maps to >1 distinct c.
 * If conflicts(L)==0 for some small L, K4 is periodic-polyalphabetic of period L
 * => method FOUND. We also print a null baseline (shuffled ciphertext) so a
 * spuriously-zero L (from sparse classes) is judged against chance.
 * Usage: kperiod CIPHERTEXT97 PLAINTEXT97 [Lmax] [nulls]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 97
static uint64_t rs=88172645463325252ULL;
static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}

/* conflicts + also injectivity violations (two plaintext letters -> same c in a class) */
static int conflicts(const int*P,const int*C,int L,int *inj_out){
  int conf=0, inj=0;
  for(int j=0;j<L;j++){
    int map[26]; int rev[26];
    for(int x=0;x<26;x++){map[x]=-1;rev[x]=-1;}
    for(int i=j;i<N;i+=L){
      int p=P[i],c=C[i];
      if(map[p]==-1) map[p]=c; else if(map[p]!=c) conf++;
      if(rev[c]==-1) rev[c]=p; else if(rev[c]!=p) inj++;
    }
  }
  if(inj_out)*inj_out=inj;
  return conf;
}
int main(int argc,char**argv){
  const char*cs=argv[1],*ps=argv[2];
  int Lmax=argc>3?atoi(argv[3]):48; int NUL=argc>4?atoi(argv[4]):2000;
  int C[N],P[N]; for(int i=0;i<N;i++){C[i]=cs[i]-'A';P[i]=ps[i]-'A';}
  printf("L  conflicts  inj-viol   null-mean-conflicts  z\n");
  for(int L=1;L<=Lmax;L++){
    int inj; int c=conflicts(P,C,L,&inj);
    /* null: shuffle C, recompute conflicts, get mean/sd */
    double sum=0,sum2=0; int Cs[N];
    for(int t=0;t<NUL;t++){
      for(int i=0;i<N;i++)Cs[i]=C[i];
      for(int i=N-1;i>0;i--){int k=rnd()%(i+1);int tmp=Cs[i];Cs[i]=Cs[k];Cs[k]=tmp;}
      int junk; double cc=conflicts(P,Cs,L,&junk); sum+=cc; sum2+=cc*cc;
    }
    double mean=sum/NUL, var=sum2/NUL-mean*mean, sd=var>0?sqrt(var):1e-9;
    double z=(c-mean)/sd;
    printf("%2d  %6d     %6d      %8.2f          %+.2f%s\n",L,c,inj,mean,z, c==0?"   <== ZERO":"");
  }
  return 0;
}
