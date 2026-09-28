/* build_qg.c — table de quadrigrammes anglais (log10) à partir d'un corpus texte (lettres A–Z seulement).
 * Usage : build_qg sortie.bin fichier1 fichier2 ...   (les en-têtes Gutenberg sont sautés grossièrement) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
int main(int argc, char **argv) {
  static double cnt[26 * 26 * 26 * 26]; double tot = 0;
  for (int f = 2; f < argc; f++) {
    FILE *fp = fopen(argv[f], "r"); if (!fp) continue;
    fseek(fp, 0, SEEK_END); long n = ftell(fp); fseek(fp, 0, SEEK_SET);
    char *b = malloc(n + 1); fread(b, 1, n, fp); b[n] = 0; fclose(fp);
    char *s = strstr(b, "*** START"); if (s) { s = strchr(s + 9, '\n'); } else s = b;
    char *e = strstr(s ? s : b, "*** END"); if (e) *e = 0;
    int h[4], k = 0;
    for (char *p = s; *p; p++) { int c = toupper((unsigned char)*p); if (c < 'A' || c > 'Z') continue;
      h[0] = h[1]; h[1] = h[2]; h[2] = h[3]; h[3] = c - 'A'; if (++k >= 4) { cnt[((h[0] * 26 + h[1]) * 26 + h[2]) * 26 + h[3]]++; tot++; } }
    free(b);
  }
  static float lp[26 * 26 * 26 * 26];
  double fl = log10(0.01 / tot);
  for (int i = 0; i < 26 * 26 * 26 * 26; i++) lp[i] = cnt[i] > 0 ? log10(cnt[i] / tot) : fl;
  FILE *o = fopen(argv[1], "wb"); fwrite(lp, sizeof lp, 1, o); fclose(o);
  fprintf(stderr, "quadrigrammes : %.0f ; plancher %.2f\n", tot, fl);
  return 0;
}
