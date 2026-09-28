/* t3_compound.c — DEUX mots-clés superposés (Vigenère composé), cellule marquée OPEN « non énumérée »
 * dans docs/two_systems_landscape.md (« Two periodic substitutions (different keywords) »).
 *
 * « the keywords required to break the code » (AP 1991, pluriel) ; Scheidt : « avec le ou les bons mots-clés ».
 * Modèle : k[i] = a[i mod p1] + b[i mod p2] (mod 26), a et b inconnus (donc TOUT couple de mots-clés).
 * La période effective est ppcm(p1, p2) (souvent > 52, hors des éliminations périodiques),
 * mais il n'y a que p1 + p2 - 1 inconnues.
 * Alphabets fixés (Sanborn : A–Z et KRYPTOS) : clair ∈ {AZ, KA}, chiffré ∈ {AZ, KA}, VIG/BEAU/VARB (12 conventions).
 * Résolution exacte : système linéaire mod 2 et mod 13 (Z26 = Z2 × Z13), pivot de Gauss.
 * p1 = 1..30, p2 = p1+1..48. Témoins : 200 chiffrés aléatoires, même balayage.
 * Règle fixée d'avance : une case (p1,p2,convention) n'est « signal » que si K4 y est compatible alors que
 * ≤ 1 % des témoins le sont ; on compare aussi le NOMBRE total de cases compatibles au témoin.
 */
#include "k4lib.h"

static int P24[NCRIB], C24[NCRIB];
static int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
static int lcm(int a, int b) { return a / gcd(a, b) * b; }

/* résout A x = y mod q (q premier) ; A : n lignes, m colonnes (m ≤ 80) ; renvoie 1 si compatible */
static int solvable_modq(int n, int m, int A[][80], int *y, int q) {
  int M[NCRIB][81];
  for (int i = 0; i < n; i++) { for (int j = 0; j < m; j++) M[i][j] = ((A[i][j] % q) + q) % q; M[i][m] = ((y[i] % q) + q) % q; }
  int row = 0;
  for (int col = 0; col < m && row < n; col++) {
    int piv = -1; for (int i = row; i < n; i++) if (M[i][col]) { piv = i; break; }
    if (piv < 0) continue;
    if (piv != row) for (int j = 0; j <= m; j++) { int t = M[row][j]; M[row][j] = M[piv][j]; M[piv][j] = t; }
    int inv = 1; for (int t = 1; t < q; t++) if (M[row][col] * t % q == 1) { inv = t; break; }
    for (int j = 0; j <= m; j++) M[row][j] = M[row][j] * inv % q;
    for (int i = 0; i < n; i++) if (i != row && M[i][col]) { int f = M[i][col]; for (int j = 0; j <= m; j++) M[i][j] = ((M[i][j] - f * M[row][j]) % q + q) % q; }
    row++;
  }
  for (int i = row; i < n; i++) if (M[i][m]) return 0;
  return 1;
}

static int compound_ok(const int *k24, int p1, int p2) {
  int A[NCRIB][80] = {{0}}, m = p1 + p2;
  for (int t = 0; t < NCRIB; t++) { int i = CRIBPOS[t]; A[t][i % p1] = 1; A[t][p1 + i % p2] = 1; }
  int y[NCRIB]; for (int t = 0; t < NCRIB; t++) y[t] = k24[t];
  return solvable_modq(NCRIB, m, A, y, 2) && solvable_modq(NCRIB, m, A, y, 13);
}

static int AZID[26], RANK[2][26];
static void keys_for(int conv, int *k24) {
  int ap = conv / 6, ac = (conv / 3) % 2, mode = conv % 3;
  for (int t = 0; t < NCRIB; t++) {
    int x = RANK[ap][P24[t]], y = RANK[ac][C24[t]];
    k24[t] = mode == VIG ? md(y - x) : mode == BEAU ? md(y + x) : md(x - y);
  }
}
static const char *AL[2] = {"AZ", "KA"};

#define P1MAX 30
#define P2MAX 48
static unsigned char hitK4[12][P1MAX + 1][P2MAX + 1];
static int nullcnt[12][P1MAX + 1][P2MAX + 1];

