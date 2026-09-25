/* t29_pas7_phases.c — « disque tourné de temps en temps » : clé de 7 lettres + décalage libre par bloc de 7, à TOUTE phase
 * (relais du 25/09/2026). Motif : ce modèle explique d'un coup les coïncidences à l'écart 7 (clé identique sur plusieurs
 * blocs), les doublets à phase fixe (écart m[4] − m[5] constant), le conflit 65/72 (décalage entre les blocs 9 et 10) et
 * l'échec de la période 7 pure. kwsweep.c et k4x.c (famille R) ne l'ont testé qu'avec des blocs commençant en 0.
 * Clé : k_i = m[(i − φ) mod 7] + a[(i − φ) div 7], φ = 0..6. Alphabets : fichier de genalpha (26 octets, à l'endroit ;
 * la famille est invariante par K → −K + c, donc l'alphabet inversé et la variante VARB sont couverts par VIG/BEAU).
 * Types : Q3 σ/σ, Q2 (clair A–Z, chiffré σ), Q1 (clair σ, chiffré A–Z), Q4a (clair σ, chiffré KRYPTOS), Q4b (clair KRYPTOS,
 * chiffré σ). e_min exact (nombre minimal de lettres des cribs à déclarer fausses) par recherche élaguée, seuil CUT.
 * Témoins : NNULL chiffrés « K4 mélangé » dans la même passe. Usage : t29 alphas.bin [NNULL] [CUT]
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
typedef struct { int ph[NP], seg[NP], R; } Lay;
static Lay LAY[7];
static int CUT;
/* recherche exacte : a[0] = 0 ; les candidats pour a[q] sont les valeurs K_t − K_u + a[seg u] (même colonne, u dans un
 * segment déjà affecté) et UNE valeur « libre » qui n'en égale aucune (le segment ne s'accorde alors avec aucun précédent).
 * Toute solution optimale est de cette forme, à un renommage près des composantes : la recherche est exacte. */
