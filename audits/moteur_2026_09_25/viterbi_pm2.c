/* viterbi_pm2.c — « le chiffré est-il le clair décalé de ±T lettre par lettre ? » (25/09, nuit)
 * Pour une fenêtre [a, b) de K4 (et de témoins K4 mélangé), cherche par programmation dynamique le clair anglais le plus
 * probable (quadrigrammes, log10) parmi tous ceux qui s'écartent du chiffré de −T à +T (alphabet A–Z) à chaque position.
 * Contrôle positif : anglais chiffré avec une clé aléatoire dans [−T, T].  Usage : viterbi_pm2 a b T NNULL  (CTX = chiffré)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static float QG[456976];
static double best(const int *C, int a, int b, int T, char *out) {
  int W = 2 * T + 1, n = b - a; if (n < 4) return 0;
  /* état = (choix i-2, i-1, i) ; score cumulé des quadrigrammes */
  int NS = W * W * W; double *S = malloc(sizeof(double) * NS * n); int *BP = malloc(sizeof(int) * NS * n);
  #define L(i, d) (((C[a + (i)] + (d) - T) % 26 + 26) % 26)
  for (int s = 0; s < NS; s++) S[2 * NS + s] = 0, BP[2 * NS + s] = -1;
  for (int i = 3; i < n; i++) for (int s = 0; s < NS; s++) { int d1 = s / (W * W), d2 = s / W % W, d3 = s % W; double bs = -1e30; int bp = 0;
      for (int d0 = 0; d0 < W; d0++) { int ps = (d0 * W + d1) * W + d2; double v = S[(i - 1) * NS + ps] + QG[((L(i - 3, d0) * 26 + L(i - 2, d1)) * 26 + L(i - 1, d2)) * 26 + L(i, d3)]; if (v > bs) { bs = v; bp = ps; } }
      S[i * NS + s] = bs; BP[i * NS + s] = bp; }
  int bs = 0; for (int s = 1; s < NS; s++) if (S[(n - 1) * NS + s] > S[(n - 1) * NS + bs]) bs = s;
  double r = S[(n - 1) * NS + bs] / (n - 3);
  if (out) { int st = bs; for (int i = n - 1; i >= 3; i--) { out[i] = 'A' + L(i, st % W); if (i == 3) { out[2] = 'A' + L(2, st / W % W); out[1] = 'A' + L(1, st / (W * W)); int p = BP[i * NS + st]; out[0] = 'A' + L(0, p / (W * W)); } st = BP[i * NS + st]; } out[n] = 0; }
  free(S); free(BP); return r;
}
int main(int argc, char **argv) {
  int a = atoi(argv[1]), b = atoi(argv[2]), T = atoi(argv[3]), NN = atoi(argv[4]);
  FILE *f = fopen("qg.bin", "rb"); if (fread(QG, 4, 456976, f) != 456976) return 1; fclose(f);
  const char *K4 = getenv("CTX") ? getenv("CTX") : "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
  int C[97]; for (int i = 0; i < 97; i++) C[i] = K4[i] - 'A';
  char out[128]; double s0 = best(C, a, b, T, out); printf("K4 [%d,%d) ±%d : %.3f  %s\n", a, b, T, s0, out);
  int ge = 0; double mx = -99, sum = 0;
  for (int z = 1; z <= NN; z++) { int D[97]; memcpy(D, C, sizeof D); uint64_t s = (5000 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = D[i]; D[i] = D[j]; D[j] = t; }
    double v = best(D, a, b, T, 0); sum += v; if (v > mx) mx = v; ge += v >= s0; }
  if (NN) printf("témoins : moyenne %.3f max %.3f ; p = %.3f\n", sum / NN, mx, (ge + 1.0) / (NN + 1));
  return 0;
}
