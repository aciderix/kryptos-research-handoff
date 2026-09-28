/* double_vig.c — MASQUAGE = Vigenère COMPOSÉ (2 mots-clés publics) + alphabet keyé.
 * p_i = svinv[ md( sv[c_i] + s1*sv[Ka[i%la]] + s2*sv[Kb[i%lb]] ) ], signes s1,s2 ∈ {-1,+1}.
 * Période effective = LCM(la,lb) ⇒ IC plat. Déterministe, public, 1:1, polyalpha. Enumérable.
 * Usage: double_vig qg CT sigmaAlpha  (mots-clés en dur; sweep paires×signes) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 97
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static int md(int x){x%=26;return x<0?x+26:x;}
static float*QG;
static const char*W[]={"PALIMPSEST","ABSCISSA","KRYPTOS","IQLUSION","SHADOW","LUCID","MEMORY",
"BERLIN","CLOCK","NORTHEAST","EASTNORTHEAST","SANBORN","SCHEIDT","LANGLEY","WELTZEITUHR","INVISIBLE",
"SHADING","NUANCE","MAGNETIC","DIGETAL","INTERPRETATION","POSITION","UNDERGROUND","LODESTONE","COMPASS",
"GIRASOL","TENTATIVELY","IDBYROWS","WHOKNOWS","BERLINCLOCK"};
static int NW=sizeof(W)/sizeof(W[0]);
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b2="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b2[i]-'A';}
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  const char*cts=argv[2];int ct[N];for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
  const char*sa=argv[3];int sv[26],svinv[26];for(int i=0;i<26;i++)sv[sa[i]-'A']=i;for(int i=0;i<26;i++)svinv[sv[i]]=i;
  double best=-1e9;char bpt[N+1];int bm=0;char ba[32],bb[32];int bs1=0,bs2=0;
  for(int A=0;A<NW;A++)for(int B=0;B<NW;B++){
    int la=strlen(W[A]),lb=strlen(W[B]);
    for(int s1=-1;s1<=1;s1+=2)for(int s2=-1;s2<=1;s2+=2){
      int pt[N];
      for(int i=0;i<N;i++){int ka=sv[W[A][i%la]-'A'],kb=sv[W[B][i%lb]-'A'];
        int pv=md(sv[ct[i]]+s1*ka+s2*kb);pt[i]=svinv[pv];}
      int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
      double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
      double q=no?qo/no:0;double sc=q+0.4*m;
      if(sc>best){best=sc;bm=m;bs1=s1;bs2=s2;strcpy(ba,W[A]);strcpy(bb,W[B]);for(int i=0;i<N;i++)bpt[i]='A'+pt[i];bpt[N]=0;}
    }
  }
  printf("best Ka=%s(%+d) Kb=%s(%+d) cribs=%d/24 PT=%s\n",ba,bs1,bb,bs2,bm,bpt);
  return 0;
}
