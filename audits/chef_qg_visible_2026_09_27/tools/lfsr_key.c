/* lfsr_key.c — clé LONGUE générée par récurrence linéaire sur Z26 (graine courte mémorisable).
 * k_i = (a*k_{i-1}+b*k_{i-2}) mod 26, graine (k0,k1). Peu de dof (a,b,graine) ⇒ DÉCIDABLE.
 * Vigenère/Beau/var, alphabet public σ. Déterministe par (a,b,k0,k1,σ,form) ⇒ cribs+qg+NUL.
 * Usage: lfsr_key qg CT sigmaAlpha  (sweep a,b,k0,k1,form interne) ; option env NULLMODE=1 -> random CT stats */
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
int main(int argc,char**argv){
 const char*e="EASTNORTHEAST",*b2="BERLINCLOCK";
 for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
 for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b2[i]-'A';}
 FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
 const char*cts=argv[2];int ct[N];for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
 const char*sa=argv[3];int sv[26],svinv[26];for(int i=0;i<26;i++)sv[sa[i]-'A']=i;for(int i=0;i<26;i++)svinv[sv[i]]=i;
 double best=-1e9;int bm=0,ba=0,bb=0,bk0=0,bk1=0,bf=0;char bpt[N+1];
 for(int form=0;form<3;form++)
 for(int a=0;a<26;a++)for(int b=0;b<26;b++)
 for(int k0=0;k0<26;k0++)for(int k1=0;k1<26;k1++){
   int k[N];k[0]=k0;k[1]=k1;for(int i=2;i<N;i++)k[i]=md(a*k[i-1]+b*k[i-2]);
   int pt[N];
   for(int i=0;i<N;i++){int cv=sv[ct[i]],pv; if(form==0)pv=md(cv-k[i]);else if(form==1)pv=md(k[i]-cv);else pv=md(cv+k[i]); pt[i]=svinv[pv];}
   int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
   double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
   double q=no?qo/no:0;double sc=q+0.4*m;
   if(sc>best){best=sc;bm=m;ba=a;bb=b;bk0=k0;bk1=k1;bf=form;for(int i=0;i<N;i++)bpt[i]='A'+pt[i];bpt[N]=0;}
 }
 double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((bpt[i]-'A')*26*26*26+(bpt[i+1]-'A')*26*26+(bpt[i+2]-'A')*26+(bpt[i+3]-'A'))];no++;}}
 printf("best a=%d b=%d k0=%d k1=%d form=%d cribs=%d/24 qoff=%.4f PT=%s\n",ba,bb,bk0,bk1,bf,bm,no?qo/no:0,bpt);
 return 0;}
