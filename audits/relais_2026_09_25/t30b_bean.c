/* t30b_bean.c — le cas « période 8, Beaufort, saut +2 dans BERLINCLOCK » (T30) tient-il seulement à l'égalité de Bean ?
 * Témoins conditionnés : chiffrés aléatoires aux 24 positions, avec c27 = c65 imposé (comme K4). Alphabet quelconque (lincsp). */
#define LC_MAXV 64
#define LC_THREADPRIVATE
#include "../vision_2026_09_24/lincsp.h"
#include <omp.h>
static int run(const int *C24, int p, int x, int s, int mode) {
  lc_reset(26 + p); lc_limit = 5000000;
  for (int t = 0; t < NCRIB; t++) { int i = CRIBPOS[t], kv[1] = {26 + (i + (i >= x ? s : 0) + 2 * p) % p}; lc_add_enc(CRIBPT[t], C24[t], kv, 1, mode); }
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}
int main(void) {
  k4_init(); int C0[NCRIB]; for (int t = 0; t < NCRIB; t++) C0[t] = CT[CRIBPOS[t]];
  int t27 = 6, t65 = 13 + 2;   /* indices de crib des positions 27 et 65 */
  for (int x = 64; x <= 67; x++) {
    int r = run(C0, 8, x, 2, BEAU); long N = 4000, pu = 0, pb = 0;
#pragma omp parallel for reduction(+:pu,pb)
    for (long z = 0; z < N; z++) { uint64_t s = 0x9E3779B97F4A7C15ULL * (z + 1 + 100000 * x); int C[NCRIB];
      for (int t = 0; t < NCRIB; t++) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; C[t] = s % 26; }
      pu += run(C, 8, x, 2, BEAU) > 0; C[t65] = C[t27]; pb += run(C, 8, x, 2, BEAU) > 0; }
    printf("p=8 BEAU saut +2 en %d : K4 %d ; témoins uniformes %.4f ; témoins avec c27 = c65 : %.4f\n", x, r, (double)pu / N, (double)pb / N);
  }
  return 0;
}
