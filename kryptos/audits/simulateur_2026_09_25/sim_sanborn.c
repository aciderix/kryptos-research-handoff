/* sim_sanborn.c — « simulateur Sanborn » (25/09/2026) : quels procédés faisables à la main laissent l'empreinte « 7 » de K4 ?
 * On ne teste plus une famille de clés contre les 24 lettres des cribs : on fabrique des millions de faux K4 avec chaque procédé
 * (clair anglais Gutenberg de 97 lettres, cribs EASTNORTHEAST / BERLINCLOCK insérés à leur place si CRIB=1, clés tirées au hasard,
 * alphabet KRYPTOS de Sanborn), et l'on mesure la probabilité que le procédé reproduise ce que l'on voit sur les 97 lettres :
 *   A7   : coïncidences c[i] = c[i+7]                                  (K4 : 9)
 *   Dmax : doublets c[i] = c[i+1] dans la classe i mod 7 la plus remplie (K4 : 5 sur 6, classe 4)
 *   P(voisin) : A7 ≥ 9 et A14+A21 ≤ 5 (profil de K4 : écart 7 seul) ; P(voisin&D) : ce profil et Dmax ≥ 5
 *   cd   : doublets aux trois paires de crib 25–26, 32–33, 67–68       (K4 : 3 sur 3 : NO→QQ, ST→SS, IN→TT)
 *   IC7  : indice de coïncidence moyen des 7 colonnes                  (K4 : 0,042 ; une vraie période 7 donne ≈ 0,066)
 *   A14+A21 : coïncidences aux écarts 14 et 21                         (K4 : 2 + 3 = 5, au niveau du hasard : le « 7 » ne relie
 *             que des voisins ; une vraie période 7 élève aussi 14 et 21)
 * Usage : sim_sanborn corpus.txt mots.txt [NSIM] [famille]    (CRIB=0 pour de l'anglais sans cribs)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#define N 97
static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int SIG[26], SINV[26], CRIB = 1;
static unsigned char *COR; static long NCOR;
static char (*W7)[8]; static int NW7;
typedef struct { uint64_t a, b; } R;
static inline uint64_t nx(R *r) { uint64_t s1 = r->a, s0 = r->b; r->a = s0; s1 ^= s1 << 23; r->b = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26); return r->b + s0; }
static inline int rn(R *r, int n) { return (int)((nx(r) >> 33) % (uint64_t)n); }
static inline double ru(R *r) { return (nx(r) >> 11) * (1.0 / 9007199254740992.0); }
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }
static inline int V(int p, int k) { return SINV[md(SIG[p] + k)]; }      /* Vigenère dans l'alphabet KRYPTOS (Quagmire III) */
static inline int B(int p, int k) { return SINV[md(k - SIG[p])]; }      /* Beaufort */
static void eng(R *r, int *p, int n) { long s = (long)(nx(r) % (uint64_t)(NCOR - n)); for (int i = 0; i < n; i++) p[i] = COR[s + i]; }
static void clair(R *r, int *p) {
  eng(r, p, N);
  if (CRIB) { const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK"; for (int i = 0; i < 13; i++) p[21 + i] = e[i] - 'A'; for (int i = 0; i < 11; i++) p[63 + i] = b[i] - 'A'; }
}
static void perm(R *r, int *o, int n) { for (int i = 0; i < n; i++) o[i] = i; for (int i = n - 1; i > 0; i--) { int j = rn(r, i + 1), t = o[i]; o[i] = o[j]; o[j] = t; } }
/* ordre de lecture d'une grille de largeur W (dernière ligne incomplète) : colonnes dans l'ordre ord[], haut→bas ou bas→haut */
static int colpos(int *pos, int n, int W, const int *ord, int up) {
  int H = (n + W - 1) / W, k = 0;
  for (int q = 0; q < W; q++) { int col = ord ? ord[q] : q;
    for (int rr = 0; rr < H; rr++) { int row = up ? H - 1 - rr : rr; int i = row * W + col; if (i < n) pos[k++] = i; } }
  return k;
}
static void colread(const int *x, int *y, int n, int W, const int *ord, int up) { int pos[128]; colpos(pos, n, W, ord, up); for (int k = 0; k < n; k++) y[k] = x[pos[k]]; }
static void colwrite(const int *x, int *y, int n, int W, const int *ord, int up) { int pos[128]; colpos(pos, n, W, ord, up); for (int k = 0; k < n; k++) y[pos[k]] = x[k]; }
static void k3rot(const int *x, int *y, int n, int w1, int w2) { int t[128]; colread(x, t, n, w1, NULL, 1); colread(t, y, n, w2, NULL, 1); }

static const char *NAME[] = {
  "hasard uniforme",                                        /* 0 */
  "Vigenère KA période 7, clé aléatoire",                   /* 1 */
  "Vigenère KA période 7, mot de 7 lettres",                /* 2 */
  "Vigenère KA période 2–20 (≠ 7, 14)",                     /* 3 */
  "clé courante anglaise",                                  /* 4 */
  "autoclé clair écart 7",                                  /* 5 */
  "autoclé chiffré écart 7",                                /* 6 */
  "autoclé clair écart 1",                                  /* 7 */
  "autoclé chiffré écart 1",                                /* 8 */
  "clé ligne + colonne (largeur 7)",                        /* 9 */
  "clé progressive période 7",                              /* 10 */
  "période 7 + clé courante",                               /* 11 */
  "période 7 puis transposition 7 colonnes",                /* 12 */
  "transposition 7 colonnes puis période 7",                /* 13 */
  "période 7 puis lignes de 7 permutées",                   /* 14 */
  "période 7 + erreurs de Sanborn (4 %)",                   /* 15 */
  "période 7 + une lettre omise (saut de phase)",           /* 16 */
  "période 7 + autoclé chiffré écart 7 (mixte)",            /* 17 */
  "période 7 + autoclé clair écart 7 (mixte)",              /* 18 */
  "clé transposée (mot L=8–20 en 7 colonnes)",              /* 19 */
  "clé de Fibonacci k[i]=k[i-7]+k[i-6]",                    /* 20 */
  "clé en chaîne k[i]=k[i-7]+k[i-1]",                       /* 21 */
  "disque tourné par bloc de 7",                            /* 22 */
  "deux clés superposées 7 et 8",                           /* 23 */
  "Beaufort KA période 7",                                  /* 24 */
  "Porta période 7",                                        /* 25 */
  "transposition 7 colonnes seule (témoin)",                /* 26 */
  "Vigenère KA période 14",                                 /* 27 */
  "Vigenère KA période 21",                                 /* 28 */
  "Quagmire III alphabet quelconque, période 7",            /* 29 */
  "période 7 puis méthode K3 (deux rotations)",             /* 30 */
  "méthode K3 (deux rotations) puis période 7",             /* 31 */
  "période 7 + masque en dents de scie (Matson)",           /* 32 */
  "période 7 avec k4 = k5",                                 /* 33 */
  "autoclé clair écart 7, Beaufort",                        /* 34 */
  "transposition 7 colonnes puis clé courante",             /* 35 */
  "Chaocipher",                                             /* 36 */
  "Gronsfeld (chiffres) période 7",                         /* 37 */
  "période 7 puis addition en chaîne c[i]+=c[i-1]",         /* 38 */
  "clé KRYPTOS (0..6 dans KA) période 7",                   /* 39 */
  "période 7 puis grille 14 colonnes lue en colonnes",      /* 40 */
  "grille 14 colonnes lue en colonnes puis période 7",      /* 41 */
  "période 7 puis grille 7×14 écrite en colonnes",          /* 42 */
  "période 14 puis transposition 7 colonnes",               /* 43 */
  "clé courante puis transposition 7 colonnes",             /* 44 */
  "Beaufort mixte : période 7 + chiffré écart 7",           /* 45 */
  "période 7 + erreurs de copie c=p (10 %)",                /* 46 */
  "période 7 puis autoclé chiffré écart 7",                 /* 47 */
  "autoclé clair écart 7, Variant Beaufort",                /* 48 */
  "autoclé clair écart 7, Vig., chiffré en alphabet ≠",     /* 49 */
  "autoclé clair écart 7, lettre-clé dans un alphabet ≠",   /* 50 */
  "autoclé clair écart 7, Vig. A–Z",                        /* 51 */
};
#define NF ((int)(sizeof NAME / sizeof NAME[0]))

static void gen(int f, R *r, int *c) {
  int p[N], k[N], m[32], t[N], o[32], e[N];
  clair(r, p);
  for (int j = 0; j < 32; j++) m[j] = rn(r, 26);
  switch (f) {
  case 0: for (int i = 0; i < N; i++) c[i] = rn(r, 26); break;
  case 1: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7]); break;
  case 2: { const char *w = W7[rn(r, NW7)]; for (int i = 0; i < N; i++) c[i] = V(p[i], SIG[w[i % 7] - 'A']); } break;
  case 3: { int P; do P = 2 + rn(r, 19); while (P == 7 || P == 14); for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % P]); } break;
  case 4: eng(r, e, N); for (int i = 0; i < N; i++) c[i] = V(p[i], SIG[e[i]]); break;
  case 5: for (int i = 0; i < N; i++) c[i] = V(p[i], i < 7 ? m[i] : SIG[p[i - 7]]); break;
  case 6: for (int i = 0; i < N; i++) c[i] = V(p[i], i < 7 ? m[i] : SIG[c[i - 7]]); break;
  case 7: for (int i = 0; i < N; i++) c[i] = V(p[i], i < 1 ? m[0] : SIG[p[i - 1]]); break;
  case 8: for (int i = 0; i < N; i++) c[i] = V(p[i], i < 1 ? m[0] : SIG[c[i - 1]]); break;
  case 9: { int a[15]; for (int j = 0; j < 15; j++) a[j] = rn(r, 26); for (int i = 0; i < N; i++) c[i] = V(p[i], a[i / 7] + m[i % 7]); } break;
  case 10: { int s = 1 + rn(r, 25); for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7] + s * (i / 7)); } break;
  case 11: eng(r, e, N); for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7] + SIG[e[i]]); break;
  case 12: for (int i = 0; i < N; i++) t[i] = V(p[i], m[i % 7]); perm(r, o, 7); colread(t, c, N, 7, o, 0); break;
  case 13: perm(r, o, 7); colread(p, t, N, 7, o, 0); for (int i = 0; i < N; i++) c[i] = V(t[i], m[i % 7]); break;
  case 14: { for (int i = 0; i < N; i++) t[i] = V(p[i], m[i % 7]); perm(r, o, 13);
      for (int q = 0; q < 13; q++) { for (int j = 0; j < 7; j++) c[q * 7 + j] = t[o[q] * 7 + j]; }
      for (int i = 91; i < N; i++) c[i] = t[i]; } break;
  case 15: for (int i = 0; i < N; i++) { double u = ru(r); c[i] = u < 0.02 ? p[i] : u < 0.04 ? V(p[i], m[(i + 1) % 7]) : V(p[i], m[i % 7]); } break;
  case 16: { int x = 1 + rn(r, N - 1); for (int i = 0; i < N; i++) c[i] = V(p[i], m[(i + (i >= x)) % 7]); } break;
  case 17: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7] + (i >= 7 ? SIG[c[i - 7]] : 0)); break;
  case 18: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7] + (i >= 7 ? SIG[p[i - 7]] : 0)); break;
  case 19: { int L = 8 + rn(r, 13), kk[32], q = 0; perm(r, o, 7); int pos[32]; colpos(pos, L, 7, o, 0); for (int j = 0; j < L; j++) kk[q++] = m[pos[j]];
      for (int i = 0; i < N; i++) c[i] = V(p[i], kk[i % L]); } break;
  case 20: for (int i = 0; i < N; i++) { k[i] = i < 7 ? m[i] : md(k[i - 7] + k[i - 6]); c[i] = V(p[i], k[i]); } break;
  case 21: for (int i = 0; i < N; i++) { k[i] = i < 7 ? m[i] : md(k[i - 7] + k[i - 1]); c[i] = V(p[i], k[i]); } break;
  case 22: { int s = 1 + rn(r, 25); for (int i = 0; i < N; i++) c[i] = V(p[i], m[0] + s * (i / 7)); } break;
  case 23: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7] + m[8 + i % 8]); break;
  case 24: for (int i = 0; i < N; i++) c[i] = B(p[i], m[i % 7]); break;
  case 25: for (int i = 0; i < N; i++) { int g = m[i % 7] / 2; c[i] = p[i] < 13 ? 13 + (p[i] + g) % 13 : (p[i] - 13 - g + 26) % 13; } break;
  case 26: perm(r, o, 7); colread(p, c, N, 7, o, 0); break;
  case 27: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 14]); break;
  case 28: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 21]); break;
  case 29: { int s[26], si[26]; perm(r, s, 26); for (int i = 0; i < 26; i++) si[s[i]] = i;
      for (int i = 0; i < N; i++) c[i] = si[md(s[p[i]] + m[i % 7])]; } break;
  case 30: { int w1 = 4 + rn(r, 21), w2 = 4 + rn(r, 21); for (int i = 0; i < N; i++) t[i] = V(p[i], m[i % 7]); k3rot(t, c, N, w1, w2); } break;
  case 31: { int w1 = 4 + rn(r, 21), w2 = 4 + rn(r, 21); k3rot(p, t, N, w1, w2); for (int i = 0; i < N; i++) c[i] = V(t[i], m[i % 7]); } break;
  case 32: { int A = 2 + rn(r, 5), ph = rn(r, 2 * A); for (int i = 0; i < N; i++) { int z = (i + ph) % (2 * A), s = z <= A ? z : 2 * A - z; c[i] = SINV[md(SIG[V(p[i], m[i % 7])] + s)]; } } break;
  case 33: m[5] = m[4]; for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7]); break;
  case 34: for (int i = 0; i < N; i++) c[i] = B(p[i], i < 7 ? m[i] : SIG[p[i - 7]]); break;
  case 35: perm(r, o, 7); colread(p, t, N, 7, o, 0); eng(r, e, N); for (int i = 0; i < N; i++) c[i] = V(t[i], SIG[e[i]]); break;
  case 36: { int L[26], Rr[26], tmp[26]; perm(r, L, 26); perm(r, Rr, 26);
      for (int i = 0; i < N; i++) { int idx = 0; while (Rr[idx] != p[i]) idx++; c[i] = L[idx];
        for (int q = 0; q < 26; q++) { tmp[q] = L[(q + idx) % 26]; }
        memcpy(L, tmp, sizeof L); { int x = L[1]; for (int q = 1; q < 13; q++) L[q] = L[q + 1]; L[13] = x; }
        for (int q = 0; q < 26; q++) { tmp[q] = Rr[(q + idx + 1) % 26]; }
        memcpy(Rr, tmp, sizeof Rr); { int x = Rr[2]; for (int q = 2; q < 13; q++) Rr[q] = Rr[q + 1]; Rr[13] = x; } } } break;
  case 37: for (int i = 0; i < N; i++) c[i] = V(p[i], m[i % 7] % 10); break;
  case 38: for (int i = 0; i < N; i++) { int x = SIG[p[i]] + m[i % 7]; c[i] = SINV[md(x + (i ? SIG[c[i - 1]] : 0))]; } break;
  case 39: for (int i = 0; i < N; i++) c[i] = V(p[i], i % 7); break;
  case 40: for (int i = 0; i < N; i++) t[i] = V(p[i], m[i % 7]); perm(r, o, 14); colread(t, c, N, 14, o, 0); break;
  case 41: perm(r, o, 14); colread(p, t, N, 14, o, 0); for (int i = 0; i < N; i++) c[i] = V(t[i], m[i % 7]); break;
  case 42: for (int i = 0; i < N; i++) t[i] = V(p[i], m[i % 7]); perm(r, o, 7); colwrite(t, c, N, 7, o, 0); break;
  case 43: for (int i = 0; i < N; i++) t[i] = V(p[i], m[i % 14]); perm(r, o, 7); colread(t, c, N, 7, o, 0); break;
  case 44: eng(r, e, N); for (int i = 0; i < N; i++) t[i] = V(p[i], SIG[e[i]]); perm(r, o, 7); colread(t, c, N, 7, o, 0); break;
  case 45: for (int i = 0; i < N; i++) c[i] = B(p[i], m[i % 7] + (i >= 7 ? SIG[c[i - 7]] : 0)); break;
  case 46: for (int i = 0; i < N; i++) c[i] = ru(r) < 0.10 ? p[i] : V(p[i], m[i % 7]); break;
  case 47: for (int i = 0; i < N; i++) { int x = SIG[V(p[i], m[i % 7])]; c[i] = SINV[md(x + (i >= 7 ? SIG[c[i - 7]] : 0))]; } break;
  case 48: for (int i = 0; i < N; i++) c[i] = SINV[md(SIG[p[i]] - (i < 7 ? m[i] : SIG[p[i - 7]]))]; break;
  case 49: { int s[26]; perm(r, s, 26); for (int i = 0; i < N; i++) c[i] = s[md(SIG[p[i]] + (i < 7 ? m[i] : SIG[p[i - 7]]))]; } break;
  case 50: { int s[26]; perm(r, s, 26); for (int i = 0; i < N; i++) c[i] = V(p[i], i < 7 ? m[i] : s[p[i - 7]]); } break;
  case 51: for (int i = 0; i < N; i++) c[i] = (p[i] + (i < 7 ? m[i] : p[i - 7])) % 26; break;
  }
}

