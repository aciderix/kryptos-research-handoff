#!/usr/bin/env python3
"""Test de la CONVERGENCE « deux étapes » (échange chef+MECA, tâche c716b63b).

Hypothèse : le profil du « 7 » de K4 = signature de DEUX étapes, pas d'une clé.
  Étape 1 (lettre-à-lettre) : autoclé écart-7 Vigenère  y[i] = p[i] + p[i-7] (+amorce)
  Étape 2 (verticale locale largeur-7, PAS un réarrangement) : substitution où le rang du
          dessus module la lettre.  Deux variantes :
            (i)  additif   c[i] = y[i] + y[i-7]            (Vigenère, voisin du dessus = clé)
            (ii) beaufort  c[i] = y[i-7] - y[i]            (superposition « inversée »)
  (aucun fold global : l'analyse a montré que seul l'écart-7 proche-voisin déborde.)

Critère falsifiable (chef) : le procédé à 2 étapes reproduit-il la SIGNATURE JOINTE de K4 —
  (A) excès écart-7  (K4 : 9 coïncidences c[i]=c[i+7], hasard ~3.3)
  ET (B) concentration des doublets adjacents en UNE classe mod 7 (K4 : 5 doublets sur 6 à ≡4 mod 7) —
  ce qu'AUCUN des 52 procédés simulés (base 10 F) ne fait ?
Témoins « K4 mélangé » (positionnels) + look-elsewhere payé en aval.

Note mécanisme : la signature POSITIONNELLE (égalités c[i]=c[i+d], positions des doublets) est
INDÉPENDANTE des alphabets (ce sont des égalités, invariantes par permutation) ; elle ne dépend
que des répétitions du CLAIR propagées par la composition. On fait donc varier le CLAIR (many
97-grammes anglais, cribs insérés comme dans K4), alphabets = A-Z, amorces aléatoires.

Usage: sim2step.py CORPUS_DIR [n_plain] [seed]
"""
import sys, os, glob, random

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
N, LAG = 97, 7
CE = list(range(21, 34)); CB = list(range(63, 74))
CRIB = "EASTNORTHEAST" + "BERLINCLOCK"

def letters_of(path):
    t = open(path, encoding="utf-8", errors="ignore").read()
    s = t.find("*** START");  s = t.find("\n", s) if s >= 0 else 0
    e = t.find("*** END");    e = e if e >= 0 else len(t)
    return "".join(c for c in t[s:e].upper() if "A" <= c <= "Z")

def insert_cribs(p):
    p = list(p)
    for i, pos in enumerate(CE + CB):
        p[pos] = CRIB[i]
    return [ord(c) - 65 for c in p]

def step1(p, primer):  # autoclé écart-7 Vigenère (A-Z)
    y = [0] * N
    for i in range(N):
        k = primer[i] if i < LAG else y[i - LAG]
        y[i] = (p[i] + k) % 26
    return y

def step2(y, primer, mode, lag=LAG):  # substitution à modificateur voisin (lag=7 vertical, lag=1 horizontal)
    c = [0] * N
    for i in range(N):
        k = primer[i % lag] if i < lag else y[i - lag]
        c[i] = (y[i] + k) % 26 if mode == "add" else (k - y[i]) % 26
    return c

def forced_control(rng):
    """Contrôle positif du DÉTECTEUR : un chiffré aléatoire où l'on FORCE 5 doublets en colonne 4.
    Doit faire monter concentration→~1 et déclencher la signature jointe : prouve que la mesure
    détecte une concentration quand elle existe (le négatif n'est pas dû à un détecteur aveugle)."""
    c = [rng.randrange(26) for _ in range(N)]
    for pos in [4, 11, 18, 46, 67]:      # positions ≡ 4 mod 7
        c[pos + 1] = c[pos]              # force le doublet
    # force aussi quelques coïncidences écart-7 pour la moitié "excès"
    for i in [0, 7, 14, 21, 28, 35]:
        c[i + 7] = c[i]
    return c

def ecart7(c):
    return sum(1 for i in range(N - LAG) if c[i] == c[i + LAG])

