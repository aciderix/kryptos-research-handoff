/* t20_erreur_familles.c — les éliminations T11, T11b et T16 résistent-elles à UNE erreur de chiffrement ?
 * (audit « vision », 24/09/2026 ; suite de T19)
 *
 * Motif : le petit fragment « Covert Operations » de Sanborn (97 lettres, méthode de K1, base 7 §9.4) a 4 erreurs sur 97.
 * Deux sont un GLISSEMENT DE CLÉ (la lettre de clé suivante), une est une lettre claire RECOPIÉE sans chiffrement.
 *
 * Familles (clé en i = variable de clé + constante, alphabet σ QUELCONQUE, VIG / BEAU / VARB) :
 *   G0  clé périodique p = 1..26 (témoin de méthode : le registre la dit éliminée même avec une erreur pour p ≤ 12) ;
 *   G1  T11 : disque tourné par bloc de 7, phase φ, motif s·j (s = 0..25) ou KRYPTOS (A–Z), endroit/envers ;
 *   G2  T11b : bloc de n = 5..30 (n ≠ 7), motif s·j ;
 *   G3  T16 : mot-clé de longueur L = 1..7 écrit en lignes de 7 et relu par colonnes (7! ordres), N = 97/98, deux sens.
 * Modèles d'erreur, sur une seule position de crib e :
 *   A  erreur quelconque : la contrainte en e est retirée (24 variantes) ;
 *   B  erreur « à la Sanborn » :
 *      copie : le clair a été recopié tel quel, donc chiffré = clair en e ; seules les positions où c'est le cas sont
 *              candidates (pour K4 : 32, S → S, et 73, K → K) ; la contrainte en e est retirée ;
 *      glissement : la clé utilisée en e est celle de e + 1 ou de e − 1 (48 variantes).
 * Un cas est « compatible avec au plus une erreur » si le cas exact ou l'une des variantes l'est.
 * Déduplication : deux cas qui donnent la même clé (à renommage près) aux positions des cribs et à leurs voisines sont
 * identiques ; on ne résout qu'une fois.
 * Témoins : cas tirés au hasard dans la famille, chiffré aléatoire aux 24 positions ; moyenne × nombre de cas.
 * Usage : t20 groupe [tranche ntranches [ntémoins [x]]]   (groupe 0..3 ; « x » : témoins seuls, sans K4).
 * Compilé avec -DDIST : t20 groupe graine 1 R → nombre de cas compatibles pour R chiffrés aléatoires (distribution).
 * Compilé avec -DCOUNTONLY : nombre de cas distincts après déduplication.
 * Résultats : results_t20_g0.txt … results_t20_g3.txt.
 */
#include "lincsp.h"

#define ONE 26
#define NB  40 /* variables de clé au plus, après renommage */
static int P24[NCRIB], C24[NCRIB];
static int KV[K4LEN + 2], KC[K4LEN + 2]; /* clé en i : variable KV[i] (≥ 0) + constante KC[i] */
static int EPOS[NCRIB];

/* résout avec : drop = indice de crib retiré (−1 : aucun) ; slip_t / slip_d : crib dont la clé est prise en e + d */
static int solve(int mode, int drop, int slip_t, int slip_d) {
  lc_reset(27 + NB); lc_limit = 2000000;
  for (int t = 0; t < NCRIB; t++) {
    if (t == drop) continue;
    int kp = EPOS[t] + (t == slip_t ? slip_d : 0);
    int sg = mode == VARB ? 1 : -1;
    int vars[4] = {C24[t], P24[t], 27 + KV[kp], ONE}, co[4] = {1, mode == BEAU ? 1 : -1, sg, sg * KC[kp]};
    lc_add(4, vars, co);
  }
  if (!lc_assign(ONE, 1)) return 0;
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}
/* évalue un cas : bit 0 = exact ; bit 1 = au plus une erreur quelconque ; bit 2 = au plus une erreur « Sanborn » ;
 * bit 3 = non tranché quelque part. resA / resB : bitmask des positions de crib qui sauvent (si demandé) */
