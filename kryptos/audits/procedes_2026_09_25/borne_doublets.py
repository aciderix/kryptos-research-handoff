"""borne_doublets.py — combien de doublets un chiffre DÉCHIFFRABLE et POSITIONNEL peut-il produire ? (25/09/2026)

Argument : si le chiffre est déchiffrable lettre à lettre (bijection p → c à état donné), alors, à état fixé,
c_{i+1} = c_i pour UNE SEULE valeur de p_{i+1}, notée f(contexte). Donc
    P(doublet en i) = P(p_{i+1} = f(contexte)).
Selon ce que l'état « sait » du clair, f est plus ou moins libre :
  (a) clé seule, alphabets à décalage (Quagmire, Vigenère…) : f(a) = σ⁻¹(σ(a) + d)   → « résonance » (meilleur d, meilleur σ)
  (b) clé seule, alphabets quelconques par position                     : f bijection quelconque (problème d'affectation)
  (c) état dépendant du clair précédent (autoclé, etc.)                 : f fonction quelconque de p_i  (resp. de p_{i-1} p_i)
On calcule ces bornes sur l'anglais (Gutenberg), avec et sans les contraintes des cribs (f(N)=O, f(S)=T, f(I)=N),
puis la probabilité d'obtenir ≥ 5 doublets sur les 14 cases de la colonne 4 de K4 (et ≤ 1 sur les 82 autres).
Usage : python3 borne_doublets.py corpus.txt alphas_kw.bin"""
import sys, re, numpy as np
from math import comb
from scipy.optimize import linear_sum_assignment
txt = re.sub("[^A-Z]", "", open(sys.argv[1], errors="ignore").read().upper())
E = np.frombuffer(txt.encode(), dtype=np.uint8) - 65
N = len(E)
B2 = np.zeros((26, 26)); np.add.at(B2, (E[:-1], E[1:]), 1); B2 /= B2.sum()           # P(a, b)
P1 = B2.sum(1)
B3 = np.zeros((26, 26, 26)); np.add.at(B3, (E[:-2], E[1:-1], E[2:]), 1); B3 /= B3.sum()
L = lambda s: ord(s) - 65
cons = {L('N'): L('O'), L('S'): L('T'), L('I'): L('N')}
def binom_tail(n, k, p): return sum(comb(n, j) * p**j * (1-p)**(n-j) for j in range(k, n+1))
print(f"corpus {N} lettres ; doublets anglais (a,a) : {np.trace(B2):.4f}")
res = []
# (a) résonance : alphabets A–Z, KRYPTOS, puis tous les alphabets à mot-clé
def reso(order, need=False):
    s = np.empty(26, int); s[[L(c) for c in order]] = np.arange(26)
    best = (0, None)
    for d in range(26):
        f = np.empty(26, int); inv = np.empty(26, int); inv[s] = np.arange(26); f = inv[(s + d) % 26]
        if need and any(f[a] != b for a, b in cons.items()): continue
        v = B2[np.arange(26), f].sum()
        if v > best[0]: best = (v, d)
    return best
for nm, al in (("A–Z", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"), ("KRYPTOS", "KRYPTOSABCDEFGHIJLMNQUVWXZ"), ("GIRASOL", "GIRASOLBCDEFHJKMNPQTUVWXYZ")):
    v, d = reso(al); vc, dc = reso(al, True)
    res.append((f"(a) résonance, alphabet {nm}", v, vc))
raw = open(sys.argv[2], "rb").read(); na = len(raw) // 26
bv, bvc, bw, bwc = 0, 0, "", ""
for k in range(na):
    al = raw[k*26:(k+1)*26].decode()
    v, _ = reso(al); vc, _ = reso(al, True)
    if v > bv: bv, bw = v, al
    if vc > bvc: bvc, bwc = vc, al
res.append((f"(a) résonance, meilleur des {na} alphabets à mot-clé", bv, bvc))
print("   meilleur alphabet à mot-clé :", bw, " ; avec contrainte des cribs :", bwc)
# (b) bijection quelconque : affectation maximale
r, c = linear_sum_assignment(-B2); vb = B2[r, c].sum()
M = B2.copy(); big = 1e3
for a, b in cons.items(): M[a, :] = -big; M[:, b] = -big; M[a, b] = B2[a, b]   # impose f(a)=b
r2, c2 = linear_sum_assignment(-M); vbc = B2[r2, c2].sum()
res.append(("(b) bijection quelconque (alphabets libres par position)", vb, vbc))
# (c) fonction de la lettre précédente, puis des deux précédentes
vf = B2.max(1).sum(); vfc = sum(B2[a].max() if a not in cons else B2[a, cons[a]] for a in range(26))
res.append(("(c1) fonction de p_i (autoclé, état dépendant du clair)", vf, vfc))
vf2 = B3.max(2).sum()
vf2c = sum(B3[x, a].max() if a not in cons else B3[x, a, cons[a]] for x in range(26) for a in range(26))
res.append(("(c2) fonction de p_{i-1} p_i", vf2, vf2c))
print(f"\n{'borne':62s} {'taux':>6s} {'P(≥5/14)':>9s} | {'avec cribs':>10s} {'P(≥5/14)':>9s}")
for nm, v, vc in res:
    print(f"{nm:62s} {v:6.3f} {binom_tail(14,5,v):9.4f} | {vc:10.3f} {binom_tail(14,5,vc):9.4f}")
# K4 : les 3 doublets de crib sont imposés ; il en faut 2 de plus sur les 11 autres cases de la colonne 4
print("\nK4 : 5 doublets sur 14 cases (dont 3 aux cribs, NO, ST, IN) ; hors colonne 4 : 1 sur 82.")
for nm, v, vc in res:
    pc = B2[L('N'), L('O')]/P1[L('N')] if False else None
print("P(≥2 doublets sur les 11 cases hors cribs | taux) :", ", ".join(f"{vc:.3f}→{binom_tail(11,2,vc):.3f}" for _, _, vc in res))
# fréquences conditionnelles des trois bigrammes des cribs
for a, b in cons.items():
    print(f"P({chr(b+65)} | {chr(a+65)}) = {B2[a,b]/P1[a]:.3f} ; meilleur successeur de {chr(a+65)} : {chr(B2[a].argmax()+65)} ({B2[a].max()/P1[a]:.3f})")
