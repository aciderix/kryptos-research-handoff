/* indep_kernel.c — EXPOSE la variété crib-EXACTE du 2-alphabets Vigenère autoclé écart-7, en
 * forme paramétrique, pour la FUSION (chef code un recuit qg_big DANS la variété).
 *
 * Vars (59) : tau[0..25] (alphabet chiffré), kappa[26..32] (7 amorces), sig[33..58] (alphabet clair).
 * Modèle : x_i = tau[c_i] - k_i ; k_i=kappa_{i%7}(i<7) sinon x_{i-7} ; pt_i=siginv[x_i].
 * Crib (pos i, lettre v) : sig[v] - x_i(tau,kappa) = 0 (HOMOGÈNE, coeffs ±1).
 * Système = 24 lignes. RREF Z/26 (pivots unités) ⇒ rang 24, noyau de dim 59-24=35.
 * Tout point = Σ_{k=0..34} alpha_k · V_k  (alpha ∈ Z_26^35 ; x0=0 car homogène).
 * decode(alpha) : reconstruit tau,sig,kappa ; si tau ET sig sont des PERMUTATIONS ⇒ 24/24 cribs
 * GARANTIS, on décode le clair. (Les lettres d'alphabet hors-cribs = « complétion » : ~la moitié
 * des 35 dims ; recommandation : recuit factorisé (tau perm,kappa) + mono-solve sig est + robuste
 * que bouger alpha brut, car un alpha aléatoire donne rarement des permutations.)
 *
 * Sorties : indep_variety_basis.txt (35 vecteurs × 59 coeffs, mod 26) pour le moteur du chef.
 * Usage : indep_kernel        (self-test + génère le fichier de base)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NV 59
#define KAP(r) (26+(r))
#define SIG(z) (33+(z))
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int cribL[97],C[97];
static inline int md(int x){x%=26;return x<0?x+26:x;}
static int inv26[26]; static void init_inv(void){for(int a=0;a<26;a++){inv26[a]=0;for(int x=1;x<26;x++)if(a*x%26==1){inv26[a]=x;break;}}}
static int M[64][NV+1]; static int nrow=0;
static void build_eq(int i,int v){int*row=M[nrow];memset(row,0,sizeof(int)*(NV+1));
  row[SIG(v)]=md(row[SIG(v)]+1);
  int r=i%7,j=i/7,s=1; for(int m=0,pos=i;m<=j;m++,pos-=7){row[C[pos]]=md(row[C[pos]]-s);s=-s;}
  int ks=((j+1)&1)?-1:1; row[KAP(r)]=md(row[KAP(r)]-ks); nrow++; }
/* RREF Z/26 pivots unités */
static int pivotcol[NV],isfree[NV],rank=0;
static int rref(void){rank=0;for(int c=0;c<NV;c++)isfree[c]=1;
  for(int col=0;col<NV&&rank<nrow;col++){int pr=-1;for(int r=rank;r<nrow;r++)if(inv26[M[r][col]]){pr=r;break;}if(pr<0)continue;
    for(int c=0;c<=NV;c++){int t=M[rank][c];M[rank][c]=M[pr][c];M[pr][c]=t;}
    int iv=inv26[M[rank][col]];for(int c=0;c<=NV;c++)M[rank][c]=M[rank][c]*iv%26;
    for(int r=0;r<nrow;r++)if(r!=rank&&M[r][col]){int f=M[r][col];for(int c=0;c<=NV;c++)M[r][c]=md(M[r][c]-f*M[rank][c]);}
    pivotcol[rank]=col;isfree[col]=0;rank++;}
  return rank; }
