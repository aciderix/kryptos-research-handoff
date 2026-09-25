"""t25_retournement.py — « on retourne la feuille » (Sanborn, Big Techday 2013) : la convention (Vigenère, Beaufort,
Beaufort variante) change selon la ligne de la feuille, la clé garde sa période p.
Motifs de retournement : par ligne du cuivre (31 ; K4 en colonne 27), par ligne de 7 (phase φ), en alternance
lettre à lettre de période q. Alphabet σ quelconque, Quagmire III (et Q4 en option). e_min exact par CP-SAT, témoins.
Usage : t25 p ntémoins motif      motif ∈ {line31, row7, alt}
"""
import sys, itertools, random, json
from ortools.sat.python import cp_model
from emin_cpsat import CRIBPOS, CRIBPT, K4C
MODES = ("VIG", "BEAU", "VARB")

def emin_mixed(p, conv, cc, q="Q3", timeout=30):
    m = cp_model.CpModel()
    s1 = [m.NewIntVar(0, 25, f"a{L}") for L in range(26)]; m.AddAllDifferent(s1)
    s2 = s1
    if q == "Q4":
        s2 = [m.NewIntVar(0, 25, f"b{L}") for L in range(26)]; m.AddAllDifferent(s2); m.Add(s2[cc[0]] == 0)
    k = [m.NewIntVar(0, 25, f"k{j}") for j in range(p)]
    m.Add(s1[CRIBPT[0]] == 0)
    ok = []
    for t, i in enumerate(CRIBPOS):
        mode = conv(i); o = m.NewBoolVar(f"o{t}"); ok.append(o)
        c, pp = cc[t], CRIBPT[t]
        e = (s2[c] - s1[pp] - k[i % p]) if mode == "VIG" else (s2[c] + s1[pp] - k[i % p]) if mode == "BEAU" else (s2[c] - s1[pp] + k[i % p])
        z = m.NewIntVar(-3, 3, f"z{t}"); m.Add(e == 26 * z).OnlyEnforceIf(o)
    m.Maximize(sum(ok))
    sv = cp_model.CpSolver(); sv.parameters.max_time_in_seconds = timeout; sv.parameters.num_workers = 1
    st = sv.Solve(m)
    if st != cp_model.OPTIMAL: return None, None
    return 24 - int(round(sv.ObjectiveValue())), [CRIBPOS[t] for t in range(24) if not sv.Value(ok[t])]

def patterns(kind):
    if kind == "line31":
        # lignes 26, 27, 28 portent les cribs ; la ligne 25 n'en porte pas
        for a in itertools.product(MODES, repeat=3):
            yield f"lignes26-28={'/'.join(a)}", (lambda a: (lambda i: a[(i + 27) // 31 - 1]))(a)
    elif kind == "row7":
        for phi in range(7):
            rows = sorted({(i - phi) // 7 for i in CRIBPOS})
            for a in itertools.product(MODES, repeat=len(rows)):
                d = dict(zip(rows, a))
                yield f"phi={phi} " + ",".join(f"{r}:{x[0]}" for r, x in d.items()), (lambda d, phi: (lambda i: d[(i - phi) // 7]))(d, phi)
    elif kind == "alt":
        for qq in (2, 3):
            for a in itertools.product(MODES, repeat=qq):
                if len(set(a)) == 1: continue
                yield f"alt{qq}={'/'.join(a)}", (lambda a, qq: (lambda i: a[i % qq]))(a, qq)

if __name__ == "__main__":
    p, NN, kind = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]
    q = sys.argv[4] if len(sys.argv) > 4 else "Q3"
    rnd = random.Random(25)
    for name, conv in patterns(kind):
        e, drop = emin_mixed(p, conv, K4C, q)
        if e is None: print(json.dumps(dict(p=p, pat=name, emin=None))); continue
        rec = dict(p=p, q=q, pat=name, emin=e, drop=drop)
        if e <= 1 and NN:
            h = {}
            for z in range(NN):
                cc = [rnd.randrange(26) for _ in range(24)]
                ez, _ = emin_mixed(p, conv, cc, q)
                h[str(ez)] = h.get(str(ez), 0) + 1
            rec["null"] = h
        print(json.dumps(rec, ensure_ascii=False), flush=True)
