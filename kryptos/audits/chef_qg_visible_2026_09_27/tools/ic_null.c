/* null: pour 2000 shuffles de K4, applique le MÊME pipeline (toutes transpo W2-24 ×
 * {id,rev}×{inv,fwd}, max colIC période 2-16) et donne la distribution du MAX global.
 * Compare au K4 réel (0.0769). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 97
static char base[N+1]="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static double colic(const char*s,int n,int p,int off){int cnt[26]={0},tot=0;for(int i=off;i<n;i+=p){cnt[s[i]-'A']++;tot++;}if(tot<2)return 0;double num=0;for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);return num/((double)tot*(tot-1));}
static double maxic(const char*s){double best=0;for(int p=2;p<=16;p++){double a=0;for(int o=0;o<p;o++)a+=colic(s,N,p,o);a/=p;if(a>best)best=a;}return best;}
/* de-transpose colonnaire (inverse), order id/rev */
static void detr(const char*ct,int W,int rev,char*out){
 int rows=(N+W-1)/W,rem=N%W,h[64];for(int j=0;j<W;j++)h[j]=(rem==0)?rows:(j<rem?rows:rows-1);
 char g[64][64];memset(g,0,sizeof g);int p=0;
 for(int k=0;k<W;k++){int col=rev?W-1-k:k;for(int r=0;r<h[col];r++)if(p<N)g[r][col]=ct[p++];}
 int o=0;for(int r=0;r<rows;r++)for(int c=0;c<W;c++)if(g[r][c])out[o++]=g[r][c];out[o]=0;
}
static void fwdtr(const char*t,int W,int rev,char*out){
 int rows=(N+W-1)/W;char g[64][64];memset(g,0,sizeof g);int p=0;
 for(int r=0;r<rows;r++)for(int c=0;c<W;c++){if(p<N)g[r][c]=t[p++];}
 int o=0;for(int k=0;k<W;k++){int col=rev?W-1-k:k;for(int r=0;r<rows;r++)if(g[r][col])out[o++]=g[r][col];}out[o]=0;
}
static double pipeline(const char*s){double best=0;char t[N+1];
 for(int W=2;W<=24;W++)for(int rev=0;rev<2;rev++){detr(s,W,rev,t);double a=maxic(t);if(a>best)best=a;fwdtr(s,W,rev,t);a=maxic(t);if(a>best)best=a;}
 return best;}
int main(void){
 double k4=pipeline(base);
 srand(7); int T=2000,ge=0;double sum=0,mx=0;char buf[N+1];strcpy(buf,base);
 for(int t=0;t<T;t++){for(int i=N-1;i>0;i--){int j=rand()%(i+1);char c=buf[i];buf[i]=buf[j];buf[j]=c;}
   double m=pipeline(buf);sum+=m;if(m>mx)mx=m;if(m>=k4)ge++;}
 printf("K4 pipeline max colIC = %.4f\n",k4);
 printf("null (2000 shuffles): mean_max=%.4f  max_max=%.4f  P(null>=K4)=%.3f\n",sum/T,mx,(double)ge/T);
 return 0;}
