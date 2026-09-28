/* t11b_blocs.c — « disque tourné une fois par bloc de n », n = 2..30 (généralisation de T11) (audit « vision », 24/09/2026)
 *
 * Motif : le « 7 » de K4 (doublets en colonnes 4–5 des blocs de 7, coïncidences à l'écart 7, base 7 §8.2)
 * n'est pas une substitution périodique (paire 65/72). Famille serrée qui garde une structure de 7 sans période :
 *   blocs de 7 commençant à la phase φ (0..6) ; dans le bloc r, à la place j (0..6) :
 *   clé k_i = b_r + motif[j], b_r LIBRE par bloc (le disque est tourné d'une quantité quelconque entre deux blocs),
 *   motif fixé AVANT calcul :
 *     s·j pour s = 0..25 (s = 0 : clé constante par bloc ; s = 1 : +1 par lettre, soit KRYPTOS lu dans KA) ;
 *     KRYPTOS lu dans A–Z (10,17,24,15,19,14,18) et à l'envers.
 *   Cela couvre aussi la clé progressive globale avec saut libre tous les 7 (s·i = s·j + constante de bloc).
 * Juge : Quagmire III, alphabet σ QUELCONQUE (lincsp.h), VIG / BEAU / VARB.
 * Témoins : 20 chiffrés aléatoires (lettres i.i.d. aux 24 positions de cribs) par cas compatible, et taux global.
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
  /* t11b_null : pour des chiffrés aléatoires, nombre de pas s (0..25) compatibles par (n, φ, convention) ;
     on retient, par chiffré, le maximum sur les sous-familles, et le compte pour (n = 4, φ = 1, BEAU). */
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int slice = atoi(argv[1]), nslices = atoi(argv[2]), NC = atoi(argv[3]), nmax = atoi(argv[4]);
  rng_s ^= 0xA0761D6478BD642FULL * (slice + 3);
  static int motif[26][32];
  for (int s0 = 0; s0 < 26; s0++) for (int j = 0; j < 32; j++) motif[s0][j] = s0 * j;
  for (int z = 0; z < NC; z++) {
    int isK4 = (slice == 0 && z == 0);
    for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = isK4 ? CT[CRIBPOS[t]] : (int)(rng() % 26); }
    int best = 0, bestn = 0, bestphi = 0, bestm = 0, c41 = 0;
    for (BL = 2; BL <= nmax; BL++) for (int m = 0; m < 3; m++) for (int phi = 0; phi < BL; phi++) {
      int cnt = 0; for (int s0 = 0; s0 < 26; s0++) cnt += run(phi, motif[s0], m, 2000000) > 0;
      if (BL == 4 && phi == 1 && m == BEAU) c41 = cnt;
      if (BL >= 4 && cnt > best) { best = cnt; bestn = BL; bestphi = phi; bestm = m; }
    }
    printf("%s max(n>=4)=%d (n=%d φ=%d %s) ; n=4 φ=1 BEAU : %d\n", isK4 ? "K4" : "ALEA", best, bestn, bestphi, MODENAME[bestm], c41);
  }
  return 0;
}
