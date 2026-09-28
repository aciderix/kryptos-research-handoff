/* quag2.c — test DÉTERMINISTE : K4 = autoclé écart-7 à DEUX alphabets FIXES (σ,τ) ?
 *
 * Structure exploitée : à lag 7, le clair se scinde en 7 chaînes (position mod 7),
 * chacune ENTIÈREMENT déterminée par UNE amorce kappa_c. Or CHAQUE chaîne contient
 * des cribs (21-33 et 63-73 couvrent les 7 résidus). Donc pour (σ,τ) donnés :
 *   - pour chaque chaîne c, on essaie les 26 kappa_c, on garde celui qui maximise le
 *     nb de cribs corrects de la chaîne. Si (σ,τ) est le bon couple, TOUTES les cribs
 *     d'une chaîne s'accordent sur un même kappa_c (cohérence = validation interne).
 *   - sinon, conflits -> couple réfuté.
 * Puis on déchiffre tout et on score au quadgramme fort. Test rapide, fiable, conclusif.
 *
 * Modèle (= ac7) : tau[ct_i] = x_i + k_i ; k_i = kappa_i (i<7) sinon x_{i-7} ; pt_i=siginv[x_i].
 * décryptage chaîne : x_c=tau[ct_c]-kappa_c ; x_{c+7}=tau[ct_{c+7}]-x_c ; ...
 *
 * Compile: cc -O2 -o quag2 quag2.c -lm
 * Usage:   quag2 qg.bin CT sigmaAlpha tauAlpha
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 97
#define LAG 7
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N],ncrib;
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";ncrib=0;
 for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';ncrib++;}
 for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';ncrib++;}}
static int md(int x){x%=26;return x<0?x+26:x;}
static float*QG;
int main(int argc,char**argv){
  if(argc<5){fprintf(stderr,"usage: quag2 qg CT sigma tau\n");return 1;}
  setcribs();
  FILE*f=fopen(argv[1],"rb");QG=malloc(sizeof(float)*456976);if(fread(QG,sizeof(float),456976,f)!=456976)return 2;fclose(f);
  const char*cts=argv[2]; int ct[N]; for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
  const char*sa=argv[3],*ta=argv[4];
  int sig[26],siginv[26],tau[26];
  for(int i=0;i<26;i++){ sig[sa[i]-'A']=i; tau[ta[i]-'A']=i; }
  for(int i=0;i<26;i++) siginv[sig[i]]=i;
  int x[N]; int totcrib=0; int chainconf=0;
  for(int c=0;c<LAG;c++){
    /* positions de la chaîne */
    int pos[32],np=0; for(int i=c;i<N;i+=LAG) pos[np++]=i;
    int bestk=0,bestm=-1,bestconf=99;
    for(int k=0;k<26;k++){
      int xc,prev=0,m=0,conf=0;
      for(int j=0;j<np;j++){int i=pos[j];
        xc = (j==0)? md(tau[ct[i]]-k) : md(tau[ct[i]]-prev);
        prev=xc; int p=siginv[xc];
        if(iscrib[i]){ if(p==cribL[i]) m++; else conf++; }
      }
      if(m>bestm || (m==bestm&&conf<bestconf)){bestm=m;bestk=k;bestconf=conf;}
    }
    /* applique bestk */
    int prev=0; for(int j=0;j<np;j++){int i=pos[j]; int xc=(j==0)?md(tau[ct[i]]-bestk):md(tau[ct[i]]-prev); prev=xc; x[i]=xc;}
    totcrib+=bestm; chainconf+=bestconf;
  }
  int pt[N]; for(int i=0;i<N;i++)pt[i]=siginv[x[i]];
  double qo=0;int no=0; for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d]; if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
  printf("cribs=%d/%d chainconf=%d qoff=%.4f PT=",totcrib,ncrib,chainconf,no?qo/no:0);
  for(int i=0;i<N;i++)putchar('A'+pt[i]); putchar('\n');
  return 0;
}
