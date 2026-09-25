/* t35_fautes_de_recopie.c — le « 7 » de K4 vient-il de FAUTES DE RECOPIE dans une grille de 7 colonnes ? (25/09/2026, soir)
 *
 * Idée (hors cribs). Le simulateur et la borne des doublets montrent qu'aucun chiffre déchiffrable ne produit l'empreinte
 * « 7 » de K4 : coïncidences à l'écart 7 seulement (pas 14, 21…) et doublets concentrés en colonne 4 → 5. En revanche,
 * une recopie à la main depuis une feuille de 7 colonnes la produit naturellement :
 *   - l'œil qui glisse sur la case du dessus recopie la lettre 7 rangs avant (écart 7, jamais 14) ;
 *   - la lettre précédente répétée au même endroit de chaque ligne (pli, blanc entre deux groupes) donne des doublets
 *     dans une seule colonne.
 * Le modèle désigne SANS AJUSTEMENT les lettres fautives : la seconde lettre de chaque répétition. Dans les cribs :
 *   22 (L, copie de 15), 72 (P, copie de 65), 26 (Q), 33 (S), 68 (T) (doublets) ⇒ masque M = {22, 26, 33, 68, 72}.
 * Si c'est vrai, le chiffre lui-même peut être SIMPLE (la feuille de 7 colonnes suggère la clé KRYPTOS, période 7).
 * Test : on retire M des cribs (19 lettres restent) et on reteste les clés périodiques p = 1..26,
 *   (A) alphabet QUELCONQUE (Quagmire III, VIG / BEAU / VARB, solveur exact lincsp) ;
 *   (B) alphabets à mot-clé (101 088), 5 types × VIG/BEAU, nombre minimal d'erreurs restantes (e_min exact).
 * Témoins : le même K4 avec 200 masques de 5 positions de crib tirés au hasard (pour (B)) ou 60 (pour (A)), et des chiffrés
 * aléatoires avec le masque M. Le masque M n'est significatif que s'il fait mieux que les masques au hasard.
 * Usage : t35 alphas.bin [NMASK]
 */
#define LC_MAXV 64
#define LC_THREADPRIVATE
#include "../vision_2026_09_24/lincsp.h"
#include <omp.h>

static const int MASKM[5] = {22, 26, 33, 68, 72};
static int solveA(const int *C24, const int *use, int p, int mode) {
  lc_reset(26 + p); lc_limit = 3000000;
  for (int t = 0; t < NCRIB; t++) { if (!use[t]) continue; int kv[1] = {26 + CRIBPOS[t] % p}; lc_add_enc(CRIBPT[t], C24[t], kv, 1, mode); }
  if (!lc_assign('E' - 'A', 0)) return 0;
  return lc_solve();
}
static void maskuse(const int *pos5, int *use) { for (int t = 0; t < NCRIB; t++) { use[t] = 1; for (int q = 0; q < 5; q++) if (CRIBPOS[t] == pos5[q]) use[t] = 0; } }
static void randmask(int *pos5) { int idx[NCRIB]; for (int t = 0; t < NCRIB; t++) idx[t] = t;
  for (int q = 0; q < 5; q++) { int j = q + rng() % (NCRIB - q); int x = idx[q]; idx[q] = idx[j]; idx[j] = x; pos5[q] = CRIBPOS[idx[q]]; } }

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  int NM = argc > 2 ? atoi(argv[2]) : 200;
  int C0[NCRIB]; for (int t = 0; t < NCRIB; t++) C0[t] = CT[CRIBPOS[t]];
  /* ---------- (A) alphabet quelconque ---------- */
  printf("(A) Quagmire III, alphabet quelconque, périodes 1..26, VIG/BEAU/VARB ; cas compatibles (sur 78)\n");
  int use[NCRIB]; maskuse(MASKM, use);
  int nA = 0; char list[4096] = ""; int ll = 0;
  for (int m = 0; m < 3; m++) for (int p = 1; p <= 26; p++) { int r = solveA(C0, use, p, m); if (r != 0) { nA++; ll += sprintf(list + ll, " %s p=%d%s", MODENAME[m], p, r < 0 ? "(?)" : ""); } }
  printf("  K4, masque M = {22,26,33,68,72} : %d cas :%s\n", nA, list);
  { int none[5] = {-1, -1, -1, -1, -1}; int u0[NCRIB]; maskuse(none, u0); int n0 = 0; ll = 0; list[0] = 0;
    for (int m = 0; m < 3; m++) for (int p = 1; p <= 26; p++) { int r = solveA(C0, u0, p, m); if (r != 0) { n0++; ll += sprintf(list + ll, " %s p=%d%s", MODENAME[m], p, r < 0 ? "(?)" : ""); } }
    printf("  K4, sans masque : %d cas :%s\n", n0, list); }
  int NMA = NM < 60 ? NM : 60, geA = 0; double sA = 0; int masks[512][5];
  for (int z = 0; z < NMA; z++) randmask(masks[z]);
  int cntA[512];
