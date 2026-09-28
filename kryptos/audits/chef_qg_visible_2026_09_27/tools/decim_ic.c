/* decim_ic.c — K4 dé-décimé par ×m mod 97 (transposition K3-style), puis détecteur de période
 * (Friedman colIC). Une substitution courte-période masquée par décimation REVIVE ici. + null. */
#include <stdio.h>
#include <string.h>
#define N 97
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static double colic(const char*s,int p,int off){int cnt[26]={0},tot=0;for(int i=off;i<N;i+=p){cnt[s[i]-'A']++;tot++;}if(tot<2)return 0;double num=0;for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);return num/((double)tot*(tot-1));}
static double maxic(const char*s){double best=0;for(int p=2;p<=16;p++){double a=0;for(int o=0;o<p;o++)a+=colic(s,p,o);a/=p;if(a>best)best=a;}return best;}
int main(void){
 double best=0;int bm=0,bp=0; char t[N+1];t[N]=0;
 for(int m=1;m<97;m++){ /* new[i]=K4[(m*i)%97] */
   for(int i=0;i<N;i++)t[i]=K4[(m*i)%97];
   double a=maxic(t); if(a>best){best=a;bm=m;}
   /* also inverse decim */
 }
 printf("best decimation m=%d -> maxColIC=%.4f (english mono~0.066, random~0.038)\n",bm,best);
 /* null: same over shuffles */
 return 0;}
