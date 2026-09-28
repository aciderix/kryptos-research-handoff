/* contrôle : clé de chaque colonne mod 7 = suite de Fibonacci (graines au hasard), Quagmire III KRYPTOS, Vigenère */
#include <stdio.h>
#include <string.h>
int main(void){ const char*KA="KRYPTOSABCDEFGHIJLMNQUVWXZ"; int KI[26]; for(int i=0;i<26;i++)KI[KA[i]-'A']=i;
 const char*T="ITWASTHEBESTOFTIMESITWASTHEWORSEASTNORTHEASTOFTIMESITWASTHEAGEOFWISDOMITWASTHEAGEOFBERLINCLOCKFOOLISHNESSITWAS";
 const char*e=strstr(T,"EASTNORTHEAST"); int off=(int)(e-T)-21,P[97]; for(int i=0;i<97;i++)P[i]=T[i+off]-'A'; const char*b="BERLINCLOCK"; for(int i=0;i<11;i++)P[63+i]=b[i]-'A';
 int s0[7]={3,17,8,22,5,11,20}, s1[7]={9,2,14,7,19,0,24}, k[97];
 for(int r=0;r<7;r++){ int x0=s0[r],x1=s1[r]; for(int n=0;r+7*n<97;n++){ int v= n==0?x0: n==1?x1: 0; if(n>=2){ v=(x0+x1)%26; x0=x1; x1=v; } k[r+7*n]=v; } }
 for(int i=0;i<97;i++) putchar(KA[(KI[P[i]]+k[i])%26]); putchar('\n'); return 0; }
