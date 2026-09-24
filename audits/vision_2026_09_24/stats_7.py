# stats_7.py — le « 7 » de K4 est-il du hasard ? (24/09/2026)
#
# Trois anomalies « 7 » signalées séparément, par trois sources indépendantes :
#   D : doublets de lettres en position ≡ 4 (mod 7) — J. Gillogly, archive du groupe, 2005 ;
#   C : coïncidences à l'écart 7 (c[i] = c[i+7]) — la « rugosité à l'intervalle 7 » du mémo NSA de 1992 ;
#   V : bigrammes verticaux répétés en largeur 21 = 3 × 7 — relevé communautaire (base 4, phénomène 2).
# Hypothèse nulle : K4 mélangé (mêmes lettres, positions échangeables). 100 000 mélanges.
# Correction du choix du module : pour chaque texte, on prend le MEILLEUR module m de 2 à 24
# (D mod m, C à l'écart m, V en largeur 3m ≤ 48), puis on compare K4 à la même procédure sur les mélanges.
import numpy as np

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c = np.array([ord(x) - 65 for x in K4])
rng = np.random.default_rng(99)
N = 100000
S = np.vstack([c[None, :], np.array([rng.permutation(c) for _ in range(N)])])   # ligne 0 = K4

def vert(a, w):
    """paires (i<j) avec (a[i], a[i+w]) == (a[j], a[j+w])"""
    code = a[:, :-w] * 26 + a[:, w:]
    out = np.zeros(a.shape[0], int)
    for x in range(676):
        n = (code == x).sum(1); out += n * (n - 1) // 2
    return out

def pv(x):
    """p unilatérale de chaque texte sous la loi nulle empirique (lignes 1..N)"""
    ref = np.sort(x[1:]); return 1 - np.searchsorted(ref, x, side='left') / N

M = list(range(2, 25))
dbl = (S[:, 1:] == S[:, :-1]); nd = dbl.sum(1); pos = np.arange(96)
PD, PC, PV = {}, {}, {}
for m in M:
    cnt = np.stack([dbl[:, pos % m == r].sum(1) for r in range(m)], 1)
    PD[m] = pv(np.where(nd >= 4, cnt.max(1) / np.maximum(nd, 1), 0.0))   # part max des doublets dans une classe
    PC[m] = pv((S[:, m:] == S[:, :-m]).sum(1))
    PV[m] = pv(vert(S, 3 * m)) if 3 * m <= 48 else np.ones(N + 1)

print("K4 : doublets", [i for i in range(96) if K4[i] == K4[i + 1]], "; coïncidences à l'écart 7 :",
      int((c[7:] == c[:-7]).sum()), "; bigrammes verticaux répétés en largeur 21 :", int(vert(c[None, :], 21)[0]))
print(f"module 7 seul : p(D) = {PD[7][0]:.5f} ; p(C) = {PC[7][0]:.5f} ; p(V) = {PV[7][0]:.5f}")
print(f"corrélations sous le nul (module 7) : D/C {np.corrcoef(PD[7][1:], PC[7][1:])[0,1]:+.3f} ; D/V {np.corrcoef(PD[7][1:], PV[7][1:])[0,1]:+.3f} ; C/V {np.corrcoef(PC[7][1:], PV[7][1:])[0,1]:+.3f}")
for name, P in [("D seul", PD), ("C seul", PC), ("V seul", PV)]:
    arr = np.stack([P[m] for m in M]); sc = arr.min(0)
    print(f"{name}, meilleur module 2–24 : p corrigée = {np.mean(sc[1:] <= sc[0]):.5f}")
for name, f in [("D × C", lambda m: PD[m] * PC[m]), ("D × V", lambda m: PD[m] * PV[m]),
                ("C × V", lambda m: PC[m] * PV[m]), ("D × C × V", lambda m: PD[m] * PC[m] * PV[m])]:
    arr = np.stack([f(m) for m in M]); sc = arr.min(0); best = M[int(np.argmin(arr[:, 0]))]
    print(f"{name} : meilleur module pour K4 = {best} ; p corrigée = {np.mean(sc[1:] <= sc[0]):.5f} ({int(np.sum(sc[1:] <= sc[0]))}/{N})")
# Conséquences directes, sans statistique : deux coïncidences à l'écart 7 tombent sur les cribs.
#   c[15] = c[22] = L avec clair[22] = A ; c[65] = c[72] = P avec clair[65] = R, clair[72] = C.
#   (65, 72) : même lettre chiffrée, même classe mod 7, clairs différents => aucune substitution de période 7 (ou diviseur).
#   Autoclé sur le chiffré à l'écart 7 (additive, alphabet quelconque) : c[i] = c[i-7] <=> σ(clair[i]) = 0,
#   donc σ(A) = σ(C) = 0 : impossible.
