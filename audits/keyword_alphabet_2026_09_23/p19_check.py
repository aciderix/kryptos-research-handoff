"""Follow-up of the period-19 cells: larger null + extraction of solutions."""
import json, sys
from multiprocessing import Pool
from pysat.solvers import Cadical153
from keyword_quagmire import Model, _alph, _relation, keyword_constraint, K4, ANCH, AZ, rand_ct

def build(ct, mode, p, q, L):
    m = Model(); P, C = _alph(m, q)
    for nm in list(m.perms): keyword_constraint(m, nm, L)
    for i, pt in ANCH.items():
        _relation(m, P, C, ct[i], pt, [(1, f"k{i % p}")], mode)
    return m

def nul(args):
    s, mode, q, L = args
    m = build(rand_ct(20000 + s), mode, 19, q, L)
    return m.solve()

def solutions(mode, q, L, maxn=50):
    m = build(K4, mode, 19, q, L)
    names = [n for n in {k[0] for k in m.lit} if n[0] in "PCk" and not n.startswith("_")]
    sols = []
    with Cadical153(bootstrap_with=m.clauses) as s:
        while len(sols) < maxn and s.solve():
            mod = set(l for l in s.get_model() if l > 0)
            val = {n: next(v for v in range(26) if m.lit[(n, v)] in mod) for n in names}
            sols.append(val)
            # block this assignment of alphabet + key
            s.add_clause([-m.lit[(n, val[n])] for n in names])
    return sols

if __name__ == "__main__":
    for q, mode, L in (("II","vig",11),("II","beau",10),("IV","vig",6),("IV","beau",6)):
        with Pool(4) as pool:
            ns = sum(pool.map(nul, [(s, mode, q, L) for s in range(200)]))
        print(q, mode, "L", L, "null SAT", ns, "/200", flush=True)
