/* selftest.c — contrôles des deux moteurs exacts (positifs, négatifs, reproduction du dépôt) */
#include "k4lib.h"
#include <time.h>

static int P24[NCRIB], C24[NCRIB];

static void rand_perm(int *s) { for (int i = 0; i < 26; i++) s[i] = i; for (int i = 25; i > 0; i--) { int j = rng() % (i + 1); int t = s[i]; s[i] = s[j]; s[j] = t; } }

static const char *K1PT = "BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION";

int main(void) {
  k4_init();
  for (int t = 0; t < NCRIB; t++) { P24[t] = CRIBPT[t]; C24[t] = CT[CRIBPOS[t]]; }
  int k[NCRIB], ok;

  /* 1. σ = A–Z identité, clé = clé de Vigenère/Beaufort réelle : doit être compatible */
  for (int m = 0; m < 3; m++) {
    for (int t = 0; t < NCRIB; t++) {
      int p = P24[t], c = C24[t];
      k[t] = m == VIG ? md(c - p) : m == BEAU ? md(c + p) : md(p - c);
    }
    printf("eng_num AZ-true-key %s : %d (attendu 1)\n", MODENAME[m], eng_num(P24, C24, k, NCRIB, m));
  }
  /* 2. témoins positifs aléatoires : σ aléatoire, clé numérique aléatoire, faux chiffré */
  int pos = 0, posl = 0, neg = 0, negl = 0, N = 2000;
  clock_t t0 = clock();
  for (int it = 0; it < N; it++) {
    int s[26], inv[26], m = it % 3; rand_perm(s); for (int i = 0; i < 26; i++) inv[s[i]] = i;
    int fc[NCRIB], kl[NCRIB];
    for (int t = 0; t < NCRIB; t++) {
      k[t] = rng() % 26; kl[t] = rng() % 26;
      int vp = s[P24[t]];
      fc[t] = inv[m == VIG ? md(vp + k[t]) : m == BEAU ? md(k[t] - vp) : md(vp - k[t])];
    }
    pos += eng_num(P24, fc, k, NCRIB, m);
    /* clé en lettres : c = σ^-1(σp + σk) */
    for (int t = 0; t < NCRIB; t++) {
      int vp = s[P24[t]], vk = s[kl[t]];
      fc[t] = inv[m == VIG ? md(vp + vk) : m == BEAU ? md(vk - vp) : md(vp - vk)];
    }
    posl += eng_let(P24, fc, kl, NCRIB, m);
    /* négatifs : vraie paire (P,C) de K4, clé aléatoire */
    for (int t = 0; t < NCRIB; t++) { k[t] = rng() % 26; kl[t] = rng() % 26; }
    neg += eng_num(P24, C24, k, NCRIB, m);
    negl += eng_let(P24, C24, kl, NCRIB, m);
  }
  printf("positifs eng_num %d/%d ; eng_let %d/%d\n", pos, N, posl, N);
  printf("clés aléatoires compatibles avec K4 : eng_num %d/%d ; eng_let %d/%d  (%.2fs)\n", neg, N, negl, N, (double)(clock() - t0) / CLOCKS_PER_SEC);

  /* 3. reproduction du dépôt : clé courante = clair de K1, tous décalages, QIII tout alphabet => aucun */
  int L = strlen(K1PT), hits = 0, tried = 0;
  for (int off = -21; off + 73 < L; off++) {
    int ok2 = 1; for (int t = 0; t < NCRIB; t++) { int j = CRIBPOS[t] + off; if (j < 0 || j >= L) { ok2 = 0; break; } k[t] = K1PT[j] - 'A'; }
    if (!ok2) continue;
    for (int m = 0; m < 3; m++) { tried++; hits += eng_let(P24, C24, k, NCRIB, m); }
  }
  printf("clé courante K1 (QIII tout alphabet) : %d compatibles sur %d (dépôt : 0)\n", hits, tried);
  (void)ok;
  return 0;
}
