/* t14_glissements.c — les doublets de K4 sont-ils des glissements de copie ? (audit « vision », 24/09/2026)
 *
 * Hypothèse : Sanborn chiffrait à la main, par groupes de 7 ; aux colonnes 4–5 il aurait parfois recopié la lettre
 * précédente (ou suivante). Les trois doublets situés dans les cribs (NO→QQ en 25–26, ST→SS en 32–33, IN→TT en 67–68)
 * porteraient alors une lettre FAUSSE. On retire les lettres suspectes des cribs et on reteste la clé périodique :
 *   masque A : on retire 26, 33, 68 (la 2e lettre du doublet est la copie) ;
 *   masque B : on retire 25, 32, 67 (la 1re lettre est la copie) ;
 *   masque 0 : aucun retrait (référence, doit redonner les éliminations connues).
 * Famille : Quagmire III, alphabet σ QUELCONQUE, clé périodique p = 1..22, VIG / BEAU / VARB (lincsp.h).
 * Témoins : 20 chiffrés aléatoires par cas (lettres i.i.d. aux positions de cribs), mêmes retraits.
 */
#include "lincsp.h"

static int P[NCRIB], C[NCRIB], USE[NCRIB];

static int run(int p, int mode, long limit) {
  lc_reset(26 + p);
  lc_limit = limit;
  for (int t = 0; t < NCRIB; t++) {
    if (!USE[t]) continue;
    int kv[1] = {26 + CRIBPOS[t] % p};
    lc_add_enc(P[t], C[t], kv, 1, mode);
  }
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int masks[3][3] = {{-1, -1, -1}, {26, 33, 68}, {25, 32, 67}};
  const char *mname[3] = {"aucun retrait", "retrait 26,33,68", "retrait 25,32,67"};
  long LIM = 2000000;
  for (int mk = 0; mk < 3; mk++) {
    for (int t = 0; t < NCRIB; t++) { USE[t] = 1; for (int q = 0; q < 3; q++) if (CRIBPOS[t] == masks[mk][q]) USE[t] = 0; }
    int nk = 0; double exp = 0;
    printf("== %s ==\n", mname[mk]);
    for (int m = 0; m < 3; m++) for (int p = 1; p <= 22; p++) {
      for (int t = 0; t < NCRIB; t++) { P[t] = CRIBPT[t]; C[t] = CT[CRIBPOS[t]]; }
      int r = run(p, m, LIM);
      int nn = 0, nu = 0;
      for (int z = 0; z < 20; z++) { for (int t = 0; t < NCRIB; t++) C[t] = rng() % 26; int rz = run(p, m, LIM); nn += rz > 0; nu += rz < 0; }
      exp += nn / 20.0;
      if (r != 0) { nk++; printf("  %s p=%d : K4 %s ; témoins %d/20 (non tranchés %d)\n", MODENAME[m], p, r > 0 ? "COMPATIBLE" : "NON TRANCHÉ", nn, nu); }
    }
    printf("  total : K4 compatible dans %d cas sur 66 ; attendu au hasard %.1f\n", nk, exp);
  }
  return 0;
}
