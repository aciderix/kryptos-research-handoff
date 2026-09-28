/* bijcheck.c — la variété crib-EXACTE de l'autoclé écart-7 contient-elle une PERMUTATION ?
 * Test algébrique (sans recherche) : une égalité alph[a]=alph[b] (a!=b) est IMPLIQUÉE par les
 * cribs ssi ajouter (e_a - e_b) ne change ni le rang mod2 ni le rang mod13. Si UNE seule paire
 * est forcée => aucune permutation => modèle IMPOSSIBLE (avec ce jeu de cribs).
 *
 * Modes :
 *   TIE   : sigma=tau=pi (33 vars = 26 pi + 7 kappa).  On teste pi[a]=pi[b].
 *   INDEP : sigma,tau libres (59 vars = 26 tau + 7 kappa + 26 sig).  On teste sig[a]=sig[b] ET tau[a]=tau[b].
 * Pour chaque mode : cribs complets (drop 0), puis balayage drop-1 (24) et drop-2 (276) pour voir
 * si retirer des cribs (régime d'erreur Sanborn) rend une permutation possible.
 *
 * Usage : bijcheck [tie|indep]     (défaut : les deux)
 */
#include <stdio.h>
#include <string.h>
#define MAXV 59
#define TIE_NV 33
#define IND_NV 59
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int cribpos[24],cribval[24];
static int C[97];
static inline int md(int x){x%=26;return x<0?x+26:x;}
static int inv_modp(int a,int p){a%=p;if(a<0)a+=p;for(int x=1;x<p;x++)if(a*x%p==1)return x;return 0;}

/* variables : TIE : 0..25 pi, 26..32 kappa.
 *             INDEP: 0..25 tau, 26..32 kappa, 33..58 sig.  x_i utilise tau ; crib pin sig. */
static int TIE=1, NV=33;
#define KAP(r) (26+(r))
#define SIG(z) (33+(z))
/* construit une ligne crib (pos i, lettre v) dans row[NV] */
static void build_eq(int *row,int i,int v){
  memset(row,0,sizeof(int)*NV);
  if(TIE) row[v]=md(row[v]+1); else row[SIG(v)]=md(row[SIG(v)]+1);
  int r=i%7,j=i/7,s=1;
  for(int m=0,pos=i;m<=j;m++,pos-=7){ row[C[pos]]=md(row[C[pos]]-s); s=-s; } /* tau (=col c) */
  int ks=((j+1)&1)?-1:1; row[KAP(r)]=md(row[KAP(r)]-ks);
}
static int rank_modp(int rows[][MAXV],int nr,int p){
  static int A[320][MAXV]; for(int i=0;i<nr;i++)for(int c=0;c<NV;c++)A[i][c]=((rows[i][c]%p)+p)%p;
  int rank=0;
  for(int c=0;c<NV && rank<nr;c++){ int pr=-1; for(int r=rank;r<nr;r++)if(A[r][c]){pr=r;break;} if(pr<0)continue;
    for(int k=0;k<NV;k++){int t=A[rank][k];A[rank][k]=A[pr][k];A[pr][k]=t;}
    int iv=inv_modp(A[rank][c],p); for(int k=0;k<NV;k++)A[rank][k]=A[rank][k]*iv%p;
    for(int r=0;r<nr;r++)if(r!=rank&&A[r][c]){int f=A[r][c];for(int k=0;k<NV;k++)A[r][k]=((A[r][k]-f*A[rank][k])%p+p)%p;}
    rank++; }
  return rank;
}
/* remplit S avec les lignes crib des positions NON droppées (drop = masque 24 bits) ; renvoie count */
static int fill(int S[][MAXV],int dropmask){
  int n=0; for(int a=0;a<24;a++){ if(dropmask&(1<<a))continue; build_eq(S[n],cribpos[a],cribval[a]); n++; } return n;
}
/* nb de paires de lettres forcées égales dans un ensemble donné de colonnes-alphabet [c0,c0+26) */
static int forced_pairs(int S[][MAXV],int n,int c0,int r2,int r13){
  int cnt=0;
  for(int a=0;a<26;a++)for(int b=a+1;b<26;b++){
    static int R[320][MAXV]; for(int i=0;i<n;i++)memcpy(R[i],S[i],sizeof(int)*NV);
    memset(R[n],0,sizeof(int)*NV); R[n][c0+a]=1; R[n][c0+b]=md(-1);
    if(rank_modp(R,n+1,2)==r2 && rank_modp(R,n+1,13)==r13) cnt++;
  }
  return cnt;
}
/* renvoie 0 si une permutation est possible (0 paire forcée sur chaque alphabet), sinon >0 */
static int infeasible(int dropmask,int*out_detail){
  static int S[320][MAXV]; int n=fill(S,dropmask);
  int r2=rank_modp(S,n,2), r13=rank_modp(S,n,13);
  int bad=0;
  if(TIE){ bad = forced_pairs(S,n,0,r2,r13); }
  else { int bt=forced_pairs(S,n,0,r2,r13); int bs=forced_pairs(S,n,SIG(0),r2,r13); bad=bt+bs; if(out_detail){out_detail[0]=bt;out_detail[1]=bs;} }
  return bad;
}
static void run_mode(void){
  printf("\n===== MODE %s (NV=%d) =====\n",TIE?"TIE (sigma=tau)":"INDEP (sigma,tau libres)",NV);
  int det[2];
  int bad0=infeasible(0,det);
  if(TIE) printf("drop 0 : paires pi forcées égales = %d => %s\n",bad0,bad0?"PERMUTATION IMPOSSIBLE":"permutation possible");
  else    printf("drop 0 : forcées tau=%d sig=%d (total %d) => %s\n",det[0],det[1],bad0,bad0?"PERMUTATION IMPOSSIBLE":"permutation possible");
  if(bad0==0){ printf("  (variété non vide en permutations : recherche/énumération pertinente)\n"); return; }
  /* drop-1 : un crib retiré suffit-il ? */
  int best1=999,arg1=-1; for(int d=0;d<24;d++){int bb=infeasible(1<<d,det); if(bb<best1){best1=bb;arg1=d;}}
  printf("drop 1 (meilleur : retirer crib pos %d) : %d paires forcées => %s\n",cribpos[arg1],best1,best1?"toujours impossible":"PERMUTATION POSSIBLE avec 23/24 cribs");
  /* drop-2 : deux cribs retirés suffisent-ils ? */
  int best2=999,ai=-1,aj=-1; for(int d1=0;d1<24;d1++)for(int d2=d1+1;d2<24;d2++){int bb=infeasible((1<<d1)|(1<<d2),det); if(bb<best2){best2=bb;ai=d1;aj=d2;}}
  printf("drop 2 (meilleur : retirer pos %d,%d) : %d paires forcées => %s\n",cribpos[ai],cribpos[aj],best2,best2?"toujours impossible":"PERMUTATION POSSIBLE avec 22/24 cribs");
}
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  int n=0; for(int i=0;i<13;i++){cribpos[n]=CE[i];cribval[n]=e[i]-'A';n++;} for(int i=0;i<11;i++){cribpos[n]=CB[i];cribval[n]=b[i]-'A';n++;}
  for(int i=0;i<97;i++)C[i]=K4[i]-'A';
  int doTie=1,doInd=1; if(argc>1){ if(!strcmp(argv[1],"tie"))doInd=0; if(!strcmp(argv[1],"indep"))doTie=0; }
  if(doTie){ TIE=1; NV=TIE_NV; run_mode(); }
  if(doInd){ TIE=0; NV=IND_NV; run_mode(); }
  return 0;
}