static int evaluate(int mode, uint32_t *resA, uint32_t *resB, int full) {
  int out = 0, r = solve(mode, -1, -1, 0);
  if (r < 0) out |= 8;
  if (r > 0) return 7 | out;
  uint32_t ma = 0, mb = 0;
  for (int pass = 0; pass < 2; pass++)          /* A : retrait ; d'abord les positions « copie » (chiffré = clair) */
    for (int t = 0; t < NCRIB; t++) {
      if ((C24[t] == P24[t]) != (pass == 0)) continue;
      if (!full && ma && (mb || pass == 1)) break;
      int x = solve(mode, t, -1, 0);
      if (x < 0) out |= 8;
      if (x > 0) { ma |= 1u << t; if (pass == 0) mb |= 1u << t; }
    }
  if (ma) out |= 2;
  if (mb) out |= 4;
  if (!(out & 4) || full) {                     /* B : glissement ±1 */
    for (int t = 0; t < NCRIB && (full || !(out & 4)); t++) for (int d = -1; d <= 1; d += 2) {
      int x = solve(mode, -1, t, d);
      if (x < 0) out |= 8;
      if (x > 0) { mb |= 1u << t; out |= 4; if (!full) break; }
    }
  }
  if (out & 4) out |= 2; /* une erreur « Sanborn » est une erreur */
  if (resA) *resA = ma; if (resB) *resB = mb;
  return out;
}

/* ---------- familles : chaque cas remplit KV/KC sur 0..97 ---------- */
static int g_mode;
static int pis[5040][7];
static void set_case(int g, long c) {
  static int kz[7] = {10, 17, 24, 15, 19, 14, 18};
  if (g == 0) { int p = c % 26 + 1; g_mode = c / 26; for (int i = 0; i <= K4LEN; i++) { KV[i] = i % p; KC[i] = 0; } return; }
  if (g == 1) {
    int q = c % 28, phi = (c / 28) % 7; g_mode = c / (28 * 7);
    for (int i = 0; i <= K4LEN; i++) { int r = (i - phi + 7) / 7, j = ((i - phi) % 7 + 7) % 7;
      KV[i] = r; KC[i] = q < 26 ? q * j : (q == 26 ? kz[j] : kz[6 - j]); }
    return;
  }
  if (g == 2) {
    long k = c; int n;
    for (n = 5; n <= 30; n++) { if (n == 7) continue; long sz = 3L * n * 26; if (k < sz) break; k -= sz; }
    int s = k % 26, phi = (k / 26) % n; g_mode = k / (26 * n);
    for (int i = 0; i <= K4LEN; i++) { int r = (i - phi + n) / n, j = ((i - phi) % n + n) % n; KV[i] = r; KC[i] = s * j % 26; }
    return;
  }
  /* g == 3 : T16 */
  long k = c; int L = k % 7 + 1; k /= 7; g_mode = k % 3; k /= 3; int inv = k % 2; k /= 2; int N = 97 + k % 2; k /= 2;
  const int *pi = pis[k];
  int h = (N + 6) / 7, full = N % 7 == 0 ? 7 : N % 7, rd[98], t = 0;
  if (!inv) { for (int q = 0; q < 7; q++) { int col = pi[q], hc = col < full ? h : h - 1; for (int r = 0; r < hc; r++) rd[t++] = r * 7 + col; } }
  else { int rank[98]; for (int q = 0; q < 7; q++) { int col = pi[q], hc = col < full ? h : h - 1; for (int r = 0; r < hc; r++) rank[r * 7 + col] = t++; }
         for (int m = 0; m < N; m++) rd[m] = rank[m]; }
  for (int i = 0; i <= K4LEN; i++) { KV[i] = i < N ? rd[i] % L : 0; KC[i] = 0; }
}
/* renommage des variables aux positions utiles ; renvoie une empreinte 64 bits */
static uint64_t canon(void) {
  int map[128]; for (int i = 0; i < 128; i++) map[i] = -1; int nx = 0; uint64_t h = 1469598103934665603ULL ^ g_mode;
  for (int t = 0; t < NCRIB; t++) for (int d = -1; d <= 1; d++) { int i = EPOS[t] + d;
    if (map[KV[i]] < 0) map[KV[i]] = nx++;
    h = (h ^ (uint64_t)(map[KV[i]] * 32 + KC[i] % 26)) * 1099511628211ULL; }
  /* réécrit KV avec le renommage (pour rester sous NB variables) */
  for (int i = 0; i <= K4LEN; i++) KV[i] = map[KV[i]] >= 0 ? map[KV[i]] : NB - 1;
  return h;
}
/* table de hachage empreinte -> résultat */
#define HS (1 << 22)
static uint64_t hkey[HS]; static int hval[HS];
static int *hget(uint64_t k) { uint64_t i = k & (HS - 1); while (hkey[i] && hkey[i] != k) i = (i + 1) & (HS - 1); if (!hkey[i]) { hkey[i] = k; hval[i] = -1; } return &hval[i]; }

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int g = argc > 1 ? atoi(argv[1]) : 1, slice = argc > 3 ? atoi(argv[2]) : 0, nsl = argc > 3 ? atoi(argv[3]) : 1;
  rng_s ^= 0x9E3779B97F4A7C15ULL * (slice + 1 + 100 * g);
  for (int t = 0; t < NCRIB; t++) EPOS[t] = CRIBPOS[t];
  { int pi[7] = {0, 1, 2, 3, 4, 5, 6}, n = 0;
    do { memcpy(pis[n++], pi, sizeof pi); } while (({ int i = 5; while (i >= 0 && pi[i] > pi[i + 1]) i--; int ok = i >= 0;
      if (ok) { int j = 6; while (pi[j] < pi[i]) j--; int x = pi[i]; pi[i] = pi[j]; pi[j] = x;
      for (int l = i + 1, r = 6; l < r; l++, r--) { x = pi[l]; pi[l] = pi[r]; pi[r] = x; } } ok; })); }
  long ncase[4] = {78, 3 * 7 * 28, 0, 5040L * 2 * 2 * 3 * 7};
  for (int n = 5; n <= 30; n++) if (n != 7) ncase[2] += 3L * n * 26;
  const char *GN[4] = {"G0 périodique p=1..26", "G1 T11 bloc de 7", "G2 T11b blocs 5..30", "G3 T16 clé transposée L≤7"};

