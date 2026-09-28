/* gapscan.c — balaie l'ÉCART g de l'autoclé (chaînes mod g, g amorces kappa) et mesure, pour
 * CHAQUE g, la variété crib-EXACTE : dimension (33-rang TIE / (52+g)-rang INDEP) et nb de paires
 * de lettres d'alphabet FORCÉES égales (=> permutation impossible). But : g=7 (|KRYPTOS|) est-il
 * spécial ? Un petit g (peu d'amorces) => variété plus contrainte (dim basse). Si un g donne une
 * variété BASSE dimension AVEC permutations possibles => candidat à énumérer/résoudre.
 *
 * Modèle : x_i = tau(c_i) - k_i ; k_i = kappa_{i%g} si i<g sinon x_{i-g}. crib : sig(v)=x_i.
 *   x_i = Σ_{m=0..j} (-1)^m tau(c_{i-mg}) + (-1)^{j+1} kappa_{i%g}, j=i/g. (coeffs ±1)
 * TIE : sig=tau=pi (26+g vars). INDEP : tau,kappa,sig séparés (52+g vars).
 * Usage : gapscan
 */
#include <stdio.h>
#include <string.h>
#define MAXV 100
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int cribpos[24],cribval[24];
static int C[97];
static inline int md(int x){x%=26;return x<0?x+26:x;}
static int inv_modp(int a,int p){a%=p;if(a<0)a+=p;for(int x=1;x<p;x++)if(a*x%p==1)return x;return 0;}
static int NV, TIE, GAP;
/* colonnes : tau/pi 0..25 ; kappa 26..26+GAP-1 ; (INDEP) sig 26+GAP..51+GAP */
#define KAP(r) (26+(r))
#define SIG(z) (26+GAP+(z))
static void build_eq(int *row,int i,int v){
  memset(row,0,sizeof(int)*NV);
  if(TIE) row[v]=md(row[v]+1); else row[SIG(v)]=md(row[SIG(v)]+1);
  int r=i%GAP,j=i/GAP,s=1;
  for(int m=0,pos=i;m<=j;m++,pos-=GAP){ row[C[pos]]=md(row[C[pos]]-s); s=-s; }
  int ks=((j+1)&1)?-1:1; row[KAP(r)]=md(row[KAP(r)]-ks);
}
static int rank_modp(int rows[][MAXV],int nr,int p){
  static int A[400][MAXV]; for(int i=0;i<nr;i++)for(int c=0;c<NV;c++)A[i][c]=((rows[i][c]%p)+p)%p;
  int rank=0;
  for(int c=0;c<NV && rank<nr;c++){ int pr=-1; for(int r=rank;r<nr;r++)if(A[r][c]){pr=r;break;} if(pr<0)continue;
    for(int k=0;k<NV;k++){int t=A[rank][k];A[rank][k]=A[pr][k];A[pr][k]=t;}
    int iv=inv_modp(A[rank][c],p); for(int k=0;k<NV;k++)A[rank][k]=A[rank][k]*iv%p;
    for(int r=0;r<nr;r++)if(r!=rank&&A[r][c]){int f=A[r][c];for(int k=0;k<NV;k++)A[r][k]=((A[r][k]-f*A[rank][k])%p+p)%p;}
    rank++; }
  return rank;
}
/* nb de colonnes qui APPARAISSENT (coeff nonzero dans au moins une ligne) */
static int nappear(int rows[][MAXV],int nr){int ap[MAXV]={0};for(int i=0;i<nr;i++)for(int c=0;c<NV;c++)if(rows[i][c])ap[c]=1;int n=0;for(int c=0;c<NV;c++)n+=ap[c];return n;}
static int forced_pairs(int S[][MAXV],int n,int c0,int r2,int r13){
  int cnt=0; for(int a=0;a<26;a++)for(int b=a+1;b<26;b++){
    static int R[400][MAXV]; for(int i=0;i<n;i++)memcpy(R[i],S[i],sizeof(int)*NV);
    memset(R[n],0,sizeof(int)*NV); R[n][c0+a]=1; R[n][c0+b]=md(-1);
    if(rank_modp(R,n+1,2)==r2 && rank_modp(R,n+1,13)==r13) cnt++; }
  return cnt;
}
int main(void){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  int n=0; for(int i=0;i<13;i++){cribpos[n]=CE[i];cribval[n]=e[i]-'A';n++;} for(int i=0;i<11;i++){cribpos[n]=CB[i];cribval[n]=b[i]-'A';n++;}
  for(int i=0;i<97;i++)C[i]=K4[i]-'A';
  printf("g | TIE: cols dim2 dim13 forcedPI | INDEP: cols dim2 dim13 forcedTAU forcedSIG\n");
  printf("--+-------------------------------+----------------------------------------\n");
  for(GAP=1;GAP<=14;GAP++){
    static int S[400][MAXV]; int nr;
    /* TIE */
    TIE=1; NV=26+GAP; nr=0; for(int a=0;a<24;a++){build_eq(S[nr],cribpos[a],cribval[a]);nr++;}
    int ap_t=nappear(S,nr),r2t=rank_modp(S,nr,2),r13t=rank_modp(S,nr,13);
    int ft=forced_pairs(S,nr,0,r2t,r13t);
    int dt2=ap_t-r2t, dt13=ap_t-r13t;
    /* INDEP */
    TIE=0; NV=52+GAP; nr=0; for(int a=0;a<24;a++){build_eq(S[nr],cribpos[a],cribval[a]);nr++;}
    int ap_i=nappear(S,nr),r2i=rank_modp(S,nr,2),r13i=rank_modp(S,nr,13);
    int fs=forced_pairs(S,nr,SIG(0),r2i,r13i); int fta=forced_pairs(S,nr,0,r2i,r13i);
    int di2=ap_i-r2i;
    printf("%2d| %4d  %3d   %3d    %3d      | %4d  %3d   %3d    %3d      %3d\n",
      GAP, ap_t,dt2,dt13,ft, ap_i,di2,ap_i-r13i,fta,fs);
  }
  printf("\nLecture : forcedPI>0 (TIE) ou forcedTAU/SIG>0 (INDEP) => permutation IMPOSSIBLE a cet ecart.\n");
  printf("dim basse AVEC forced=0 => variete contrainte & non vide => candidat a enumerer.\n");
  return 0;
}
