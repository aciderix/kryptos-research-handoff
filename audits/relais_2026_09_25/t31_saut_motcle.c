/* t31_saut_motcle.c — clé PÉRIODIQUE (p = 2..26) avec UN ou DEUX sauts de phase de ±1 (lettre omise ou clé perdue), alphabets à
 * mot-clé, erreurs comptées (relais du 25/09/2026).
 * Motif : sur la maquette de 1988, Sanborn omet un T (« WITHOUT (T)HE KEY ») et toute la suite du Vigenère RUG glisse d'un rang ;
 * dans K2, l'X omis fait de même. Ici : k_i = K[(i + Σ s·[i ≥ x]) mod p], un saut x ∈ 22..73 (dans ENE, entre les cribs, dans
 * BERLINCLOCK), ou deux sauts (un dans chaque crib, périodes ≤ 13). e_min exact = 24 − Σ_j (plus grande multiplicité).
 * Invariance K → −K + c : alphabet à l'endroit, VIG et BEAU suffisent. Types Q3, Q2, Q1, Q4a, Q4b. Témoins : K4 mélangé.
 * Usage : t31 alphas.bin [NNULL]   (CUT=4 par défaut : seuls les cas à e ≤ CUT sont calculés exactement ; p ≤ 13)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
#define NP 24
static int POS[NP], PT[NP];
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
typedef struct { unsigned char j[NP]; unsigned char p; short x1, x2; signed char s1, s2; uint64_t pm[5]; int ncls; } Cfg;
static int PA[276], PB[276];
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { POS[i] = 21 + i; PT[i] = e[i] - 'A'; }
  for (int i = 0; i < 11; i++) { POS[13 + i] = 63 + i; PT[13 + i] = b[i] - 'A'; }
  int NNULL = argc > 2 ? atoi(argv[2]) : 10;
  /* configurations */
  int cap = 20000, nc = 0; Cfg *cf = malloc(sizeof(Cfg) * cap);
  for (int p = 2; p <= 26; p++) {
    int SMAX = getenv("SMAX") ? atoi(getenv("SMAX")) : 1;
    for (int x = 21; x <= 74; x++) for (int s = -SMAX; s <= SMAX; s++) { if (!s) continue;
      if (x == 21 && s == SMAX) { Cfg c = {{0}, p, 999, 999, 0, 0}; for (int t = 0; t < NP; t++) c.j[t] = POS[t] % p; cf[nc++] = c; } /* sans saut */
      if (x == 21) continue;
      Cfg c = {{0}, p, x, 999, s, 0}; for (int t = 0; t < NP; t++) c.j[t] = (POS[t] + (POS[t] >= x ? s : 0) + p) % p; cf[nc++] = c; }
    if (p <= 13) for (int x = 22; x <= 33; x++) for (int s = -1; s <= 1; s += 2) for (int y = 64; y <= 73; y++) for (int u = -1; u <= 1; u += 2) {
      Cfg c = {{0}, p, x, y, s, u}; for (int t = 0; t < NP; t++) c.j[t] = (POS[t] + (POS[t] >= x ? s : 0) + (POS[t] >= y ? u : 0) + 2 * p) % p; cf[nc++] = c; }
  }
  { int q = 0; for (int t = 0; t < NP; t++) for (int u = t + 1; u < NP; u++) { PA[q] = t; PB[q] = u; q++; } }
  for (int c = 0; c < nc; c++) { memset(cf[c].pm, 0, sizeof cf[c].pm); unsigned char seen[26] = {0}; cf[c].ncls = 0;
    for (int t = 0; t < NP; t++) if (!seen[cf[c].j[t]]) { seen[cf[c].j[t]] = 1; cf[c].ncls++; }
    for (int q = 0; q < 276; q++) if (cf[c].j[PA[q]] == cf[c].j[PB[q]]) cf[c].pm[q >> 6] |= 1ULL << (q & 63); }
  int CUT = getenv("CUT") ? atoi(getenv("CUT")) : 4;
  FILE *fa = fopen(argv[1], "rb"); fseek(fa, 0, SEEK_END); long na = ftell(fa) / 26; fseek(fa, 0, SEEK_SET);
  unsigned char *AL = malloc(na * 26); if (fread(AL, 26, na, fa) != (size_t)na) return 2; fclose(fa);
  int NCT = 1 + NNULL; int (*CTS)[97] = malloc(sizeof(int[97]) * NCT);
  const char *ctx = getenv("CTX"); for (int i = 0; i < 97; i++) CTS[0][i] = (ctx ? ctx[i] : K4[i]) - 'A';
  uint64_t s = 2024;
  for (int z = 1; z < NCT; z++) { memcpy(CTS[z], CTS[0], sizeof CTS[0]);
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int q = CTS[z][i]; CTS[z][i] = CTS[z][j]; CTS[z][j] = q; } }
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  printf("%d configurations, %ld alphabets, %d chiffrés\n", nc, na, NCT);
  int best[64], best2[64]; for (int z = 0; z < NCT; z++) best[z] = best2[z] = 99;   /* best : p ≤ 13 ; best2 : p 14..26 */
  long hist[64][25]; memset(hist, 0, sizeof hist);