#ifdef DIST
  /* distribution, sur des chiffrés aléatoires, du NOMBRE de cas compatibles (les cas d'une famille sont corrélés) */
  { static long rep[1 << 20], mult[1 << 20]; long nu = 0;
    for (long c = 0; c < ncase[g]; c++) { set_case(g, c); int *v = hget(canon()); if (*v < 0) { *v = nu; rep[nu] = c; mult[nu++] = 0; } mult[*v]++; }
    int R = argc > 4 ? atoi(argv[4]) : 50;
    for (int z = 0; z < R; z++) {
      for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = rng() % 26; }
      long e = 0, a = 0, b = 0;
      for (long u = 0; u < nu; u++) { set_case(g, rep[u]); canon(); int r = evaluate(g_mode, NULL, NULL, 0);
        e += mult[u] * (r & 1); a += mult[u] * ((r >> 1) & 1); b += mult[u] * ((r >> 2) & 1); }
      printf("témoin %d : exact %ld, A %ld, B %ld\n", z, e, a, b);
    }
    return 0; }
#endif
#ifdef COUNTONLY
  { long u = 0; for (long c = 0; c < ncase[g]; c++) { set_case(g, c); int *v = hget(canon()); if (*v < 0) { *v = 0; u++; } }
    printf("%s : %ld cas, %ld distincts\n", GN[g], ncase[g], u); return 0; }
