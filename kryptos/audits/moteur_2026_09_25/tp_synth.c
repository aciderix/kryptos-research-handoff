#include <stdio.h>
#include <string.h>
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
int main(void){ const char *T="ITWASTHEBESTOFTIMESITWASTHEWORSEASTNORTHEASTOFTIMESITWASTHEAGEOFWISDOMITWASTHEAGEOFBERLINCLOCKFOOLISHNESSITWAS";
 const char *e=strstr(T,"EASTNORTHEAST"); int off=(int)(e-T)-21; int P[97]; for(int i=0;i<97;i++)P[i]=T[i+off]-'A'; const char*b="BERLINCLOCK"; for(int i=0;i<11;i++)P[63+i]=b[i]-'A';
 int KI[26]; for(int i=0;i<26;i++)KI[KA[i]-'A']=i; int key[5]={3,17,8,22,11}; int I[97];
 for(int i=0;i<97;i++) I[i]= KA[(KI[P[i]]+key[i%5])%26]-'A';   /* substitution d'abord */
 int ord[7]={3,1,4,0,6,2,5}, w=7, h=14, full=97%7, t=0, pi[97];
 for(int q=0;q<w;q++){int c=ord[q],hc=c<full?h:h-1; for(int r=0;r<hc;r++){int rr=hc-1-r; pi[rr*w+c]=t++;}}
 char C[98]; for(int i=0;i<97;i++) C[pi[i]]='A'+I[i]; C[97]=0; puts(C); return 0; }
