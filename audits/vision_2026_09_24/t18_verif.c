/* t18_autocle_chaines.c — autoclé sur le CLAIR, avec les CHAÎNES entre cribs (audit « vision », 24/09/2026)
 *
 * Hypothèse de la NSA (1992) pour la « rugosité à l'intervalle 7 » : autoclé sur le clair. C'est aussi la famille
 * simulée qui reproduit le mieux la signature « 7 » de K4 (sim_7_chaines.py).
 * Les tests précédents (campagne du 22/09, T12, T17) n'utilisaient que les paires (i−L, i) toutes deux dans les cribs.
 * Or, dans l'alphabet σ du tableau, la relation est LINÉAIRE et se propage de L en L à travers le clair inconnu :
 *   VIG  : σp_{j+L} = σc_{j+L} − σp_j      BEAU : σp_{j+L} = σp_j − σc_{j+L}      VARB : σp_{j+L} = σc_{j+L} + σp_j
 * d'où, pour deux positions de crib a < b avec b − a = mL (et aucune position de crib entre elles sur la chaîne) :
 *   VIG  : σp_b = Σ_{t=1..m} (−1)^{m−t} σc_{a+tL} + (−1)^m σp_a
 *   BEAU : σp_b = σp_a − Σ σc_{a+tL}         VARB : σp_b = σp_a + Σ σc_{a+tL}
 * Ces équations ne font intervenir que des lettres CONNUES (chiffré partout, clair aux cribs) : elles relient
 * EASTNORTHEAST à BERLINCLOCK dès que L divise l'écart. Sens « arrière » (clé = clair L avant, le cas classique) et
 * « avant » (clé = clair L après, T17) ; L = 1..96 ; alphabet σ QUELCONQUE, même alphabet pour la clé (Quagmire III).
 * Témoins : 100 chiffrés aléatoires de 97 lettres par cas compatible ; taux global. Contrôle positif : faux K4.
 */
#define LC_MAXT 40
#include "lincsp.h"

static int P97[K4LEN]; /* clair connu ou −1 */

/* ajoute l'équation de chaîne entre a et b = a + mL (sens arrière : la clé en j vient de j − L) */
static void add_chain(const int *ct, int a, int b, int L, int mode) {
  int vars[64], co[64], n = 0, m = (b - a) / L;
  vars[n] = P97[b]; co[n++] = 1;                       /* σp_b − [expression] = 0 */
  if (mode == VIG) {
    for (int t = 1; t <= m; t++) { vars[n] = ct[a + t * L]; co[n++] = ((m - t) % 2 == 0) ? -1 : 1; }
    vars[n] = P97[a]; co[n++] = (m % 2 == 0) ? -1 : 1;
  } else if (mode == BEAU) {
    vars[n] = P97[a]; co[n++] = -1;
    for (int t = 1; t <= m; t++) { vars[n] = ct[a + t * L]; co[n++] = 1; }
  } else {
    vars[n] = P97[a]; co[n++] = -1;
    for (int t = 1; t <= m; t++) { vars[n] = ct[a + t * L]; co[n++] = -1; }
  }
  lc_add(n, vars, co);
}

/* sens : 0 = arrière (clé = clair L avant) ; 1 = avant (clé = clair L après : on renverse le texte) */
static int ONLY_LOCAL = 0;
static int run(const int *ct0, int L, int mode, int dir, int *ncons) {
  int ct[K4LEN], pt[K4LEN];
  for (int i = 0; i < K4LEN; i++) { int j = dir ? K4LEN - 1 - i : i; ct[i] = ct0[j]; pt[i] = -1; }
  for (int q = 0; q < NCRIB; q++) { int i = dir ? K4LEN - 1 - CRIBPOS[q] : CRIBPOS[q]; pt[i] = CRIBPT[q]; }
  memcpy(P97, pt, sizeof pt);
  lc_reset(26); lc_limit = 5000000; int n = 0;
  for (int b = 0; b < K4LEN; b++) {
    if (P97[b] < 0) continue;
    for (int a = b - L; a >= 0; a -= L) { if (ONLY_LOCAL && a != b - L) break; if (P97[a] >= 0) { add_chain(ct, a, b, L, mode); n++; break; } }
  }
  *ncons = n;
  if (n == 0) return 1;
  return lc_solve();
}

int main(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  k4_init();
  /* vérifications : (1) sans chaînes (paires voisines seulement), L = 7 : doit redonner le résultat du 22/09 ;
     (2) contrôles positifs dans les 3 conventions et les 2 sens, 20 faux K4 chacun */
  for (int m = 0; m < 3; m++) for (int dir = 0; dir < 2; dir++) {
    int nc; ONLY_LOCAL = 1; int r1 = run(CT, 7, m, dir, &nc); int n1 = nc; ONLY_LOCAL = 0; int r2 = run(CT, 7, m, dir, &nc);
    printf("L=7 %s %s : sans chaînes %d (%d contraintes) ; avec chaînes %d (%d contraintes)\n", MODENAME[m], dir ? "avant" : "arrière", r1, n1, r2, nc);
  }
  for (int m = 0; m < 3; m++) for (int dir = 0; dir < 2; dir++) {
    int ok = 0;
    for (int trial = 0; trial < 20; trial++) {
      int s[26], inv[26], pt[K4LEN], ct[K4LEN];
      for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int x = s[i]; s[i] = s[j]; s[j] = x; }
      for (int i = 0; i < 26; i++) inv[s[i]] = i;
      for (int i = 0; i < K4LEN; i++) pt[i] = rng() % 26; for (int q = 0; q < NCRIB; q++) pt[CRIBPOS[q]] = CRIBPT[q];
      for (int i = 0; i < K4LEN; i++) {
        int src = dir ? i + 7 : i - 7;
        int k = (src >= 0 && src < K4LEN) ? s[pt[src]] : (int)(rng() % 26);
        ct[i] = m == VIG ? inv[md(s[pt[i]] + k)] : (m == BEAU ? inv[md(k - s[pt[i]])] : inv[md(s[pt[i]] - k)]);
      }
      int nc; ok += run(ct, 7, m, dir, &nc) > 0;
    }
    printf("contrôle positif L=7 %s %s : %d/20 reconnus\n", MODENAME[m], dir ? "avant" : "arrière", ok);
  }
  return 0;
}
