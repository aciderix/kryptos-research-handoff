/* constkey.c — clean 0-free-parameter test: is K4's keystream K=C-P the base-26 expansion
 * of a famous IRRATIONAL constant (sqrt-family + golden ratio phi)? Pure C bignum (base 2^32),
 * floor(sqrt(n)*26^D) via bit-by-bit integer sqrt; phi=(26^D+isqrt(5*26^(2D)))/2.
 * Match K (and P-C) against each constant's base-26 stream at ALL offsets, fwd & reversed.
 * Random expectation ~97/26=3.7 matches (SD~1.9); a real key => ~97. e & pi covered by MECA.
 * Usage: constkey CT PT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define NL 80          /* limbs (2^32 each) -> ~2560 bits, ample for 26^150 */
typedef struct{ uint32_t d[NL]; } BN;
static void bz(BN*a){memset(a->d,0,sizeof a->d);}
static void bset(BN*a,uint32_t v){bz(a);a->d[0]=v;}
static int btop(const BN*a){for(int i=NL-1;i>=0;i--)if(a->d[i])return i;return -1;}
static int bncmp(const BN*a,const BN*b){for(int i=NL-1;i>=0;i--){if(a->d[i]!=b->d[i])return a->d[i]<b->d[i]?-1:1;}return 0;}
static void bsub(BN*a,const BN*b){ /* a-=b, assume a>=b */ int64_t br=0; for(int i=0;i<NL;i++){int64_t v=(int64_t)a->d[i]-b->d[i]-br; if(v<0){v+=(1LL<<32);br=1;}else br=0; a->d[i]=(uint32_t)v;} }
static void badd(BN*a,const BN*b){ uint64_t c=0; for(int i=0;i<NL;i++){uint64_t v=(uint64_t)a->d[i]+b->d[i]+c; a->d[i]=(uint32_t)v; c=v>>32;} }
static void bmul_small(BN*a,uint32_t m){ uint64_t c=0; for(int i=0;i<NL;i++){uint64_t v=(uint64_t)a->d[i]*m+c; a->d[i]=(uint32_t)v; c=v>>32;} }
/* a = a*b (both fit) */
static void bmul(const BN*a,const BN*b,BN*r){ uint64_t tmp[NL]; memset(tmp,0,sizeof tmp);
  for(int i=0;i<NL;i++){ if(!a->d[i])continue; uint64_t c=0; for(int j=0;i+j<NL;j++){ uint64_t v=tmp[i+j]+(uint64_t)a->d[i]*b->d[j]+c; tmp[i+j]=v&0xffffffff; c=v>>32; } }
  bz(r); for(int i=0;i<NL;i++)r->d[i]=(uint32_t)tmp[i]; }
static void bshr1(BN*a){ uint32_t c=0; for(int i=NL-1;i>=0;i--){uint32_t nc=a->d[i]&1; a->d[i]=(a->d[i]>>1)|(c<<31); c=nc;} }
/* set bit k */
static void bsetbit(BN*a,int k){ a->d[k>>5]|=(1u<<(k&31)); }
/* isqrt: r=floor(sqrt(M)) via bit-by-bit */
static void bisqrt(const BN*M,BN*res){ BN root; bz(&root);
  int top=btop(M); if(top<0){bz(res);return;}
  int hb=top*32+31; while(hb>0 && !((M->d[hb>>5]>>(hb&31))&1)) hb--;
  int start=hb/2+1;                 /* sqrt has ~half the bits */
  for(int b=start;b>=0;b--){        /* greedy: set each bit high->low if cand^2<=M */
    BN cand=root; bsetbit(&cand,b);
    BN sq; bmul(&cand,&cand,&sq);
    if(bncmp(&sq,M)<=0) root=cand;
  }
  *res=root;
}
/* extract D base-26 digits (fractional) from R=floor(x*26^D): repeated divmod 26, LSB first */
static void base26_digits(BN R,int D,int*out){
  for(int k=0;k<D;k++){ uint64_t rem=0; for(int i=NL-1;i>=0;i--){ uint64_t cur=(rem<<32)|R.d[i]; R.d[i]=(uint32_t)(cur/26); rem=cur%26; } out[k]=(int)rem; }
  /* out[0]=least significant = last fractional digit; reverse to get d1,d2,... */
  for(int i=0,j=D-1;i<j;i++,j--){int t=out[i];out[i]=out[j];out[j]=t;}
}
#define N 97
static int md(int x){x%=26;return x<0?x+26:x;}
static int matchmax(const int*stream,int slen,const int*K,const char*name){
  int best=0,bestoff=0,bestdir=0;
  for(int dir=0;dir<2;dir++){
    for(int off=0; off+N<=slen; off++){
      int m=0; for(int i=0;i<N;i++){int s= dir? stream[off+N-1-i] : stream[off+i]; if(s==K[i])m++;}
      if(m>best){best=m;bestoff=off;bestdir=dir;}
    }
  }
  printf("  %-8s best matches=%2d/97 (off=%d dir=%d)\n",name,best,bestoff,bestdir);
  return best;
}
static void gen_sqrt(int n,int D,int*digits){ /* floor(sqrt(n)*26^D) base26 digits */
  BN B; bset(&B,1); for(int i=0;i<D;i++)bmul_small(&B,26);   /* B=26^D */
  BN B2; bmul(&B,&B,&B2);                                     /* 26^(2D) */
  BN M=B2; bmul_small(&M,(uint32_t)n);                        /* n*26^(2D) */
  BN R; bisqrt(&M,&R);
  base26_digits(R,D,digits);
}
static void gen_phi(int D,int*digits){ /* phi=(1+sqrt5)/2 ; floor(phi*26^D)=(26^D+floor(sqrt5*26^D... )) */
  BN B; bset(&B,1); for(int i=0;i<D;i++)bmul_small(&B,26);
  BN B2; bmul(&B,&B,&B2); BN M=B2; bmul_small(&M,5); BN R; bisqrt(&M,&R); /* floor(sqrt5*26^D) */
  badd(&R,&B); bshr1(&R);                                     /* (26^D+floor)/2 */
  base26_digits(R,D,digits);
}
int main(int argc,char**argv){
  const char*cs=argv[1],*ps=argv[2];
  int C[N],P[N],K[N],K2[N];
  for(int i=0;i<N;i++){C[i]=cs[i]-'A';P[i]=ps[i]-'A';K[i]=md(C[i]-P[i]);K2[i]=md(P[i]-C[i]);}
  int D=150; int dig[160];
  int roots[]={2,3,5,6,7,8,10,11,13};
  printf("K=C-P vs constant base-26 streams (D=%d). random ~3.7/97, SD~1.9. investigate if >15.\n",D);
  int worst=0;
  for(int r=0;r<(int)(sizeof roots/sizeof*roots);r++){
    gen_sqrt(roots[r],D,dig); char nm[16]; snprintf(nm,16,"sqrt%d",roots[r]);
    int a=matchmax(dig,D,K,nm); int b=matchmax(dig,D,K2,nm);
    if(a>worst)worst=a; if(b>worst)worst=b;
  }
  gen_phi(D,dig); { int a=matchmax(dig,D,K,"phi"); int b=matchmax(dig,D,K2,"phi"); if(a>worst)worst=a; if(b>worst)worst=b; }
  printf("=> MAX matches over all constants/offsets/dirs = %d/97 %s\n", worst,
         worst>15? "*** INVESTIGATE ***":"= NOISE (negatif, coherent OTP)");
  return 0;
}
