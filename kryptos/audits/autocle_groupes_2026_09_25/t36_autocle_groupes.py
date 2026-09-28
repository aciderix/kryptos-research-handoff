"""T36 - plaintext autokey at lag 7 computed in another group than Z/26 ("change the base of
language ... not to another language, to something else", Scheidt 2020).

Letters are mapped injectively to the elements of a finite abelian group G = Z/n1 x Z/n2 x ...
(grid coordinates added cell by cell, 5-bit codes combined by XOR, etc.). The same mapping
sigma is used for plaintext, key and ciphertext (as in K1-K2). Three conventions:
  VIG  sigma(c_i) = sigma(p_i) + sigma(p_{i-7}) + d     (the form the simulator designates)
  BEAU sigma(c_i) = sigma(p_{i-7}) - sigma(p_i) + d
  VAR  sigma(c_i) = sigma(p_i) - sigma(p_{i-7}) + d
Chain equations (as T18/T34): inside each class mod 7, between two consecutive crib positions
a < b = a + 7m, the relation propagates through the unknown plaintext and gives one equation
between known letters only: 24 crib positions - 7 classes = 17 equations. The unknown
intermediate plaintext letters are not required to be letters (relaxation), so UNSAT = eliminated.

For each group, convention and ciphertext, CP-SAT maximises the number of equations that hold
(e_min = 17 - max). Controls: planted ciphertexts built with the same family (positive), and
shuffled K4 (null). Usage: python3 t36_autocle_groupes.py [time_limit_s] [n_null] [groups...]"""
import sys, random, itertools, json, time
from ortools.sat.python import cp_model

CT = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIBS = {21: "EASTNORTHEAST", 63: "BERLINCLOCK"}
PT = {}
for s, w in CRIBS.items():
    for j, ch in enumerate(w):
        PT[s + j] = ch
LAG = 7
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

def chains():
    pos = sorted(PT)
    out = []
    for r in range(LAG):
        cl = [p for p in pos if p % LAG == r]
        for a, b in zip(cl, cl[1:]):
            out.append((a, b))
    return out

def equation(a, b, mode, ct):
    """Return (coef dict over letters, coef of d) for: sigma(p_b) - F(...) = 0 in G."""
    m = (b - a) // LAG
    coef = {}
    def add(ch, k):
        coef[ch] = coef.get(ch, 0) + k
    add(PT[b], 1)
    cd = 0
    if mode == "VIG":      # x_t = c_t - d - x_{t-1}
        for t in range(1, m + 1):
            sgn = (-1) ** (m - t)
            add(ct[a + LAG * t], -sgn); cd += sgn          # -(sgn*(c - d))
        add(PT[a], -((-1) ** m))
    elif mode == "BEAU":   # x_t = x_{t-1} - c_t + d
        for t in range(1, m + 1):
            add(ct[a + LAG * t], 1); cd -= 1
        add(PT[a], -1)
    elif mode == "VAR":    # x_t = x_{t-1} + c_t - d
        for t in range(1, m + 1):
            add(ct[a + LAG * t], -1); cd += 1
        add(PT[a], -1)
    return {k: v for k, v in coef.items() if v != 0}, cd

def solve(ct, group, mode, tlimit=30.0, min_ok=None):
    eqs = [equation(a, b, mode, ct) for a, b in chains()]
    letters = sorted({ch for co, _ in eqs for ch in co})
    M = cp_model.CpModel()
    comps = list(group)
    v = {ch: [M.NewIntVar(0, n - 1, f"{ch}{k}") for k, n in enumerate(comps)] for ch in letters}
    dv = [M.NewIntVar(0, n - 1, f"d{k}") for k, n in enumerate(comps)]
    strides, s = [], 1
    for n in reversed(comps):
        strides.insert(0, s); s *= n
    idx = {}
    for ch in letters:
        iv = M.NewIntVar(0, s - 1, f"i{ch}")
        M.Add(iv == sum(st * x for st, x in zip(strides, v[ch])))
        idx[ch] = iv
    M.AddAllDifferent(list(idx.values()))
    M.Add(idx[letters[0]] == 0)   # translation symmetry: sigma -> sigma + g changes only d
    oks = []
    for e, (co, cd) in enumerate(eqs):
        ok = M.NewBoolVar(f"ok{e}")
        oks.append(ok)
        for k, n in enumerate(comps):
            bound = (sum(abs(c) for c in co.values()) + abs(cd)) * n
            q = M.NewIntVar(-bound, bound, f"q{e}_{k}")
            M.Add(sum(c * v[ch][k] for ch, c in co.items()) + cd * dv[k] == n * q).OnlyEnforceIf(ok)
    if min_ok is None:
        M.Maximize(sum(oks))
    else:
        M.Add(sum(oks) >= min_ok)
    S = cp_model.CpSolver()
    S.parameters.max_time_in_seconds = tlimit
    S.parameters.num_workers = 4
    st = S.Solve(M)
    if min_ok is not None:
        return {cp_model.OPTIMAL: "SAT", cp_model.FEASIBLE: "SAT", cp_model.INFEASIBLE: "UNSAT"}.get(st, "UNKNOWN")
    if st in (cp_model.OPTIMAL, cp_model.FEASIBLE):
        best = int(S.ObjectiveValue()); bound = int(S.BestObjectiveBound())
        return 17 - best, 17 - bound, st == cp_model.OPTIMAL
    if st == cp_model.INFEASIBLE:
        return None
    return (None, None, False)

