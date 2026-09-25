/* t22_periodique.c — e_min pour la clé périodique (p = 1..26), Quagmire I–IV, alphabets quelconques.
 * Question : combien d'erreurs de chiffrement faudrait-il pour qu'un chiffre périodique (le système de K1–K2, du
 * Cyrillic Projector, du petit fragment de 97 lettres) soit vrai ? Et K4 en demande-t-il moins que le hasard ?
 * Usage : t22 [ntémoins] [emax]
 */
#include "emin.h"
int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0); k4_init();
  int NN = argc > 1 ? atoi(argv[1]) : 100, EMAX = argc > 2 ? atoi(argv[2]) : 7;
  int pc[NCRIB], cc[NCRIB], rc[NCRIB];
  for (int t = 0; t < NCRIB; t++) { pc[t] = CRIBPT[t]; cc[t] = CT[CRIBPOS[t]]; }
  static const int QS[4] = {Q3, Q4, Q2, Q1};
  for (int qi = 0; qi < 4; qi++) for (int mode = 0; mode < 2; mode++) {
    int q = QS[qi];
    if ((q == Q4 || q == Q1) && mode == BEAU) continue;   /* équivalent à VIG par σ1 → −σ1 */
    for (int p = 1; p <= 26; p++) {
      Fam F; memset(&F, 0, sizeof F); F.q = q; F.mode = mode; F.nkey = p;
      for (int i = 0; i < K4LEN + 2; i++) { F.nk[i] = 1; F.kv[i][0] = i % p; F.kc[i][0] = 1; F.kk[i] = 0; }
      Ihs I; uint32_t H; int e = emin(&F, pc, cc, EMAX, &I, &H);
      /* témoins */
      int hist[16] = {0}; long ns = 0;
      for (int z = 0; z < NN; z++) {
        for (int t = 0; t < NCRIB; t++) rc[t] = rng() % 26;
        Ihs J; int ez = emin(&F, pc, rc, EMAX, &J, NULL); hist[ez]++; ns += J.nsolve;
      }
      int le = 0; for (int x = 0; x <= e && x <= EMAX + 1; x++) le += hist[x];
      printf("%s %s p=%2d : K4 e_min=%d", QN[q], MODEN[mode], p, e);
      if (e <= EMAX) { printf(" (ex. retirer"); for (int t = 0; t < NCRIB; t++) if (H >> t & 1) printf(" %d", CRIBPOS[t]); printf(")"); }
      printf("%s ; témoins e_min :", I.undecided ? " [non tranché]" : "");
      for (int x = 0; x <= EMAX + 1; x++) printf(" %d", hist[x]);
      printf(" ; P(témoin ≤ K4) = %.3f\n", (double)le / NN);
    }
  }
  return 0;
}
