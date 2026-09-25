/* kwsweep.c — alphabets à mot-clé × clés à structure de pas 7, avec erreurs (audit « mot-clé pas 7 », 24/09/2026).
 *
 * Pour chaque alphabet à mot-clé (liste de mots ; modes : standard = mot puis reste A–Z ; suite = mot puis reste à partir
 * de la lettre qui suit la dernière du mot ; et leurs inverses), chaque type (Q3 σ/σ, Q2 A–Z/σ, Q1 σ/A–Z,
 * Q4a σ/KRYPTOS, Q4b KRYPTOS/σ) et chaque convention (VIG, BEAU, VARB), les cribs donnent la clé K_t en 24 points.
 * Familles (clé(i) = m[i mod 7] + a[segment(i)]) :
 *   L  segment = ligne du cuivre (lignes de 31, K4 en colonne 27) : lignes 26, 27, 28 portent les cribs ;
 *   R  segment = ligne de 7 (phase 0) : lignes 3, 4, 9, 10 portent les cribs ;
 *   P  un seul segment (période 7 pure, témoin de méthode).
 * On calcule le plus grand nombre de lettres des cribs compatibles (24 − e_min), exactement pour L et P, et exactement
 * pour R tant que e_min ≤ 3 (voir la note dans fit_R).
 * Sortie : les meilleurs cas, et l'histogramme de e_min ; option « null » : même balayage sur un chiffré aléatoire.
 * Usage : kwsweep mots.txt [graine_null]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int CT[97], POS[24], PT[24];
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static int LINE[97], ROW[97];
/* segment indices relatifs pour les 24 positions */
static int segL[24], segR[24], ph[24];
static int best_hist[3][25];
/* L : lignes 26 (réf.), 27, 28 : on énumère (a27, a28) ∈ 26², puis m_j = majorité */
static int fit_L(const int *K, int *bm, int *ba) {
  int best = -1;
  for (int a1 = 0; a1 < 26; a1++) for (int a2 = 0; a2 < 26; a2++) {
    int a[3] = {0, a1, a2}, cnt[7][26]; memset(cnt, 0, sizeof cnt);
    for (int t = 0; t < 24; t++) cnt[ph[t]][md(K[t] - a[segL[t]])]++;
    int s = 0, mm[7];
    for (int j = 0; j < 7; j++) { int b = 0; for (int v = 1; v < 26; v++) if (cnt[j][v] > cnt[j][b]) b = v; s += cnt[j][b]; mm[j] = b; }
    if (s > best) { best = s; memcpy(bm, mm, sizeof mm); ba[0] = 0; ba[1] = a1; ba[2] = a2; }
  }
  return best;
}
/* R : lignes de 7 n° 3 (réf.), 4, 9, 10. Candidats pour a_r : K(r, j) − K(3, j) sur les phases communes.
 * Exact tant que chaque ligne garde au moins une phase commune juste avec la ligne 3 (vrai si e_min ≤ 3). */
