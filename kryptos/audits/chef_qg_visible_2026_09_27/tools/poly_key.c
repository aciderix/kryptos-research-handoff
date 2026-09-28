/* poly_key.c — clé POLYNOMIALE k_i=(a·i^2+b·i+c) mod26 (deg2) et (d·i^3+...) deg3. Peu de dof, apériodique.
 * Vigenère/Beau/var, alphabet public. Déterministe → énumérable → décisif. Usage: poly_key qg CT sigma deg */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
 int deg=atoi(argv[4]);
 int i2[N],i3[N];for(int i=0;i<N;i++){i2[i]=(i*i)%26;i3[i]=(i*i%26*i)%26;}
 double best=-1e9;int bm=0,ba=0,bb=0,bc=0,bd=0,bf=0;char bpt[N+1];
 int DMAX=(deg>=3)?26:1;
 for(int form=0;form<3;form++)
 for(int d=0;d<DMAX;d++)for(int a=0;a<26;a++)for(int b=0;b<26;b++)for(int c=0;c<26;c++){
   int pt[N];
   for(int i=0;i<N;i++){int kv=md(d*i3[i]+a*i2[i]+b*i+c);int cv=sv[ct[i]],pv;
     if(form==0)pv=md(cv-kv);else if(form==1)pv=md(kv-cv);else pv=md(cv+kv);pt[i]=svinv[pv];}
   int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
   double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int k=0;k<4;k++)in|=iscrib[i+k];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
   double q=no?qo/no:0;double sc=q+0.4*m;
   if(sc>best){best=sc;bm=m;ba=a;bb=b;bc=c;bd=d;bf=form;for(int i=0;i<N;i++)bpt[i]='A'+pt[i];bpt[N]=0;}
 }
 double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int k=0;k<4;k++)in|=iscrib[i+k];if(!in){qo+=QG[((bpt[i]-'A')*17576+(bpt[i+1]-'A')*676+(bpt[i+2]-'A')*26+(bpt[i+3]-'A'))];no++;}}
 printf("deg%d best d=%d a=%d b=%d c=%d form=%d cribs=%d/24 qoff=%.4f PT=%s\n",deg,bd,ba,bb,bc,bf,bm,no?qo/no:0,bpt);
 return 0;}