#pragma omp parallel for schedule(dynamic, 1)
  for (int z = 0; z < NMA; z++) { int u[NCRIB]; maskuse(masks[z], u); int n = 0;
    for (int m = 0; m < 3; m++) for (int p = 1; p <= 26; p++) n += solveA(C0, u, p, m) != 0; cntA[z] = n; }
  for (int z = 0; z < NMA; z++) { sA += cntA[z]; geA += cntA[z] >= nA; }
  printf("  K4, %d masques au hasard (5 positions de crib) : moyenne %.2f cas ; masques ≥ M : %d/%d\n", NMA, sA / NMA, geA, NMA);
  { int nr = 0; double sr = 0; int Cr[NCRIB];
    for (int z = 0; z < 30; z++) { for (int t = 0; t < NCRIB; t++) Cr[t] = rng() % 26; int n = 0;
      for (int m = 0; m < 3; m++) for (int p = 1; p <= 26; p++) n += solveA(Cr, use, p, m) != 0; sr += n; nr += n >= nA; }
    printf("  chiffrés aléatoires, masque M : moyenne %.2f cas ; ≥ K4 : %d/30\n", sr / 30, nr); }

  /* ---------- (B) alphabets à mot-clé, e_min exact ---------- */
  FILE *fa = fopen(argv[1], "rb"); fseek(fa, 0, SEEK_END); long n0 = ftell(fa) / 26; fseek(fa, 0, SEEK_SET);
  unsigned char *AL = malloc((n0 + 2) * 26); if (fread(AL, 26, n0, fa) != (size_t)n0) return 2; fclose(fa);
  memcpy(AL + n0 * 26, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26); memcpy(AL + (n0 + 1) * 26, "KRYPTOSABCDEFGHIJLMNQUVWXZ", 26); long na = n0 + 2;
  const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"; int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  /* masques : 0 = M, 1 = aucun, 2.. = au hasard */
  int NMB = NM + 2; int (*MK)[5] = malloc(sizeof(int[5]) * NMB);
  memcpy(MK[0], MASKM, sizeof MASKM); for (int q = 0; q < 5; q++) MK[1][q] = -1; for (int z = 2; z < NMB; z++) randmask(MK[z]);
  unsigned char (*US)[NCRIB] = malloc(NCRIB * NMB);
  for (int z = 0; z < NMB; z++) { int u[NCRIB]; maskuse(MK[z], u); for (int t = 0; t < NCRIB; t++) US[z][t] = u[t]; }
  /* pour chaque masque : e_min par période (1..26), et nombre de configurations à e = 0 pour p = 7 et p ≤ 13 */
  int (*best)[27] = malloc(sizeof(int[27]) * NMB); long *z7 = calloc(NMB, sizeof(long)), *z13 = calloc(NMB, sizeof(long));
  for (int z = 0; z < NMB; z++) for (int p = 0; p < 27; p++) best[z][p] = 99;
  printf("\n(B) alphabets à mot-clé : %ld × 5 types × VIG/BEAU, périodes 1..26 ; %d masques (M, aucun, %d au hasard)\n", na, NMB, NM);
