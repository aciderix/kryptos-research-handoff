"""t26_deepseek.py — hypothèses DeepSeek (25/09) : clé ligne + colonne en largeur 7, k[7n+j] = a[j] + b[n], alphabet σ
quelconque (Q3) ; variante Fibonacci b[n] = b[n−1] + b[n−2] ; variante « boussole » a[j] = 6·j.
e_min exact (CP-SAT) pour K4 et pour des témoins (lettres chiffrées aléatoires aux 24 positions, et K4 mélangé)."""
import sys, random, json
from ortools.sat.python import cp_model
from emin_cpsat import CRIBPOS, CRIBPT, K4C, CT

def emin(cc, mode, fib=False, a6=False, timeout=30):
    m = cp_model.CpModel()
    s = [m.NewIntVar(0, 25, f"s{x}") for x in range(26)]; m.AddAllDifferent(s); m.Add(s[CRIBPT[0]] == 0)
    a = [m.NewIntVar(0, 25, f"a{j}") for j in range(7)]
    b = [m.NewIntVar(0, 25, f"b{n}") for n in range(14)]
    if a6:
        for j in range(7): m.Add(a[j] == (6 * j) % 26)
    if fib:
        for n in range(2, 14):
            z = m.NewIntVar(0, 2, f"f{n}"); m.Add(b[n] == b[n - 1] + b[n - 2] - 26 * z)
    ok = []
    for t, i in enumerate(CRIBPOS):
        o = m.NewBoolVar(f"o{t}"); ok.append(o)
        p, c, j, n = CRIBPT[t], cc[t], i % 7, i // 7
        e = (s[c] - s[p] - a[j] - b[n]) if mode == "VIG" else (s[c] + s[p] - a[j] - b[n]) if mode == "BEAU" else (s[c] - s[p] + a[j] + b[n])
        z = m.NewIntVar(-4, 4, f"z{t}"); m.Add(e == 26 * z).OnlyEnforceIf(o)
    m.Maximize(sum(ok))
    sv = cp_model.CpSolver(); sv.parameters.max_time_in_seconds = timeout; sv.parameters.num_workers = 1
    st = sv.Solve(m)
    if st != cp_model.OPTIMAL: return None
    return 24 - int(round(sv.ObjectiveValue()))

if __name__ == "__main__":
    NN = int(sys.argv[1]) if len(sys.argv) > 1 else 50
    rnd = random.Random(26)
    FAMS = (("ligne+colonne 7", {}), ("Fibonacci sur les lignes", {"fib": True}), ("boussole a[j] = 6j", {"a6": True}))
    for name, kw in [f for f in FAMS if len(sys.argv) < 3 or f[0] != "ligne+colonne 7"]:
        for mode in ("VIG", "BEAU", "VARB"):
            e = emin(K4C, mode, **kw)
            hu, hs = {}, {}
            for z in range(NN):
                cc = [rnd.randrange(26) for _ in range(24)]
                x = emin(cc, mode, **kw); hu[str(x)] = hu.get(str(x), 0) + 1
                sh = list(CT); rnd.shuffle(sh); cs = [ord(sh[i]) - 65 for i in CRIBPOS]
                x = emin(cs, mode, **kw); hs[str(x)] = hs.get(str(x), 0) + 1
            print(json.dumps(dict(famille=name, mode=mode, K4=e, temoins_aleatoires=hu, temoins_K4_melange=hs), ensure_ascii=False), flush=True)
