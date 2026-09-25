/* t32_progressive_motcle.c — clé PROGRESSIVE avec alphabets à mot-clé (relais du 25/09/2026).
 * k_i = K[i mod p] + s·floor((i − φ)/p) : le mot-clé de p lettres avance de s à chaque tour (disque ou tableau décalé d'un cran
 * à chaque passage). Avec un alphabet quelconque, la famille était trop lâche (base 2, « progressive » : compatible pour p = 8,
 * 10–13…). Avec les alphabets de Sanborn, on calcule le nombre minimal d'erreurs (24 − Σ plus grande multiplicité par classe,
 * après retrait de s·tour). p = 2..26, s = 1..25, phase de départ du tour φ = 0..p−1 (le tour commence où Sanborn a commencé
 * le mot-clé ; seul φ mod p compte). Types Q3, Q2, Q1, Q4a, Q4b ; VIG / BEAU (inverses et VARB couverts par K → −K + c,
 * s → −s). Témoins : K4 mélangé. Usage : t32 alphas.bin [NNULL]
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
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { POS[i] = 21 + i; PT[i] = e[i] - 'A'; }
  for (int i = 0; i < 11; i++) { POS[13 + i] = 63 + i; PT[13 + i] = b[i] - 'A'; }
  int NNULL = argc > 2 ? atoi(argv[2]) : 10, SHOW = getenv("SHOW") ? atoi(getenv("SHOW")) : 2, CUT = getenv("CUT") ? atoi(getenv("CUT")) : 5;
  FILE *fa = fopen(argv[1], "rb"); fseek(fa, 0, SEEK_END); long na = ftell(fa) / 26; fseek(fa, 0, SEEK_SET);
  unsigned char *AL = malloc(na * 26); if (fread(AL, 26, na, fa) != (size_t)na) return 2; fclose(fa);
  int NCT = 1 + NNULL; int (*CTS)[97] = malloc(sizeof(int[97]) * NCT);
  const char *ctx = getenv("CTX"); for (int i = 0; i < 97; i++) CTS[0][i] = (ctx ? ctx[i] : K4[i]) - 'A';
  uint64_t sd = 77;
  for (int z = 1; z < NCT; z++) { memcpy(CTS[z], CTS[0], sizeof CTS[0]);
    for (int i = 96; i > 0; i--) { sd ^= sd << 13; sd ^= sd >> 7; sd ^= sd << 17; int j = sd % (i + 1); int q = CTS[z][i]; CTS[z][i] = CTS[z][j]; CTS[z][j] = q; } }
  /* configurations (p, φ) : classe et tour de chaque position de crib */
  typedef struct { unsigned char p, phi, cls[NP], tour[NP]; int np, ncls; unsigned char pa[300], pb[300], pd[300]; } Cfg;
  Cfg *cf = malloc(sizeof(Cfg) * 400); int nc = 0;
  for (int p = 2; p <= 26; p++) for (int phi = 0; phi < p; phi++) { Cfg c; c.p = p; c.phi = phi;
    for (int t = 0; t < NP; t++) { c.cls[t] = POS[t] % p; c.tour[t] = (POS[t] - phi + 4 * p) / p; }
    c.np = 0; unsigned char seen[26] = {0}; c.ncls = 0; for (int t = 0; t < NP; t++) if (!seen[c.cls[t]]) { seen[c.cls[t]] = 1; c.ncls++; }
    for (int t = 0; t < NP; t++) for (int u = t + 1; u < NP; u++) if (c.cls[t] == c.cls[u]) { c.pa[c.np] = t; c.pb[c.np] = u; c.pd[c.np] = md(c.tour[u] - c.tour[t]); c.np++; }
    cf[nc++] = c; }
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  printf("%d configurations (p, φ) × 25 pas, %ld alphabets, %d chiffrés\n", nc, na, NCT);
  int best[64][27]; for (int z = 0; z < NCT; z++) for (int p = 0; p < 27; p++) best[z][p] = 99;
#pragma omp parallel
  {
    int lb[64][27]; for (int z = 0; z < NCT; z++) for (int p = 0; p < 27; p++) lb[z][p] = 99;
#pragma omp for schedule(dynamic, 16)
    for (long ai = 0; ai < na; ai++) {
      int S[26]; for (int i = 0; i < 26; i++) S[AL[ai * 26 + i] - 'A'] = i;
      for (int ty = 0; ty < 5; ty++) {
        const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
        const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
        for (int mo = 0; mo < 2; mo++) for (int z = 0; z < NCT; z++) {
          int K[NP]; for (int t = 0; t < NP; t++) { int x = X[CTS[z][POS[t]]], y = Y[PT[t]]; K[t] = mo == 0 ? md(x - y) : md(x + y); }
          for (int c = 0; c < nc; c++) { const Cfg *C = &cf[c];
            /* paires en accord pour chaque pas s : K_u − K_t ≡ s·(tour_u − tour_t) */
            int agree[26] = {0};
            for (int q = 0; q < C->np; q++) { int dl = md(K[C->pb[q]] - K[C->pa[q]]), d = C->pd[q];
              for (int s = 1; s < 26; s++) if (md(s * d) == dl) agree[s]++; }
            for (int s = 1; s < 26; s++) {
              if (C->ncls + agree[s] < NP - CUT) continue;          /* borne : sat ≤ classes + paires en accord */
              int cnt[26][26]; unsigned char used[26] = {0}; int mx[26] = {0}, sat = 0;
              for (int t = 0; t < NP; t++) { int j = C->cls[t], v = md(K[t] - s * C->tour[t]);
                if (!used[j]) { used[j] = 1; memset(cnt[j], 0, sizeof cnt[j]); } if (++cnt[j][v] > mx[j]) mx[j] = cnt[j][v]; }
              for (int j = 0; j < C->p; j++) sat += mx[j];
              int ee = NP - sat; if (ee < lb[z][C->p]) lb[z][C->p] = ee;
              if (z == 0 && ee <= SHOW && C->p <= 13) {
#pragma omp critical
                { printf("K4 e=%d p=%d φ=%d pas=%d type=%d %s alphabet=", ee, C->p, C->phi, s, ty, mo ? "BEAU" : "VIG");
                  for (int i = 0; i < 26; i++) putchar(AL[ai * 26 + i]); putchar('\n'); }
              }
            }
          }
        }
      }
    }
#pragma omp critical
    for (int z = 0; z < NCT; z++) for (int p = 0; p < 27; p++) if (lb[z][p] < best[z][p]) best[z][p] = lb[z][p];
  }
  printf("erreurs minimales par période (au-delà de CUT = %d : non calculé, affiché 99) (K4 puis témoins) :\n", CUT); printf("   p :"); for (int p = 2; p <= 26; p++) printf(" %2d", p); printf("\n");
  for (int z = 0; z < NCT; z++) { printf("%s", z ? "  t :" : "  K4:"); for (int p = 2; p <= 26; p++) printf(" %2d", best[z][p]); printf("\n"); }
  return 0;
}
