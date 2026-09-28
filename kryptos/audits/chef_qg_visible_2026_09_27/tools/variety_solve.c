/* variety_solve.c — recherche DANS la variété crib-EXACTE (2-alph autoclé écart-7), scorée qg_big.
 * Système homogène sur Z26 : pour chaque crib (pos i, clair v) : sig[v] - x_i = 0,
 *   x_i = Σ_{m>=0} (-1)^m tau[c_{i-7m}] - (-1)^M kappa_{i%7}  (M = i div 7).
 * Vars: 0..25=sig, 26..51=tau, 52..58=kappa (59). Noyau via Gauss mod 2 & mod 13, CRT par coord.
 * Points = CRT(Σ a_i B2_i mod2, Σ b_j B13_j mod13). Recuit sur (a,b) : chaque point = 24/24 cribs
 * EXACTS ; on décode pt=siginv[x] (pénalité si sig/tau non bijectifs) et on score qg_big.
 * Compile: cc -O2 -o variety_solve variety_solve.c -lm ; Usage: variety_solve qg.bin iters restarts seed
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define N 97
#define NV 59
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static int md(int x){x%=26;return x<0?x+26:x;}
static float*QG;
static uint64_t rs; static uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;} static double ur(void){return (rnd()>>11)*(1.0/9007199254740992.0);}

/* --- Gauss mod p : renvoie base du noyau (rows) dans ker[][NV], nb = kdim --- */
static int nullspace(int A[][NV],int nr,int p,int ker[][NV]){
  int M[64][NV]; for(int i=0;i<nr;i++)for(int j=0;j<NV;j++)M[i][j]=((A[i][j]%p)+p)%p;
  int pivcol[64],npiv=0; int row=0; int where[NV]; for(int j=0;j<NV;j++)where[j]=-1;
  for(int col=0;col<NV && row<nr;col++){
    int sel=-1; for(int i=row;i<nr;i++) if(M[i][col]%p){sel=i;break;}
    if(sel<0) continue;
    for(int j=0;j<NV;j++){int t=M[sel][j];M[sel][j]=M[row][j];M[row][j]=t;}
    /* normalize pivot to 1 */
    int inv=1; for(int x=1;x<p;x++) if((M[row][col]*x)%p==1){inv=x;break;}
    for(int j=0;j<NV;j++) M[row][j]=(M[row][j]*inv)%p;
    for(int i=0;i<nr;i++) if(i!=row && M[i][col]%p){int f=M[i][col];for(int j=0;j<NV;j++)M[i][j]=((M[i][j]-f*M[row][j])%p+p)%p;}
    where[col]=row; pivcol[npiv++]=col; row++;
  }
  int kd=0;
  for(int col=0;col<NV;col++) if(where[col]<0){ /* free var */
    for(int j=0;j<NV;j++)ker[kd][j]=0; ker[kd][col]=1;
    for(int pc=0;pc<npiv;pc++){int c=pivcol[pc];int r=where[c]; ker[kd][c]=((-M[r][col])%p+p)%p;}
    kd++;
  }
  return kd;
}
/* CRT coord: x≡a mod2, x≡b mod13 -> mod26 */
static int crt(int a,int b){ for(int x=0;x<26;x++) if(x%2==((a%2)+2)%2 && x%13==((b%13)+13)%13) return x; return 0; }

static double qg(const int*p){double s=0;for(int i=0;i+3<N;i++)s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];return s;}

