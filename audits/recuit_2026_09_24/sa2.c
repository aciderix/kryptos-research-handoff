/* sa2.c — recuit sur l'alphabet seul ; la clé structurée est DÉDUITE des cribs par vote (audit « recuit », 24/09/2026).
 *
 * Modèle : clé(i) = m[i mod p] + a[seg(i)], seg(i) = (i + o) / W (W = 0 : un seul segment).
 * Types : Q3 (σ des deux côtés, K1–K2) ; Q2 (clair A–Z, chiffré σ : Cyrillic Projector) ; Q1 (clair σ, chiffré A–Z).
 * Conventions : VIG σc(c) = σp(p) + k ; BEAU σc(c) = k − σp(p) ; VARB σc(c) = σp(p) − k.
 * Pour un alphabet donné, chaque lettre des cribs impose une valeur de clé K_t ; on ajuste (m, a) pour maximiser le
 * nombre d'accords (vote alterné), les éléments sans crib sont réglés par les quadrigrammes. Les désaccords sont
 * permis (erreurs de Sanborn) mais coûtent wc chacun.
 * Usage : sa2 qg.bin CT Q mode p W o iters restarts seed [wc]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
static float *QG;
static int N, CTv[128], cribP[128], iscrib[128], cpos[32], ncr;
static int Q, MODE, P, W, O, NS, SEG[128];
static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static inline double urand(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
typedef struct { int s[26], inv[26]; int m[64], a[64]; } St;   /* s : alphabet libre (valeur de chaque lettre) */
static double wc = 6.0;
/* valeurs côté clair / chiffré */
static inline int vp(const St *S, int L) { return Q == 2 ? L : S->s[L]; }
static inline int vc(const St *S, int L) { return Q == 3 ? L : S->s[L]; }
static inline int lp(const St *S, int v) { return Q == 2 ? v : S->inv[v]; }
static inline int keyof(int c, int p) { return MODE == 0 ? md(c - p) : MODE == 1 ? md(c + p) : md(p - c); }
static inline int plainof(int c, int k) { return MODE == 0 ? md(c - k) : MODE == 1 ? md(k - c) : md(c + k); }
static int hasm[64], hasa[64];
static int fitkey(St *S) {
  int K[32];
  for (int t = 0; t < ncr; t++) { int i = cpos[t]; K[t] = keyof(vc(S, CTv[i]), vp(S, cribP[i])); }
  for (int s = 0; s < NS; s++) if (hasa[s]) S->a[s] = 0;
  int agree = 0;
  for (int rep = 0; rep < 3; rep++) {
    for (int j = 0; j < P; j++) if (hasm[j]) { int h[26] = {0}, b = S->m[j], bh = -1;
      for (int t = 0; t < ncr; t++) if (cpos[t] % P == j) h[md(K[t] - S->a[SEG[cpos[t]]])]++;
      for (int v = 0; v < 26; v++) if (h[v] > bh) { bh = h[v]; b = v; } S->m[j] = b; }
    for (int s = 1; s < NS; s++) if (hasa[s]) { int h[26] = {0}, b = S->a[s], bh = -1;
      for (int t = 0; t < ncr; t++) if (SEG[cpos[t]] == s) h[md(K[t] - S->m[cpos[t] % P])]++;
      for (int v = 0; v < 26; v++) if (h[v] > bh) { bh = h[v]; b = v; } S->a[s] = b; }
  }
  for (int t = 0; t < ncr; t++) { int i = cpos[t]; if (md(S->m[i % P] + S->a[SEG[i]]) == K[t]) agree++; }
  return agree;
}
static void decrypt(const St *S, int *pt) { for (int i = 0; i < N; i++) pt[i] = lp(S, plainof(vc(S, CTv[i]), md(S->m[i % P] + S->a[SEG[i]]))); }
static double qscore(const int *pt) { double s = 0; for (int i = 0; i + 3 < N; i++) s += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; return s; }
static double full(St *S, int *agree) {
  int a = fitkey(S); int pt[128]; decrypt(S, pt); if (agree) *agree = a; return qscore(pt) + wc * a;
}
/* éléments de clé sans crib : réglés par les quadrigrammes */
static void tune_free(St *S) {
  for (int j = 0; j < P; j++) if (!hasm[j]) { double b = -1e18; int bv = 0; for (int v = 0; v < 26; v++) { S->m[j] = v; int pt[128]; decrypt(S, pt); double q = qscore(pt); if (q > b) { b = q; bv = v; } } S->m[j] = bv; }
  for (int s = 1; s < NS; s++) if (!hasa[s]) { double b = -1e18; int bv = 0; for (int v = 0; v < 26; v++) { S->a[s] = v; int pt[128]; decrypt(S, pt); double q = qscore(pt); if (q > b) { b = q; bv = v; } } S->a[s] = bv; }
}
int main(int argc, char **argv) {
  if (argc < 11) { fprintf(stderr, "usage\n"); return 1; }
  FILE *f = fopen(argv[1], "rb"); QG = malloc(sizeof(float) * 456976); if (fread(QG, sizeof(float), 456976, f) != 456976) return 2; fclose(f);
  const char *ct = argv[2]; N = strlen(ct); for (int i = 0; i < N; i++) CTv[i] = ct[i] - 'A';
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { iscrib[21 + i] = 1; cribP[21 + i] = e[i] - 'A'; cpos[ncr++] = 21 + i; }
  for (int i = 0; i < 11; i++) { iscrib[63 + i] = 1; cribP[63 + i] = b[i] - 'A'; cpos[ncr++] = 63 + i; }
  Q = !strcmp(argv[3], "Q3") ? 0 : !strcmp(argv[3], "Q2") ? 2 : 3;   /* Q4 non traité ici */
  MODE = !strcmp(argv[4], "VIG") ? 0 : !strcmp(argv[4], "BEAU") ? 1 : 2;
  P = atoi(argv[5]); W = atoi(argv[6]); O = atoi(argv[7]);
  long iters = atol(argv[8]); int R = atoi(argv[9]); rs = 0x9E3779B97F4A7C15ULL * (atol(argv[10]) + 1);
  if (argc > 11) wc = atof(argv[11]);
  for (int i = 0; i < N; i++) SEG[i] = W ? (i + O) / W : 0;
  NS = SEG[N - 1] + 1;
  for (int t = 0; t < ncr; t++) { hasm[cpos[t] % P] = 1; hasa[SEG[cpos[t]]] = 1; }
  double T0 = getenv("T0") ? atof(getenv("T0")) : 4.0, T1 = getenv("T1") ? atof(getenv("T1")) : 0.2;
  St best; double bestsc = -1e18; int bestag = 0;
  for (int r = 0; r < R; r++) {
    St S; memset(&S, 0, sizeof S);
    for (int i = 0; i < 26; i++) S.s[i] = i; for (int i = 25; i > 0; i--) { int j = rnd() % (i + 1); int t = S.s[i]; S.s[i] = S.s[j]; S.s[j] = t; }
    for (int i = 0; i < 26; i++) S.inv[S.s[i]] = i;
    int ag; double cur = full(&S, &ag); St bs = S; double bsc = cur;
    for (long it = 0; it < iters; it++) {
      double T = T0 * pow(T1 / T0, (double)it / iters);
      St X = S; int u = rnd() % 26, v = rnd() % 26; if (u == v) continue;
      int t = X.s[u]; X.s[u] = X.s[v]; X.s[v] = t; X.inv[X.s[u]] = u; X.inv[X.s[v]] = v;
      if ((it & 255) == 0) tune_free(&X);
      double sc = full(&X, &ag);
      if (sc >= cur || urand() < exp((sc - cur) / T)) { S = X; cur = sc; if (cur > bsc) { bsc = cur; bs = S; } }
    }
    tune_free(&bs); bsc = full(&bs, &ag);
    if (bsc > bestsc) { bestsc = bsc; best = bs; bestag = ag; }
  }
  int pt[128]; decrypt(&best, pt); double q = qscore(pt);
  double qo = 0; int no = 0;
  for (int i = 0; i + 3 < N; i++) { int in = 0; for (int d = 0; d < 4; d++) in |= iscrib[i + d]; if (!in) { qo += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; no++; } }
  printf("%.2f %.4f %.4f %d ", bestsc, q / (N - 3), no ? qo / no : 0, bestag);
  for (int i = 0; i < N; i++) putchar('A' + pt[i]);
  printf(" m="); for (int j = 0; j < P; j++) printf("%d,", best.m[j]); printf(" a="); for (int s = 0; s < NS; s++) printf("%d,", best.a[s]);
  printf(" alpha="); for (int v = 0; v < 26; v++) putchar('A' + best.inv[v]); printf("\n");
  return 0;
}
