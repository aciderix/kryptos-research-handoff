/* runkey.c — K4 = running-key (clé courante) déterministe avec un TEXTE PUBLIC comme clé.
 * Hypothèse artiste : l'œuvre se déchiffre avec son PROPRE texte résolu (K1/K2/K3), 100% public.
 * σ = alphabet keyé (KRYPTOS comme K1-K3, ou autre). Formes Vig/Beaufort/variante. On balaie
 * l'OFFSET d'alignement de la clé. Déterministe => rapide, conclusif. Score qg_big + cribs.
 * value sv[] = position dans l'alphabet σ. Vig: sv[p]=sv[c]-sv[k]; Beau: sv[p]=sv[k]-sv[c]; var: sv[p]=sv[c]+sv[k].
 * Compile: cc -O2 -o runkey runkey.c -lm ; Usage: runkey qg CT keytext sigmaAlpha form
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 97
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N],ncrib;
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";ncrib=0;
 for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';ncrib++;}
 for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';ncrib++;}}
static int md(int x){x%=26;return x<0?x+26:x;}
static float*QG;
int main(int argc,char**argv){
  if(argc<6){fprintf(stderr,"usage: runkey qg CT keytext sigma form\n");return 1;}
  setcribs();
  FILE*f=fopen(argv[1],"rb");QG=malloc(sizeof(float)*456976);if(fread(QG,sizeof(float),456976,f)!=456976)return 2;fclose(f);
  const char*cts=argv[2]; int ct[N]; for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
  const char*kt=argv[3]; int KL=strlen(kt); int key[4096]; for(int i=0;i<KL&&i<4096;i++)key[i]=kt[i]-'A';
  const char*sa=argv[4]; int sv[26]; for(int i=0;i<26;i++) sv[sa[i]-'A']=i;
  int svinv[26]; for(int i=0;i<26;i++) svinv[sv[i]]=i;
  int form=atoi(argv[5]);
  int bestoff=-1,bestm=-1; double bestq=-1e18; char bestpt[N+1];
  for(int off=0; off+N<=KL; off++){
    int pt[N];
    for(int i=0;i<N;i++){int cv=sv[ct[i]],kv=sv[key[off+i]],pv;
      if(form==0)pv=md(cv-kv); else if(form==1)pv=md(kv-cv); else pv=md(cv+kv);
      pt[i]=svinv[pv];}
    int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
    double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
    double q=no?qo/no:0;
    /* on garde le meilleur par cribs puis qg */
    if(m>bestm || (m==bestm && q>bestq)){bestm=m;bestq=q;bestoff=off;for(int i=0;i<N;i++)bestpt[i]='A'+pt[i];bestpt[N]=0;}
  }
  printf("bestoff=%d cribs=%d/%d qoff=%.4f PT=%s\n",bestoff,bestm,ncrib,bestq,bestpt);
  return 0;
}
