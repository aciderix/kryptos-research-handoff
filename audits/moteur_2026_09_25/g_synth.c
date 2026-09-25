#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void kw(const char*w,int*S){int u[26]={0},n=0;char a[26];for(;*w;w++){int c=*w-'A';if(!u[c]){u[c]=1;a[n++]=*w;}}for(int c=0;c<26;c++)if(!u[c])a[n++]='A'+c;for(int i=0;i<26;i++)S[a[i]-'A']=i;}
int main(int argc,char**argv){const char*T="ITWASTHEBESTOFTIMESITWASTHEWORSEASTNORTHEASTOFTIMESITWASTHEAGEOFWISDOMITWASTHEAGEOFBERLINCLOCKFOOLISHNESSITWAS";
 const char*e=strstr(T,"EASTNORTHEAST");int off=(int)(e-T)-21,P[97];for(int i=0;i<97;i++)P[i]=T[i+off]-'A';const char*b="BERLINCLOCK";for(int i=0;i<11;i++)P[63+i]=b[i]-'A';
 int S[26],SI[26];kw("GIRASOL",S);for(int i=0;i<26;i++)SI[S[i]]=i; int R[64],Cc[64]; srand(7); for(int i=0;i<64;i++){R[i]=rand()%26;Cc[i]=rand()%26;}
 int mode=atoi(argv[1]), q1=atoi(argv[2]), q2=argc>3?atoi(argv[3]):0; int C[97];
 for(int i=0;i<97;i++){int k; if(mode==0) k=R[i/q1]+Cc[i%q1]; else if(mode==1) k=R[i%q1]+Cc[i%q2]; else k=-1;
   if(mode==2){ /* autoclé chiffré vers l'avant : besoin de c_{i+L}, on chiffre à rebours */ }
   if(mode<2) C[i]=SI[(S[P[i]]+k)%26]; }
 if(mode==2){int L=q1; for(int i=96;i>=0;i--){int k= i+L<97? S[C[i+L]] : R[i%7]; C[i]=SI[(S[P[i]]+k)%26];}}
 if(mode==3){int L=q1; int PP[97]; for(int i=96;i>=0;i--){int k= i+L<97? S[P[i+L]] : R[i%7]; C[i]=SI[(S[P[i]]+k)%26];} (void)PP;}
 for(int i=0;i<97;i++)putchar('A'+C[i]);putchar('\n');return 0;}
