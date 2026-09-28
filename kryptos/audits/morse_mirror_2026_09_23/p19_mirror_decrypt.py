"""Decrypt K4 with the solutions of the period-19 mirror cells (Quagmire III)."""
import sys, collections
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from pysat.solvers import Cadical153
import k4_algebraic as ka
from k4_algebraic import K4, ANCH, AZ
MIR = {"A":"N","N":"A","B":"V","V":"B","D":"U","U":"D","F":"L","L":"F","G":"W","W":"G","Q":"Y","Y":"Q"}
m_ = lambda s: "".join(MIR.get(c, c) for c in s)
def solve_all(side, var, mode, p=19, maxn=300):
    ct = m_(K4) if side in ("ct", "both") else K4
    anch = {i: (m_(c) if side in ("pt", "both") else c) for i, c in ANCH.items() if not (var == "b" and i == 70)}
    m = ka.Model(); P, C = ka._alph(m, "III")
    for i, pt in anch.items(): ka._relation(m, P, C, ct[i], pt, [(1, f"k{i % p}")], mode)
    names = [f"P{c}" for c in AZ] + [f"k{r}" for r in range(p) if (f"k{r}", 0) in m.lit]
    outs = collections.Counter()
    with Cadical153(bootstrap_with=m.clauses) as s:
        n = 0
        while n < maxn and s.solve():
            mod = set(l for l in s.get_model() if l > 0)
            val = {x: next(v for v in range(26) if m.lit[(x, v)] in mod) for x in names}
            pos = {c: val["P" + c] for c in AZ}; inv = {v: c for c, v in pos.items()}
            sg = 1 if mode == "vig" else -1
            out = "".join(inv[(sg * (pos[ch] - val[f"k{i % p}"])) % 26] if f"k{i % p}" in val else "." for i, ch in enumerate(ct))
            if side in ("pt", "both"): out = m_(out)
            outs[out] += 1; n += 1
            s.add_clause([-m.lit[(x, val[x])] for x in names])
    return n, outs
for cell in (("ct","a","beau"),("ct","b","vig"),("pt","b","vig"),("both","b","vig")):
    n, outs = solve_all(*cell)
    print(cell, n, "solutions (max 300)")
    for t, k in outs.most_common(2): print("   ", t)
