/* t30_saut_de_phase.c — clé périodique où Sanborn « perd sa place » dans le mot-clé : un SAUT DE PHASE persistant de s
 * (±1..±3) à partir d'une position x, y compris À L'INTÉRIEUR d'un crib (relais du 25/09/2026).
 * Motif : dans K2, l'X omis a décalé la clé pour tout le reste ; dans le petit fragment, la clé glisse d'une lettre (§9.4).
 * Le registre (« crib décalé ») couvre les sauts ENTRE les cribs ; ici x parcourt 22..73 (dans ENE, entre, dans BERLINCLOCK),
 * et l'on teste aussi DEUX sauts (un dans chaque crib). Clé : k_i = K[(i + s·[i ≥ x]) mod p], p = 2..26.
 * Juge : Quagmire III, alphabet σ QUELCONQUE (lincsp.h), VIG / BEAU / VARB ; témoins : chiffrés aléatoires, même balayage.
 * Usage : t30 [NNULL] [pmax_deux_sauts]   (deux sauts : périodes 2..pmax seulement, 7 par défaut)
 */
#define LC_MAXV 64
#define LC_THREADPRIVATE   /* lincsp.h : une copie des variables statiques par thread */
#include "../vision_2026_09_24/lincsp.h"
#include <omp.h>

static int P24[NCRIB];
static int run(const int *C24, int p, int x1, int s1, int x2, int s2, int mode) {
  lc_reset(26 + p); lc_limit = 2000000;
  for (int t = 0; t < NCRIB; t++) {
    int i = CRIBPOS[t], sh = (i >= x1 ? s1 : 0) + (i >= x2 ? s2 : 0);
    int kv[1] = {26 + (i + sh + 2 * p) % p};  /* indice de clé : position mod p (et non mod 26) */
    lc_add_enc(P24[t], C24[t], kv, 1, mode);
  }
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int NN = argc > 1 ? atoi(argv[1]) : 50, P2 = argc > 2 ? atoi(argv[2]) : 7;
  for (int t = 0; t < NCRIB; t++) P24[t] = CRIBPT[t];
  /* liste des cas : un saut (x dans 22..73, hors 34..62 déjà couvert mais gardé pour contrôle) ou deux sauts (un par crib) */
  typedef struct { int p, x1, s1, x2, s2, mode; } Case;
  int cap = 400000, nc = 0; Case *cs = malloc(sizeof(Case) * cap);
  int PMAX = getenv("PMAX") ? atoi(getenv("PMAX")) : 26;
  for (int p = 2; p <= PMAX; p++) for (int mode = 0; mode < 3; mode++) {
    for (int x = 22; x <= 73; x++) for (int s = -3; s <= 3; s++) if (s) cs[nc++] = (Case){p, x, s, 999, 0, mode};
    if (p <= P2) for (int x = 22; x <= 33; x++) for (int s = -3; s <= 3; s++) if (s) for (int y = 64; y <= 73; y++) for (int u = -3; u <= 3; u++) if (u)
      cs[nc++] = (Case){p, x, s, y, u, mode};
  }
  printf("%d cas\n", nc);
  /* contrôle positif : période 7, saut +1 en 69 (dans BERLINCLOCK), σ aléatoire */
  { int C[NCRIB], s[26], inv[26], K[7]; for (int i = 0; i < 26; i++) s[i] = i;
    for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int q = s[i]; s[i] = s[j]; s[j] = q; } for (int i = 0; i < 26; i++) inv[s[i]] = i;
    for (int j = 0; j < 7; j++) K[j] = rng() % 26;
    for (int t = 0; t < NCRIB; t++) { int i = CRIBPOS[t], k = K[(i + (i >= 69 ? 1 : 0)) % 7]; C[t] = inv[md(s[P24[t]] + k)]; }
    printf("contrôle positif (p=7, saut +1 en 69, VIG) : %d (attendu 1)\n", run(C, 7, 69, 1, 999, 0, VIG)); }
  int C0[NCRIB]; for (int t = 0; t < NCRIB; t++) C0[t] = CT[CRIBPOS[t]];
  long comp = 0, und = 0; double expn = 0; long nfull = 0;
  int (*NC)[NCRIB] = malloc(sizeof(int[NCRIB]) * NN);
  for (int z = 0; z < NN; z++) for (int t = 0; t < NCRIB; t++) NC[z][t] = rng() % 26;
#pragma omp parallel for schedule(dynamic, 64) reduction(+:comp,und,expn,nfull)
  for (int c = 0; c < nc; c++) {
    Case k = cs[c];
    int r = run(C0, k.p, k.x1, k.s1, k.x2, k.s2, k.mode);
    int pass = 0; for (int z = 0; z < NN; z++) pass += run(NC[z], k.p, k.x1, k.s1, k.x2, k.s2, k.mode) > 0;
    expn += (double)pass / NN; nfull++;
    if (r < 0) und++;
    if (r > 0) { comp++;
#pragma omp critical
      printf("K4 COMPATIBLE p=%d saut %+d en %d%s %s ; témoins %d/%d\n", k.p, k.s1, k.x1, k.x2 < 999 ? "" : "", MODENAME[k.mode], pass, NN);
      if (k.x2 < 999) {
#pragma omp critical
        printf("   (second saut %+d en %d)\n", k.s2, k.x2); }
    }
  }
  printf("TOTAL : %ld cas ; K4 compatible %ld ; non tranchés %ld ; attendu au hasard %.1f\n", nfull, comp, und, expn);
  return 0;
}
