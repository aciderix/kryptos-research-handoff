"""amas_14x7.py — les lettres identiques forment-elles des amas sur la feuille 14 × 7 (K4 précédé du « ? ») ? (25/09/2026)
Composantes connexes de lettres identiques (voisinage orthogonal) ; on compte les amas d'au moins 3 cases, et la taille totale
des amas d'au moins 2 cases, contre 100 000 mélanges des lettres de K4 (« ? » en tête). Sans hypothèse de clé."""
import random
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
W = 7
def amas(s):
    n = len(s); seen = [False] * n; comps = []
    for i in range(n):
        if seen[i] or s[i] == '?': continue
        st = [i]; seen[i] = True; comp = [i]
        while st:
            j = st.pop(); r, c = divmod(j, W)
            for dr, dc in ((0, 1), (0, -1), (1, 0), (-1, 0)):
                rr, cc = r + dr, c + dc; k = rr * W + cc
                if 0 <= cc < W and 0 <= rr and k < n and not seen[k] and s[k] == s[i]:
                    seen[k] = True; st.append(k); comp.append(k)
        comps.append(comp)
    big3 = [c for c in comps if len(c) >= 3]
    return len(big3), sum(len(c) for c in comps if len(c) >= 2), big3
s = "?" + K4
n3, tot2, big = amas(s)
print("K4 (avec « ? ») : amas d'au moins 3 cases :", n3, [(s[c[0]], sorted(divmod(k, W) for k in c)) for c in big], "; cases dans des amas ≥ 2 :", tot2)
L = list(K4); random.seed(11); N = 100000; g3 = g2 = 0; m3 = m2 = 0
for _ in range(N):
    random.shuffle(L); a, b, _ = amas("?" + "".join(L)); g3 += a >= n3; g2 += b >= tot2; m3 += a; m2 += b
print(f"mélanges : amas ≥ 3 en moyenne {m3/N:.2f}, P(≥ {n3}) = {g3/N:.5f} ; cases en amas ≥ 2 en moyenne {m2/N:.2f}, P(≥ {tot2}) = {g2/N:.5f}")
