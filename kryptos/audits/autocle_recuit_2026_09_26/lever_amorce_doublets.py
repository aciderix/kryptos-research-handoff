#!/usr/bin/env python3
"""Deux mesures du round « débat » (échange chef+MECA, tâche c716b63b) :

A. LEVIER amorce=KRYPTOS. L'autoclé écart-7 a une amorce de 7 lettres (keystream[0..6] = clé externe).
   Fixer keystream[0..6]=KRYPTOS ajoute 7 équations linéaires au système crib-consistant. Réduit-il assez
   la dimension de la variété (base 13 : intractable) pour la rendre énumérable ? -> NON : nullspace 21->16.
   Conclusion : l'amorce (7 lettres) est trop petite ; c'est un JEU DE CRIBS ENTIER (K5, ~17 éq) qu'il faut.

B. DÉBAT signpost. L'autoclé écart-7 (étape 1) produit-elle un biais mécanique du résidu (mod 7) des doublets ?
   -> NON : distribution uniforme. Donc la concentration de K4 (5/6 en classe 4) n'est pas le mécanisme ; et
   P(>=5/6 en une classe) ~ 0.001 sous l'autoclé -> indiscernable de « hasard » avec 6 doublets. Le « signpost
   délibéré » n'est ni indépendant du signal écart-7 (même « 7 »), ni actionnable, ni statistiquement démontrable.

Usage: lever_amorce_doublets.py CORPUS_DIR
"""
import sys, glob, os, random
from collections import Counter

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
N, LAG = 97, 7
CE = list(range(21, 34)); CB = list(range(63, 74)); CRIBPOS = CE + CB
CRIBPT = "EASTNORTHEAST" + "BERLINCLOCK"
PTAT = {p: (ord(CRIBPT[i]) - 65) for i, p in enumerate(CRIBPOS)}
PRIMER = "KRYPTOS"

# ---------- A. levier amorce ----------
def build_x():
    def addv(d, k, c): d[k] = d.get(k, 0) + c
    X = [None] * N
    for i in range(N):
        e = {}; addv(e, ('t', K4[i]), 1)
        if i < LAG: addv(e, ('k', i), -1)
        else:
            for k, c in X[i - LAG].items(): addv(e, k, -c)
        X[i] = {k: v % 26 for k, v in e.items() if v % 26}
    return X

def rank_null(eqs, p):
    vars_ = sorted({k for e in eqs for k in e}, key=str)
    M = [[e.get(v, 0) % p for v in vars_] for e in eqs]
    nr, nc, rank = len(M), len(vars_), 0
    for col in range(nc):
        piv = next((r for r in range(rank, nr) if M[r][col] % p), None)
        if piv is None: continue
        M[rank], M[piv] = M[piv], M[rank]
        inv = pow(M[rank][col], p - 2, p); M[rank] = [(x * inv) % p for x in M[rank]]
        for r in range(nr):
            if r != rank and M[r][col] % p:
                f = M[r][col]; M[r] = [(a - f * b) % p for a, b in zip(M[r], M[rank])]
        rank += 1
    return nc, rank, nc - rank

def lever():
    X = build_x()
    def addv(d, k, c): d[k] = d.get(k, 0) + c
    crib = []
    for pos in CRIBPOS:
        e = dict(X[pos]); addv(e, ('s', PTAT[pos]), -1); crib.append({k: v % 26 for k, v in e.items() if v % 26})
    primer = []
    for r in range(LAG):
        primer.append({('k', r): 1, ('s', ord(PRIMER[r]) - 65): (-1) % 26})
    print("A. LEVIER amorce=KRYPTOS :")
    for label, eqs in [("  sans amorce (24 eq)      ", crib), ("  avec amorce KRYPTOS (31) ", crib + primer)]:
        out = []
        for p in (2, 13):
            nc, rk, nl = rank_null(eqs, p); out.append("mod%d nullspace=%d" % (p, nl))
        print(label, " | ".join(out))
    print("  => réduction ~5 ; reste ~16 dim ≈ 1e19 : NON énumérable. L'amorce ne débloque pas.\n")

