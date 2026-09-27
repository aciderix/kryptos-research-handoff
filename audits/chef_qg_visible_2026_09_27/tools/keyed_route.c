/* keyed_route.c — BLIND test of the AUTHENTIC K3 gesture: KEYED COLUMNAR transposition (columns read in
 * the alphabetical-rank order of a keyword), width = keyword length, top-down AND bottom-up (K3 read bottom-up),
 * optionally applied TWICE (K3 was described as two grids). Then Quagmire-III(KRYPTOS) with a period-L key
 * pinned by the authentic cribs (EAST pins, BERLIN checks). Both models (transpose C, or transpose P). NULL.
 * Convention locked on K1 (id=1). Usage: keyed_route qg.bin */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define N 97
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char*KAL="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int sK[26]; static float*QG;
static int md(int x){x%=26;return x<0?x+26:x;}
static uint64_t rs=0xABC1DEF2ULL; static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static int cp[24],cpl[24];
static void setc(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";int n=0;
 for(int i=0;i<13;i++){cp[n]=21+i;cpl[n]=e[i]-'A';n++;}for(int i=0;i<11;i++){cp[n]=63+i;cpl[n]=b[i]-'A';n++;}}
static int iscrib(int p){for(int q=0;q<24;q++)if(cp[q]==p)return 1;return 0;}
static double qg_off(const int*p){double s=0;int c=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)if(iscrib(i+d))in=1;if(!in){s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];c++;}}return c?s/c:0;}

/* column rank order for a keyword: colorder[k] = which column is read k-th (stable sort by letter then pos) */
static void colorder(const char*kw,int W,int*ord){
  int used[64]={0};
  for(int k=0;k<W;k++){int bestc=-1;char bl=127;
    for(int c=0;c<W;c++)if(!used[c]&&kw[c]<bl){bl=kw[c];bestc=c;}
    /* pick leftmost among equals */
    for(int c=0;c<W;c++)if(!used[c]&&kw[c]==bl){bestc=c;break;}
    used[bestc]=1; ord[k]=bestc;}
}
/* keyed columnar read: fill text row-major into W-wide grid, read columns in ord order, each column top-down(dir0)/bottom-up(dir1).
 * returns src[] output->source index (length N). */
static int keyed_read(int W,const int*ord,int dir,int*src){
  int H=(N+W-1)/W,idx=0;
  for(int k=0;k<W;k++){int c=ord[k];
    if(dir==0)for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;}
    else for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;}}
  return idx;
}
/* compose: src2 = apply route (src) twice */
static void compose(const int*a,const int*b,int*out){for(int i=0;i<N;i++)out[i]=a[b[i]];}

/* Model1: W1[j]=C[src[j]]; plaintext[j]=deQ(W1[j],kw[j%L]). EAST pins kw, BERLIN checks. */
static int solveM1(const int*C,const int*src,int L,int*pt,double*qoff){
  int W1[N];for(int j=0;j<N;j++)W1[j]=C[src[j]];
  int kw[32];for(int r=0;r<L;r++)kw[r]=-1;
  for(int q=0;q<24;q++){int j=cp[q];int need=md(sK[W1[j]]-sK[cpl[q]]);int r=j%L;if(kw[r]<0)kw[r]=need;else if(kw[r]!=need)return -1;}
  for(int r=0;r<L;r++)if(kw[r]<0)return -2;
  for(int i=0;i<N;i++)pt[i]=KAL[md(sK[W1[i]]-kw[i%L])]-'A';
  int m=0;const char*b="BERLINCLOCK";for(int i=0;i<11;i++)if(pt[63+i]==b[i]-'A')m++;*qoff=qg_off(pt);return m;
}
int main(int argc,char**argv){
  for(int i=0;i<26;i++)sK[KAL[i]-'A']=i; setc();
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  int C[N];for(int i=0;i<N;i++)C[i]=K4[i]-'A';
  const char*kws[]={"KRYPTOS","PALIMPSEST","ABSCISSA","ORDERING","KRYPTOSABCDEFGHIJLMNQUVWXZ"};int nkw=5;
  double best=-1e9;int bestpt[N];char bkw[32];int bdir=0,bL=0,bdbl=0;int found=0;
  for(int w=0;w<nkw;w++){const char*kw=kws[w];int W=strlen(kw);if(W>31)W=31; int ord[32];colorder(kw,W,ord);
    for(int dir=0;dir<2;dir++){int src[N];if(keyed_read(W,ord,dir,src)!=N)continue;
      for(int dbl=0;dbl<2;dbl++){int use[N];if(dbl){compose(src,src,use);}else memcpy(use,src,sizeof use);
        for(int L=1;L<=24;L++){int pt[N];double qoff;int m=solveM1(C,use,L,pt,&qoff);if(m<0)continue;found++;
          if(m>=9&&L<24)printf("kw=%s W=%d dir=%d dbl=%d L=%d BERLIN=%d/11 qoff=%.3f%s\n",kw,W,dir,dbl,L,m,qoff,qoff>-2.4?" <==ENGLISH?":"");
          if(qoff>best&&L<24){best=qoff;memcpy(bestpt,pt,sizeof pt);strcpy(bkw,kw);bdir=dir;bL=L;bdbl=dbl;}}}}}
  printf("consistent(L<24) found=%d\n",found);
  if(best>-1e8){printf("BEST kw=%s dir=%d dbl=%d L=%d qoff=%.3f\nPT=",bkw,bdir,bdbl,bL,best);for(int i=0;i<N;i++)putchar('A'+bestpt[i]);putchar('\n');}
  else printf("no consistent L<24 solution for any keyed route\n");
  return 0;
}
