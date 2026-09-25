/* k4x.c — moteur unique, C + OpenMP (audit « moteur », 25/09/2026).
 * Pour chaque alphabet candidat (fichier binaire de genalpha, 26 octets ; l'inverse est aussi essayé quand il compte),
 * 5 types (Q3 σ/σ, Q2 A–Z/σ, Q1 σ/A–Z, Q4a σ/KRYPTOS, Q4b KRYPTOS/σ) × 3 conventions (VIG, BEAU, VARB),
 * on évalue des familles de clés sur K4 ET sur NNULL chiffrés « K4 mélangé » dans la même passe.
 * Familles (option -e, liste de lettres) :
 *   p  clé période 7 : pure (P), + décalage par ligne du cuivre (L), + décalage par ligne de 7 (R) ; e_min exact.
 *   b  disque tourné par bloc de n = 5..14, toutes phases (T11/T11b) ; motif s·j (s = 0..25) ou KRYPTOS ; e_min exact.
 *   t  T16 : mot-clé de longueur 1..7 écrit en lignes de 7, relu par colonnes (5 040 ordres, ou l'inverse) ; e_min exact.
 *   a  autoclé sur le clair, L = 1..13 (lettre-clé lue dans l'alphabet du clair, A–Z ou KRYPTOS) ; e_min.
 *   c  autoclé sur le chiffré, L = 1..96 : lettres des cribs reproduites (max, si au moins 11 cribs après L).
 *   r  clé courante anglaise : score quadrigrammes des deux fragments de clé (max) ; il faut -q qg.bin.
 *   m  période 7, convention (VIG/BEAU/VARB) fixée par ligne du cuivre, par ligne de 7, ou en alternance 2/3 ; e_min.
 * Invariances exploitées (familles p, b, t) : inverser l'alphabet ou passer de VIG à VARB change la clé en ±K + constante
 * dans le même type ; ces familles sont invariantes par K → −K + c (motifs KRYPTOS ajoutés avec leur opposé), donc on
 * n'y essaie que l'alphabet à l'endroit et VIG/BEAU. Élagage exact : dans un groupe (phase, bloc, classe), la plus grande
 * multiplicité c vérifie c ≤ 1 + (nombre de paires égales) ; on ne calcule exactement que ce qui peut battre le record.
 * Sortie : pour chaque famille, meilleur de K4, distribution des meilleurs des témoins, p empirique ; cas notables de K4.
 * Usage : k4x -a alphas.bin -e pbtacrm -n NNULL [-q qg.bin] [-s graine] [-T seuil] [-2 themes.bin]   (CTX=… remplace K4, contrôle positif)
 * -2 : Quagmire IV ; au lieu des 5 types, chaque alphabet est apparié à chaque alphabet thématique (clair θ / chiffré σ et
 *      clair σ / chiffré θ). Pour les familles p, b, t, inverser l'un ou l'autre alphabet ne change rien (VIG ↔ BEAU au signe près).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#define NP 24
#define NPR 276
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int POS[NP], PT[NP], ISC[97], CPL[97];
static int NCT; static int (*CTS)[97];
static float *QG;
static unsigned char M26T[256];
static inline int md(int x) { return M26T[x + 128]; } /* x dans [−128, 127] */
enum { F_P, F_L, F_R, F_B, F_T, F_A, F_C, F_RK, F_ML, F_MR, F_MA, NF };
static const char *FNAME[NF] = {"p7 pure", "p7 + décalage ligne cuivre", "p7 + décalage ligne de 7", "blocs T11/T11b",
  "T16 clé transposée", "autoclé clair L≤13", "autoclé chiffré (cribs justes, max)", "clé courante anglaise (score, max)",
  "p7 convention par ligne cuivre", "p7 convention par ligne de 7", "p7 convention alternance 2/3"};
static const int FMAX[NF] = {0,0,0,0,0,0,1,1,0,0,0}; /* 1 : plus grand = meilleur */
static int thr = 3, NTH = 0, NTY = 5; static int (*THS)[26];
/* paires */
static int PA[NPR], PB[NPR];
typedef struct { uint64_t w[5]; } M5;
static inline int pc5(const M5 *a, const M5 *b) { return __builtin_popcountll(a->w[0] & b->w[0]) + __builtin_popcountll(a->w[1] & b->w[1]) +
  __builtin_popcountll(a->w[2] & b->w[2]) + __builtin_popcountll(a->w[3] & b->w[3]) + __builtin_popcountll(a->w[4] & b->w[4]); }
