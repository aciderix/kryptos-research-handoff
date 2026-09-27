#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 97
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int md(int x){x%=26;return x<0?x+26:x;}
int main(int argc,char**argv){ // argv1=sigmaAlpha argv2=tauAlpha argv3=plaintext97 seedcsv optional
 int sig[26],tau[26],taui[26]; const char*sa=argv[1],*ta=argv[2];
 for(int i=0;i<26;i++){sig[sa[i]-'A']=i; tau[ta[i]-'A']=i;} for(int i=0;i<26;i++)taui[tau[i]]=i;
 const char*e="EASTNORTHEAST",*b="BERLINCLOCK"; int pt[N];
 const char*p=argv[3]; for(int i=0;i<N;i++)pt[i]=p[i]-'A';
 for(int i=0;i<13;i++)pt[CE[i]]=e[i]-'A'; for(int i=0;i<11;i++)pt[CB[i]]=b[i]-'A';
 int kap[7]; for(int i=0;i<7;i++)kap[i]=(argc>4)?atoi(strtok(i==0?strdup(argv[4]):NULL,","))%26:(i*3+1)%26;
 int x[N],ct[N]; for(int i=0;i<N;i++){int k=(i<7)?kap[i]:x[i-7]; x[i]=sig[pt[i]]; ct[i]=taui[md(x[i]+k)];}
 for(int i=0;i<N;i++)putchar('A'+ct[i]); putchar(' '); printf("PT="); for(int i=0;i<N;i++)putchar('A'+pt[i]); putchar('\n');
 return 0;}
