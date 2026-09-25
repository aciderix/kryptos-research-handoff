/* k4sa.c — T5 : attaque sur le texte entier à partir des solutions EXACTES des cribs (Quagmire III, alphabet σ quelconque).
 * Modèle : σ(c_i) − σ(p_i) = k_i (VIG ; VARB est le même avec σ inversé), k_i = m[i mod 7] + a[segment(i)] :
 *   P : période 7 pure ; L : + décalage par ligne du cuivre ; R : + décalage par ligne de 7.
 * 1. Graphe des lettres : chaque lettre des cribs relie σ(c) et σ(p). Un arbre couvrant exprime chaque lettre d'une
 *    composante comme σ(racine) + combinaison linéaire de la clé ; chaque arête hors de l'arbre donne une équation
 *    linéaire mod 26 sur la clé. On énumère EXACTEMENT toutes les clés qui satisfont ces équations et qui n'envoient pas
 *    deux lettres d'une même composante au même rang (σ injective).
 * 2. Pour chaque clé exacte, les 24 lettres des cribs sont justes par construction ; il reste à placer les composantes
 *    (une base chacune, la première fixée à 0 : σ n'est défini qu'à une rotation près) et les lettres libres.
 *    Recuit (déplacement d'une composante, échange de lettres libres) noté aux quadrigrammes du clair entier.
 * Usage : k4sa FAM(P|L|R) [iters par clé] [redémarrages] [graine]   (CTX = chiffré ; défaut K4)
 * Sortie : nombre de clés exactes, meilleur score (log10 moyen / quadrigramme) et clair ; meilleure clé.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <omp.h>
#define NP 24
#define N 97
#define NV 10 /* m0..m6, a1..a3 (a0 = 0) */
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int POS[NP], PT[NP], CT[N], SEG[N], NSEG; static float *QG; static char FAM;
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static int segof(int i) { return FAM == 'P' ? 0 : FAM == 'L' ? (i + 27) / 31 : (i / 7 == 3 ? 0 : i / 7 == 4 ? 1 : i / 7 == 9 ? 2 : i / 7 == 10 ? 3 : -1); }
/* variables de clé d'une position i : m[i%7] et a[seg] (seg 0 → rien) */
static void kvars(int i, int *c) { memset(c, 0, sizeof(int) * NV); c[i % 7]++; int s = SEG[i]; if (s > 0) c[6 + s]++; }
static int comp[26], root[26], coef[26][NV], incrib[26], NC; static int EQ[64][NV], NEQ;
static int keyval(const int *k, int i) { return md(k[i % 7] + (SEG[i] > 0 ? k[6 + SEG[i]] : 0)); }
static double score(const int *P) { double s = 0; for (int i = 0; i + 3 < N; i++) s += QG[((P[i] * 26 + P[i + 1]) * 26 + P[i + 2]) * 26 + P[i + 3]]; return s; }
static uint64_t rs;
#pragma omp threadprivate(rs)
static inline uint32_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return (uint32_t)rs; }
/* décodage : σ connu (pos[letter]) → clair */
static void decode(const int *pos, const int *k, int *P) { int inv[26]; for (int x = 0; x < 26; x++) inv[pos[x]] = x;
  for (int i = 0; i < N; i++) P[i] = inv[md(pos[CT[i]] - keyval(k, i))]; }
