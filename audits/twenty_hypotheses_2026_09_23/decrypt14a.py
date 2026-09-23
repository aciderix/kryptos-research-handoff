import sys
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
import k4_algebraic as ka
from pysat.solvers import Cadical153
from tests import perms14, K4, ANCH
AZ = ka.AZ; P = perms14()
def dec(name, order, mode, p, maxn=200):
    perm = P[name]; pos_of = {s: t for t, s in enumerate(perm)}
    m = ka.Model(); Pp, C = ka._alph(m, "III")
    for i, pt in ANCH.items():
        t = pos_of[i]; ka._relation(m, Pp, C, K4[t], pt, [(1, f"k{(t if order == 'TS' else i) % p}")], mode)
    names = [f"P{c}" for c in AZ] + [f"k{r}" for r in range(p) if (f"k{r}", 0) in m.lit]
    outs = []
    with Cadical153(bootstrap_with=m.clauses) as s:
        while len(outs) < maxn and s.solve():
            mod = set(l for l in s.get_model() if l > 0)
            v = {x: next(u for u in range(26) if m.lit[(x, u)] in mod) for x in names}
            sig = {c: v["P" + c] for c in AZ}; inv = {u: c for c, u in sig.items()}
            pt = ["?"] * 97
            for t in range(97):
                i = perm[t]; k = v.get(f"k{(t if order == 'TS' else i) % p}")
                pt[i] = "." if k is None else inv[((sig[K4[t]] - k) if mode == "vig" else (k - sig[K4[t]])) % 26]
            outs.append("".join(pt)); s.add_clause([-m.lit[(x, v[x])] for x in names])
    cons = "".join(c if all(o[j] == c for o in outs) else "?" for j, c in enumerate(outs[0]))
    return len(outs), cons, outs[0]
for cell in (("mul88", "ST", "vig", 7), ("k3form93", "ST", "vig", 7), ("k3form50", "ST", "vig", 7)):
    n, cons, ex = dec(*cell); print(cell, n, "solutions\n   consensus:", cons, "\n   exemple  :", ex)
