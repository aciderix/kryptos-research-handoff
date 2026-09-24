/* t1_sculpture_keys.c — les TEXTES PHYSIQUES de la sculpture comme clé courante, lus dans tous les sens.
 *
 * Question : une clé « cachée sur la sculpture » (Scheidt 2003), lue sur l'objet lui-même
 * (tableau, panneau chiffré K1–K4, Morse), dans un sens de lecture géométrique simple,
 * rend-elle compte des 24 lettres des cribs ?
 *
 * Paramètres fixés AVANT calcul :
 *  Sources : tableau (avec / sans étiquettes de ligne, en-têtes compris), panneau chiffré (« ? » retirés),
 *            panneau chiffré avec « ? » comptés comme cases vides, Morse K0 (2 ordres).
 *  Lectures (1D) : 13 parcours (lignes G→D, D→G, inversion totale, lignes de bas en haut, boustrophédons,
 *            colonnes haut→bas / bas→haut, gauche→droite / droite→gauche, colonnes justifiées à droite,
 *            diagonales, anti-diagonales). Tous les décalages (cycliques).
 *  Lectures (2D) : K4 à sa place physique (lignes 25–28) ; clé = lettre de la source en (ligne+dr, col+dc),
 *            source telle quelle / miroir G-D / miroir H-B / rotation 180° ; tous dr, dc couvrant les cribs.
 *  Juges exacts (aucun score d'anglais) :
 *    L3 : σ quelconque, lettre-clé lue dans σ (VIG/BEAU/VARB)          — eng_let
 *    NA : σ quelconque, clé = rang A–Z de la lettre-clé                  — eng_num
 *    NK : σ quelconque, clé = rang KRYPTOS de la lettre-clé              — eng_num
 *    FX : alphabets fixés (clair/chiffré ∈ {AZ,KA}, rang ∈ {AZ,KA}, 3 modes) : meilleur score /24
 *  Témoin : même balayage complet sur NNULL chiffrés aléatoires (cribs inchangés).
 */
#include "k4lib.h"
#include <time.h>

#define MAXL 64
#define MAXR 40
typedef struct { char g[MAXR][MAXL]; int nr, len[MAXR]; char name[48]; } Grid;

static Grid srcs[8]; static int nsrc = 0;
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

static int AZID[26];
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
  const int *alph[2] = {AZID, KA}; int rank[2][26];
  for (int i = 0; i < 26; i++) { rank[0][i] = i; rank[1][i] = KAINV[i]; }
  for (int ap = 0; ap < 2; ap++) for (int ac = 0; ac < 2; ac++) for (int kr = 0; kr < 2; kr++) for (int m = 0; m < 3; m++) {
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
    if (L < 98) continue;
    for (int off = 0; off < L; off++) {
      for (int t = 0; t < NCRIB; t++) kl[t] = seq[(CRIBPOS[t] + off) % L] - 'A';
      snprintf(tag, sizeof tag, "src=%s ord=%s off=%d", srcs[s].name, ORDNAME[o], off);
      eval_key(kl, T1d, verbose, tag);
    }
  }
  /* 2D : sources tableau (0,1) et panneau (2) */
  for (int s = 0; s < nsrc; s++) {
    Grid *G = &srcs[s]; if (G->nr < 20 || s == 2) continue; /* panel_noQ : géométrie décalée, exclu en 2D */
    int W = 0; for (int r = 0; r < G->nr; r++) if (G->len[r] > W) W = G->len[r];
    for (int tr = 0; tr < 4; tr++) for (int dr = 0; dr < G->nr; dr++) for (int dc = 0; dc < W; dc++) {
      int ok = 1;
      for (int t = 0; t < NCRIB && ok; t++) {
        int i = CRIBPOS[t], r = K4R[i] + dr, c = K4C[i] + dc;
        /* enroulement (cylindre, comme les « projection cylinders » de Sanborn) : lignes et colonnes modulo */
        r = ((r % G->nr) + G->nr) % G->nr;
        if (tr & 2) r = G->nr - 1 - r;      /* miroir haut-bas */
        if (G->len[r] == 0) { ok = 0; break; }
        c = ((c % G->len[r]) + G->len[r]) % G->len[r];
        if (tr & 1) c = G->len[r] - 1 - c;  /* miroir gauche-droite (ligne par ligne) */
        char ch = G->g[r][c]; if (ch < 'A' || ch > 'Z') { ok = 0; break; }
        kl[t] = ch - 'A';
      }
      if (!ok) continue;
      if (s == 3 && tr == 0 && dr == 0 && dc == 0) continue; /* clé = chiffré lui-même : trivial */
      snprintf(tag, sizeof tag, "2D src=%s tr=%d dr=%d dc=%d", G->name, tr, dr, dc);
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
  load_grid("data/tableau_wikipedia.txt", "tableau+labels", 0, 0);
  load_grid("data/tableau_wikipedia.txt", "tableau_body", 1, 0);
  load_grid("data/panel_wikipedia.txt", "panel_noQ", 0, 1);
  load_grid("data/panel_wikipedia.txt", "panel_Qhole", 0, 0);
  linear_src("SOSRQLUCIDEEEMEMORYEEESHADOWEEFORCESEEEEETISYOURPOSITIONEEDIGETALEEEINTERPRETATIEEVIRTUALLYEEEEEEEINVISIBLE", "morse_rumkin_list");
  linear_src("TISYOURPOSITIONEEDIGETALEEEINTERPRETATISOSRQLUCIDEEEMEMORYEEESHADOWEEFORCESEEEEEEEVIRTUALLYEEEEEEEINVISIBLE", "morse_by_slab");
  /* vérif position physique : K4[0..3] = OBKR en ligne 25 col 27.. */
  printf("contrôle position (avec ?) : %.4s\n", &srcs[3].g[24][27]);

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
