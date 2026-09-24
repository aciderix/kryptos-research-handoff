/* t8_inv563.c — la « chaîne de masquage » proposée sur le groupe le 23/09/2017 (C. Ellis, « Ed's Masking String »),
 * sans explication publiée. Ses 252 chiffres sont le développement décimal de 1/563 à partir de la 3ᵉ décimale
 * (0,00|177619893428…) ; période 281. On l'essaie comme clé NUMÉRIQUE additive :
 *   D1 : un chiffre par lettre (0–9) ;
 *   D2 : paires de chiffres lues mod 26 (00–99 → 0–25), les deux phases ;
 * tous décalages dans la période. Juges : NUM (σ quelconque, même σ côté clair et chiffré, eng_num) et 12 conventions
 * à alphabets fixés (A–Z / KRYPTOS côté clair et côté chiffré, VIG/BEAU/VARB), meilleur score sur 24.
 * Témoin : 10 chiffrés aléatoires, même balayage.
 */
#include "k4lib.h"

static int P24[NCRIB], C24[NCRIB];
static int dig[2 * 281 + 4];

typedef struct { long n, num; int fx; } T;

static void judge(const int *k, T *R) {
  int rank[2][26], AZ[26];
  for (int i = 0; i < 26; i++) { rank[0][i] = i; rank[1][i] = KAINV[i]; AZ[i] = i; }
  const int *alph[2] = {AZ, KA};
  R->n++;
  for (int m = 0; m < 3; m++) if (eng_num(P24, C24, k, NCRIB, m)) R->num++;
  for (int ap = 0; ap < 2; ap++) for (int ac = 0; ac < 2; ac++) for (int m = 0; m < 3; m++) {
    int s = 0; for (int t = 0; t < NCRIB; t++) if (fixed_pred(P24[t], k[t] % 26, m, rank[ap], alph[ac]) == C24[t]) s++;
    if (s > R->fx) R->fx = s;
  }
}

static void sweep(T *R1, T *R2) {
  int k[NCRIB];
  for (int off = 0; off < 281; off++) {                     /* D1 : un chiffre par position */
    for (int t = 0; t < NCRIB; t++) k[t] = dig[(CRIBPOS[t] + off) % 281];
    judge(k, R1);
  }
  for (int off = 0; off < 562; off++) {                     /* D2 : paires de chiffres mod 26, toutes phases */
    for (int t = 0; t < NCRIB; t++) {
      int a = (2 * CRIBPOS[t] + off) % 281, b = (2 * CRIBPOS[t] + off + 1) % 281;
      k[t] = (10 * dig[a] + dig[b]) % 26;
    }
    judge(k, R2);
  }
}

int main(void) {
  k4_init();
  /* 1/563 : chiffres par division longue, en commençant à la 3ᵉ décimale (comme la chaîne proposée) */
  int r = 1, all[300];
  for (int i = 0; i < 283; i++) { r *= 10; all[i] = r / 563; r %= 563; }
  for (int i = 0; i < 281; i++) dig[i] = all[i + 2];
  printf("contrôle : 1/563 à partir de la 3e décimale = ");
  for (int i = 0; i < 20; i++) printf("%d", dig[i]);
  printf("… (attendu 17761989342806394316)\n");
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  T a = {0}, b = {0}; sweep(&a, &b);
  printf("K4  D1 (1 chiffre)     : %ld décalages | σ quelconque compatibles %ld | alphabets fixés max %d/24\n", a.n, a.num, a.fx);
  printf("K4  D2 (paires mod 26) : %ld décalages | σ quelconque compatibles %ld | alphabets fixés max %d/24\n", b.n, b.num, b.fx);
  for (int z = 0; z < 10; z++) {
    for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
    T x = {0}, y = {0}; sweep(&x, &y);
    printf("témoin %d : D1 σ %ld, fixés %d/24 | D2 σ %ld, fixés %d/24\n", z, x.num, x.fx, y.num, y.fx);
  }
  return 0;
}
