/* t9_chain_kryptos.c — addition en chaîne en base 26 (règles R1–R4 de T2) avec l'amorce de 7 lettres « KRYPTOS »,
 * lue en rangs A–Z (10,17,24,15,19,14,18) ou en rangs KRYPTOS (0..6), à l'endroit et à l'envers, amorce en position 0 de K4.
 * Juges : σ quelconque (Quagmire III, eng_num) et alphabets fixés (A–Z/KRYPTOS, 12 conventions), meilleur score sur 24. */
#include "k4lib.h"
int main(void) {
  k4_init();
  int P24[NCRIB], C24[NCRIB];
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  int rank[2][26], AZ[26]; for (int i = 0; i < 26; i++) { rank[0][i] = i; rank[1][i] = KAINV[i]; AZ[i] = i; }
  const int *alph[2] = {AZ, KA};
  const char *W = "KRYPTOS";
  for (int enc = 0; enc < 2; enc++) for (int rev = 0; rev < 2; rev++) for (int rule = 1; rule <= 4; rule++) {
    int k[K4LEN];
    for (int i = 0; i < 7; i++) { int c = W[rev ? 6 - i : i] - 'A'; k[i] = enc ? KAINV[c] : c; }
    for (int i = 7; i < K4LEN; i++) {
      int v = rule == 1 ? k[i-7] + k[i-6] : rule == 2 ? k[i-7] + k[i-1] : rule == 3 ? k[i-7] - k[i-6] : k[i-7] + k[i-6] + k[i-1];
      k[i] = md(v);
    }
    int k24[NCRIB]; for (int t = 0; t < NCRIB; t++) k24[t] = k[CRIBPOS[t]];
    int q3 = 0; for (int m = 0; m < 3; m++) q3 += eng_num(P24, C24, k24, NCRIB, m);
    int fx = 0;
    for (int ap = 0; ap < 2; ap++) for (int ac = 0; ac < 2; ac++) for (int m = 0; m < 3; m++) {
      int s = 0; for (int t = 0; t < NCRIB; t++) if (fixed_pred(P24[t], k24[t], m, rank[ap], alph[ac]) == C24[t]) s++;
      if (s > fx) fx = s;
    }
    printf("amorce KRYPTOS %s %s, règle R%d : σ quelconque compatibles %d | alphabets fixés max %d/24\n",
           enc ? "rangs KRYPTOS" : "rangs A-Z", rev ? "envers" : "endroit", rule, q3, fx);
  }
  return 0;
}
