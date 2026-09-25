/* k4coord.c — autoclé sur le clair à l'écart 7 « dans une autre base » (proposition du 25/09) :
 * les lettres sont placées dans une grille R × C (R·C = 26 : 2 × 13 ou 13 × 2) remplie par un alphabet σ ;
 * on additionne séparément la ligne (mod R) et la colonne (mod C) :
 *   chiffré = case( ligne(p) ⊕ ligne(k), colonne(p) ⊕ colonne(k) ), avec k = clair[i − 7]
 * ⊕ : VIG (p + k), BEAU (k − p), VARB (p − k), la même sur les deux coordonnées.
 * La lettre-clé est placée dans la même grille, ou dans la grille A–Z, ou KRYPTOS.
 * Les cribs donnent 10 équations directes (i = 28–33 et 70–73, où clair[i] et clair[i − 7] sont connus).
 * (Une grille de 25 cases, Polybe 5 × 5, est exclue d'emblée : K4 contient les 26 lettres.)
 * Sortie : plus petit nombre d'équations fausses, K4 et témoins K4 mélangé.
 * Usage : k4coord alphas.bin NNULL   (CTX = chiffré de contrôle)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "rb"); fseek(f, 0, SEEK_END); long na = ftell(f) / 26; fseek(f, 0, SEEK_SET);
  char *AB = malloc(na * 26); if (fread(AB, 26, na, f) != (size_t)na) return 1; fclose(f);
  int NN = atoi(argv[2]), NCT = NN + 1; int (*CT)[97] = malloc(sizeof(*CT) * NCT);
  const char *c0 = getenv("CTX") ? getenv("CTX") : K4; for (int i = 0; i < 97; i++) CT[0][i] = c0[i] - 'A';
  for (int z = 1; z < NCT; z++) { memcpy(CT[z], CT[0], sizeof CT[0]); uint64_t s = (5000 + z) * 0x9E3779B97F4A7C15ULL;
    for (int i = 96; i > 0; i--) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; int j = s % (i + 1); int t = CT[z][i]; CT[z][i] = CT[z][j]; CT[z][j] = t; } }
  int P[97]; memset(P, -1, sizeof P); const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) P[21 + i] = e[i] - 'A'; for (int i = 0; i < 11; i++) P[63 + i] = b[i] - 'A';
  int EQ[16], ne = 0; for (int i = 7; i < 97; i++) if (P[i] >= 0 && P[i - 7] >= 0) EQ[ne++] = i;
  fprintf(stderr, "%d équations directes, %ld alphabets, %d chiffrés\n", ne, na, NCT);
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  int *best = malloc(sizeof(int) * NCT); for (int z = 0; z < NCT; z++) best[z] = 99;
  const int SH[2][2] = {{2, 13}, {13, 2}};
  #pragma omp parallel
  { int *lb = malloc(sizeof(int) * NCT); for (int z = 0; z < NCT; z++) lb[z] = 99;
    #pragma omp for schedule(dynamic, 64)
    for (long ai = 0; ai < na * 2; ai++) {
      int S[26], SI[26]; const char *a = AB + (ai >> 1) * 26; int rev = ai & 1;
      for (int i = 0; i < 26; i++) { S[(rev ? a[25 - i] : a[i]) - 'A'] = i; }
      for (int x = 0; x < 26; x++) SI[S[x]] = x;
      for (int sh = 0; sh < 2; sh++) { int R = SH[sh][0], C = SH[sh][1];
        for (int kr = 0; kr < 3; kr++) { const int *Z = kr == 0 ? S : kr == 1 ? AZ : KAP; /* rang de la lettre-clé */
          for (int mo = 0; mo < 3; mo++) for (int z = 0; z < NCT; z++) { int err = 0;
            for (int q = 0; q < ne && err < lb[z]; q++) { int i = EQ[q], sp = S[P[i]], sk = Z[P[i - 7]];
              int rp = sp / C, cp = sp % C, rk = sk / C, ck = sk % C, r, c;
              if (mo == 0) { r = (rp + rk) % R; c = (cp + ck) % C; } else if (mo == 1) { r = (rk - rp + R) % R; c = (ck - cp + C) % C; } else { r = (rp - rk + R) % R; c = (cp - ck + C) % C; }
              if (SI[r * C + c] != CT[z][i]) err++; }
            if (err < lb[z]) { lb[z] = err; if (z == 0 && err <= 2) {
                #pragma omp critical
                printf("K4 : %d fausses sur %d, alph=%.26s sens=%d grille=%dx%d clé lue=%d conv=%d\n", err, ne, a, rev, R, C, kr, mo); } } } } } }
    #pragma omp critical
    for (int z = 0; z < NCT; z++) if (lb[z] < best[z]) best[z] = lb[z];
    free(lb); }
  int ge = 0, mn = 99, mx = 0; double s = 0; for (int z = 1; z < NCT; z++) { ge += best[z] <= best[0]; if (best[z] < mn) mn = best[z]; if (best[z] > mx) mx = best[z]; s += best[z]; }
  printf("== autoclé écart 7 en grille 2 × 13 / 13 × 2 : K4 au mieux %d fausses sur %d | témoins min %d moyenne %.2f max %d | p = %.3f\n", best[0], ne, mn, s / NN, mx, (ge + 1.0) / (NN + 1));
  return 0;
}
