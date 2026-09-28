/* genalpha.c — génère les alphabets candidats (sans doublons, un alphabet et son inverse comptent pour un).
 * Formes : L0 standard (mot puis reste A–Z), L1 « suite » (reste à partir de la lettre qui suit la dernière du mot),
 *          Mw* matrice (alphabet standard écrit en lignes de w = 2..13, relu par colonnes : G→D haut/bas, D→G haut/bas,
 *          ordre alphabétique des lettres du mot si w = nombre de lettres distinctes),
 *          K2 deux mots-clés (mot1 + mot2 + reste) pour les couples d'une liste thématique × dictionnaire.
 * Usage : genalpha mots.txt [themes.txt] > alphas.bin   (26 octets par alphabet, lettres A–Z) ; étiquettes sur stderr si LABELS=1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define HS (1u << 25)
static uint64_t *H; static int lab;
static uint64_t hash26(const char *a) { uint64_t h = 1469598103934665603ULL; for (int i = 0; i < 26; i++) h = (h ^ (uint8_t)a[i]) * 1099511628211ULL; return h | 1; }
static int add(const char *a, const char *w, const char *form) {
  char r[26]; for (int i = 0; i < 26; i++) r[i] = a[25 - i];
  uint64_t h1 = hash26(a), h2 = hash26(r), h = h1 < h2 ? h1 : h2;
  uint32_t i = h & (HS - 1);
  while (H[i]) { if (H[i] == h) return 0; i = (i + 1) & (HS - 1); }
  H[i] = h; fwrite(a, 1, 26, stdout); if (lab) fprintf(stderr, "%s %s\n", w, form); return 1;
}
static void kwalpha(const char *w, int cont, char *a) {
  int used[26] = {0}, n = 0;
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (c < 0 || c > 25 || used[c]) continue; used[c] = 1; a[n++] = 'A' + c; }
  int start = cont && n ? (a[n - 1] - 'A' + 1) % 26 : 0;
  for (int k = 0; k < 26; k++) { int c = (start + k) % 26; if (!used[c]) { used[c] = 1; a[n++] = 'A' + c; } }
}
static void matrices(const char *w, const char *a) {
  char out[27]; out[26] = 0; char form[16];
  int u[26], nu = 0, used[26] = {0};
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (c >= 0 && c < 26 && !used[c]) { used[c] = 1; u[nu++] = c; } }
  for (int W = 2; W <= 13; W++) {
    int rows = (26 + W - 1) / W;
    for (int lr = 0; lr < 2; lr++) for (int ud = 0; ud < 2; ud++) {
      int n = 0;
      for (int cc = 0; cc < W; cc++) { int c = lr ? W - 1 - cc : cc;
        for (int rr = 0; rr < rows; rr++) { int r = ud ? rows - 1 - rr : rr; int idx = r * W + c; if (idx < 26) out[n++] = a[idx]; } }
      snprintf(form, sizeof form, "M%d%c%c", W, lr ? 'R' : 'L', ud ? 'u' : 'd'); add(out, w, form);
    }
    if (nu == W) { int ord[26]; for (int i = 0; i < W; i++) ord[i] = i;
      for (int i = 0; i < W; i++) for (int j = i + 1; j < W; j++) if (u[ord[j]] < u[ord[i]]) { int t = ord[i]; ord[i] = ord[j]; ord[j] = t; }
      int n = 0; for (int k = 0; k < W; k++) { int c = ord[k]; for (int r = 0; r < rows; r++) { int idx = r * W + c; if (idx < 26) out[n++] = a[idx]; } }
      snprintf(form, sizeof form, "M%dK", W); add(out, w, form); }
  }
}
int main(int argc, char **argv) {
  H = calloc(HS, 8); lab = getenv("LABELS") != NULL;
  int nomat = getenv("NOMAT") != NULL;
  FILE *f = fopen(argv[1], "r"); char w[128], a[27]; a[26] = 0; long n = 0;
  static char words[400000][32]; int nw = 0;
  while (fscanf(f, "%127s", w) == 1) {
    for (char *p = w; *p; p++) if (*p >= 'a' && *p <= 'z') *p -= 32;
    if (nw < 400000) { strncpy(words[nw], w, 31); words[nw][31] = 0; nw++; }
    kwalpha(w, 0, a); n += add(a, w, "L0");
    kwalpha(w, 1, a); n += add(a, w, "L1");
    if (!nomat) { kwalpha(w, 0, a); matrices(w, a); }
  }
  if (argc > 2) { FILE *t = fopen(argv[2], "r"); char th[64];
    while (fscanf(t, "%63s", th) == 1) for (int i = 0; i < nw; i++) {
      char cat[128]; snprintf(cat, sizeof cat, "%s%s", th, words[i]); kwalpha(cat, 0, a); add(a, cat, "K2");
      snprintf(cat, sizeof cat, "%s%s", words[i], th); kwalpha(cat, 0, a); add(a, cat, "K2"); } }
  fprintf(stderr, "alphabets distincts : %ld\n", n);
  return 0;
}
