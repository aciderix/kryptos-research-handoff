/* contrôle : clé k_n = 3 k_{n-1} + 5 k_{n-2} + 7 k_{n-3} + 11 (mod 26), Quagmire III KRYPTOS, Vigenère */
#include <stdio.h>
#include <string.h>
int main(void){ const char*KA="KRYPTOSABCDEFGHIJLMNQUVWXZ"; int KI[26]; for(int i=0;i<26;i++)KI[KA[i]-'A']=i;
 const char*T="ITWASTHEBESTOFTIMESITWASTHEWORSEASTNORTHEASTOFTIMESITWASTHEAGEOFWISDOMITWASTHEAGEOFBERLINCLOCKFOOLISHNESSITWAS";
 const char*e=strstr(T,"EASTNORTHEAST"); int off=(int)(e-T)-21,P[97]; for(int i=0;i<97;i++)P[i]=T[i+off]-'A'; const char*b="BERLINCLOCK"; for(int i=0;i<11;i++)P[63+i]=b[i]-'A';
 int k[97]; k[0]=4;k[1]=19;k[2]=8; for(int n=3;n<97;n++) k[n]=(3*k[n-1]+5*k[n-2]+7*k[n-3]+11)%26;
 for(int i=0;i<97;i++) putchar(KA[(KI[P[i]+'A'-'A']+k[i])%26]); putchar('\n'); return 0; }
