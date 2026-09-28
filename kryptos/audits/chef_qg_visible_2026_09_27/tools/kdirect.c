#include <stdio.h>
#include <string.h>
#define N 97
static const char*C="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char*P="THECOMPASSROSEISHEREXEASTNORTHEASTTHISISYOURPOSITIONXCOMMISSIONBERLINCLOCKWHICHISNORTHEASTOFHEREX";
static int md(int x){x%=26;return x<0?x+26:x;}
int main(void){
 int c[N],p[N],K[N]; for(int i=0;i<N;i++){c[i]=C[i]-'A';p[i]=P[i]-'A';K[i]=md(c[i]-p[i]);}
 /* direct autokey checks: K[i]==P[i-lag]? and K[i]==C[i-lag]? count matches */
 printf("Plaintext-autokey test: #{i>=lag : K[i]==P[i-lag]} / (97-lag)\n");
 for(int lag=1;lag<=14;lag++){int m=0;for(int i=lag;i<N;i++)if(K[i]==p[i-lag])m++;printf(" lag%d: %d/%d\n",lag,m,N-lag);}
 printf("Ciphertext-autokey test: #{i>=lag : K[i]==C[i-lag]}\n");
 for(int lag=1;lag<=14;lag++){int m=0;for(int i=lag;i<N;i++)if(K[i]==c[i-lag])m++;printf(" lag%d: %d/%d\n",lag,m,N-lag);}
 /* K under KRYPTOS keyed alphabet: sv[letter]=index in keyed alpha */
 const char*KA="KRYPTOSABCDEFGHIJLMNQUVWXZ"; int sv[26]; for(int i=0;i<26;i++)sv[KA[i]-'A']=i;
 int Kk[N]; for(int i=0;i<N;i++)Kk[i]=md(sv[c[i]]-sv[p[i]]);
 int cnt[26]={0};for(int i=0;i<N;i++)cnt[Kk[i]]++;double num=0;for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);
 printf("\nK under KRYPTOS-keyed alphabet: IC=%.4f\n",num/((double)N*(N-1)));
 printf("Kk letters: "); for(int i=0;i<N;i++)putchar('A'+Kk[i]); printf("\n");
 /* also: is K itself = P shifted? or C? already above. Print K numeric for external checks */
 return 0;}