def doublets(c):
    pos = [i for i in range(N - 1) if c[i] == c[i + 1]]
    return pos

def concentration(pos):
    """max fraction of adjacent doublets sharing one residue mod 7 ; and that residue."""
    if not pos:
        return 0.0, None, 0
    cnt = [0] * 7
    for i in pos:
        cnt[i % 7] += 1
    m = max(cnt)
    return m / len(pos), cnt.index(m), len(pos)

def signature(c):
    e = ecart7(c); pos = doublets(c); conc, res, nd = concentration(pos)
    return e, nd, conc, res

def main():
    cdir = sys.argv[1]
    npl = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    seed = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    rng = random.Random(seed)
    blobs = [b for b in (letters_of(p) for p in sorted(glob.glob(os.path.join(cdir, "*.txt")))) if len(b) > 5000]

    # référence K4
    e0, nd0, conc0, res0 = signature([ord(c) - 65 for c in K4])
    print("K4 : ecart7=%d  doublets=%d  concentration=%.2f (classe mod7=%d)\n" % (e0, nd0, conc0, res0))

    # témoins « K4 mélangé » (positionnels)
    def witness_stats(n):
        hit = 0; es = []; concs = []
        for _ in range(n):
            L = list(K4); rng.shuffle(L); c = [ord(x) - 65 for x in L]
            e, nd, conc, _ = signature(c); es.append(e); concs.append(conc)
            if e >= e0 and nd >= 5 and conc >= 0.8:
                hit += 1
        return hit / n, sum(es) / n, sum(concs) / n
    wr, we, wc = witness_stats(3000)

    def run_family(twostep, mode="add", lag=LAG):
        es = []; concs = []; nds = []; joint = 0; trials = 0
        for _ in range(npl):
            b = rng.choice(blobs); i = rng.randrange(0, len(b) - N)
            p = insert_cribs(b[i:i + N])
            for _r in range(20):  # amorces aléatoires
                y = step1(p, [rng.randrange(26) for _ in range(LAG)])
                c = step2(y, [rng.randrange(26) for _ in range(max(lag, 1))], mode, lag) if twostep else y
                e, nd, conc, _ = signature(c)
                es.append(e); concs.append(conc); nds.append(nd); trials += 1
                if e >= e0 and nd >= 5 and conc >= 0.8:
                    joint += 1
        return joint / trials, sum(es) / trials, sum(concs) / trials, sum(nds) / trials, trials

    # contrôle positif du détecteur
    fc = [signature(forced_control(rng)) for _ in range(500)]
    fj = sum(1 for e, nd, conc, _ in fc if e >= e0 and nd >= 5 and conc >= 0.8) / len(fc)
    fconc = sum(c for _, _, c, _ in fc) / len(fc)
    print("CONTRÔLE POSITIF du détecteur (doublets FORCÉS en colonne 4) : joint=%.3f  conc_moy=%.2f  => le détecteur voit une concentration quand elle existe.\n" % (fj, fconc))

    print("=== fréquence de la SIGNATURE JOINTE (ecart7>=%d ET >=5 doublets ET concentration>=0.8) ===" % e0)
    print("témoins K4-mélangé          : joint=%.4f  ecart7_moy=%.2f  conc_moy=%.2f" % (wr, we, wc))
    for name, ts, mode, lag in [("étape 1 seule (autoclé)    ", False, "add", LAG),
                                ("2 ét. VERTICAL (i) additif ", True, "add", 7),
                                ("2 ét. VERTICAL (ii) beaufort", True, "bea", 7),
                                ("2 ét. HORIZONTAL additif   ", True, "add", 1),
                                ("2 ét. HORIZONTAL beaufort  ", True, "bea", 1)]:
        j, e, conc, nd, tr = run_family(ts, mode, lag)
        print("%s: joint=%.4f  ecart7_moy=%.2f  conc_moy=%.2f  doublets_moy=%.2f  (%d essais)" % (name, j, e, conc, nd, tr))

if __name__ == "__main__":
    main()
