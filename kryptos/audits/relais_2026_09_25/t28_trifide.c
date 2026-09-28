/* t28_trifide.c — trifide de Delastelle (cube 3×3×3, 27 symboles), période P et PHASE φ libres (relais du 25/09/2026)
 *
 * Idée. Le « 7 » de K4 (doublets en 18, 25, 32, 46, 67 ; coïncidences à l'écart 7) n'est pas une clé périodique. Un chiffre
 * FRACTIONNANT à période 7 en produit un sans clé : dans un bloc de 7, les lettres chiffrées 0 et 1 sont faites de la même
 * coordonnée (la « couche ») de six lettres claires, les lettres 5 et 6 de la colonne. Si les blocs commencent en φ = 4
 * (début de la première ligne pleine de K4 sur le cuivre), les doublets de K4 sont tous en tête de bloc, et chaque crib
 * contient un bloc entier (NORTHEA en 25–31, INCLOCK en 67–73). Le registre dit le trifide éliminé « pour toutes les
 * périodes » (E-S-09, E-S-42b, E-S-44) ; on vérifie ici toutes les PHASES, que ces preuves ne couvrent peut-être pas.
 *
 * Trifide standard : pour un bloc p_0..p_{P−1}, t(p) = (a, b, c) ∈ {0,1,2}³ ; suite S = a_0..a_{P−1} b_0..b_{P−1} c_0..c_{P−1} ;
 * lettre chiffrée j = t⁻¹(S[3j], S[3j+1], S[3j+2]). Le cube t est une bijection des 27 symboles (26 lettres + « # »).
 * Test exact : chaque lettre chiffrée d'un bloc qui touche les cribs impose, composante par composante, l'égalité de ses
 * trits avec ceux des lettres claires connues (une lettre claire inconnue laisse la composante libre). Union-find sur les
 * 81 trits, puis recherche d'un cube bijectif compatible (retour arrière, limite de nœuds).
 * Simulation : trifide de période 7 sur de l'anglais (fenêtres de 97 lettres du corpus), cubes aléatoires : doublets par
 * phase et coïncidences à l'écart 7, comparés à K4.
 * Usage : t28 corpus.txt [ntémoins]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int PTK[97]; /* clair connu ou −1 */

/* ---------- test exact ---------- */
typedef struct { int par[81]; } UF;
static int fnd(UF *u, int x) { while (u->par[x] != x) { u->par[x] = u->par[u->par[x]]; x = u->par[x]; } return x; }
static void uni(UF *u, int x, int y) { x = fnd(u, x); y = fnd(u, y); if (x != y) u->par[x] = y; }
typedef struct { UF u; int val[81]; int used[27]; int assigned[27]; long nodes, lim; int order[27], nord; } St;
static int tripof(St *s, int X, int *t) { int ok = 1; for (int m = 0; m < 3; m++) { t[m] = s->val[fnd(&s->u, X * 3 + m)]; if (t[m] < 0) ok = 0; } return ok; }
static int search(St *s, int k) {
  if (++s->nodes > s->lim) return -1;
  if (k == s->nord) return 1;
  int X = s->order[k];
  for (int tr = 0; tr < 27; tr++) {
    if (s->used[tr]) continue;
    int t[3] = {tr / 9, (tr / 3) % 3, tr % 3}, set[3] = {-1, -1, -1}, ok = 1;
    for (int m = 0; m < 3 && ok; m++) { int r = fnd(&s->u, X * 3 + m); if (s->val[r] < 0) { s->val[r] = t[m]; set[m] = r; } else if (s->val[r] != t[m]) ok = 0; }
    if (ok) {
      /* les symboles déjà placés gardent des triplets distincts ; ceux entièrement déterminés par les classes aussi */
      s->used[tr] = 1; s->assigned[X] = tr;
      for (int Y = 0; Y < 27 && ok; Y++) if (Y != X && s->assigned[Y] < 0) { int ty[3]; if (tripof(s, Y, ty)) { int q = ty[0] * 9 + ty[1] * 3 + ty[2]; if (s->used[q]) ok = 0; } }
      if (ok) { int r = search(s, k + 1); if (r) return r; }
      s->used[tr] = 0; s->assigned[X] = -1;
    }
    for (int m = 0; m < 3; m++) if (set[m] >= 0) s->val[set[m]] = -1;
  }
  return 0;
}
/* 1 = compatible, 0 = impossible, −1 = non tranché ; *ncons = nombre d'égalités posées */
static int trifid_ok(const int *ct, int P, int phi, long lim, int *ncons) {
  St s; for (int i = 0; i < 81; i++) { s.u.par[i] = i; s.val[i] = -1; }
  for (int i = 0; i < 27; i++) { s.used[i] = 0; s.assigned[i] = -1; }
  int nc = 0;
  for (int st = phi - P; st < 97; st += P) {
    int a = st < 0 ? 0 : st, b = st + P; if (b > 97) b = 97; int len = b - a;   /* bloc tronqué aux bords : période = longueur */
    int known = 0; for (int i = a; i < b; i++) if (PTK[i] >= 0) known = 1; if (!known) continue;
    for (int j = 0; j < len; j++) for (int m = 0; m < 3; m++) {
      int k = 3 * j + m, comp = k / len, idx = k % len, pl = PTK[a + idx];
      if (pl < 0) continue;
      uni(&s.u, ct[a + j] * 3 + m, pl * 3 + comp); nc++;
    }
  }
  *ncons = nc;
  /* deux symboles distincts aux trois composantes liées : contradiction immédiate */
  for (int X = 0; X < 27; X++) for (int Y = X + 1; Y < 27; Y++) {
    int same = 1; for (int m = 0; m < 3; m++) if (fnd(&s.u, X * 3 + m) != fnd(&s.u, Y * 3 + m)) same = 0;
    if (same) return 0; }
  /* ordre : symboles les plus contraints d'abord */
  int deg[27] = {0}; for (int X = 0; X < 27; X++) for (int m = 0; m < 3; m++) { int r = fnd(&s.u, X * 3 + m);
    for (int Y = 0; Y < 27; Y++) for (int q = 0; q < 3; q++) if ((Y != X || q != m) && fnd(&s.u, Y * 3 + q) == r) deg[X]++; }
  s.nord = 0; for (int X = 0; X < 27; X++) s.order[s.nord++] = X;
  for (int i = 0; i < 27; i++) for (int j = i + 1; j < 27; j++) if (deg[s.order[j]] > deg[s.order[i]]) { int t = s.order[i]; s.order[i] = s.order[j]; s.order[j] = t; }
  s.nodes = 0; s.lim = lim;
  return search(&s, 0);
}

