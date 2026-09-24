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
 * Témoins : programme séparé t16_null.c (π et chiffré tirés au hasard) ; on compare les TOTAUX de compatibilités.
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
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int slice = argc > 2 ? atoi(argv[1]) : 0, nslices = argc > 2 ? atoi(argv[2]) : 1;
  rng_s ^= 0x9E3779B97F4A7C15ULL * (slice + 1);
  int pi[7] = {0, 1, 2, 3, 4, 5, 6}, idx = 0;
  long tried = 0, comp = 0, undec = 0;
  /* contrôle positif (tranche 0) : faux K4 fabriqué avec π = KRYPTOS, L = 10, N = 97, sens direct */
  if (slice == 0) {
    int kp[7] = {0, 3, 6, 2, 5, 1, 4}, rd[98], cls[98], s[26], inv[26], a[10];
    build(7, 97, kp, 0, rd); for (int i = 0; i < 97; i++) cls[i] = rd[i] % 10;
    for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int x = s[i]; s[i] = s[j]; s[j] = x; }
    for (int i = 0; i < 26; i++) inv[s[i]] = i; for (int j = 0; j < 10; j++) a[j] = rng() % 26;
    for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = inv[md(s[P24[t]] + a[cls[CRIBPOS[t]]])]; }
    printf("contrôle positif (π=KRYPTOS, L=10, VIG) : %d (attendu 1)\n", run(cls, 10, VIG, 20000000));
  }
  do {
    if (idx++ % nslices != slice) continue;
    for (int N = 97; N <= 98; N++) for (int inv = 0; inv < 2; inv++) {
      int rd[98]; build(7, N, pi, inv, rd);
      for (int L = 1; L <= 14; L++) {
        int cls[98]; for (int i = 0; i < N; i++) cls[i] = rd[i] % L;
        for (int m = 0; m < 3; m++) {
          for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
          int r = run(cls, L, m, 2000000); tried++;
          if (r < 0) { undec++; continue; }
          if (r == 0) continue;
          comp++;
          printf("COMPATIBLE π=%d%d%d%d%d%d%d N=%d %s L=%d %s\n", pi[0], pi[1], pi[2], pi[3], pi[4], pi[5], pi[6], N, inv ? "inverse" : "direct", L, MODENAME[m]);
        }
      }
    }
  } while (({ int i = 5; while (i >= 0 && pi[i] > pi[i + 1]) i--; int ok = i >= 0;
              if (ok) { int j = 6; while (pi[j] < pi[i]) j--; int x = pi[i]; pi[i] = pi[j]; pi[j] = x;
                        for (int l = i + 1, r = 6; l < r; l++, r--) { x = pi[l]; pi[l] = pi[r]; pi[r] = x; } } ok; }));
  printf("TRANCHE %d/%d : %ld cas, %ld compatibles, %ld non tranchés\n", slice, nslices, tried, comp, undec);
  return 0;
}
