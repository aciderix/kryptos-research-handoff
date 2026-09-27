/* keyletters.c — la clé est-elle une PHRASE anglaise mémorisée ? Les cribs révèlent la clé:
 * Vig: kL_i=σ⁻¹((σ(c_i)-σ(p_i))%26) ; Beau: σ⁻¹((σ(p_i)+σ(c_i))%26)? on teste 3 formes.
 * Affiche les LETTRES de clé sur les 2 plages contiguës de cribs (21-33, 63-73). */
#include <stdio.h>
#include <string.h>
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int md(int x){x%=26;return x<0?x+26:x;}
int main(int argc,char**argv){
 const char*sa=argv[1];int sv[26],svinv[26];for(int i=0;i<26;i++)sv[sa[i]-'A']=i;for(int i=0;i<26;i++)svinv[sv[i]]=i;
 const char*e="EASTNORTHEAST"; int c1=21;
 const char*b="BERLINCLOCK"; int c2=63;
 for(int form=0;form<3;form++){
  printf("form%d (0Vig 1Beau 2var) sigma=%s\n",form,sa);
  printf("  key@21-33: ");
  for(int i=0;i<13;i++){int p=e[i]-'A',c=K4[c1+i]-'A';int k;
    if(form==0)k=md(sv[c]-sv[p]);else if(form==1)k=md(sv[p]+sv[c]);else k=md(sv[p]-sv[c]);
    putchar('A'+svinv[k]);}
  printf("\n  key@63-73: ");
  for(int i=0;i<11;i++){int p=b[i]-'A',c=K4[c2+i]-'A';int k;
    if(form==0)k=md(sv[c]-sv[p]);else if(form==1)k=md(sv[p]+sv[c]);else k=md(sv[p]-sv[c]);
    putchar('A'+svinv[k]);}
  printf("\n");
 }
 return 0;}
