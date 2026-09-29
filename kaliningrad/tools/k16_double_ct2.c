/* K16 — double transposition à grandes clés, IDP porté de CrypTool 2 (Lasry)
   Réimplémentation en C de l'évaluation IDP et des mouvements de CrypPlugins/IDPAttack/IDPAnalyser.cs (CrypTool 2,
   https://github.com/CrypToolProject/CrypTool-2, licence Apache 2.0) ; méthode : Lasry, Kopal & Wacker, Cryptologia 38(3), 2014. : recuit sur K2 noté par le potentiel de juxtaposition des colonnes (IDP, bigrammes
   allemands), puis recuit sur K1 (quadrigrammes). Voir experiments/K14_double_grandes_cles/PREREGISTRATION.md.
   Compilation : gcc -O2 -o k14 tools/k14_double_idp.c -lm
   Usage :
     k14 ctrl  <qg.bin> <texte_reserve> <nessais> <wmin> <wmax> <connues 0/1> <R2> <I2> <R1> <I1> <graine>
     k14 solve <qg.bin> <lettres> <wmin> <wmax> <R2> <I2> <R1> <I1> <graine>
     k14 null  <qg.bin> <lettres> <nnull> <wmin> <wmax> <R2> <I2> <R1> <I1> <graine> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static float *QG; static double BG[26][26];
static unsigned long long rs;
static unsigned long long rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static int rint_(int n) { return (int)(rnd() % (unsigned long long)n); }
static double T2A = 0.05, T2B = 0.002;
static double rnd01(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
#define D 3

static void mapping(int n, int w, const int *perm, int *pos) {
    int h = n / w, r = n % w, start[64], k = 0;
    for (int j = 0; j < w; j++) { int col = perm[j]; start[col] = k; k += h + (col < r); }
    for (int i = 0; i < n; i++) pos[i] = start[i % w] + i / w;
}
static void encrypt2(const int *p, int n, int w1, const int *k1, int w2, const int *k2, int *c) {
    int p1[2048], p2[2048], t[2048]; mapping(n, w1, k1, p1); mapping(n, w2, k2, p2);
    for (int i = 0; i < n; i++) t[p1[i]] = p[i];
    for (int j = 0; j < n; j++) c[p2[j]] = t[j];
}
static void undo(const int *c, int n, int w, const int *k, int *t) { int p[2048]; mapping(n, w, k, p); for (int j = 0; j < n; j++) t[j] = c[p[j]]; }

static double quad(const int *p, int n) { double s = 0; for (int i = 0; i + 3 < n; i++) s += QG[((p[i] * 26 + p[i + 1]) * 26 + p[i + 2]) * 26 + p[i + 3]]; return s / (n - 3); }

/* IDP, portage fidèle de CrypTool 2 (IDPAnalyser.cs, Lasry) : plages exactes de fin de colonne, fenêtre glissante,
   puis score de matrice par chaîne gloutonne (chaque colonne a au plus un voisin gauche et un droit, sans cycle). */
static int MINE[64], MAXE[64], CURW = -1, CURN = -1;
static void colends(int n, int w) {
    int full = n / w, nl = n % w;
    for (int i = 0; i < w; i++) { MINE[i] = full * (i + 1) - 1; MAXE[i] = (i < nl) ? full * (i + 1) + i : MINE[i] + nl; }
    for (int i = 0; i < w; i++) {
        int idx = w - 1 - i;
        if (MAXE[idx] > n - 1 - full * i) MAXE[idx] = n - 1 - full * i;
        if (i < nl) { int v = n - 1 - full * i - i; if (MINE[idx] < v) MINE[idx] = v; }
        else { int v = MAXE[idx] - nl; if (MINE[idx] < v) MINE[idx] = v; }
    }
    CURW = w; CURN = n;
}
static double chain_score(double m[64][64], int d) {
    int left[64], right[64]; for (int i = 0; i < d; i++) left[i] = right[i] = -1;
    double sum = 0;
    for (int it = 1; it <= d; it++) {
        double best = -1e18; int b1 = -1, b2 = -1;
        for (int p1 = 0; p1 < d; p1++) {
            if (right[p1] != -1) continue;
            int inc[64] = {0};
            if (it != d) { int cur = p1; while (left[cur] != -1) { cur = left[cur]; inc[cur] = 1; } }
            for (int p2 = 0; p2 < d; p2++) {
                if (left[p2] != -1 || inc[p2] || p1 == p2) continue;
                if (m[p1][p2] > best) { best = m[p1][p2]; b1 = p1; b2 = p2; }
            }
        }
        if (b1 != -1) { sum += best; left[b2] = b1; right[b1] = b2; }
    }
    return sum / d;
}
static double idp(const int *t, int n, int w1) {
    if (CURW != w1 || CURN != n) colends(n, w1);
    static double m[64][64]; int full = n / w1;
    for (int c1 = 0; c1 < w1; c1++) for (int c2 = 0; c2 < w1; c2++) {
        if (c1 == c2) { m[c1][c2] = -1e18; continue; }
        double best = -1e18;
        if (n % w1 == 0) {
            int p1 = MINE[c1], p2 = MINE[c2]; double s = 0;
            for (int l = 0; l < full; l++) s += BG[t[p1 - l]][t[p2 - l]];
            best = s;
        } else {
            int s1 = MINE[c1] - full + 1, s2 = MINE[c2] - full + 1, o1 = MAXE[c1] - MINE[c1], o2 = MAXE[c2] - MINE[c2];
            for (int pass = 0; pass < 2; pass++) {
                int omax = pass == 0 ? o2 : o1;
                for (int off = pass; off <= omax; off++) {
                    int p1 = s1 + (pass ? off : 0), p2 = s2 + (pass ? 0 : off); double s = 0; int a = p1, b = p2;
                    if (a < 0 || b < 0 || a + full > n || b + full > n) continue;
                    for (int i = 0; i < full; i++) s += BG[t[a + i]][t[b + i]];
                    if (s > best) best = s;
                    int q1 = a + full, q2 = b + full, k = 0;
                    while (q1 <= MAXE[c1] && q2 <= MAXE[c2]) { s += BG[t[q1]][t[q2]] - BG[t[a + k]][t[b + k]]; k++; q1++; q2++; if (s > best) best = s; }
                }
            }
        }
        m[c1][c2] = best / full;
    }
    return chain_score(m, w1);
}

