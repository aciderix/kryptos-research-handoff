/* t6_new_sources.c — clés courantes tirées des NOUVEAUX textes versés le 24/09 (sources/docs_utilisateur_2026_09_24).
 *  S1 : journal de fouilles de Howard Carter, 28 oct.–31 déc. 1922 (Griffith Institute, « Carter Notes.pdf ») —
 *       c'est le texte du JOURNAL, pas le livre (le livre, vol. 1 Gutenberg, est déjà testé dans le dépôt).
 *  S2 : noms des 14 directeurs (plaques de laiton du hall, relevé NSA/CIA du 18/11/1991, « The CIA Courtyard »).
 *  S3 : inscription de la statue de Donovan.   S4 : buste de Dulles.   S5 : noms du « Book of Honor » (relevé 1991).
 *  S6 : S2+S3+S4+S5 concaténés dans l'ordre du mémo.
 * Tous décalages (cycliques). Juges : L3 (σ quelconque, lettre-clé dans σ), NA/NK (σ quelconque, clé = rang A–Z / KRYPTOS),
 * FX (alphabets fixés, meilleur score /24). Témoin : 10 chiffrés aléatoires.
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

int main(void) {
  k4_init(); el_limit = 2000000;
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  static char carter_raw[200000], buf[6][200000]; int L[6];
  FILE *f = fopen("../../sources/docs_utilisateur_2026_09_24/texte/carter_notes.txt", "r");
  size_t nr = fread(carter_raw, 1, sizeof carter_raw - 1, f); carter_raw[nr] = 0; fclose(f);
  L[0] = letters_only(carter_raw, buf[0]);
  L[1] = letters_only("William Joseph Donovan Sidney William Souers Hoyt Sanford Vandenberg Roscoe Henry Hillenkoetter Walter Bedell Smith Allen Welsh Dulles John Alex McCone William Francis Raborn Richard Helms James R Schlesinger William E Colby George Bush Stansfield Turner William J Casey", buf[1]);
  L[2] = letters_only("Founding Father Major General William J Donovan Director Office of Strategic Services Forerunner of the CIA", buf[2]);
  L[3] = letters_only("Allen Welsh Dulles Director of Central Intelligence His Monument Is Around Us", buf[3]);
  L[4] = letters_only("Jerome P Ginley William P Boteler Howard Carey Frank G Grace Jr Wilburn S Rose Nels L Benson John G Merriman Buster Edens Edward Johnson John W Waltz Louisa A OJibway Walter L Ray Billy Jack Johnson Jack W Weeks Paul C Davis David L Konzelman William E Bennett Richard S Welch Robert C Ames Scott J Vanlieshout Curtis R Wood William F Buckley Richard D Krobock", buf[4]);
  snprintf(buf[5], sizeof buf[5], "%s%s%s%s", buf[1], buf[2], buf[3], buf[4]); L[5] = strlen(buf[5]);
  const char *nm[6] = {"journal Carter 1922", "directeurs CIA (plaques)", "statue Donovan", "buste Dulles", "Book of Honor (1991)", "hall CIA concaténé"};
  for (int s = 0; s < 6; s++) {
    char dbl[400000]; /* séquence pour les sources courtes : répétée pour couvrir 97 positions */
    int LL = L[s]; strcpy(dbl, buf[s]);
    while ((int)strlen(dbl) < 200) strcat(dbl, buf[s]);
    LL = strlen(dbl);
    T R = {0}; eval(dbl, LL, &R);
    printf("%-26s (%d lettres) : %ld décalages | L3 %ld | NA %ld | NK %ld | alphabets fixés max %d/24\n", nm[s], L[s], R.n, R.l3, R.na, R.nk, R.fx);
    fflush(stdout);
  }
  /* témoins : même balayage sur chiffrés aléatoires, source = journal de Carter (la plus longue) */
  for (int z = 0; z < 10; z++) {
    for (int t = 0; t < NCRIB; t++) C24[t] = rng() % 26;
    T R = {0}; eval(buf[0], L[0], &R);
    printf("témoin %d (journal Carter) : L3 %ld | NA %ld | NK %ld | fixés max %d/24\n", z, R.l3, R.na, R.nk, R.fx);
    fflush(stdout);
  }
  return 0;
}
