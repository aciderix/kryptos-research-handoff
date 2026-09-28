"""t13_hill7.py — chiffre de Hill par blocs de n (n = 2..8), matrices à petits coefficients (audit « vision », 24/09/2026)

Motif : le « 7 » de K4 (base 7 §8.2) est une structure de blocs sans période ; un Hill par blocs de 7 en est une.
Sanborn : « Kryptos utilise des matrices » ; Scheidt : masques « binaires » ; R. Bean (2018) a essayé une matrice 7 × 7
binaire tirée des coordonnées de K2. Avec une matrice QUELCONQUE, 24 lettres ne suffisent pas (7 inconnues par ligne,
2 blocs connus) ; avec des coefficients restreints, le test devient décidable.

Paramètres fixés avant calcul :
  taille de bloc n = 2..8 ; phase φ = 0..n−1 (début des blocs) ;
  coefficients : binaires {0,1} ; ternaires {−1,0,1} ; {0,1,2} ;
  numérotation : A=0, A=1 (Z=26≡0), rang dans KRYPTOSABC… ;
  sens : chiffrement C = M·P, ou déchiffrement P = M·C (M appliquée au chiffré) ;
  une ligne j de M est contrainte par chaque bloc où les lettres dont elle a besoin sont connues.
Verdict par cas : « compatible » si chaque ligne j a au moins une ligne candidate ; on compte aussi les lignes à 0 candidat.
Témoins : 50 chiffrés aléatoires (97 lettres i.i.d.), même procédure.
"""
import itertools, random
import numpy as np

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
PT = {i: ch for i, ch in zip(range(21, 34), "EASTNORTHEAST")}
PT.update({i: ch for i, ch in zip(range(63, 74), "BERLINCLOCK")})
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
NUM = {"A=0": lambda ch: ord(ch) - 65, "A=1": lambda ch: (ord(ch) - 64) % 26, "KA": lambda ch: KA.index(ch)}
COEF = {"bin": (0, 1), "ter": (-1, 0, 1), "012": (0, 1, 2)}

def row_candidates(n, coefs):
    return np.array(list(itertools.product(coefs, repeat=n)), dtype=np.int64)

CANDS = {(n, cname): row_candidates(n, cs) for n in range(2, 9) for cname, cs in COEF.items() if len(cs) ** n <= 50000}

def case(ct, n, phi, cname, num, direction, pt=None):
    """renvoie (nb de lignes sans candidat, nb total de contraintes) ; 0 ligne vide = compatible"""
    f = NUM[num]
    c = [f(ch) for ch in ct]
    p = {i: f(ch) for i, ch in (pt or PT).items()}
    R = CANDS[(n, cname)]
    empty = 0; ncons = 0
    starts = [s for s in range(phi - n, 97, n)]
    for j in range(n):
        ok = np.ones(len(R), bool)
        for s in starts:
            idx = [s + k for k in range(n)]
            if min(idx) < 0 or max(idx) > 96:
                continue
            tgt = s + j
            if direction == "enc":      # c[tgt] = Σ m_k p[s+k] : il faut tout le bloc clair connu
                if not all(i in p for i in idx):
                    continue
                vec = np.array([p[i] for i in idx]); val = c[tgt]
            else:                         # p[tgt] = Σ m_k c[s+k] : il faut le clair de la cible
                if tgt not in p:
                    continue
                vec = np.array([c[i] for i in idx]); val = p[tgt]
            ncons += 1
            ok &= ((R @ vec - val) % 26 == 0)
        if not ok.any():
            empty += 1
    return empty, ncons

def positive_controls():
    """faux K4 fabriqué par un vrai Hill 7 × 7 binaire (A=0, φ=0), dans les deux sens : doit être reconnu"""
    rnd = random.Random(5)
    AL = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    M = np.array([[rnd.randint(0, 1) for _ in range(7)] for _ in range(7)])
    # chiffrement : clair aléatoire contenant les cribs, C = M·P par blocs complets, reste aléatoire
    pt = [rnd.randrange(26) for _ in range(97)]
    for i, ch in PT.items(): pt[i] = ord(ch) - 65
    ct = [rnd.randrange(26) for _ in range(97)]
    for s0 in range(0, 91, 7):
        blk = M @ np.array(pt[s0:s0 + 7]) % 26
        ct[s0:s0 + 7] = list(blk)
    r_enc = case("".join(AL[x] for x in ct), 7, 0, "bin", "A=0", "enc")[0]
    # déchiffrement : chiffré aléatoire, P = M·C, les « cribs » sont pris dans ce faux clair
    ct2 = [rnd.randrange(26) for _ in range(97)]
    pt2 = list(ct2)
    for s0 in range(0, 91, 7):
        pt2[s0:s0 + 7] = list(M @ np.array(ct2[s0:s0 + 7]) % 26)
    fake = {i: AL[pt2[i]] for i in PT}
    r_dec = case("".join(AL[x] for x in ct2), 7, 0, "bin", "A=0", "dec", pt=fake)[0]
    print(f"contrôle positif Hill 7 × 7 binaire : chiffrement {'reconnu' if r_enc == 0 else 'MANQUÉ'}, déchiffrement {'reconnu' if r_dec == 0 else 'MANQUÉ'}")

def detail_n7():
    rnd = random.Random(7)
    nulls = ["".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(97)) for _ in range(100)]
    print("Détail n = 7 (cas avec au moins 2 contraintes par ligne) : lignes sans candidat pour K4 ; témoins compatibles sur 100")
    for cname in COEF:
        for num in NUM:
            for d in ("enc", "dec"):
                for phi in range(7):
                    e, nc = case(K4, 7, phi, cname, num, d)
                    if nc < 14: continue
                    ps = sum(case(z, 7, phi, cname, num, d)[0] == 0 for z in nulls)
                    print(f"  {cname} {num} {d} φ={phi} : {nc} contraintes ; lignes vides {e} ; témoins {ps}/100")

def main():
    positive_controls()
    detail_n7()
    rnd = random.Random(2024)
    nulls = ["".join(rnd.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(97)) for _ in range(50)]
    tot = comp = 0; exp = 0.0
    for (n, cname) in sorted(CANDS):
        for num in NUM:
            for direction in ("enc", "dec"):
                for phi in range(n):
                    e, nc = case(K4, n, phi, cname, num, direction)
                    if nc < n:          # moins d'une contrainte par ligne en moyenne : non tranchable
                        continue
                    tot += 1
                    passes = sum(case(z, n, phi, cname, num, direction)[0] == 0 for z in nulls)
                    exp += passes / len(nulls)
                    if e == 0:
                        comp += 1
                        print(f"n={n} {cname} {num} {direction} φ={phi} ({nc} contraintes) : K4 COMPATIBLE ; témoins {passes}/{len(nulls)}")
    print(f"TOTAL : {tot} cas tranchables ; K4 compatible dans {comp} ; attendu pour un chiffré aléatoire : {exp:.1f}")

if __name__ == "__main__":
    main()