/* ---------- chiffrement (simulation, contrôle positif) ---------- */
static void trifid_enc(const int *pt, int n, const int *cube /* symbole -> triplet 0..26 */, const int *inv, int P, int phi, int *ct) {
  for (int st = phi - P; st < n; st += P) {
    int a = st < 0 ? 0 : st, b = st + P; if (b > n) b = n; int len = b - a; if (len <= 0) continue;
    int S[3 * 64];
    for (int j = 0; j < len; j++) { int t = cube[pt[a + j]]; S[j] = t / 9; S[len + j] = (t / 3) % 3; S[2 * len + j] = t % 3; }
    for (int j = 0; j < len; j++) ct[a + j] = inv[S[3 * j] * 9 + S[3 * j + 1] * 3 + S[3 * j + 2]];
  }
}
static uint64_t rs = 88172645463325252ULL;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static void randcube(int *cube, int *inv) { int p[27]; for (int i = 0; i < 27; i++) p[i] = i;
  for (int i = 26; i > 0; i--) { int j = rnd() % (i + 1); int x = p[i]; p[i] = p[j]; p[j] = x; }
  for (int X = 0; X < 27; X++) { cube[X] = p[X]; inv[p[X]] = X; } }

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  for (int i = 0; i < 97; i++) PTK[i] = -1;
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) PTK[21 + i] = e[i] - 'A'; for (int i = 0; i < 11; i++) PTK[63 + i] = b[i] - 'A';
  int ct[97]; for (int i = 0; i < 97; i++) ct[i] = K4[i] - 'A';
  int NN = argc > 2 ? atoi(argv[2]) : 200;
  /* corpus */
  FILE *f = fopen(argv[1], "r"); fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
  char *raw = malloc(n + 1); if (fread(raw, 1, n, f) != (size_t)n) return 2; fclose(f);
  int *txt = malloc(sizeof(int) * n); long nt = 0; for (long i = 0; i < n; i++) { int c = raw[i]; if (c >= 'a' && c <= 'z') c -= 32; if (c >= 'A' && c <= 'Z') txt[nt++] = c - 'A'; }

  /* 1. simulation : trifide période 7 sur de l'anglais */
  { long NS = 200000; long dphase[7] = {0}, ndbl = 0, lag7 = 0; long sig = 0, sig_d = 0;
    for (long r = 0; r < NS; r++) {
      int cube[27], inv[27], pt[97], c[97]; randcube(cube, inv); long o = rnd() % (nt - 100); for (int i = 0; i < 97; i++) pt[i] = txt[o + i];
      trifid_enc(pt, 97, cube, inv, 7, 0, c);
      int cnt[7] = {0}, nd = 0, l7 = 0;
      for (int i = 0; i < 96; i++) if (c[i] == c[i + 1]) { cnt[i % 7]++; nd++; dphase[i % 7]++; ndbl++; }
      for (int i = 0; i + 7 < 97; i++) if (c[i] == c[i + 7]) l7++;
      lag7 += l7; int mx = 0; for (int j = 0; j < 7; j++) if (cnt[j] > mx) mx = cnt[j];
      if (nd >= 4 && mx >= nd - 1) sig_d++;
      if (nd >= 4 && mx >= nd - 1 && l7 >= 9) sig++;
    }
    printf("SIMULATION trifide période 7 (blocs en 0), %ld textes anglais de 97 lettres, cubes aléatoires :\n", NS);
    printf("  doublets par phase (i mod 7 du premier) :"); for (int j = 0; j < 7; j++) printf(" %d:%.3f", j, (double)dphase[j] / NS); printf("  (total %.2f par texte)\n", (double)ndbl / NS);
    printf("  coïncidences à l'écart 7 : %.2f par texte (K4 : 9)\n", (double)lag7 / NS);
    printf("  « au moins 4 doublets, tous sauf un dans une même phase » : %.4f ; et en plus ≥ 9 coïncidences à l'écart 7 : %.5f\n", (double)sig_d / NS, (double)sig / NS);
  }

  /* 2. contrôle positif : trifide période 7, phase 4, cube aléatoire, clair anglais avec les cribs */
  { int ok = 0, tot = 0;
    for (int r = 0; r < 20; r++) {
      int cube[27], inv[27], pt[97], c[97]; randcube(cube, inv); long o = rnd() % (nt - 100);
      for (int i = 0; i < 97; i++) pt[i] = PTK[i] >= 0 ? PTK[i] : txt[o + i];
      trifid_enc(pt, 97, cube, inv, 7, 4, c); int nc; int v = trifid_ok(c, 7, 4, 20000000, &nc); ok += v == 1; tot++;
    }
    printf("contrôle positif (P = 7, φ = 4) : %d/%d reconnus\n", ok, tot);
  }

  /* 3. K4 : toutes périodes 2..40, toutes phases ; témoins : chiffrés aléatoires et K4 mélangé */
  printf("K4 — trifide, périodes 2..40, toutes phases :\n");
  long tot = 0, comp = 0, und = 0; double expU = 0, expS = 0;
