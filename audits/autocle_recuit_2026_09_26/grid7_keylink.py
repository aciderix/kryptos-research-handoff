#!/usr/bin/env python3
"""MÉCA — tâche « question centrale » base 08 §7 (round chef+MECA, base 16 §11) :
la clé Vigenère AUX 24 POSITIONS DE CRIB (aveugle : chiffré + 2 cribs AUTHENTIQUES seulement,
AUCUN faux clair) est-elle compatible avec un MOT-CLÉ écrit dans une grille 7-large + décalage
par ligne ? Modèle testé (le plus serré possible) :

    k[i] = A[col] + s * row   (mod 26),   col = i % 7,  row = i // 7

- A[col] = base du mot-clé pour la colonne (7 inconnues),  s = décalage vertical par ligne (1 inconnue).
- Si vrai aux cribs, ça DÉTERMINE la clé aux 73 autres positions (extrapolation A[col] à toutes les lignes).
- Le chef mesure « k[i+7]-k[i] = -3 sur 4/10 paires » -> s = -3 candidat. On teste TOUS les s.

RIGUEUR (garde-fou chef + règle d'or) :
- tolérance Sanborn : on autorise jusqu'à EMAX positions de crib en désaccord avec le meilleur A[col].
- CONTRÔLE NUL OBLIGATOIRE : même test sur (a) flux de clé aléatoires, (b) K4 mélangé (colonnes
  permutées) -> combien de positions « collent » PAR HASARD ? Sans ça = paréidolie (Hallström 7/24).

Convention Vigenère standard : k = c - p (mod 26). On teste aussi Beaufort k = p - c et c + p.

Usage: grid7_keylink.py
"""
import random

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
N, W = 97, 7
CE = list(range(21, 34)); CB = list(range(63, 74))
CRIBPOS = CE + CB
CRIBPT = "EASTNORTHEAST" + "BERLINCLOCK"
assert len(CRIBPT) == len(CRIBPOS) == 24
C = [ord(c) - 65 for c in K4]
P = {pos: ord(CRIBPT[i]) - 65 for i, pos in enumerate(CRIBPOS)}

def keystream(conv):
    """clé aux positions de crib selon la convention."""
    k = {}
    for pos in CRIBPOS:
        c, p = C[pos], P[pos]
        if conv == "vig":  k[pos] = (c - p) % 26      # c = p + k
        elif conv == "beau": k[pos] = (p - c) % 26    # c = p - k (Beaufort)
        else: k[pos] = (c + p) % 26                    # c = k - p (variante)
    return k

def fit_model(k, s):
    """Pour un décalage vertical s donné, vote A[col] par colonne, renvoie (nb_accord, A, details).
    k : dict pos->val (aux cribs). Modèle k[i] = A[col] + s*row."""
    from collections import Counter, defaultdict
    percol = defaultdict(list)
    for pos, kv in k.items():
        col = pos % W; row = pos // W
        percol[col].append(((kv - s * row) % 26, pos))   # A candidat = k - s*row
    A = {}; agree = 0
    for col, vals in percol.items():
        cnt = Counter(a for a, _ in vals)
        best_a, best_n = cnt.most_common(1)[0]
        A[col] = best_a; agree += best_n
    return agree, A

def best_over_s(k):
    best = (-1, None, None)
    for s in range(26):
        agree, A = fit_model(k, s)
        if agree > best[0]: best = (agree, s, A)
    return best  # (agree, s, A)

def null_random(ntrials=20000, conv="vig"):
    """Contrôle nul (a) : flux de clé aléatoire aux mêmes 24 positions (mêmes col/row)."""
    rng = random.Random(7)
    dist = []
    for _ in range(ntrials):
        k = {pos: rng.randrange(26) for pos in CRIBPOS}
        dist.append(best_over_s(k)[0])
    return dist

def null_shuffle(ntrials=20000, conv="vig"):
    """Contrôle nul (b) : K4 mélangé -> chiffré permuté, mêmes cribs aux mêmes positions."""
    rng = random.Random(11)
    dist = []
    base = C[:]
    for _ in range(ntrials):
        sh = base[:]; rng.shuffle(sh)
        k = {}
        for pos in CRIBPOS:
            c, p = sh[pos], P[pos]
            k[pos] = (c - p) % 26 if conv == "vig" else ((p - c) % 26 if conv == "beau" else (c + p) % 26)
        dist.append(best_over_s(k)[0])
    return dist

def pval(dist, obs):
    return sum(1 for d in dist if d >= obs) / len(dist)

def fit_percol_line(k, emax_per_col=0):
    """Modèle 2 (le plus généreux) : chaque colonne a SA droite k=A[col]+s[col]*row.
    Une colonne 'passe' si toutes ses positions de crib (moins emax_per_col) sont sur UNE droite mod 26.
    Renvoie (nb_colonnes_qui_passent, nb_colonnes_contraignantes(>=3 pts))."""
    from collections import defaultdict
    percol = defaultdict(list)
    for pos, kv in k.items():
        percol[pos % W].append((pos // W, kv))
    passed = 0; constraining = 0
    for col, pts in percol.items():
        if len(pts) < 3:  # <3 pts : une droite passe toujours -> non contraignant
            continue
        constraining += 1
        ok = False
        for s in range(26):
            from collections import Counter
            cnt = Counter((kv - s * r) % 26 for r, kv in pts)
            best_n = cnt.most_common(1)[0][1]
            if len(pts) - best_n <= emax_per_col:
                ok = True; break
        if ok: passed += 1
    return passed, constraining

def null_percol(ntrials=20000, conv="vig", emax_per_col=0):
    rng = random.Random(23); dist = []
    for _ in range(ntrials):
        k = {pos: rng.randrange(26) for pos in CRIBPOS}
        dist.append(fit_percol_line(k, emax_per_col)[0])
    return dist

if __name__ == "__main__":
    print("24 positions de crib, grille 7-large. Modèle k[i]=A[col]+s*row.\n")
    for conv in ("vig", "beau", "sum"):
        k = keystream(conv)
        agree, s, A = best_over_s(k)
        # comptage colonnes/lignes couvertes
        from collections import defaultdict
        cov = defaultdict(set)
        for pos in CRIBPOS: cov[pos % W].add(pos // W)
        ncols_multi = sum(1 for c in cov if len(cov[c]) >= 2)  # colonnes avec >=2 lignes (contraignantes)
        dR = null_random(6000, conv); dS = null_shuffle(6000, conv)
        print(f"[{conv}] meilleur accord = {agree}/24  (s={s}={s-26 if s>13 else s})  "
              f"colonnes multi-lignes={ncols_multi}")
        print(f"      null aléatoire : moy={sum(dR)/len(dR):.1f} max={max(dR)}  p(>= {agree})={pval(dR,agree):.4f}")
        print(f"      null mélangé   : moy={sum(dS)/len(dS):.1f} max={max(dS)}  p(>= {agree})={pval(dS,agree):.4f}")
        print(f"      A[col] = {[A.get(c) for c in range(W)]}")
        # Modèle 2 : droite libre par colonne (shift vertical propre à chaque colonne)
        for emax in (0, 1):
            passed, constr = fit_percol_line(k, emax)
            dP = null_percol(6000, conv, emax)
            print(f"      [droite/col, <= {emax} err/col] {passed}/{constr} colonnes contraignantes passent  "
                  f"| null moy={sum(dP)/len(dP):.2f} max={max(dP)} p(>= {passed})={pval(dP,passed):.4f}")
        print()
