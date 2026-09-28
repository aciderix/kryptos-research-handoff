"""Forced plaintext letters for the K4 one-slip hits of row_restart.py (Quagmire III).
A letter outside the cribs is FORCED if it is the same in every alphabet/key compatible with the 23 kept
anchors. A real mechanism must predict readable English there; a chance fit gives few or gibberish letters.
The dropped (slipped) position shows what the mechanism would have put there instead of the crib letter."""
import json, os, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "algebraic_elimination_2026_09_22"))
from pysat.solvers import Cadical153
from k4_algebraic import AZ, ANCH, K4, Model, _alph, _relation
import row_restart as rr

CELLS = [("PANEL", 8, "beau", 65), ("WS", 8, "beau", 25), ("PANEL", 13, "beau", 63)]


def analyse(al, p, mode, slip):
    idx = rr.ALIGN[al]
    anch = {k: v for k, v in ANCH.items() if k != slip}
    m = Model(); P, C = _alph(m, "III")
    for j, ch in anch.items():
        _relation(m, P, C, K4[j], ch, [(1, f"k{idx(j) % p}")], mode)
    sg = 1 if mode == "vig" else -1
    for j in range(97):
        m.var(f"x{j}")
        m.lin([(1, "P" + K4[j]), (-sg, f"x{j}"), (-1, f"k{idx(j) % p}")])
    text, forced = [], 0
    with Cadical153(bootstrap_with=m.clauses) as s:
        assert s.solve()
        mod = set(l for l in s.get_model() if l > 0)
        val = {n: v for (n, v), lit in m.lit.items() if lit in mod}
        inv_p = {val["P" + c]: c for c in AZ}
        for j in range(97):
            l0 = inv_p[val[f"x{j}"]]
            if j in anch:
                text.append(l0.lower()); continue
            before = len(m.clauses)
            m.lin([(1, f"x{j}"), (-1, "P" + l0), (-1, f"d{j}")])
            for cl in m.clauses[before:]:
                s.add_clause(cl)
            if not s.solve(assumptions=[-m.lit[(f"d{j}", 0)]]):
                text.append(l0); forced += 1
            else:
                text.append(".")
    return "".join(text), forced


if __name__ == "__main__":
    out = {}
    for c in CELLS:
        t, f = analyse(*c)
        out[f"{c}"] = {"text (lowercase = crib, UPPER = forced, . = free)": t, "forced_outside_cribs": f}
        print(c, f, "\n ", t)
    json.dump(out, open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "results_row_restart_forced.json"), "w"), indent=1)
