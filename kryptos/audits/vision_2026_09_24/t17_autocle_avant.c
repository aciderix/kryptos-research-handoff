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

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int rnd[K4LEN]; long tot = 0, comp = 0, und = 0; double expn = 0; int NN = 40;
  /* contrôle positif : faux K4 chiffré par autoclé avant sur le chiffré, L = 7, clé dans σ, VIG */
  {
    int s[26], inv[26], pt[K4LEN], ct[K4LEN];
    for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int x = s[i]; s[i] = s[j]; s[j] = x; }
    for (int i = 0; i < 26; i++) inv[s[i]] = i;
    for (int i = 0; i < K4LEN; i++) pt[i] = rng() % 26; for (int q = 0; q < NCRIB; q++) pt[CRIBPOS[q]] = CRIBPT[q];
    for (int i = K4LEN - 1; i >= 0; i--) { int k = i + 7 <= 96 ? s[ct[i + 7]] : (int)(rng() % 26); ct[i] = inv[md(s[pt[i]] + k)]; }
    int nc; printf("contrôle positif (chiffré, L=7, σ, VIG) : %d (attendu 1)\n", run(ct, 1, 0, 7, VIG, &nc));
  }
  for (int src = 0; src < 2; src++) for (int t = 0; t < 5; t++) for (int m = 0; m < 3; m++) for (int L = 1; L <= 96; L++) {
    int nc, r = run(CT, src, t, L, m, &nc);
    if (nc < 6) continue;
    tot++;
    if (r < 0) { und++; continue; }
    int pass = 0;
    for (int z = 0; z < NN; z++) { for (int i = 0; i < K4LEN; i++) rnd[i] = rng() % 26; int nc2; pass += run(rnd, src, t, L, m, &nc2) > 0; }
    expn += (double)pass / NN;
    if (r > 0) { comp++; printf("%s clé=%s %s L=%d (%d contraintes) : K4 COMPATIBLE ; témoins %d/%d\n", src ? "chiffré" : "clair", TN[t], MODENAME[m], L, nc, pass, NN); }
  }
  printf("TOTAL : %ld cas tranchables ; K4 compatible dans %ld ; non tranchés %ld ; attendu au hasard %.1f\n", tot, comp, und, expn);
  return 0;
}
