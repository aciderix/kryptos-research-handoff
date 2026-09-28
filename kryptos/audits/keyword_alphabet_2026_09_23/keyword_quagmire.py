"""Exact test: periodic Quagmire whose unknown alphabet(s) are KEYWORD alphabets
(prefix of L arbitrary distinct letters, then the remaining letters in A-Z order).
Parameters are fixed in README.md.  Usage: python3 keyword_quagmire.py [nnull]"""
import json, random, sys, os
from multiprocessing import Pool
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "algebraic_elimination_2026_09_22"))
from k4_algebraic import Model, _alph, _relation, K4, ANCH, AZ, rand_ct

LMAX = 12
PERIODS = range(1, 53)
FAMS = ("II", "I", "IV", "III")


def keyword_constraint(m, pname, L):
    """letters absent from the keyword occupy positions >= L in alphabetical order."""
    for xi in range(26):
        for yi in range(xi + 1, 26):
            nx, ny = f"{pname}{AZ[xi]}", f"{pname}{AZ[yi]}"
            for a in range(L, 26):
                for b in range(L, a):
                    m.clauses.append([-m.lit[(nx, a)], -m.lit[(ny, b)]])


def sat(ct, mode, p, q, L=LMAX):
    m = Model()
    P, C = _alph(m, q)
    for nm in m.perms:
        keyword_constraint(m, nm, L)
    for i, pt in ANCH.items():
        _relation(m, P, C, ct[i], pt, [(1, f"k{i % p}")], mode)
    return m.solve()


def kw_alpha(r, L):
    pre = r.sample(AZ, L)
    return pre + [c for c in AZ if c not in pre]


def synth(seed, mode, p, q):
    r = random.Random(seed)
    L = r.randint(3, LMAX)
    Pa = kw_alpha(r, L) if q in ("I", "III", "IV") else list(AZ)
    Ca = {"I": list(AZ), "II": kw_alpha(r, L), "III": Pa, "IV": kw_alpha(r, r.randint(3, LMAX))}[q]
    key = [r.randrange(26) for _ in range(p)]
    pt = [r.choice(AZ) for _ in range(97)]
    for i, c in ANCH.items():
        pt[i] = c
    s = 1 if mode == "vig" else -1
    return "".join(Ca[(s * Pa.index(pt[i]) + key[i % p]) % 26] for i in range(97))


def job(args):
    q, mode, p, nnull = args
    k4 = sat(K4, mode, p, q)
    nulls = sum(sat(rand_ct(9000 + s), mode, p, q) for s in range(nnull))
    pos = sat(synth(p * 7 + len(q), mode, p, q), mode, p, q)
    minL = None
    if k4:
        for L in range(0, LMAX + 1):
            if sat(K4, mode, p, q, L):
                minL = L
                break
    return {"q": q, "mode": mode, "p": p, "K4": k4, "K4_min_L": minL,
            "null_sat": nulls, "n_null": nnull, "positive": pos}


if __name__ == "__main__":
    nnull = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    jobs = [(q, md, p, nnull) for q in FAMS for md in ("vig", "beau") for p in PERIODS]
    out = []
    with Pool(os.cpu_count()) as pool:
        for r in pool.imap_unordered(job, jobs):
            out.append(r)
            print(json.dumps(r), flush=True)
            with open("results.jsonl", "a") as f:
                f.write(json.dumps(r) + "\n")
