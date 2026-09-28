/* t27_autocle_motcle_chaines.c — autoclé sur le CLAIR, alphabets à mot-clé, erreurs comptées par ÉQUATIONS DE CHAÎNE
 * (relais du 25/09/2026 ; complète ../motcle_pas7_2026_09_24/kwautokey.c et ../vision_2026_09_24/t19_une_erreur.c)
 *
 * Pourquoi. kwautokey.c propage le clair depuis la PREMIÈRE lettre de crib de chaque classe (mod L) et compte les lettres de
 * crib non reproduites. En autoclé, une lettre de chiffré fausse corrompt tout le reste de sa chaîne : une seule erreur peut
 * donc produire plusieurs désaccords. Ici, on RÉ-ANCRE la chaîne à chaque lettre de crib : entre deux positions de crib
 * consécutives a < b d'une même classe, on propage depuis le clair connu en a et l'on compare en b. Chaque équation (a → b)
 * n'est faussée que par une erreur dans le segment ]a, b] ; changer c_b suffit à la réparer. Donc :
 *     nombre minimal d'erreurs = nombre d'équations fausses (F).
 * Modèle « copie » (erreur vue chez Sanborn, base 7 §9.4) : l'unique équation fausse se termine sur une position de crib où
 * chiffré = clair (32 : S → S, 73 : K → K pour K4), comme si la lettre claire avait été recopiée sans chiffrement.
 *
 * Modèle de chiffrement (comme kwautokey.c) : X(c_i) = Y(p_i) ⊕ k_i, k_i = Z(p_{i−L}) ; X, Y alphabets du chiffré et du clair
 * (Q3 : σ/σ ; Q2 : A–Z/σ côté clair A–Z ; Q1 ; Q4a σ/KA ; Q4b KA/σ), Z = lecture de la lettre-clé (σ du clair, A–Z, KRYPTOS) ;
 * ⊕ : VIG y + k, BEAU k − y, VARB y − k. Alphabets : chaque mot sous 4 formes (standard, « suite », et inverses), dédoublonnés.
 * L = 1..48. Pour les cas à F ≤ 1 et L ≤ 13 (clair entièrement déterminé), on déchiffre les 97 lettres en ré-ancrant et on
 * note l'anglais (quadrigrammes log10 moyens ; anglais ≈ −4,2, charabia ≈ −6).
 * Parallélisé avec OpenMP sur les alphabets.
 * Usage : t27 qg.bin mots.txt [mode_témoin graine]   mode_témoin : 0 = K4, 1 = lettres uniformes, 2 = K4 mélangé, 3 = contrôle
 *         positif (clair anglais, PALIMPSEST, écart 7, VIG, Q3, clé lue en σ, UNE copie en 73), 4 = même contrôle avec une
 *         lettre fausse en 42 (hors cribs, au milieu d'une chaîne).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

static const char *K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const char *KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int CT[97], ISC[97], CP[97], COPY[97];
static float *QG;
#define LMAX 48
static int PREV[LMAX + 1][97];   /* PREV[L][b] = position de crib précédente dans la classe de b, ou −1 */
static inline int md(int x) { x %= 26; return x < 0 ? x + 26 : x; }

