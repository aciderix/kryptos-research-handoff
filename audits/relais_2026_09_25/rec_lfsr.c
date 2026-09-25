/* rec_lfsr.c — la clé de K4 suit-elle une récurrence linéaire (LFSR mod 26) ? (relais DeepSeek n° 3, 25/09)
 * k_n = a1·k_{n−1} + … + ar·k_{n−r} + c  (mod 26), ordre r = 1..5, coefficients inconnus.
 * Les cribs donnent la clé sur deux fragments consécutifs (21–33 : 13 valeurs ; 63–73 : 11 valeurs), soit
 * (13 − r) + (11 − r) équations linéaires en r + 1 inconnues. Cohérence mod 26 ⇔ cohérence mod 2 et mod 13 (restes chinois),
 * testée par élimination de Gauss dans GF(2) et GF(13). On essaie aussi chaque fragment seul (8 signes = 66–73 compris).
 * Alphabets : fichier binaire de genalpha (26 octets) ; types Q3, Q2, Q1, Q4a, Q4b ; VIG, BEAU, VARB ; les deux sens.
 * Témoins : NNULL chiffrés K4 mélangé.  Usage : rec_lfsr alphas.bin NNULL   (CTX = chiffré de contrôle)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static int inv13[13];
/* système A x = b (nr lignes, nc inconnues) cohérent modulo p (p = 2 ou 13) ? */
static int consistent(int nr, int nc, int A[][8], const int *b, int p) {
  int M[32][8]; for (int i = 0; i < nr; i++) { for (int j = 0; j < nc; j++) M[i][j] = A[i][j] % p; M[i][nc] = b[i] % p; }
  int row = 0;
  for (int col = 0; col < nc && row < nr; col++) { int piv = -1; for (int i = row; i < nr; i++) if (M[i][col]) { piv = i; break; } if (piv < 0) continue;
    for (int j = 0; j <= nc; j++) { int t = M[row][j]; M[row][j] = M[piv][j]; M[piv][j] = t; }
    int iv = p == 2 ? 1 : inv13[M[row][col]]; for (int j = 0; j <= nc; j++) M[row][j] = M[row][j] * iv % p;
    for (int i = 0; i < nr; i++) if (i != row && M[i][col]) { int f = M[i][col]; for (int j = 0; j <= nc; j++) M[i][j] = ((M[i][j] - f * M[row][j]) % p + p) % p; }
    row++; }
  for (int i = row; i < nr; i++) if (M[i][nc]) return 0;
  return 1;
}
/* frag : 0 = les deux fragments ; 1 = 21–33 ; 2 = 63–73 ; 3 = 66–73 (les 8 signes) */
static int test(const int *k, int r, int frag) {
  int A[32][8], b[32], n = 0; const int st[2] = {21, 63}, en[2] = {33, 73};
  for (int f = 0; f < 2; f++) { if (frag == 1 && f == 1) continue; if ((frag == 2 || frag == 3) && f == 0) continue;
    int s0 = frag == 3 ? 66 : st[f];
    for (int i = s0 + r; i <= en[f]; i++) { for (int j = 1; j <= r; j++) A[n][j - 1] = k[i - j]; A[n][r] = 1; b[n] = k[i]; n++; } }
  if (n <= r + 1) return -1; /* pas d'équation en surplus : non testable */
  return consistent(n, r + 1, A, b, 2) && consistent(n, r + 1, A, b, 13);
}
int main(int argc, char **argv) {
  for (int a = 1; a < 13; a++) for (int x = 1; x < 13; x++) if (a * x % 13 == 1) inv13[a] = x;
  FILE *f = fopen(argv[1], "rb"); fseek(f, 0, SEEK_END); long na = ftell(f) / 26; fseek(f, 0, SEEK_SET);
  char *AB = malloc(na * 26); if (fread(AB, 26, na, f) != (size_t)na) return 1; fclose(f);
  int NN = atoi(argv[2]), NCT = NN + 1; int (*CT)[97] = malloc(sizeof(*CT) * NCT);
  const char *c0 = getenv("CTX") ? getenv("CTX") : K4; for (int i = 0; i < 97; i++) CT[0][i] = c0[i] - 'A';
  for (int z = 1; z < NCT; z++) { memcpy(CT[z], CT[0], sizeof CT[0]); uint64_t s = (5000 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CT[z][i]; CT[z][i] = CT[z][j]; CT[z][j] = t; }
    if (getenv("DBL")) CT[z][68] = CT[z][67]; } /* DBL : témoins avec un doublet forcé en 67–68, comme K4 (TT) */
  int PT[97]; const char *e = "EASTNORTHEAST", *bb = "BERLINCLOCK"; for (int i = 0; i < 13; i++) PT[21 + i] = e[i] - 'A'; for (int i = 0; i < 11; i++) PT[63 + i] = bb[i] - 'A';
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  /* cnt[z][frag][r] : nombre de cas (alphabet × sens × type × convention) cohérents */
  long (*cnt)[4][6] = calloc(NCT, sizeof(*cnt));
  #pragma omp parallel for schedule(dynamic, 64)
  for (long ai = 0; ai < na * 2; ai++) {
    int S[26]; const char *a = AB + (ai >> 1) * 26; int rev = ai & 1; for (int i = 0; i < 26; i++) S[(rev ? a[25 - i] : a[i]) - 'A'] = i;
    long loc[4][6]; long (*lc)[4][6] = calloc(NCT, sizeof(*lc)); (void)loc;
    for (int ty = 0; ty < 5; ty++) { const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP, *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
      for (int z = 0; z < NCT; z++) for (int mo = 0; mo < 3; mo++) { int k[97];
        for (int i = 21; i <= 73; i++) { if (i > 33 && i < 63) continue; int x = X[CT[z][i]], y = Y[PT[i]]; k[i] = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x);
          if (z == 0 && ai < 2 && 0) {} }
        for (int fr = 0; fr < 4; fr++) for (int r = 1; r <= 5; r++) { int t = test(k, r, fr); if (t == 1) { lc[z][fr][r]++;
              if (z == 0 && (fr == 0 || r >= 3)) {
                #pragma omp critical
                printf("K4 cohérent : fragment %d ordre %d alph=%.26s sens=%d type=%d conv=%d\n", fr, r, a, rev, ty, mo); } } } } }
    #pragma omp critical
    for (int z = 0; z < NCT; z++) for (int fr = 0; fr < 4; fr++) for (int r = 1; r <= 5; r++) cnt[z][fr][r] += lc[z][fr][r];
    free(lc);
  }
  const char *fn[4] = {"21–33 + 63–73", "21–33 seul", "63–73 seul", "66–73 (8 signes)"};
  printf("== %ld alphabets × 2 sens × 5 types × 3 conventions ; %d témoins K4 mélangé ==\n", na, NN);
  for (int fr = 0; fr < 4; fr++) for (int r = 1; r <= 5; r++) {
    long *v = malloc(sizeof(long) * NCT); int nv = 0, ge = 0, pos = 0; for (int z = 1; z < NCT; z++) { v[nv++] = cnt[z][fr][r]; ge += cnt[z][fr][r] >= cnt[0][fr][r]; pos += cnt[z][fr][r] > 0; }
    { int cmpl(const void *x, const void *y); qsort(v, nv, sizeof(long), cmpl); }
    int eq = fr == 0 ? 24 - 2 * r : fr == 1 ? 13 - r : fr == 2 ? 11 - r : 8 - r;
    printf("%-18s ordre %d (%2d équations, %d inconnues) : K4 %8ld | témoins min %ld médiane %ld max %ld | p = %.4f | témoins avec au moins un cas : %.1f %%\n", fn[fr], r, eq, r + 1, cnt[0][fr][r], v[0], v[nv / 2], v[nv - 1], (ge + 1.0) / (NN + 1), 100.0 * pos / NN); free(v);
  }
  return 0;
}
int cmpl(const void *x, const void *y) { long a = *(const long *)x, b = *(const long *)y; return (a > b) - (a < b); }