typedef struct { int A7, A1421, D, Dmax, Dr4, cd; double ic7; } St;
static void stats(const int *c, St *s) {
  int Dr[7] = {0}; s->A7 = 0; s->D = 0;
  for (int i = 0; i + 7 < N; i++) s->A7 += c[i] == c[i + 7];
  s->A1421 = 0; for (int i = 0; i + 14 < N; i++) s->A1421 += c[i] == c[i + 14]; for (int i = 0; i + 21 < N; i++) s->A1421 += c[i] == c[i + 21];
  for (int i = 0; i + 1 < N; i++) if (c[i] == c[i + 1]) { s->D++; Dr[i % 7]++; }
  s->Dmax = 0; for (int j = 0; j < 7; j++) if (Dr[j] > s->Dmax) s->Dmax = Dr[j];
  s->Dr4 = Dr[4];
  s->cd = (c[25] == c[26]) + (c[32] == c[33]) + (c[67] == c[68]);
  double ic = 0; for (int j = 0; j < 7; j++) { int cnt[26] = {0}, n = 0; for (int i = j; i < N; i += 7) { cnt[c[i]]++; n++; }
    double a = 0; for (int q = 0; q < 26; q++) a += cnt[q] * (cnt[q] - 1.0); ic += a / (n * (n - 1.0)); }
  s->ic7 = ic / 7;
}