static void randperm(int *p, int w) { for (int i = 0; i < w; i++) p[i] = i; for (int i = w - 1; i > 0; i--) { int j = rint_(i + 1), x = p[i]; p[i] = p[j]; p[j] = x; } }
static void mutate(int *k, int w) {
    int a = rint_(w), b = rint_(w); if (a == b) return;
    int mv = rint_(3);
    if (mv == 0) { int x = k[a]; k[a] = k[b]; k[b] = x; }
    else if (mv == 1) { if (a > b) { int x = a; a = b; b = x; } while (a < b) { int x = k[a]; k[a] = k[b]; k[b] = x; a++; b--; } }
    else { int v = k[a]; if (a < b) memmove(k + a, k + a + 1, sizeof(int) * (b - a)); else memmove(k + b + 1, k + b, sizeof(int) * (a - b)); k[b] = v; }
}
static void mutate_ct2(int *k, int w) {       /* répertoire de mouvements de CrypTool 2 */
    int r = rint_(100), tmp[64];
    if (r < 50) { int n = 1 + rint_(9); for (int i = 0; i < n; i++) { int a = rint_(w), b = rint_(w), x = k[a]; k[a] = k[b]; k[b] = x; } }
    else if (r < 70) { int n = 1 + rint_(2); for (int j = 0; j < n; j++) { int l = 1 + rint_(w - 1), f = rint_(w), t = (f + l + rint_(w - l > 0 ? w - l : 1)) % w;
                        for (int i = 0; i < l; i++) { int x = k[(f + i) % w]; k[(f + i) % w] = k[(t + i) % w]; k[(t + i) % w] = x; } } }
    else if (r < 90) { int l = 1 + rint_(w - 1), f = rint_(w), t = (f + 1 + rint_(w - 1)) % w; memcpy(tmp, k, sizeof(int) * w);
                        int t0 = (t - f + w) % w, nn = (t0 + l) % w; for (int i = 0; i < nn; i++) { int ff = (f + i) % w, tt = (((t0 + i) % nn) + f) % w; k[tt] = tmp[ff]; } }
    else { int p = 1 + rint_(w - 1); memcpy(tmp, k, sizeof(int) * w); memcpy(k, tmp + p, sizeof(int) * (w - p)); memcpy(k + w - p, tmp, sizeof(int) * p); }
}
static double stage2(const int *c, int n, int w1, int w2, int R, int I, int *bk2) {
    int k[64], q[64], t[2048]; double best = -1e18;
    for (int r = 0; r < R; r++) {
        randperm(k, w2); undo(c, n, w2, k, t); double cur = idp(t, n, w1);
        for (int it = 0; it < I; it++) {
            memcpy(q, k, sizeof(int) * w2); mutate_ct2(q, w2); undo(c, n, w2, q, t); double s = idp(t, n, w1);
            double T = T2A * pow(T2B / T2A, (double)it / I);
            if (s >= cur || rnd01() < exp((s - cur) / T)) { cur = s; memcpy(k, q, sizeof(int) * w2); if (cur > best) { best = cur; memcpy(bk2, k, sizeof(int) * w2); } }
        }
    }
    return best;
}
static double stage1(const int *t, int n, int w1, int R, int I, int *bk1, int *out) {
    int k[64], q[64], p[2048], pos[2048]; double best = -1e18;
    for (int r = 0; r < R; r++) {
        randperm(k, w1); mapping(n, w1, k, pos); for (int i = 0; i < n; i++) p[i] = t[pos[i]]; double cur = quad(p, n);
        for (int it = 0; it < I; it++) {
            memcpy(q, k, sizeof(int) * w1); mutate(q, w1); mapping(n, w1, q, pos); for (int i = 0; i < n; i++) p[i] = t[pos[i]];
            double s = quad(p, n), T = 0.3 * pow(0.005 / 0.3, (double)it / I);
            if (s >= cur || rnd01() < exp((s - cur) / T)) { cur = s; memcpy(k, q, sizeof(int) * w1); if (cur > best) { best = cur; memcpy(bk1, k, sizeof(int) * w1); memcpy(out, p, sizeof(int) * n); } }
        }
    }
    return best;
}

