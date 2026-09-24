/* t5_fold.c — la « pliure » de K4 : les deux cribs sont en miroir autour du centre (position 48).
 * Fait : 23+73 = 24+72 = … = 33+63 = 96. Les positions 23–33 (ASTNORTHEAST sans le E initial… soit STNORTHEAST)
 * et 73–63 (KCOLCNILREB) se font face quand on plie le texte en deux : 11 paires dont clair ET chiffré sont connus.
 * Familles « pliure » testées (clé = lettre en face), fixées avant calcul, σ quelconque (même alphabet), VIG/BEAU/VARB :
 *   F1 : clé(i) = clair(96-i), pour TOUTES les positions connues ;
 *   F2 : clé(j) = clair(96-j) pour la seconde moitié seulement (j > 48) ;
 *   F3 : clé(i) = clair(96-i) pour la première moitié seulement (i < 48) ;
 *   F4 : clé(j) = chiffré(96-j) pour j > 48 seulement (« autoclé réfléchie ») ;
 *   F5 : clé(i) = chiffré(96-i) pour i < 48 seulement ;
 *   F6 : pliure en un autre centre s (clé(i) = chiffré(s-i)), s = 0..192, toutes positions : = clé courante « K4 inversé ».
 * Juge : eng_let (σ quelconque, lettre-clé lue dans σ). Témoin : 2 000 chiffrés aléatoires par famille.
 */
#include "k4lib.h"

static int PTF[K4LEN]; /* -1 = inconnu */

static int run(const int *ct, int fam, int mode, int s) {
  int p[NCRIB], c[NCRIB], k[NCRIB], n = 0;
  for (int t = 0; t < NCRIB; t++) {
    int i = CRIBPOS[t], j = (fam == 6 ? s : 96) - i, kl = -1;
    if (j < 0 || j >= K4LEN) continue;
    switch (fam) {
      case 1: kl = PTF[j]; break;
      case 2: if (i > 48) kl = PTF[j]; break;
      case 3: if (i < 48) kl = PTF[j]; break;
      case 4: if (i > 48) kl = ct[j]; break;
      case 5: if (i < 48) kl = ct[j]; break;
      case 6: kl = ct[j]; break;
    }
    if (kl < 0) continue;
    p[n] = CRIBPT[t]; c[n] = ct[i]; k[n] = kl; n++;
  }
  if (n < 3) return -1;
  return eng_let(p, c, k, n, mode);
}

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  el_limit = 2000000;
  k4_init();
  for (int i = 0; i < K4LEN; i++) PTF[i] = -1;
  for (int t = 0; t < NCRIB; t++) PTF[CRIBPOS[t]] = CRIBPT[t];
  printf("paires en miroir (i, 96-i) dont les deux clairs sont connus :\n");
  for (int t = 0; t < 13; t++) { int i = CRIBPOS[t], j = 96 - i; if (PTF[j] >= 0) printf("  %2d %c/%c  <->  %2d %c/%c\n", i, 'A' + PTF[i], K4CT[i], j, 'A' + PTF[j], K4CT[j]); }
  for (int fam = 1; fam <= 6; fam++) for (int m = 0; m < 3; m++) {
    int k4 = 0, tot = 0, und = 0;
    if (fam == 6) { for (int s = 0; s <= 192; s++) { int r = run(CT, fam, m, s); if (r == -1 && el_abort) und++; else if (r >= 0) { tot++; k4 += r; } } }
    else { int r = run(CT, fam, m, 0); if (r < 0) und++; else k4 = r; tot = 1; }
    long nul = 0, nund = 0, N = fam == 6 ? 20 : 200;
    for (int z = 0; z < N; z++) {
      int rc[K4LEN]; for (int i = 0; i < K4LEN; i++) rc[i] = rng() % 26;
      if (fam == 6) { for (int s = 0; s <= 192; s++) { int r = run(rc, fam, m, s); if (r > 0) nul++; if (r < 0 && el_abort) nund++; } }
      else { int r = run(rc, fam, m, 0); nul += r > 0; nund += r < 0; }
    }
    printf("F%d %s : K4 compatible %d/%d (non tranché %d) ; témoin %ld/%ld compatibles (non tranchés %ld)\n", fam, MODENAME[m], k4, tot, und, nul, N, nund);
    fflush(stdout);
  }
  return 0;
}
