/* score_pt.c — JUGE PARTAGÉ : évalue un clair candidat (97 lettres) au qg_big + cribs.
 * Verdict décisif commun aux deux cerveaux. Seuil « anglais réel » : qoff >= -2.6 (typ. -2.0),
 * charabia <= -3.2. Vrai hit = qoff>=-2.6 ET cribs=24 (sans forçage) ET lisible.
 * Compile: cc -O2 -o score_pt score_pt.c -lm ; Usage: score_pt qg.bin PLAINTEXT97  (ou stdin)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}
  FILE*f=fopen(argv[1],"rb"); float*QG=malloc(4*456976); if(fread(QG,4,456976,f)!=456976)return 2; fclose(f);
  char buf[256]; const char*s;
  if(argc>2) s=argv[2]; else { if(!fgets(buf,sizeof buf,stdin))return 1; s=buf; }
  int pt[N],n=0; for(const char*c=s;*c&&n<N;c++){int u=*c;if(u>='a'&&u<='z')u-=32;if(u>='A'&&u<='Z')pt[n++]=u-'A';}
  if(n<N){fprintf(stderr,"need 97 letters, got %d\n",n);return 1;}
  double qo=0;int no=0; for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d]; if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
  double q=no?qo/no:0; int m=0; for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
  const char*verdict = (q>=-2.6 && m==24)?"CANDIDAT SOLUTION (vérifier lisibilité!)" : (q>=-2.6)?"anglais mais cribs incomplets" : "charabia";
  printf("qoff=%.4f cribs=%d/24 verdict=%s\n",q,m,verdict);
  return 0;
}
