/* recherche_procedes.c — recherche automatique de PROCÉDÉS (et non de clés) qui fabriquent l'empreinte de K4 (25/09/2026).
 *
 * Grammaire : chiffre positionnel et déchiffrable, faisable à la main, dans l'alphabet σ (KRYPTOS par défaut) :
 *   VIG : σc_i = σp_i + k_i     BEAU : σc_i = k_i − σp_i     VAR : σc_i = σp_i − k_i
 *   k_i = base_i + Σ_t [la règle t s'applique à la colonne i mod 7] · s_t · α_t(x_t[i − L_t])
 *   base : aucune | période 7 | clé par ligne de 7 | clé courante anglaise | progressive
 *   terme : x = clair ou chiffré, écart L ∈ {1..8, 14}, lettre lue dans α ∈ {σ, A–Z, ρ (alphabet à mot-clé tiré au hasard)},
 *           signe ±, appliqué à toutes les colonnes, à une seule colonne j, ou à toutes sauf j.
 *   (écart ≥ 1 : la clé ne dépend que du passé, le chiffre est donc déchiffrable.)
 * Chaque procédé est simulé NSIM fois (clair anglais Gutenberg avec les cribs à leur place, clés et ρ tirés au hasard).
 * Note : log-vraisemblance des comptes de K4 sous les taux du procédé (binomiales, pseudo-comptes 0,5) :
 *   doublets par colonne mod 7 (K4 : 1,0,0,0,5,0,0 ; ou ceux du chiffré CTX pour un contrôle), coïncidences aux écarts 7, 14, 21 (9, 2, 3), doublets aux paires de crib
 *   25–26, 32–33, 67–68 (les trois présents), paires égales dans chaque colonne mod 7 (IC par colonne : écarte les procédés
 *   qui laissent passer le clair dans une colonne). Puis, pour les meilleurs, probabilité directe de la signature jointe.
 * Usage : recherche_procedes corpus.txt alphas.bin [NSIM] [NTOP] [NSIM2]   env : SIGMA=AZ pour σ = A–Z ; TWO=1 : deux termes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <omp.h>
#define N 97
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int SIG[26], SINV[26];
static unsigned char *COR; static long NCOR; static unsigned char *AL; static long NA;
typedef struct { uint64_t a, b; } R;
static inline uint64_t nx(R *r) { uint64_t s1 = r->a, s0 = r->b; r->a = s0; s1 ^= s1 << 23; r->b = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26); return r->b + s0; }
static inline int rn(R *r, int n) { return (int)((nx(r) >> 33) % (uint64_t)n); }
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static const int LAGS[9] = {1, 2, 3, 4, 5, 6, 7, 8, 14};
typedef struct { signed char src, lag, alph, sgn, restr, col; } Term;   /* src 0 clair, 1 chiffré ; restr 0 toutes, 1 seule col, 2 sauf col */
typedef struct { int mode, base, nt; Term t[2]; } Proc;
static const char *MODEN[] = {"VIG", "BEAU", "VAR"}, *BASEN[] = {"aucune", "période 7", "ligne de 7", "courante", "progressive"};

static void describe(const Proc *P, char *s) {
  int n = sprintf(s, "%s, base %s", MODEN[P->mode], BASEN[P->base]);
  for (int t = 0; t < P->nt; t++) { const Term *T = &P->t[t];
    n += sprintf(s + n, " %c %s[i-%d] lu en %s%s", T->sgn > 0 ? '+' : '-', T->src ? "chiffré" : "clair", T->lag, T->alph == 0 ? "σ" : T->alph == 1 ? "A–Z" : "ρ",
                 T->restr == 0 ? "" : T->restr == 1 ? " (colonne " : " (sauf colonne ");
    if (T->restr) n += sprintf(s + n, "%d)", T->col); }
}
static inline int applies(const Term *T, int i) { int j = i % 7; return T->restr == 0 || (T->restr == 1 && j == T->col) || (T->restr == 2 && j != T->col); }

