/* t10_girasol.c — les textes de la MAQUETTE « Pre-K » de 1988 (tableau GIRASOL) comme clé courante, lus dans tous les sens.
 * Motif : Sanborn et Scheidt réagissent vivement au mot GIRASOL (2015) ; la maquette porte un tableau GIRASOL, une table
 * ordinaire et un message chiffré de 7 × 21 (Vigenère, clé RUG) ; les signaux de K4 sont en 7 et 21 (base 7, base 8).
 * Sources (data/girasol_*.txt) : tableau GIRASOL 15 lignes (relevé) et 26 lignes (extrapolé), table ordinaire (15 lignes),
 * chiffré 7 × 21 et son clair. Lectures 1D : les 13 parcours de T1, tous décalages.
 * Lectures 2D (cylindre, 4 orientations) : K4 à sa place physique (lignes de 31 du panneau) ET K4 en lignes de 21.
 * Juges : ceux de T1, plus l'alphabet GIRASOL parmi les alphabets fixés. Témoin : chiffrés aléatoires.
 */
#include "k4lib.h"
#include <time.h>

#define MAXL 64
#define MAXR 40
typedef struct { char g[MAXR][MAXL]; int nr, len[MAXR]; char name[48]; } Grid;

static Grid srcs[12]; static int nsrc = 0;
static int P24[NCRIB], C24[NCRIB];

static void load_grid(const char *fn, const char *name, int drop_label, int drop_q) {
  FILE *f = fopen(fn, "r"); if (!f) { perror(fn); exit(1); }
  Grid *G = &srcs[nsrc++]; memset(G, 0, sizeof *G); strcpy(G->name, name);
  char line[256];
  while (fgets(line, sizeof line, f)) {
    int n = strlen(line); while (n && (line[n - 1] == '\n' || line[n - 1] == '\r')) line[--n] = 0;
    int o = 0;
    for (int i = 0; i < n; i++) {
      char ch = line[i];
      if (drop_label && i == 0) continue;           /* colonne des étiquettes (ou blanc d'en-tête) */
      if (ch == '?' && drop_q) continue;
      G->g[G->nr][o++] = (ch >= 'A' && ch <= 'Z') ? ch : '.';
    }
    G->len[G->nr++] = o;
  }
  fclose(f);
}
static void linear_src(const char *s, const char *name) {
  Grid *G = &srcs[nsrc++]; memset(G, 0, sizeof *G); strcpy(G->name, name);
  int n = strlen(s), r = 0, o = 0;
  for (int i = 0; i < n; i++) { if (o == MAXL - 1) { G->len[r] = o; r++; o = 0; } G->g[r][o++] = s[i]; }
  G->len[r] = o; G->nr = r + 1;
}

/* produit la séquence 1D (lettres seulement) selon le parcours ord */
static int read_order(const Grid *G, int ord, char *out) {
  int n = 0, W = 0;
  for (int r = 0; r < G->nr; r++) if (G->len[r] > W) W = G->len[r];
#define PUT(ch) do { char _c = (ch); if (_c >= 'A' && _c <= 'Z') out[n++] = _c; } while (0)
#define AT(r, c) ((c) >= 0 && (c) < G->len[r] ? G->g[r][c] : '.')
#define ATR(r, c) /* justifié à droite */ ((c) - (W - G->len[r]) >= 0 && (c) - (W - G->len[r]) < G->len[r] ? G->g[r][(c) - (W - G->len[r])] : '.')
  switch (ord) {
    case 0: for (int r = 0; r < G->nr; r++) for (int c = 0; c < G->len[r]; c++) PUT(G->g[r][c]); break;
    case 1: for (int r = 0; r < G->nr; r++) for (int c = G->len[r] - 1; c >= 0; c--) PUT(G->g[r][c]); break;
    case 2: for (int r = G->nr - 1; r >= 0; r--) for (int c = G->len[r] - 1; c >= 0; c--) PUT(G->g[r][c]); break;
    case 3: for (int r = G->nr - 1; r >= 0; r--) for (int c = 0; c < G->len[r]; c++) PUT(G->g[r][c]); break;
    case 4: for (int r = 0; r < G->nr; r++) if (r % 2 == 0) for (int c = 0; c < G->len[r]; c++) PUT(G->g[r][c]); else for (int c = G->len[r] - 1; c >= 0; c--) PUT(G->g[r][c]); break;
    case 5: for (int r = 0; r < G->nr; r++) if (r % 2 == 1) for (int c = 0; c < G->len[r]; c++) PUT(G->g[r][c]); else for (int c = G->len[r] - 1; c >= 0; c--) PUT(G->g[r][c]); break;
    case 6: for (int c = 0; c < W; c++) for (int r = 0; r < G->nr; r++) PUT(AT(r, c)); break;
    case 7: for (int c = 0; c < W; c++) for (int r = G->nr - 1; r >= 0; r--) PUT(AT(r, c)); break;
    case 8: for (int c = W - 1; c >= 0; c--) for (int r = 0; r < G->nr; r++) PUT(AT(r, c)); break;
    case 9: for (int c = W - 1; c >= 0; c--) for (int r = G->nr - 1; r >= 0; r--) PUT(AT(r, c)); break;
    case 10: for (int c = 0; c < W; c++) for (int r = G->nr - 1; r >= 0; r--) PUT(ATR(r, c)); break;
    case 11: for (int d = 0; d < W + G->nr; d++) for (int r = 0; r < G->nr; r++) PUT(AT(r, d - r)); break;
    case 12: for (int d = -G->nr; d < W; d++) for (int r = 0; r < G->nr; r++) PUT(AT(r, d + r)); break;
  }
  return n;
#undef PUT
}
static const char *ORDNAME[13] = {"rowsLR", "rowsRL", "reverse", "rowsBT_LR", "boustro", "boustroRL", "colsTB", "colsBT(K3)", "colsTB_RL", "colsBT_RL", "colsBT_rightjust", "diag", "antidiag"};

