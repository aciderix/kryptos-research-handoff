"""Tests #2 #3 #8 #14 #15 #19 #20 (see README).  Usage: python3 tests.py PART   (PART in 2,3,8,14a,14b,15,19,20)
Every SAT call has a conflict budget (None = undecided).  Results: results_PART.json."""
import sys, json, random, itertools
import numpy as np
from multiprocessing import Pool
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
import k4_algebraic as ka
from k4_algebraic import K4, ANCH, AZ, KA, rand_ct, columnar_perm
from pysat.solvers import Cadical153
BUDGET = 100000
N = 97
def solve(m):
    if m.unsat: return False
    with Cadical153(bootstrap_with=m.clauses) as s:
        s.conf_budget(BUDGET); return s.solve_limited()
def model(ct, q, md, items):
    """items: list of (ct_pos, pt_letter, key_terms, key_const)"""
    m = ka.Model(); P, C = ka._alph(m, q)
    for t, pt, kt, kc in items: ka._relation(m, P, C, ct[t], pt, kt, md, key_const=kc)
    return solve(m)
def nulls(fn, n=10, base=5000):
    return sum(bool(fn(rand_ct(base + s))) for s in range(n))

# ---------------- #2 partition by closed-counter letters
CLOSED = set("ABDOPQR")
def t2(ct, md, p): return model(ct, "III", md, [(i, pt, [(1, f"k{'X' if pt in CLOSED else 'Y'}{i % p}")], 0) for i, pt in ANCH.items()])
# ---------------- #3 physical row offset + periodic
def prow(i): return (i + 27) // 31
def t3(ct, md, p): return model(ct, "III", md, [(i, pt, [(1, f"r{prow(i)}"), (1, f"k{i % p}")], 0) for i, pt in ANCH.items()])
# ---------------- #8 K2 coordinate digits
DIG = [3, 8, 5, 7, 6, 5, 7, 7, 8, 4, 4]
def t8(ct, q, md, ph, rev):
    d = DIG[::-1] if rev else DIG
    return model(ct, q, md, [(i, pt, [], d[(i + ph) % 11]) for i, pt in ANCH.items()])
# ---------------- #14 modular permutations on 97 (out[t] = in[perm[t]], i.e. ct position t holds pt index perm[t])
def gens():
    return [g for g in range(2, 97) if len({pow(g, k, 97) for k in range(1, 97)}) == 96]
def perms14():
    P = {}
    for a in range(1, 97):
        P[f"mul{a}"] = [(a * x) % 97 for x in range(N)]
        P[f"k3form{a}"] = [((x + 1) * a - 1) % 97 for x in range(N)]
    for g in gens():
        f = [0] + [pow(g, x, 97) for x in range(1, 97)]
        P[f"exp{g}"] = f
        inv = [0] * N
        for x, y in enumerate(f): inv[y] = x
        P[f"log{g}"] = inv
    for k, v in P.items(): assert sorted(v) == list(range(N)), k
    return P
def tperm(ct, perm, order, md, p):
    pos_of = {src: t for t, src in enumerate(perm)}
    return model(ct, "III", md, [(pos_of[i], pt, [(1, f"k{(pos_of[i] if order == 'TS' else i) % p}")], 0) for i, pt in ANCH.items()])
