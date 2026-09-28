/* kanalyze.c — voie hybride : K = C - P (keystream) depuis clair PUBLIC reconstruit. Analyse structure. */
#include <stdio.h>
#include <string.h>
#define N 97
static const char*C="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char*P="THECOMPASSROSEISHEREXEASTNORTHEASTTHISISYOURPOSITIONXCOMMISSIONBERLINCLOCKWHICHISNORTHEASTOFHEREX";
static int md(int x){x%=26;return x<0?x+26:x;}
int main(void){
 int lc=strlen(C),lp=strlen(P);
 printf("len C=%d len P=%d\n",lc,lp);
 /* verify cribs */
 const char*e="EASTNORTHEAST",*b="BERLINCLOCK"; int ok1=1,ok2=1;
 for(int i=0;i<13;i++)if(P[21+i]!=e[i])ok1=0;
 for(int i=0;i<11;i++)if(P[63+i]!=b[i])ok2=0;
 printf("crib EASTNORTHEAST@21-33: %s ; crib BERLINCLOCK@63-73: %s\n",ok1?"OK":"FAIL",ok2?"OK":"FAIL");
 /* K = C - P (Vigenere AZ) */
 int K[N]; for(int i=0;i<N;i++)K[i]=md((C[i]-'A')-(P[i]-'A'));
 printf("\nK (Vig, C-P) letters:\n"); for(int i=0;i<N;i++)putchar('A'+K[i]); printf("\n");
 /* K Beaufort variants */
 printf("\nK (C+P):\n"); for(int i=0;i<N;i++)putchar('A'+md((C[i]-'A')+(P[i]-'A'))); printf("\n");
 printf("K (P-C):\n"); for(int i=0;i<N;i++)putchar('A'+md((P[i]-'A')-(C[i]-'A'))); printf("\n");
 /* IC of K (Vig) */
 int cnt[26]={0}; for(int i=0;i<N;i++)cnt[K[i]]++; double num=0; for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);
 printf("\nIC(K Vig)=%.4f (english~0.066, random~0.038)\n",num/((double)N*(N-1)));
 /* autocorrelation of K: #matches K[i]==K[i+g] */
 printf("autocorr K (matches at gap g):\n");
 for(int g=1;g<=24;g++){int m=0;for(int i=0;i+g<N;i++)if(K[i]==K[i+g])m++;printf(" g=%d:%d",g,m);} printf("\n(hasard ~ (97-g)/26 ≈ 3-3.7)\n");
 return 0;}
