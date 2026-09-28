/* t17_autocle_avant.c — autoclé « vers l'avant » : la clé en i vient de la position i + L (audit « vision », 24/09/2026)
 *
 * Les autoclés testées (campagne du 22/09, T12) prennent la clé L positions AVANT. Une autoclé qui prend la clé
 * L positions APRÈS se chiffre de la fin vers le début et se déchiffre à rebours : procédé « réfléchi » (Scheidt 2007 :
 * « reflective »), « régénératif » (2011). Elle n'était pas couverte.
 *   clair  : k_i = σ(p_{i+L})  (contrainte seulement si i+L est dans un crib) ;
 *   chiffré : k_i = σ(c_{i+L}) (toujours connue) ;
 *   lettre-clé lue dans le MÊME alphabet σ que le tableau (Quagmire III, σ quelconque), ou dans un alphabet FIXE τ
 *   (A–Z, KRYPTOS, et leurs inverses) ; L = 1..96 ; VIG / BEAU / VARB.
 * Juge : lincsp.h (σ quelconque). Témoins : 40 chiffrés aléatoires de 97 lettres par cas compatible, et taux global.
 */
#include "lincsp.h"

static int inCrib(int i) { return (i >= 21 && i <= 33) || (i >= 63 && i <= 73); }
static int ptAt(int i) { for (int t = 0; t < NCRIB; t++) if (CRIBPOS[t] == i) return CRIBPT[t]; return -1; }
static int tau(int t, int x) { switch (t) { case 1: return x; case 2: return KAINV[x]; case 3: return 25 - x; default: return 25 - KAINV[x]; } }
static const char *TN[5] = {"σ (même alphabet)", "A-Z", "KRYPTOS", "A-Z inv", "KRYPTOS inv"};

/* t = 0 : clé lue dans σ ; t = 1..4 : clé lue dans τ fixe. Renvoie 1/0/−1 ; *nc = nombre de contraintes */
static int run(const int *ct, int src_ct, int t, int L, int mode, int *nc) {
  lc_reset(27); lc_limit = 5000000; int n = 0;
  for (int q = 0; q < NCRIB; q++) {
    int i = CRIBPOS[q], j = i + L, key;
    if (j > 96) continue;
    if (src_ct) key = ct[j]; else { if (!inCrib(j)) continue; key = ptAt(j); }
    int p = CRIBPT[q], c = ct[i];
    /* VIG : σc − σp − k = 0 ; BEAU : σc + σp − k = 0 ; VARB : σc − σp + k = 0 */
    int sg = mode == VARB ? 1 : -1;
    if (t == 0) { int v[3] = {c, p, key}, co[3] = {1, mode == BEAU ? 1 : -1, sg}; lc_add(3, v, co); }
    else { int v[3] = {c, p, 26}, co[3] = {1, mode == BEAU ? 1 : -1, sg * tau(t, key)}; lc_add(3, v, co); }
    n++;
  }
  *nc = n;
  if (n < 2) return 1;
  if (!lc_assign(26, 1)) return 0;
  return lc_solve();
}

int main(int argc, char **argv) {
  /* t17_un : taux au hasard précis pour UNE configuration : argv = src(0 clair,1 chiffré) τ mode L N graine */
  k4_init();
  int src = atoi(argv[1]), t = atoi(argv[2]), m = atoi(argv[3]), L = atoi(argv[4]), N = atoi(argv[5]);
  rng_s ^= 0x9E3779B97F4A7C15ULL * (atoi(argv[6]) + 1);
  int nc, r = run(CT, src, t, L, m, &nc), rnd[K4LEN], pass = 0, und = 0;
  for (int z = 0; z < N; z++) { for (int i = 0; i < K4LEN; i++) rnd[i] = rng() % 26; int nc2, rz = run(rnd, src, t, L, m, &nc2); pass += rz > 0; und += rz < 0; }
  printf("src=%d τ=%d %s L=%d (%d contraintes) : K4 %d ; témoins %d/%d (non tranchés %d)\n", src, t, MODENAME[m], L, nc, r, pass, N, und);
  return 0;
}
