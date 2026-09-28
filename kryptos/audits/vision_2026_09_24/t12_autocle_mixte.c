/* t12_autocle_mixte.c — autoclé « mixte » à l'écart L (audit « vision », 24/09/2026)
 *
 * Motif : l'excès de coïncidences à l'écart 7 (NSA 1992 : « interval 7 property » ; base 7 §8.2) est la signature
 * naturelle d'une dépendance entre lettres distantes de 7. L'autoclé où la lettre-clé est lue dans le MÊME alphabet
 * que le tableau est déjà éliminée ou au niveau du hasard (campagne du 22/09). Ici, la lettre-clé est lue dans un
 * alphabet FIXE τ différent de l'alphabet σ QUELCONQUE du tableau (famille non couverte) :
 *   autoclé chiffré : k_i = τ(c_{i−L}) ;  autoclé clair : k_i = τ(p_{i−L}) (seulement si i−L est dans un crib) ;
 *   τ ∈ {A–Z, KRYPTOS, A–Z inversé, KRYPTOS inversé} ; L = 1..96 ; VIG / BEAU / VARB ; juge eng_num (σ quelconque).
 * Témoins : chiffrés aléatoires de 97 lettres (la clé dépend du chiffré, donc on tire tout le chiffré).
 */
#include "k4lib.h"

static int tau_val(int t, int letter) {
  switch (t) { case 0: return letter; case 1: return KAINV[letter]; case 2: return 25 - letter; default: return 25 - KAINV[letter]; }
}
static const char *TNAME[4] = {"A-Z", "KRYPTOS", "A-Z inv", "KRYPTOS inv"};
static int inCrib(int i) { return (i >= 21 && i <= 33) || (i >= 63 && i <= 73); }
static int ptAt(int i) { for (int t = 0; t < NCRIB; t++) if (CRIBPOS[t] == i) return CRIBPT[t]; return -1; }

/* renvoie 1 compatible, 0 non ; nb = nombre de positions contraintes */
static int test(const int *ct, int auto_ct, int t, int L, int mode, int *nb) {
  int p[NCRIB], c[NCRIB], k[NCRIB], n = 0;
  for (int q = 0; q < NCRIB; q++) {
    int i = CRIBPOS[q], src;
    if (i - L < 0) continue;
    if (auto_ct) src = ct[i - L];
    else { if (!inCrib(i - L)) continue; src = ptAt(i - L); }
    p[n] = CRIBPT[q]; c[n] = ct[i]; k[n] = tau_val(t, src); n++;
  }
  *nb = n;
  if (n < 2) return 1;
  return eng_num(p, c, k, n, mode);
}

int main(void) {
  k4_init();
  int rnd[K4LEN];
  long tot = 0, nk = 0; double nullrate = 0; int NN = 50;
  for (int a = 0; a < 2; a++) for (int t = 0; t < 4; t++) for (int m = 0; m < 3; m++) for (int L = 1; L <= 96; L++) {
    int nb, r = test(CT, a, t, L, m, &nb);
    if (nb < 6) continue; /* trop peu de contraintes pour trancher */
    tot++;
    int pass = 0;
    for (int z = 0; z < NN; z++) { for (int i = 0; i < K4LEN; i++) rnd[i] = rng() % 26; int nb2; pass += test(rnd, a, t, L, m, &nb2); }
    nullrate += (double)pass / NN;
    if (r) { nk++; printf("%s τ=%s %s L=%d (%d contraintes) : K4 COMPATIBLE ; témoins %d/%d\n", a ? "chiffré" : "clair", TNAME[t], MODENAME[m], L, nb, pass, NN); }
  }
  printf("TOTAL : %ld cas tranchables ; K4 compatible dans %ld ; attendu pour un chiffré aléatoire : %.1f\n", tot, nk, nullrate);
  return 0;
}