typedef struct { long dcol[7], a7, a14, a21, cr[3], joint, prof, dmax5, icc[7]; } Cnt;
static void sim(const Proc *P, R *r, Cnt *C) {
  int p[N], c[N], sp[N], sc[N], rho[26], m[7], a[15], pr[16], e[N];
  long s = (long)(nx(r) % (uint64_t)(NCOR - N)); for (int i = 0; i < N; i++) p[i] = COR[s + i];
  { const char *E = "EASTNORTHEAST", *B = "BERLINCLOCK"; for (int i = 0; i < 13; i++) p[21 + i] = E[i] - 'A'; for (int i = 0; i < 11; i++) p[63 + i] = B[i] - 'A'; }
  { long k = rn(r, (int)NA); for (int i = 0; i < 26; i++) rho[AL[k * 26 + i] - 'A'] = i; }
  for (int j = 0; j < 7; j++) m[j] = rn(r, 26);
  for (int j = 0; j < 15; j++) a[j] = rn(r, 26);
  for (int j = 0; j < 16; j++) pr[j] = rn(r, 26);
  int stp = 1 + rn(r, 25);
  if (P->base == 3) { long s2 = (long)(nx(r) % (uint64_t)(NCOR - N)); for (int i = 0; i < N; i++) e[i] = SIG[COR[s2 + i]]; }
  for (int i = 0; i < N; i++) {
    int k = P->base == 1 ? m[i % 7] : P->base == 2 ? a[i / 7] : P->base == 3 ? e[i] : P->base == 4 ? stp * i : 0;
    for (int t = 0; t < P->nt; t++) { const Term *T = &P->t[t]; if (!applies(T, i)) continue;
      int v; if (i - T->lag < 0) v = pr[(i + t * 7) & 15]; else { int x = T->src ? c[i - T->lag] : p[i - T->lag]; v = T->alph == 0 ? SIG[x] : T->alph == 1 ? x : rho[x]; }
      k += T->sgn * v; }
    sp[i] = SIG[p[i]];
    int y = P->mode == 0 ? sp[i] + k : P->mode == 1 ? k - sp[i] : sp[i] - k;
    sc[i] = md(y); c[i] = SINV[sc[i]];
  }
  int dc[7] = {0}, nd = 0;
  for (int i = 0; i + 1 < N; i++) if (c[i] == c[i + 1]) { dc[i % 7]++; nd++; }
  for (int j = 0; j < 7; j++) C->dcol[j] += dc[j];
  int a7 = 0, a14 = 0, a21 = 0;
  for (int i = 0; i + 7 < N; i++) a7 += c[i] == c[i + 7];
  for (int i = 0; i + 14 < N; i++) a14 += c[i] == c[i + 14];
  for (int i = 0; i + 21 < N; i++) a21 += c[i] == c[i + 21];
  C->a7 += a7; C->a14 += a14; C->a21 += a21;
  for (int j = 0; j < 7; j++) { int cnt[26] = {0}; for (int i = j; i < N; i += 7) cnt[c[i]]++; long pp = 0; for (int q = 0; q < 26; q++) pp += cnt[q] * (cnt[q] - 1) / 2; C->icc[j] += pp; }
  int c1 = c[25] == c[26], c2 = c[32] == c[33], c3 = c[67] == c[68];
  C->cr[0] += c1; C->cr[1] += c2; C->cr[2] += c3;
  int prof = a7 >= 9 && a14 + a21 <= 5; C->prof += prof;
  C->dmax5 += dc[4] >= 5;
  C->joint += prof && dc[4] >= 5 && c1 && c2 && c3;
  (void)nd;
}
/* log-vraisemblance des comptes de K4 sous les taux estimés */
static double lbin(long k, long n, double p) { return lgamma(n + 1.0) - lgamma(k + 1.0) - lgamma(n - k + 1.0) + k * log(p) + (n - k) * log(1 - p); }
static int K4D[7], FA7, FA14, FA21, FCR[3], FIC[7], NPAIR[7]; static const int NSLOT[7] = {14, 14, 14, 14, 14, 14, 13};
static void features(const char *ct) {   /* empreinte du chiffré observé (K4, ou CTX pour le contrôle) */
  memset(K4D, 0, sizeof K4D); for (int i = 0; i + 1 < N; i++) if (ct[i] == ct[i + 1]) K4D[i % 7]++;
  FA7 = FA14 = FA21 = 0; for (int i = 0; i + 7 < N; i++) FA7 += ct[i] == ct[i + 7]; for (int i = 0; i + 14 < N; i++) FA14 += ct[i] == ct[i + 14];
  for (int i = 0; i + 21 < N; i++) FA21 += ct[i] == ct[i + 21];
  FCR[0] = ct[25] == ct[26]; FCR[1] = ct[32] == ct[33]; FCR[2] = ct[67] == ct[68];
  for (int j = 0; j < 7; j++) { int cnt[26] = {0}, n = 0; for (int i = j; i < N; i += 7) { cnt[ct[i] - 'A']++; n++; } FIC[j] = 0; for (int q = 0; q < 26; q++) FIC[j] += cnt[q] * (cnt[q] - 1) / 2; NPAIR[j] = n * (n - 1) / 2; }
}
static double score(const Cnt *C, long ns, double *parts) {
  double L = 0, ld = 0, la = 0, lc = 0;
  for (int j = 0; j < 7; j++) { double r = (C->dcol[j] + 0.5) / (ns * (double)NSLOT[j] + 1); ld += lbin(K4D[j], NSLOT[j], r); }
  la += lbin(FA7, 90, (C->a7 + 0.5) / (ns * 90.0 + 1)) + lbin(FA14, 83, (C->a14 + 0.5) / (ns * 83.0 + 1)) + lbin(FA21, 76, (C->a21 + 0.5) / (ns * 76.0 + 1));
  for (int q = 0; q < 3; q++) { double r = (C->cr[q] + 0.5) / (ns + 1.0); lc += FCR[q] ? log(r) : log(1 - r); }
  double li = 0; for (int j = 0; j < 7; j++) li += lbin(FIC[j], NPAIR[j], (C->icc[j] + 0.5) / (ns * (double)NPAIR[j] + 1));
  L = ld + la + lc + li; if (parts) { parts[0] = ld; parts[1] = la; parts[2] = lc; parts[3] = li; }
  return L;
}
typedef struct { Proc P; double L, ld, la, lc, li; Cnt C; } Res;
static int cmpres(const void *x, const void *y) { double a = ((const Res *)x)->L, b = ((const Res *)y)->L; return a < b ? 1 : a > b ? -1 : 0; }