#endif
  /* contrôle positif (tranche 0) : faux K4 fabriqué dans la famille, avec UNE erreur de chaque type */
  if (slice == 0) {
    for (int typ = 0; typ < 3; typ++) {
      long c = (long)(rng() % ncase[g]); set_case(g, c); canon();
      int s[26], inv[26]; for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int x = s[i]; s[i] = s[j]; s[j] = x; }
      for (int i = 0; i < 26; i++) inv[s[i]] = i; int kv[NB]; for (int j = 0; j < NB; j++) kv[j] = rng() % 26;
      int bad = rng() % NCRIB;
      for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; int kp = EPOS[t]; if (t == bad && typ == 2) kp += 1;
        int k = md(kv[KV[kp]] + KC[kp]);
        C24[t] = g_mode == VIG ? inv[md(s[P24[t]] + k)] : g_mode == BEAU ? inv[md(k - s[P24[t]])] : inv[md(s[P24[t]] - k)];
        if (t == bad && typ == 0) C24[t] = (C24[t] + 1 + rng() % 25) % 26;   /* lettre fausse quelconque */
        if (t == bad && typ == 1) C24[t] = P24[t];                           /* clair recopié */ }
      uint32_t a, b; int r = evaluate(g_mode, &a, &b, 0);
      printf("contrôle positif %s, erreur %s en crib %d : exact %d, ≤1 erreur quelconque %d, ≤1 erreur Sanborn %d\n", GN[g],
             typ == 0 ? "quelconque" : typ == 1 ? "copie" : "glissement", bad, r & 1, (r >> 1) & 1, (r >> 2) & 1);
    }
  }
  /* K4 */
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  long tot = 0, uniq = 0, nE = 0, nA = 0, nB = 0, nU = 0; long histA[NCRIB] = {0}, histB[NCRIB] = {0};
  for (long c = slice; c < ncase[g] && argc <= 5; c += nsl) {  /* argv[5] présent : témoins seulement */
    set_case(g, c); uint64_t k = canon(); int *v = hget(k); tot++;
    if (*v < 0) { uint32_t a = 0, b = 0; *v = evaluate(g_mode, &a, &b, 1); uniq++;
      for (int t = 0; t < NCRIB; t++) { if (a >> t & 1) histA[t]++; if (b >> t & 1) histB[t]++; }
      if ((*v & 1) || (*v & 4) || g < 2) { if (*v & 7) printf("  cas %ld (%s) : exact %d, A %d, B %d ; sauvé par A en", c, MODENAME[g_mode], *v & 1, (*v >> 1) & 1, (*v >> 2) & 1);
        if (*v & 7) { for (int t = 0; t < NCRIB; t++) if (a >> t & 1) printf(" %d", EPOS[t]); printf(" ; par B en"); for (int t = 0; t < NCRIB; t++) if (b >> t & 1) printf(" %d", EPOS[t]); printf("\n"); } } }
    if ((*v & 8) && g == 0) printf("  cas %ld (%s, p=%ld) : non tranché (limite de nœuds) ; exact %d, A %d, B %d\n", c, MODENAME[g_mode], c % 26 + 1, *v & 1, (*v >> 1) & 1, (*v >> 2) & 1);
    nE += *v & 1; nA += (*v >> 1) & 1; nB += (*v >> 2) & 1; nU += (*v >> 3) & 1;
  }
  printf("%s, tranche %d/%d : %ld cas (%ld distincts) ; K4 compatible : exact %ld, ≤1 erreur quelconque %ld, ≤1 erreur Sanborn %ld ; non tranchés %ld\n",
         GN[g], slice, nsl, tot, uniq, nE, nA, nB, nU);
  printf("  positions de crib qui sauvent K4 (cas distincts) — A :"); for (int t = 0; t < NCRIB; t++) if (histA[t]) printf(" %d:%ld", EPOS[t], histA[t]);
  printf("\n  — B :"); for (int t = 0; t < NCRIB; t++) if (histB[t]) printf(" %d:%ld", EPOS[t], histB[t]); printf("\n");
  if (argc > 5) tot = (ncase[g] - slice + nsl - 1) / nsl;
  /* témoins : cas tirés au hasard (parmi ceux de la tranche), chiffré aléatoire */
  int NN = argc > 4 ? atoi(argv[4]) : 400; long zE = 0, zA = 0, zB = 0;
  for (int z = 0; z < NN; z++) {
    long c = slice + nsl * (long)(rng() % ((ncase[g] - slice + nsl - 1) / nsl)); set_case(g, c); canon();
    for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
    int r = evaluate(g_mode, NULL, NULL, 0); zE += r & 1; zA += (r >> 1) & 1; zB += (r >> 2) & 1;
  }
  printf("  témoins (%d tirages) : taux exact %.4f, ≤1 quelconque %.4f, ≤1 Sanborn %.4f ; attendu sur %ld cas : %.1f / %.1f / %.1f\n",
         NN, (double)zE / NN, (double)zA / NN, (double)zB / NN, tot, tot * (double)zE / NN, tot * (double)zA / NN, tot * (double)zB / NN);
  return 0;
}
