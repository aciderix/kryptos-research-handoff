/* route_solve2.c — BLIND Model 2: C = Quagmire_enc( route(P) ). Decrypt: M=deQ(C,kw); P[j]=M[src[j]].
 * keyword applies in CIPHERTEXT position space. For crib output-position j, C-position q=src[j];
 * kw[q mod L] pinned = KAL[(posK(C[q])-posK(crib[j]))]. Require all-crib consistent + all L residues pinned,
 * then decrypt fully and score qg_big. Expanded routes incl. 90-deg rotations. NULL = random perms.
 * Convention locked on K1 (id=1). Usage: route_solve2 qg.bin */
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
static uint64_t rs=0x1234ABCDULL; static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static int cp[24],cpl[24];
static void setc(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";int n=0;
 for(int i=0;i<13;i++){cp[n]=21+i;cpl[n]=e[i]-'A';n++;}for(int i=0;i<11;i++){cp[n]=63+i;cpl[n]=b[i]-'A';n++;}}
static int iscrib(int p){for(int q=0;q<24;q++)if(cp[q]==p)return 1;return 0;}
static double qg_off(const int*p){double s=0;int c=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)if(iscrib(i+d))in=1;if(!in){s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];c++;}}return c?s/c:0;}
static int build_route(int W,int mode,int*src){
  int H=(N+W-1)/W; int idx=0;
  switch(mode){
    case 0: for(int i=0;i<N;i++)src[idx++]=i; break;
    case 1: for(int c=0;c<W;c++)for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;} break;
    case 2: for(int c=0;c<W;c++)for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;} break;
    case 3: for(int r=H-1;r>=0;r--)for(int c=0;c<W;c++){int p=r*W+c;if(p<N)src[idx++]=p;} break;
    case 4: for(int c=0;c<W;c++){if(c&1)for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;}else for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;}} break;
    case 5: for(int c=W-1;c>=0;c--)for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;} break;
    case 6: for(int r=0;r<H;r++)for(int c=W-1;c>=0;c--){int p=r*W+c;if(p<N)src[idx++]=p;} break;
    case 7: for(int c=W-1;c>=0;c--)for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;} break;
    /* 90-deg rotations: read grid as if rotated (r,c)->(c, H-1-r) etc, over transposed dims */
    case 8: for(int r=0;r<H;r++)for(int c=0;c<W;c++){int p=c*H+r; if(p<N)src[idx++]=p;} break; /* transpose read */
  }
  return idx;
}
static int solve(const int*C,const int*src,int L,int*outpt,double*qoff){
  int kw[32]; for(int r=0;r<L;r++)kw[r]=-1;
  for(int q=0;q<24;q++){int j=cp[q]; int Cpos=src[j]; int need=md(sK[C[Cpos]]-sK[cpl[q]]); int r=Cpos%L;
     if(kw[r]<0)kw[r]=need; else if(kw[r]!=need)return -1;}
  for(int r=0;r<L;r++)if(kw[r]<0)return -2;
  int M[N]; for(int q=0;q<N;q++)M[q]=md(sK[C[q]]-kw[q%L]); /* M index in keyed space -> letter KAL[M] */
  for(int j=0;j<N;j++)outpt[j]=KAL[M[src[j]]]-'A';
  int m=0;const char*b="BERLINCLOCK";for(int i=0;i<11;i++)if(outpt[63+i]==b[i]-'A')m++;
  *qoff=qg_off(outpt); return m;
}
int main(int argc,char**argv){
  for(int i=0;i<26;i++)sK[KAL[i]-'A']=i; setc();
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  int C[N];for(int i=0;i<N;i++)C[i]=K4[i]-'A';
  int Ws[]={7,14,4,21,31,2,13}; int nW=7;
  double best=-1e9;int bestpt[N],bW=0,bm=0,bL=0,bMode=0; int found=0;
  for(int wi=0;wi<nW;wi++){int W=Ws[wi];for(int mode=0;mode<9;mode++){int src[N];if(build_route(W,mode,src)!=N)continue;
    for(int L=1;L<=24;L++){int pt[N];double qoff;int m=solve(C,src,L,pt,&qoff);if(m<0)continue;found++;
      if(m>=8||qoff>-2.6)printf("W=%d mode=%d L=%d BERLIN=%d/11 qoff=%.3f%s\n",W,mode,L,m,qoff,qoff>-2.4?" <==ENGLISH?":"");
      if(qoff>best){best=qoff;memcpy(bestpt,pt,sizeof pt);bW=W;bm=m;bL=L;bMode=mode;}}}}
  printf("consistent solutions found=%d\n",found);
  if(best>-1e8){printf("BEST W=%d mode=%d L=%d BERLIN=%d/11 qoff=%.3f\nPT=",bW,bMode,bL,bm,best);for(int i=0;i<N;i++)putchar('A'+bestpt[i]);putchar('\n');}
  double nb=-1e9;int T=40000;
  for(int t=0;t<T;t++){int src[N];for(int i=0;i<N;i++)src[i]=i;for(int i=N-1;i>0;i--){int k=rnd()%(i+1);int tmp=src[i];src[i]=src[k];src[k]=tmp;}
    for(int L=1;L<=13;L++){int pt[N];double qoff;int m=solve(C,src,L,pt,&qoff);if(m<0)continue;if(qoff>nb)nb=qoff;}}
  printf("NULL(%d) best qoff=%.3f => real %s null by %.3f\n",T,nb,best>nb?"BEATS":"<=",best-nb);
  return 0;
}
