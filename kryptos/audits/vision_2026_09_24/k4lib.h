/* k4lib.h — noyau exact pour K4 (audit « vision », 24/09/2026)
 *
 * Tout ce qui suit travaille sous H1 : la lettre gravée i se déchiffre en la lettre claire i.
 * Deux moteurs exacts, sans score d'anglais :
 *   - eng_num  : clé NUMÉRIQUE connue aux 24 positions, alphabet σ INCONNU (26! couverts),
 *                même σ pour le clair et le chiffré (famille Quagmire III, système de K1-K2).
 *   - eng_let  : clé en LETTRES connues, lettre-clé lue dans le même σ inconnu
 *                (σ(C) = σ(P) + σ(K) ; exactement la convention de K1-K2 avec un alphabet quelconque).
 * Conventions : VIG  C = P + k ; BEAU C = k - P ; VARB C = P - k.
 */
#ifndef K4LIB_H
#define K4LIB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const char K4CT[] =
  "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
#define K4LEN 97
#define NCRIB 24
static int CRIBPOS[NCRIB];
static int CRIBPT[NCRIB];
static int CT[K4LEN];
static const char KA_STR[] = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int KA[26], KAINV[26];

enum { VIG = 0, BEAU = 1, VARB = 2 };
static const char *MODENAME[3] = {"VIG", "BEAU", "VARB"};

static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }

