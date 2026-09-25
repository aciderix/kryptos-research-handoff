/* emin.h — nombre minimal d'erreurs e_min : combien de lettres des cribs faut-il déclarer fausses
 * pour que la famille devienne compatible ? Méthode exacte : « ensembles couvrants implicites »
 * (on accumule des noyaux incompatibles minimaux, on cherche le plus petit ensemble qui les touche tous,
 * on le teste ; s'il échoue, on extrait un nouveau noyau hors de lui). Résultat optimal par construction.
 */
#ifndef EMIN_H
#define EMIN_H
#include "imx.h"

#define MAXCORES 4096
typedef struct { uint32_t core[MAXCORES]; int ncore; long nsolve; int undecided; } Ihs;

static int sol(Ihs *I, const Fam *F, const int *pc, const int *cc, uint32_t mask) {
  I->nsolve++; int r = solve_mask(F, pc, cc, mask);
  if (r < 0) { I->undecided = 1; return 1; }   /* non tranché : compté comme compatible (prudent pour e_min) */
  return r;
}
/* noyau minimal dans R (supposé incompatible) */
static uint32_t shrink(Ihs *I, const Fam *F, const int *pc, const int *cc, uint32_t R) {
  for (int t = 0; t < NCRIB; t++) if (R >> t & 1) { uint32_t R2 = R & ~(1u << t); if (!sol(I, F, pc, cc, R2)) R = R2; }
  return R;
}
/* plus petit ensemble H (|H| = k) qui touche tous les noyaux ; énumération par taille croissante */
static int hits_all(const Ihs *I, uint32_t H) { for (int j = 0; j < I->ncore; j++) if (!(I->core[j] & H)) return 0; return 1; }
static int find_hs(const Ihs *I, int k, uint32_t cand, uint32_t *out, uint32_t cur, int start) {
  if (k == 0) { if (hits_all(I, cur)) { *out = cur; return 1; } return 0; }
  /* élagage : le premier noyau non touché doit être touché par un élément ≥ start */
  int j; for (j = 0; j < I->ncore; j++) if (!(I->core[j] & cur)) break;
  if (j == I->ncore) { *out = cur; return 1; }
  uint32_t need = I->core[j] & cand;
  for (int t = 0; t < NCRIB; t++) if (need >> t & 1) {
    if (find_hs(I, k - 1, cand, out, cur | 1u << t, 0)) return 1;
  }
  return 0;
}
#define FULL ((1u << NCRIB) - 1)
/* renvoie e_min (≤ emax) ou emax+1 si plus */
static int emin(const Fam *F, const int *pc, const int *cc, int emax, Ihs *I, uint32_t *best) {
  I->ncore = 0; I->nsolve = 0; I->undecided = 0;
  if (sol(I, F, pc, cc, FULL)) { if (best) *best = 0; return 0; }
  I->core[I->ncore++] = shrink(I, F, pc, cc, FULL);
  for (int k = 1; k <= emax; ) {
    uint32_t H;
    if (!find_hs(I, k, FULL, &H, 0, 0)) { k++; continue; }
    if (sol(I, F, pc, cc, FULL & ~H)) { if (best) *best = H; return k; }
    if (I->ncore >= MAXCORES) return emax + 1;
    I->core[I->ncore++] = shrink(I, F, pc, cc, FULL & ~H);
  }
  return emax + 1;
}
/* tous les ensembles de taille k qui rendent compatible (après emin) */
static int enum_opt(const Fam *F, const int *pc, const int *cc, int k, Ihs *I, uint32_t *list, int maxl) {
  int n = 0;
  /* combinaisons de k parmi 24 qui touchent tous les noyaux connus */
  int idx[8]; for (int i = 0; i < k; i++) idx[i] = i;
  if (k == 0) { list[0] = 0; return 1; }
  while (1) {
    uint32_t H = 0; for (int i = 0; i < k; i++) H |= 1u << idx[i];
    if (hits_all(I, H) && sol(I, F, pc, cc, FULL & ~H)) { if (n < maxl) list[n] = H; n++; }
    int i = k - 1; while (i >= 0 && idx[i] == NCRIB - k + i) i--;
    if (i < 0) break;
    idx[i]++; for (int j = i + 1; j < k; j++) idx[j] = idx[j - 1] + 1;
  }
  return n;
}
#endif