#pragma omp parallel
  {
    int lb[64], lb2[64]; long lh[64][25]; for (int z = 0; z < NCT; z++) lb[z] = lb2[z] = 99; memset(lh, 0, sizeof lh);
#pragma omp for schedule(dynamic, 16)
    for (long ai = 0; ai < na; ai++) {
      int S[26]; for (int i = 0; i < 26; i++) S[AL[ai * 26 + i] - 'A'] = i;
      for (int ty = 0; ty < 5; ty++) {
        const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
        const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
        for (int mo = 0; mo < 2; mo++) for (int z = 0; z < NCT; z++) {
          unsigned char K[NP]; for (int t = 0; t < NP; t++) { int x = X[CTS[z][POS[t]]], y = Y[PT[t]]; K[t] = mo == 0 ? md(x - y) : md(x + y); }
          int bz = 99, bz2 = 99;
          uint64_t eq[5] = {0}; for (int q = 0; q < 276; q++) if (K[PA[q]] == K[PB[q]]) eq[q >> 6] |= 1ULL << (q & 63);
          for (int c = 0; c < nc; c++) {
            const Cfg *C = &cf[c]; if (C->p > 13) continue;
            int npair = __builtin_popcountll(eq[0] & C->pm[0]) + __builtin_popcountll(eq[1] & C->pm[1]) + __builtin_popcountll(eq[2] & C->pm[2])
                      + __builtin_popcountll(eq[3] & C->pm[3]) + __builtin_popcountll(eq[4] & C->pm[4]);
            if (C->ncls + npair < NP - CUT) continue;          /* ne peut pas atteindre e ≤ CUT */
            int cnt[26][26]; int sat = 0; unsigned char used[26] = {0}; int mx[26];
            for (int j = 0; j < C->p; j++) mx[j] = 0;
            for (int t = 0; t < NP; t++) { int j = C->j[t]; if (!used[j]) { used[j] = 1; memset(cnt[j], 0, sizeof cnt[j]); } if (++cnt[j][K[t]] > mx[j]) mx[j] = cnt[j][K[t]]; }
            for (int j = 0; j < C->p; j++) sat += mx[j];
            int ee = NP - sat; if (ee > CUT) continue;
            lh[z][ee]++;
            if (ee < bz) bz = ee;
            if (z == 0 && ee <= (getenv("SHOW") ? atoi(getenv("SHOW")) : 2)) {
#pragma omp critical
              { printf("K4 e=%d p=%d saut %+d en %d", ee, C->p, C->s1, C->x1); if (C->x2 < 999) printf(" et %+d en %d", C->s2, C->x2);
                printf(" type=%d %s alphabet=", ty, mo ? "BEAU" : "VIG"); for (int i = 0; i < 26; i++) putchar(AL[ai * 26 + i]); putchar('\n'); }
            }
          }
          if (bz < lb[z]) lb[z] = bz; if (bz2 < lb2[z]) lb2[z] = bz2;
        }
      }
    }
#pragma omp critical
    for (int z = 0; z < NCT; z++) { if (lb[z] < best[z]) best[z] = lb[z]; if (lb2[z] < best2[z]) best2[z] = lb2[z]; for (int k = 0; k < 25; k++) hist[z][k] += lh[z][k]; }
  }
  for (int z = 0; z < NCT; z++) { printf("%s %2d : minimum (p ≤ 13) : %s%d ; histogramme :", z ? "témoin" : "K4    ", z, best[z] > CUT ? "> " : "", best[z] > CUT ? CUT : best[z]); for (int k = 0; k <= 5; k++) printf(" e=%d:%ld", k, hist[z][k]); printf("\n"); }
  return 0;
}