/* recherche sur les paires de largeurs. Mode criblage (K16_SCREEN=Rs,Is,top) : passage rapide sur toutes les paires, puis
   recherche profonde (R2, I2) seulement sur les « top » paires au meilleur IDP. */
static double full(const int *c, int n, int wmin, int wmax, int kw1, int kw2, int R2, int I2, int R1, int I1,
                   int *bw1, int *bw2, int *bk1, int *bk2, int *bout) {
    double best = -1e18; int k1[64], k2[64], t[2048], out[2048];
    int pw1[512], pw2[512], np = 0; double pid[512];
    for (int w1 = (kw1 ? kw1 : wmin); w1 <= (kw1 ? kw1 : wmax); w1++)
        for (int w2 = (kw2 ? kw2 : wmin); w2 <= (kw2 ? kw2 : wmax); w2++) { pw1[np] = w1; pw2[np] = w2; pid[np] = 0; np++; }
    int Rs = 0, Is = 0, top = np;
    if (getenv("K16_SCREEN")) sscanf(getenv("K16_SCREEN"), "%d,%d,%d", &Rs, &Is, &top);
    if (Rs > 0 && top < np) {
        for (int i = 0; i < np; i++) {
            /* normalisation : niveau de hasard de l'IDP pour cette paire de largeurs (40 clés K2 aléatoires) */
            double m = 0, m2 = 0; int rk[64], tt[2048];
            for (int r = 0; r < 40; r++) { randperm(rk, pw2[i]); undo(c, n, pw2[i], rk, tt); double v = idp(tt, n, pw1[i]); m += v; m2 += v * v; }
            m /= 40; double sd = sqrt(m2 / 40 - m * m) + 1e-9;
            pid[i] = (stage2(c, n, pw1[i], pw2[i], Rs, Is, k2) - m) / sd;
        }
        for (int i = 0; i < np; i++) for (int j = i + 1; j < np; j++) if (pid[j] > pid[i]) {
            double x = pid[i]; pid[i] = pid[j]; pid[j] = x; int a = pw1[i]; pw1[i] = pw1[j]; pw1[j] = a; a = pw2[i]; pw2[i] = pw2[j]; pw2[j] = a; }
        if (getenv("K16_VERBOSE")) { fprintf(stderr, "criblage :"); for (int i = 0; i < 8 && i < np; i++) fprintf(stderr, " (%d,%d)%.3f", pw1[i], pw2[i], pid[i]); fprintf(stderr, "\n"); }
        np = top;
    }
    for (int i = 0; i < np; i++) {
        int w1 = pw1[i], w2 = pw2[i];
        stage2(c, n, w1, w2, R2, I2, k2); undo(c, n, w2, k2, t);
        double s = stage1(t, n, w1, R1, I1, k1, out);
        if (s > best) { best = s; *bw1 = w1; *bw2 = w2; memcpy(bk1, k1, sizeof k1); memcpy(bk2, k2, sizeof k2); memcpy(bout, out, sizeof(int) * n); }
    }
    return best;
}

static int load_letters(const char *s, int *out, int max) {
    int n = 0; for (; *s && n < max; s++) if (*s >= 'a' && *s <= 'z') out[n++] = *s - 'a'; else if (*s >= 'A' && *s <= 'Z') out[n++] = *s - 'A';
    return n;
}