int main(int argc,char**argv){
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=CE[i]?0:0;}
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  long IT=atol(argv[2]);int R=atoi(argv[3]);rs=0x9E3779B97F4A7C15ULL*(atol(argv[4])+1);
  int C[N]; for(int i=0;i<N;i++)C[i]=K4[i]-'A';
  /* build crib rows */
  int A[64][NV],nr=0;
  int cribpos[24],cribv[24],nc=0;
  for(int i=0;i<13;i++){cribpos[nc]=CE[i];cribv[nc]=e[i]-'A';nc++;}
  for(int i=0;i<11;i++){cribpos[nc]=CB[i];cribv[nc]=b[i]-'A';nc++;}
  for(int q=0;q<nc;q++){int i=cribpos[q],v=cribv[q];
    for(int j=0;j<NV;j++)A[nr][j]=0;
    A[nr][v]+=1;                 /* sig[v] */
    int M=i/7,jr=i%7;
    for(int m=0;m<=M;m++){int sign=(m%2==0)?1:-1; A[nr][26+C[i-7*m]] += -sign;} /* -(-1)^m tau[..] */
    int signM=(M%2==0)?1:-1; A[nr][52+jr] += signM; /* +(-1)^M kappa */
    nr++;
  }
  int B2[NV][NV],B13[NV][NV];
  int d2=nullspace(A,nr,2,B2), d13=nullspace(A,nr,13,B13);
  fprintf(stderr,"kernel dims: mod2=%d mod13=%d (points=2^%d*13^%d)\n",d2,d13,d2,d13);
  /* hill-climb over a in Z2^d2, b in Z13^d13 */
  int besta[NV],bestb[NV]; double best=-1e18; int bestpt[N];
  double LAM=6.0;
  for(int r=0;r<R;r++){
    int a[NV],bb[NV]; for(int i=0;i<d2;i++)a[i]=rnd()%2; for(int j=0;j<d13;j++)bb[j]=rnd()%13;
    double cur=-1e18; int havecur=0;
    double T0=4.0,T1=0.1;
    for(long it=0;it<IT;it++){
      /* build vector */
      int v2[NV],v13[NV]; for(int c=0;c<NV;c++){v2[c]=0;v13[c]=0;}
      for(int i=0;i<d2;i++) if(a[i]) for(int c=0;c<NV;c++) v2[c]^=B2[i][c];
      for(int j=0;j<d13;j++) if(bb[j]) for(int c=0;c<NV;c++) v13[c]=(v13[c]+bb[j]*0+B13[j][c]*bb[j])%13; /* placeholder */
      /* NOTE: proper: v13 = sum bb[j]*B13[j] mod13 */
      for(int c=0;c<NV;c++)v13[c]=0;
      for(int j=0;j<d13;j++) if(bb[j]) for(int c=0;c<NV;c++) v13[c]=(v13[c]+bb[j]*B13[j][c])%13;
      int sig[26],tau[26],kap[7];
      for(int L=0;L<26;L++) sig[L]=crt(v2[L],v13[L]);
      for(int L=0;L<26;L++) tau[L]=crt(v2[26+L],v13[26+L]);
      for(int j=0;j<7;j++) kap[j]=crt(v2[52+j],v13[52+j]);
      /* bijection penalty + siginv */
      int cntS[26],cntT[26]; for(int z=0;z<26;z++){cntS[z]=0;cntT[z]=0;}
      for(int L=0;L<26;L++){cntS[sig[L]]++;cntT[tau[L]]++;}
      int viol=0; for(int z=0;z<26;z++){if(cntS[z]==0)viol++;if(cntT[z]==0)viol++;}
      int siginv[26]; for(int z=0;z<26;z++)siginv[z]=0; for(int L=0;L<26;L++)siginv[sig[L]]=L;
      /* decode */
      int x[N],pt[N];
      for(int i=0;i<N;i++){int k=(i<7)?kap[i]:x[i-7]; x[i]=md(tau[C[i]]-k); pt[i]=siginv[x[i]];}
      double sc=qg(pt)-LAM*viol;
      if(!havecur){cur=sc;havecur=1;}
      double T=T0*pow(T1/T0,(double)it/IT);
      /* propose a move: flip one a or change one b */
      int mv=rnd()%(d2+d13); int old,idx; int isb=0;
      if(mv<d2){idx=mv;old=a[idx];a[idx]^=1;}
      else{isb=1;idx=mv-d2;old=bb[idx];bb[idx]=rnd()%13;}
      /* recompute quickly (full) */
      for(int c=0;c<NV;c++){v2[c]=0;v13[c]=0;}
      for(int i=0;i<d2;i++) if(a[i]) for(int c=0;c<NV;c++) v2[c]^=B2[i][c];
      for(int j=0;j<d13;j++) if(bb[j]) for(int c=0;c<NV;c++) v13[c]=(v13[c]+bb[j]*B13[j][c])%13;
      for(int L=0;L<26;L++) sig[L]=crt(v2[L],v13[L]);
      for(int L=0;L<26;L++) tau[L]=crt(v2[26+L],v13[26+L]);
      for(int j=0;j<7;j++) kap[j]=crt(v2[52+j],v13[52+j]);
      for(int z=0;z<26;z++){cntS[z]=0;cntT[z]=0;} for(int L=0;L<26;L++){cntS[sig[L]]++;cntT[tau[L]]++;}
      viol=0; for(int z=0;z<26;z++){if(cntS[z]==0)viol++;if(cntT[z]==0)viol++;}
      for(int z=0;z<26;z++)siginv[z]=0; for(int L=0;L<26;L++)siginv[sig[L]]=L;
      for(int i=0;i<N;i++){int k=(i<7)?kap[i]:x[i-7]; x[i]=md(tau[C[i]]-k); pt[i]=siginv[x[i]];}
      double nsc=qg(pt)-LAM*viol;
      if(nsc>=cur || ur()<exp((nsc-cur)/T)){ cur=nsc;
        if(nsc>best && viol==0){best=nsc; for(int i=0;i<N;i++)bestpt[i]=pt[i];} }
      else { if(isb) bb[idx]=old; else a[idx]=old; }
    }
  }
  if(best<-1e17){ printf("no bijective (perm) point reached in budget\n"); return 0; }
  double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((bestpt[i]*26+bestpt[i+1])*26+bestpt[i+2])*26+bestpt[i+3]];no++;}}
  int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&bestpt[i]==cribL[i])m++;
  printf("BEST(perm) cribs=%d/24 qoff=%.4f PT=",m,no?qo/no:0);
  for(int i=0;i<N;i++)putchar('A'+bestpt[i]); putchar('\n');
  return 0;
}