def elements(group):
    return list(itertools.product(*[range(n) for n in group]))

def add_el(x, y, group, sx=1, sy=1):
    return tuple((sx * a + sy * b) % n for a, b, n in zip(x, y, group))

def planted(group, mode, rng, corpus):
    """A fake K4 built with the family: random sigma into G, random d, English with cribs."""
    els = elements(group)
    for _ in range(2000):
        sig = dict(zip(AZ, rng.sample(els, 26)))
        inv = {e: ch for ch, e in sig.items()}
        d = rng.choice(els)
        st = rng.randrange(len(corpus) - 200)
        pt = list(corpus[st:st + 97])
        for p, ch in PT.items():
            pt[p] = ch
        ok, ct = True, []
        for i in range(97):
            if i < LAG:
                ct.append(rng.choice(AZ)); continue      # priming: any letter
            x, y = sig[pt[i]], sig[pt[i - LAG]]
            if mode == "VIG":
                e = add_el(add_el(x, y, group), d, group)
            elif mode == "BEAU":
                e = add_el(add_el(y, x, group, 1, -1), d, group)
            else:
                e = add_el(add_el(x, y, group, 1, -1), d, group)
            if e not in inv:
                ok = False; break
            ct.append(inv[e])
        if ok:
            return "".join(ct)
    return None

def shuffled(rng):
    l = list(CT); rng.shuffle(l); return "".join(l)

GROUPS = {
    "Z26": (26,), "Z27": (27,), "Z3xZ9": (3, 9), "Z3^3": (3, 3, 3), "Z28": (28,), "Z2xZ14": (2, 14),
    "Z29": (29,), "Z30": (30,), "Z31": (31,), "Z32": (32,), "Z2xZ16": (2, 16), "Z4xZ8": (4, 8),
    "Z2xZ2xZ8": (2, 2, 8), "Z2xZ4xZ4": (2, 4, 4), "Z2^3xZ4": (2, 2, 2, 4), "Z2^5 (XOR 5 bits)": (2, 2, 2, 2, 2),
    "Z33": (33,), "Z34": (34,), "Z35": (35,), "Z36": (36,), "Z2xZ18": (2, 18), "Z3xZ12": (3, 12), "Z6xZ6": (6, 6),
}

if __name__ == "__main__":
    tl = float(sys.argv[1]) if len(sys.argv) > 1 else 30
    nnull = int(sys.argv[2]) if len(sys.argv) > 2 else 8
    names = sys.argv[3:] or list(GROUPS)
    import os
    # English corpus for the planted controls: ../erreurs_multiples_2026_09_24/corpus_get.sh (Gutenberg)
    cpath = os.environ.get("CORPUS", "corpus_all.txt")
    corpus = "".join(ch for ch in open(cpath, errors="ignore").read(3_000_000).upper() if ch in AZ)
    rng = random.Random(36)
    res = {}
    for name in names:
        g = GROUPS[name]
        exp2 = all(n == 2 for n in g)
        for mode in (["VIG"] if exp2 else ["VIG", "BEAU", "VAR"]):
            t0 = time.time()
            k4 = solve(CT, g, mode, tl)
            ctl = planted(g, mode, rng, corpus)
            ctlr = solve(ctl, g, mode, tl) if ctl else "no planted text (not closed)"
            nulls = [solve(shuffled(rng), g, mode, tl) for _ in range(nnull)]
            res[f"{name} {mode}"] = dict(k4=k4, control=ctlr, nulls=nulls)
            print(f"{name:18s} {mode:4s} K4 e_min={k4} | control={ctlr} | nulls={[n[0] if n else n for n in nulls]} ({time.time()-t0:.0f}s)", flush=True)
    json.dump(res, open("results_t36.json", "w"), indent=1, default=str)
