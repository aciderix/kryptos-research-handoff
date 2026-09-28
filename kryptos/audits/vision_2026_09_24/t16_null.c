/* t16_cle_transposee.c — la méthode de K3 appliquée à la CLÉ (audit « vision », 24/09/2026)
 *
 * Sanborn : « matrices tournées et retournées » (2019), « deux matrices ». Les cribs positionnels interdisent une
 * transposition finale du TEXTE ; mais la transposition peut porter sur la CLÉ (base 8, §5.1).
 * Famille : mot-clé numérique a[0..L−1] INCONNU (L = 1..14), répété, écrit en lignes dans une grille de largeur w = 7
 * (N = 97 ou 98 cases), puis relu par colonnes dans un ordre π (les 7! = 5040 ordres, dont KRYPTOS = 0362514
 * utilisé pour K3) ; variante inverse : écrit par colonnes dans l'ordre π, relu en lignes.
 * La clé de la position i de K4 est alors a[classe(i)], classe(i) = (indice de la case lue au rang i) mod L.
 * Juge : Quagmire III, alphabet σ QUELCONQUE (lincsp.h), VIG / BEAU / VARB.
 * Découpage : argv[1] = tranche, argv[2] = nombre de tranches (sur π), pour lancer plusieurs processus.
 * Témoins : pour chaque cas compatible, 20 chiffrés aléatoires (lettres i.i.d. aux positions de cribs).
 */
#include "lincsp.h"

static int P24[NCRIB], C24[NCRIB];

/* ordre de lecture : rd[i] = indice ligne-par-ligne de la case lue au rang i */
static void build(int w, int N, const int *pi, int inverse, int *rd) {
  int h = (N + w - 1) / w, full = N % w == 0 ? w : N % w; /* colonnes 0..full−1 ont h cases, les autres h−1 */
  int k = 0;
  if (!inverse) {
    for (int q = 0; q < w; q++) { int col = pi[q], hc = col < full ? h : h - 1;
      for (int r = 0; r < hc; r++) rd[k++] = r * w + col; }
  } else {
    /* écrit par colonnes dans l'ordre π : la case (r, col) reçoit le rang colonne-par-colonne ; on relit en lignes */
    int rank[256]; int t = 0;
    for (int q = 0; q < w; q++) { int col = pi[q], hc = col < full ? h : h - 1;
      for (int r = 0; r < hc; r++) rank[r * w + col] = t++; }
    for (int m = 0; m < N; m++) rd[k++] = rank[m];
  }
}

static int run(const int *cls, int L, int mode, long limit) {
  lc_reset(26 + L); lc_limit = limit;
  for (int t = 0; t < NCRIB; t++) { int kv[1] = {26 + cls[CRIBPOS[t]]}; lc_add_enc(P24[t], C24[t], kv, 1, mode); }
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}

int main(int argc, char **argv) {
  /* t16_null : taux de compatibilité au hasard (π tiré au hasard, chiffré aléatoire aux positions de cribs) */
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int slice = argc > 2 ? atoi(argv[1]) : 0, nslices = argc > 2 ? atoi(argv[2]) : 1, R = argc > 3 ? atoi(argv[3]) : 200;
  rng_s ^= 0xD1B54A32D192ED03ULL * (slice + 7);
  int idx = 0;
  for (int L = 1; L <= 14; L++) for (int N = 97; N <= 98; N++) for (int inv = 0; inv < 2; inv++) for (int m = 0; m < 3; m++) {
    if (idx++ % nslices != slice) continue;
    int hit = 0, und = 0;
    for (int z = 0; z < R; z++) {
      int pi[7] = {0, 1, 2, 3, 4, 5, 6};
      for (int i = 6; i > 0; i--) { int j = rng() % (i + 1); int x = pi[i]; pi[i] = pi[j]; pi[j] = x; }
      int rd[98], cls[98]; build(7, N, pi, inv, rd); for (int i = 0; i < N; i++) cls[i] = rd[i] % L;
      for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = rng() % 26; }
      int r = run(cls, L, m, 2000000); hit += r > 0; und += r < 0;
    }
    printf("NULL L=%d N=%d %s %s : %d/%d compatibles (non tranchés %d)\n", L, N, inv ? "inverse" : "direct", MODENAME[m], hit, R, und);
  }
  return 0;
}
