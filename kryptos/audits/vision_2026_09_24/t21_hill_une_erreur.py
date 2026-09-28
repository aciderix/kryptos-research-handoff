"""t21_hill_une_erreur.py — l'élimination de T13 (Hill par blocs, petits coefficients) résiste-t-elle à UNE erreur ?
(audit « vision », 24/09/2026 ; suite de T19–T20)

Une lettre de chiffré fausse en e invalide toutes les contraintes de ligne qui lisent le chiffré en e :
  chiffrement (c = M·p)   : la seule contrainte dont la cible est e ;
  déchiffrement (p = M·c) : toutes les contraintes du bloc qui contient e.
Modèles : A = erreur quelconque en une position e (0..96) ; B = « copie » (clair recopié), seulement là où chiffré = clair
(32 et 73 pour K4). Le glissement de clé n'a pas de sens pour un Hill.
Même grille de cas que T13 (n = 2..8, 3 jeux de coefficients, 3 numérotations, 2 sens, toutes phases) ; témoins :
50 chiffrés aléatoires par cas.
"""
import random
import numpy as np
from t13_hill7 import K4, PT, NUM, CANDS

def constraints(ct, n, phi, cname, num, direction):
    """liste, par ligne j, de (ensemble des positions de chiffré lues, vecteur booléen des candidats qui passent)"""
    f = NUM[num]
    c = [f(ch) for ch in ct]
    p = {i: f(ch) for i, ch in PT.items()}
    R = CANDS[(n, cname)]
    rows = []
    for j in range(n):
        lst = []
        for s in range(phi - n, 97, n):
            idx = [s + k for k in range(n)]
            if min(idx) < 0 or max(idx) > 96:
                continue
            tgt = s + j
            if direction == "enc":
                if not all(i in p for i in idx):
                    continue
                vec = np.array([p[i] for i in idx]); val = c[tgt]; used = {tgt}
            else:
                if tgt not in p:
                    continue
                vec = np.array([c[i] for i in idx]); val = p[tgt]; used = set(idx)
            lst.append((used, (R @ vec - val) % 26 == 0))
        rows.append(lst)
    return rows

def verdict(ct, n, phi, cname, num, direction):
    """(exact, A, B, positions qui sauvent) ; ncons"""
    rows = constraints(ct, n, phi, cname, num, direction)
    ncons = sum(len(r) for r in rows)
    def ok(err):
        for lst in rows:
            m = None
            for used, v in lst:
                if err is not None and err in used:
                    continue
                m = v if m is None else (m & v)
            if m is not None and not m.any():
                return False
        return True
    exact = ok(None)
    if exact:
        return True, True, True, [], ncons
    pos = sorted({e for lst in rows for used, _ in lst for e in used})
    save = [e for e in pos if ok(e)]
    copies = [e for e in save if e in PT and ct[e] == PT[e]]
    return False, bool(save), bool(copies), save, ncons

def main():
    rnd = random.Random(2025)
    nulls = ["".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(97)) for _ in range(50)]
    tot = 0; k = [0, 0, 0]; z = [0.0, 0.0, 0.0]
    for (n, cname) in sorted(CANDS):
        if n > 6:          # n = 7, 8 : déjà au niveau du hasard sans erreur (T13) ; seul n ≤ 6 était « éliminé »
            continue
        for num in NUM:
            for d in ("enc", "dec"):
                for phi in range(n):
                    e, a, b, save, nc = verdict(K4, n, phi, cname, num, d)
                    if nc < n:
                        continue
                    tot += 1
                    k[0] += e; k[1] += a; k[2] += b
                    zz = [verdict(x, n, phi, cname, num, d)[:3] for x in nulls]
                    for i in range(3):
                        z[i] += sum(t[i] for t in zz) / len(nulls)
                    if a:
                        print(f"n={n} {cname} {num} {d} φ={phi} : exact {int(e)}, A {int(a)}, B {int(b)} ; sauvé par une erreur en {save} ; témoins A {sum(t[1] for t in zz)}/{len(nulls)}")
    print(f"TOTAL n = 2..6 : {tot} cas ; K4 compatible : exact {k[0]}, ≤1 erreur quelconque {k[1]}, ≤1 copie {k[2]} ; "
          f"attendu au hasard : {z[0]:.1f} / {z[1]:.1f} / {z[2]:.1f}")

if __name__ == "__main__":
    main()
