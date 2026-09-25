"""T36b - same as T36, but the ciphertext coding tau is independent of the plaintext coding sigma
(the case the simulator designates: key letter read in the plaintext coding, ciphertext coding free):
  VIG  tau(c_i) = sigma(p_i) + sigma(p_{i-7}) + d   (d is absorbed into tau)
  BEAU tau(c_i) = sigma(p_{i-7}) - sigma(p_i) + d
  VAR  tau(c_i) = sigma(p_i) - sigma(p_{i-7}) + d
Question: does this family keep any power of decision on the 24 crib letters?
Usage: t36_tau_libre.py [time_limit_s] [n_null] [groups...]"""
import sys, random, time
from ortools.sat.python import cp_model
from t36_autocle_groupes import CT, PT, LAG, AZ, GROUPS, chains, shuffled

def equation2(a, b, mode, ct):
    """(sigma coefficients, tau coefficients) of the chain equation between crib positions a < b."""
    m = (b - a) // LAG
    s, t = {}, {}
    def add(dct, ch, k):
        dct[ch] = dct.get(ch, 0) + k
    add(s, PT[b], 1)
    if mode == "VIG":
        for j in range(1, m + 1):
            add(t, ct[a + LAG * j], -((-1) ** (m - j)))
        add(s, PT[a], -((-1) ** m))
    elif mode == "BEAU":
        for j in range(1, m + 1):
            add(t, ct[a + LAG * j], 1)
        add(s, PT[a], -1)
    else:
        for j in range(1, m + 1):
            add(t, ct[a + LAG * j], -1)
        add(s, PT[a], -1)
    return {k: v for k, v in s.items() if v}, {k: v for k, v in t.items() if v}

def solve2(ct, group, mode, tlimit=30.0):
    eqs = [equation2(a, b, mode, ct) for a, b in chains()]
    M = cp_model.CpModel()
    comps = list(group)
    strides, sz = [], 1
    for n in reversed(comps):
        strides.insert(0, sz); sz *= n
    def coding(tag, letters):
        v, idx = {}, {}
        for ch in letters:
            v[ch] = [M.NewIntVar(0, n - 1, f"{tag}{ch}{k}") for k, n in enumerate(comps)]
            idx[ch] = M.NewIntVar(0, sz - 1, f"{tag}i{ch}")
            M.Add(idx[ch] == sum(st * x for st, x in zip(strides, v[ch])))
        if len(idx) > 1:
            M.AddAllDifferent(list(idx.values()))
        return v, idx
    sl = sorted({ch for s, _ in eqs for ch in s})
    tl = sorted({ch for _, t in eqs for ch in t})
    sv, sidx = coding("s", sl)
    tv, _ = coding("t", tl)
    M.Add(sidx[sl[0]] == 0)
    oks = []
    for e, (s, t) in enumerate(eqs):
        ok = M.NewBoolVar(f"ok{e}"); oks.append(ok)
        for k, n in enumerate(comps):
            bound = (sum(map(abs, s.values())) + sum(map(abs, t.values()))) * n
            q = M.NewIntVar(-bound, bound, f"q{e}_{k}")
            M.Add(sum(c * sv[ch][k] for ch, c in s.items()) + sum(c * tv[ch][k] for ch, c in t.items()) == n * q).OnlyEnforceIf(ok)
    M.Maximize(sum(oks))
    S = cp_model.CpSolver()
    S.parameters.max_time_in_seconds = tlimit
    S.parameters.num_workers = 4
    st = S.Solve(M)
    if st in (cp_model.OPTIMAL, cp_model.FEASIBLE):
        return 17 - int(S.ObjectiveValue()), st == cp_model.OPTIMAL
    return None, False

if __name__ == "__main__":
    tlim = float(sys.argv[1]) if len(sys.argv) > 1 else 30
    nnull = int(sys.argv[2]) if len(sys.argv) > 2 else 10
    names = sys.argv[3:] or ["Z26", "Z27", "Z3^3", "Z30", "Z2^5 (XOR 5 bits)", "Z6xZ6"]
    rng = random.Random(3637)
    for name in names:
        g = GROUPS[name]
        for mode in (["VIG"] if all(n == 2 for n in g) else ["VIG", "BEAU", "VAR"]):
            t0 = time.time()
            k4 = solve2(CT, g, mode, tlim)
            nulls = [solve2(shuffled(rng), g, mode, tlim)[0] for _ in range(nnull)]
            print(f"{name:18s} {mode:4s} K4 e_min={k4} | nulls={nulls} ({time.time()-t0:.0f}s)", flush=True)
