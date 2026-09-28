/* overlay_key.c — clé-grille géométrique: shift_i=(A*row+B*col+C) mod 26, alphabet σ public.
 * Subsume overlay tableau (tabula recta A=B=1), ID-BY-ROWS (B=0), clé-colonne (A=0), diagonales.
 * Déterministe/énumérable. Usage: overlay_key qg CT sigmaAlpha  (row,col lus de k4_rowcol.csv) */
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
static int row_[N],col_[N],ct[N];
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b2="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b2[i]-'A';}
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  FILE*g=fopen("k4_rowcol.csv","r");char ln[64];int n=0;
  while(fgets(ln,sizeof ln,g)){int r,c;char L;if(sscanf(ln,"%d,%d,%c",&r,&c,&L)==3){row_[n]=r;col_[n]=c;ct[n]=L-'A';n++;}}
  fclose(g);
  const char*sa=argv[2];int sv[26],svinv[26];for(int i=0;i<26;i++)sv[sa[i]-'A']=i;for(int i=0;i<26;i++)svinv[sv[i]]=i;
  double best=-1e9;int bm=0,bA=0,bB=0,bC=0,bf=0;char bpt[N+1];
  for(int form=0;form<3;form++)for(int A=-4;A<=4;A++)for(int B=-4;B<=4;B++)for(int C=0;C<26;C++){
    int pt[N];
    for(int i=0;i<N;i++){int kv=md(A*row_[i]+B*col_[i]+C);int cv=sv[ct[i]],pv;
      if(form==0)pv=md(cv-kv);else if(form==1)pv=md(kv-cv);else pv=md(cv+kv);pt[i]=svinv[pv];}
    int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
    double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
    double q=no?qo/no:0;double sc=q+0.4*m;
    if(sc>best){best=sc;bm=m;bA=A;bB=B;bC=C;bf=form;for(int i=0;i<N;i++)bpt[i]='A'+pt[i];bpt[N]=0;}
  }
  printf("best A=%d B=%d C=%d form=%d cribs=%d/24 qoff-incl PT=%s\n",bA,bB,bC,bf,bm,bpt);
  return 0;
}
