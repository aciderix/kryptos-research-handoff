/* t2_chain.c — clés par ADDITION EN CHAÎNE (famille Gromark / VIC), bases 10 et 26, alphabet inconnu.
 *
 * « Simple, mémorisable, exécutable des années plus tard avec le bon mot-clé » (Scheidt 2011) ;
 * « changer la base du langage » (Scheidt 2020) ; addition en chaîne = cœur du VIC soviétique
 * et du Gromark (Bean 2021). Bean a traité la base 10, amorce de 5, DEUX alphabets libres (39 amorces).
 * Cases non couvertes, fixées AVANT calcul :
 *   règles : R1 k[i] = k[i-n] + k[i-n+1] (Gromark) ; R2 k[i] = k[i-n] + k[i-1] (Fibonacci décalé) ;
 *            R3 k[i] = k[i-n] - k[i-n+1] ; R4 k[i] = k[i-n] + k[i-n+1] + k[i-1]  (toutes mod B)
 *   bases B = 10 (amorces de 2 à 6 chiffres) et B = 26 (amorces de 2 à 5 lettres) ;
 *   amorce en position 0 de K4 (les règles sont inversibles : tout décalage revient à une autre amorce) ;
 *   juges : (a) QIII σ quelconque (même alphabet clair/chiffré, comme K1–K2), VIG/BEAU/VARB — eng_num
 *           (b) QIV : deux alphabets quelconques — eng_num4 (peu puissant ; comparé au témoin)
 *   témoin : mêmes balayages sur chiffrés aléatoires (taux de passage).
 */
#include "k4lib.h"
#include <time.h>

static int P24[NCRIB], C24[NCRIB];

/* QIV : σC(c) = σP(p) + k ; nœuds 0..25 = lettres claires, 26..51 = lettres chiffrées */
static int q4_ncomp; static int q4_nopt[52]; static uint64_t q4_opt[52][26];
static int q4_place(int ci, uint32_t uP, uint32_t uC) {
  if (ci == q4_ncomp) return 1;
  for (int j = 0; j < q4_nopt[ci]; j++) {
    uint32_t mP = (uint32_t)q4_opt[ci][j], mC = (uint32_t)(q4_opt[ci][j] >> 32);
    if (!(mP & uP) && !(mC & uC) && q4_place(ci + 1, uP | mP, uC | mC)) return 1;
  }
  return 0;
}
static int eng_num4(const int *p, const int *c, const int *k, int n) {
  int adjn[52] = {0}, adj[52][32][2], present[52] = {0};
  for (int t = 0; t < n; t++) {
    int a = p[t], b = 26 + c[t], off = md(k[t]);
    adj[a][adjn[a]][0] = b; adj[a][adjn[a]][1] = off; adjn[a]++;
    adj[b][adjn[b]][0] = a; adj[b][adjn[b]][1] = md(-off); adjn[b]++;
    present[a] = present[b] = 1;
  }
  int ofs[52], seen[52] = {0}; q4_ncomp = 0;
  for (int r = 0; r < 52; r++) {
    if (!present[r] || seen[r]) continue;
    int comp[52], nc = 0, q = 0; seen[r] = 1; ofs[r] = 0; comp[nc++] = r;
    while (q < nc) {
      int u = comp[q++];
      for (int j = 0; j < adjn[u]; j++) {
        int v = adj[u][j][0], no = md(ofs[u] + adj[u][j][1]);
        if (!seen[v]) { seen[v] = 1; ofs[v] = no; comp[nc++] = v; }
        else if (ofs[v] != no) return 0;
      }
    }
    uint32_t mP = 0, mC = 0;
    for (int i = 0; i < nc; i++) {
      int v = comp[i];
      if (v < 26) { if (mP >> ofs[v] & 1) return 0; mP |= 1u << ofs[v]; }
      else { if (mC >> ofs[v] & 1) return 0; mC |= 1u << ofs[v]; }
    }
    int no = 0;
    for (int x = 0; x < 26; x++) {
      uint32_t rp = ((mP << x) | (mP >> (26 - x))) & 0x3FFFFFF, rc = ((mC << x) | (mC >> (26 - x))) & 0x3FFFFFF;
      if (x == 0) { rp = mP; rc = mC; }
      q4_opt[q4_ncomp][no++] = (uint64_t)rp | ((uint64_t)rc << 32);
    }
    q4_nopt[q4_ncomp++] = no;
  }
  /* la 1re composante peut être fixée (translation globale) */
  if (q4_ncomp) q4_nopt[0] = 1;
  return q4_place(0, 0, 0);
}

static int gen_key(int rule, int B, int n, const int *primer, int *k) {
  for (int i = 0; i < n; i++) k[i] = primer[i];
  for (int i = n; i < K4LEN; i++) {
    int v;
    switch (rule) {
      case 1: v = k[i - n] + k[i - n + 1]; break;
      case 2: v = k[i - n] + k[i - 1]; break;
      case 3: v = k[i - n] - k[i - n + 1]; break;
      default: v = k[i - n] + k[i - n + 1] + k[i - 1]; break;
    }
    v %= B; if (v < 0) v += B; k[i] = v;
  }
  return 0;
}

typedef struct { long tried, q3[3], q4; } Res;