static int sweep(unsigned char out[12][P1MAX + 1][P2MAX + 1]) {
  int tot = 0, k24[NCRIB];
  for (int conv = 0; conv < 12; conv++) {
    keys_for(conv, k24);
    for (int p1 = 1; p1 <= P1MAX; p1++) for (int p2 = p1 + 1; p2 <= P2MAX; p2++) {
      int ok = compound_ok(k24, p1, p2);
      out[conv][p1][p2] = ok; tot += ok;
    }
  }
  return tot;
}

int main(int argc, char **argv) {
  int NNULL = argc > 1 ? atoi(argv[1]) : 200;
  k4_init();
  for (int i = 0; i < 26; i++) { AZID[i] = i; RANK[0][i] = i; RANK[1][i] = KAINV[i]; }
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  /* contrôle positif : faux K4 chiffré en QIII-KA avec PALIMPSEST (10) + ABSCISSA (8) */
  {
    const char *w1 = "PALIMPSEST", *w2 = "ABSCISSA"; int save[NCRIB]; memcpy(save, C24, sizeof save);
    for (int t = 0; t < NCRIB; t++) { int i = CRIBPOS[t]; int kv = KAINV[w1[i % 10] - 'A'] + KAINV[w2[i % 8] - 'A']; C24[t] = KA[md(KAINV[P24[t]] + kv)]; }
    int k24[NCRIB]; keys_for(1 * 6 + 1 * 3 + VIG, k24);
    printf("contrôle positif (10,8) KA/KA VIG : %d (attendu 1) ; (7,9) : %d\n", compound_ok(k24, 8, 10), compound_ok(k24, 7, 9));
    memcpy(C24, save, sizeof save);
  }
  int totK4 = sweep(hitK4);
  printf("K4 : %d cases compatibles sur %d\n", totK4, 12 * (P1MAX * (2 * P2MAX - P1MAX - 1) / 2));
  static unsigned char tmp[12][P1MAX + 1][P2MAX + 1];
  long totnull = 0; int ge = 0;
  for (int z = 0; z < NNULL; z++) {
    for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
    int tn = sweep(tmp); totnull += tn; if (tn >= totK4) ge++;
    for (int c = 0; c < 12; c++) for (int p1 = 1; p1 <= P1MAX; p1++) for (int p2 = p1 + 1; p2 <= P2MAX; p2++) nullcnt[c][p1][p2] += tmp[c][p1][p2];
  }
  printf("témoins : moyenne %.1f cases compatibles ; P(témoin ≥ K4) = %d/%d\n", (double)totnull / NNULL, ge, NNULL);
  /* cases sévères : compatibles pour K4, rares au hasard */
  int nsev = 0; double expsev = 0;
  for (int c = 0; c < 12; c++) for (int p1 = 1; p1 <= P1MAX; p1++) for (int p2 = p1 + 1; p2 <= P2MAX; p2++) {
    double r = (double)nullcnt[c][p1][p2] / NNULL;
    if (r <= 0.01) { expsev += r; if (hitK4[c][p1][p2]) { nsev++; printf("  SÉVÈRE : clair %s chiffré %s %s p1=%d p2=%d ppcm=%d (hasard %.3f)\n", AL[c / 6], AL[(c / 3) % 2], MODENAME[c % 3], p1, p2, lcm(p1, p2), r); } }
  }
  printf("cases sévères (hasard ≤ 1 %%) compatibles pour K4 : %d ; attendu au hasard : %.2f\n", nsev, expsev);
  /* plus petites (p1 + p2) compatibles pour K4, par convention */
  for (int c = 0; c < 12; c++) {
    int best = 999, b1 = 0, b2 = 0;
    for (int p1 = 1; p1 <= P1MAX; p1++) for (int p2 = p1 + 1; p2 <= P2MAX; p2++) if (hitK4[c][p1][p2] && p1 + p2 < best) { best = p1 + p2; b1 = p1; b2 = p2; }
    printf("  %s/%s %s : plus petit couple compatible (%d,%d) somme %d ; hasard à ce couple %.2f\n", AL[c / 6], AL[(c / 3) % 2], MODENAME[c % 3], b1, b2, best, b1 ? (double)nullcnt[c][b1][b2] / NNULL : 0);
  }
  return 0;
}