static inline void set5(M5 *m, int i) { m->w[i >> 6] |= 1ULL << (i & 63); }
/* ------------------ période 7 + décalages par segment ------------------ */
static int ph[NP], SEGL[NP], SEGR[NP];
static int SPt[64], SPu[64], NSP; /* paires de même phase */
static unsigned char MCODE[144][64]; /* conventions par combinaison : 27 ligne cuivre, 81 ligne de 7, 9 + 27 alternance */
static int exact7(const int *K, const int *seg, const int *a) {
  int agree = 0;
  for (int j = 0; j < 7; j++) { int cnt[26] = {0}, bc = 0;
    for (int t = j < 0 ? 0 : 0; t < NP; t++) if (ph[t] == j) { int v = md(K[t] - a[seg[t]]); if (++cnt[v] > bc) bc = cnt[v]; }
    agree += bc; }
  return NP - agree;
}
/* renvoie e_min exact si e_min ≤ cut, sinon une borne inférieure > cut */
static int fit7(const int *K, const int *seg, int nseg, int cut) {
  int E = 0; int H[4][4][26]; memset(H, 0, sizeof H);
  for (int q = 0; q < NSP; q++) { int t = SPt[q], u = SPu[q], dk = md(K[u] - K[t]);
    if (seg[t] == seg[u]) E += dk == 0; else H[seg[t]][seg[u]][dk]++; }
  int need = NP - cut - 7 - E; /* il faut Σ H ≥ need */
  int mx = 0; for (int s = 0; s < nseg; s++) for (int r = s + 1; r < nseg; r++) { int m = 0; for (int d = 0; d < 26; d++) if (H[s][r][d] > m) m = H[s][r][d]; mx += m; }
  if (mx < need) return NP - 7 - E - mx;
  int best = 99, a[4] = {0, 0, 0, 0};
  if (nseg == 1) return exact7(K, seg, a);
  if (nseg == 3) { for (a[1] = 0; a[1] < 26; a[1]++) for (a[2] = 0; a[2] < 26; a[2]++) {
      int h = H[0][1][a[1]] + H[0][2][a[2]] + H[1][2][md(a[2] - a[1])]; if (h < need) continue;
      int e = exact7(K, seg, a); if (e < best) best = e; }
    return best < 99 ? best : cut + 1; }
  for (a[1] = 0; a[1] < 26; a[1]++) for (a[2] = 0; a[2] < 26; a[2]++) {
    int h0 = H[0][1][a[1]] + H[0][2][a[2]] + H[1][2][md(a[2] - a[1])];
    for (a[3] = 0; a[3] < 26; a[3]++) { int h = h0 + H[0][3][a[3]] + H[1][3][md(a[3] - a[1])] + H[2][3][md(a[3] - a[2])]; if (h < need) continue;
      int e = exact7(K, seg, a); if (e < best) best = e; } }
  return best < 99 ? best : cut + 1;
}
/* ------------------ blocs T11/T11b ------------------ */
#define NBMAX 100
static M5 BCM[NBMAX]; static int BNB[NBMAX], BN[NBMAX]; static signed char BBLK[NBMAX][NP], BJ[NBMAX][NP]; static int NBL, MINNB;
static uint32_t SOL[14][26]; /* SOL[d][dk] : pas s tels que s·d ≡ dk */
static const int kz[7] = {10, 17, 24, 15, 19, 14, 18};
static int exact_blk(const int *K, int bi, int mot /* 0..25 : s ; 26..29 : KRYPTOS */) {
  int v[NP];
  for (int t = 0; t < NP; t++) { int j = BJ[bi][t], m;
    if (mot < 26) m = (mot * j) % 26; else { int z = (mot & 1) ? kz[6 - j] : kz[j]; m = mot >= 28 ? -z : z; }
    v[t] = md(K[t] - m); }
  int agree = 0, t = 0;
  while (t < NP) { int u = t; while (u < NP && BBLK[bi][u] == BBLK[bi][t]) u++;
    int cnt[26] = {0}, bc = 0; for (int x = t; x < u; x++) if (++cnt[v[x]] > bc) bc = cnt[v[x]];
    agree += bc; t = u; }
  return NP - agree;
}
static int eval_blocks(const int *K, const int *DK, int cut) {
  M5 EQ[26]; memset(EQ, 0, sizeof EQ); int tot[26] = {0};
  for (int p = 0; p < NPR; p++) { int d = POS[PB[p]] - POS[PA[p]]; if (d > 13) continue;
    uint32_t m = SOL[d][DK[p]]; while (m) { int s = __builtin_ctz(m); m &= m - 1; set5(&EQ[s], p); tot[s]++; } }
  int best = cut + 1;
  for (int s = 0; s < 26; s++) { if (NP - MINNB - tot[s] > cut) continue;
    for (int bi = 0; bi < NBL; bi++) { if (NP - BNB[bi] - pc5(&EQ[s], &BCM[bi]) > cut) continue;
      int e = exact_blk(K, bi, s); if (e < best) best = e; } }
  for (int bi = 0; bi < NBL; bi++) if (BN[bi] == 7) for (int mot = 26; mot < 30; mot++) { int e = exact_blk(K, bi, mot); if (e < best) best = e; }
  return best;
}
/* ------------------ T16 : partitions distinctes des 24 positions ------------------ */
static M5 *T16P; static int *T16NC, NT16; static signed char (*T16M)[NP];
static int eval_t16(const int *K, const M5 *EQ0, int cut) {
  int best = cut + 1, tot = __builtin_popcountll(EQ0->w[0]) + __builtin_popcountll(EQ0->w[1]) + __builtin_popcountll(EQ0->w[2]) + __builtin_popcountll(EQ0->w[3]) + __builtin_popcountll(EQ0->w[4]);
  for (int q = 0; q < NT16; q++) {
    if (NP - T16NC[q] - tot > cut) break; /* partitions triées par nombre de classes décroissant */
    if (NP - T16NC[q] - pc5(EQ0, &T16P[q]) > cut) continue;
    const signed char *cl = T16M[q]; int cnt[8][26]; memset(cnt, 0, sizeof(int) * 26 * T16NC[q]); int bc[8] = {0}, agree = 0;
    for (int t = 0; t < NP; t++) { int c = cl[t]; if (++cnt[c][K[t]] > bc[c]) bc[c] = cnt[c][K[t]]; }
    for (int c = 0; c < T16NC[q]; c++) agree += bc[c];
    if (NP - agree < best) best = NP - agree; }
  return best;
}
static void build_t16(void) {
  static int pis[5040][7]; int n = 0, pi[7] = {0, 1, 2, 3, 4, 5, 6};
  do { memcpy(pis[n++], pi, sizeof pi); int i = 5; while (i >= 0 && pi[i] > pi[i + 1]) i--; if (i < 0) break;
    int j = 6; while (pi[j] < pi[i]) j--; int x = pi[i]; pi[i] = pi[j]; pi[j] = x; for (int l = i + 1, r = 6; l < r; l++, r--) { x = pi[l]; pi[l] = pi[r]; pi[r] = x; } } while (1);
  int cap = 5040 * 7 * 2 * 2; T16M = malloc(sizeof(*T16M) * cap); T16P = calloc(cap, sizeof(M5)); T16NC = malloc(sizeof(int) * cap); NT16 = 0;
  uint64_t *hs = calloc(1 << 22, 8);
  for (int k = 0; k < n; k++) for (int inv = 0; inv < 2; inv++) for (int N = 97; N <= 98; N++) for (int L = 1; L <= 7; L++) {
    int h = (N + 6) / 7, full = N % 7 == 0 ? 7 : N % 7, rd[98], t = 0, rank[98];
    for (int qq = 0; qq < 7; qq++) { int col = pis[k][qq], hc = col < full ? h : h - 1; for (int r = 0; r < hc; r++) { if (!inv) rd[t] = r * 7 + col; else rank[r * 7 + col] = t; t++; } }
    if (inv) for (int m = 0; m < N; m++) rd[m] = rank[m];
    /* rd[i] = rang de lecture de la position i ; la clé de i est la lettre (rang mod L) du mot */
    signed char cl[NP]; int map[98]; for (int i = 0; i < 98; i++) map[i] = -1; int nx = 0; uint64_t hh = 1469598103934665603ULL;
    for (int u = 0; u < NP; u++) { int c = rd[POS[u]] % L; if (map[c] < 0) map[c] = nx++; cl[u] = map[c]; hh = (hh ^ (uint64_t)cl[u]) * 1099511628211ULL; }
    hh |= 1; uint32_t ix = hh & ((1 << 22) - 1); int dup = 0; while (hs[ix]) { if (hs[ix] == hh) { dup = 1; break; } ix = (ix + 1) & ((1 << 22) - 1); }
    if (dup) continue;
    hs[ix] = hh; memcpy(T16M[NT16], cl, NP); T16NC[NT16] = nx;
    for (int p = 0; p < NPR; p++) if (cl[PA[p]] == cl[PB[p]]) set5(&T16P[NT16], p);
    NT16++; }
  free(hs);
  /* tri par nombre de classes décroissant (tri par dénombrement, stable) */
  signed char (*m2)[NP] = malloc(sizeof(*m2) * NT16); M5 *p2 = malloc(sizeof(M5) * NT16); int *n2 = malloc(sizeof(int) * NT16), k = 0;
  for (int c = 7; c >= 1; c--) for (int q = 0; q < NT16; q++) if (T16NC[q] == c) { memcpy(m2[k], T16M[q], NP); p2[k] = T16P[q]; n2[k] = c; k++; }
  free(T16M); free(T16P); free(T16NC); T16M = m2; T16P = p2; T16NC = n2;
}
/* ------------------ main ------------------ */
int main(int argc, char **argv) {
  const char *afile = 0, *ev = "p", *qf = 0, *thf = 0; int NNULL = 20; uint64_t seed0 = 1000;
  for (int i = 1; i < argc; i++) { if (!strcmp(argv[i], "-a")) afile = argv[++i]; else if (!strcmp(argv[i], "-e")) ev = argv[++i];
    else if (!strcmp(argv[i], "-n")) NNULL = atoi(argv[++i]); else if (!strcmp(argv[i], "-q")) qf = argv[++i];
    else if (!strcmp(argv[i], "-s")) seed0 = strtoull(argv[++i], 0, 10); else if (!strcmp(argv[i], "-T")) thr = atoi(argv[++i]); else if (!strcmp(argv[i], "-2")) thf = argv[++i]; }
  for (int x = -128; x < 128; x++) M26T[x + 128] = ((x % 26) + 26) % 26;
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = b[i] - 'A'; n++; }
  for (int t = 0; t < NP; t++) { ISC[POS[t]] = 1; CPL[POS[t]] = PT[t]; ph[t] = POS[t] % 7;
    SEGL[t] = (POS[t] + 27) / 31 - 1; /* lignes 26, 27, 28 du cuivre (K4 commence en colonne 27 de la ligne 25) */
    int r = POS[t] / 7; SEGR[t] = r == 3 ? 0 : r == 4 ? 1 : r == 9 ? 2 : 3; }
  { int p = 0; for (int t = 0; t < NP; t++) for (int u = t + 1; u < NP; u++) { PA[p] = t; PB[p] = u; p++; } }
  NSP = 0; for (int t = 0; t < NP; t++) for (int u = t + 1; u < NP; u++) if (ph[t] == ph[u]) { SPt[NSP] = t; SPu[NSP] = u; NSP++; }
  for (int d = 1; d < 14; d++) for (int s = 0; s < 26; s++) SOL[d][(s * d) % 26] |= 1u << s;
  for (int cb = 0; cb < 144; cb++) { int modeOf[NP];
    for (int t = 0; t < NP; t++) { int sg, c;
      if (cb < 27) { sg = SEGL[t]; c = cb; } else if (cb < 108) { sg = SEGR[t]; c = cb - 27; } else if (cb < 117) { sg = POS[t] % 2; c = cb - 108; } else { sg = POS[t] % 3; c = cb - 117; }
      for (int q = 0; q < sg; q++) c /= 3; modeOf[t] = c % 3; }
    for (int q = 0; q < NSP; q++) MCODE[cb][q] = modeOf[SPt[q]] * 3 + modeOf[SPu[q]]; }
  NCT = 1 + NNULL; CTS = malloc(sizeof(*CTS) * NCT);
  const char *K4X = getenv("CTX") ? getenv("CTX") : K4;
  for (int i = 0; i < 97; i++) CTS[0][i] = K4X[i] - 'A';
  for (int z = 1; z < NCT; z++) { memcpy(CTS[z], CTS[0], sizeof CTS[0]); uint64_t s = (seed0 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CTS[z][i]; CTS[z][i] = CTS[z][j]; CTS[z][j] = t; } }
  if (qf) { FILE *f = fopen(qf, "rb"); QG = malloc(456976 * 4); if (!f || fread(QG, 4, 456976, f) != 456976) return 2; fclose(f); }
  int doP = !!strchr(ev, 'p'), doB = !!strchr(ev, 'b'), doA = !!strchr(ev, 'a'), doC = !!strchr(ev, 'c'), doR = !!strchr(ev, 'r') && QG, doM = !!strchr(ev, 'm'), doT = !!strchr(ev, 't');
  NBL = 0; MINNB = 99;
  for (int nn = 5; nn <= 14; nn++) for (int phi = 0; phi < nn; phi++) { int bi = NBL++; BN[bi] = nn;
    for (int t = 0; t < NP; t++) { int i = POS[t]; BBLK[bi][t] = (i - phi + nn) / nn; BJ[bi][t] = ((i - phi) % nn + nn) % nn; }
    int nb = 1; for (int t = 1; t < NP; t++) if (BBLK[bi][t] != BBLK[bi][t - 1]) nb++; BNB[bi] = nb; if (nb < MINNB) MINNB = nb;
    memset(&BCM[bi], 0, sizeof(M5)); for (int p = 0; p < NPR; p++) if (BBLK[bi][PA[p]] == BBLK[bi][PB[p]]) set5(&BCM[bi], p); }
  if (doT) { build_t16(); fprintf(stderr, "T16 : %d partitions distinctes\n", NT16); }
  FILE *af = fopen(afile, "rb"); if (!af) return 4; fseek(af, 0, SEEK_END); long na = ftell(af) / 26; fseek(af, 0, SEEK_SET);
  char *AB = malloc(na * 26); if (fread(AB, 26, na, af) != (size_t)na) return 3; fclose(af);
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  if (thf) { FILE *f = fopen(thf, "rb"); if (!f) return 5; fseek(f, 0, SEEK_END); NTH = ftell(f) / 26; fseek(f, 0, SEEK_SET);
    THS = malloc(sizeof(*THS) * NTH); for (int q = 0; q < NTH; q++) { char w[26]; if (fread(w, 1, 26, f) != 26) return 5; for (int i = 0; i < 26; i++) THS[q][w[i] - 'A'] = i; } fclose(f);
    NTY = 2 * NTH; fprintf(stderr, "Quagmire IV : %d alphabets thématiques\n", NTH); }
  float (*best)[NF] = malloc(sizeof(*best) * NCT);
  for (int z = 0; z < NCT; z++) for (int f = 0; f < NF; f++) best[z][f] = FMAX[f] ? -1e9f : 99;
  double t0 = omp_get_wtime();
  #pragma omp parallel
  {
    float (*lb)[NF] = malloc(sizeof(*lb) * NCT);
    for (int z = 0; z < NCT; z++) for (int f = 0; f < NF; f++) lb[z][f] = FMAX[f] ? -1e9f : 99;
    #pragma omp for schedule(dynamic, 16)
    for (long ai = 0; ai < na * 2; ai++) {
      int S[26]; const char *a = AB + (ai >> 1) * 26; int rev = ai & 1;
      for (int i = 0; i < 26; i++) S[(rev ? a[25 - i] : a[i]) - 'A'] = i;
      int inv = !rev; /* familles invariantes : seulement à l'endroit */
      for (int ty = 0; ty < NTY; ty++) {
        const int *Y, *X;
        if (!NTH) { Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP; X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S; }
        else { const int *T = THS[ty >> 1]; if (ty & 1) { Y = S; X = T; } else { Y = T; X = S; } } /* Quagmire IV : alphabet thématique d'un côté */
        int YI[26]; for (int i = 0; i < 26; i++) YI[Y[i]] = i;
        for (int z = 0; z < NCT; z++) {
          const int *CT = CTS[z];
          int Km[3][NP];
          for (int t = 0; t < NP; t++) { int x = X[CT[POS[t]]], y = Y[PT[t]]; Km[0][t] = md(x - y); Km[1][t] = md(x + y); Km[2][t] = md(y - x); }
          #define CUT(f) (z == 0 ? (lb[z][f] > thr ? (int)lb[z][f] : thr) : (int)lb[z][f] - 1)
          #define UPD(f, v, mo) do { float _v = (v); if (FMAX[f] ? _v > lb[z][f] : _v < lb[z][f]) lb[z][f] = _v; \
            if (z == 0 && !FMAX[f] && _v <= thr) { _Pragma("omp critical(pr)") printf("K4 %s : e=%d alph=%.26s rev=%d type=%d mode=%d\n", FNAME[f], (int)_v, a, rev, ty, mo); } } while (0)
          for (int mo = 0; mo < 3; mo++) {
            const int *K = Km[mo];
            if (inv && mo < 2 && (doP || doB || doT)) {
              int DK[NPR]; M5 EQ0; memset(&EQ0, 0, sizeof EQ0);
              for (int p = 0; p < NPR; p++) { DK[p] = md(K[PB[p]] - K[PA[p]]); if (!DK[p]) set5(&EQ0, p); }
              if (doP) { int segP[NP] = {0};
                UPD(F_P, fit7(K, segP, 1, CUT(F_P)), mo); UPD(F_L, fit7(K, SEGL, 3, CUT(F_L)), mo); UPD(F_R, fit7(K, SEGR, 4, CUT(F_R)), mo); }
              if (doB) UPD(F_B, eval_blocks(K, DK, CUT(F_B)), mo);
              if (doT) UPD(F_T, eval_t16(K, &EQ0, CUT(F_T)), mo);
            }
            if (doR) {
              for (int zr = 0; zr < 3; zr++) { const int *Z = zr == 0 ? Y : zr == 1 ? AZ : KAP; int ZI[26]; for (int i = 0; i < 26; i++) ZI[Z[i]] = i;
                int k[NP]; for (int t = 0; t < NP; t++) k[t] = ZI[K[t]];
                for (int dir = 0; dir < 2; dir++) { int kk[NP];
                  for (int t = 0; t < 13; t++) kk[t] = dir ? k[12 - t] : k[t]; for (int t = 0; t < 11; t++) kk[13 + t] = dir ? k[23 - t] : k[13 + t];
                  double s = 0; for (int i = 0; i + 3 < 13; i++) s += QG[((kk[i] * 26 + kk[i + 1]) * 26 + kk[i + 2]) * 26 + kk[i + 3]];
                  for (int i = 13; i + 3 < 24; i++) s += QG[((kk[i] * 26 + kk[i + 1]) * 26 + kk[i + 2]) * 26 + kk[i + 3]];
                  float sc = s / 18.0; if (sc > lb[z][F_RK]) { lb[z][F_RK] = sc; if (z == 0 && sc > -4.3) {
                    _Pragma("omp critical(pr)") printf("K4 clé courante %.3f alph=%.26s rev=%d type=%d mode=%d lu=%d dir=%d\n", sc, a, rev, ty, mo, zr, dir); } } } }
            }
            if (doA) {
              for (int zr = 0; zr < 3; zr++) { const int *Z = zr == 0 ? Y : zr == 1 ? AZ : KAP;
                for (int L = 1; L <= 13; L++) { int cut = CUT(F_A), err = 0;
                  for (int r = 0; r < L && err <= cut; r++) { int s0 = -1; for (int t = 0; t < NP; t++) if (POS[t] % L == r) { s0 = POS[t]; break; }
                    int p = CPL[s0];
                    for (int i = s0 + L; i < 97; i += L) { int k = Z[p], x = X[CT[i]], y = mo == 0 ? md(x - k) : mo == 1 ? md(k - x) : md(x + k); p = YI[y];
                      if (ISC[i] && p != CPL[i] && ++err > cut) break; } }
                  if (err <= cut) UPD(F_A, err, mo); } }
            }
            if (doC) {
              int kreq[NP]; for (int t = 0; t < NP; t++) { int x = X[CT[POS[t]]], y = Y[PT[t]]; kreq[t] = mo == 0 ? md(x - y) : mo == 1 ? md(x + y) : md(y - x); }
              for (int zr = 0; zr < 3; zr++) { const int *Z = zr == 0 ? X : zr == 1 ? AZ : KAP;
                int ag[97] = {0}, bk[26][97], nb[26] = {0};
                for (int i = 0; i < 73; i++) { int v = Z[CT[i]]; bk[v][nb[v]++] = i; }
                for (int t = 0; t < NP; t++) { int v = kreq[t]; for (int q = 0; q < nb[v] && bk[v][q] < POS[t]; q++) ag[POS[t] - bk[v][q]]++; }
                for (int L = 1; L <= 73 - 10; L++) if (ag[L] > lb[z][F_C]) lb[z][F_C] = ag[L]; } /* L ≤ 63 : au moins les 11 lettres de BERLINCLOCK après L */
            }
          }
          if (doM) {
            /* paires de même phase : masque 9 bits des couples de conventions (mt, mu) qui donnent la même valeur */
            uint16_t pm[64]; for (int q = 0; q < NSP; q++) { int t = SPt[q], u = SPu[q]; pm[q] = 0;
              for (int x = 0; x < 3; x++) for (int y = 0; y < 3; y++) if (Km[x][t] == Km[y][u]) pm[q] |= 1 << (x * 3 + y); }
            for (int cb = 0; cb < 144; cb++) { int cnt[NP]; for (int t = 0; t < NP; t++) cnt[t] = 1;
              const unsigned char *code = MCODE[cb];
              for (int q = 0; q < NSP; q++) if (pm[q] >> code[q] & 1) { cnt[SPt[q]]++; cnt[SPu[q]]++; }
              int bc[7] = {0}, agree = 0; for (int t = 0; t < NP; t++) if (cnt[t] > bc[ph[t]]) bc[ph[t]] = cnt[t];
              for (int j = 0; j < 7; j++) agree += bc[j];
              UPD(cb < 27 ? F_ML : cb < 108 ? F_MR : F_MA, NP - agree, cb); }
          }
        }
      }
    }
    #pragma omp critical
    for (int z = 0; z < NCT; z++) for (int f = 0; f < NF; f++) if (FMAX[f] ? lb[z][f] > best[z][f] : lb[z][f] < best[z][f]) best[z][f] = lb[z][f];
    free(lb);
  }
  double dt = omp_get_wtime() - t0;
  fprintf(stderr, "alphabets %ld, %d chiffrés, %.1f s\n", na, NCT, dt);
  printf("== résumé (%ld alphabets, %d témoins K4 mélangé, graine %llu) ==\n", na, NNULL, (unsigned long long)seed0);
  for (int f = 0; f < NF; f++) {
    if ((f <= F_R && !doP) || (f == F_B && !doB) || (f == F_A && !doA) || (f == F_C && !doC) || (f == F_RK && !doR) || (f >= F_ML && !doM) || (f == F_T && !doT)) continue;
    int better = 0; float v[1024]; int nv = 0;
    for (int z = 1; z < NCT; z++) { v[nv++] = best[z][f]; if (FMAX[f] ? best[z][f] >= best[0][f] : best[z][f] <= best[0][f]) better++; }
    for (int i = 0; i < nv; i++) for (int j = i + 1; j < nv; j++) if (v[j] < v[i]) { float x = v[i]; v[i] = v[j]; v[j] = x; }
    if (!nv) { printf("%-40s K4 %7.3f\n", FNAME[f], best[0][f]); continue; }
    printf("%-40s K4 %7.3f | témoins min %7.3f médiane %7.3f max %7.3f | p = %.3f\n", FNAME[f], best[0][f], v[0], v[nv / 2], v[nv - 1], (better + 1.0) / (NNULL + 1));
  }
  return 0;
}
