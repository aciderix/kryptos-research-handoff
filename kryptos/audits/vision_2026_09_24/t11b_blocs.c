/* t11b_blocs.c — « disque tourné une fois par bloc de n », n = 2..30 sauf 7 (généralisation de T11, 24/09/2026)
 *
 * Blocs de n lettres commençant à la phase φ (0..n−1) ; dans le bloc r, à la place j : clé k_i = b_r + s·j,
 * b_r LIBRE par bloc, pas s = 0..25 fixé avant calcul (s = 0 : clé constante par bloc ; s = 1 : +1 par lettre).
 * Juge : Quagmire III, alphabet σ QUELCONQUE (lincsp.h), VIG / BEAU / VARB. Témoins : 20 chiffrés aléatoires par cas.
 * Découpage : argv[1] = tranche, argv[2] = nombre de tranches. Témoin global de famille : t11b_null.c.
 */
#include "lincsp.h"

static int P24[NCRIB], C24[NCRIB];
#define ONE 26 /* variable fixée à 1, porte les constantes */

static int BL = 7;
static int run(int phi, const int *motif, int mode, long limit) {
  int rmin = 1000, nb;
  for (int t = 0; t < NCRIB; t++) { int r = (CRIBPOS[t] - phi + BL) / BL; if (r < rmin) rmin = r; }
  nb = 0;
  for (int t = 0; t < NCRIB; t++) { int r = (CRIBPOS[t] - phi + BL) / BL - rmin; if (r + 1 > nb) nb = r + 1; }
  lc_reset(27 + nb);
  lc_limit = limit;
  for (int t = 0; t < NCRIB; t++) {
    int i = CRIBPOS[t], r = (i - phi + BL) / BL - rmin, j = ((i - phi) % BL + BL) % BL;
    /* VIG : σC − σP − b − m = 0 ; BEAU : σC + σP − b − m = 0 ; VARB : σC − σP + b + m = 0 */
    int sg = mode == VARB ? 1 : -1;
    int vars[4] = {C24[t], P24[t], 27 + r, ONE}, co[4] = {1, mode == BEAU ? 1 : -1, sg, sg * motif[j]};
    lc_add(4, vars, co);
  }
  if (!lc_assign(ONE, 1)) return 0;
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int slice = argc > 2 ? atoi(argv[1]) : 0, nslices = argc > 2 ? atoi(argv[2]) : 1;
  rng_s ^= 0x9E3779B97F4A7C15ULL * (slice + 1);
  long LIM = 5000000;
  /* motifs dans le bloc : s·j, s = 0..25 (s = 0 : clé constante par bloc ; s = 1 : +1 par lettre) */
  static int motif[26][32];
  for (int s0 = 0; s0 < 26; s0++) for (int j = 0; j < 32; j++) motif[s0][j] = s0 * j;
  long ncase = 0, nk = 0, nund = 0; double exp_null = 0; int idx = 0;
  for (BL = 2; BL <= 30; BL++) {
    if (BL == 7) continue; /* déjà fait (T11) */
    for (int m = 0; m < 3; m++) for (int phi = 0; phi < BL; phi++) for (int s0 = 0; s0 < 26; s0++) {
      if (idx++ % nslices != slice) continue;
      for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
      int r = run(phi, motif[s0], m, LIM); ncase++;
      if (r < 0) { nund++; printf("n=%d %s φ=%d s=%d : non tranché\n", BL, MODENAME[m], phi, s0); continue; }
      int nn = 0;
      for (int z = 0; z < 20; z++) { for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26; if (run(phi, motif[s0], m, LIM) > 0) nn++; }
      exp_null += nn / 20.0;
      if (r > 0) { nk++; printf("n=%d %s φ=%d s=%d : K4 COMPATIBLE ; témoins %d/20\n", BL, MODENAME[m], phi, s0, nn); }
    }
  }
  printf("TRANCHE %d/%d : %ld cas ; K4 compatible dans %ld ; non tranchés %ld ; attendu au hasard %.1f\n", slice, nslices, ncase, nk, nund, exp_null);
  return 0;
}
