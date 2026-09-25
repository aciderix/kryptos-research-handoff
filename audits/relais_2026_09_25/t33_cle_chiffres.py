"""t33_cle_chiffres.py — clés numériques (Gronsfeld, Gromark, coordonnées, dates…) avec alphabets à mot-clé (relais du 25/09/2026).
Si la clé ne prend que des valeurs 0–9, les 24 valeurs imposées par les cribs doivent toutes être dans 0–9 (dans le sens de lecture
de la clé : +k ou −k). Pour un alphabet au hasard, la probabilité est ≈ (10/26)^24 ≈ 10⁻¹⁰ : le filtre suffit à lui seul.
On le passe sur tous les alphabets (mots du dictionnaire et thèmes, formes standard et « suite », inversés), 5 types, 3 conventions,
en tolérant e lettres hors 0–9. Témoins : K4 mélangé.
"""
import numpy as np, sys, os
OFFS = os.environ.get("OFFS") == "1"
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
POS = list(range(21, 34)) + list(range(63, 74)); PTX = "EASTNORTHEAST" + "BERLINCLOCK"
A = np.frombuffer(open(sys.argv[1], "rb").read(), dtype=np.uint8).reshape(-1, 26) - 65
A = np.concatenate([A, A[:, ::-1]])                      # l'inverse compte ici (la famille n'est pas symétrique)
rank = np.zeros_like(A); rows = np.arange(len(A))[:, None]; rank[rows, A] = np.arange(26)[None, :]   # lettre -> rang
azr = np.arange(26); kar = np.zeros(26, int); kar[[ord(c) - 65 for c in KA]] = np.arange(26)
def keys(ct):
    c = np.array([ord(ct[i]) - 65 for i in POS]); p = np.array([ord(x) - 65 for x in PTX])
    out = []
    for ty in range(5):
        Y = rank if ty in (0, 2, 3) else (np.broadcast_to(azr, rank.shape) if ty == 1 else np.broadcast_to(kar, rank.shape))
        X = rank if ty in (0, 1, 4) else (np.broadcast_to(azr, rank.shape) if ty == 2 else np.broadcast_to(kar, rank.shape))
        x = X[:, c]; y = Y[:, p]
        for mo, k in (("VIG", (x - y) % 26), ("BEAU", (x + y) % 26), ("VARB", (y - x) % 26)):
            out.append((ty, mo, k))
    return out
def run(ct, show=False):
    best = 99
    for ty, mo, k in keys(ct):
        # fenêtre de 10 valeurs consécutives à n'importe quel décalage c0 (clé = chiffre + constante)
        bad = np.min(np.stack([(((k - c0) % 26) > 9).sum(1) for c0 in (range(26) if OFFS else [0])]), axis=0)
        m = bad.min(); best = min(best, m)
        if show and m <= 3:
            for i in np.where(bad <= 3)[0][:10]:
                print(f"  type {ty} {mo} : {bad[i]} hors 0–9 ; alphabet {''.join(AZ[v] for v in A[i])} ; clé {''.join(str(v) if v < 10 else '·' for v in k[i])}")
    return best
print("K4 : minimum de valeurs hors 0–9 :", run(K4, True))
rng = np.random.default_rng(1); L = list(K4); r = []
for z in range(30):
    rng.shuffle(L); r.append(run("".join(L)))
print("témoins K4 mélangé (30) :", sorted(r))
