/* keyread.c — pour un alphabet PUBLIC fixé, LIT le flux de clé aux 24 positions de cribs.
 * Vig: k_i=(σ(c_i)-σ(p_i))%26 ; Beau: k_i=(σ(p_i)+σ(c_i))%26 ; var: k_i=(σ(p_i)-σ(c_i))%26.
 * Cherche une structure (le clé "simple/mémorisable" de Scheidt doit se voir sur 24 valeurs). */
#include <stdio.h>
#include <string.h>
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int md(int x){x%=26;return x<0?x+26:x;}
int main(int argc,char**argv){
 const char*sa=argv[1];int sv[26];for(int i=0;i<26;i++)sv[sa[i]-'A']=i;
 const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
 int pos[24],pl[24],np=0;
 for(int i=0;i<13;i++){pos[np]=CE[i];pl[np]=e[i]-'A';np++;}
 for(int i=0;i<11;i++){pos[np]=CB[i];pl[np]=b[i]-'A';np++;}
 for(int form=0;form<3;form++){
  printf("=== sigma=%s form=%d (0Vig 1Beau 2var) ===\n",sa,form);
  int kv[97];for(int i=0;i<97;i++)kv[i]=-1;
  for(int j=0;j<np;j++){int i=pos[j];int c=sv[K4[i]-'A'],p=sv[pl[j]];
    int k;if(form==0)k=md(c-p);else if(form==1)md(p+c),k=md(p+c);else k=md(p-c);kv[i]=k;}
  printf(" pos:key  "); for(int j=0;j<np;j++)printf("%d:%d ",pos[j],kv[pos[j]]); printf("\n");
  /* motif mod 7 : les positions de meme residu mod7 ont-elles meme clé ? (periode 7 masquee) */
  printf(" par residu mod7:\n");
  for(int r=0;r<7;r++){printf("  r%d:",r);for(int i=r;i<97;i+=7)if(kv[i]>=0)printf(" [%d]=%d",i,kv[i]);printf("\n");}
 }
 return 0;}
