/* k4s.c — T3 : clé périodique (p = 1..14) avec le modèle d'erreur observé chez Sanborn (petit fragment de 97 lettres).
 * Une lettre des cribs est « juste » si sa clé vaut m[phase]. Une erreur est « à la Sanborn » si :
 *   - glissement : la clé employée est celle d'une phase voisine, m[phase ± 1] (erreurs 9 et 87 du fragment) ;
 *   - copie : le clair a été gravé tel quel, chiffré = clair (erreur 22) ;
 *   - décalage commun : clé = m[phase] + δ avec le même δ pour au moins deux erreurs (9, 22, 87 : même écart −5).
 * Pour chaque alphabet, type (Q3, Q2, Q1, Q4a, Q4b) et convention, et pour chaque période, on calcule :
 *   e    : e_min sans restriction (majorité par phase) ;
 *   eS0  : nombre minimal d'erreurs si TOUTES sont à la Sanborn (99 si impossible avec ≤ SMAX erreurs) ;
 *   eS1  : idem en admettant au plus une erreur quelconque (l'erreur 91 du fragment n'a pas de lecture simple).
 * Invariance : inverser l'alphabet ou passer à VARB change K en ±K + c ; le modèle est invariant, on n'essaie que VIG/BEAU à l'endroit.
 * Une phase sans lettre juste a une clé libre : on l'accepte alors pour tout glissement (léger excès d'indulgence, commun à K4 et aux témoins).
 * Usage : k4s -a alphas.bin -n NNULL [-s graine] [-S SMAX]
 * Env : CTX = chiffré de 97 lettres ; CRIBS = "21:EASTNORTHEAST,63:BERLINCLOCK" (défaut) ; PMAX = 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#define MAXP 40
#define PMX 26
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int NP, POS[MAXP], PT[MAXP], NCT, PMAX = 14, SMAX = 3;
static int (*CTS)[97];
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
/* e_min par majorité */
static int emaj(const int *K, int p, int *m) {
  int agree = 0;
  for (int j = 0; j < p; j++) { int cnt[26] = {0}, bc = 0, bv = -1;
    for (int t = 0; t < NP; t++) if (POS[t] % p == j && ++cnt[K[t]] > bc) { bc = cnt[K[t]]; bv = K[t]; }
    agree += bc; if (m) m[j] = bv; }
  return NP - agree;
}
/* le sous-ensemble E (masque) d'erreurs est-il admissible ? renvoie le nombre minimal d'erreurs « quelconques » nécessaires */
static int check(const int *K, const int *cp, int p, uint64_t E) {
  int m[PMX]; for (int j = 0; j < p; j++) m[j] = -1;
  for (int t = 0; t < NP; t++) if (!(E >> t & 1)) { int j = POS[t] % p; if (m[j] < 0) m[j] = K[t]; else if (m[j] != K[t]) return 99; }
  int best = 99;
  for (int dl = 0; dl < 26; dl++) { int free = 0, nd = 0; /* dl = 0 : pas de décalage commun ; sinon il doit servir à au moins 2 erreurs */
    for (int t = 0; t < NP; t++) if (E >> t & 1) { int j = POS[t] % p, a = (j + p - 1) % p, b = (j + 1) % p;
      if (cp[t]) continue;
      if (p > 1 && (m[a] < 0 || m[a] == K[t] || m[b] < 0 || m[b] == K[t])) continue;
      if (dl && m[j] >= 0 && md(K[t] - m[j]) == dl) { nd++; continue; }
      free++; }
    if (nd == 1) free++;
    if (free < best) best = free; if (!best) break; }
  return best;
}
static void sanb(const int *K, const int *cp, int p, int e, int *eS0, int *eS1) {
  *eS0 = *eS1 = 99; if (e > SMAX) return;
  /* énumère les sous-ensembles de taille k = e..SMAX */
  for (int k = e; k <= SMAX && *eS0 == 99; k++) {
    int idx[8]; for (int i = 0; i < k; i++) idx[i] = i;
    while (1) { uint64_t E = 0; for (int i = 0; i < k; i++) E |= 1ULL << idx[i];
      int f = check(K, cp, p, E);
      if (f == 0) { *eS0 = k; if (*eS1 > k) *eS1 = k; break; }
      if (f == 1 && *eS1 > k) *eS1 = k;
      int i = k - 1; while (i >= 0 && idx[i] == NP - k + i) i--; if (i < 0) break; idx[i]++; for (int q = i + 1; q < k; q++) idx[q] = idx[q - 1] + 1; }
  }
}
int main(int argc, char **argv) {
  const char *afile = 0; int NNULL = 20; uint64_t seed0 = 1000;
  for (int i = 1; i < argc; i++) { if (!strcmp(argv[i], "-a")) afile = argv[++i]; else if (!strcmp(argv[i], "-n")) NNULL = atoi(argv[++i]);
    else if (!strcmp(argv[i], "-s")) seed0 = strtoull(argv[++i], 0, 10); else if (!strcmp(argv[i], "-S")) SMAX = atoi(argv[++i]); }
  if (getenv("PMAX")) PMAX = atoi(getenv("PMAX"));
  const char *cr = getenv("CRIBS") ? getenv("CRIBS") : "21:EASTNORTHEAST,63:BERLINCLOCK";
  NP = 0; { const char *s = cr; while (*s) { int p0 = atoi(s); s = strchr(s, ':') + 1; int i = 0; while (*s >= 'A' && *s <= 'Z') { POS[NP] = p0 + i; PT[NP] = *s - 'A'; NP++; i++; s++; } if (*s == ',') s++; } }
  NCT = 1 + NNULL; CTS = malloc(sizeof(*CTS) * NCT);
  const char *K4X = getenv("CTX") ? getenv("CTX") : K4;
  for (int i = 0; i < 97; i++) CTS[0][i] = K4X[i] - 'A';
  for (int z = 1; z < NCT; z++) { memcpy(CTS[z], CTS[0], sizeof CTS[0]); uint64_t s = (seed0 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CTS[z][i]; CTS[z][i] = CTS[z][j]; CTS[z][j] = t; } }
  FILE *af = fopen(afile, "rb"); if (!af) return 4; fseek(af, 0, SEEK_END); long na = ftell(af) / 26; fseek(af, 0, SEEK_SET);
  char *AB = malloc(na * 26); if (fread(AB, 26, na, af) != (size_t)na) return 3; fclose(af);
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  /* best[z][p][0..2] = e, eS0, eS1 */
  int (*best)[PMX + 1][3] = malloc(sizeof(*best) * NCT);
  for (int z = 0; z < NCT; z++) for (int p = 0; p <= PMX; p++) for (int k = 0; k < 3; k++) best[z][p][k] = 99;
  double t0 = omp_get_wtime();
  #pragma omp parallel
  {
    int (*lb)[PMX + 1][3] = malloc(sizeof(*lb) * NCT);
    for (int z = 0; z < NCT; z++) for (int p = 0; p <= PMX; p++) for (int k = 0; k < 3; k++) lb[z][p][k] = 99;
    #pragma omp for schedule(dynamic, 16)
    for (long ai = 0; ai < na; ai++) {
      int S[26]; const char *a = AB + ai * 26; for (int i = 0; i < 26; i++) S[a[i] - 'A'] = i;
      for (int ty = 0; ty < 5; ty++) {
        const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
        const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
        for (int z = 0; z < NCT; z++) { const int *CT = CTS[z]; int cp[MAXP];
          for (int t = 0; t < NP; t++) cp[t] = CT[POS[t]] == PT[t];
          for (int mo = 0; mo < 2; mo++) { int K[MAXP];
            for (int t = 0; t < NP; t++) { int x = X[CT[POS[t]]], y = Y[PT[t]]; K[t] = mo == 0 ? md(x - y) : md(x + y); }
            for (int p = 1; p <= PMAX; p++) { int e = emaj(K, p, 0), s0, s1;
              if (e < lb[z][p][0]) lb[z][p][0] = e;
              if (e > SMAX || (e >= lb[z][p][1] && e >= lb[z][p][2] && z)) continue;
              sanb(K, cp, p, e, &s0, &s1);
              if (s0 < lb[z][p][1]) lb[z][p][1] = s0; if (s1 < lb[z][p][2]) lb[z][p][2] = s1;
              if (z == 0 && (s0 <= 2 || s1 <= 1)) {
                #pragma omp critical(pr)
                printf("K4 p=%d e=%d eS0=%d eS1=%d alph=%.26s type=%d mode=%d\n", p, e, s0, s1, a, ty, mo); }
            } } } } }
    #pragma omp critical
    for (int z = 0; z < NCT; z++) for (int p = 0; p <= PMX; p++) for (int k = 0; k < 3; k++) if (lb[z][p][k] < best[z][p][k]) best[z][p][k] = lb[z][p][k];
    free(lb);
  }
  fprintf(stderr, "alphabets %ld, %d chiffrés, %d lettres de crib, %.1f s\n", na, NCT, NP, omp_get_wtime() - t0);
  printf("== résumé (%ld alphabets, %d témoins K4 mélangé, graine %llu, SMAX %d) ==\n", na, NNULL, (unsigned long long)seed0, SMAX);
  const char *nm[3] = {"e_min", "eS0 (toutes à la Sanborn)", "eS1 (≤ 1 quelconque)"};
  for (int p = 1; p <= PMAX; p++) for (int k = 0; k < 3; k++) {
    int v[1024], nv = 0, better = 0;
    for (int z = 1; z < NCT; z++) { v[nv++] = best[z][p][k]; better += best[z][p][k] <= best[0][p][k]; }
    for (int i = 0; i < nv; i++) for (int j = i + 1; j < nv; j++) if (v[j] < v[i]) { int x = v[i]; v[i] = v[j]; v[j] = x; }
    if (!nv) { printf("p=%2d %-28s K4 %2d\n", p, nm[k], best[0][p][k]); continue; }
    printf("p=%2d %-28s K4 %2d | témoins min %2d médiane %2d max %2d | p = %.3f\n", p, nm[k], best[0][p][k], v[0], v[nv / 2], v[nv - 1], (better + 1.0) / (NNULL + 1));
  }
  return 0;
}