/* recuit pour une clé : renvoie le meilleur score et remplit bestpos */
static double anneal(const int *k, int iters, int *bestpos) {
  int rel[26]; for (int x = 0; x < 26; x++) if (incrib[x]) { int s = 0; for (int v = 0; v < NV; v++) s += coef[x][v] * k[v]; rel[x] = md(s); }
  /* composantes : NC, lettres libres */
  int base[26], pos[26], occ[26], P[N]; int nfree = 0, fr[26]; for (int x = 0; x < 26; x++) if (!incrib[x]) fr[nfree++] = x;
  double best = -1e30;
  for (int restart = 0; restart < 4; restart++) {
    /* placement initial aléatoire sans chevauchement */
    int ok = 0;
    for (int tries = 0; tries < 200 && !ok; tries++) {
      for (int i = 0; i < 26; i++) occ[i] = -1; ok = 1;
      for (int c = 0; c < NC && ok; c++) { base[c] = c == 0 ? 0 : rnd() % 26; for (int x = 0; x < 26; x++) if (incrib[x] && comp[x] == c) { int p = md(base[c] + rel[x]); if (occ[p] >= 0) { ok = 0; break; } occ[p] = x; pos[x] = p; } }
    }
    if (!ok) continue;
    { int q = 0; for (int p = 0; p < 26; p++) if (occ[p] < 0) { int x = fr[q++]; occ[p] = x; pos[x] = p; } }
    decode(pos, k, P); double cur = score(P);
    double T0 = 2.0, T1 = 0.05;
    for (int it = 0; it < iters; it++) {
      double T = T0 * pow(T1 / T0, (double)it / iters);
      int npos[26]; memcpy(npos, pos, sizeof pos);
      int mv = rnd() % 3;
      if (mv == 0 && nfree >= 2) { int a = fr[rnd() % nfree], b = fr[rnd() % nfree]; if (a == b) continue; int t = npos[a]; npos[a] = npos[b]; npos[b] = t; }
      else if (NC > 1) { int c = 1 + rnd() % (NC - 1), d = 1 + rnd() % 25; /* déplacer la composante c de d ; les lettres libres délogées prennent les places libérées */
        int nocc[26]; for (int p = 0; p < 26; p++) nocc[p] = -1;
        for (int x = 0; x < 26; x++) if (incrib[x] && comp[x] != c) nocc[npos[x]] = x;
        int bad = 0; for (int x = 0; x < 26; x++) if (incrib[x] && comp[x] == c) { int p = md(npos[x] + d); if (nocc[p] >= 0) { bad = 1; break; } nocc[p] = x; npos[x] = p; }
        if (bad) continue;
        /* lettres libres : garder leur place si possible, sinon remplir les trous */
        int moved[26], nm = 0; for (int q = 0; q < nfree; q++) { int x = fr[q]; if (nocc[npos[x]] < 0) nocc[npos[x]] = x; else moved[nm++] = x; }
        for (int p = 0, q = 0; p < 26 && q < nm; p++) if (nocc[p] < 0) { nocc[p] = moved[q]; npos[moved[q]] = p; q++; }
      } else continue;
      decode(npos, k, P); double sc = score(P);
      if (sc >= cur || exp((sc - cur) / T) * 4294967296.0 > rnd()) { cur = sc; memcpy(pos, npos, sizeof pos); if (cur > best) { best = cur; memcpy(bestpos, pos, sizeof pos); } }
    }
  }
  return best;
}
int main(int argc, char **argv) {
  FAM = argv[1][0]; int iters = argc > 2 ? atoi(argv[2]) : 3000; int maxsol = 2000000; uint64_t seed = argc > 4 ? strtoull(argv[4], 0, 10) : 1;
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = b[i] - 'A'; n++; }
  const char *C = getenv("CTX") ? getenv("CTX") : K4; for (int i = 0; i < N; i++) CT[i] = C[i] - 'A';
  { FILE *f = fopen(getenv("QG") ? getenv("QG") : "qg.bin", "rb"); QG = malloc(456976 * 4); if (!f || fread(QG, 4, 456976, f) != 456976) return 2; fclose(f); }
  /* segments renumérotés : le segment de la première lettre de crib vaut 0 */
  int s0 = segof(POS[0]); for (int i = 0; i < N; i++) { int s = segof(i); SEG[i] = s - s0; if (SEG[i] < 0) SEG[i] = 0; } /* L : les 4 lettres de la ligne 25 prennent le décalage de la ligne 26 */
  /* SEG hors cribs pour R : on garde la ligne de 7 comme segment (clé des lignes sans crib inconnue → segment 0 par défaut) */
  /* graphe */
  for (int x = 0; x < 26; x++) { comp[x] = -1; incrib[x] = 0; }
  for (int t = 0; t < NP; t++) { incrib[CT[POS[t]]] = 1; incrib[PT[t]] = 1; }
  NC = 0; NEQ = 0; int used[NP] = {0};
  for (int x = 0; x < 26; x++) if (incrib[x] && comp[x] < 0) {
    int q[26], h = 0, tl = 0; q[tl++] = x; comp[x] = NC; root[x] = x; memset(coef[x], 0, sizeof coef[x]);
    while (h < tl) { int u = q[h++];
      for (int t = 0; t < NP; t++) if (!used[t]) { int c = CT[POS[t]], p = PT[t], kc[NV]; kvars(POS[t], kc);
        /* σ(c) = σ(p) + k */
        if (c == u || p == u) { int o = c == u ? p : c;
          if (comp[o] < 0) { used[t] = 1; comp[o] = NC; for (int v = 0; v < NV; v++) coef[o][v] = o == c ? coef[u][v] + kc[v] : coef[u][v] - kc[v]; q[tl++] = o; } } } }
    NC++; }
  for (int t = 0; t < NP; t++) if (!used[t]) { int c = CT[POS[t]], p = PT[t], kc[NV]; kvars(POS[t], kc);
    for (int v = 0; v < NV; v++) EQ[NEQ][v] = md(coef[c][v] - coef[p][v] - kc[v]); NEQ++; }
  int nl = 0; for (int x = 0; x < 26; x++) nl += incrib[x];
  fprintf(stderr, "famille %c : %d lettres dans les cribs, %d composantes, %d équations\n", FAM, nl, NC, NEQ);
  /* variables utiles */
  int use[NV] = {0}; for (int t = 0; t < NP; t++) { int kc[NV]; kvars(POS[t], kc); for (int v = 0; v < NV; v++) if (kc[v]) use[v] = 1; }
  /* dernière variable (dans l'ordre) de chaque équation : on vérifie l'équation quand elle est affectée */
  int ord[NV], no = 0; for (int v = 0; v < NV; v++) if (use[v]) ord[no++] = v;
  int lastv[64]; for (int q = 0; q < NEQ; q++) { lastv[q] = -1; for (int i = 0; i < no; i++) if (EQ[q][ord[i]]) lastv[q] = i; }
  /* énumération exacte par retour arrière */
  int (*SOLS)[NV] = malloc(sizeof(int) * NV * maxsol); long nsol = 0, nrej = 0;
  int k[NV] = {0}, lvl = 0; k[ord[0]] = -1;
  while (lvl >= 0) {
    if (++k[ord[lvl]] >= 26) { k[ord[lvl]] = -1; lvl--; continue; }
    int ok = 1; for (int q = 0; q < NEQ && ok; q++) if (lastv[q] == lvl) { int s = 0; for (int v = 0; v < NV; v++) s += EQ[q][v] * (use[v] ? k[v] : 0); if (md(s)) ok = 0; }
    if (!ok) continue;
    if (lvl == no - 1) { /* injectivité dans chaque composante */
      int seen[64][26]; memset(seen, 0, sizeof(int) * 26 * NC); int inj = 1;
      for (int x = 0; x < 26 && inj; x++) if (incrib[x]) { int s = 0; for (int v = 0; v < NV; v++) s += coef[x][v] * (use[v] ? k[v] : 0); s = md(s); if (seen[comp[x]][s]++) inj = 0; }
      if (!inj) { nrej++; continue; }
      if (nsol < maxsol) { for (int v = 0; v < NV; v++) SOLS[nsol][v] = use[v] ? k[v] : 0; } nsol++;
      continue; }
    lvl++; k[ord[lvl]] = -1;
  }
  fprintf(stderr, "clés exactes : %ld (rejetées pour collision : %ld)\n", nsol, nrej);
  if (nsol > maxsol) nsol = maxsol;
  double gbest = -1e30; int gpos[26], gk[NV]; long gi = -1;
  double t0 = omp_get_wtime();
  #pragma omp parallel
  { rs = (seed * 0x9E3779B97F4A7C15ULL) ^ (omp_get_thread_num() + 1) * 0xD1B54A32D192ED03ULL; if (!rs) rs = 1;
    #pragma omp for schedule(dynamic, 8)
    for (long si = 0; si < nsol; si++) { int pos[26]; double s = anneal(SOLS[si], iters, pos);
      #pragma omp critical
      if (s > gbest) { gbest = s; memcpy(gpos, pos, sizeof pos); memcpy(gk, SOLS[si], sizeof gk); gi = si; } } }
  int P[N]; if (gi >= 0) decode(gpos, gk, P);
  printf("famille %c : %ld clés exactes ; meilleur score %.3f par quadrigramme ; %.1f s\n", FAM, nsol, gbest / (N - 3), omp_get_wtime() - t0);
  if (gi >= 0) { printf("clair : "); for (int i = 0; i < N; i++) putchar('A' + P[i]); printf("\nσ : "); char a[27]; for (int x = 0; x < 26; x++) a[gpos[x]] = 'A' + x; a[26] = 0; printf("%s  clé :", a);
    for (int v = 0; v < NV; v++) printf(" %d", gk[v]); printf("\n"); }
  return 0;
}
