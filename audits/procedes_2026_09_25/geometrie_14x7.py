"""geometrie_14x7.py — où sont les répétitions de K4 dans une grille de 7 colonnes ? (25/09/2026, nuit ; sans famille de clés)
K4 précédé du « ? » fait 98 = 14 × 7 caractères : un rectangle plein. On compte, pour chaque direction de voisinage, les paires
de lettres égales, et on compare à 100 000 mélanges des lettres de K4 (le « ? » reste en tête). Aussi sans le « ? ».
Résultat : seules deux relations dépassent le hasard, toutes deux orthogonales :
  - la lettre du dessous = la lettre du dessus (9 contre 3,25 ; p = 0,005), répartie sur les 7 colonnes ;
  - la dernière paire de chaque ligne (colonnes 5 → 6 avec le « ? ») : 5 contre 0,5 (p = 6 × 10⁻⁵ avant correction).
  Aucune diagonale (2 et 3 contre 2,8), rien à deux cases (2 contre 2,5 et 3,0)."""
import random
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
DIRS = {"droite": (0, 1), "dessous": (1, 0), "diag. bas-droite": (1, 1), "diag. bas-gauche": (1, -1), "deux à droite": (0, 2), "deux dessous": (2, 0)}
def rel(s, W=7):
    n = len(s); H = (n + W - 1) // W; R = {}
    def g(r, c):
        i = r * W + c
        return s[i] if 0 <= c < W and 0 <= r and i < n else None
    for name, (dr, dc) in DIRS.items():
        cnt = edge = 0
        for r in range(H):
            for c in range(W):
                a, b = g(r, c), g(r + dr, c + dc)
                if a and b and a != '?' and b != '?' and a == b:
                    cnt += 1
                    if name == "droite" and c == W - 2: edge += 1
        R[name] = cnt
        if name == "droite": R["droite, dernière paire de la ligne"] = edge
    return R
cols = [((i + 1) % 7) for i in range(90) if K4[i] == K4[i + 7]]
print("colonnes (avec « ? ») des 9 répétitions verticales :", sorted(cols))
for lab, pre in (("avec « ? » en tête (98 = 14 × 7)", "?"), ("sans « ? »", "")):
    obs = rel(pre + K4); N = 100000; ge = dict.fromkeys(obs, 0); mean = dict.fromkeys(obs, 0)
    L = list(K4); random.seed(7)
    for _ in range(N):
        random.shuffle(L); t = rel(pre + "".join(L))
        for k in obs: ge[k] += t[k] >= obs[k]; mean[k] += t[k]
    print("==", lab)
    for k in obs: print(f"  {k:36s} K4 = {obs[k]:2d}   mélanges : moyenne {mean[k]/N:5.2f}   p = {ge[k]/N:.5f}")