static void mkalpha(const char *w, int cont, int rev, int *pos) {
  char a[27]; int n = 0, used[26] = {0};
  for (const char *p = w; *p; p++) { int c = *p - 'A'; if (c < 0 || c > 25 || used[c]) continue; used[c] = 1; a[n++] = 'A' + c; }
  int start = cont && n ? (a[n - 1] - 'A' + 1) % 26 : 0;
  for (int k = 0; k < 26; k++) { int c = (start + k) % 26; if (!used[c]) { used[c] = 1; a[n++] = 'A' + c; } }
  for (int i = 0; i < 26; i++) pos[(rev ? a[25 - i] : a[i]) - 'A'] = i;
}
typedef struct { const int *X, *Y, *Z; int YI[26], ZI[26]; int mo; } Ctx;
static inline int dec(const Ctx *c, int ct, int pprev) {        /* clair en i connaissant c_i et p_{i−L} */
  int k = c->Z[pprev], x = c->X[ct], y = c->mo == 0 ? md(x - k) : c->mo == 1 ? md(k - x) : md(x + k);
  return c->YI[y];
}
static inline int back(const Ctx *c, int ct, int p) {           /* p_{i−L} connaissant c_i et p_i */
  int x = c->X[ct], y = c->Y[p], k = c->mo == 0 ? md(x - y) : c->mo == 1 ? md(x + y) : md(y - x);
  return c->ZI[k];
}
/* F = équations fausses ; *last = extrémité b de la (dernière) équation fausse ; *neq = nombre d'équations */
static int chains(const Ctx *c, int L, int *last, int *neq) {
  int F = 0, n = 0;
  for (int b = 21; b <= 73; b++) {
    if (!ISC[b]) continue; int a = PREV[L][b]; if (a < 0) continue; n++;
    int p = CP[a];
    for (int t = a + L; t <= b; t += L) p = dec(c, CT[t], p);  /* p_{t−L} → p_t ; aux positions de crib intermédiaires : aucune (a est la précédente) */
    if (p != CP[b]) { F++; *last = b; }
  }
  *neq = n; return F;
}
/* ancien comptage (kwautokey.c, k4x -e a) : propagation depuis la 1re lettre de crib de chaque classe, SANS ré-ancrage */
static int oldcount(const Ctx *c, int L) {
  int err = 0;
  for (int r = 0; r < L; r++) { int s0 = -1; for (int i = 21; i <= 73; i++) if (ISC[i] && i % L == r) { s0 = i; break; }
    if (s0 < 0) continue; int p = CP[s0];
    for (int i = s0 + L; i < 97; i += L) { p = dec(c, CT[i], p); if (ISC[i] && p != CP[i]) err++; } }
  return err;
}
/* clair complet ré-ancré (L ≤ 13 : chaque classe a une position de crib) ; renvoie le score quadrigramme moyen */
static double fullpt(const Ctx *c, int L, char *out) {
  int pt[97];
  for (int r = 0; r < L; r++) {
    int s = -1; for (int i = 21; i <= 73; i++) if (ISC[i] && i % L == r) { s = i; break; }
    if (s < 0) return -99;
    for (int i = s; i < 97; i += L) {
      if (ISC[i]) pt[i] = CP[i]; else pt[i] = dec(c, CT[i], pt[i - L]);
    }
    for (int i = s; i - L >= 0; i -= L) pt[i - L] = back(c, CT[i], pt[i]);
  }
  double q = 0; for (int i = 0; i + 3 < 97; i++) q += QG[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]];
  if (out) { for (int i = 0; i < 97; i++) out[i] = 'A' + pt[i]; out[97] = 0; }
  return q / 94;
}

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "rb"); QG = malloc(456976 * 4); if (!f || fread(QG, 4, 456976, f) != 456976) return 2; fclose(f);
  int nullmode = argc > 3 ? atoi(argv[3]) : 0; uint64_t seed = argc > 4 ? strtoull(argv[4], 0, 10) : 1;
  const char *e = "EASTNORTHEAST", *b = "BERLINCLOCK";
  for (int i = 0; i < 13; i++) { ISC[21 + i] = 1; CP[21 + i] = e[i] - 'A'; }
  for (int i = 0; i < 11; i++) { ISC[63 + i] = 1; CP[63 + i] = b[i] - 'A'; }
  uint64_t s = seed * 0x9E3779B97F4A7C15ULL + 12345;
#define RNG (s ^= s << 13, s ^= s >> 7, s ^= s << 17, s)
  for (int i = 0; i < 97; i++) CT[i] = K4[i] - 'A';
  if (nullmode == 1) for (int i = 0; i < 97; i++) CT[i] = RNG % 26;
  if (nullmode == 2) for (int i = 96; i > 0; i--) { int j = RNG % (i + 1); int x = CT[i]; CT[i] = CT[j]; CT[j] = x; }
  if (nullmode == 3 || nullmode == 4) {  /* contrôle positif : clair anglais (Carter, en partie), PALIMPSEST, L = 7, VIG, Q3, clé en σ */
    const char *eng = "WHENTHEYHADFINISHEDTHEYWENTBACKTOTHECITYTHROUGHTHEOLDGATEANDEASTNORTHEASTOFTHEWALLTHEYFOUNDTHEBERLINCLOCKSTANDINGALONEINTHESQUAREBYNIGHT";
    int pt[97]; for (int i = 0; i < 97; i++) pt[i] = eng[i] - 'A';
    for (int i = 0; i < 13; i++) pt[21 + i] = e[i] - 'A'; for (int i = 0; i < 11; i++) pt[63 + i] = b[i] - 'A';
    int S[26], SI[26]; mkalpha("PALIMPSEST", 0, 0, S); for (int i = 0; i < 26; i++) SI[S[i]] = i;
    for (int i = 0; i < 97; i++) { int k = i >= 7 ? S[pt[i - 7]] : (int)(RNG % 26); CT[i] = SI[md(S[pt[i]] + k)]; }
    if (nullmode == 3) CT[73] = pt[73];   /* erreur « copie » : le K clair recopié tel quel */
    else CT[42] = (CT[42] + 7) % 26;      /* mode 4 : une lettre fausse au milieu d'une chaîne (hors cribs) */
  }
  for (int i = 0; i < 97; i++) COPY[i] = ISC[i] && CT[i] == CP[i];
  for (int L = 1; L <= LMAX; L++) for (int bb = 0; bb < 97; bb++) { PREV[L][bb] = -1;
    if (!ISC[bb]) continue; for (int a = bb - L; a >= 0; a -= L) if (ISC[a]) { PREV[L][bb] = a; break; } }
  printf("mode %d graine %llu ; positions « copie » :", nullmode, (unsigned long long)seed); for (int i = 0; i < 97; i++) if (COPY[i]) printf(" %d", i); printf("\n");

  /* alphabets dédoublonnés */
  FILE *wf = fopen(argv[2], "r"); char w[64]; int cap = 1 << 20, na = 0; int (*AL)[26] = malloc(sizeof(int[26]) * cap); char (*NM)[40] = malloc(40 * cap);
  uint64_t *H = calloc(1 << 22, 8);
  while (fscanf(wf, "%63s", w) == 1) for (int cont = 0; cont < 2; cont++) for (int rev = 0; rev < 2; rev++) {
    int S[26]; mkalpha(w, cont, rev, S); uint64_t h = 1469598103934665603ULL; for (int i = 0; i < 26; i++) h = (h ^ S[i]) * 1099511628211ULL;
    uint64_t k = h | 1; size_t j = k & ((1 << 22) - 1); int dup = 0;
    while (H[j]) { if (H[j] == k) { dup = 1; break; } j = (j + 1) & ((1 << 22) - 1); }
    if (dup) continue; H[j] = k; if (na == cap) break;
    memcpy(AL[na], S, sizeof S); snprintf(NM[na], 40, "%.30s/%d%d", w, cont, rev); na++;
  }
  fclose(wf);
  printf("alphabets distincts : %d ; threads : %d\n", na, omp_get_max_threads());
  int AZ[26], KAP[26]; for (int i = 0; i < 26; i++) { AZ[i] = i; KAP[KA[i] - 'A'] = i; }
  static const char *TN[5] = {"Q3", "Q2", "Q1", "Q4a", "Q4b"}, *MN[3] = {"VIG", "BEAU", "VARB"}, *ZN[3] = {"σ", "AZ", "KA"};
  long hist7[20] = {0}, histall[20] = {0}, ncase = 0, n7 = 0, copy7 = 0, copyall = 0; double best = -99; char bestd[200] = "", bestpt[98] = "";