#pragma omp parallel for schedule(dynamic) reduction(+:tot,comp,und,expU,expS)
  for (int idx = 0; idx < 39 * 40; idx++) {
    int P = idx / 40 + 2, phi = idx % 40; if (phi >= P) continue;
    int nc, r = trifid_ok(ct, P, phi, 20000000, &nc);
    uint64_t loc = 0x9E3779B97F4A7C15ULL * (idx + 1); int pu = 0, ps = 0, zu = 0;
    for (int z = 0; z < NN; z++) {
      int c2[97], c3[97];
      for (int i = 0; i < 97; i++) { loc ^= loc << 13; loc ^= loc >> 7; loc ^= loc << 17; c2[i] = loc % 26; c3[i] = ct[i]; }
      for (int i = 96; i > 0; i--) { loc ^= loc << 13; loc ^= loc >> 7; loc ^= loc << 17; int j = loc % (i + 1); int x = c3[i]; c3[i] = c3[j]; c3[j] = x; }
      int n2, v2 = trifid_ok(c2, P, phi, 2000000, &n2), v3 = trifid_ok(c3, P, phi, 2000000, &n2);
      pu += v2 == 1; ps += v3 == 1; zu += v2 < 0;
    }
    tot++; if (r == 1) comp++; if (r < 0) und++;
    expU += (double)pu / NN; expS += (double)ps / NN;
    if (r != 0 || P == 7) {
#pragma omp critical
      printf("  P=%d φ=%d : %d égalités ; K4 %s ; témoins aléatoires %d/%d, K4 mélangé %d/%d (non tranchés %d)\n", P, phi, nc,
             r == 1 ? "COMPATIBLE" : r == 0 ? "impossible" : "NON TRANCHÉ", pu, NN, ps, NN, zu);
    }
  }
  printf("TOTAL : %ld cas ; K4 compatible %ld, non tranché %ld ; attendu au hasard : %.1f (aléatoire), %.1f (K4 mélangé)\n", tot, comp, und, expU, expS);
  return 0;
}
