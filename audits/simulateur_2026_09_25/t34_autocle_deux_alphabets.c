/* t34_autocle_deux_alphabets.c — le procédé que le simulateur désigne, testé à fond (25/09/2026).
 *
 * Le simulateur (sim_sanborn.c) montre que le « 7 » de K4 ne relie que des voisins (écart 7 : 9 coïncidences ; écarts 14 et 21 :
 * au niveau du hasard). Parmi 52 procédés, un seul rend ce profil naturel : l'autoclé sur le CLAIR à l'écart 7, forme
 * VIGENÈRE (clé = lettre claire 7 rangs avant, lue dans l'alphabet du clair) : τ(c_i) = σ(p_i) + σ(p_{i−7}).
 * Beaufort, Variante, et la lettre-clé lue dans un autre alphabet ne produisent PAS l'excès (familles 34, 48, 50).
 * En revanche l'alphabet du chiffré τ est libre (famille 49) : c'est le cas Quagmire IV, que T18 (σ = τ) et T27 (τ ∈ {σ, KRYPTOS})
 * n'ont pas couvert.
 *
 * Équations : dans chaque classe i mod 7, entre deux positions de crib consécutives a < b = a + 7m, la relation se propage à
 * travers le clair inconnu : σp_b = Σ_{t=1..m} (−1)^{m−t} (τc_{a+7t} + d) + (−1)^m σp_a, avec d un décalage libre (rotation de τ).
 * 24 lettres de crib dans 7 classes → 17 équations ; une lettre fausse (erreur de Sanborn) en casse au plus deux.
 * Balayage : σ = alphabets à mot-clé (+ A–Z, KRYPTOS), τ = mêmes alphabets, à l'endroit et à l'envers, d = 0..25.
 * Recherche exacte de toutes les paires à ≤ F équations fausses (tiroirs : F+1 blocs, un bloc au moins est exact).
 * Pour les meilleures paires, le clair ENTIER est déterminé (propagation de 7 en 7 depuis les cribs) : on le lit et on le note
 * aux quadrigrammes anglais.
 * Usage : t34 alphas.bin qg.bin [NNULL] [F]     env : L (écart, défaut 7), CTX (chiffré de contrôle), SHOW (nb de clairs affichés)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#define N 97
#define NE_MAX 24
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int P97[N], LAG = 7;
static float *QG;
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
typedef struct { int ne; int co[NE_MAX][26]; int odd[NE_MAX]; int pa[NE_MAX], pb[NE_MAX], sg[NE_MAX]; } Eqs;

static void build(const int *ct, Eqs *E) {
  E->ne = 0;
  for (int r = 0; r < LAG; r++) {
    int last = -1;
    for (int i = r; i < N; i += LAG) {
      if (P97[i] < 0) continue;
      if (last >= 0) { int e = E->ne++, m = (i - last) / LAG; memset(E->co[e], 0, sizeof E->co[e]);
        for (int t = 1; t <= m; t++) E->co[e][ct[last + t * LAG]] += ((m - t) % 2 == 0) ? 1 : -1;
        E->odd[e] = m & 1; E->pa[e] = P97[last]; E->pb[e] = P97[i]; E->sg[e] = (m % 2 == 0) ? 1 : -1; }
      last = i;
    }
  }
}
/* S_e(σ) = σp_b − (−1)^m σp_a */
static inline int S_e(const Eqs *E, int e, const int *sg) { return md(sg[E->pb[e]] - E->sg[e] * sg[E->pa[e]]); }
static inline int T_e(const Eqs *E, int e, const int *ta) { int s = 0; for (int l = 0; l < 26; l++) if (E->co[e][l]) s += E->co[e][l] * ta[l]; return md(s); }