int main(int argc, char **argv) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  if (argc < 3) { fprintf(stderr, "usage: %s corpus.txt mots.txt [NSIM] [famille]\n", argv[0]); return 1; }
  if (getenv("CRIB")) CRIB = atoi(getenv("CRIB"));
  long NS = argc > 3 ? atol(argv[3]) : 1000000; int F1 = argc > 4 ? atoi(argv[4]) : -1;
  for (int i = 0; i < 26; i++) { SIG[KA[i] - 'A'] = i; SINV[i] = KA[i] - 'A'; }
  FILE *f = fopen(argv[1], "rb"); fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
  unsigned char *raw = malloc(sz); if (fread(raw, 1, sz, f) != (size_t)sz) return 2; fclose(f);
  COR = malloc(sz); NCOR = 0; for (long i = 0; i < sz; i++) { int ch = raw[i]; if (ch >= 'a' && ch <= 'z') ch -= 32; if (ch >= 'A' && ch <= 'Z') COR[NCOR++] = ch - 'A'; }
  free(raw);
  f = fopen(argv[2], "r"); char line[256]; int cap = 1 << 16; W7 = malloc(sizeof *W7 * cap); NW7 = 0;
  while (fgets(line, sizeof line, f)) { int n = strcspn(line, "\r\n"); line[n] = 0; if (n != 7) continue; int ok = 1; for (int i = 0; i < 7; i++) if (line[i] < 'A' || line[i] > 'Z') ok = 0; if (ok && NW7 < cap) memcpy(W7[NW7++], line, 8); }
  fclose(f);
  int c0[N]; for (int i = 0; i < N; i++) c0[i] = K4[i] - 'A'; St s0; stats(c0, &s0);
  printf("corpus : %ld lettres ; mots de 7 lettres : %d ; cribs insérés : %s ; %ld chiffrés par procédé\n", NCOR, NW7, CRIB ? "oui" : "non", NS);
  printf("K4 : A7=%d A14+A21=%d doublets=%d Dmax=%d (classe 4 : %d) cribs-doublets=%d IC7=%.4f\n\n", s0.A7, s0.A1421, s0.D, s0.Dmax, s0.Dr4, s0.cd, s0.ic7);
  printf("%-46s %5s %5s %6s | %8s %8s %8s %9s %10s %8s %9s\n", "procédé", "doubl", "A7", "IC7", "P(A7≥9)", "P(voisin)", "P(Dmax≥5)", "P(A7&D)", "P(voisin&D)", "P(cd=3)", "P(tout)");
  for (int fam = 0; fam < NF; fam++) {
    if (F1 >= 0 && fam != F1) continue;
    long nA = 0, nV = 0, nVD = 0, nD = 0, nAD = 0, nC = 0, nAll = 0; double sD = 0, sA = 0, sI = 0;
#pragma omp parallel reduction(+:nA,nV,nVD,nD,nAD,nC,nAll,sD,sA,sI)
    {
      R r = { 0x9E3779B97F4A7C15ULL ^ (uint64_t)(fam * 1000003 + omp_get_thread_num() * 7919 + 1), 0xD1B54A32D192ED03ULL + (uint64_t)fam * 31 + omp_get_thread_num() };
      for (int w = 0; w < 20; w++) nx(&r);
      int c[N]; St s;
#pragma omp for schedule(static)
      for (long it = 0; it < NS; it++) {
        gen(fam, &r, c); stats(c, &s);
        int a = s.A7 >= 9, d = s.Dmax >= 5, cc = s.cd == 3;
        nA += a; nV += a && s.A1421 <= 5; nVD += a && s.A1421 <= 5 && d; nD += d; nAD += a && d; nC += cc; nAll += a && s.Dr4 >= 5 && cc;
        sD += s.D; sA += s.A7; sI += s.ic7;
      }
    }
    printf("%2d %-43s %5.2f %5.2f %6.4f | %8.2e %8.2e %8.2e %9.2e %10.2e %8.2e %9.2e\n", fam, NAME[fam], sD / NS, sA / NS, sI / NS,
           (double)nA / NS, (double)nV / NS, (double)nD / NS, (double)nAD / NS, (double)nVD / NS, (double)nC / NS, (double)nAll / NS);
  }
  return 0;
}
