/* route_solve.c — BLIND rigorous test of: K4 = grid-route transposition + Quagmire-III(KRYPTOS) keyword.
 * Convention LOCKED on K1 (id=1): keyletter = KAL[(posK(C)-posK(P)) mod26]; decrypt P: posK(P)=posK(W)-sK(key).
 * Model: working text W[j]=C[route_source[j]] (route un-does Sanborn's grid gesture). Plaintext[j]=deQ(W[j],kw[j mod L]).
 * Anti-pareidolia: the EAST crib (13 consecutive) PINS the keyword for L<=13; the BERLIN crib (11) must then be
 * CONSISTENT (require >=10/11) — a strong filter no random route passes. Then decrypt all & score qg_big.
 * NULL control: many random permutations through the same pipeline. Usage: route_solve qg.bin
 */
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
static uint64_t rs=0xDEADBEEFCAFEULL; static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
/* cribs */
static int cribpos[24],cribP[24];
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";int n=0;
 for(int i=0;i<13;i++){cribpos[n]=21+i;cribP[n]=e[i]-'A';n++;}
 for(int i=0;i<11;i++){cribpos[n]=63+i;cribP[n]=b[i]-'A';n++;}}
static int iscrib(int p){for(int q=0;q<24;q++)if(cribpos[q]==p)return 1;return 0;}
static double qg(const int*p){double s=0;int c=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)if(iscrib(i+d))in=0;/*count all*/ s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];c++;}return s/c;}
static double qg_offcrib(const int*p){double s=0;int c=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)if(iscrib(i+d))in=1;if(!in){s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];c++;}}return c?s/c:0;}

/* build route source-index list for a grid width W, read mode; returns length (should be N) */
static int build_route(int W,int mode,int*src){
  int H=(N+W-1)/W; int idx=0;
  /* grid[r][c] = C-index r*W+c if <N else -1 */
  switch(mode){
    case 0: for(int i=0;i<N;i++)src[idx++]=i; break;                              /* identity */
    case 1: for(int c=0;c<W;c++)for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;} break; /* col top-down */
    case 2: for(int c=0;c<W;c++)for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;} break; /* col bottom-up (K3) */
    case 3: for(int r=H-1;r>=0;r--)for(int c=0;c<W;c++){int p=r*W+c;if(p<N)src[idx++]=p;} break; /* row bottom-up */
    case 4: for(int c=0;c<W;c++){if(c&1)for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;} else for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;}} break; /* boustrophedon cols */
    case 5: for(int c=W-1;c>=0;c--)for(int r=0;r<H;r++){int p=r*W+c;if(p<N)src[idx++]=p;} break; /* col right-to-left top-down */
    case 6: for(int r=0;r<H;r++)for(int c=W-1;c>=0;c--){int p=r*W+c;if(p<N)src[idx++]=p;} break; /* row reversed (mirror) */
    case 7: for(int c=W-1;c>=0;c--)for(int r=H-1;r>=0;r--){int p=r*W+c;if(p<N)src[idx++]=p;} break; /* 180 rotation-ish */
  }
  return idx;
}
/* given working text W[], try to solve keyword by EAST, check BERLIN, decrypt & score.
 * returns berlin-match count; fills best. */
static int try_route(const int*Wt,int L,int*outpt,double*qoff){
  int kw[32]; int pinned[32]; for(int i=0;i<L;i++)pinned[i]=-1;
  /* pin from ALL cribs; detect inconsistency */
  for(int q=0;q<24;q++){int j=cribpos[q]; int need=md(sK[Wt[j]]-sK[cribP[q]]); int r=j%L;
     if(pinned[r]<0)pinned[r]=need; else if(pinned[r]!=need) return -1; /* inconsistent */ }
  for(int r=0;r<L;r++){ if(pinned[r]<0) return -2; kw[r]=pinned[r]; } /* need all residues pinned */
  /* decrypt all */
  for(int i=0;i<N;i++){int pk=md(sK[Wt[i]]-kw[i%L]); outpt[i]=KAL[pk]-'A';}
  /* berlin match (already forced by pin, but recount as sanity) + qoff */
  int m=0;const char*b="BERLINCLOCK";for(int i=0;i<11;i++)if(outpt[63+i]==b[i]-'A')m++;
  *qoff=qg_offcrib(outpt);
  return m;
}
int main(int argc,char**argv){
  for(int i=0;i<26;i++)sK[KAL[i]-'A']=i; setcribs();
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976){fprintf(stderr,"qg load fail\n");return 2;}fclose(f);
  int C[N];for(int i=0;i<N;i++)C[i]=K4[i]-'A';
  int Ws[]={7,14,4,21,31,2,13}; int nW=sizeof(Ws)/sizeof(*Ws);
  double best=-1e9; int bestpt[N],bestW=0,bestmode=0,bestL=0,bestB=0;
  printf("=== REAL routes (width x mode x L), require all-crib consistent, report BERLIN match & qoff ===\n");
  for(int wi=0;wi<nW;wi++){int W=Ws[wi];
    for(int mode=0;mode<8;mode++){
      int src[N]; int len=build_route(W,mode,src); if(len!=N)continue;
      int Wt[N]; for(int j=0;j<N;j++)Wt[j]=C[src[j]];
      for(int L=1;L<=24;L++){
        int pt[N]; double qoff; int m=try_route(Wt,L,pt,&qoff);
        if(m<0)continue;
        if(m>=8 || qoff>-2.6){ printf("W=%2d mode=%d L=%2d BERLIN=%2d/11 qoff=%.3f%s\n",W,mode,L,m,qoff, qoff>-2.4?"  <== ENGLISH?":""); }
        if(qoff>best){best=qoff;memcpy(bestpt,pt,sizeof pt);bestW=W;bestmode=mode;bestL=L;bestB=m;}
      }
    }
  }
  printf("\nBEST: W=%d mode=%d L=%d BERLIN=%d/11 qoff=%.3f\nPT=",bestW,bestmode,bestL,bestB,best);
  for(int i=0;i<N;i++)putchar('A'+bestpt[i]); putchar('\n');
  /* NULL: random permutations */
  double nbest=-1e9; int T=20000;
  for(int t=0;t<T;t++){
    int src[N]; for(int i=0;i<N;i++)src[i]=i; for(int i=N-1;i>0;i--){int k=rnd()%(i+1);int tmp=src[i];src[i]=src[k];src[k]=tmp;}
    int Wt[N];for(int j=0;j<N;j++)Wt[j]=C[src[j]];
    for(int L=1;L<=13;L++){int pt[N];double qoff;int m=try_route(Wt,L,pt,&qoff);if(m<0)continue;if(qoff>nbest)nbest=qoff;}
  }
  printf("NULL (%d random routes) best qoff=%.3f  => real %s null by %.3f\n",T,nbest, best>nbest?"BEATS":"<=",best-nbest);
  return 0;
}
