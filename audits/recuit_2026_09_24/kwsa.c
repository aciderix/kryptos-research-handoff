/* kwsa.c — recuit sur le MOT-CLÉ (chaîne quelconque de 3 à 14 lettres, hors dictionnaire compris) pour la clé courante
 * anglaise : alphabet = mot-clé puis reste A–Z (et son inverse) ; score = meilleur, sur les 5 types, 3 conventions et
 * 3 lectures de clé, du log10 moyen des quadrigrammes des deux fragments de clé (13 + 11 lettres), endroit ou envers.
 * Mouvements : changer, insérer ou retirer une lettre du mot-clé. Usage : kwsa qg.bin iters restarts seed ; CTX, SHUF.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
static const char *K4DEF = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int CT[97], POS[24], PT[24]; static float *QG;
static uint64_t rs = 88172645463325252ULL;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static inline double ur(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int AZ[26], KAP[26];
static void mkalpha(const char *w, int rev, int *pos) {
  char a[27]; int n = 0, used[26] = {0};
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (used[c]) continue; used[c] = 1; a[n++] = 'A' + c; }
  for (int c = 0; c < 26; c++) if (!used[c]) { used[c] = 1; a[n++] = 'A' + c; }
  for (int i = 0; i < 26; i++) pos[(rev ? a[25 - i] : a[i]) - 'A'] = i;
}
static double fs(const int *k, int n) { double s = 0; for (int i = 0; i + 3 < n; i++) s += QG[((k[i] * 26 + k[i + 1]) * 26 + k[i + 2]) * 26 + k[i + 3]]; return s; }
static char bestdesc[200];
static double score(const char *w, int want_desc) {
  double best = -99;
  for (int rev = 0; rev < 2; rev++) {
    int S[26]; mkalpha(w, rev, S);
    for (int ty = 0; ty < 5; ty++) {
      const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
      const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
      for (int zr = 0; zr < 3; zr++) {
        const int *Z = zr == 0 ? (ty == 1 ? S : Y) : zr == 1 ? AZ : KAP;
        int ZI[26]; for (int i = 0; i < 26; i++) ZI[Z[i]] = i;
        for (int mo = 0; mo < 3; mo++) {
          int k[24];
          for (int t = 0; t < 24; t++) { int x = X[CT[POS[t]]], y = Y[PT[t]]; int kv = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x); k[t] = ZI[kv]; }
          for (int dir = 0; dir < 2; dir++) {
            int kk[24]; for (int t = 0; t < 13; t++) kk[t] = dir ? k[12 - t] : k[t]; for (int t = 0; t < 11; t++) kk[13 + t] = dir ? k[23 - t] : k[13 + t];
            double sc = (fs(kk, 13) + fs(kk + 13, 11)) / 18.0;
            if (sc > best) { best = sc; if (want_desc) { char key[26]; for (int t = 0; t < 24; t++) key[t + (t >= 13)] = 'A' + kk[t]; key[13] = '|'; key[25] = 0;
                snprintf(bestdesc, sizeof bestdesc, "%s rev=%d type=%d mode=%d lu=%d dir=%d", key, rev, ty, mo, zr, dir); } }
          }
        }
      }
    }
  }
  return best;
}
int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "rb"); QG = malloc(456976 * 4); if (fread(QG, 4, 456976, f) != 456976) return 2; fclose(f);
  long iters = atol(argv[2]); int R = atoi(argv[3]); rs ^= 0x9E3779B97F4A7C15ULL * (atol(argv[4]) + 1);
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = b[i] - 'A'; n++; }
  const char *K4 = getenv("CTX") ? getenv("CTX") : K4DEF; for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A';
  if (getenv("SHUF")) { uint64_t s = strtoull(getenv("SHUF"), 0, 10) * 0x9E3779B97F4A7C15ULL; for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CT[i]; CT[i] = CT[j]; CT[j] = t; } }
  for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  double gb = -99; char gw[32] = "";
  for (int r = 0; r < R; r++) {
    char w[32]; int L = 4 + rnd() % 8; for (int i = 0; i < L; i++) w[i] = 'A' + rnd() % 26; w[L] = 0;
    double cur = score(w, 0), bb = cur; char bw[32]; strcpy(bw, w);
    for (long it = 0; it < iters; it++) {
      double T = 0.08 * pow(0.004 / 0.08, (double)it / iters);
      char x[32]; strcpy(x, w); int l = strlen(x), m = rnd() % 10;
      if (m < 7 || l <= 3 || l >= 14) { if (m >= 7 && l <= 3) { x[l] = 'A' + rnd() % 26; x[l + 1] = 0; } else x[rnd() % l] = 'A' + rnd() % 26; }
      else if (m < 9) { int p = rnd() % (l + 1); memmove(x + p + 1, x + p, l - p + 1); x[p] = 'A' + rnd() % 26; }
      else { int p = rnd() % l; memmove(x + p, x + p + 1, l - p); }
      double s = score(x, 0);
      if (s >= cur || ur() < exp((s - cur) / T)) { strcpy(w, x); cur = s; if (cur > bb) { bb = cur; strcpy(bw, w); } }
    }
    if (bb > gb) { gb = bb; strcpy(gw, bw); }
  }
  score(gw, 1); printf("%.3f %s %s\n", gb, gw, bestdesc);
  return 0;
}
