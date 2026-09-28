/* k4tp.c — T4 « TABP » : transposition à colonnes + substitution périodique (hypothèse NSA 1992 : « alphabets puis transposition »).
 * Transposition π (position du clair i → position du chiffré π(i)) : clair écrit en lignes de w = 2..20, relu par colonnes
 * dans un ordre de colonnes (toutes les permutations pour w ≤ 8 ; ordres tirés des mots du dictionnaire de w lettres
 * pour 9 ≤ w ≤ 20), colonnes lues de haut en bas ou de bas en haut (route de K3) ; π et son inverse.
 * Substitution périodique de période p = 1..13, deux ordres : substitution d'abord (phase = i mod p) ou transposition
 * d'abord (phase = π(i) mod p). Alphabets : KRYPTOS et A–Z, en Q3 (KA/KA), A–Z/A–Z, A–Z/KA, KA/A–Z ; VIG, BEAU
 * (VARB = VIG avec la clé opposée ; même e_min, même déchiffrement).
 * Pour chaque cas : e_min (majorité par phase) ; si e_min ≤ EDEC, la clé (majorité) déchiffre les 97 lettres et on note
 * le texte (quadrigrammes, log10 moyen). K4 et NNULL chiffrés « K4 mélangé » dans la même passe.
 * Usage : k4tp -w mots.txt -q qg.bin -n NNULL [-s graine] [-E EDEC]    (CTX=… : chiffré de contrôle)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#define NP 24
#define N 97
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int POS[NP], PT[NP], NCT; static int (*CTS)[N]; static float *QG;
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
typedef struct { unsigned char w, ord[20]; } Ord;
static Ord *ORD; static long NORD, CAP;
static uint64_t *HS; static const uint32_t HM = (1u << 24) - 1;
static void addord(int w, const unsigned char *o) {
  uint64_t h = 1469598103934665603ULL ^ w; for (int i = 0; i < w; i++) h = (h ^ o[i]) * 1099511628211ULL; h |= 1;
  uint32_t ix = h & HM; while (HS[ix]) { if (HS[ix] == h) return; ix = (ix + 1) & HM; } HS[ix] = h;
  if (NORD == CAP) { CAP = CAP ? CAP * 2 : 65536; ORD = realloc(ORD, CAP * sizeof(Ord)); }
  ORD[NORD].w = w; memcpy(ORD[NORD].ord, o, w); NORD++;
}
static void perms(int w, unsigned char *a, int k) { if (k == w) { addord(w, a); return; }
  for (int i = k; i < w; i++) { unsigned char t = a[k]; a[k] = a[i]; a[i] = t; perms(w, a, k + 1); t = a[k]; a[k] = a[i]; a[i] = t; } }
/* construit π : colonnes lues dans l'ordre ord (ord[q] = colonne lue en q-ième), haut→bas ou bas→haut */
static void buildpi(const Ord *o, int up, int *pi) {
  int w = o->w, h = (N + w - 1) / w, full = N % w ? N % w : w, t = 0;
  for (int q = 0; q < w; q++) { int c = o->ord[q], hc = c < full ? h : h - 1;
    for (int r = 0; r < hc; r++) { int rr = up ? hc - 1 - r : r; pi[rr * w + c] = t++; } }
}
int main(int argc, char **argv) {
  const char *wf = 0, *qf = 0; int NNULL = 20, EDEC = 3; uint64_t seed0 = 1000;
  for (int i = 1; i < argc; i++) { if (!strcmp(argv[i], "-w")) wf = argv[++i]; else if (!strcmp(argv[i], "-q")) qf = argv[++i];
    else if (!strcmp(argv[i], "-n")) NNULL = atoi(argv[++i]); else if (!strcmp(argv[i], "-s")) seed0 = strtoull(argv[++i], 0, 10);
    else if (!strcmp(argv[i], "-E")) EDEC = atoi(argv[++i]); }
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; int n = 0;
  for (int i = 0; i < 13; i++) { POS[n] = 21 + i; PT[n] = e[i] - 'A'; n++; }
  for (int i = 0; i < 11; i++) { POS[n] = 63 + i; PT[n] = b[i] - 'A'; n++; }
  NCT = 1 + NNULL; CTS = malloc(sizeof(*CTS) * NCT);
  const char *K4X = getenv("CTX") ? getenv("CTX") : K4;
  for (int i = 0; i < N; i++) CTS[0][i] = K4X[i] - 'A';
  for (int z = 1; z < NCT; z++) { memcpy(CTS[z], CTS[0], sizeof CTS[0]); uint64_t s = (seed0 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = N - 1; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CTS[z][i]; CTS[z][i] = CTS[z][j]; CTS[z][j] = t; } }
  { FILE *f = fopen(qf, "rb"); QG = malloc(456976 * 4); if (!f || fread(QG, 4, 456976, f) != 456976) return 2; fclose(f); }
  HS = calloc(HM + 1, 8);
  for (int w = 2; w <= 8; w++) { unsigned char a[20]; for (int i = 0; i < w; i++) a[i] = i; perms(w, a, 0); }
  long nperm = NORD;
  if (wf) { FILE *f = fopen(wf, "r"); char wd[128];
    while (fscanf(f, "%127s", wd) == 1) { int w = strlen(wd); if (w < 9 || w > 20) continue; unsigned char o[20]; int ok = 1;
      for (int i = 0; i < w; i++) { if (wd[i] >= 'a' && wd[i] <= 'z') wd[i] -= 32; if (wd[i] < 'A' || wd[i] > 'Z') ok = 0; }
      if (!ok) continue;
      /* ordre numérique du mot-clé (lettres égales numérotées de gauche à droite) : on lit d'abord la colonne de la plus petite lettre */
      int used[20] = {0}; for (int q = 0; q < w; q++) { int bi = -1; for (int i = 0; i < w; i++) if (!used[i] && (bi < 0 || wd[i] < wd[bi])) bi = i; used[bi] = 1; o[q] = bi; }
      addord(w, o); }
    fclose(f); }
  fprintf(stderr, "ordres de colonnes : %ld (permutations complètes w ≤ 8 : %ld)\n", NORD, nperm);
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  const int *YS[4] = {KAP, AZ, AZ, KAP}, *XS[4] = {KAP, AZ, KAP, AZ};
  enum { PMAXX = 13 };
  int (*be)[PMAXX + 1] = malloc(sizeof(*be) * NCT); float (*bs)[PMAXX + 1] = malloc(sizeof(*bs) * NCT);
  for (int z = 0; z < NCT; z++) for (int p = 0; p <= PMAXX; p++) { be[z][p] = 99; bs[z][p] = -99; }
  double t0 = omp_get_wtime();
  #pragma omp parallel
  {
    int (*le)[PMAXX + 1] = malloc(sizeof(*le) * NCT); float (*ls)[PMAXX + 1] = malloc(sizeof(*ls) * NCT);
    for (int z = 0; z < NCT; z++) for (int p = 0; p <= PMAXX; p++) { le[z][p] = 99; ls[z][p] = -99; }
    unsigned stp[PMAXX * 26] = {0}, stamp = 0; int cnt[PMAXX * 26];
    #pragma omp for schedule(dynamic, 64)
    for (long oi = 0; oi < NORD * 2; oi++) {
      int pi0[N], pi[N], inv[N]; buildpi(&ORD[oi >> 1], oi & 1, pi0);
      for (int iv = 0; iv < 2; iv++) {
        if (!iv) memcpy(pi, pi0, sizeof pi); else for (int i = 0; i < N; i++) pi[pi0[i]] = i;
        for (int i = 0; i < N; i++) inv[pi[i]] = i;
        for (int ty = 0; ty < 4; ty++) { const int *Y = YS[ty], *X = XS[ty]; int YI[26]; for (int i = 0; i < 26; i++) YI[Y[i]] = i;
          for (int z = 0; z < NCT; z++) { const int *CT = CTS[z];
            for (int mo = 0; mo < 2; mo++) { int K[NP];
              for (int t = 0; t < NP; t++) { int x = X[CT[pi[POS[t]]]], y = Y[PT[t]]; K[t] = mo == 0 ? md(x - y) : md(x + y); }
              for (int sf = 0; sf < 2; sf++) /* sf = 1 : substitution d'abord, phase = i ; sinon phase = π(i) */
                for (int p = 1; p <= PMAXX; p++) {
                  int m[PMAXX], bc[PMAXX], agree = 0; stamp++;
                  for (int j = 0; j < p; j++) { bc[j] = 0; m[j] = -1; }
                  for (int t = 0; t < NP; t++) { int ph = (sf ? POS[t] : pi[POS[t]]) % p, ix = ph * 26 + K[t];
                    if (stp[ix] != stamp) { stp[ix] = stamp; cnt[ix] = 0; }
                    if (++cnt[ix] > bc[ph]) { bc[ph] = cnt[ix]; m[ph] = K[t]; } }
                  for (int j = 0; j < p; j++) agree += bc[j];
                  int em = NP - agree; if (em < le[z][p]) le[z][p] = em;
                  if (em > EDEC) continue;
                  int miss = 0; for (int j = 0; j < p; j++) miss += m[j] < 0; if (miss) continue;
                  /* déchiffrement complet : clair i = f(chiffré π(i), clé de la phase) */
                  int P[N]; for (int i = 0; i < N; i++) { int k = m[(sf ? i : pi[i]) % p], x = X[CT[pi[i]]]; P[i] = YI[mo == 0 ? md(x - k) : md(k - x)]; }
                  double s = 0; for (int i = 0; i + 3 < N; i++) s += QG[((P[i] * 26 + P[i + 1]) * 26 + P[i + 2]) * 26 + P[i + 3]];
                  float sc = s / (N - 3); if (sc > ls[z][p]) ls[z][p] = sc;
                  if (z == 0 && (sc > -5.0 || em <= 1)) {
                    #pragma omp critical(pr)
                    { printf("K4 p=%d e=%d score %.3f w=%d ord=", p, em, sc, ORD[oi >> 1].w); for (int q = 0; q < ORD[oi >> 1].w; q++) printf("%d.", ORD[oi >> 1].ord[q]);
                      printf(" bas→haut=%d inv=%d type=%d mode=%d subst_dabord=%d : ", (int)(oi & 1), iv, ty, mo, sf); for (int i = 0; i < N; i++) putchar('A' + P[i]); putchar('\n'); }
                  }
                }
            } } } } (void)inv; }
    #pragma omp critical
    for (int z = 0; z < NCT; z++) for (int p = 0; p <= PMAXX; p++) { if (le[z][p] < be[z][p]) be[z][p] = le[z][p]; if (ls[z][p] > bs[z][p]) bs[z][p] = ls[z][p]; }
  }
  fprintf(stderr, "%.1f s\n", omp_get_wtime() - t0);
  printf("== résumé (%ld ordres × 2 routes × π, π⁻¹ ; %d témoins K4 mélangé, graine %llu ; score noté si e ≤ %d) ==\n", NORD, NNULL, (unsigned long long)seed0, EDEC);
  for (int p = 1; p <= PMAXX; p++) for (int k = 0; k < 2; k++) {
    float v[1024]; int nv = 0, better = 0; float k4 = k ? bs[0][p] : be[0][p];
    for (int z = 1; z < NCT; z++) { float x = k ? bs[z][p] : be[z][p]; v[nv++] = x; better += k ? x >= k4 : x <= k4; }
    for (int i = 0; i < nv; i++) for (int j = i + 1; j < nv; j++) if (v[j] < v[i]) { float x = v[i]; v[i] = v[j]; v[j] = x; }
    if (!nv) { printf("p=%2d %s K4 %7.3f\n", p, k ? "score max" : "e_min    ", k4); continue; }
    printf("p=%2d %s K4 %7.3f | témoins min %7.3f médiane %7.3f max %7.3f | p = %.3f\n", p, k ? "score max" : "e_min    ", k4, v[0], v[nv / 2], v[nv - 1], (better + 1.0) / (NNULL + 1));
  }
  return 0;
}