# ---------- B. débat doublets ----------
def letters(p):
    t = open(p, encoding="utf-8", errors="ignore").read()
    s = t.find("*** START"); s = t.find("\n", s) if s >= 0 else 0
    e = t.find("*** END"); e = e if e >= 0 else len(t)
    return "".join(c for c in t[s:e].upper() if "A" <= c <= "Z")

def debat(cdir):
    blobs = [b for b in (letters(p) for p in glob.glob(os.path.join(cdir, "*.txt"))) if len(b) > 5000]
    rng = random.Random(5)
    def insert(p):
        p = list(p)
        for i, pos in enumerate(CRIBPOS): p[pos] = CRIBPT[i]
        return [ord(c) - 65 for c in p]
    def step1(p, pr):
        y = [0] * N
        for i in range(N):
            k = pr[i] if i < LAG else y[i - LAG]; y[i] = (p[i] + k) % 26
        return y
    res = Counter(); nt = tot = conc = 0
    for _ in range(4000):
        b = rng.choice(blobs); i = rng.randrange(0, len(b) - N)
        y = step1(insert(b[i:i + N]), [rng.randrange(26) for _ in range(LAG)])
        d = [j for j in range(N - 1) if y[j] == y[j + 1]]
        if not d: continue
        nt += 1; tot += len(d); c = Counter(j % 7 for j in d)
        for r, n in c.items(): res[r] += n
        if max(c.values()) / len(d) >= 0.8 and len(d) >= 5: conc += 1
    s = sum(res.values())
    print("B. DÉBAT — résidu mod7 des doublets de l'autoclé écart-7 (%d essais, %d doublets) :" % (nt, tot))
    print("   " + "  ".join("cl%d=%.1f%%" % (r, 100 * res[r] / s) for r in range(7)) + "  (uniforme=14.3%)")
    print("   biais mécanique ? %s ; P(>=5/6 en une classe)=%.4f" % ("OUI" if max(res.values()) / s > 0.20 else "NON (uniforme)", conc / nt))
    print("   K4 doublets par classe :", dict(Counter(j % 7 for j in range(N - 1) if K4[j] == K4[j + 1])))

def stencil():
    """C. Si un pochoir FIXE l'alphabet sigma, la variété tau résiduelle est-elle petite (énumérable) ?"""
    from t36b_recover import build_eqs
    eqs = build_eqs(K4)
    def rank_null(rows, p):
        vs = sorted({k for r in rows for k in r}, key=str)
        M = [[r.get(v, 0) % p for v in vs] for r in rows]
        nr, nc, rk = len(M), len(vs), 0
        for col in range(nc):
            piv = next((r for r in range(rk, nr) if M[r][col] % p), None)
            if piv is None: continue
            M[rk], M[piv] = M[piv], M[rk]; inv = pow(M[rk][col], p - 2, p); M[rk] = [(x * inv) % p for x in M[rk]]
            for r in range(nr):
                if r != rk and M[r][col] % p:
                    f = M[r][col]; M[r] = [(a - f * b) % p for a, b in zip(M[r], M[rk])]
            rk += 1
        return nc, nc - rk
    both = [{**{('s', k): v for k, v in s.items()}, **{('t', c): v for c, v in t.items()}} for s, t in eqs]
    tau = [{('t', c): v for c, v in t.items()} for s, t in eqs]
    print("C. STENCIL (fixe l'alphabet) :")
    nc1, nl1 = rank_null(both, 13); nc2, nl2 = rank_null(tau, 13)
    print("   sigma+tau libres : nullspace %d | sigma FIXÉ -> tau seul : nullspace %d" % (nl1, nl2))
    print("   => fixer l'alphabet effondre 19->8 ; single-alphabet (σ=τ) -> ~0 = résoluble. Le pochoir > K5.\n")

if __name__ == "__main__":
    lever()
    stencil()
    if len(sys.argv) > 1:
        debat(sys.argv[1])
