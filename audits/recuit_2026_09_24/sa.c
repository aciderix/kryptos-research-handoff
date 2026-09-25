/* sa.c — attaque par recuit simulé d'un Quagmire à clé structurée, sur les 97 lettres (audit « recuit », 24/09/2026).
 *
 * Modèle : clé(i) = m[i mod p] + a[seg(i)], seg(i) = (i + o) / W (W = 0 : un seul segment).
 * Types : Q3 (même σ des deux côtés), Q4 (σ1 clair, σ2 chiffré), Q2 (clair A–Z, chiffré σ), Q1 (clair σ, chiffré A–Z).
 * Conventions : VIG σ2(c) = σ1(p) + k ; BEAU σ2(c) = k − σ1(p) ; VARB σ2(c) = σ1(p) − k.
 * Score : somme des log10 des quadrigrammes du clair (97 lettres) + wc × (lettres des cribs retrouvées).
 * Les cribs sont souples : une lettre fausse (erreur de Sanborn) coûte wc au lieu d'être interdite.
 * Usage : sa qg.bin CT Q mode p W o iters restarts seed [wc]
 * Sortie : meilleur score, score quadrigrammes par lettre (hors cribs), cribs retrouvés, clair, clé, alphabets.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
static float *QG;
static int N, CTv[200], cribP[200], iscrib[200];
static int Q, MODE, P, W, O, NS;
static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static inline double urand(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
typedef struct { int s1[26], i1[26], s2[26], m[64], a[64]; } St;
static int seg(int i) { return W ? (i + O) / W : 0; }
static void decrypt(const St *S, int *pt) {
  for (int i = 0; i < N; i++) {
    int v = S->s2[CTv[i]], k = S->m[i % P] + S->a[seg(i)], x;
    x = MODE == 0 ? v - k : MODE == 1 ? k - v : v + k;
    pt[i] = S->i1[md(x)];
  }
}
static double wc = 4.0;
static double score(const St *S, double *qs, int *nm) {
  int pt[200]; decrypt(S, pt); double s = 0; int m = 0;
  for (int i = 0; i + 3 < N; i++) s += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]];
  for (int i = 0; i < N; i++) if (iscrib[i] && pt[i] == cribP[i]) m++;
  if (qs) *qs = s; if (nm) *nm = m;
  return s + wc * m;
}
static void randperm(int *s, int *inv) { for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rnd() % (i + 1); int t = s[i]; s[i] = s[j]; s[j] = t; } for (int i = 0; i < 26; i++) inv[s[i]] = i; }
static void ident(int *s, int *inv) { for (int i = 0; i < 26; i++) s[i] = inv[i] = i; }
static void sync(St *S) { if (Q == 0) { memcpy(S->s2, S->s1, sizeof S->s1); } }
int main(int argc, char **argv) {
  if (argc < 11) { fprintf(stderr, "usage\n"); return 1; }
  FILE *f = fopen(argv[1], "rb"); QG = malloc(sizeof(float) * 456976); if (fread(QG, sizeof(float), 456976, f) != 456976) return 2; fclose(f);
  const char *ct = argv[2]; N = strlen(ct); for (int i = 0; i < N; i++) CTv[i] = ct[i] - 'A';
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { iscrib[21 + i] = 1; cribP[21 + i] = e[i] - 'A'; }
  for (int i = 0; i < 11; i++) { iscrib[63 + i] = 1; cribP[63 + i] = b[i] - 'A'; }
  Q = !strcmp(argv[3], "Q3") ? 0 : !strcmp(argv[3], "Q4") ? 1 : !strcmp(argv[3], "Q2") ? 2 : 3;
  MODE = !strcmp(argv[4], "VIG") ? 0 : !strcmp(argv[4], "BEAU") ? 1 : 2;
  P = atoi(argv[5]); W = atoi(argv[6]); O = atoi(argv[7]);
  long iters = atol(argv[8]); int R = atoi(argv[9]); rs = 0x9E3779B97F4A7C15ULL * (atol(argv[10]) + 1);
  if (argc > 11) wc = atof(argv[11]);
  NS = W ? (N - 1 + O) / W + 1 : 1;
  St best; double bestsc = -1e18;
  for (int r = 0; r < R; r++) {
    St S; 
    int dummy[26];
    if (Q == 2) ident(S.s1, S.i1); else randperm(S.s1, S.i1);
    if (Q == 1 || Q == 2) randperm(S.s2, dummy);
    if (Q == 3) ident(S.s2, dummy);
    sync(&S);
    for (int j = 0; j < P; j++) S.m[j] = rnd() % 26; for (int s = 0; s < NS; s++) S.a[s] = s ? rnd() % 26 : 0;
    double cur = score(&S, NULL, NULL), T0 = 3.0, T1 = 0.05;
    St bs = S; double bsc = cur;
    for (long it = 0; it < iters; it++) {
      double T = T0 * pow(T1 / T0, (double)it / iters);
      int nkey = P + NS - 1;
      if ((int)(rnd() % 100) < 25 && nkey > 0) {
        /* élément de clé : on essaie les 26 valeurs et on tire selon Boltzmann */
        int j = rnd() % nkey; int *slot = j < P ? &S.m[j] : &S.a[1 + j - P];
        double sv[26], mx = -1e18;
        for (int v = 0; v < 26; v++) { *slot = v; sv[v] = score(&S, NULL, NULL); if (sv[v] > mx) mx = sv[v]; }
        double z = 0; for (int v = 0; v < 26; v++) { sv[v] = exp((sv[v] - mx) / T); z += sv[v]; }
        double u = urand() * z; int v = 0; while (v < 25 && (u -= sv[v]) > 0) v++;
        *slot = v; cur = score(&S, NULL, NULL); if (cur > bsc) { bsc = cur; bs = S; }
        continue;
      }
      St X = S;
      int side = Q == 1 ? (int)(rnd() % 2) : (Q == 2 ? 1 : 0);   /* 0 : σ1 ; 1 : σ2 */
      int u = rnd() % 26, v = rnd() % 26; if (u == v) continue;
      if (side == 0) { int t = X.s1[u]; X.s1[u] = X.s1[v]; X.s1[v] = t; X.i1[X.s1[u]] = u; X.i1[X.s1[v]] = v; sync(&X); }
      else { int t = X.s2[u]; X.s2[u] = X.s2[v]; X.s2[v] = t; }
      double sc = score(&X, NULL, NULL);
      if (sc >= cur || urand() < exp((sc - cur) / T)) { S = X; cur = sc; if (cur > bsc) { bsc = cur; bs = S; } }
    }
    if (bsc > bestsc) { bestsc = bsc; best = bs; }
  }
  double qs; int nm; score(&best, &qs, &nm); int pt[200]; decrypt(&best, pt);
  /* score quadrigrammes hors cribs : fenêtres entièrement hors des cribs */
  double qo = 0; int no = 0;
  for (int i = 0; i + 3 < N; i++) { int in = 0; for (int d = 0; d < 4; d++) in |= iscrib[i + d]; if (!in) { qo += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; no++; } }
  printf("%.2f %.4f %.4f %d ", bestsc, qs / (N - 3), no ? qo / no : 0, nm);
  for (int i = 0; i < N; i++) putchar('A' + pt[i]);
  printf(" key=");
  for (int j = 0; j < P; j++) printf("%d,", best.m[j]); printf("|"); for (int s = 0; s < NS; s++) printf("%d,", best.a[s]);
  printf(" s1="); for (int i = 0; i < 26; i++) putchar('A' + best.i1[i]);
  if (Q == 1 || Q == 2) { int inv2[26]; for (int i = 0; i < 26; i++) inv2[best.s2[i]] = i; printf(" s2="); for (int i = 0; i < 26; i++) putchar('A' + inv2[i]); }
  printf("\n");
  return 0;
}
