/* clockkey.c — EXPLICIT closure: is K4's keystream derived from the Berlin Clock (Mengenlehreuhr)?
 * Mengenlehreuhr per minute HH:MM lamp rows: r1=H/5 (0..4), r2=H%5 (0..4), r3=M/5 (0..11), r4=M%5 (0..4),
 * plus seconds lamp (ignored, minute granularity). We build several natural deterministic streams over a
 * full day (1440 min, wrap) and, per encoding, match K against the stream at ALL start-minutes & both
 * directions, in BOTH alphabet conventions (AZ additive C-P, and KRYPTOS-keyed key index). We report the
 * best match vs a STRICT uniform null (K is ~uniform, IC 0.039), so any excess would show real structure.
 * Usage: clockkey CT PT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#define N 97
static uint64_t rs=1234567891011ULL;
static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static const char*KAL="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int posK[26];
static int md(int x){x%=26;return x<0?x+26:x;}

/* match K against a cyclic stream of length L, all offsets & 2 dirs; return best match count */
static int bestmatch(const int*stream,int L,const int*K){
  int best=0;
  for(int dir=0;dir<2;dir++) for(int off=0;off<L;off++){
    int m=0; for(int i=0;i<N;i++){int idx=dir?(off-i+ (long)L*3):(off+i); idx%=L; int s=stream[idx]; if(s==K[i])m++;}
    if(m>best)best=m;
  }
  return best;
}
/* strict null: same stream, but K replaced by uniform-random 97-length; return mean+max over T */
static void nullstat(const int*stream,int L,int T,double*mean,int*mx){
  double s=0; int mm=0;
  for(int t=0;t<T;t++){ int R[N]; for(int i=0;i<N;i++)R[i]=rnd()%26; int b=bestmatch(stream,L,R); s+=b; if(b>mm)mm=b; }
  *mean=s/T; *mx=mm;
}
int main(int argc,char**argv){
  for(int i=0;i<26;i++)posK[KAL[i]-'A']=i;
  const char*cs=argv[1],*ps=argv[2];
  int C[N],P[N];
  for(int i=0;i<N;i++){C[i]=cs[i]-'A';P[i]=ps[i]-'A';}
  /* two key conventions */
  int Kaz[N],Kkr[N],Kaz2[N],Kkr2[N];
  for(int i=0;i<N;i++){
    Kaz[i]=md(C[i]-P[i]); Kaz2[i]=md(P[i]-C[i]);
    Kkr[i]=md(posK[C[i]]-posK[P[i]]); Kkr2[i]=md(posK[P[i]]-posK[C[i]]);
  }
  /* Build clock encodings over the day. We store into big arrays. */
  static int encSum[1440], encBits[1440], enc4[1440*4], encRed[1440];
  for(int t=0;t<1440;t++){
    int H=t/60,M=t%60; int r1=H/5,r2=H%5,r3=M/5,r4=M%5;
    encSum[t]=(r1+r2+r3+r4)%26;                 /* A: total lit lamps mod26 (range 0..23) */
    /* B: 24-lamp binary vector -> integer mod26 : bits = r1 ones then (4-r1) zeros etc. compact: value */
    int bits=0,nb=0;
    for(int k=0;k<4;k++){bits=(bits<<1)|(k<r1);nb++;}
    for(int k=0;k<4;k++){bits=(bits<<1)|(k<r2);nb++;}
    for(int k=0;k<11;k++){bits=(bits<<1)|(k<r3);nb++;}
    for(int k=0;k<4;k++){bits=(bits<<1)|(k<r4);nb++;}
    encBits[t]=((unsigned)bits)%26;             /* B: lamp bitfield mod26 */
    enc4[4*t+0]=r1;enc4[4*t+1]=r2;enc4[4*t+2]=r3%26;enc4[4*t+3]=r4;  /* C: 4 digits/min stream */
    encRed[t]=(r3>=3?1:0)+(r3>=6?1:0)+(r3>=9?1:0);  /* D: number of red quarter lamps lit (0..3) */
  }
  struct{const char*nm;const int*s;int L;} encs[]={
    {"sum_mod26",encSum,1440},{"lampbits_mod26",encBits,1440},{"rows4_stream",enc4,1440*4},{"redquarters",encRed,1440}
  };
  struct{const char*nm;const int*K;} keys[]={
    {"K_AZ(C-P)",Kaz},{"K_AZ(P-C)",Kaz2},{"K_KRYPTOS(C-P)",Kkr},{"K_KRYPTOS(P-C)",Kkr2}
  };
  printf("Mengenlehreuhr -> keystream closure test (K ~uniform, random expect ~3.7/97)\n");
  int worst_excess=0;
  for(int e=0;e<4;e++){
    double nmean; int nmax; nullstat(encs[e].s,encs[e].L,3000,&nmean,&nmax);
    printf("[%s] null: mean-best=%.2f max-best(3000 draws)=%d\n",encs[e].nm,nmean,nmax);
    for(int k=0;k<4;k++){
      int b=bestmatch(encs[e].s,encs[e].L,keys[k].K);
      int excess=b-nmax;
      printf("   %-16s bestmatch=%2d/97  %s\n",keys[k].nm,b, b>nmax?"*** EXCEEDS NULL ***":"<= null (noise)");
      if(excess>worst_excess)worst_excess=excess;
    }
  }
  printf("=> %s\n", worst_excess>0? "*** a key EXCEEDS its null — INVESTIGATE ***"
                                   : "AUCUN encodage horloge ne dépasse le null => FERMÉ (négatif, cohérent OTP)");
  return 0;
}