static int freelist[NV],nfree=0;
static int basis[NV][NV]; /* basis[k][var] = coeff de alpha_k dans var (k=0..nfree-1) */
static void build_basis(void){
  nfree=0;for(int c=0;c<NV;c++)if(isfree[c])freelist[nfree++]=c;
  for(int k=0;k<nfree;k++){int fv=freelist[k]; for(int v=0;v<NV;v++)basis[k][v]=0; basis[k][fv]=1;
    /* var pivot p : x_p = -Σ_free M[rowp][f]·alpha_f ; contribution de alpha_k (=fv) : -M[rowp][fv] */
    for(int t=0;t<rank;t++){int col=pivotcol[t]; basis[k][col]=md(-M[t][fv]); }
  }
}
static void decode(const int*alpha,int*var){ for(int v=0;v<NV;v++){int s=0;for(int k=0;k<nfree;k++)s=md(s+alpha[k]*basis[k][v]);var[v]=s;} }
int main(void){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK"; init_inv();
  for(int i=0;i<13;i++)cribL[CE[i]]=e[i]-'A'; for(int i=0;i<11;i++)cribL[CB[i]]=b[i]-'A';
  for(int i=0;i<97;i++)C[i]=K4[i]-'A';
  nrow=0; for(int k=0;k<13;k++)build_eq(CE[k],cribL[CE[k]]); for(int k=0;k<11;k++)build_eq(CB[k],cribL[CB[k]]);
  int rk=rref(); build_basis();
  printf("=== variété crib-EXACTE INDEP (2 alphabets) autoclé écart-7 ===\n");
  printf("vars=%d, rang=%d, dim noyau=%d (alpha ∈ Z_26^%d ; x0=0)\n",NV,rk,nfree,nfree);
  /* SELF-TEST : plante (tau,sig,kappa), fabrique K4', vérifie que decode(alpha vrai) rend tout */
  {int tau[26]={7,2,19,11,23,4,16,0,25,9,14,1,20,6,18,21,3,10,24,12,5,8,15,17,22,13};
   int sig[26]={4,17,0,11,25,8,1,20,9,2,15,22,6,13,24,3,7,10,19,12,5,23,16,21,14,18};
   int kap[7]={5,11,2,20,8,14,1};
   int taui[26];for(int i=0;i<26;i++)taui[tau[i]]=i; int sigi[26];for(int i=0;i<26;i++)sigi[sig[i]]=i;
   /* fabrique un clair anglais+cribs, chiffre en autoclé écart-7 : ct_i = taui[(x_i + k_i)] , x_i=sig[pt_i] */
   const char*eng="WEAREINTHEEASTNORTHEASTQUADRANTLOOKINGTOWARDTHEBERLINCLOCKATMIDNIGHTUNDERTHESTARSXYZFILLERWORDSOK";
   int pt0[97];for(int i=0;i<97;i++)pt0[i]=eng[i]-'A'; for(int i=0;i<13;i++)pt0[CE[i]]=cribL[CE[i]];for(int i=0;i<11;i++)pt0[CB[i]]=cribL[CB[i]];
   int x[97],ct[97];for(int i=0;i<97;i++){int ki=(i<7)?kap[i]:x[i-7]; x[i]=sig[pt0[i]]; ct[i]=taui[md(x[i]+ki)];}
   /* le vrai (tau,sig,kappa) est-il un point de NOTRE variété (bâtie sur le VRAI K4) ? Non : la variété
      dépend de C=K4. Ici on refait la variété sur ct planté pour valider le décodeur. */
   nrow=0;for(int i=0;i<97;i++)C[i]=ct[i]; for(int k=0;k<13;k++)build_eq(CE[k],cribL[CE[k]]);for(int k=0;k<11;k++)build_eq(CB[k],cribL[CB[k]]);
   rref();build_basis();
   int truevar[NV];for(int i=0;i<26;i++)truevar[i]=tau[i]; for(int r=0;r<7;r++)truevar[KAP(r)]=kap[r]; for(int z=0;z<26;z++)truevar[SIG(z)]=sig[z];
   /* alpha vrai = valeurs des vars libres ; decode doit reproduire tout truevar */
   int alpha[NV];for(int k=0;k<nfree;k++)alpha[k]=truevar[freelist[k]];
   int var[NV];decode(alpha,var); int ok=1;for(int v=0;v<NV;v++)if(var[v]!=truevar[v])ok=0;
   /* et le clair décodé == pt0 ? */
   int tv[26];for(int i=0;i<26;i++)tv[i]=var[i]; int sv[26];for(int z=0;z<26;z++)sv[z]=var[SIG(z)]; int svi[26];for(int z=0;z<26;z++)svi[sv[z]]=z;
   int kp[7];for(int r=0;r<7;r++)kp[r]=var[KAP(r)]; int xx[97],rec=0; for(int i=0;i<97;i++){int ki=(i<7)?kp[i]:xx[i-7]; xx[i]=md(tv[ct[i]]-ki); if(svi[xx[i]]==pt0[i])rec++;}
   printf("SELF-TEST : decode(alpha_vrai)==(tau,sig,kappa) %s ; clair recouvré %d/97 %s\n",ok?"OUI":"NON",rec,rec==97?"PASS":"FAIL");
  }
  /* régénère la base sur le VRAI K4 et l'écrit */
  for(int i=0;i<97;i++)C[i]=K4[i]-'A'; nrow=0; for(int k=0;k<13;k++)build_eq(CE[k],cribL[CE[k]]);for(int k=0;k<11;k++)build_eq(CB[k],cribL[CB[k]]); rref();build_basis();
  FILE*o=fopen("indep_variety_basis.txt","w");
  fprintf(o,"# variété crib-EXACTE INDEP autoclé écart-7 sur K4 réel. dim=%d. vars: 0-25=tau,26-32=kappa,33-58=sig.\n",nfree);
  fprintf(o,"# point = Σ alpha_k * V_k (mod 26), alpha ∈ Z_26^%d, x0=0. decode: pt_i=siginv[tau[c_i]-k_i].\n",nfree);
  fprintf(o,"nfree %d\nfreevars",nfree); for(int k=0;k<nfree;k++)fprintf(o," %d",freelist[k]); fprintf(o,"\n");
  for(int k=0;k<nfree;k++){fprintf(o,"V%02d",k);for(int v=0;v<NV;v++)fprintf(o," %d",basis[k][v]);fprintf(o,"\n");}
  fclose(o);
  printf("base écrite: indep_variety_basis.txt (%d vecteurs × %d).\n",nfree,NV);
  return 0;
}
