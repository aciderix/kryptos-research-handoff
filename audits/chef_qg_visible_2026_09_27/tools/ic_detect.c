/* ic_detect.c — détecteur de PÉRIODICITÉ (Friedman): pour un texte, calcule l'IC moyen
 * par colonne pour chaque période p=2..16. Un Vigenère périodique masqué par transposition
 * REVIT ici (IC colonne ~0.066 vs ~0.038 aléatoire). Usage: ic_detect STRING */
#include <stdio.h>
#include <string.h>
static double colic(const char*s,int n,int p,int off){
 int cnt[26]={0},tot=0; for(int i=off;i<n;i+=p){cnt[s[i]-'A']++;tot++;}
 if(tot<2)return 0; double num=0;for(int k=0;k<26;k++)num+=cnt[k]*(cnt[k]-1);
 return num/((double)tot*(tot-1));
}
int main(int argc,char**argv){const char*s=argv[1];int n=strlen(s);
 double best=0;int bp=0;
 for(int p=1;p<=16;p++){double a=0;for(int o=0;o<p;o++)a+=colic(s,n,p,o);a/=p;
   if(p==1||a>best){if(p>1&&a>best){best=a;bp=p;}}
 }
 printf("maxColIC=%.4f @period=%d  (aleatoire~0.038, monoalpha~0.066)\n",best,bp);
 return 0;}
