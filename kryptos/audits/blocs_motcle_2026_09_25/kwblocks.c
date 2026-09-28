/* kwblocks.c — T11/T11b rejugés avec les alphabets à mot-clé (audit « blocs mot-clé », 25/09/2026).
 * Clé(i) = a[bloc(i)] + motif(j), bloc de n lettres à la phase φ, j = rang dans le bloc ; motif = s·j (s = 0..25),
 * ou KRYPTOS lu dans A–Z (endroit, envers) pour n = 7. Alphabet à mot-clé fixé ⇒ clé connue aux 24 positions ;
 * e_min exact = 24 − Σ_blocs (effectif de la valeur la plus fréquente de K_t − motif(j_t)).
 * Types Q3/Q2/Q1/Q4a/Q4b, conventions VIG/BEAU/VARB. n = 2..30.
 * Sortie : histogramme global de e_min et cas avec e_min ≤ seuil (défaut 3).
 * Usage : kwblocks mots.txt [graine_mélange] ; ONLYSTD=1 pour ne garder que les formes standard et inverse.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int CT[97], POS[24], PT[24];
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static void mkalpha(const char *w, int cont, int rev, int *pos) {
  char a[27]; int n = 0, used[26] = {0};
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (c < 0 || c > 25 || used[c]) continue; used[c] = 1; a[n++] = 'A' + c; }
  int start = cont && n ? (a[n - 1] - 'A' + 1) % 26 : 0;
  for (int k = 0; k < 26; k++) { int c = (start + k) % 26; if (!used[c]) { used[c] = 1; a[n++] = 'A' + c; } }
  for (int i = 0; i < 26; i++) pos[(rev ? a[25 - i] : a[i]) - 'A'] = i;
}
int main(int argc, char **argv) {
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n0 = 0;
  for (int i = 0; i < 13; i++) { POS[n0] = 21 + i; PT[n0] = e[i] - 'A'; n0++; }
  for (int i = 0; i < 11; i++) { POS[n0] = 63 + i; PT[n0] = b[i] - 'A'; n0++; }
  const char *K4X = getenv("CTX") ? getenv("CTX") : K4; for (int i = 0; i < 97; i++) CT[i] = K4X[i] - 'A';
  uint64_t seed = argc > 2 ? strtoull(argv[2], 0, 10) : 0;
  if (seed) { uint64_t s = seed * 0x9E3779B97F4A7C15ULL; for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CT[i]; CT[i] = CT[j]; CT[j] = t; } }
  int thr = getenv("THR") ? atoi(getenv("THR")) : 3;
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  static const int kz[7] = {10, 17, 24, 15, 19, 14, 18}; /* KRYPTOS en rangs A–Z */
  /* précalcul des cas : (n, φ, motif) -> bloc et rang pour les 24 positions */
  typedef struct { int n, phi, mot; signed char blk[24], j[24]; } Case;
  Case *cs = malloc(sizeof(Case) * 20000); int nc = 0;
  int NMIN = getenv("NMIN") ? atoi(getenv("NMIN")) : 5, NMAX = getenv("NMAX") ? atoi(getenv("NMAX")) : 14;
  for (int n = NMIN; n <= NMAX; n++) for (int phi = 0; phi < n; phi++) for (int mot = 0; mot < 26 + (n == 7 ? 2 : 0); mot++) {
    Case *c = &cs[nc++]; c->n = n; c->phi = phi; c->mot = mot;
    for (int t = 0; t < 24; t++) { int i = POS[t]; c->blk[t] = (i - phi + n) / n; c->j[t] = ((i - phi) % n + n) % n; }
  }
  static const char *TN[5] = {"Q3", "Q2", "Q1", "Q4a", "Q4b"}, *MN[3] = {"VIG", "BEAU", "VARB"};
  long hist[25] = {0}; FILE *wf = fopen(argv[1], "r"); char w[64];
  int ncont = getenv("ONLYSTD") ? 1 : 2;
  while (fscanf(wf, "%63s", w) == 1) for (int cont = 0; cont < ncont; cont++) for (int rev = 0; rev < 2; rev++) {
    int S[26]; mkalpha(w, cont, rev, S);
    for (int ty = 0; ty < 5; ty++) {
      const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
      const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
      for (int mo = 0; mo < 3; mo++) {
        int K[24]; for (int t = 0; t < 24; t++) { int x = X[CT[POS[t]]], y = Y[PT[t]]; K[t] = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x); }
        for (int ci = 0; ci < nc; ci++) {
          const Case *c = &cs[ci];
          int v[24];
          for (int t = 0; t < 24; t++) { int m = c->mot < 26 ? c->mot * c->j[t] : (c->mot == 26 ? kz[c->j[t]] : kz[6 - c->j[t]]); v[t] = md(K[t] - m); }
          /* regroupement par bloc : les blocs sont croissants dans l'ordre des positions */
          int agree = 0, t = 0;
          while (t < 24) {
            int u = t; while (u < 24 && c->blk[u] == c->blk[t]) u++;
            int best = 0; for (int a = t; a < u; a++) { int cnt = 0; for (int b2 = t; b2 < u; b2++) cnt += v[b2] == v[a]; if (cnt > best) best = cnt; }
            agree += best; t = u;
          }
          int em = 24 - agree; hist[em]++;
          if (em <= thr) printf("CAS %s c%d r%d %s %s n=%d phi=%d motif=%d : e_min %d\n", w, cont, rev, TN[ty], MN[mo], c->n, c->phi, c->mot, em);
        }
      }
    }
  }
  printf("histogramme e_min :"); for (int x = 0; x <= 24; x++) if (hist[x]) printf(" %d:%ld", x, hist[x]); printf("\n");
  return 0;
}
