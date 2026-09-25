/* motif_kz_tj_di.c — « trois bigrammes répétés dans le même ordre, en progression arithmétique aux deux endroits » (KZ, TJ, DI :
 * 45/50/55 puis 77/80/83). Rareté mesurée sur 2 millions de mélanges de K4 et de textes uniformes (relais du 25/09/2026). */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
static const char *K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
/* renvoie le nombre de configurations (triplets de bigrammes distincts) ; mode 0 : AP aux deux endroits ; mode 1 : même ordre seulement */
static int stat(const int *c, int mode, int verbose) {
  int pos[676][8], np[676]; memset(np, 0, sizeof np);
  for (int i = 0; i < 96; i++) { int b = c[i] * 26 + c[i + 1]; if (np[b] < 8) pos[b][np[b]++] = i; }
  int occ[200][2], no = 0;  /* paires d'occurrences d'un même bigramme */
  for (int b = 0; b < 676; b++) for (int x = 0; x < np[b]; x++) for (int y = x + 1; y < np[b]; y++) if (no < 200) { occ[no][0] = pos[b][x]; occ[no][1] = pos[b][y]; no++; }
  int cnt = 0;
  for (int u = 0; u < no; u++) for (int v = 0; v < no; v++) for (int w = 0; w < no; w++) {
    int a1 = occ[u][0], a2 = occ[v][0], a3 = occ[w][0], b1 = occ[u][1], b2 = occ[v][1], b3 = occ[w][1];
    if (!(a1 < a2 && a2 < a3 && b1 < b2 && b2 < b3)) continue;
    if (a2 - a1 < 2 || a3 - a2 < 2 || b2 - b1 < 2 || b3 - b2 < 2) continue;   /* bigrammes disjoints */
    if (a3 + 1 >= b1) continue;                                                    /* les deux groupes ne se chevauchent pas */
    if (mode == 0 && !(a2 - a1 == a3 - a2 && b2 - b1 == b3 - b2)) continue;
    cnt++; if (verbose) printf("  (%d,%d) (%d,%d) (%d,%d)\n", a1, b1, a2, b2, a3, b3);
  }
  return cnt;
}
int main(void) {
  int c[97]; for (int i = 0; i < 97; i++) c[i] = K4[i] - 'A';
  printf("K4 : AP aux deux endroits %d ; même ordre %d\n", stat(c, 0, 1), stat(c, 1, 0));
  int k0 = stat(c, 0, 0), k1 = stat(c, 1, 0); long N = 2000000, h0 = 0, h1 = 0, hu = 0;
#pragma omp parallel reduction(+:h0,h1,hu)
  { uint64_t s = 0x9E3779B97F4A7C15ULL * (omp_get_thread_num() + 7);
#pragma omp for
    for (long r = 0; r < N; r++) { int d[97]; memcpy(d, c, sizeof d);
      for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int x = d[i]; d[i] = d[j]; d[j] = x; }
      h0 += stat(d, 0, 0) >= k0; h1 += stat(d, 1, 0) >= k1;
      int e[97]; for (int i = 0; i < 97; i++) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; e[i] = s % 26; } hu += stat(e, 0, 0) >= k0; } }
  printf("mélanges de K4 (%ld) : P(AP ≥ K4) = %.2e ; P(même ordre ≥ K4) = %.3f ; lettres uniformes : P(AP ≥ K4) = %.2e\n", N, (double)h0 / N, (double)h1 / N, (double)hu / N);
  return 0;
}
