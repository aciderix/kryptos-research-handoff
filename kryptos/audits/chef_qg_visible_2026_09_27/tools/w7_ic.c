/* w7_ic.c — K4 dé-transposé par colonnes largeur 7, TOUS les 5040 ordres de colonnes, puis
 * détecteur de période (Friedman colIC). Une substitution périodique masquée par transposition
 * colonnaire-7 REVIVE ici. Compare K4 au null (shuffles). 7=|KRYPTOS|, engage l'excès écart-7. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 97
#define W 7
static char K4[]="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static double colic(const char*s,int p,int off){int cnt[26]={0},tot=0;for(int i=off;i<N;i+=p){cnt[s[i]-'A']++;tot++;}if(tot<2)return 0;double num=0;for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);return num/((double)tot*(tot-1));}
static double maxic(const char*s){double best=0;for(int p=2;p<=16;p++){double a=0;for(int o=0;o<p;o++)a+=colic(s,p,o);a/=p;if(a>best)best=a;}return best;}
/* de-transpose width-7, column order perm[0..6] (which source column is read k-th).
   grid filled column-by-column in perm order (that's how CT was read); we invert to row-major. */
static int heights[W];
static void detr(const char*ct,const int*perm,char*out){
 int rows=(N+W-1)/W, rem=N%W; for(int j=0;j<W;j++)heights[j]=(rem==0)?rows:(j<rem?rows:rows-1);
 char g[16][W]; memset(g,0,sizeof g); int p=0;
 for(int k=0;k<W;k++){int col=perm[k];for(int r=0;r<heights[col];r++)if(p<N)g[r][col]=ct[p++];}
 int o=0;for(int r=0;r<rows;r++)for(int c=0;c<W;c++)if(g[r][c])out[o++]=g[r][c];out[o]=0;
}
static int perm[W],used[W]; static double bestic; static int bestperm[W];
static void rec(const char*ct,int d,char*tmp){
 if(d==W){detr(ct,perm,tmp);double a=maxic(tmp);if(a>bestic){bestic=a;memcpy(bestperm,perm,sizeof perm);}return;}
 for(int i=0;i<W;i++)if(!used[i]){used[i]=1;perm[d]=i;rec(ct,d+1,tmp);used[i]=0;}
}
static double pipeline(const char*ct){bestic=0;memset(used,0,sizeof used);char tmp[N+1];rec(ct,0,tmp);return bestic;}
int main(void){
 double k=pipeline(K4);
 printf("K4 best-over-5040 maxColIC=%.4f, perm=",k);for(int i=0;i<W;i++)printf("%d",bestperm[i]);printf("\n");
 srand(11);int T=200,ge=0;double mx=0;char buf[N+1];strcpy(buf,K4);
 for(int t=0;t<T;t++){for(int i=N-1;i>0;i--){int j=rand()%(i+1);char c=buf[i];buf[i]=buf[j];buf[j]=c;}double m=pipeline(buf);if(m>mx)mx=m;if(m>=k)ge++;}
 printf("null(200 shuffles): max=%.4f P(null>=K4)=%.3f\n",mx,(double)ge/T);
 return 0;}
