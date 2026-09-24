"""stats_clef.py — structure de la clé déduite des cribs, alphabet fixé (audit « vision », 24/09/2026)

Pour chaque convention (A–Z ou KRYPTOS ; Vigenère ou Beaufort ; Variante = −Vigenère, même statistique),
la clé aux 24 positions de cribs est entièrement déterminée. On mesure :
  IC de la clé (valeurs répétées) ; égalités proches (même valeur à distance ≤ 3 dans un même crib).
Témoin : chiffrés aléatoires tirés des lettres de K4 (200 000), MAXIMUM sur les 4 conventions (correction du choix).
"""
import numpy as np
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
POS = list(range(21, 34)) + list(range(63, 74)); PT = "EASTNORTHEAST" + "BERLINCLOCK"
AZI = {ch: i for i, ch in enumerate("ABCDEFGHIJKLMNOPQRSTUVWXYZ")}; KAI = {ch: i for i, ch in enumerate(KA)}
CONV = [(n, al, m) for n, al in (("A-Z", AZI), ("KRYPTOS", KAI)) for m in ("VIG", "BEAU")]
def keys(ct, al, m):
    return np.array([(al[ct[i]] - al[p]) % 26 if m == "VIG" else (al[ct[i]] + al[p]) % 26 for i, p in zip(POS, PT)])
def ic(k):
    c = np.bincount(k, minlength=26); return (c * (c - 1)).sum() / (len(k) * (len(k) - 1))
def near(k):
    return sum(1 for a in range(24) for b in range(a + 1, min(a + 4, 24)) if (a < 13) == (b < 13) and k[a] == k[b])
for n, al, m in CONV:
    k = keys(K4, al, m)
    print(f"{n:8s} {m}: clé {''.join(chr(65 + x) for x in k[:13])} | {''.join(chr(65 + x) for x in k[13:])} ; IC {ic(k):.4f} ; égalités proches {near(k)}")
rng = np.random.default_rng(1); L = list(K4); N = 200000
kic = max(ic(keys(K4, al, m)) for _, al, m in CONV); knr = max(near(keys(K4, al, m)) for _, al, m in CONV)
kb = ic(keys(K4, AZI, "BEAU")); nb = near(keys(K4, AZI, "BEAU"))
h1 = h2 = h3 = h4 = 0
for _ in range(N):
    ct = "".join(rng.choice(L, 97)); ks = [keys(ct, al, m) for _, al, m in CONV]
    h1 += max(ic(k) for k in ks) >= kic; h2 += max(near(k) for k in ks) >= knr
    h3 += ic(ks[1]) >= kb; h4 += near(ks[1]) >= nb
print(f"Beaufort A–Z seul : P(IC >= {kb:.4f}) = {h3/N:.4f} ; P(égalités proches >= {nb}) = {h4/N:.5f}")
print(f"Maximum sur 4 conventions : P(IC >= {kic:.4f}) = {h1/N:.4f} ; P(égalités proches >= {knr}) = {h2/N:.5f}")
