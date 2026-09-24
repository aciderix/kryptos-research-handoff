/* t11_bloc7.c — « disque tourné une fois par bloc de 7 » (audit « vision », 24/09/2026)
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

static int run(int phi, const int *motif, int mode, long limit) {
  int rmin = 1000, nb;
  for (int t = 0; t < NCRIB; t++) { int r = (CRIBPOS[t] - phi + 7) / 7; if (r < rmin) rmin = r; }
  nb = 0;
  for (int t = 0; t < NCRIB; t++) { int r = (CRIBPOS[t] - phi + 7) / 7 - rmin; if (r + 1 > nb) nb = r + 1; }
  lc_reset(27 + nb);
  lc_limit = limit;
  for (int t = 0; t < NCRIB; t++) {
    int i = CRIBPOS[t], r = (i - phi + 7) / 7 - rmin, j = ((i - phi) % 7 + 7) % 7;
    /* VIG : σC − σP − b − m = 0 ; BEAU : σC + σP − b − m = 0 ; VARB : σC − σP + b + m = 0 */
    int sg = mode == VARB ? 1 : -1;
    int vars[4] = {C24[t], P24[t], 27 + r, ONE}, co[4] = {1, mode == BEAU ? 1 : -1, sg, sg * motif[j]};
    lc_add(4, vars, co);
  }
  if (!lc_assign(ONE, 1)) return 0;
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int motifs[28][7]; char names[28][24]; int nm = 0;
  for (int s = 0; s < 26; s++) { for (int j = 0; j < 7; j++) motifs[nm][j] = s * j; sprintf(names[nm], "s=%d", s); nm++; }
  { int kz[7] = {10, 17, 24, 15, 19, 14, 18}; for (int j = 0; j < 7; j++) { motifs[nm][j] = kz[j]; motifs[nm + 1][j] = kz[6 - j]; }
    strcpy(names[nm], "KRYPTOS(A-Z)"); strcpy(names[nm + 1], "SOTPYRK(A-Z)"); nm += 2; }
  long LIM = 5000000;
  /* contrôle positif : faux K4 fabriqué avec φ = 3, s = 1, σ aléatoire, b aléatoires */
  {
    int s[26], inv[26]; for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int t = s[i]; s[i] = s[j]; s[j] = t; }
    for (int i = 0; i < 26; i++) inv[s[i]] = i;
    int b[20]; for (int r = 0; r < 20; r++) b[r] = rng() % 26;
    for (int t = 0; t < NCRIB; t++) { int i = CRIBPOS[t], r = (i - 3 + 7) / 7, j = ((i - 3) % 7 + 7) % 7; P24[t] = CRIBPT[t]; C24[t] = inv[md(s[P24[t]] + b[r] + motifs[1][j])]; }
    printf("contrôle positif (φ=3, s=1, VIG) : %d (attendu 1)\n", run(3, motifs[1], VIG, LIM));
  }
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  int nk = 0, nund = 0, ncase = 0; double exp_null = 0;
  for (int m = 0; m < 3; m++) for (int phi = 0; phi < 7; phi++) for (int q = 0; q < nm; q++) {
    for (int t = 0; t < NCRIB; t++) C24[t] = CT[CRIBPOS[t]];
    int r = run(phi, motifs[q], m, LIM); ncase++;
    if (r < 0) { nund++; printf("%s φ=%d %s : non tranché\n", MODENAME[m], phi, names[q]); continue; }
    /* témoins pour tous les cas (taux de passage de la famille) */
    int nn = 0;
    for (int z = 0; z < 20; z++) { for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26; if (run(phi, motifs[q], m, LIM) > 0) nn++; }
    exp_null += nn / 20.0;
    if (r > 0) { nk++; printf("%s φ=%d %s : K4 COMPATIBLE ; témoins %d/20\n", MODENAME[m], phi, names[q], nn); }
  }
  printf("TOTAL : %d cas ; K4 compatible dans %d ; non tranchés %d ; attendu pour un chiffré aléatoire : %.1f\n", ncase, nk, nund, exp_null);
  return 0;
}