/* résultats */
typedef struct { long tried, l3[3], na[3], nk[3]; int fxbest; } Tally;

static int CTcur[K4LEN];
static void set_ct(const int *ct) { memcpy(CTcur, ct, sizeof CTcur); for (int t = 0; t < NCRIB; t++) C24[t] = CTcur[CRIBPOS[t]]; }

static int AZID[26], GA[26], GAINV[26];
static void eval_key(const int *kl, Tally *T, int verbose, const char *tag) {
  int kn[NCRIB];
  T->tried++;
  for (int m = 0; m < 3; m++) {
    if (eng_let(P24, C24, kl, NCRIB, m)) { T->l3[m]++; if (verbose) printf("HIT L3 %s %s\n", MODENAME[m], tag); }
    for (int t = 0; t < NCRIB; t++) kn[t] = kl[t];
    if (eng_num(P24, C24, kn, NCRIB, m)) { T->na[m]++; if (verbose) printf("HIT NA %s %s\n", MODENAME[m], tag); }
    for (int t = 0; t < NCRIB; t++) kn[t] = KAINV[kl[t]];
    if (eng_num(P24, C24, kn, NCRIB, m)) { T->nk[m]++; if (verbose) printf("HIT NK %s %s\n", MODENAME[m], tag); }
  }
  /* alphabets fixés */
  const int *alph[3] = {AZID, KA, GA}; int rank[3][26];
  for (int i = 0; i < 26; i++) { rank[0][i] = i; rank[1][i] = KAINV[i]; rank[2][i] = GAINV[i]; }
  for (int ap = 0; ap < 3; ap++) for (int ac = 0; ac < 3; ac++) for (int kr = 0; kr < 3; kr++) for (int m = 0; m < 3; m++) {
    int s = 0;
    for (int t = 0; t < NCRIB; t++) if (fixed_pred(P24[t], rank[kr][kl[t]], m, rank[ap], alph[ac]) == C24[t]) s++;
    if (s > T->fxbest) { T->fxbest = s; if (verbose && s >= 12) printf("FX %d/24 ap=%d ac=%d kr=%d %s %s\n", s, ap, ac, kr, MODENAME[m], tag); }
  }
}

/* position physique de K4 sur le panneau : ligne 24 (0-based) col 27..30, puis lignes 25..27 col 0..30 */
static int K4R[K4LEN], K4C[K4LEN];

static void run_all(Tally *T1d, Tally *T2d, int verbose) {
  char seq[4096]; int kl[NCRIB]; char tag[160];
  for (int s = 0; s < nsrc; s++) for (int o = 0; o < 13; o++) {
    int L = read_order(&srcs[s], o, seq);
    if (L < 40) continue;
    for (int off = 0; off < L; off++) {
      for (int t = 0; t < NCRIB; t++) kl[t] = seq[(CRIBPOS[t] + off) % L] - 'A';
      snprintf(tag, sizeof tag, "src=%s ord=%s off=%d", srcs[s].name, ORDNAME[o], off);
      eval_key(kl, T1d, verbose, tag);
    }
  }
  /* 2D : toutes les sources ; K4 en place physique (lay=0) ou en lignes de 21 (lay=1) ; cylindre, 4 orientations */
  for (int s = 0; s < nsrc; s++) {
    Grid *G = &srcs[s]; if (G->nr < 2) continue;
    int W = 0; for (int r = 0; r < G->nr; r++) if (G->len[r] > W) W = G->len[r];
    for (int lay = 0; lay < 2; lay++)
    for (int tr = 0; tr < 4; tr++) for (int dr = 0; dr < G->nr; dr++) for (int dc = 0; dc < W; dc++) {
      int ok = 1;
      for (int t = 0; t < NCRIB && ok; t++) {
        int i = CRIBPOS[t];
        int r = (lay ? i / 21 : K4R[i]) + dr, c = (lay ? i % 21 : K4C[i]) + dc;
        r = ((r % G->nr) + G->nr) % G->nr;
        if (tr & 2) r = G->nr - 1 - r;
        if (G->len[r] == 0) { ok = 0; break; }
        c = ((c % G->len[r]) + G->len[r]) % G->len[r];
        if (tr & 1) c = G->len[r] - 1 - c;
        char ch = G->g[r][c]; if (ch < 'A' || ch > 'Z') { ok = 0; break; }
        kl[t] = ch - 'A';
      }
      if (!ok) continue;
      snprintf(tag, sizeof tag, "2D src=%s lay=%d tr=%d dr=%d dc=%d", G->name, lay, tr, dr, dc);
      eval_key(kl, T2d, verbose, tag);
    }
  }
}

