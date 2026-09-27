/* variety_dim.c — dimension de la variete crib-consistante de l'autocle ecart-7 Vigenere
 * (deux alphabets libres), et sa REDUCTION quand on injecte les contraintes de clair
 * derivees des coincidences ecart-7 (idee chef, publique) : c_i=c_{i+7} => p_{i-7}=p_{i+7}.
 *
 * Modele : x_i = sigma(p_i) ; x_i = SUM_{m=0..j} (-1)^m tau(c_{i-7m}) + (-1)^{j+1} kappa_r,
 *   avec r=i%7, j=i/7.  Inconnues : tau[0..25], kappa[0..6], sigma[0..25] (59 colonnes).
 * Contrainte "p_i = v connu"  : sigma(v) - SUM(-1)^m tau(c_{i-7m}) - (-1)^{j+1} kappa_r = 0.
 * Contrainte "p_a = p_b"      : x_a - x_b = 0 (sigma s'annule ; equation en tau,kappa).
 * dim de la variete = (#colonnes APPARAISSANT) - rang, calcule mod 2 et mod 13 (26=2*13).
 * On imprime dim a chaque etape cumulative pour voir la chute.
 *
 * Usage : variety_dim
 */
#include <stdio.h>
#include <string.h>
#define NC 59   /* 26 tau + 7 kappa + 26 sigma */
#define TAU(y) (y)
#define KAP(r) (26+(r))
#define SIG(z) (33+(z))
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int C[97];
#define MAXR 200
static int Mrow[MAXR][NC]; static int nrow=0;
static int appear[NC];

static void addrow(const int*row){ memcpy(Mrow[nrow],row,sizeof(int)*NC); nrow++; for(int c=0;c<NC;c++) if(row[c]%26) appear[c]=1; }

/* x_i coefficients dans row (ajoute), signe global sgn (+1 => +x_i, -1 => -x_i) */
static void add_xi(int*row,int i,int sgn){
  int r=i%7, j=i/7, s=1; /* s=(-1)^m */
  for(int m=0,pos=i; m<=j; m++,pos-=7){ row[TAU(C[pos])]=(row[TAU(C[pos])]+sgn*s+26)%26; s=-s; }
  int ks=((j+1)&1)? -1: 1; /* (-1)^{j+1} */
  row[KAP(r)]=(row[KAP(r)]+sgn*ks+26)%26;
}
static int TIE=0; /* si 1 : sigma=tau (Quagmire III) -> colonne sigma(z) fusionnee avec tau(z) */
static int SIGc(int z){ return TIE? TAU(z) : SIG(z); }
static void known(int i,int v){ int row[NC]; memset(row,0,sizeof row); row[SIGc(v)]=(row[SIGc(v)]+1)%26; add_xi(row,i,-1); addrow(row); }
static void equal(int a,int b){ int row[NC]; memset(row,0,sizeof row); add_xi(row,a,+1); add_xi(row,b,-1); addrow(row); }

/* rang mod p sur les colonnes qui apparaissent */
static int inv_modp(int a,int p){ a%=p; if(a<0)a+=p; for(int x=1;x<p;x++) if(a*x%p==1) return x; return 0; }
static int rank_modp(int p){
  int cols[NC],nc=0; for(int c=0;c<NC;c++) if(appear[c]) cols[nc++]=c;
  static int A[MAXR][NC]; for(int i=0;i<nrow;i++) for(int k=0;k<nc;k++) A[i][k]=((Mrow[i][cols[k]]%p)+p)%p;
  int rank=0;
  for(int k=0;k<nc && rank<nrow;k++){
    int piv=-1; for(int i=rank;i<nrow;i++) if(A[i][k]){piv=i;break;}
    if(piv<0) continue;
    for(int k2=0;k2<nc;k2++){int t=A[rank][k2];A[rank][k2]=A[piv][k2];A[piv][k2]=t;}
    int iv=inv_modp(A[rank][k],p);
    for(int k2=0;k2<nc;k2++) A[rank][k2]=A[rank][k2]*iv%p;
    for(int i=0;i<nrow;i++) if(i!=rank && A[i][k]){ int f=A[i][k]; for(int k2=0;k2<nc;k2++) A[i][k2]=((A[i][k2]-f*A[rank][k2])%p+p)%p; }
    rank++;
  }
  return rank;
}
static int nappear(void){int n=0;for(int c=0;c<NC;c++)if(appear[c])n++;return n;}
static void report(const char*label){
  int na=nappear(), r2=rank_modp(2), r13=rank_modp(13);
  printf("%-28s | cols=%2d rows=%2d | rank2=%2d rank13=%2d | dim2=%2d dim13=%2d\n",
         label,na,nrow,r2,r13,na-r2,na-r13);
}

int main(int argc,char**argv){
  if(argc>1 && !strcmp(argv[1],"tie")){ TIE=1; printf("=== MODE TIE (sigma=tau, Quagmire III) ===\n"); }
  else printf("=== MODE INDEP (sigma,tau libres, T36b) ===\n");
  for(int i=0;i<97;i++) C[i]=K4[i]-'A';
  /* --- ETAPE 0 : 24 cribs --- */
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int k=0;k<13;k++) known(21+k, e[k]-'A');
  for(int k=0;k<11;k++) known(63+k, b[k]-'A');
  report("24 cribs (base 13)");
  /* --- ETAPE 1 : valeurs NEUVES (coincidence ecart-7, cote non-crib) --- */
  known(8,'A'-'A');  report("+ p8=A (c15=c22)");
  known(39,'N'-'A'); report("+ p39=N (c32=c39)");
  known(83,'C'-'A'); report("+ p83=C (c76=c83)");
  /* --- ETAPE 2 : egalites NEUVES entre positions inconnues --- */
  equal(0,14);  report("+ p0=p14 (c7=c14)");
  equal(5,19);  report("+ p5=p19 (c12=c19)");
  equal(38,52); report("+ p38=p52 (c45=c52)");
  equal(79,93); report("+ p79=p93 (c86=c93)");
  /* --- ETAPE 3 : contraintes derivables des cribs (controle : ne doivent PAS baisser) --- */
  known(20,'C'-'A'); report("+ p20=C (Bean+ec7, derivable)");
  known(58,'C'-'A'); report("+ p58=C (c65=c72, derivable)");
  return 0;
}
