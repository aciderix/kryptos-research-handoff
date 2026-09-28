/* kwcak.c — autoclé sur le CHIFFRÉ à l'écart L (1..96), alphabets à mot-clé : le clair des positions ≥ L est
 * entièrement déterminé par l'alphabet (clé = lettre chiffrée gravée L rangs avant). On compte les lettres des cribs
 * reproduites et on note les positions ≥ L (quadrigrammes, log10 moyen). Usage : kwcak qg.bin mots.txt [graine_null]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static const char *K4DEF = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int CT[97], ISC[97], CP[97]; static float *QG;
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
  const char *K4 = getenv("CTX") ? getenv("CTX") : K4DEF;
  uint64_t seed = argc > 3 ? strtoull(argv[3], 0, 10) : 0;
  if (seed) { uint64_t s = seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 97; i++) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; CT[i] = s % 26; } }
  else for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A';
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  static const char *TN[5] = {"Q3", "Q2", "Q1", "Q4a", "Q4b"}, *MN[3] = {"VIG", "BEAU", "VARB"}, *ZN[3] = {"σ", "AZ", "KA"};
  FILE *wf = fopen(argv[2], "r"); char w[64]; long nc = 0;
  double bestq[97]; char bestd[97][200]; for (int L = 0; L < 97; L++) { bestq[L] = -99; bestd[L][0] = 0; }
  int bestag[97] = {0};
  while (fscanf(wf, "%63s", w) == 1) for (int cont = 0; cont < 2; cont++) for (int rev = 0; rev < 2; rev++) {
    int S[26]; mkalpha(w, cont, rev, S);
    for (int ty = 0; ty < 5; ty++) {
      const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
      const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
      int YI[26]; for (int i = 0; i < 26; i++) YI[Y[i]] = i;
      for (int zr = 0; zr < 3; zr++) {
        const int *Z = zr == 0 ? X : zr == 1 ? AZ : KAP;       /* lettre-clé (chiffrée) lue dans l'alphabet du chiffré, A–Z ou KRYPTOS */
        for (int mo = 0; mo < 3; mo++) for (int L = 1; L <= 96; L++) {
          int pt[97], ag = 0, nn = 0; nc++;
          for (int i = L; i < 97; i++) { int k = Z[CT[i - L]], x = X[CT[i]]; int y = mo == 0 ? md(x - k) : mo == 1 ? md(k - x) : md(x + k); pt[i] = YI[y];
            if (ISC[i]) { nn++; if (pt[i] == CP[i]) ag++; } }
          if (97 - L < 20) continue;
          double q = 0; for (int i = L; i + 3 < 97; i++) q += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; q /= (97 - L - 3);
          if (q > bestq[L]) { bestq[L] = q; char s[98]; for (int i = L; i < 97; i++) s[i - L] = 'A' + pt[i]; s[97 - L] = 0;
            snprintf(bestd[L], 200, "%s c%d r%d %s %s lu-%s cribs %d/%d %s", w, cont, rev, TN[ty], MN[mo], ZN[zr], ag, nn, s); }
          if (nn >= 8 && ag >= nn - 1) { printf("CRIBS L=%d %s c%d r%d %s %s lu-%s : %d/%d quadri %.3f\n", L, w, cont, rev, TN[ty], MN[mo], ZN[zr], ag, nn, q); }
          if (ag > bestag[L]) bestag[L] = ag;
        }
      }
    }
  }
  printf("cas %ld\n", nc);
  for (int L = 1; L <= 77; L++) printf("L=%d meilleur accord cribs %d ; meilleur quadri %.3f : %s\n", L, bestag[L], bestq[L], bestd[L]);
  return 0;
}
