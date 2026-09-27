/* tie_known_read.c — test DETERMINISTE (idee chef msg 21256fef) : sigma=tau FIXE a un alphabet
 * CONNU de l'oeuvre (AZ, KRYPTOS, PALIMPSEST, ABSCISSA), autocle ecart-7, kappa (7 amorces)
 * derive des cribs (majorite par residu => tolere les erreurs). On LIT le clair complet et on le
 * note au quadgram anglais. Si un clair est lisible (qoff ~ anglais) avec peu d'erreurs de crib
 * ET coherent avec le clair etendu (p8=A,p20=C,p39=N,p58=C,p83=C) => SOLUTION sans K5.
 * Le "drop 1 crib en 32/73" est capture par crib_errors<=1 (kappa majoritaire ignore l'intrus).
 * conv VIG/BEA/VAR x src PLAIN/CIPHER x 4 alphabets = 24 lectures deterministes.
 *
 * Usage : tie_known_read qg.bin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
#define LAG 7
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
/* clair etendu gratuit (theoreme coincidences) : positions sures conditionnelles au modele */
static const int EX_POS[5]={8,20,39,58,83}; static const char EX_LET[5]={'A','C','N','C','C'};
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static const char*KW[4]={"","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*BN[4]={"AZ","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*CN[3]={"VIG","BEAU","VAR"};static const char*SN[2]={"PLAIN","CIPHER"};
static void mkperm(int b,int*perm){int seen[26]={0},al[26],n=0;
  for(const char*p=KW[b];*p;p++){int L=*p-'A';if(!seen[L]){seen[L]=1;al[n++]=L;}}
  for(int L=0;L<26;L++)if(!seen[L])al[n++]=L; for(int i=0;i<26;i++)perm[al[i]]=i;}
static inline int fx(int c,int t,int g){if(c==0)return md(t-g);if(c==1)return md(g-t);return md(t+g);}
static float *QG;
static double qoff(const int*p){double q=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}}return no?q/no:0;}
/* decode sigma=tau=perm ; kappa majoritaire par residu ; renvoie erreurs crib, remplit pt */
static int decode(int conv,int src,const int*ct,const int*perm,int*pt){
  int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i; int err=0;
  for(int r=0;r<LAG;r++){
    int reqk[16],nreq=0,fix=0; int aprev=0,bprev=0; int aj[N],bj[N];
    for(int i=r,j=0;i<N;i+=LAG,j++){int t=perm[ct[i]];int a,b;
      if(j==0){if(conv==0){a=25;b=t;}else if(conv==1){a=1;b=md(-t);}else{a=1;b=t;}}
      else if(src==0){if(conv==0){a=md(-aprev);b=md(t-bprev);}else if(conv==1){a=aprev;b=md(bprev-t);}else{a=aprev;b=md(t+bprev);}}
      else{int g=perm[ct[i-LAG]];a=0;b=fx(conv,t,g);}
      aj[i]=a;bj[i]=b;aprev=a;bprev=b;
      if(iscrib[i]){int want=perm[cribL[i]]; if(a==0){if(md(b)!=want)fix++;} else reqk[nreq++]=md(a*(want-b));}
    }
    int bk=0,bc=0;for(int q=0;q<nreq;q++){int c=0;for(int p2=0;p2<nreq;p2++)if(reqk[p2]==reqk[q])c++;if(c>bc){bc=c;bk=reqk[q];}}
    err+=fix+(nreq-bc); for(int i=r;i<N;i+=LAG)pt[i]=inv[md(aj[i]*bk+bj[i])];
  }
  return err;
}
int main(int argc,char**argv){
  setcribs(); if(argc<2){fprintf(stderr,"usage: tie_known_read qg.bin\n");return 1;}
  FILE*f=fopen(argv[1],"rb");QG=malloc(sizeof(float)*456976);
  if(fread(QG,sizeof(float),456976,f)!=456976){fprintf(stderr,"qg\n");return 2;}fclose(f);
  int ct[N];for(int i=0;i<N;i++)ct[i]=K4[i]-'A';
  printf("sigma=tau CONNU, autocle ecart-7, kappa derive cribs. cible solution: qoff~-4.3, cribs 0-1.\n");
  printf("%-11s %-5s %-6s | cribErr | ex(5) | qoff | plaintext\n","alpha","conv","src");
  for(int b=0;b<4;b++){int perm[26];mkperm(b,perm);
    for(int conv=0;conv<3;conv++)for(int src=0;src<2;src++){
      int pt[N];int e=decode(conv,src,ct,perm,pt);
      int ex=0;for(int k=0;k<5;k++)if(pt[EX_POS[k]]==EX_LET[k]-'A')ex++;
      double q=qoff(pt);
      printf("%-11s %-5s %-6s |  %2d/24  |  %d/5  | %.2f | ",BN[b],CN[conv],SN[src],e,ex,q);
      for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');
    }}
  return 0;
}