#pragma omp parallel
  {
    long h7[20] = {0}, ha[20] = {0}, nc = 0, c7 = 0, ca = 0, m7 = 0; double bq = -99; char bd[200] = "", bp[98] = "";
#pragma omp for schedule(dynamic, 64)
    for (int ai = 0; ai < na; ai++) {
      const int *S = AL[ai];
      for (int ty = 0; ty < 5; ty++) {
        Ctx c; c.Y = ty == 0 ? S : ty == 1 ? AZ : ty == 2 ? S : ty == 3 ? S : KAP;
        c.X = ty == 0 ? S : ty == 1 ? S : ty == 2 ? AZ : ty == 3 ? KAP : S;
        for (int i = 0; i < 26; i++) c.YI[c.Y[i]] = i;
        for (int zr = 0; zr < 3; zr++) {
          c.Z = zr == 0 ? c.Y : zr == 1 ? AZ : KAP; if (zr > 0 && c.Z == c.Y) continue;
          for (int i = 0; i < 26; i++) c.ZI[c.Z[i]] = i;
          for (int mo = 0; mo < 3; mo++) { c.mo = mo;
            for (int L = 1; L <= LMAX; L++) {
              int last = -1, neq, F = chains(&c, L, &last, &neq);
              if (neq < 8) continue;                      /* trop peu d'équations : sans pouvoir */
              nc++; int Fc = F > 19 ? 19 : F; ha[Fc]++; if (L == 7) { h7[Fc]++; m7++; }
              int cp = F == 1 && COPY[last];
              if (cp) { ca++; if (L == 7) c7++; }
              if (F <= 1 && L <= 13) {
                char pt[98]; double q = fullpt(&c, L, pt);
                if (q > bq) { bq = q; snprintf(bd, sizeof bd, "%s %s %s clé-%s L=%d F=%d%s", NM[ai], TN[ty], MN[mo], ZN[zr], L, F, cp ? " (copie)" : ""); memcpy(bp, pt, 98); }
                if (L == 7 || q > -5.2) {
#pragma omp critical
                  printf("CAS %s %s %s clé-%s L=%d : F=%d%s (fausse en %d) ; ancien comptage %d ; %d équations ; quadri %.3f %s\n", NM[ai], TN[ty], MN[mo], ZN[zr], L, F,
                         cp ? " copie" : "", F ? last : -1, oldcount(&c, L), neq, q, pt);
                }
              }
            }
          }
        }
      }
    }
#pragma omp critical
    { for (int i = 0; i < 20; i++) { hist7[i] += h7[i]; histall[i] += ha[i]; } ncase += nc; n7 += m7; copy7 += c7; copyall += ca;
      if (bq > best) { best = bq; strcpy(bestd, bd); memcpy(bestpt, bp, 98); } }
  }
  printf("cas (≥ 8 équations) : %ld, dont L = 7 : %ld\n", ncase, n7);
  printf("L = 7, histogramme de F (erreurs minimales) :"); for (int i = 0; i < 20; i++) if (hist7[i]) printf(" %d:%ld", i, hist7[i]); printf("\n");
  printf("tous L, histogramme de F :"); for (int i = 0; i < 20; i++) if (histall[i]) printf(" %d:%ld", i, histall[i]); printf("\n");
  printf("sauvés par une seule « copie » : L = 7 : %ld ; tous L : %ld\n", copy7, copyall);
  printf("meilleur clair (F ≤ 1, L ≤ 13) : %.3f %s %s\n", best, bestd, bestpt);
  return 0;
}
