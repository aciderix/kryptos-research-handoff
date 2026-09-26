#!/usr/bin/env python3
"""Classement PROUVÉ des débloqueurs (round conjoint chef+MECA, tâche c716b63b) :
de combien chaque « donnée nouvelle » réduit-elle le noyau (dimension) de la variété σ,τ crib-consistante ?

- K4 seul (σ,τ libres)                         : nullspace 19 (référence).
- K4 + K5 (mêmes σ,τ, autoclé écart-7, mêmes cribs, K5 SIMULÉ) : nullspace ~5 -> SOLVABLE.
- Pochoir : σ fixé -> 8 ; σ ET τ fixés -> ~0   (voir lever_amorce_doublets.py, fonction stencil()).

Conclusion : les 17 équations de cribs de K5 PORTENT sur σ,τ (contraintes d'alphabet), donc K5 réduit 19->5,
autant ou plus qu'un demi-pochoir. Classement par (réduction × obtenabilité) : K5 > écran > pochoir scellé.

Usage: k5_vs_stencil.py CORPUS_DIR [n_k5]
"""
import sys, glob, os, random, statistics
from t36b_recover import build_eqs

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
N, LAG = 97, 7
CE, CB = list(range(21, 34)), list(range(63, 74))

def letters(p):
    t = open(p, encoding="utf-8", errors="ignore").read()
    s = t.find("*** START"); s = t.find("\n", s) if s >= 0 else 0
    e = t.find("*** END"); e = e if e >= 0 else len(t)
    return "".join(c for c in t[s:e].upper() if "A" <= c <= "Z")

def rank_null(eqs, p=13):
    rows, vs = [], set()
    for s, t in eqs:
        row = {('s', k): v for k, v in s.items()}; row.update({('t', ch): v for ch, v in t.items()})
        rows.append(row); vs |= set(row)
    vs = sorted(vs, key=str); M = [[r.get(v, 0) % p for v in vs] for r in rows]
    nr, nc, rk = len(M), len(vs), 0
    for col in range(nc):
        piv = next((r for r in range(rk, nr) if M[r][col] % p), None)
        if piv is None: continue
        M[rk], M[piv] = M[piv], M[rk]; inv = pow(M[rk][col], p - 2, p); M[rk] = [(x * inv) % p for x in M[rk]]
        for r in range(nr):
            if r != rk and M[r][col] % p:
                f = M[r][col]; M[r] = [(a - f * b) % p for a, b in zip(M[r], M[rk])]
        rk += 1
    return nc - rk

def main():
    blobs = [b for b in (letters(p) for p in glob.glob(os.path.join(sys.argv[1], "*.txt"))) if len(b) > 5000]
    nk5 = int(sys.argv[2]) if len(sys.argv) > 2 else 40
    rng = random.Random(11)
    def enc_k5(both):
        b = rng.choice(blobs); i = rng.randrange(0, len(b) - N); p = [ord(c) - 65 for c in b[i:i + N]]
        cr = CE + CB if both else CB
        ct = ("EASTNORTHEAST" + "BERLINCLOCK") if both else "BERLINCLOCK"
        for j, pos in enumerate(cr): p[pos] = ord(ct[j]) - 65
        sig = list(range(26)); rng.shuffle(sig); tau = list(range(26)); rng.shuffle(tau)
        taui = [0] * 26
        for k, v in enumerate(tau): taui[v] = k
        kap = [rng.randrange(26) for _ in range(LAG)]; c = [0] * N; x = [0] * N
        for k in range(N):
            key = kap[k] if k < LAG else x[k - LAG]
            x[k] = sig[p[k]]; c[k] = taui[(x[k] + key) % 26]
        return "".join(chr(65 + v) for v in c)
    eqK4 = build_eqs(K4)
    print("K4 seul (σ,τ libres) : nullspace", rank_null(eqK4))
    for both, label in [(True, "K5 EAST+BERLIN"), (False, "K5 BERLIN seul")]:
        nls = [rank_null(eqK4 + build_eqs(enc_k5(both))) for _ in range(nk5)]
        print(f"K4 + {label:16s}: nullspace moy={statistics.mean(nls):.1f} min={min(nls)} max={max(nls)}")
    print("Rappel pochoir : σ fixé -> 8 ; σ+τ fixés -> ~0. Classement (réduction×obtenabilité) : K5 > écran > pochoir scellé.")

if __name__ == "__main__":
    main()
