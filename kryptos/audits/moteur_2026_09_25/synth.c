/* synth.c — chiffrés synthétiques de contrôle pour k4x (97 lettres, cribs EASTNORTHEAST en 21 et BERLINCLOCK en 63).
 * Usage : synth FAMILLE MOTCLE TYPE MODE [param] [graine]
 *   FAMILLE : P (période 7), L (période 7 + décalage par ligne du cuivre), R (+ décalage par ligne de 7),
 *             B (blocs de n = param, motif s·j), T (T16, mot de longueur param), A (autoclé clair, écart param),
 *             C (autoclé chiffré, écart param), M (période 7, convention par ligne du cuivre).
 *   TYPE 0..4 = Q3, Q2, Q1, Q4a, Q4b ; MODE 0..2 = VIG, BEAU, VARB.  Sortie : le chiffré sur stdout.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static const char *TXT = "ITWASTHEBESTOFTIMESITWASTHEWORSEASTNORTHEASTOFTIMESITWASTHEAGEOFWISDOMITWASTHEAGEOFBERLINCLOCKFOOLISHNESSITWASTHEEPOCHOFBELIEF";
static uint64_t rs = 88172645463325252ULL;
static int rnd(int n) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs % n; }
static int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
int main(int argc, char **argv) {
  char fam = argv[1][0]; const char *w = argv[2]; int ty = atoi(argv[3]), mo = atoi(argv[4]), prm = argc > 5 ? atoi(argv[5]) : 7;
  if (argc > 6) rs = strtoull(argv[6], 0, 10) * 0x9E3779B97F4A7C15ULL | 1;
  int S[26], used[26] = {0}, n = 0, AZ[26], KAP[26]; char a[26];
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (!used[c]) { used[c] = 1; a[n++] = *p; } }
  for (int c = 0; c < 26; c++) if (!used[c]) a[n++] = 'A' + c;
  for (int i = 0; i < 26; i++) { S[a[i] - 'A'] = i; AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
  const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
  int XI[26]; for (int i = 0; i < 26; i++) XI[X[i]] = i;
  /* clair : 97 lettres, cribs aux bonnes places */
  int P[97]; int off = 30 - 21; /* TXT[30..42] = EASTNORTHEAST */
  const char *e = strstr(TXT, "EASTNORTHEAST"); off = (int)(e - TXT) - 21;
  for (int i = 0; i < 97; i++) P[i] = TXT[i + off] - 'A';
  const char *b = "BERLINCLOCK"; for (int i = 0; i < 11; i++) P[63 + i] = b[i] - 'A';
  int m7[7], aoff[8], k[97], C[97], modes[4];
  for (int j = 0; j < 7; j++) m7[j] = rnd(26);
  for (int s = 0; s < 8; s++) aoff[s] = rnd(26);
  for (int s = 0; s < 4; s++) modes[s] = rnd(3);
  int ord[7] = {0, 1, 2, 3, 4, 5, 6}; for (int i = 6; i > 0; i--) { int j = rnd(i + 1), t = ord[i]; ord[i] = ord[j]; ord[j] = t; }
  int rd[97]; { int t = 0; for (int q = 0; q < 7; q++) for (int r = 0; r * 7 + ord[q] < 97; r++) rd[r * 7 + ord[q]] = t++; }
  int wk[7]; for (int i = 0; i < 7; i++) wk[i] = rnd(26);
  int phi = rnd(prm), sstep = rnd(26);
  for (int i = 0; i < 97; i++) {
    int kk = 0, mm = mo;
    switch (fam) {
      case 'P': kk = m7[i % 7]; break;
      case 'L': kk = m7[i % 7] + aoff[(i + 27) / 31]; break;
      case 'R': kk = m7[i % 7] + aoff[i / 7 % 8]; break;
      case 'B': { int blk = (i - phi + prm) / prm, j = ((i - phi) % prm + prm) % prm; kk = aoff[blk % 8] + sstep * j; } break;
      case 'T': kk = wk[rd[i] % prm]; break;
      case 'M': kk = m7[i % 7]; mm = modes[(i + 27) / 31]; break;
      case 'A': kk = i < prm ? m7[i % 7] : Y[P[i - prm]]; break;
      case 'K': kk = Y["THEQUICKLYFADINGLIGHTOFTHEAFTERNOONFELLACROSSTHEROOMWHERETHEOLDMANSATREADINGHISLETTERSSLOWLYANDCAREFULLYEVERYDAY"[i] - 'A']; break;
      case 'C': kk = i < prm ? m7[i % 7] : X[C[i - prm]]; break;
    }
    kk = md(kk); int y = Y[P[i]], x = mm == 0 ? md(y + kk) : mm == 1 ? md(kk - y) : md(y - kk);
    C[i] = XI[x]; k[i] = kk;
  }
  (void)k;
  for (int i = 0; i < 97; i++) putchar('A' + C[i]); putchar('\n');
  return 0;
}
