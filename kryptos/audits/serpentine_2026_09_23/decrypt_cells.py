"""Decrypt K4 under the most striking serpentine cells (short period, 0/10 controls).
For each cell: enumerate up to 300 solutions (alphabet + key); print the consensus plaintext
(letter shown only if identical in all enumerated solutions, '?' otherwise)."""
import sys
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
import k4_algebraic as ka
from pysat.solvers import Cadical153
from serpentine import snake, K4, ANCH
AZ = ka.AZ
def solutions(w, kind, order, mode, p, maxn=300):
    perm = snake(w, kind); pos_of = {src: t for t, src in enumerate(perm)}
    m = ka.Model(); P, C = ka._alph(m, "III")
    for i, pt in ANCH.items():
        t = pos_of[i]; kidx = t if order == "TS" else i
        ka._relation(m, P, C, K4[t], pt, [(1, f"k{kidx % p}")], mode)
    keys = [f"k{r}" for r in range(p) if (f"k{r}", 0) in m.lit]
    names = [f"P{c}" for c in AZ] + keys
    outs = []
    with Cadical153(bootstrap_with=m.clauses) as s:
        while len(outs) < maxn and s.solve():
            mod = set(l for l in s.get_model() if l > 0)
            val = {x: next(v for v in range(26) if m.lit[(x, v)] in mod) for x in names}
            sig = {c: val["P" + c] for c in AZ}; inv = {v: c for c, v in sig.items()}
            pt = ["?"] * 97
            for t in range(97):
                i = perm[t]                                   # ciphertext position t holds plaintext index i (TS) / sub index i (ST)
                kidx = t if order == "TS" else i
                k = val.get(f"k{kidx % p}")
                if k is None: pt[i] = "."; continue
                x = (sig[K4[t]] - k) % 26 if mode == "vig" else (k - sig[K4[t]]) % 26
                pt[i] = inv[x]
            outs.append("".join(pt))
            s.add_clause([-m.lit[(x, val[x])] for x in names])
    cons = "".join(c if all(o[j] == c for o in outs) else "?" for j, c in enumerate(outs[0])) if outs else ""
    return len(outs), cons, outs[:2]
for cell in [(25, "colsDown", "ST", "vig", 7), (26, "colsDown", "TS", "vig", 8), (26, "colsDown", "ST", "vig", 8),
             (11, "rowsRL", "ST", "beau", 8), (27, "rowsRL", "ST", "beau", 8), (10, "rowsLR", "ST", "vig", 8),
             (7, "colsUp", "ST", "beau", 10), (8, "colsDown", "ST", "vig", 10)]:
    n, cons, ex = solutions(*cell)
    print(cell, n, "solutions")
    print("   consensus:", cons)
    print("   exemple  :", ex[0] if ex else "")
