/* gromark_bijcheck.c — CONFIRMATION EXACTE (demande chef) : pour chaque amorce Gromark de Bean,
 * la variété crib-EXACTE (2 alphabets tau,sig ; k_i CONNU) force-t-elle une collision (⇒ impossible)
 * ou laisse-t-elle une variété (⇒ sous-déterminée, charabia = confirme le nul du chef) ?
 * Modèle : tau[c_i] = (sig[p_i] + k_i) % 26 ; k=amorce(5 chiffres) puis k_i=(k_{i-5}+k_{i-4})%10.
 * Crib i : tau[c_i] - sig[p_i] = k_i (mod 26). Vars : tau[0..25], sig[26..51]. 24 lignes.
 * dim = (colonnes apparaissant) - rang (mod2 & mod13). Collisions forcées tau[a]=tau[b], sig[a]=sig[b].
 * Consistance : le système Ax=b admet-il une solution (permutation possible) ?
 * Usage : gromark_bijcheck bean_gt_10_5_primers.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NV 52
#define SIG(z) (26+(z))
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int cribpos[24],cribval[24],C[97];
static inline int md(int x){x%=26;return x<0?x+26:x;}
static int inv_modp(int a,int p){a%=p;if(a<0)a+=p;for(int x=1;x<p;x++)if(a*x%p==1)return x;return 0;}
/* rows augmentées : NV coeffs + 1 rhs (col NV) */
static int rank_modp(int rows[][NV+1],int nr,int p,int incl_rhs){
  static int A[64][NV+1]; int W=incl_rhs?NV+1:NV;
  for(int i=0;i<nr;i++)for(int c=0;c<=NV;c++)A[i][c]=((rows[i][c]%p)+p)%p;
  int rank=0;
  for(int c=0;c<W && rank<nr;c++){int pr=-1;for(int r=rank;r<nr;r++)if(A[r][c]){pr=r;break;}if(pr<0)continue;
    for(int k=0;k<=NV;k++){int t=A[rank][k];A[rank][k]=A[pr][k];A[pr][k]=t;}
    int iv=inv_modp(A[rank][c],p);for(int k=0;k<=NV;k++)A[rank][k]=A[rank][k]*iv%p;
    for(int r=0;r<nr;r++)if(r!=rank&&A[r][c]){int f=A[r][c];for(int k=0;k<=NV;k++)A[r][k]=((A[r][k]-f*A[rank][k])%p+p)%p;}
    rank++;}
  return rank;
}
static int nappear(int rows[][NV+1],int nr){int ap[NV]={0};for(int i=0;i<nr;i++)for(int c=0;c<NV;c++)if(rows[i][c])ap[c]=1;int n=0;for(int c=0;c<NV;c++)n+=ap[c];return n;}
/* x_a=x_b IMPLIQUÉ (collision réelle) ssi (e_a-e_b | 0) est dans l'espace-ligne AUGMENTÉ [A|b] :
 * i.e. rang([A|b]) inchangé quand on ajoute la ligne (e_a-e_b | rhs=0). Utilise incl_rhs=1. */
static int forced(int rows[][NV+1],int nr,int c0,int rAug2,int rAug13){
  int cnt=0;for(int a=0;a<26;a++)for(int b=a+1;b<26;b++){
    static int R[64][NV+1];for(int i=0;i<nr;i++)memcpy(R[i],rows[i],sizeof(int)*(NV+1));
    memset(R[nr],0,sizeof(int)*(NV+1));R[nr][c0+a]=1;R[nr][c0+b]=md(-1);R[nr][NV]=0;
    if(rank_modp(R,nr+1,2,1)==rAug2 && rank_modp(R,nr+1,13,1)==rAug13)cnt++;}
  return cnt;
}
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK"; int n=0;
  for(int i=0;i<13;i++){cribpos[n]=CE[i];cribval[n]=e[i]-'A';n++;}
  for(int i=0;i<11;i++){cribpos[n]=CB[i];cribval[n]=b[i]-'A';n++;}
  for(int i=0;i<97;i++)C[i]=K4[i]-'A';
  if(argc<2){fprintf(stderr,"usage: gromark_bijcheck primers.txt\n");return 1;}
  FILE*f=fopen(argv[1],"r"); char line[64]; int totfeasible=0,totforced=0,np=0,mindim=999,maxdim=-1;
  printf("amorce | dim2 dim13 | forced tau/sig | consistant(perm possible)\n");
  while(fgets(line,sizeof line,f)){
    int d[5],ok=1; if(strlen(line)<5)continue;
    for(int i=0;i<5;i++){if(line[i]<'0'||line[i]>'9'){ok=0;break;}d[i]=line[i]-'0';}
    if(!ok)continue; np++;
    int k[97]; for(int i=0;i<5;i++)k[i]=d[i]; for(int i=5;i<97;i++)k[i]=(k[i-5]+k[i-4])%10;
    static int rows[64][NV+1]; for(int a=0;a<24;a++){int i=cribpos[a];memset(rows[a],0,sizeof(int)*(NV+1));
      rows[a][C[i]]=1; rows[a][SIG(cribval[a])]=md(-1); rows[a][NV]=md(k[i]); }
    int ap=nappear(rows,24);
    int rA2=rank_modp(rows,24,2,0), rA13=rank_modp(rows,24,13,0);
    /* consistance : rang augmenté == rang non-augmenté (mod2 ET mod13) */
    int rAug2=rank_modp(rows,24,2,1), rAug13=rank_modp(rows,24,13,1);
    int cons=(rAug2==rA2 && rAug13==rA13);
    int ft=forced(rows,24,0,rAug2,rAug13), fs=forced(rows,24,SIG(0),rAug2,rAug13);
    int dim2=ap-rA2, dim13=ap-rA13;
    if(dim2<mindim)mindim=dim2; if(dim2>maxdim)maxdim=dim2;
    if(cons)totfeasible++; if(ft+fs>0)totforced++;
    if(np<=6||ft+fs>0||!cons) printf("%d%d%d%d%d |  %2d   %2d  |   %d / %d      | %s\n",d[0],d[1],d[2],d[3],d[4],dim2,dim13,ft,fs,cons?"oui":"NON");
  }
  fclose(f);
  printf("\n%d amorces : consistantes(perm possible)=%d, avec collision forcée=%d ; dim2 in [%d,%d]\n",np,totfeasible,totforced,mindim,maxdim);
  printf("=> %s\n", (maxdim>=8)?"variété de HAUTE dimension par amorce ⇒ SOUS-DÉTERMINÉ (confirme le nul du chef : crib-compatibles indiscernables du charabia)":"dim basse — réexaminer");
  return 0;
}
