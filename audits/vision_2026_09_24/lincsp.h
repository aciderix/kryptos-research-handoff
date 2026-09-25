/* lincsp.h — solveur exact de contraintes linéaires mod 26 avec alphabet σ inconnu (toutes-différentes).
 * Variables 0..25 : σ(lettre) (toutes différentes). Variables 26.. : paramètres de clé (libres dans Z26).
 * Contrainte : Σ coef·var ≡ 0 (mod 26), coef ∈ {+1, -1} (un même var peut apparaître deux fois : coef ±2 ou 0).
 * Propagation : quand une seule variable reste libre et que son coefficient total est ±1, elle est fixée.
 * Symétries utilisées par l'appelant (optionnel) : valeurs pré-fixées.
 */
#ifndef LINCSP_H
#define LINCSP_H
#include "k4lib.h"

#ifndef LC_MAXV
#define LC_MAXV 160
#endif
#ifndef LC_MAXC
#define LC_MAXC 64
#endif
#ifndef LC_MAXT
#define LC_MAXT 8
#endif
typedef struct { int nv; int v[LC_MAXT]; int c[LC_MAXT]; } LCon;

static LCon lc_con[LC_MAXC]; static int lc_ncon, lc_nvar;
static int lc_val[LC_MAXV];
static uint32_t lc_used;
static int lc_vcon[LC_MAXV][LC_MAXC], lc_nvc[LC_MAXV];
static int lc_trail[LC_MAXV], lc_nt;
static long lc_nodes, lc_limit;
#ifdef LC_THREADPRIVATE /* une copie des variables du solveur par thread OpenMP (t30, 25/09) */
#pragma omp threadprivate(lc_con, lc_ncon, lc_nvar, lc_val, lc_used, lc_vcon, lc_nvc, lc_trail, lc_nt, lc_nodes, lc_limit)
#endif

static void lc_reset(int nvar) {
  lc_nvar = nvar; lc_ncon = 0; lc_used = 0; lc_nt = 0; lc_nodes = 0;
  for (int i = 0; i < nvar; i++) { lc_val[i] = -1; lc_nvc[i] = 0; }
}
/* ajoute Σ coef·var ≡ 0 ; les doublons sont fusionnés */
static void lc_add(int n, const int *vars, const int *coefs) {
  LCon *C = &lc_con[lc_ncon];
  C->nv = 0;
  for (int i = 0; i < n; i++) {
    int f = -1; for (int j = 0; j < C->nv; j++) if (C->v[j] == vars[i]) f = j;
    if (f >= 0) C->c[f] += coefs[i]; else { C->v[C->nv] = vars[i]; C->c[C->nv] = coefs[i]; C->nv++; }
  }
  /* retirer les coefficients nuls mod 26 */
  int m = 0; for (int j = 0; j < C->nv; j++) if (md(C->c[j]) != 0) { C->v[m] = C->v[j]; C->c[m] = md(C->c[j]); m++; }
  C->nv = m;
  for (int j = 0; j < C->nv; j++) lc_vcon[C->v[j]][lc_nvc[C->v[j]]++] = lc_ncon;
  lc_ncon++;
}
static int lc_assign(int x, int v);
static int lc_prop_con(int ci) {
  LCon *C = &lc_con[ci];
  int s = 0, fr = -1, nf = 0;
  for (int j = 0; j < C->nv; j++) { int x = C->v[j]; if (lc_val[x] < 0) { nf++; fr = j; } else s += C->c[j] * lc_val[x]; }
  if (nf == 0) return md(s) == 0;
  if (nf > 1) return 1;
  int c = md(C->c[fr]);
  if (c == 1) return lc_assign(C->v[fr], md(-s));
  if (c == 25) return lc_assign(C->v[fr], md(s));
  /* coefficient non inversible (±2, 13…) : vérifier qu'une solution existe au moins */
  for (int v = 0; v < 26; v++) if (md(c * v + s) == 0) return 1;
  return 0;
}
static int lc_assign(int x, int v) {
  if (lc_val[x] >= 0) return lc_val[x] == v;
  if (x < 26) { if (lc_used >> v & 1) return 0; lc_used |= 1u << v; }
  lc_val[x] = v; lc_trail[lc_nt++] = x;
  for (int j = 0; j < lc_nvc[x]; j++) if (!lc_prop_con(lc_vcon[x][j])) return 0;
  return 1;
}
static void lc_undo(int to) {
  while (lc_nt > to) { int x = lc_trail[--lc_nt]; if (x < 26) lc_used &= ~(1u << lc_val[x]); lc_val[x] = -1; }
}
/* renvoie 1 = compatible, 0 = impossible, -1 = limite de nœuds atteinte */
static int lc_search(void) {
  if (lc_limit && ++lc_nodes > lc_limit) return -1;
  int best = -1, bs = -1;
  for (int x = 0; x < lc_nvar; x++) if (lc_val[x] < 0 && lc_nvc[x] > 0) {
    int s = 0;
    for (int j = 0; j < lc_nvc[x]; j++) {
      LCon *C = &lc_con[lc_vcon[x][j]]; int f = 0;
      for (int k = 0; k < C->nv; k++) if (lc_val[C->v[k]] < 0) f++;
      s += 100 / f;
    }
    if (s > bs) { bs = s; best = x; }
  }
  if (best < 0) { for (int i = 0; i < lc_ncon; i++) if (!lc_prop_con(i)) return 0; return 1; }
  for (int v = 0; v < 26; v++) {
    if (best < 26 && (lc_used >> v & 1)) continue;
    int mark = lc_nt;
    if (lc_assign(best, v)) { int r = lc_search(); if (r) { if (r < 0) lc_undo(mark); return r; } }
    lc_undo(mark);
  }
  return 0;
}
/* à appeler après avoir posé les contraintes : propage tout puis cherche */
static int lc_solve(void) {
  for (int i = 0; i < lc_ncon; i++) if (!lc_prop_con(i)) return 0;
  return lc_search();
}
/* ajoute la contrainte de chiffrement pour la position crib t : clé = somme des variables kv[0..nk-1]
 * VIG : σC - σP - Σk = 0 ; BEAU : σC + σP - Σk = 0 ; VARB : σC - σP + Σk = 0 */
static void lc_add_enc(int p, int c, const int *kv, int nk, int mode) {
  int vars[LC_MAXT], co[LC_MAXT], n = 0;
  vars[n] = c; co[n++] = 1;
  vars[n] = p; co[n++] = mode == BEAU ? 1 : -1;
  for (int j = 0; j < nk; j++) { vars[n] = kv[j]; co[n++] = mode == VARB ? 1 : -1; }
  lc_add(n, vars, co);
}
#endif