static void print_tally(const char *h, Tally *T) {
  printf("%s : %ld clés | L3 VIG/BEAU/VARB %ld/%ld/%ld | NA %ld/%ld/%ld | NK %ld/%ld/%ld | alphabets fixés max %d/24\n",
         h, T->tried, T->l3[0], T->l3[1], T->l3[2], T->na[0], T->na[1], T->na[2], T->nk[0], T->nk[1], T->nk[2], T->fxbest);
}

int main(int argc, char **argv) {
  int NNULL = argc > 1 ? atoi(argv[1]) : 10;
  k4_init();
  for (int i = 0; i < 26; i++) AZID[i] = i;
  for (int t = 0; t < NCRIB; t++) P24[t] = CRIBPT[t];
  for (int i = 0; i < K4LEN; i++) { if (i < 4) { K4R[i] = 24; K4C[i] = 27 + i; } else { K4R[i] = 25 + (i - 4) / 31; K4C[i] = (i - 4) % 31; } }
  { const char *g = "GIRASOLBCDEFHJKMNPQTUVWXYZ"; for (int i = 0; i < 26; i++) { GA[i] = g[i] - 'A'; GAINV[g[i] - 'A'] = i; } }
  load_grid("data/girasol_tableau15.txt", "girasol_tableau15", 0, 0);
  load_grid("data/girasol_tableau26.txt", "girasol_tableau26", 0, 0);
  load_grid("data/girasol_table_rev15.txt", "table_rev15", 0, 0);
  load_grid("data/girasol_ct7x21.txt", "maquette_ct7x21", 0, 0);
  load_grid("data/girasol_pt7x21.txt", "maquette_pt7x21", 0, 0);
  { /* bloc chiffré répété (tel que sur la maquette) et clair répété, en linéaire */
    static char buf[2][400]; buf[0][0] = buf[1][0] = 0;
    for (int k = 0; k < 2; k++) for (int r = 0; r < 7; r++) { strcat(buf[0], srcs[3].g[r]); strcat(buf[1], srcs[4].g[r]); }
    linear_src(buf[0], "maquette_ct_x2"); linear_src(buf[1], "maquette_pt_x2");
  }
  Tally A = {0}, B = {0};
  clock_t t0 = clock();
  set_ct(CT);
  run_all(&A, &B, 1);
  print_tally("K4 1D", &A); print_tally("K4 2D", &B);
  printf("temps K4 : %.1fs\n", (double)(clock() - t0) / CLOCKS_PER_SEC);

  /* témoins */
  long nl3 = 0, nnum = 0; int fxmax1[40] = {0}, fxmax2[40] = {0};
  for (int z = 0; z < NNULL; z++) {
    int rct[K4LEN]; for (int i = 0; i < K4LEN; i++) rct[i] = rng() % 26;
    set_ct(rct);
    Tally a = {0}, b = {0};
    run_all(&a, &b, 0);
    for (int m = 0; m < 3; m++) { nl3 += a.l3[m] + b.l3[m]; nnum += a.na[m] + a.nk[m] + b.na[m] + b.nk[m]; }
    fxmax1[a.fxbest]++; fxmax2[b.fxbest]++;
    printf("témoin %d : ", z); print_tally("1D", &a); printf("           "); print_tally("2D", &b);
    fflush(stdout);
  }
  printf("TÉMOINS (%d chiffrés aléatoires) : total L3 %ld, total NA+NK %ld\n", NNULL, nl3, nnum);
  printf("distribution du max alphabets fixés 1D :"); for (int s = 0; s < 40; s++) if (fxmax1[s]) printf(" %d:%d", s, fxmax1[s]); printf("\n");
  printf("distribution du max alphabets fixés 2D :"); for (int s = 0; s < 40; s++) if (fxmax2[s]) printf(" %d:%d", s, fxmax2[s]); printf("\n");
  return 0;
}