static int best_sat;
#pragma omp threadprivate(best_sat)
static void rec(const int *K, const Lay *L, int *a, int q) {
  int sat = 0, rest = 0;
  for (int j = 0; j < 7; j++) {
    int cnt[27] = {0}, mx = 0, r = 0;
    for (int t = 0; t < NP; t++) if (L->ph[t] == j) { if (L->seg[t] < q) { int v = a[L->seg[t]] < 0 ? 26 : md(K[t] - a[L->seg[t]]);
          if (v == 26) { if (mx < 1) mx = 1; } else if (++cnt[v] > mx) mx = cnt[v]; } else r++; }
    sat += mx; rest += r;
  }
  if (sat + rest <= best_sat) return;
  if (q == L->R) { if (sat > best_sat) best_sat = sat; return; }
  int cand[64], nc = 0;
  for (int t = 0; t < NP; t++) if (L->seg[t] == q) for (int u = 0; u < NP; u++) if (L->seg[u] < q && L->ph[u] == L->ph[t] && a[L->seg[u]] >= 0) {
    int v = md(K[t] - K[u] + a[L->seg[u]]), d = 0; for (int k = 0; k < nc; k++) if (cand[k] == v) d = 1; if (!d) cand[nc++] = v; }
  for (int k = 0; k < nc; k++) { a[q] = cand[k]; rec(K, L, a, q + 1); }
  a[q] = -1; rec(K, L, a, q + 1);   /* valeur libre : le segment s'isole (ses entrées ne comptent que seules dans leur colonne) */
}
static int emin(const int *K, const Lay *L) {
  int a[8] = {0}; best_sat = NP - CUT - 1;
  rec(K, L, a, 1);
  return best_sat >= NP - CUT ? NP - best_sat : CUT + 1;
}
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { POS[i] = 21 + i; PT[i] = e[i] - 'A'; }
  for (int i = 0; i < 11; i++) { POS[13 + i] = 63 + i; PT[13 + i] = b[i] - 'A'; }
  int NNULL = argc > 2 ? atoi(argv[2]) : 20; CUT = argc > 3 ? atoi(argv[3]) : 4;
  for (int f = 0; f < 7; f++) { int s0 = 99; Lay *L = &LAY[f];
    for (int t = 0; t < NP; t++) { L->ph[t] = (POS[t] - f) % 7; int s = (POS[t] - f) / 7; if (s < s0) s0 = s; L->seg[t] = s; }
    /* renuméroter les segments 0..R−1, le plus peuplé en 0 */
    int ids[20], n = 0, cnt[20] = {0};
    for (int t = 0; t < NP; t++) { int s = L->seg[t], k; for (k = 0; k < n; k++) if (ids[k] == s) break; if (k == n) ids[n++] = s; cnt[k]++; L->seg[t] = k; }
    int big = 0; for (int k = 1; k < n; k++) if (cnt[k] > cnt[big]) big = k;
    for (int t = 0; t < NP; t++) { if (L->seg[t] == big) L->seg[t] = 0; else if (L->seg[t] == 0) L->seg[t] = big; }
    L->R = n; printf("phase %d : %d blocs de 7 portent des lettres des cribs\n", f, n); }
  FILE *fa = fopen(argv[1], "rb"); fseek(fa, 0, SEEK_END); long na = ftell(fa) / 26; fseek(fa, 0, SEEK_SET);
  unsigned char *AL = malloc(na * 26); if (fread(AL, 26, na, fa) != (size_t)na) return 2; fclose(fa);
  int NCT = 1 + NNULL; int (*CTS)[97] = malloc(sizeof(int[97]) * NCT);
  const char *ctx = getenv("CTX"); for (int i = 0; i < 97; i++) CTS[0][i] = (ctx ? ctx[i] : K4[i]) - 'A';  /* CTX : contrôle positif */
  uint64_t s = 1000;
  for (int z = 1; z < NCT; z++) { memcpy(CTS[z], CTS[0], sizeof CTS[0]);
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int x = CTS[z][i]; CTS[z][i] = CTS[z][j]; CTS[z][j] = x; } }
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  long (*hist)[8] = calloc(NCT, sizeof(long[8]));   /* hist[z][e] pour e ≤ CUT (index CUT+1 = au-delà) */
  printf("%ld alphabets, %d chiffrés, seuil %d, threads %d\n", na, NCT, CUT, omp_get_max_threads());
#pragma omp parallel
  {
    long (*h)[8] = calloc(NCT, sizeof(long[8]));
#pragma omp for schedule(dynamic, 32)
    for (long ai = 0; ai < na; ai++) {
      int S[26]; for (int i = 0; i < 26; i++) S[AL[ai * 26 + i] - 'A'] = i;   /* lettre -> rang */
      for (int ty = 0; ty < 5; ty++) {
        const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;   /* clair */
        const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;   /* chiffré */
        for (int mo = 0; mo < 2; mo++) for (int z = 0; z < NCT; z++) {
          int K[NP]; for (int t = 0; t < NP; t++) { int x = X[CTS[z][POS[t]]], y = Y[PT[t]]; K[t] = mo == 0 ? md(x - y) : md(x + y); }
          for (int f = 0; f < 7; f++) {
            int ee = emin(K, &LAY[f]); h[z][ee > CUT ? CUT + 1 : ee]++;
            if (z == 0 && ee <= 2) {
#pragma omp critical
              { printf("K4 e=%d φ=%d type=%d %s alphabet=", ee, f, ty, mo ? "BEAU" : "VIG"); for (int i = 0; i < 26; i++) putchar(AL[ai * 26 + i]); putchar('\n'); }
            }
          }
        }
      }
    }
#pragma omp critical
    for (int z = 0; z < NCT; z++) for (int k = 0; k < 8; k++) hist[z][k] += h[z][k];
    free(h);
  }
  for (int z = 0; z < NCT; z++) { printf("%s %2d :", z ? "témoin" : "K4    ", z); int mn = -1;
    for (int k = 0; k <= CUT; k++) { printf(" e=%d:%ld", k, hist[z][k]); if (mn < 0 && hist[z][k]) mn = k; } printf("  | minimum %d\n", mn < 0 ? CUT + 1 : mn); }
  return 0;
}
