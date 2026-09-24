/* t7_cornerstone.c — clés courantes tirées de textes « enfouis » ou « fondateurs » du site, repérés dans les fichiers du groupe (24/09).
 *  S1 : directive Truman du 22/01/1946 (création du Central Intelligence Group), un des documents scellés dans la
 *       boîte de cuivre de la pierre angulaire de la CIA (mémo CIA du 18/11/1960). Lien : K2 « IT'S BURIED OUT THERE SOMEWHERE ».
 *  S2 : « Statement of Principles » et « key thoughts » du concours artistique CIA de 1988 (le cahier des charges que Sanborn a suivi).
 *  S3 : lettre de Sanborn aux employés (15/12/1989), extrait cité par la NSA (1999) et par The Cryptogram (1991).
 *  S4 : S1+S2+S3 concaténés.  Chaque source est lue à l'endroit et à l'envers, tous décalages cycliques.
 * Juges : identiques à T6 (L3 : σ quelconque, lettre-clé lue dans σ ; NA/NK : σ quelconque, clé = rang A–Z / KRYPTOS ;
 * FX : alphabets fixés, meilleur score sur 24). Témoin : 10 chiffrés aléatoires avec S1.
 */
#include "k4lib.h"

static int P24[NCRIB], C24[NCRIB];
typedef struct { long n, l3, na, nk; int fx; } T;

static void eval(const char *seq, int L, T *R) {
  int kl[NCRIB], kn[NCRIB], rank[2][26];
  for (int i = 0; i < 26; i++) { rank[0][i] = i; rank[1][i] = KAINV[i]; }
  int AZ[26]; for (int i = 0; i < 26; i++) AZ[i] = i;
  const int *alph[2] = {AZ, KA};
  for (int off = 0; off < L; off++) {
    for (int t = 0; t < NCRIB; t++) kl[t] = seq[(CRIBPOS[t] + off) % L] - 'A';
    R->n++;
    for (int m = 0; m < 3; m++) {
      if (eng_let(P24, C24, kl, NCRIB, m) > 0) R->l3++;
      if (eng_num(P24, C24, kl, NCRIB, m)) R->na++;
      for (int t = 0; t < NCRIB; t++) kn[t] = KAINV[kl[t]];
      if (eng_num(P24, C24, kn, NCRIB, m)) R->nk++;
    }
    for (int ap = 0; ap < 2; ap++) for (int ac = 0; ac < 2; ac++) for (int kr = 0; kr < 2; kr++) for (int m = 0; m < 3; m++) {
      int s = 0; for (int t = 0; t < NCRIB; t++) if (fixed_pred(P24[t], rank[kr][kl[t]], m, rank[ap], alph[ac]) == C24[t]) s++;
      if (s > R->fx) R->fx = s;
    }
  }
}

static int letters_only(const char *in, char *out) { int n = 0; for (; *in; in++) { char c = *in; if (c >= 'a' && c <= 'z') c -= 32; if (c >= 'A' && c <= 'Z') out[n++] = c; } out[n] = 0; return n; }

/* lit un fichier data/ en sautant l'en-tête (tout ce qui précède la ligne « --- ») */
static int load_body(const char *path, char *out) {
  static char raw[200000]; FILE *f = fopen(path, "r"); if (!f) { perror(path); return 0; }
  size_t nr = fread(raw, 1, sizeof raw - 1, f); raw[nr] = 0; fclose(f);
  char *p = strstr(raw, "\n---\n"); p = p ? p + 5 : raw;
  return letters_only(p, out);
}

static void run(const char *name, const char *seq0, int L0) {
  static char fw[400000], bw[400000];
  for (int dir = 0; dir < 2; dir++) {
    char *s = dir ? bw : fw;
    for (int i = 0; i < L0; i++) s[i] = dir ? seq0[L0 - 1 - i] : seq0[i];
    s[L0] = 0;
    int LL = L0; while (LL < 200) { memcpy(s + LL, s, L0); LL += L0; } s[LL] = 0; /* source courte : répétée */
    T R = {0}; eval(s, LL, &R);
    printf("%-28s %-8s (%d lettres) : %ld décalages | L3 %ld | NA %ld | NK %ld | alphabets fixés max %d/24\n",
           name, dir ? "envers" : "endroit", L0, R.n, R.l3, R.na, R.nk, R.fx);
    fflush(stdout);
  }
}

int main(void) {
  k4_init(); el_limit = 2000000;
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  static char b[4][600000]; int L[4];
  L[0] = load_body("data/truman_1946_directive.txt", b[0]);
  L[1] = load_body("data/cia_fine_arts_principles_1988.txt", b[1]);
  L[2] = letters_only(
    "The stonework at the entrance and in the courtyard served two functions. First, it creates a natural framework for the project "
    "as a whole and is part of a landscaping scheme designed to recall the natural stone outcropping that existed on the site before the "
    "Agency, and that will endure as do mountains. Second, the tilted strata tell a story like pages of a document. Inserted between these "
    "stone pages is a flat copper sheet through which letters and symbols have been cut. This code, which includes certain ancient ciphers, "
    "begins as International Morse and increases in complexity as you move through the piece at the entrance and into the courtyard. Its "
    "placement in a geologic context reinforces the text's hiddenness as if it were a fossil or an image frozen in time. "
    "The left side of the plate is a table for deciphering and enciphering code, developed by Blaise de Vigenere in 1570. The right side is "
    "a text that can be partly deciphered by using the table and partly by using a potentially challenging enciphering system. The text, "
    "written in collaboration with a prominent fiction writer, is revealed only after the code is deciphered.", b[2]);
  strcpy(b[3], b[0]); strcat(b[3], b[1]); strcat(b[3], b[2]); L[3] = strlen(b[3]);
  const char *nm[4] = {"S1 directive Truman 1946", "S2 principes CIA 1988", "S3 lettre Sanborn 1989", "S4 S1+S2+S3"};
  for (int s = 0; s < 4; s++) run(nm[s], b[s], L[s]);
  for (int z = 0; z < 10; z++) {
    for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
    T R = {0}; eval(b[0], L[0], &R);
    printf("témoin %d (S1, endroit) : L3 %ld | NA %ld | NK %ld | fixés max %d/24\n", z, R.l3, R.na, R.nk, R.fx);
    fflush(stdout);
  }
  return 0;
}
