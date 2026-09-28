/* imx.h — moteur exact « alphabet(s) inconnu(s) + clé structurée », avec contraintes optionnelles.
 * (audit « erreurs multiples », 24/09/2026)
 *
 * Variables : S1[0..25] (alphabet σ1, toutes différentes), S2[26..51] (alphabet σ2, toutes différentes,
 * utilisé seulement en Quagmire IV), ONE = 52 (fixée à 1), puis variables de clé 53.. (libres dans Z26).
 * Contrainte : Σ coef·var ≡ 0 (mod 26).
 * Types (côté clair / côté chiffré) :
 *   Q3 : σ1 / σ1   (K1–K2 avec un alphabet quelconque)
 *   Q4 : σ1 / σ2   (deux alphabets indépendants)
 *   Q2 : A–Z / σ1  (clair droit, chiffré mélangé ; Cyrillic Projector)
 *   Q1 : σ1 / A–Z  (clair mélangé, chiffré droit)
 * Conventions : VIG  X(c) = Y(p) + k ; BEAU X(c) = k − Y(p) ; VARB X(c) = Y(p) − k.
 */
#ifndef IMX_H
#define IMX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const char K4CT[] =
  "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
#define K4LEN 97
#define NCRIB 24
static int CRIBPOS[NCRIB], CRIBPT[NCRIB], CT[K4LEN];
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static void k4_init(void) {
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { CRIBPOS[n] = 21 + i; CRIBPT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { CRIBPOS[n] = 63 + i; CRIBPT[n] = b[i] - 'A'; n++; }
  for (int i = 0; i < K4LEN; i++) CT[i] = K4CT[i] - 'A';
}
static uint64_t rng_s = 0x243F6A8885A308D3ULL;
static inline uint64_t rng(void) { rng_s ^= rng_s << 13; rng_s ^= rng_s >> 7; rng_s ^= rng_s << 17; return rng_s; }

enum { VIG = 0, BEAU = 1, VARB = 2 };
enum { Q3 = 0, Q4 = 1, Q2 = 2, Q1 = 3 };
static const char *MODEN[3] = {"VIG", "BEAU", "VARB"};
static const char *QN[4] = {"Q3", "Q4", "Q2", "Q1"};

#define ONE 52
#define KV0 53
#define MAXV 200
#define MAXC 40
#define MAXT 10
typedef struct { int nv; int v[MAXT]; int c[MAXT]; } Con;
typedef struct {
  Con con[MAXC]; int ncon, nvar;
  int val[MAXV]; uint32_t used1, used2;
  int vcon[MAXV][MAXC], nvc[MAXV];
  int trail[MAXV], nt;
  long nodes, limit;
} Csp;

static void csp_reset(Csp *S, int nvar) {
  S->nvar = nvar; S->ncon = 0; S->used1 = S->used2 = 0; S->nt = 0; S->nodes = 0;
  for (int i = 0; i < nvar; i++) { S->val[i] = -1; S->nvc[i] = 0; }
}
static void csp_add(Csp *S, int n, const int *vars, const int *coefs) {
  Con *C = &S->con[S->ncon]; C->nv = 0;
  for (int i = 0; i < n; i++) {
    int f = -1; for (int j = 0; j < C->nv; j++) if (C->v[j] == vars[i]) f = j;
    if (f >= 0) C->c[f] += coefs[i]; else { C->v[C->nv] = vars[i]; C->c[C->nv] = coefs[i]; C->nv++; }
  }
  int m = 0; for (int j = 0; j < C->nv; j++) if (md(C->c[j])) { C->v[m] = C->v[j]; C->c[m] = md(C->c[j]); m++; }
  C->nv = m;
  for (int j = 0; j < C->nv; j++) S->vcon[C->v[j]][S->nvc[C->v[j]]++] = S->ncon;
  S->ncon++;
}
static int csp_assign(Csp *S, int x, int v);
static int csp_prop(Csp *S, int ci) {
  Con *C = &S->con[ci]; int s = 0, fr = -1, nf = 0;
  for (int j = 0; j < C->nv; j++) { int x = C->v[j]; if (S->val[x] < 0) { nf++; fr = j; } else s += C->c[j] * S->val[x]; }
  if (nf == 0) return md(s) == 0;
  if (nf > 1) return 1;
  int c = C->c[fr];
  if (c == 1) return csp_assign(S, C->v[fr], md(-s));
  if (c == 25) return csp_assign(S, C->v[fr], md(s));
  for (int v = 0; v < 26; v++) if (md(c * v + s) == 0) return 1;
  return 0;
}
static int csp_assign(Csp *S, int x, int v) {
  if (S->val[x] >= 0) return S->val[x] == v;
  if (x < 26) { if (S->used1 >> v & 1) return 0; S->used1 |= 1u << v; }
  else if (x < 52) { if (S->used2 >> v & 1) return 0; S->used2 |= 1u << v; }
  S->val[x] = v; S->trail[S->nt++] = x;
  for (int j = 0; j < S->nvc[x]; j++) if (!csp_prop(S, S->vcon[x][j])) return 0;
  return 1;
}
static void csp_undo(Csp *S, int to) {
  while (S->nt > to) { int x = S->trail[--S->nt];
    if (x < 26) S->used1 &= ~(1u << S->val[x]); else if (x < 52) S->used2 &= ~(1u << S->val[x]);
    S->val[x] = -1; }
}
static int csp_search(Csp *S) {
  if (S->limit && ++S->nodes > S->limit) return -1;
  int best = -1, bs = -1;
  for (int x = 0; x < S->nvar; x++) if (S->val[x] < 0 && S->nvc[x] > 0) {
    int s = 0;
    for (int j = 0; j < S->nvc[x]; j++) { Con *C = &S->con[S->vcon[x][j]]; int f = 0;
      for (int k = 0; k < C->nv; k++) if (S->val[C->v[k]] < 0) f++;
      s += 120 / f; }
    if (s > bs) { bs = s; best = x; }
  }
  if (best < 0) { for (int i = 0; i < S->ncon; i++) if (!csp_prop(S, i)) return 0; return 1; }
  uint32_t used = best < 26 ? S->used1 : best < 52 ? S->used2 : 0;
  for (int v = 0; v < 26; v++) {
    if (used >> v & 1) continue;
    int mark = S->nt;
    if (csp_assign(S, best, v)) { int r = csp_search(S); if (r) { if (r < 0) csp_undo(S, mark); return r; } }
    csp_undo(S, mark);
  }
  return 0;
}

/* ---- description d'une famille : clé en i = Σ_j kc[i][j]·K[kv[i][j]] + kk[i] ---- */
typedef struct {
  int nk[K4LEN + 2]; int kv[K4LEN + 2][3]; int kc[K4LEN + 2][3]; int kk[K4LEN + 2];
  int nkey;   /* nombre de variables de clé */
  int q, mode;
} Fam;

/* contrainte de chiffrement de la position i (clair p, chiffré c) */
static void add_enc(Csp *S, const Fam *F, int i, int p, int c) {
  int vars[MAXT], co[MAXT], n = 0, cst = 0;
  /* côté chiffré */
  if (F->q == Q3 || F->q == Q2) { vars[n] = c; co[n++] = 1; }
  else if (F->q == Q4) { vars[n] = 26 + c; co[n++] = 1; }
  else cst += c;                      /* Q1 : chiffré droit */
  /* côté clair */
  int sp = F->mode == BEAU ? 1 : -1;   /* VIG/VARB : −Y(p) ; BEAU : +Y(p) */
  if (F->q == Q3 || F->q == Q4 || F->q == Q1) { vars[n] = p; co[n++] = sp; }
  else cst += sp * p;                 /* Q2 : clair droit */
  int sk = F->mode == VARB ? 1 : -1;   /* VIG/BEAU : −k ; VARB : +k */
  for (int j = 0; j < F->nk[i]; j++) { vars[n] = KV0 + F->kv[i][j]; co[n++] = sk * F->kc[i][j]; }
  cst += sk * F->kk[i];
  if (md(cst)) { vars[n] = ONE; co[n++] = md(cst); }
  csp_add(S, n, vars, co);
}
/* résout avec les cribs dont le bit est à 1 dans mask ; pc[] / cc[] : clair et chiffré aux 24 positions */
static long g_limit = 3000000;
static int solve_mask(const Fam *F, const int *pc, const int *cc, uint32_t mask) {
  Csp S; csp_reset(&S, KV0 + F->nkey); S.limit = g_limit;
  for (int t = 0; t < NCRIB; t++) if (mask >> t & 1) add_enc(&S, F, CRIBPOS[t], pc[t], cc[t]);
  if (!csp_assign(&S, ONE, 1)) return 0;
  /* brisure de symétrie : un décalage constant d'un alphabet est absorbé par la clé (chaque position a une variable
   * de clé de coefficient ±1) ; on fixe donc une lettre de σ1 et, en Q4, une lettre de σ2 */
  { int L = -1, M = -1;
    for (int t = 0; t < NCRIB; t++) if (mask >> t & 1) { if (L < 0) L = F->q == Q2 ? cc[t] : pc[t]; if (M < 0) M = cc[t]; }
    if (F->q != Q2 || 1) { if (L >= 0 && !csp_assign(&S, L, 0)) return 0; }
    if (F->q == Q4 && M >= 0 && !csp_assign(&S, 26 + M, 0)) return 0; }
  for (int i = 0; i < S.ncon; i++) if (!csp_prop(&S, i)) return 0;
  return csp_search(&S);
}
#endif
