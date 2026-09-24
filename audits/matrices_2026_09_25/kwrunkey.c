/* kwrunkey.c — clé courante tirée d'un texte ANGLAIS inconnu, avec alphabets à mot-clé (audit « mot-clé pas 7 »).
 * Pour un alphabet fixé, les cribs donnent la clé en 24 points : deux fragments de 13 (21–33) et 11 (63–73) lettres,
 * lues dans un alphabet de lecture (σ, A–Z ou KRYPTOS). Si la clé est un texte anglais, ces fragments en sont.
 * Score : log10 moyen des quadrigrammes des deux fragments (10 + 8 quadrigrammes), à l'endroit et à l'envers.
 * Usage : kwrunkey qg.bin mots.txt [graine_null] ; CTX=… pour un chiffré de contrôle.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static const char *K4DEF = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int CT[97], POS[24], PT[24]; static float *QG;
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static void mkalpha(const char *w, int cont, int rev, int *pos) {
  char a[27]; int n = 0, used[26] = {0};
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (c < 0 || c > 25 || used[c]) continue; used[c] = 1; a[n++] = 'A' + c; }
  int start = cont && n ? (a[n - 1] - 'A' + 1) % 26 : 0;
  for (int k = 0; k < 26; k++) { int c = (start + k) % 26; if (!used[c]) { used[c] = 1; a[n++] = 'A' + c; } }
  for (int i = 0; i < 26; i++) pos[(rev ? a[25 - i] : a[i]) - 'A'] = i;
}
static double fscore(const int *k, int n) { double s = 0; for (int i = 0; i + 3 < n; i++) s += QG[((k[i] * 26 + k[i + 1]) * 26 + k[i + 2]) * 26 + k[i + 3]]; return s; }
typedef struct { double s; char d[160]; char key[32]; } Top;
#define NT 30
static Top top[NT];
static void push(double s, const char *d, const char *key) {
  int w = 0; for (int i = 1; i < NT; i++) if (top[i].s < top[w].s) w = i;
  if (s > top[w].s) { top[w].s = s; strcpy(top[w].d, d); strcpy(top[w].key, key); }
}
int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "rb"); QG = malloc(456976 * 4); if (fread(QG, 4, 456976, f) != 456976) return 2; fclose(f);
  for (int i = 0; i < NT; i++) top[i].s = -1e9;
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = b[i] - 'A'; n++; }
  const char *K4 = getenv("CTX") ? getenv("CTX") : K4DEF;
  uint64_t seed = argc > 3 ? strtoull(argv[3], 0, 10) : 0;
  if (seed && getenv("SHUF")) { for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A'; uint64_t s = seed * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CT[i]; CT[i] = CT[j]; CT[j] = t; } }
  else if (seed) { uint64_t s = seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 97; i++) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; CT[i] = s % 26; } }
  else for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A';
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  static const char *TN[5] = {"Q3", "Q2", "Q1", "Q4a", "Q4b"}, *MN[3] = {"VIG", "BEAU", "VARB"}, *ZN[3] = {"σ", "AZ", "KA"};
  FILE *wf = fopen(argv[2], "r"); char w[64]; long nc = 0; double hist[40] = {0};
  char themes[200][64]; int nth = 0;
  if (getenv("THEME")) { FILE *tf = fopen(getenv("THEME"), "r"); while (nth < 200 && fscanf(tf, "%63s", themes[nth]) == 1) nth++; fclose(tf); }
  while (fscanf(wf, "%63s", w) == 1) for (int cont = 0; cont < (getenv("ONLYSTD") ? 1 : 2); cont++) for (int rev = 0; rev < 2; rev++) {
    int S[26]; mkalpha(w, cont, rev, S);
    for (int th = 0; th < (nth ? nth * 4 : 5); th++) {
      int Tm[26]; const int *X, *Y; const char *tname;
      if (nth) { mkalpha(themes[th / 4], (th / 2) % 2, th % 2, Tm); tname = themes[th / 4]; }
      int nty = nth ? 2 : 1;
      for (int ty2 = 0; ty2 < nty; ty2++) {
        int ty = th;
        if (nth) { Y = ty2 == 0 ? S : Tm; X = ty2 == 0 ? Tm : S; }
        else { Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP; X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S; }
        for (int zr = 0; zr < 4; zr++) {
          const int *Z = zr == 0 ? Y : zr == 1 ? AZ : zr == 2 ? KAP : X;
          if (!nth && zr == 3) continue;
          int ZI[26]; for (int i = 0; i < 26; i++) ZI[Z[i]] = i;
          for (int mo = 0; mo < 3; mo++) {
            int k[24];
            for (int t = 0; t < 24; t++) { int x = X[CT[POS[t]]], y = Y[PT[t]]; int kv = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x); k[t] = ZI[kv]; }
            for (int dir = 0; dir < 2; dir++) {
              int kk[24]; for (int t = 0; t < 13; t++) kk[t] = dir ? k[12 - t] : k[t]; for (int t = 0; t < 11; t++) kk[13 + t] = dir ? k[23 - t] : k[13 + t];
              double sc = (fscore(kk, 13) + fscore(kk + 13, 11)) / 18.0; nc++;
              char d[200], key[32];
              if (nth) snprintf(d, sizeof d, "%s c%d r%d | thème %s c%d r%d | %s %s lu-%d %s", w, cont, rev, tname, (th / 2) % 2, th % 2, ty2 == 0 ? "clair=dico" : "clair=thème", MN[mo], zr, dir ? "envers" : "endroit");
              else snprintf(d, sizeof d, "%s cont=%d rev=%d %s %s lu-%s %s", w, cont, rev, TN[ty], MN[mo], ZN[zr < 3 ? zr : 0], dir ? "envers" : "endroit");
              for (int t = 0; t < 24; t++) key[t + (t >= 13)] = 'A' + kk[t]; key[13] = '|'; key[25] = 0;
              push(sc, d, key);
            }
          }
        }
      }
    }
  }
  printf("cas %ld\n", nc);
  for (int i = 0; i < NT; i++) for (int j = i + 1; j < NT; j++) if (top[j].s > top[i].s) { Top x = top[i]; top[i] = top[j]; top[j] = x; }
  for (int i = 0; i < 12; i++) printf("%.3f %s %s\n", top[i].s, top[i].key, top[i].d);
  return 0;
}