static int fit_R(const int *K) {
  int K3[7]; for (int j = 0; j < 7; j++) K3[j] = -1;
  int Kr[4][7]; for (int r = 0; r < 4; r++) for (int j = 0; j < 7; j++) Kr[r][j] = -1;
  for (int t = 0; t < 24; t++) Kr[segR[t]][ph[t]] = K[t];
  int cand[4][8], nc[4] = {1, 0, 0, 0}; cand[0][0] = 0;
  for (int r = 1; r < 4; r++) for (int j = 0; j < 7; j++) if (Kr[r][j] >= 0 && Kr[0][j] >= 0) {
    int v = md(Kr[r][j] - Kr[0][j]), dup = 0; for (int x = 0; x < nc[r]; x++) if (cand[r][x] == v) dup = 1; if (!dup) cand[r][nc[r]++] = v; }
  int best = -1;
  for (int x1 = 0; x1 < nc[1]; x1++) for (int x2 = 0; x2 < nc[2]; x2++) for (int x3 = 0; x3 < nc[3]; x3++) {
    int a[4] = {0, cand[1][x1], cand[2][x2], cand[3][x3]}, cnt[7][26]; memset(cnt, 0, sizeof cnt);
    for (int t = 0; t < 24; t++) cnt[ph[t]][md(K[t] - a[segR[t]])]++;
    int s = 0; for (int j = 0; j < 7; j++) { int b = 0; for (int v = 1; v < 26; v++) if (cnt[j][v] > cnt[j][b]) b = v; s += cnt[j][b]; }
    if (s > best) best = s;
  }
  return best;
}
static int fit_P(const int *K) { int cnt[7][26]; memset(cnt, 0, sizeof cnt); for (int t = 0; t < 24; t++) cnt[ph[t]][K[t]]++;
  int s = 0; for (int j = 0; j < 7; j++) { int b = 0; for (int v = 1; v < 26; v++) if (cnt[j][v] > cnt[j][b]) b = v; s += cnt[j][b]; } return s; }
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static void mkalpha(const char *w, int cont, int rev, int *pos /* lettre -> rang */) {
  char a[27]; int n = 0, used[26] = {0};
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (c < 0 || c > 25 || used[c]) continue; used[c] = 1; a[n++] = 'A' + c; }
  int start = cont && n ? (a[n - 1] - 'A' + 1) % 26 : 0;
  for (int k = 0; k < 26; k++) { int c = (start + k) % 26; if (!used[c]) { used[c] = 1; a[n++] = 'A' + c; } }
  for (int i = 0; i < 26; i++) pos[(rev ? a[25 - i] : a[i]) - 'A'] = i;
}
int main(int argc, char **argv) {
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = b[i] - 'A'; n++; }
  uint64_t seed = argc > 2 ? strtoull(argv[2], 0, 10) : 0;
  if (seed) { uint64_t s = seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 97; i++) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; CT[i] = s % 26; } }
  else for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A';
  for (int t = 0; t < 24; t++) { int i = POS[t]; ph[t] = i % 7; segL[t] = (i + 27) / 31 - 26; int r = i / 7; segR[t] = r == 3 ? 0 : r == 4 ? 1 : r == 9 ? 2 : 3; }
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  FILE *f = fopen(argv[1], "r"); char w[64]; long nal = 0;
  static const char *TN[5] = {"Q3", "Q2", "Q1", "Q4a", "Q4b"}, *MN[3] = {"VIG", "BEAU", "VARB"};
  while (fscanf(f, "%63s", w) == 1) {
    for (int cont = 0; cont < 2; cont++) for (int rev = 0; rev < 2; rev++) {
      int S[26]; mkalpha(w, cont, rev, S); nal++;
      for (int ty = 0; ty < 5; ty++) {
        const int *SP = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;   /* alphabet du clair */
        const int *SC = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;   /* alphabet du chiffré */
        for (int mo = 0; mo < 3; mo++) {
          int K[24];
          for (int t = 0; t < 24; t++) { int c = SC[CT[POS[t]]], p = SP[PT[t]]; K[t] = mo == 0 ? md(c - p) : mo == 1 ? md(c + p) : md(p - c); }
          int m[7], a[3];
          int sL = fit_L(K, m, a), sR = fit_R(K), sP = fit_P(K);
          best_hist[0][24 - sL]++; best_hist[1][24 - sR]++; best_hist[2][24 - sP]++;
          if (sL >= 21 || sR >= 21 || sP >= 20)
            printf("HIT %s cont=%d rev=%d %s %s : L=%d R=%d P=%d  m=%d,%d,%d,%d,%d,%d,%d a=%d,%d\n", w, cont, rev, TN[ty], MN[mo], 24 - sL, 24 - sR, 24 - sP,
                   m[0], m[1], m[2], m[3], m[4], m[5], m[6], a[1], a[2]);
        }
      }
    }
  }
  fprintf(stderr, "alphabets %ld (× 5 types × 3 conventions)\n", nal);
  const char *FN[3] = {"L (décalage par ligne du cuivre)", "R (décalage par ligne de 7)", "P (période 7 pure)"};
  for (int k = 0; k < 3; k++) { printf("e_min %s :", FN[k]); for (int x = 0; x <= 24; x++) if (best_hist[k][x]) printf(" %d:%d", x, best_hist[k][x]); printf("\n"); }
  return 0;
}
