"""serpentine2 + a conflict budget per SAT call (hard instances -> 'undecided' instead of blocking the run).
Same cells, same rule: phase 1 K4 only (all cells, p = 1..26); phase 2 controls (10 random) only for p <= 20 where K4 is compatible.
Progress is written after every cell (progress.jsonl)."""
import json, os, sys
from multiprocessing import Pool
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
import k4_algebraic as ka
from pysat.solvers import Cadical153
from serpentine import snake, K4, ANCH, rand_ct
BUDGET = 200000
def tp(ct, mode, p, perm, order):
    pos_of = {src: t for t, src in enumerate(perm)}
    m = ka.Model(); P, C = ka._alph(m, "III")
    for i, pt in ANCH.items():
        t = pos_of[i]; kidx = t if order == "TS" else i
        ka._relation(m, P, C, ct[t], pt, [(1, f"k{kidx % p}")], mode)
    if m.unsat: return False
    with Cadical153(bootstrap_with=m.clauses) as s:
        s.conf_budget(BUDGET); return s.solve_limited()          # True / False / None (undecided)
CELLS = [(w, k, o, m) for w in range(7, 32) for k in ("rowsLR", "rowsRL", "colsDown", "colsUp")
         for o in ("TS", "ST") for m in ("vig", "beau")]
def phase1(c):
    w, k, o, m = c; perm = snake(w, k)
    return c, {p: tp(K4, m, p, perm, o) for p in range(1, 27)}
def phase2(a):
    (w, k, o, m), p = a; perm = snake(w, k)
    v = [tp(rand_ct(4000 + s), m, p, perm, o) for s in range(10)]
    return (w, k, o, m), p, v
if __name__ == "__main__":
    done = {}
    for l in open("results.jsonl"):
        if l.startswith("{"):
            r = json.loads(l); done[(r["w"], r["kind"], r["order"], r["mode"])] = r
    res = {c: {p: (p in done[c]["K4_sat_p"]) for p in range(1, 27)} for c in done}
    nul = {c: {int(p): n for p, n in done[c]["null_of_10"].items()} for c in done}
    prog = open("progress.jsonl", "w")
    with Pool(3) as pool:
        for c, d in pool.imap_unordered(phase1, [c for c in CELLS if c not in done], chunksize=1):
            res[c] = d; prog.write(json.dumps({"phase": 1, "cell": c, "K4": {str(p): v for p, v in d.items()}}) + "\n"); prog.flush()
        need = [(c, p) for c, d in res.items() for p, v in d.items() if v is True and p <= 20 and p not in nul.get(c, {})]
        for c, p, v in pool.imap_unordered(phase2, need, chunksize=1):
            nul.setdefault(c, {})[p] = v; prog.write(json.dumps({"phase": 2, "cell": c, "p": p, "null": v}) + "\n"); prog.flush()
    out = []
    for c in CELLS:
        d = res[c]
        out.append(dict(w=c[0], kind=c[1], order=c[2], mode=c[3],
                        K4_sat_p=[p for p, v in d.items() if v is True], K4_undecided_p=[p for p, v in d.items() if v is None],
                        null_of_10={str(p): (v if isinstance(v, int) else {"sat": sum(x is True for x in v), "undecided": sum(x is None for x in v)})
                                    for p, v in nul.get(c, {}).items()}))
    json.dump(out, open("results_all.json", "w"), indent=1)
    print("DONE", flush=True)
