/* gapstat.c — profil de coïncidence du chiffré K4 par écart g, vs nulls aléatoires.
 * stat(g) = #{i : c_i == c_{i+g}} (doublets à écart g). Compare K4 à 100000 chaînes
 * aléatoires de même longueur (97) et même distribution de lettres que K4 (shuffle). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
#define N 97
static int co(const char*s,int g){int c=0;for(int i=0;i+g<N;i++)if(s[i]==s[i+g])c++;return c;}
int main(void){
  srand(1234);
  printf("g  K4  null_mean  null_p95  z    p(null>=K4)\n");
  for(int g=1;g<=20;g++){
    int k=co(K4,g);
    char buf[N+1];strcpy(buf,K4);
    double sum=0,sum2=0;int ge=0,T=100000;int cnts[50]={0};
    for(int t=0;t<T;t++){
      for(int i=N-1;i>0;i--){int j=rand()%(i+1);char tmp=buf[i];buf[i]=buf[j];buf[j]=tmp;}
      int c=co(buf,g);sum+=c;sum2+=(double)c*c;if(c<50)cnts[c]++;if(c>=k)ge++;
    }
    double mean=sum/T,var=sum2/T-mean*mean,sd=var>0?__builtin_sqrt(var):1e-9;
    double z=(k-mean)/sd;
    /* p95 */
    int acc=0,p95=0;for(int c=0;c<50;c++){acc+=cnts[c];if(acc>=0.95*T){p95=c;break;}}
    printf("%-2d %-3d %-9.2f %-8d %-5.2f %.4f%s\n",g,k,mean,p95,z,(double)ge/T, g==7?"  <-- ecart 7":"");
  }
  return 0;
}