static void sweep(int rule, int B, int n, Res *R, int verbose) {
  long total = 1; for (int i = 0; i < n; i++) total *= B;
  int primer[8], k[K4LEN], k24[NCRIB];
  for (long idx = 0; idx < total; idx++) {
    long x = idx; for (int i = n - 1; i >= 0; i--) { primer[i] = x % B; x /= B; }
    gen_key(rule, B, n, primer, k);
    for (int t = 0; t < NCRIB; t++) k24[t] = k[CRIBPOS[t]];
    R->tried++;
    for (int m = 0; m < 3; m++) if (eng_num(P24, C24, k24, NCRIB, m)) {
      R->q3[m]++;
      if (verbose) { printf("HIT QIII %s rule=%d B=%d primer=", MODENAME[m], rule, B); for (int i = 0; i < n; i++) printf("%d%s", primer[i], i < n - 1 ? "," : ""); printf("\n"); }
    }
    if (eng_num4(P24, C24, k24, NCRIB)) R->q4++;
  }
}

int main(int argc, char **argv) {
  int NNULL = argc > 1 ? atoi(argv[1]) : 3;
  k4_init();
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  /* contrôle : l'amorce 26717 de Bean (base 10, R1) doit passer en QIV */
  { int pr[5] = {2, 6, 7, 1, 7}, k[K4LEN], k24[NCRIB]; gen_key(1, 10, 5, pr, k); for (int t = 0; t < NCRIB; t++) k24[t] = k[CRIBPOS[t]];
    printf("contrôle Bean 26717 QIV : %d (attendu 1) ; QIII VIG/BEAU/VARB : %d %d %d\n", eng_num4(P24, C24, k24, NCRIB),
           eng_num(P24, C24, k24, NCRIB, 0), eng_num(P24, C24, k24, NCRIB, 1), eng_num(P24, C24, k24, NCRIB, 2)); }
  /* contrôle positif QIII : faux K4 construit avec une chaîne base 26 R1 amorce (3,1,4,1) et σ aléatoire */
  {
    int s[26], inv[26]; for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int t = s[i]; s[i] = s[j]; s[j] = t; }
    for (int i = 0; i < 26; i++) inv[s[i]] = i;
    int pr[4] = {3, 1, 4, 1}, k[K4LEN], k24[NCRIB], fc[NCRIB]; gen_key(1, 26, 4, pr, k);
    for (int t = 0; t < NCRIB; t++) { k24[t] = k[CRIBPOS[t]]; fc[t] = inv[md(s[P24[t]] + k24[t])]; }
    printf("contrôle positif QIII base 26 : %d (attendu 1)\n", eng_num(P24, fc, k24, NCRIB, VIG));
  }
  int configs[][3] = { {1,10,2},{1,10,3},{1,10,4},{1,10,5},{1,10,6},{2,10,3},{2,10,4},{2,10,5},{2,10,6},{3,10,4},{3,10,5},{3,10,6},{4,10,4},{4,10,5},{4,10,6},
                       {1,26,2},{1,26,3},{1,26,4},{1,26,5},{2,26,2},{2,26,3},{2,26,4},{2,26,5},{3,26,3},{3,26,4},{3,26,5},{4,26,3},{4,26,4},{4,26,5} };
  int nconf = sizeof configs / sizeof configs[0];
  for (int ci = 0; ci < nconf; ci++) {
    int rule = configs[ci][0], B = configs[ci][1], n = configs[ci][2];
    clock_t t0 = clock();
    for (int t = 0; t < NCRIB; t++) C24[t] = CT[CRIBPOS[t]];
    Res R = {0}; sweep(rule, B, n, &R, 1);
    double dt = (double)(clock() - t0) / CLOCKS_PER_SEC;
    /* témoins : chiffrés aléatoires (seulement les 24 lettres comptent) */
    long nq3 = 0, nq4 = 0, ntr = 0;
    int nn = (B == 26 && n == 5) || (B == 10 && n == 6) ? 1 : NNULL;
    for (int z = 0; z < nn; z++) {
      for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
      Res Z = {0}; sweep(rule, B, n, &Z, 0);
      nq3 += Z.q3[0] + Z.q3[1] + Z.q3[2]; nq4 += Z.q4; ntr += Z.tried;
    }
    /* témoin CLÉ : mêmes cribs de K4, clés i.i.d. dans la base (aucune structure de chaîne) */
    long kq3 = 0, kq4 = 0, KN = R.tried < 400000 ? 400000 : R.tried;
    for (int t = 0; t < NCRIB; t++) C24[t] = CT[CRIBPOS[t]];
    for (long z = 0; z < KN; z++) {
      int k24[NCRIB]; for (int t = 0; t < NCRIB; t++) k24[t] = rng() % B;
      for (int m = 0; m < 3; m++) kq3 += eng_num(P24, C24, k24, NCRIB, m);
      kq4 += eng_num4(P24, C24, k24, NCRIB);
    }
    printf("   clés i.i.d. base %d sur les cribs de K4 : attendu QIII %.3f, QIV %.1f pour %ld clés\n", B, (double)kq3 * R.tried / KN, (double)kq4 * R.tried / KN, R.tried);
    printf("R%d base %d amorce %d : %ld clés | K4 QIII VIG/BEAU/VARB = %ld/%ld/%ld | K4 QIV = %ld | témoins (%d) : QIII %.2f par balayage, QIV %.2f par balayage | %.1fs\n",
           rule, B, n, R.tried, R.q3[0], R.q3[1], R.q3[2], R.q4, nn, (double)nq3 / nn, (double)nq4 / nn, dt);
    fflush(stdout);
  }
  return 0;
}