/* clair entier à partir de σ, τ, d (propagation depuis la première position connue de chaque classe, ré-ancrage aux cribs) */
static double decrypt(const int *ct, const int *sg, const int *ta, int d, char *out) {
  int sinv[26]; for (int l = 0; l < 26; l++) sinv[sg[l]] = l;
  int sp[N];
  for (int r = 0; r < LAG; r++) {
    int a0 = -1; for (int i = r; i < N; i += LAG) if (P97[i] >= 0) { a0 = i; break; }
    if (a0 < 0) { for (int i = r; i < N; i += LAG) sp[i] = -1; continue; }
    sp[a0] = sg[P97[a0]];
    for (int i = a0 + LAG; i < N; i += LAG) sp[i] = P97[i] >= 0 ? sg[P97[i]] : md(ta[ct[i]] + d - sp[i - LAG]);
    for (int i = a0 - LAG; i >= 0; i -= LAG) sp[i] = md(ta[ct[i + LAG]] + d - sp[i + LAG]);
  }
  double s = 0; int nq = 0;
  for (int i = 0; i < N; i++) out[i] = sp[i] < 0 ? '?' : 'A' + sinv[sp[i]];
  out[N] = 0;
  for (int i = 0; i + 3 < N; i++) { if (out[i] == '?' || out[i + 1] == '?' || out[i + 2] == '?' || out[i + 3] == '?') continue;
    s += QG[((out[i] - 'A') * 26 + out[i + 1] - 'A') * 676 + (out[i + 2] - 'A') * 26 + out[i + 3] - 'A']; nq++; }
  return nq ? s / nq : -99;
}

typedef struct { int si, ti, rev, d, bad; double q; } Hit;

/* toutes les paires à ≤ F équations fausses ; renvoie le minimum, remplit hits (au plus cap) */
static int sweep(const int *ct, const unsigned char *AL, long na, int F, Hit *hits, int cap, int *nh, long *hist) {
  Eqs E; build(ct, &E); int NE = E.ne;
  int nb = F + 1, bs[8], bo[8]; { int q = NE / nb, rr = NE % nb, o = 0; for (int b = 0; b < nb; b++) { bs[b] = q + (b < rr); bo[b] = o; o += bs[b]; } }
  /* côté σ : vecteurs S */
  unsigned char *SV = malloc(na * NE);
  for (long a = 0; a < na; a++) { int sg[26]; for (int i = 0; i < 26; i++) sg[AL[a * 26 + i] - 'A'] = i; for (int e = 0; e < NE; e++) SV[a * NE + e] = S_e(&E, e, sg); }
  /* index par bloc : tri par valeur de bloc (clé base 26, blocs ≤ 5) */
  long *start[8]; int *ord[8];
  for (int b = 0; b < nb; b++) {
    long K = 1; for (int j = 0; j < bs[b]; j++) K *= 26;
    start[b] = calloc(K + 1, sizeof(long)); ord[b] = malloc(na * sizeof(int));
    for (long a = 0; a < na; a++) { long k = 0; for (int j = 0; j < bs[b]; j++) k = k * 26 + SV[a * NE + bo[b] + j]; start[b][k + 1]++; }
    for (long k = 0; k < K; k++) start[b][k + 1] += start[b][k];
    long *pos = malloc((K + 1) * sizeof(long)); memcpy(pos, start[b], (K + 1) * sizeof(long));
    for (long a = 0; a < na; a++) { long k = 0; for (int j = 0; j < bs[b]; j++) k = k * 26 + SV[a * NE + bo[b] + j]; ord[b][pos[k]++] = (int)a; }
    free(pos);
  }
  int best = 99; *nh = 0; memset(hist, 0, sizeof(long) * 25);
#pragma omp parallel
  {
    int lbest = 99; long lh[25] = {0};
#pragma omp for schedule(dynamic, 64)
    for (long t = 0; t < 2 * na; t++) {
      long ti = t >> 1; int rev = t & 1; int ta[26];
      for (int i = 0; i < 26; i++) { int v = AL[ti * 26 + i] - 'A'; ta[v] = rev ? md(-i) : i; }
      int T0[NE_MAX]; for (int e = 0; e < NE; e++) T0[e] = T_e(&E, e, ta);
      for (int d = 0; d < 26; d++) {
        int TV[NE_MAX]; for (int e = 0; e < NE; e++) TV[e] = md(T0[e] + d * E.odd[e]);
        for (int b = 0; b < nb; b++) {
          long k = 0; for (int j = 0; j < bs[b]; j++) k = k * 26 + TV[bo[b] + j];
          for (long q = start[b][k]; q < start[b][k + 1]; q++) {
            int a = ord[b][q];
            /* ne compter la paire qu'une fois : au premier bloc exact */
            int first = 1; for (int b2 = 0; b2 < b && first; b2++) { int ok = 1; for (int j = 0; j < bs[b2]; j++) if (SV[(long)a * NE + bo[b2] + j] != TV[bo[b2] + j]) { ok = 0; break; } if (ok) first = 0; }
            if (!first) continue;
            int bad = 0; for (int e = 0; e < NE; e++) bad += SV[(long)a * NE + e] != TV[e];
            if (bad > F) continue;
            lh[bad]++; if (bad < lbest) lbest = bad;
#pragma omp critical
            { if (*nh < cap) { hits[*nh] = (Hit){a, (int)ti, rev, d, bad, 0}; (*nh)++; } }
          }
        }
      }
    }
#pragma omp critical
    { if (lbest < best) best = lbest; for (int k = 0; k < 25; k++) hist[k] += lh[k]; }
  }
  for (int b = 0; b < nb; b++) { free(start[b]); free(ord[b]); }
  free(SV);
  return best;
}