static void run_proc(const Proc *P, long ns, uint64_t seed, Cnt *out) {
  memset(out, 0, sizeof *out);
  R r = {seed ^ 0x9E3779B97F4A7C15ULL, seed * 0xD1B54A32D192ED03ULL + 7}; for (int w = 0; w < 10; w++) nx(&r);
  for (long it = 0; it < ns; it++) sim(P, &r, out);
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  long NS = argc > 3 ? atol(argv[3]) : 20000; int NTOP = argc > 4 ? atoi(argv[4]) : 25; long NS2 = argc > 5 ? atol(argv[5]) : 2000000;
  const char *KA = getenv("SIGMA") && !strcmp(getenv("SIGMA"), "AZ") ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "KRYPTOSABCDEFGHIJLMNQUVWXZ";
  for (int i = 0; i < 26; i++) { SIG[KA[i] - 'A'] = i; SINV[i] = KA[i] - 'A'; }
  FILE *f = fopen(argv[1], "rb"); fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
  unsigned char *raw = malloc(sz); if (fread(raw, 1, sz, f) != (size_t)sz) return 2; fclose(f);
  COR = malloc(sz); NCOR = 0; for (long i = 0; i < sz; i++) { int ch = raw[i]; if (ch >= 'a' && ch <= 'z') ch -= 32; if (ch >= 'A' && ch <= 'Z') COR[NCOR++] = ch - 'A'; } free(raw);
  f = fopen(argv[2], "rb"); fseek(f, 0, SEEK_END); NA = ftell(f) / 26; fseek(f, 0, SEEK_SET); AL = malloc(NA * 26); if (fread(AL, 26, NA, f) != (size_t)NA) return 3; fclose(f);
  features(getenv("CTX") ? getenv("CTX") : K4);
  printf("empreinte : doublets par colonne %d %d %d %d %d %d %d ; écarts 7/14/21 : %d %d %d ; doublets de crib %d %d %d\n", K4D[0], K4D[1], K4D[2], K4D[3], K4D[4], K4D[5], K4D[6], FA7, FA14, FA21, FCR[0], FCR[1], FCR[2]);
  /* énumération : un terme (ou aucun) */
  int cap = 40000, np = 0; Proc *PL = malloc(sizeof(Proc) * cap);
  for (int mode = 0; mode < 3; mode++) for (int base = 0; base < 5; base++) {
    if (base) PL[np++] = (Proc){mode, base, 0};
    for (int src = 0; src < 2; src++) for (int li = 0; li < 9; li++) for (int al = 0; al < 3; al++) for (int sg = -1; sg <= 1; sg += 2)
      for (int rs = 0; rs < 3; rs++) for (int col = 0; col < (rs ? 7 : 1); col++) {
        if (base == 0 && rs != 0) continue;   /* une colonne sans clé : le clair y passerait */
        Proc P = {mode, base, 1}; P.t[0] = (Term){src, LAGS[li], al, sg, rs, col}; PL[np++] = P; }
  }
  printf("σ = %.26s ; %d procédés à un terme × %ld simulations\n", KA, np, NS);
  Res *RS = malloc(sizeof(Res) * cap);
  double t0 = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 8)
  for (int q = 0; q < np; q++) { Cnt C; run_proc(&PL[q], NS, 1000003ULL * q + 17, &C); double pt[4]; double L = score(&C, NS, pt);
    RS[q] = (Res){PL[q], L, pt[0], pt[1], pt[2], pt[3], C}; }
  printf("balayage : %.0f s\n", omp_get_wtime() - t0);
  /* références */
  Proc U = {0, 0, 0}; (void)U;
  qsort(RS, np, sizeof(Res), cmpres);
  double Lref = 0; { for (int q = 0; q < np; q++) if (RS[q].P.nt == 0 && RS[q].P.base == 3 && RS[q].P.mode == 0) Lref = RS[q].L; }
  printf("référence (clé courante anglaise, quasi hasard) : logL = %.2f ; MEILLEUR ΔlogL = %.2f\n\n", Lref, RS[0].L - Lref);
  int quiet = getenv("QUIET") != NULL;
  printf("%-4s %-86s %8s %7s %7s %7s %7s | %6s %6s %6s %6s\n", "rang", "procédé", "ΔlogL", "doubl.", "écarts", "cribs", "IC col", "d(c4)", "A7", "A14", "A21");
  for (int q = 0; q < (quiet ? 0 : NTOP) && q < np; q++) { char d[256]; describe(&RS[q].P, d); const Cnt *C = &RS[q].C;
    printf("%4d %-86s %8.2f %7.2f %7.2f %7.2f %7.2f | %6.3f %6.3f %6.3f %6.3f\n", q + 1, d, RS[q].L - Lref, RS[q].ld, RS[q].la, RS[q].lc, RS[q].li,
           C->dcol[4] / (NS * 14.0), C->a7 / (NS * 90.0), C->a14 / (NS * 83.0), C->a21 / (NS * 76.0)); }
  /* deux termes : on combine les 60 meilleurs termes simples entre eux (même mode et même base) */
  if (getenv("TWO")) {
    int K = 60, n2 = 0; Proc *P2 = malloc(sizeof(Proc) * K * K);
    for (int x = 0; x < K; x++) for (int y = x + 1; y < K; y++) { const Proc *A = &RS[x].P, *B = &RS[y].P;
      if (A->nt != 1 || B->nt != 1 || A->mode != B->mode || A->base != B->base) continue;
      Proc P = *A; P.nt = 2; P.t[1] = B->t[0]; P2[n2++] = P; }
    Res *R2 = malloc(sizeof(Res) * (n2 ? n2 : 1));
#pragma omp parallel for schedule(dynamic, 4)
    for (int q = 0; q < n2; q++) { Cnt C; run_proc(&P2[q], NS, 7777ULL * q + 5, &C); double pt[4]; double L = score(&C, NS, pt); R2[q] = (Res){P2[q], L, pt[0], pt[1], pt[2], pt[3], C}; }
    qsort(R2, n2, sizeof(Res), cmpres);
    printf("\ndeux termes (%d combinaisons des 60 meilleurs) : MEILLEUR2 ΔlogL = %.2f\n", n2, n2 ? R2[0].L - Lref : 0);
    for (int q = 0; q < (quiet ? 0 : NTOP) && q < n2; q++) { char d[256]; describe(&R2[q].P, d); const Cnt *C = &R2[q].C;
      printf("%4d %-86s %8.2f %7.2f %7.2f %7.2f %7.2f | %6.3f %6.3f %6.3f %6.3f\n", q + 1, d, R2[q].L - Lref, R2[q].ld, R2[q].la, R2[q].lc, R2[q].li,
             C->dcol[4] / (NS * 14.0), C->a7 / (NS * 90.0), C->a14 / (NS * 83.0), C->a21 / (NS * 76.0)); }
    /* on remet les meilleurs à deux termes dans la liste finale */
    for (int q = 0; q < 10 && q < n2; q++) RS[NTOP + q] = R2[q];
    NTOP += 10;
  }
  if (quiet) return 0;
  /* signature jointe, estimée directement pour les meilleurs */
  printf("\nsignature jointe (profil « voisin » ET ≥ 5 doublets en colonne 4 ET les 3 doublets de crib), %ld simulations :\n", NS2);
  Proc refs[2] = {{0, 3, 0}, {0, 0, 1, {{0, 7, 0, 1, 0, 0}}}};
  for (int q = -2; q < NTOP && q < np; q++) {
    const Proc *P = q < 0 ? &refs[q + 2] : &RS[q].P; long tot = 0, pro = 0, dm = 0;
#pragma omp parallel reduction(+:tot,pro,dm)
    { Cnt C; long per = NS2 / omp_get_num_threads(); run_proc(P, per, 99991ULL * (q + 3) + 131 * omp_get_thread_num(), &C); tot += C.joint; pro += C.prof; dm += C.dmax5; }
    char d[256]; describe(P, d);
    printf("  %-90s P(jointe) = %.2e  P(profil) = %.4f  P(≥5 doublets col. 4) = %.2e\n", d, (double)tot / NS2, (double)pro / NS2, (double)dm / NS2);
  }
  return 0;
}
