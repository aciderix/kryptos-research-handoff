"""Row+column key, exact, with OR-Tools CP-SAT (AllDifferent + modular linear equations).
key(i) = a[i // w] + b[i % w];  Quagmire III (one unknown alphabet sigma, any of 26!):
  vig : sigma(ct) - sigma(pt) = a + b  (mod 26)      beau: sigma(ct) + sigma(pt) = a + b  (mod 26)
WLOG sigma('A') = 0 and b[0] = 0 (rotations are absorbed by a).  Usage: rowcol_cpsat.py MODE W [seed_for_random_ct]"""
import sys, json, time, random
from ortools.sat.python import cp_model
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
ANCH = {21 + j: c for j, c in enumerate("EASTNORTHEAST")} | {63 + j: c for j, c in enumerate("BERLINCLOCK")}
def solve(ct, mode, w, limit=600.0, workers=1):
    m = cp_model.CpModel()
    sg = {x: m.NewIntVar(0, 25, "s" + x) for x in AZ}
    m.AddAllDifferent(list(sg.values())); m.Add(sg["A"] == 0)
    rows = sorted({i // w for i in ANCH}); cols = sorted({i % w for i in ANCH})
    a = {r: m.NewIntVar(0, 25, f"a{r}") for r in rows}
    b = {c: m.NewIntVar(0, 25, f"b{c}") for c in cols}
    m.Add(b[cols[0]] == 0)
    for i, pt in ANCH.items():
        t = m.NewIntVar(-3, 3, f"t{i}")
        sgn = -1 if mode == "vig" else 1
        m.Add(sg[ct[i]] + sgn * sg[pt] - a[i // w] - b[i % w] == 26 * t)
    s = cp_model.CpSolver(); s.parameters.max_time_in_seconds = limit; s.parameters.num_search_workers = workers
    st = s.Solve(m)
    return {cp_model.OPTIMAL: True, cp_model.FEASIBLE: True, cp_model.INFEASIBLE: False}.get(st, None)
if __name__ == "__main__":
    mode, w = sys.argv[1], int(sys.argv[2])
    ct = K4
    if len(sys.argv) > 3:
        r = random.Random(int(sys.argv[3])); ct = "".join(r.choice(AZ) for _ in range(97))
    t = time.time(); res = solve(ct, mode, w)
    print(json.dumps({"mode": mode, "w": w, "ct": "K4" if len(sys.argv) <= 3 else f"rand{sys.argv[3]}", "sat": res, "sec": round(time.time() - t, 2)}))
