/* t19_une_erreur.c — l'élimination T18 résiste-t-elle à UNE erreur de chiffrement ? (audit « vision », 24/09/2026)
 *
 * Motif : le petit fragment « Covert Operations » en anglais (atelier de Sanborn, 2005, base 7 §9.3) a 97 lettres,
 * la méthode de K1, et 4 erreurs de chiffrement. Sanborn chiffre à la main avec environ 4 % d'erreurs.
 * En autoclé sur le clair, une erreur sur le chiffré en e ne se propage pas (la clé vient du clair) : elle rend
 * libre l'unique équation de chaîne où c_e apparaît. Une erreur = retirer une des équations de T18.
 * On teste, pour L = 7 (sens arrière, VIG/BEAU/VARB) : K4 avec 0 ou 1 équation retirée ; témoins aléatoires idem.
 */
#define LC_MAXT 40
#include "lincsp.h"

static int P97[K4LEN];
typedef struct { int n; int v[64]; int c[64]; } Eq;
static Eq EQ[64]; static int NEQ;

static void mk_chain(const int *ct, int a, int b, int L, int mode) {
  Eq *E = &EQ[NEQ++]; int m = (b - a) / L; E->n = 0;
  E->v[E->n] = P97[b]; E->c[E->n++] = 1;
  if (mode == VIG) {
    for (int t = 1; t <= m; t++) { E->v[E->n] = ct[a + t * L]; E->c[E->n++] = ((m - t) % 2 == 0) ? -1 : 1; }
    E->v[E->n] = P97[a]; E->c[E->n++] = (m % 2 == 0) ? -1 : 1;
  } else if (mode == BEAU) {
    E->v[E->n] = P97[a]; E->c[E->n++] = -1;
    for (int t = 1; t <= m; t++) { E->v[E->n] = ct[a + t * L]; E->c[E->n++] = 1; }
  } else {
    E->v[E->n] = P97[a]; E->c[E->n++] = -1;
    for (int t = 1; t <= m; t++) { E->v[E->n] = ct[a + t * L]; E->c[E->n++] = -1; }
  }
}
static void build(const int *ct, int L, int mode) {
  for (int i = 0; i < K4LEN; i++) P97[i] = -1;
  for (int q = 0; q < NCRIB; q++) P97[CRIBPOS[q]] = CRIBPT[q];
  NEQ = 0;
  for (int b = 0; b < K4LEN; b++) {
    if (P97[b] < 0) continue;
    for (int a = b - L; a >= 0; a -= L) if (P97[a] >= 0) { mk_chain(ct, a, b, L, mode); break; }
  }
}
static int solve_drop(int drop) {
  lc_reset(26); lc_limit = 5000000;
  for (int e = 0; e < NEQ; e++) if (e != drop) lc_add(EQ[e].n, EQ[e].v, EQ[e].c);
  return lc_solve();
}
/* renvoie le nombre d'équations dont le retrait rend le système compatible ; *exact = compatibilité sans retrait */
static int survivors(const int *ct, int L, int mode, int *exact, int *list) {
  build(ct, L, mode);
  *exact = solve_drop(-1);
  int ns = 0;
  for (int d = 0; d < NEQ; d++) if (solve_drop(d) != 0) list[ns++] = d;
  return ns;
}

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int L = 7, list[64], rnd[K4LEN];
  for (int mode = 0; mode < 3; mode++) {
    int ex, ns = survivors(CT, L, mode, &ex, list);
    printf("K4 %s L=7 : %d équations ; exact %s ; avec une erreur : %d retrait(s) sur %d rendent K4 compatible",
           MODENAME[mode], NEQ, ex > 0 ? "COMPATIBLE" : (ex < 0 ? "NON TRANCHÉ" : "IMPOSSIBLE"), ns, NEQ);
    printf("\n");
    /* détail : quelle position de crib termine chaque équation retirable */
    build(CT, L, mode);
    int k = 0;
    for (int b = 0; b < K4LEN; b++) { if (P97[b] < 0) continue; int a0 = -1;
      for (int a = b - L; a >= 0; a -= L) if (P97[a] >= 0) { a0 = a; break; }
      if (a0 >= 0) { for (int j = 0; j < ns; j++) if (list[j] == k) {
          printf("   retrait possible : chaîne %d → %d ; l'erreur serait sur le chiffré en", a0, b);
          for (int t = a0 + L; t <= b; t += L) printf(" %d", t); printf("\n"); }
        k++; } }
    /* témoins */
    int NN = 300, p0 = 0, p1 = 0;
    for (int z = 0; z < NN; z++) { for (int i = 0; i < K4LEN; i++) rnd[i] = rng() % 26;
      int e2, n2 = survivors(rnd, L, mode, &e2, list); p0 += e2 > 0; p1 += (e2 > 0 || n2 > 0); }
    printf("   témoins aléatoires (%d) : compatibles exacts %d ; compatibles avec au plus une erreur %d\n", NN, p0, p1);
  }
  return 0;
}
