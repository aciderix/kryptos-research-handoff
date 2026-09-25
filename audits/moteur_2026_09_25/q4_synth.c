#include <stdio.h>
#include <string.h>
static void kw(const char*w,int*S){int u[26]={0},n=0;char a[26];for(;*w;w++){int c=*w-'A';if(!u[c]){u[c]=1;a[n++]=*w;}}for(int c=0;c<26;c++)if(!u[c])a[n++]='A'+c;for(int i=0;i<26;i++)S[a[i]-'A']=i;}
int main(void){const char*T="ITWASTHEBESTOFTIMESITWASTHEWORSEASTNORTHEASTOFTIMESITWASTHEAGEOFWISDOMITWASTHEAGEOFBERLINCLOCKFOOLISHNESSITWAS";
 const char*e=strstr(T,"EASTNORTHEAST");int off=(int)(e-T)-21,P[97];for(int i=0;i<97;i++)P[i]=T[i+off]-'A';const char*b="BERLINCLOCK";for(int i=0;i<11;i++)P[63+i]=b[i]-'A';
 int Y[26],X[26],XI[26];kw("GIRASOL",Y);kw("PALIMPSEST",X);for(int i=0;i<26;i++)XI[X[i]]=i;int m[7]={3,9,14,1,22,7,18},a[4]={0,5,11,20};
 for(int i=0;i<97;i++){int k=(m[i%7]+a[(i+27)/31])%26;putchar('A'+XI[(Y[P[i]]+k)%26]);}putchar('\n');return 0;}
