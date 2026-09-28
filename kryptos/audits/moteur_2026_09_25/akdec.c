/* akdec ALPHABET TYPE MODE : autoclé sur le clair, L = 1..13, lecture de la clé en Y / A–Z / KRYPTOS ; déchiffrement complet depuis les cribs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
int main(int c, char **v) { int S[26], AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { S[v[1][i] - 'A'] = i; AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  int ty = atoi(v[2]), mo = atoi(v[3]);
  const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP, *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
  int YI[26]; for (int i = 0; i < 26; i++) YI[Y[i]] = i;
  float *QG = malloc(456976 * 4); FILE *f = fopen("qg.bin", "rb"); if (fread(QG, 4, 456976, f) != 456976) return 1;
  int CPL[97], ISC[97] = {0}; const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; for (int i = 0; i < 13; i++) { CPL[21 + i] = e[i] - 'A'; ISC[21 + i] = 1; } for (int i = 0; i < 11; i++) { CPL[63 + i] = b[i] - 'A'; ISC[63 + i] = 1; }
  for (int zr = 0; zr < 3; zr++) for (int L = 1; L <= 13; L++) { const int *Z = zr == 0 ? Y : zr == 1 ? AZ : KAP; int P[97]; for (int i = 0; i < 97; i++) P[i] = -1; int err = 0;
    for (int r = 0; r < L; r++) { int s0 = -1; for (int i = 0; i < 97; i++) if (ISC[i] && i % L == r) { s0 = i; break; } int p = CPL[s0]; P[s0] = p;
      for (int i = s0 + L; i < 97; i += L) { int k = Z[p], x = X[K4[i] - 'A'], y = mo == 0 ? md(x - k) : mo == 1 ? md(k - x) : md(x + k); p = YI[y]; P[i] = p; if (ISC[i] && p != CPL[i]) err++; } }
    if (err > 4) continue; double s = 0; int n = 0; for (int i = 0; i + 3 < 97; i++) if (P[i] >= 0 && P[i+1] >= 0 && P[i+2] >= 0 && P[i+3] >= 0) { s += QG[((P[i]*26+P[i+1])*26+P[i+2])*26+P[i+3]]; n++; }
    printf("lu=%d L=%d erreurs=%d score=%.3f : ", zr, L, err, s / n); for (int i = 0; i < 97; i++) putchar(P[i] < 0 ? '.' : 'A' + P[i]); putchar('\n'); }
  return 0; }
