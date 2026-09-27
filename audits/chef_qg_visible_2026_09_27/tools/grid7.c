/* grid7.c — BLIND diagnostic on the REAL crib-key in a width-7 grid (no reconstructed plaintext).
 * Uses only ciphertext + the 2 authentic cribs EASTNORTHEAST@21-33, BERLINCLOCK@63-73.
 * (1) Verify the pas-7 structure of the CIPHERTEXT: doublets & C[i]=C[i+7] coincidences, per column mod 7.
 * (2) Compute the 24 REAL crib-key values (AZ Vig k=C-P, and KRYPTOS-keyed k=posK(C)-posK(P)).
 * (3) Lay them in the 14x7 grid (pos -> row=pos/7,col=pos%7) and print vertical (gap-7) key differences.
 * Usage: grid7
 */
#include <stdio.h>
#include <string.h>
#define N 97
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char*KAL="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int posK[26];
static int md(int x){x%=26;return x<0?x+26:x;}
int main(void){
  for(int i=0;i<26;i++)posK[KAL[i]-'A']=i;
  int C[N]; for(int i=0;i<N;i++)C[i]=K4[i]-'A';
  /* (1) pas-7 doublets & coincidences by column */
  printf("=== (1) CIPHERTEXT pas-7 structure ===\n");
  int colcnt[7]={0};
  printf("adjacent doublets C[i]==C[i+1] at:");
  for(int i=0;i+1<N;i++) if(C[i]==C[i+1]){printf(" %d(col%d)",i,i%7); colcnt[i%7]++;}
  printf("\n  doublet start-col histogram mod7: ");
  for(int c=0;c<7;c++)printf("c%d=%d ",c,colcnt[c]); printf("\n");
  int co7=0; printf("C[i]==C[i+7] (vertical adjacency) at:");
  for(int i=0;i+7<N;i++) if(C[i]==C[i+7]){printf(" %d",i);co7++;}
  printf("  (total=%d, random~%.1f)\n",co7, (N-7)/26.0);
  /* (2)+(3) real crib key */
  int cribpos[24]; int cribP[24]; int nc=0;
  const char*e="EASTNORTHEAST"; for(int i=0;i<13;i++){cribpos[nc]=21+i;cribP[nc]=e[i]-'A';nc++;}
  const char*b="BERLINCLOCK";   for(int i=0;i<11;i++){cribpos[nc]=63+i;cribP[nc]=b[i]-'A';nc++;}
  int kAZ[N],kKR[N],known[N]; for(int i=0;i<N;i++){known[i]=0;}
  for(int q=0;q<nc;q++){int i=cribpos[q];
    kAZ[i]=md(C[i]-cribP[q]); kKR[i]=md(posK[C[i]]-posK[cribP[q]]); known[i]=1;}
  printf("\n=== (2) REAL crib-key (from authentic cribs only) ===\nAZ : ");
  for(int q=0;q<nc;q++)putchar('A'+kAZ[cribpos[q]]);
  printf("\nKRY: "); for(int q=0;q<nc;q++)putchar('A'+kKR[cribpos[q]]); printf("\n");
  printf("\n=== (3) 14x7 grid, AZ crib-key (.=unknown) ===\n     c0 c1 c2 c3 c4 c5 c6\n");
  for(int r=0;r<14;r++){printf("r%-2d ",r);
    for(int c=0;c<7;c++){int p=r*7+c; if(p<N&&known[p])printf(" %c ",'A'+kAZ[p]); else printf(" . ");}
    printf("\n");}
  printf("\nvertical gap-7 key diffs AZ (k[i+7]-k[i]) where both known:\n");
  for(int i=0;i+7<N;i++) if(known[i]&&known[i+7])
    printf("  pos %d->%d col%d: %c->%c  diff=%d\n",i,i+7,i%7,'A'+kAZ[i],'A'+kAZ[i+7],md(kAZ[i+7]-kAZ[i]));
  return 0;
}
