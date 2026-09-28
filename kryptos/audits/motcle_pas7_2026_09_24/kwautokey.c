/* kwautokey.c — autoclé sur le CLAIR à l'écart L (1..13) avec alphabets à mot-clé : le clair entier est déterminé.
 * (audit « mot-clé pas 7 », 24/09/2026)
 * Modèle (Q-type, convention) : X(c_i) = Y(p_i) ⊕ key_i, key_i = Z(p_{i−L}) ; X = alphabet du chiffré, Y = du clair,
 * Z = lecture de la lettre-clé (σ, A–Z ou KRYPTOS). ⊕ : VIG +, BEAU k − Y, VARB Y − k.
 * Pour L ≤ 13, chaque classe modulo L contient une position de crib : on propage le clair dans les deux sens depuis
 * les cribs (chaque classe depuis sa première position de crib), on compte les lettres des cribs reproduites,
 * et on note le clair complet (quadrigrammes, log10 moyen par quadrigramme).
 * Usage : kwautokey qg.bin mots.txt [graine_null] ; sortie : cas avec ≥ seuil accords, et meilleurs scores.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static const char *K4DEF = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"; static const char *K4;
static int CT[97], ISC[97], CP[97];
static float *QG;
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
  FILE *f = fopen(argv[1], "rb"); QG = malloc(456976 * 4); if (fread(QG, 4, 456976, f) != 456976) return 2; fclose(f);
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { ISC[21 + i] = 1; CP[21 + i] = e[i] - 'A'; }
  for (int i = 0; i < 11; i++) { ISC[63 + i] = 1; CP[63 + i] = b[i] - 'A'; }
  K4 = getenv("CTX") ? getenv("CTX") : K4DEF;
  uint64_t seed = argc > 3 ? strtoull(argv[3], 0, 10) : 0;
  if (seed) { uint64_t s = seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 97; i++) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; CT[i] = s % 26; } }
  else for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A';
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  FILE *wf = fopen(argv[2], "r"); char w[64];
  int hist[25] = {0}; double bestq = -99; char bestdesc[256] = "", bestpt[98] = "";
  static const char *TN[5] = {"Q3", "Q2", "Q1", "Q4a", "Q4b"}, *MN[3] = {"VIG", "BEAU", "VARB"}, *ZN[3] = {"σ", "AZ", "KA"};
  long ncase = 0;
  while (fscanf(wf, "%63s", w) == 1) for (int cont = 0; cont < 2; cont++) for (int rev = 0; rev < 2; rev++) {
    int S[26], SI[26]; mkalpha(w, cont, rev, S);
    for (int ty = 0; ty < 5; ty++) {
      const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
      const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
      int YI[26]; for (int i = 0; i < 26; i++) YI[Y[i]] = i; (void)SI;
      for (int zr = 0; zr < 3; zr++) {
        const int *Z = zr == 0 ? Y : zr == 1 ? AZ : KAP;     /* lecture de la lettre-clé ; σ = alphabet du clair */
        if (zr > 0 && Z == Y) continue;
        for (int mo = 0; mo < 3; mo++) for (int L = 1; L <= 13; L++) {
          ncase++;
          int pt[97], agree = 0;
          for (int r = 0; r < L; r++) {
            /* première position de crib de la classe r */
            int s = -1; for (int i = 21; i <= 73; i++) if (ISC[i] && i % L == r) { s = i; break; }
            if (s < 0) { s = -1; }
            pt[s] = CP[s];
            /* vers l'avant : p_i depuis p_{i-L} */
            for (int i = s + L; i < 97; i += L) {
              int k = Z[pt[i - L]], x = X[CT[i]], y = mo == 0 ? md(x - k) : mo == 1 ? md(k - x) : md(x + k);
              pt[i] = YI[y];
              if (ISC[i]) { if (pt[i] == CP[i]) agree++; }
            }
            /* vers l'arrière : p_{i-L} depuis p_i : k = x − y (VIG), x + y (BEAU), y − x (VARB) */
            for (int i = s; i - L >= 0; i -= L) {
              int x = X[CT[i]], y = Y[pt[i]], k = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x);
              int q = -1; for (int t = 0; t < 26; t++) if (Z[t] == k) { q = t; break; }
              pt[i - L] = q;
            }
            agree++; /* la position de départ */
          }
          int ea = 24 - agree; hist[ea]++;
          double qs = 0; for (int i = 0; i + 3 < 97; i++) qs += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; qs /= 94;
          if (ea <= 3 || qs > -4.9) {
            printf("CAS %s cont=%d rev=%d %s %s clé-lue-%s L=%d : erreurs %d, quadri %.3f ", w, cont, rev, TN[ty], MN[mo], ZN[zr], L, ea, qs);
            for (int i = 0; i < 97; i++) putchar('A' + pt[i]); putchar('\n');
          }
          if (qs > bestq) { bestq = qs; snprintf(bestdesc, sizeof bestdesc, "%s cont=%d rev=%d %s %s %s L=%d err=%d", w, cont, rev, TN[ty], MN[mo], ZN[zr], L, ea);
            for (int i = 0; i < 97; i++) bestpt[i] = 'A' + pt[i]; bestpt[97] = 0; }
        }
      }
    }
  }
  printf("cas %ld ; histogramme des erreurs :", ncase); for (int x = 0; x <= 24; x++) if (hist[x]) printf(" %d:%d", x, hist[x]);
  printf("\nmeilleur quadri %.3f : %s %s\n", bestq, bestdesc, bestpt);
  return 0;
}
