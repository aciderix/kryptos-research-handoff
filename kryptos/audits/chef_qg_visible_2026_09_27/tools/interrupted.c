/* interrupted.c — Vigenère à CLÉ INTERROMPUE (disrupted): mot-clé + reset sur lettre-déclencheur.
 * Apériodique (IC plat), 1:1, mémorisable, « une étape de plus ». Interruptor sur le CHIFFRÉ (causal).
 * decrypt: j=0; shift=σ(K[j]); pval=(σ(c_i)-shift)%26 (Vig) / (shift-σ(c_i)) (Beau) / (σ(c_i)+shift)(var);
 *   après i: si c_i==interruptor → j=0 sinon j=(j+1)%L.
 * Usage: interrupted qg CT sigmaAlpha keyword  (sweep interruptor×form en interne) */
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
 const char*kw=argv[4];int L=strlen(kw);int kv[64];for(int j=0;j<L;j++)kv[j]=sv[kw[j]-'A'];
 double best=-1e9;int bm=0,bint=0,bf=0;char bpt[N+1];
 for(int form=0;form<3;form++)for(int intr=0;intr<26;intr++){
   int pt[N],j=0;
   for(int i=0;i<N;i++){int sh=kv[j];int cv=sv[ct[i]],pv;
     if(form==0)pv=md(cv-sh);else if(form==1)pv=md(sh-cv);else pv=md(cv+sh);
     pt[i]=svinv[pv];
     if(ct[i]==intr)j=0;else j=(j+1)%L;}
   int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
   double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
   double q=no?qo/no:0;double sc=q+0.3*m;
   if(sc>best){best=sc;bm=m;bint=intr;bf=form;for(int i=0;i<N;i++)bpt[i]='A'+pt[i];bpt[N]=0;}
 }
 printf("kw=%s bestINT=%c form=%d cribs=%d/24 PT=%s\n",kw,'A'+bint,bf,bm,bpt);
 return 0;}
