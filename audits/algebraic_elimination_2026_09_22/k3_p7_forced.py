"""For each K3-route cell that is SAT for K4 at period 7, find the plaintext letters OUTSIDE the
cribs that are FORCED (identical in every alphabet/key compatible with the 24 anchors).
A real mechanism predicts new letters; a chance fit leaves them free or gibberish.

Method: solve once, read the decrypt of position j, then ask the solver for a model where that
position decrypts to anything else. UNSAT -> forced.
"""
import json

from pysat.solvers import Cadical153

from k4_algebraic import AZ, ANCH, K4, Model, _alph, _relation

CELLS = [("A", 91, "ST", "vig"), ("B", 81, "TS", "vig"), ("B", 81, "ST", "vig"),
         ("A", 85, "ST", "vig"), ("A", 58, "ST", "vig")]
P_ = 7


def layout(model, mult):
    """For every PT97 index j: (CT string position, CT letter)."""
    if model == "A":
        ct, mod, n = "?" + K4, 99, 98
        inv = pow(mult, -1, mod)
        src = [(inv * (t + 1)) % mod - 1 for t in range(n)]      # PT98 index at CT pos t
        q = src[0]
        pos98 = {i: t for t, i in enumerate(src)}
        return {j: pos98[j if j < q else j + 1] for j in range(97)}, ct
    ct, mod = K4, 98
    inv = pow(mult, -1, mod)
    src = [(inv * (t + 1)) % mod - 1 for t in range(97)]
    return {i: t for t, i in enumerate(src)}, ct


def key_index(model, order, j, t):
    if order == "ST":
        return j
    return t - 1 if model == "A" else t


def analyse(model, mult, order, mode):
    where, ct = layout(model, mult)
    m = Model()
    P, C = _alph(m, "III")
    for j, ch in ANCH.items():
        t = where[j]
        _relation(m, P, C, ct[t], ch, [(1, f"k{key_index(model, order, j, t) % P_}")], mode)
    # decrypt variable for every position: x_j with  C(ct) - s*x_j - k = 0, x_j = P(pt letter)
    sg = 1 if mode == "vig" else -1
    for j in range(97):
        t = where[j]
        m.var(f"x{j}")
        m.lin([(1, "P" + ct[t]), (-sg, f"x{j}"), (-1, f"k{key_index(model, order, j, t) % P_}")])
    with Cadical153(bootstrap_with=m.clauses) as s:
        assert s.solve()
        mod = set(l for l in s.get_model() if l > 0)
        val = {n: v for (n, v), lit in m.lit.items() if lit in mod}
        inv_p = {val["P" + c]: c for c in AZ}
        text, forced = [], 0
        for j in range(97):
            letter0 = inv_p[val[f"x{j}"]]
            if j in ANCH:
                text.append(letter0)
                continue
            # the LETTER is forced iff no model has x_j != P(letter0):  d = x_j - P(letter0) != 0
            before = len(m.clauses)
            m.lin([(1, f"x{j}"), (-1, "P" + letter0), (-1, f"d{j}")])
            for cl in m.clauses[before:]:
                s.add_clause(cl)
            if not s.solve(assumptions=[-m.lit[(f"d{j}", 0)]]):
                text.append(letter0)
                forced += 1
            else:
                text.append(".")
    return "".join(text), forced


if __name__ == "__main__":
    out = {}
    for c in CELLS:
        txt, f = analyse(*c)
        key = "-".join(map(str, c)) + "-p7"
        out[key] = {"forced_outside_cribs": f, "plaintext_forced_letters": txt}
        print(key, f, txt, flush=True)
    json.dump(out, open("results_k3_p7_forced.json", "w"), indent=1)
