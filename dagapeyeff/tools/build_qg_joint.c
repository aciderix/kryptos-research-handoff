/* build_qg_joint.c — quadgrammes en log-probabilité JOINTE : QG[abcd] = ln(count(abcd)/N), plancher ln(0.01/N).
 * Remplace le modèle conditionnel à repli (build_qg.c de kryptos) dont le repli attribue ln(0,5/13)≈-3,26 à un
 * contexte trigramme inconnu : un recuit à substitution libre exploite cette faille (optimum dégénéré ≈ -3,25).
 * Usage : build_qg_joint corpus_AZ.txt qg_joint.bin [plancher_en_comptes, défaut 0.01] */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"rb");fseek(f,0,SEEK_END);long L=ftell(f);fseek(f,0,SEEK_SET);
  char*t=malloc(L);if(fread(t,1,L,f)!=(size_t)L)return 2;fclose(f);
  long*c=calloc(456976,sizeof(long)); long N=0;
  for(long i=0;i+3<L;i++){int a=t[i]-'A',b=t[i+1]-'A',d=t[i+2]-'A',e=t[i+3]-'A';
    if(a<0||a>25||b<0||b>25||d<0||d>25||e<0||e>25)continue; c[((a*26+b)*26+d)*26+e]++;N++;}
  float*q=malloc(4*456976); double fk=argc>3?atof(argv[3]):0.01; double fl=log(fk/N);
  for(int i=0;i<456976;i++) q[i]=c[i]?(float)log((double)c[i]/N):(float)fl;
  FILE*o=fopen(argv[2],"wb");fwrite(q,4,456976,o);fclose(o);
  fprintf(stderr,"N=%ld quadgrammes, plancher=%.2f\n",N,fl);return 0;}
