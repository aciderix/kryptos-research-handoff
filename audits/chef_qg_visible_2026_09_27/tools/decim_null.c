#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
static char base[N+1]="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static double colic(const char*s,int p,int off){int cnt[26]={0},tot=0;for(int i=off;i<N;i+=p){cnt[s[i]-'A']++;tot++;}if(tot<2)return 0;double num=0;for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);return num/((double)tot*(tot-1));}
static double maxic(const char*s){double best=0;for(int p=2;p<=16;p++){double a=0;for(int o=0;o<p;o++)a+=colic(s,p,o);a/=p;if(a>best)best=a;}return best;}
static double pipe(const char*b){double best=0;char t[N+1];t[N]=0;for(int m=1;m<97;m++){for(int i=0;i<N;i++)t[i]=b[(m*i)%97];double a=maxic(t);if(a>best)best=a;}return best;}
int main(void){double k=pipe(base);srand(9);int T=300,ge=0;double mx=0;char buf[N+1];strcpy(buf,base);
 for(int t=0;t<T;t++){for(int i=N-1;i>0;i--){int j=rand()%(i+1);char c=buf[i];buf[i]=buf[j];buf[j]=c;}double m=pipe(buf);if(m>mx)mx=m;if(m>=k)ge++;}
 printf("K4=%.4f  null max=%.4f  P(null>=K4)=%.3f\n",k,mx,(double)ge/T);return 0;}
