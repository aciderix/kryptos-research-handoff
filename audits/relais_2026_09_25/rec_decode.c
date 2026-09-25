/* rec_decode : pour les cas cohérents en 66–73 (ordre 3), toutes les solutions (a1,a2,a3,c) mod 26, puis la clé prolongée
   après 73 et le clair 74–96 ; vérifie aussi 63–65 (BER). Usage : rec_decode ALPH sens type conv */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR", *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
int main(int c, char **v) { int S[26], AZ[26], KAP[26], rev = atoi(v[2]), ty = atoi(v[3]), mo = atoi(v[4]);
  for (int i = 0; i < 26; i++) { S[(rev ? v[1][25 - i] : v[1][i]) - 'A'] = i; AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP, *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
  int YI[26]; for (int i = 0; i < 26; i++) YI[Y[i]] = i;
  const char *pl = "BERLINCLOCK"; int k[97];
  for (int i = 63; i <= 73; i++) { int x = X[K4[i] - 'A'], y = Y[pl[i - 63] - 'A']; k[i] = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x); }
  int ns = 0;
  for (int a1 = 0; a1 < 26; a1++) for (int a2 = 0; a2 < 26; a2++) for (int a3 = 0; a3 < 26; a3++) for (int cc = 0; cc < 26; cc++) {
    int ok = 1; for (int n = 69; n <= 73 && ok; n++) ok = md(a1 * k[n - 1] + a2 * k[n - 2] + a3 * k[n - 3] + cc) == k[n];
    if (!ok) continue; ns++;
    int kk[97]; memcpy(kk, k, sizeof k); for (int n = 74; n < 97; n++) kk[n] = md(a1 * kk[n - 1] + a2 * kk[n - 2] + a3 * kk[n - 3] + cc);
    int back = 0; for (int n = 68; n >= 66; n--) if (md(a1 * k[n - 1] + a2 * k[n - 2] + a3 * k[n - 3] + cc) == k[n]) back++;
    if (ns <= 3) { printf("  a=(%d,%d,%d) c=%d ; 66–68 aussi reproduits : %d/3 ; clair 74–96 : ", a1, a2, a3, cc, back);
      for (int n = 74; n < 97; n++) { int x = X[K4[n] - 'A'], y = mo == 0 ? md(x - kk[n]) : mo == 1 ? md(kk[n] - x) : md(x + kk[n]); putchar('A' + YI[y]); } putchar('\n'); } }
  printf("  %d solutions\n", ns); return 0; }