#pragma omp parallel
  {
    int (*lb)[27] = malloc(sizeof(int[27]) * NMB); long *l7 = calloc(NMB, sizeof(long)), *l13 = calloc(NMB, sizeof(long));
    for (int z = 0; z < NMB; z++) for (int p = 0; p < 27; p++) lb[z][p] = 99;
#pragma omp for schedule(dynamic, 64)
    for (long a = 0; a < na; a++) {
      int S[26]; for (int i = 0; i < 26; i++) S[AL[a * 26 + i] - 'A'] = i;
      for (int ty = 0; ty < 5; ty++) {
        const int *Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
        const int *X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
        for (int mo = 0; mo < 2; mo++) {
          int K[NCRIB]; for (int t = 0; t < NCRIB; t++) { int x = X[C0[t]], y = Y[CRIBPT[t]]; K[t] = mo == 0 ? md(x - y) : md(x + y); }
          for (int p = 1; p <= 26; p++) {
            for (int z = 0; z < NMB; z++) {
              unsigned char cnt[26][26]; unsigned char used[26] = {0}; int sat = 0, nu = 0; unsigned char mx[26] = {0};
              for (int t = 0; t < NCRIB; t++) { if (!US[z][t]) continue; nu++; int j = CRIBPOS[t] % p;
                if (!used[j]) { used[j] = 1; memset(cnt[j], 0, 26); } if (++cnt[j][K[t]] > mx[j]) mx[j] = cnt[j][K[t]]; }
              for (int j = 0; j < p; j++) sat += mx[j];
              int e = nu - sat; if (e < lb[z][p]) lb[z][p] = e;
              if (e == 0) { if (p == 7) l7[z]++; if (p <= 13) l13[z]++; }
            }
          }
        }
      }
    }
#pragma omp critical
    for (int z = 0; z < NMB; z++) { for (int p = 0; p < 27; p++) if (lb[z][p] < best[z][p]) best[z][p] = lb[z][p]; z7[z] += l7[z]; z13[z] += l13[z]; }
    free(lb); free(l7); free(l13);
  }
  printf("  e_min par période (masque M, puis sans masque) :\n     p :"); for (int p = 1; p <= 26; p++) printf(" %2d", p); printf("\n");
  for (int z = 0; z < 2; z++) { printf("  %-4s:", z ? "aucun" : "M"); for (int p = 1; p <= 26; p++) printf(" %2d", best[z][p]); printf("\n"); }
  /* rang de M parmi les masques au hasard : somme des e_min pour p ≤ 13, et configurations à e = 0 */
  long sM = 0; for (int p = 1; p <= 13; p++) sM += best[0][p];
  int le = 0, ge7 = 0, ge13 = 0; double m7 = 0, m13 = 0, msum = 0; int bestp7[64] = {0};
  for (int z = 2; z < NMB; z++) { long s = 0; for (int p = 1; p <= 13; p++) s += best[z][p]; msum += s; le += s <= sM; ge7 += z7[z] >= z7[0]; ge13 += z13[z] >= z13[0]; m7 += z7[z]; m13 += z13[z]; bestp7[best[z][7] < 64 ? best[z][7] : 63]++; }
  printf("  Σ e_min (p ≤ 13) : M = %ld ; masques au hasard : moyenne %.2f ; ≤ M : %d/%d\n", sM, msum / NM, le, NM);
  printf("  configurations sans erreur, p = 7 : M = %ld ; hasard : moyenne %.1f ; ≥ M : %d/%d\n", z7[0], m7 / NM, ge7, NM);
  printf("  configurations sans erreur, p ≤ 13 : M = %ld ; hasard : moyenne %.1f ; ≥ M : %d/%d\n", z13[0], m13 / NM, ge13, NM);
  printf("  e_min à p = 7 pour les masques au hasard :"); for (int e = 0; e < 8; e++) printf(" e=%d:%d", e, bestp7[e]); printf("\n");
  return 0;
}
