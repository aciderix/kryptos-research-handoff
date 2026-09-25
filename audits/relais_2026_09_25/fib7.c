/* fib7.c — clé « Fibonacci au pas 7 » (relais DeepSeek n° 4, 25/09) : dans chaque colonne i ≡ r (mod 7), la suite
 * x_n = k[r + 7n] obéit à une récurrence commune aux 7 colonnes :
 *   ordre 2 : x_n = a·x_{n−1} + b·x_{n−2} + c
 *   ordre 3 : x_n = a·x_{n−1} + b·x_{n−2} + d·x_{n−3} + c
 * Cas particuliers proposés : Fibonacci (a = b = 1, c = 0) ; k[i] = k[i−7] + k[i−14] − k[i−21] (1, 1, −1) ;
 * k[i] = 2k[i−7] − k[i−21] (2, 0, −1). Chaque colonne a sa graine (2 ou 3 valeurs libres).
 * Les cribs donnent, par colonne, 2 à 4 valeurs (rangs n = 3, 4, 9, 10). On teste EXACTEMENT, pour chaque jeu de
 * coefficients, s'il existe des graines qui les reproduisent toutes (Gauss dans GF(2) et GF(13)) ; puis avec une lettre
 * de crib retirée (une erreur). Coefficients : tous (26³ en ordre 2) pour A–Z et KRYPTOS ; jeu réduit sinon.
 * Usage : fib7 alphas.bin NNULL ORDRE(2|3) PLEIN(0 réduit | 1 tous | 2 Fibonacci seul)   (NODROP : sans le test à une erreur)   (CTX = chiffré de contrôle)
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
static int consistent(int nr, int nc, int A[][4], const int *b, int p) {
  int M[8][5]; for (int i = 0; i < nr; i++) { for (int j = 0; j < nc; j++) M[i][j] = A[i][j] % p; M[i][nc] = b[i] % p; }
  int row = 0;
  for (int col = 0; col < nc && row < nr; col++) { int piv = -1; for (int i = row; i < nr; i++) if (M[i][col]) { piv = i; break; } if (piv < 0) continue;
    for (int j = 0; j <= nc; j++) { int t = M[row][j]; M[row][j] = M[piv][j]; M[piv][j] = t; }
    int iv = p == 2 ? 1 : inv13[M[row][col]]; for (int j = 0; j <= nc; j++) M[row][j] = M[row][j] * iv % p;
    for (int i = 0; i < nr; i++) if (i != row && M[i][col]) { int f = M[i][col]; for (int j = 0; j <= nc; j++) M[i][j] = ((M[i][j] - f * M[row][j]) % p + p) % p; }
    row++; }
  for (int i = row; i < nr; i++) if (M[i][nc]) return 0;
  return 1;
}
static int POS[24], PT[24];
/* coefficients (a, b, d, c) : existe-t-il des graines ? skip = lettre de crib retirée (−1 : aucune) */
static int ok(const int *k, int R, const int *co, int skip) {
  /* x_n = Σ_j L[n][j]·seed_j + L[n][R] (j < R) */
  int L[14][4]; memset(L, 0, sizeof L); for (int j = 0; j < R; j++) L[j][j] = 1;
  for (int n = R; n < 14; n++) for (int j = 0; j <= R; j++) { int v = co[0] * L[n - 1][j] + co[1] * L[n - 2][j] + (R == 3 ? co[2] * L[n - 3][j] : 0); if (j == R) v += co[3]; L[n][j] = md(v); }
  for (int r = 0; r < 7; r++) { int A[8][4], b[8], ne = 0;
    for (int t = 0; t < 24; t++) if (t != skip && POS[t] % 7 == r) { int n = POS[t] / 7; for (int j = 0; j < R; j++) A[ne][j] = L[n][j]; b[ne] = md(k[t] - L[n][R]); ne++; }
    if (ne <= R) continue; /* pas d'équation en surplus dans cette colonne */
    if (!consistent(ne, R, A, b, 2) || !consistent(ne, R, A, b, 13)) return 0; }
  return 1;
}
int main(int argc, char **argv) {
  for (int a = 1; a < 13; a++) for (int x = 1; x < 13; x++) if (a * x % 13 == 1) inv13[a] = x;
  FILE *f = fopen(argv[1], "rb"); fseek(f, 0, SEEK_END); long na = ftell(f) / 26; fseek(f, 0, SEEK_SET);
  char *AB = malloc(na * 26); if (fread(AB, 26, na, f) != (size_t)na) return 1; fclose(f);
  int NN = atoi(argv[2]), R = atoi(argv[3]), full = atoi(argv[4]), NCT = NN + 1; int (*CT)[97] = malloc(sizeof(*CT) * NCT);
  const char *c0 = getenv("CTX") ? getenv("CTX") : K4; for (int i = 0; i < 97; i++) CT[0][i] = c0[i] - 'A';
  for (int z = 1; z < NCT; z++) { memcpy(CT[z], CT[0], sizeof CT[0]); uint64_t s = (5000 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CT[z][i]; CT[z][i] = CT[z][j]; CT[z][j] = t; } }
  const char *e = "EASTNORTHEAST", *bb = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; } for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = bb[i] - 'A'; n++; }
  /* jeux de coefficients */
  int (*CO)[4] = malloc(sizeof(int) * 4 * 460000); int nco = 0;
  if (full == 2) { for (int c = 0; c < 26; c++) { CO[nco][0] = 1; CO[nco][1] = 1; CO[nco][2] = 0; CO[nco][3] = c; nco++; } } /* Fibonacci seul, constante libre */
  else if (full) { for (int a = 0; a < 26; a++) for (int b = 0; b < 26; b++) for (int d = 0; d < (R == 3 ? 26 : 1); d++) for (int c = 0; c < 26; c++) { CO[nco][0] = a; CO[nco][1] = b; CO[nco][2] = d; CO[nco][3] = c; nco++; } }
  else { const int sm[] = {0, 1, 2, 25, 24}; for (int a = 0; a < 5; a++) for (int b = 0; b < 5; b++) for (int d = 0; d < (R == 3 ? 5 : 1); d++) for (int c = 0; c < 26; c++) {
      CO[nco][0] = sm[a]; CO[nco][1] = sm[b]; CO[nco][2] = sm[d]; CO[nco][3] = c; nco++; } }
  fprintf(stderr, "ordre %d, %d jeux de coefficients, %ld alphabets, %d chiffrés\n", R, nco, na, NCT);
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  long (*cnt)[2] = calloc(NCT, sizeof(*cnt));
  #pragma omp parallel for schedule(dynamic, 1)
  for (long ai = 0; ai < na * 2; ai++) {
    int S[26]; const char *a = AB + (ai >> 1) * 26; int rev = ai & 1; for (int i = 0; i < 26; i++) S[(rev ? a[25 - i] : a[i]) - 'A'] = i;
    long (*lc)[2] = calloc(NCT, sizeof(*lc));
    for (int ty = 0; ty < 5; ty++) { const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP, *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
      for (int z = 0; z < NCT; z++) for (int mo = 0; mo < 3; mo++) { int k[24];
        for (int t = 0; t < 24; t++) { int x = X[CT[z][POS[t]]], y = Y[PT[t]]; k[t] = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x); }
        for (int q = 0; q < nco; q++) {
          if (ok(k, R, CO[q], -1)) { lc[z][0]++; lc[z][1]++;
            if (z == 0) {
              #pragma omp critical
              printf("K4 sans erreur : coef a=%d b=%d d=%d c=%d alph=%.26s sens=%d type=%d conv=%d\n", CO[q][0], CO[q][1], CO[q][2], CO[q][3], a, rev, ty, mo); }
            continue; }
          if (!getenv("NODROP")) for (int sk = 0; sk < 24; sk++) if (ok(k, R, CO[q], sk)) { lc[z][1]++; break; } } } }
    #pragma omp critical
    for (int z = 0; z < NCT; z++) { cnt[z][0] += lc[z][0]; cnt[z][1] += lc[z][1]; }
    free(lc);
  }
  for (int m = 0; m < 2; m++) { int ge = 0, pos = 0; long mn = 1L << 60, mx = 0; double s = 0;
    for (int z = 1; z < NCT; z++) { ge += cnt[z][m] >= cnt[0][m]; pos += cnt[z][m] > 0; if (cnt[z][m] < mn) mn = cnt[z][m]; if (cnt[z][m] > mx) mx = cnt[z][m]; s += cnt[z][m]; }
    printf("%s : K4 %ld cas | témoins min %ld moyenne %.1f max %ld ; au moins un cas : %.1f %% | p = %.4f\n", m ? "au plus une erreur" : "sans erreur",
      cnt[0][m], mn, s / NN, mx, 100.0 * pos / NN, (ge + 1.0) / (NN + 1)); }
  return 0;
}