static int cmpq(const void *x, const void *y) { const Hit *a = x, *b = y; if (a->bad != b->bad) return a->bad - b->bad; return a->q < b->q ? 1 : a->q > b->q ? -1 : 0; }

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  if (argc < 3) { fprintf(stderr, "usage: %s alphas.bin qg.bin [NNULL] [F]\n", argv[0]); return 1; }
  if (getenv("L")) LAG = atoi(getenv("L"));
  int NNULL = argc > 3 ? atoi(argv[3]) : 10, F = argc > 4 ? atoi(argv[4]) : 3, SHOW = getenv("SHOW") ? atoi(getenv("SHOW")) : 15;
  FILE *fa = fopen(argv[1], "rb"); fseek(fa, 0, SEEK_END); long n0 = ftell(fa) / 26; fseek(fa, 0, SEEK_SET);
  unsigned char *AL = malloc((n0 + 2) * 26); if (fread(AL, 26, n0, fa) != (size_t)n0) return 2; fclose(fa);
  memcpy(AL + n0 * 26, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26); memcpy(AL + (n0 + 1) * 26, "KRYPTOSABCDEFGHIJLMNQUVWXZ", 26); long na = n0 + 2;
  QG = malloc(456976 * sizeof(float)); FILE *fq = fopen(argv[2], "rb"); if (fread(QG, sizeof(float), 456976, fq) != 456976) return 3; fclose(fq);
  for (int i = 0; i < N; i++) P97[i] = -1;
  { const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; for (int i = 0; i < 13; i++) P97[21 + i] = e[i] - 'A'; for (int i = 0; i < 11; i++) P97[63 + i] = b[i] - 'A'; }
  int ct[N]; const char *ctx = getenv("CTX"); for (int i = 0; i < N; i++) ct[i] = (ctx ? ctx[i] : K4[i]) - 'A';
  Eqs E; build(ct, &E);
  printf("écart L=%d : %d équations ; %ld alphabets (σ) × %ld (τ, deux sens) × 26 décalages ; recherche exacte à ≤ %d équations fausses\n", LAG, E.ne, na, 2 * na, F);
  int cap = 2000000; Hit *hits = malloc(sizeof(Hit) * cap); int nh; long hist[25];
  double t0 = omp_get_wtime();
  int best = sweep(ct, AL, na, F, hits, cap, &nh, hist);
  printf("%s : minimum %d équation(s) fausse(s) ; paires : ", ctx ? "CTX" : "K4", best); for (int k = 0; k <= F; k++) printf(" %d→%ld", k, hist[k]); printf("   (%.1f s)\n", omp_get_wtime() - t0);
  /* clairs des meilleures paires */
  char out[N + 1];
  for (int h = 0; h < nh; h++) { int sg[26], ta[26]; for (int i = 0; i < 26; i++) sg[AL[(long)hits[h].si * 26 + i] - 'A'] = i;
    for (int i = 0; i < 26; i++) { int v = AL[(long)hits[h].ti * 26 + i] - 'A'; ta[v] = hits[h].rev ? md(-i) : i; }
    hits[h].q = decrypt(ct, sg, ta, hits[h].d, out); }
  qsort(hits, nh, sizeof(Hit), cmpq);
  double bq = -99; for (int h = 0; h < nh; h++) if (hits[h].q > bq) bq = hits[h].q;
  printf("meilleur score quadrigrammes parmi ces paires : %.3f (anglais ≈ −4,2 ; hasard ≈ −5,5 à −6)\n", bq);
  for (int h = 0, shown = 0; h < nh && shown < SHOW; h++) {
    int sg[26], ta[26]; for (int i = 0; i < 26; i++) sg[AL[(long)hits[h].si * 26 + i] - 'A'] = i;
    for (int i = 0; i < 26; i++) { int v = AL[(long)hits[h].ti * 26 + i] - 'A'; ta[v] = hits[h].rev ? md(-i) : i; }
    decrypt(ct, sg, ta, hits[h].d, out); shown++;
    printf("  faux=%d q=%.3f σ=%.26s τ=%.26s%s d=%2d  %s\n", hits[h].bad, hits[h].q, AL + (long)hits[h].si * 26, AL + (long)hits[h].ti * 26, hits[h].rev ? "(inv)" : "", hits[h].d, out);
  }
  /* par score : les 10 meilleurs clairs, toutes erreurs confondues */
  { Hit *hq = malloc(sizeof(Hit) * (nh ? nh : 1)); memcpy(hq, hits, sizeof(Hit) * nh);
    for (int i = 0; i < nh; i++) for (int j = i + 1; j < nh && j < i + 1; j++) ;
    /* tri par score seul */
    for (int i = 1; i < nh; i++) { Hit x = hq[i]; int j = i - 1; while (j >= 0 && hq[j].q < x.q) { hq[j + 1] = hq[j]; j--; } hq[j + 1] = x; if (i > 200000) break; }
    printf("meilleurs scores :\n");
    for (int h = 0; h < nh && h < 5; h++) { int sg[26], ta[26]; for (int i = 0; i < 26; i++) sg[AL[(long)hq[h].si * 26 + i] - 'A'] = i;
      for (int i = 0; i < 26; i++) { int v = AL[(long)hq[h].ti * 26 + i] - 'A'; ta[v] = hq[h].rev ? md(-i) : i; }
      decrypt(ct, sg, ta, hq[h].d, out);
      printf("  faux=%d q=%.3f σ=%.26s τ=%.26s%s d=%2d  %s\n", hq[h].bad, hq[h].q, AL + (long)hq[h].si * 26, AL + (long)hq[h].ti * 26, hq[h].rev ? "(inv)" : "", hq[h].d, out); }
    free(hq); }
  /* témoins : K4 mélangé */
  if (NNULL > 0) {
    printf("témoins (K4 mélangé) : minimum d'équations fausses ; paires à 0/1/2/3 ; meilleur score\n");
    uint64_t s = 97;
    for (int z = 0; z < NNULL; z++) {
      int c2[N]; memcpy(c2, ct, sizeof c2);
      for (int i = N - 1; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int q = c2[i]; c2[i] = c2[j]; c2[j] = q; }
      int b2 = sweep(c2, AL, na, F, hits, cap, &nh, hist);
      double q2 = -99; for (int h = 0; h < nh; h++) { int sg[26], ta[26]; for (int i = 0; i < 26; i++) sg[AL[(long)hits[h].si * 26 + i] - 'A'] = i;
        for (int i = 0; i < 26; i++) { int v = AL[(long)hits[h].ti * 26 + i] - 'A'; ta[v] = hits[h].rev ? md(-i) : i; }
        double q = decrypt(c2, sg, ta, hits[h].d, out); if (q > q2) q2 = q; }
      printf("  témoin %2d : minimum %d ;", z, b2); for (int k = 0; k <= F; k++) printf(" %ld", hist[k]); printf(" ; meilleur score %.3f\n", q2);
    }
  }
  return 0;
}