int main(int argc, char **argv) {
    if (argc < 4) return 1;
    if (getenv("K14_T2A")) T2A = atof(getenv("K14_T2A")); if (getenv("K14_T2B")) T2B = atof(getenv("K14_T2B"));
    QG = malloc(sizeof(float) * 456976); FILE *fp = fopen(argv[2], "rb");
    if (!fp || fread(QG, sizeof(float), 456976, fp) != 456976) { fprintf(stderr, "qg ?\n"); return 1; } fclose(fp);
    for (int a = 0; a < 26; a++) for (int b = 0; b < 26; b++) { double s = 0; for (int c = 0; c < 676; c++) s += exp(QG[(a * 26 + b) * 676 + c]); BG[a][b] = log(s); }
    if (!strcmp(argv[1], "ctrl")) {
        fp = fopen(argv[3], "rb"); fseek(fp, 0, SEEK_END); long L = ftell(fp); fseek(fp, 0, SEEK_SET);
        char *tx = malloc(L + 1); if (fread(tx, 1, L, fp) != (size_t)L) return 2; tx[L] = 0; fclose(fp);
        int *txt = malloc(sizeof(int) * (L + 1)); int M = load_letters(tx, txt, L);
        int ne = atoi(argv[4]), wmin = atoi(argv[5]), wmax = atoi(argv[6]), known = atoi(argv[7]);
        int R2 = atoi(argv[8]), I2 = atoi(argv[9]), R1 = atoi(argv[10]), I1 = atoi(argv[11]); rs = strtoull(argv[12], 0, 10) * 2654435761ULL + 41;
        int n = 979, ok = 0;
        for (int e = 0; e < ne; e++) {
            int p[2048], c[2048], k1[64], k2[64], b1[64], b2[64], out[2048], bw1, bw2, idx[2048], cidx[2048], t[2048], oidx[2048], pos[2048];
            int o = rint_(M - n); memcpy(p, txt + o, sizeof(int) * n);
            int w1 = wmin + rint_(wmax - wmin + 1), w2 = wmin + rint_(wmax - wmin + 1); randperm(k1, w1); randperm(k2, w2);
            encrypt2(p, n, w1, k1, w2, k2, c);
            double s = full(c, n, wmin, wmax, known ? w1 : 0, known ? w2 : 0, R2, I2, R1, I1, &bw1, &bw2, b1, b2, out);
            for (int i = 0; i < n; i++) idx[i] = i;
            encrypt2(idx, n, w1, k1, w2, k2, cidx); undo(cidx, n, bw2, b2, t); mapping(n, bw1, b1, pos);
            for (int i = 0; i < n; i++) oidx[i] = t[pos[i]];
            int good = 0; for (int i = 0; i + 1 < n; i++) good += oidx[i + 1] == oidx[i] + 1 || oidx[i + 1] == oidx[i] - 1;
            int k2ok = 1; for (int j = 0; j < w2 && bw2 == w2; j++) if (b2[j] != k2[j]) k2ok = 0;
            ok += good >= 0.9 * (n - 1);
            printf("essai %d : vrai w=%d,%d | trouvé w=%d,%d K2 %s score=%.4f (vrai %.4f) contacts=%d/%d\n", e, w1, w2, bw1, bw2,
                   (bw2 == w2 && k2ok) ? "exacte" : "fausse", s, quad(p, n), good, n - 1);
            fflush(stdout);
        }
        printf("SUCCES %d/%d (w=%d..%d, largeurs %s, R2=%d I2=%d R1=%d I1=%d)\n", ok, ne, wmin, wmax, known ? "connues" : "cherchées", R2, I2, R1, I1);
    } else {
        int c[2048], n = load_letters(argv[3], c, 2048), isnull = !strcmp(argv[1], "null"), a = isnull ? 5 : 4;
        int nn = isnull ? atoi(argv[4]) : 1, wmin = atoi(argv[a]), wmax = atoi(argv[a + 1]);
        int R2 = atoi(argv[a + 2]), I2 = atoi(argv[a + 3]), R1 = atoi(argv[a + 4]), I1 = atoi(argv[a + 5]); rs = strtoull(argv[a + 6], 0, 10) * 2654435761ULL + 43;
        for (int k = 0; k < nn; k++) {
            int cc[2048], b1[64], b2[64], out[2048], bw1, bw2; memcpy(cc, c, sizeof(int) * n);
            if (isnull) for (int i = n - 1; i > 0; i--) { int j = rint_(i + 1), x = cc[i]; cc[i] = cc[j]; cc[j] = x; }
            double s = full(cc, n, wmin, wmax, 0, 0, R2, I2, R1, I1, &bw1, &bw2, b1, b2, out);
            printf("%s %d score=%.4f w=%d,%d ", isnull ? "NUL" : "REEL", k, s, bw1, bw2);
            for (int i = 0; i < n && i < 160; i++) putchar('a' + out[i]);
            putchar('\n'); fflush(stdout);
        }
    }
    return 0;
}