# ---------------- #19 columnar DYAHR / YAR
# ---------------- #20 row-keyed substitution + columns bottom->top on width 31
def grid_perm(off):
    cells = {}
    for i in range(N): n = off + i; cells[(n // 31, n % 31)] = i
    rows = max(r for r, c in cells) + 1
    return [cells[(r, c)] for c in range(31) for r in range(rows - 1, -1, -1) if (r, c) in cells]
def t20(ct, off, order, md, periodic_p):
    perm = grid_perm(off); pos_of = {src: t for t, src in enumerate(perm)}
    items = []
    for i, pt in ANCH.items():
        t = pos_of[i]; row = (off + i) // 31 if order == "ST" else (t + 27) // 31
        kt = [(1, f"r{row}")] + ([(1, f"k{(i if order == 'ST' else t) % periodic_p}")] if periodic_p else [])
        items.append((t, pt, kt, 0))
    return model(ct, "III", md, items)

def job14(a):
    name, perm, order, md = a
    out = []
    for p in range(1, 27):
        r = tperm(K4, perm, order, md, p)
        if r is True and p <= 20: out.append((p, True, nulls(lambda c: tperm(c, perm, order, md, p))))
        elif r is not False: out.append((p, r, None))
    return name, order, md, out

if __name__ == "__main__":
    part = sys.argv[1]; R = {}
    if part == "2":
        for md in ("vig", "beau"):
            R[md] = [(p, (k := t2(K4, md, p)), nulls(lambda c: t2(c, md, p), 20) if k else None) for p in range(1, 27)]
    elif part == "3":
        for md in ("vig", "beau"):
            R[md] = [(p, (k := t3(K4, md, p)), nulls(lambda c: t3(c, md, p), 20) if k else None) for p in range(1, 27)]
    elif part == "8":
        for q in ("I", "II", "III", "IV"):
            for md in ("vig", "beau"):
                for rev in (False, True):
                    hits = [ph for ph in range(11) if t8(K4, q, md, ph, rev)]
                    R[f"{q} {md} rev={rev}"] = {"K4_phases": hits,
                        "null_any_phase_of_20": sum(any(t8(rand_ct(600 + s), q, md, ph, rev) for ph in range(11)) for s in range(20))}
    elif part == "14a":
        P = perms14()
        jobs = [(n, pm, o, md) for n, pm in P.items() for o in ("TS", "ST") for md in ("vig", "beau")]
        with Pool(3) as pool:
            for name, order, md, out in pool.imap_unordered(job14, jobs, chunksize=4):
                R[f"{name} {order} {md}"] = out
    elif part == "14b":
        # all affine x -> a x + b mod 97 with FIXED alphabets (AZ/KA pairs), periodic p 1..26, both orders, Vig/Beau/var
        idx = np.array(sorted(ANCH)); ptl = [ANCH[i] for i in idx]
        best_short = []; count_compat = {}
        for a in range(1, 97):
            for b in range(97):
                perm = [(a * x + b) % 97 for x in range(N)]; pos = {s: t for t, s in enumerate(perm)}
                tpos = np.array([pos[i] for i in idx])
                for order in ("TS", "ST"):
                    kidx = tpos if order == "TS" else idx
                    for pa in (AZ, KA):
                        for ca in (AZ, KA):
                            c = np.array([ca.index(K4[t]) for t in tpos]); q = np.array([pa.index(x) for x in ptl])
                            for md, req in (("vig", (c - q) % 26), ("beau", (c + q) % 26), ("var", (q - c) % 26)):
                                for p in range(1, 27):
                                    res = kidx % p; ok = True
                                    for rr in np.unique(res):
                                        v = req[res == rr]
                                        if (v != v[0]).any(): ok = False; break
                                    if ok:
                                        count_compat[p] = count_compat.get(p, 0) + 1
                                        if p <= 12: best_short.append((a, b, order, pa == KA, ca == KA, md, p))
        R = {"compatible_count_by_p": count_compat, "short_period_hits": best_short[:200], "n_short": len(best_short)}
    elif part == "15":
        # LCG key X(n+1) = a X(n) + c mod 26, X(0) = seed at K4 position 0 ; fixed alphabets: all a, c, seed
        idx = sorted(ANCH); fixed = []
        for a in range(26):
            for c in range(26):
                for s in range(26):
                    X = [s]
                    for _ in range(96): X.append((a * X[-1] + c) % 26)
                    for pa in (AZ, KA):
                        for ca in (AZ, KA):
                            for md in ("vig", "beau", "var"):
                                sc = 0
                                for i in idx:
                                    cc, q, k = ca.index(K4[i]), pa.index(ANCH[i]), X[i]
                                    sc += {"vig": (q + k) % 26 == cc, "beau": (k - q) % 26 == cc, "var": (q - k) % 26 == cc}[md]
                                if sc >= 8: fixed.append((sc, a, c, s, pa == KA, ca == KA, md))
        fixed.sort(reverse=True)
        anyal = []
        for a in range(26):
            for c in range(26):
                X = [11]
                for _ in range(96): X.append((a * X[-1] + c) % 26)
                for md in ("vig", "beau"):
                    if model(K4, "III", md, [(i, pt, [], X[i]) for i, pt in ANCH.items()]): anyal.append((a, c, md))
        R = {"fixed_best": fixed[:10], "fixed_max": fixed[0][0] if fixed else None, "seed11_anyalphabet_compatible": anyal}
    elif part == "19":
        for key in ("DYAHR", "YAR"):
            perm = columnar_perm(N, key)
            for order in ("TS", "ST"):
                for md in ("vig", "beau"):
                    out = []
                    for p in range(1, 27):
                        r = tperm(K4, perm, order, md, p)
                        if r: out.append((p, nulls(lambda c: tperm(c, perm, order, md, p))))
                        elif r is None: out.append((p, "undecided"))
                    R[f"{key} {order} {md}"] = out
    elif part == "20":
        for off in (27, 0):
            for order in ("ST", "TS"):
                for md in ("vig", "beau"):
                    for pp in (None, 2, 3, 4, 5, 6, 7):
                        r = t20(K4, off, order, md, pp)
                        R[f"off={off} {order} {md} periodic={pp}"] = {"K4": r, "null_of_20": nulls(lambda c: t20(c, off, order, md, pp), 20) if r else None}
    json.dump(R, open(f"results_{part}.json", "w"), indent=1, default=str)
    print("DONE", part, flush=True)
