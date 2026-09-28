/* t4_compound_anyalpha.c — deux mots-clés superposés, ALPHABET QUELCONQUE (Quagmire III, 26!).
 * k[i] = a[i mod p1] + b[i mod p2] ; σ(C) = σ(P) + k (VIG), σ(C) = k − σ(P) (BEAU), σ(C) = σ(P) − k (VARB).
 * Validation préalable : cas b absent (clé périodique simple) → doit reproduire le résultat SAT du dépôt
 * (QIII-Vig : aucune période 1–26 compatible ; Beau : 23, 26 compatibles parmi 1–26).
 * Symétries : σ(E) = 0 (translation absorbée par a), b[0] = 0 (jauge).
 * Témoin : 20 chiffrés aléatoires par couple (p1, p2) où K4 est compatible.
 */
#include "lincsp.h"
#include <time.h>

static int P24[NCRIB], C24[NCRIB];

static int run_model(int p1, int p2, int mode, long limit) {
  lc_reset(26 + p1 + p2);
  lc_limit = limit;
  for (int t = 0; t < NCRIB; t++) {
    int i = CRIBPOS[t], kv[2], nk = 0;
    kv[nk++] = 26 + i % p1;
    if (p2) kv[nk++] = 26 + p1 + i % p2;
    lc_add_enc(P24[t], C24[t], kv, nk, mode);
  }
  if (!lc_assign('E' - 'A', 0)) return 0;
  if (p2 && !lc_assign(26 + p1, 0)) return 0;
  return lc_solve();
}

int main(int argc, char **argv) {
  int SUMMAX = argc > 1 ? atoi(argv[1]) : 20;
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  printf("Validation (clé périodique simple, alphabet quelconque) :\n");
  for (int m = 0; m < 2; m++) {
    printf("  %s compatibles p=1..26 :", MODENAME[m]);
    for (int p = 1; p <= 26; p++) { int r = run_model(p, 0, m, 50000000); if (r) printf(" %d%s", p, r < 0 ? "?" : ""); }
    printf("\n");
  }
  fflush(stdout);
  /* contrôle positif : faux K4, σ aléatoire, clé = a[i%7] + b[i%10] */
  {
    int s[26], inv[26]; for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int t = s[i]; s[i] = s[j]; s[j] = t; }
    for (int i = 0; i < 26; i++) inv[s[i]] = i;
    int a[7], b[10]; for (int i = 0; i < 7; i++) a[i] = rng() % 26; for (int i = 0; i < 10; i++) b[i] = rng() % 26;
    int save[NCRIB]; memcpy(save, C24, sizeof save);
    for (int t = 0; t < NCRIB; t++) { int i = CRIBPOS[t]; C24[t] = inv[md(s[P24[t]] + a[i % 7] + b[i % 10])]; }
    printf("contrôle positif (7,10) VIG : %d (attendu 1 ; -1 = limite)\n", run_model(7, 10, VIG, 20000000));
    memcpy(C24, save, sizeof save);
  }
  long LIMIT = 2000000;
  for (int m = 0; m < 3; m++) {
    int nk = 0, nund = 0, nsev = 0; double expect = 0;
    for (int p1 = 2; p1 < SUMMAX; p1++) for (int p2 = p1 + 1; p1 + p2 <= SUMMAX; p2++) {
      if (p2 % p1 == 0) continue; /* b absorbe a : cas périodique simple déjà traité */
      clock_t t0 = clock();
      memcpy(C24, C24, 0);
      for (int t = 0; t < NCRIB; t++) C24[t] = CT[CRIBPOS[t]];
      int r = run_model(p1, p2, m, LIMIT);
      if (r < 0) { nund++; printf("  %s (%d,%d) : non tranché (limite)\n", MODENAME[m], p1, p2); continue; }
      if (!r) continue;
      nk++;
      /* témoin */
      int nn = 0, nu = 0;
      for (int z = 0; z < 10; z++) {
        for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
        int rz = run_model(p1, p2, m, LIMIT); if (rz > 0) nn++; if (rz < 0) nu++;
      }
      double rate = nn / 10.0; expect += rate;
      if (nn <= 1) nsev++;
      printf("  %s (%d,%d) ppcm %d : K4 COMPATIBLE ; témoins %d/10 (non tranchés %d) %.1fs\n", MODENAME[m], p1, p2, p1 * p2, nn, nu, (double)(clock() - t0) / CLOCKS_PER_SEC);
      fflush(stdout);
    }
    printf("%s : couples compatibles pour K4 = %d ; sévères (témoin ≤ 1/10) = %d ; non tranchés = %d\n", MODENAME[m], nk, nsev, nund);
    fflush(stdout);
  }
  return 0;
}