static void k4_init(void) {
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  int n = 0;
  for (int i = 0; i < 13; i++) { CRIBPOS[n] = 21 + i; CRIBPT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { CRIBPOS[n] = 63 + i; CRIBPT[n] = b[i] - 'A'; n++; }
  for (int i = 0; i < K4LEN; i++) CT[i] = K4CT[i] - 'A';
  for (int i = 0; i < 26; i++) { KA[i] = KA_STR[i] - 'A'; KAINV[KA[i]] = i; }
}

/* xorshift pour les témoins */
static uint64_t rng_s = 88172645463325252ULL;
static inline uint64_t rng(void) { rng_s ^= rng_s << 13; rng_s ^= rng_s >> 7; rng_s ^= rng_s << 17; return rng_s; }

/* ---------------------------------------------------------------------------------------------
 * eng_num : existe-t-il σ bijective Z26 telle que, pour chaque position t,
 *   VIG : σ(c_t) = σ(p_t) + k_t    BEAU : σ(c_t) = k_t - σ(p_t)    VARB : σ(c_t) = σ(p_t) - k_t ?
 * Méthode : composantes connexes ; chaque lettre v = s_v·x + o_v (s = ±1) relativement à la racine x ;
 * cycles => contrainte sur x ; puis placement injectif des composantes (retour arrière sur masques 26 bits).
 * --------------------------------------------------------------------------------------------- */
typedef struct { int a, b, k; } Edge; /* relation σ(b) = s·σ(a) + k, s = +1 (VIG/VARB) ou -1 (BEAU) */

static int en_nopt[26];
static uint32_t en_opt[26][26];
static int en_ncomp;

static int en_place(int ci, uint32_t used) {
  if (ci == en_ncomp) return 1;
  for (int j = 0; j < en_nopt[ci]; j++)
    if (!(en_opt[ci][j] & used) && en_place(ci + 1, used | en_opt[ci][j])) return 1;
  return 0;
}

/* p[], c[] : lettres (0..25) ; k[] : clé numérique ; n positions */
static int eng_num(const int *p, const int *c, const int *k, int n, int mode) {
  int adjn[26] = {0};
  int adj[26][64][3]; /* voisin, s, off : σ(nb) = s·σ(me) + off */
  int present[26] = {0};
  for (int t = 0; t < n; t++) {
    int a = p[t], b = c[t], s, off;
    if (mode == VIG) { s = 1; off = md(k[t]); }
    else if (mode == VARB) { s = 1; off = md(-k[t]); }
    else { s = -1; off = md(k[t]); }
    /* σ(b) = s σ(a) + off  ;  inverse : σ(a) = s σ(b) - s·off */
    adj[a][adjn[a]][0] = b; adj[a][adjn[a]][1] = s; adj[a][adjn[a]][2] = off; adjn[a]++;
    adj[b][adjn[b]][0] = a; adj[b][adjn[b]][1] = s; adj[b][adjn[b]][2] = md(-s * off); adjn[b]++;
    present[a] = present[b] = 1;
  }
  int sgn[26], ofs[26], seen[26] = {0};
  en_ncomp = 0;
  for (int r = 0; r < 26; r++) {
    if (!present[r] || seen[r]) continue;
    int comp[26], nc = 0, q = 0;
    seen[r] = 1; sgn[r] = 1; ofs[r] = 0; comp[nc++] = r;
    /* contraintes sur x : a·x ≡ b (mod 26) accumulées sous forme d'un masque de x admissibles */
    uint32_t xok = (1u << 26) - 1;
    while (q < nc) {
      int u = comp[q++];
      for (int j = 0; j < adjn[u]; j++) {
        int v = adj[u][j][0], s = adj[u][j][1], off = adj[u][j][2];
        int ns = s * sgn[u], no = md(s * ofs[u] + off);
        if (!seen[v]) { seen[v] = 1; sgn[v] = ns; ofs[v] = no; comp[nc++] = v; }
        else if (sgn[v] != ns || ofs[v] != no) {
          /* sgn[v] x + ofs[v] ≡ ns x + no */
          uint32_t m = 0;
          for (int x = 0; x < 26; x++) if (md(sgn[v] * x + ofs[v]) == md(ns * x + no)) m |= 1u << x;
          xok &= m;
          if (!xok) return 0;
        }
      }
    }
    int no = 0;
    for (int x = 0; x < 26; x++) {
      if (!(xok >> x & 1)) continue;
      uint32_t m = 0; int bad = 0;
      for (int i = 0; i < nc; i++) {
        int v = md(sgn[comp[i]] * x + ofs[comp[i]]);
        if (m >> v & 1) { bad = 1; break; }
        m |= 1u << v;
      }
      if (!bad) {
        int dup = 0; for (int j = 0; j < no; j++) if (en_opt[en_ncomp][j] == m) { dup = 1; break; }
        if (!dup) en_opt[en_ncomp][no++] = m;
      }
    }
    if (!no) return 0;
    en_nopt[en_ncomp++] = no;
  }
  /* ordonner les composantes par nombre d'options croissant */
  for (int i = 0; i < en_ncomp; i++)
    for (int j = i + 1; j < en_ncomp; j++)
      if (en_nopt[j] < en_nopt[i]) {
        int t = en_nopt[i]; en_nopt[i] = en_nopt[j]; en_nopt[j] = t;
        uint32_t tmp[26]; memcpy(tmp, en_opt[i], sizeof tmp); memcpy(en_opt[i], en_opt[j], sizeof tmp); memcpy(en_opt[j], tmp, sizeof tmp);
      }
  return en_place(0, 0);
}

/* ---------------------------------------------------------------------------------------------
 * eng_let : clé en lettres, même σ inconnu.  VIG : σc = σp + σk ; BEAU : σc = σk - σp ; VARB : σc = σp - σk.
 * CSP à retour arrière avec propagation (dès que deux lettres d'une contrainte sont fixées, la troisième l'est).
 * Symétrie : σ -> u·σ (u inversible mod 26) préserve les trois équations ; la 1re lettre branchée
 * ne prend donc que les valeurs {0, 1, 2, 13}.
 * --------------------------------------------------------------------------------------------- */
typedef struct { int c, p, k; } Tri;
static Tri el_con[64]; static int el_n, el_mode;
static int el_sig[26]; static uint32_t el_used;
static int el_lcon[26][64], el_nl[26];
static int el_trail[32], el_nt;
static long el_nodes, el_limit = 0; static int el_abort;

static inline int el_solve3(int which, int vc, int vp, int vk) {
  /* renvoie la valeur manquante (which : 0=c,1=p,2=k) */
  switch (el_mode) {
    case VIG: return which == 0 ? md(vp + vk) : which == 1 ? md(vc - vk) : md(vc - vp);
    case BEAU: return which == 0 ? md(vk - vp) : which == 1 ? md(vk - vc) : md(vc + vp);
    default: return which == 0 ? md(vp - vk) : which == 1 ? md(vc + vk) : md(vp - vc);
  }
}
static inline int el_check(int vc, int vp, int vk) {
  switch (el_mode) {
    case VIG: return vc == md(vp + vk);
    case BEAU: return vc == md(vk - vp);
    default: return vc == md(vp - vk);
  }
}
static int el_assign(int L, int v);
static int el_prop(int L) {
  for (int j = 0; j < el_nl[L]; j++) {
    Tri *t = &el_con[el_lcon[L][j]];
    int sc = el_sig[t->c], sp = el_sig[t->p], sk = el_sig[t->k];
    int un = (sc < 0) + (sp < 0) + (sk < 0);
    if (un == 0) { if (!el_check(sc, sp, sk)) return 0; continue; }
    if (un != 1) continue;
    int which = sc < 0 ? 0 : sp < 0 ? 1 : 2;
    int L2 = which == 0 ? t->c : which == 1 ? t->p : t->k;
    /* la lettre manquante ne doit apparaître qu'une fois dans le triplet */
    int occ = (t->c == L2) + (t->p == L2) + (t->k == L2);
    if (occ != 1) continue;
    int v = el_solve3(which, sc, sp, sk);
    if (!el_assign(L2, v)) return 0;
  }
  return 1;
}
static int el_assign(int L, int v) {
  if (el_sig[L] >= 0) return el_sig[L] == v;
  if (el_used >> v & 1) return 0;
  el_sig[L] = v; el_used |= 1u << v; el_trail[el_nt++] = L;
  return el_prop(L);
}
static void el_undo(int to) {
  while (el_nt > to) { int L = el_trail[--el_nt]; el_used &= ~(1u << el_sig[L]); el_sig[L] = -1; }
}
static int el_search(int first) {
  el_nodes++;
  if (el_limit && el_nodes > el_limit) { el_abort = 1; return 0; }
  /* choisir la lettre non fixée la plus contrainte */
  int best = -1, bs = -1;
  for (int L = 0; L < 26; L++) if (el_sig[L] < 0 && el_nl[L] > 0) {
    int s = 0;
    for (int j = 0; j < el_nl[L]; j++) {
      Tri *t = &el_con[el_lcon[L][j]];
      s += 1 + (el_sig[t->c] >= 0) + (el_sig[t->p] >= 0) + (el_sig[t->k] >= 0);
    }
    if (s > bs) { bs = s; best = L; }
  }
  if (best < 0) {
    /* toutes les lettres contraintes sont fixées : vérifier toutes les contraintes */
    for (int i = 0; i < el_n; i++) if (!el_check(el_sig[el_con[i].c], el_sig[el_con[i].p], el_sig[el_con[i].k])) return 0;
    return 1;
  }
  static const int firstvals[4] = {0, 1, 2, 13};
  int nv = first ? 4 : 26;
  for (int ii = 0; ii < nv; ii++) {
    int v = first ? firstvals[ii] : ii;
    if (el_used >> v & 1) continue;
    int mark = el_nt;
    if (el_assign(best, v) && el_search(0)) return 1;
    el_undo(mark);
  }
  return 0;
}
/* p, c, k : lettres aux n positions */
static int eng_let(const int *p, const int *c, const int *k, int n, int mode) {
  el_mode = mode; el_n = n; el_used = 0; el_nt = 0; el_nodes = 0; el_abort = 0;
  for (int L = 0; L < 26; L++) { el_sig[L] = -1; el_nl[L] = 0; }
  for (int t = 0; t < n; t++) {
    el_con[t].c = c[t]; el_con[t].p = p[t]; el_con[t].k = k[t];
    int ls[3] = {c[t], p[t], k[t]};
    for (int a = 0; a < 3; a++) {
      int dup = 0; for (int b = 0; b < a; b++) if (ls[b] == ls[a]) dup = 1;
      if (!dup) el_lcon[ls[a]][el_nl[ls[a]]++] = t;
    }
  }
  /* pré-test : C = P impose k ≡ 0 (VIG/VARB) : cas traité naturellement par la recherche */
  int r = el_search(1);
  return el_abort ? -1 : r;
}

/* score en alphabets fixés : combien des 24 lettres des cribs sont reproduites
 * aP, aC : alphabets (tableaux lettre->rang) ; kv : valeur numérique de la clé */
static inline int fixed_pred(int pt, int kv, int mode, const int *rankP, const int *alphC) {
  int x = rankP[pt];
  int y = mode == VIG ? x + kv : mode == BEAU ? kv - x : x - kv;
  return alphC[md(y)];
}

#endif
