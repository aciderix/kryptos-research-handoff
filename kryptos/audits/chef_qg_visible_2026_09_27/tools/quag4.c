/* quag4.c — QUAGMIRE IV : alphabet clair PA (keyé kw1), alphabet chiffré CA (keyé kw2), clé-mot périodique.
 * shift_i = pos de key[i%L] dans A-Z. c_i = CA[(PA_idx(p_i)+shift_i)%26]. decrypt: p_i=PA[(CA_idx(c_i)-shift_i)%26].
 * Déterministe par (PA,CA,key). K1/K2 = Quagmire III ; IV = "one more step". Usage: quag4 qg CT PA CA key */
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
 const char*pa=argv[3],*ca=argv[4],*key=argv[5];
 int PAidx[26],CAidx[26]; for(int i=0;i<26;i++){PAidx[pa[i]-'A']=i;CAidx[ca[i]-'A']=i;}
 int L=strlen(key);
 int pt[N];
 for(int i=0;i<N;i++){int sh=key[i%L]-'A'; int idx=md(CAidx[ct[i]]-sh); pt[i]=pa[idx];}
 int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&(pt[i]-'A')==cribL[i])m++;
 double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int k=0;k<4;k++)in|=iscrib[i+k];if(!in){qo+=QG[(((pt[i]-'A')*26+(pt[i+1]-'A'))*26+(pt[i+2]-'A'))*26+(pt[i+3]-'A')];no++;}}
 printf("cribs=%d/24 qoff=%.4f PT=%s\n",m,no?qo/no:0,pt);
 return 0;}
