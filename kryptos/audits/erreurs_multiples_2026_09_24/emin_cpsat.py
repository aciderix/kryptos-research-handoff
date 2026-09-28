"""emin_cpsat.py — nombre minimal d'erreurs (e_min) par CP-SAT (OR-Tools), exact.
Modèle : alphabets inconnus (valeurs 0..25 toutes différentes), clé structurée
  clé(i) = Σ coef·K[v] + const ; X(c) − s·Y(p) − sk·clé(i) = 26·z  (z entier borné), imposé seulement si ok[t].
Objectif : maximiser Σ ok[t].  e_min = 24 − optimum.
Types : Q3 (σ/σ), Q4 (σ1 clair / σ2 chiffré), Q2 (A–Z / σ), Q1 (σ / A–Z). Conventions VIG, BEAU, VARB.
"""
from ortools.sat.python import cp_model
CT = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIBPOS = list(range(21, 34)) + list(range(63, 74))
CRIBPT = [ord(x) - 65 for x in "EASTNORTHEAST" + "BERLINCLOCK"]
K4C = [ord(CT[i]) - 65 for i in CRIBPOS]

def emin(fam, pc, cc, q="Q3", mode="VIG", timeout=60.0, workers=1, forbid=None, want_all=False):
    """fam(i) -> list of (keyvar, coef) , const.  Renvoie (e_min, ensemble retiré, statut)."""
    m = cp_model.CpModel()
    s1 = [m.NewIntVar(0, 25, f"a{L}") for L in range(26)]
    m.AddAllDifferent(s1)
    s2 = None
    if q == "Q4":
        s2 = [m.NewIntVar(0, 25, f"b{L}") for L in range(26)]
        m.AddAllDifferent(s2)
    keys = {}
    ok = []
    first = True
    for t, i in enumerate(CRIBPOS):
        p, c = pc[t], cc[t]
        terms, cst = fam(i)
        o = m.NewBoolVar(f"ok{t}"); ok.append(o)
        expr = 0
        # côté chiffré
        if q in ("Q3", "Q2"): expr += s1[c]
        elif q == "Q4": expr += s2[c]
        else: expr += c
        sp = 1 if mode == "BEAU" else -1
        if q in ("Q3", "Q4", "Q1"): expr += sp * s1[p]
        else: expr += sp * p
        sk = 1 if mode == "VARB" else -1
        for (v, co) in terms:
            if v not in keys: keys[v] = m.NewIntVar(0, 25, f"k{v}")
            expr += sk * co * keys[v]
        expr += sk * cst
        # borne de z : |expr| ≤ 25*(2+Σ|coef|) + |cst|
        B = (25 * (2 + sum(abs(co) for _, co in terms)) + abs(cst)) // 26 + 1
        z = m.NewIntVar(-B, B, f"z{t}")
        m.Add(expr == 26 * z).OnlyEnforceIf(o)
    # brisure de symétrie (décalage d'alphabet absorbé par la clé)
    if q == "Q2": m.Add(s1[cc[0]] == 0)
    else: m.Add(s1[pc[0]] == 0)
    if q == "Q4": m.Add(s2[cc[0]] == 0)
    if forbid:
        for S in forbid:  # interdire des solutions déjà trouvées (énumération)
            m.AddBoolOr([ok[t] for t in S])
    m.Maximize(sum(ok))
    sv = cp_model.CpSolver()
    sv.parameters.max_time_in_seconds = timeout
    sv.parameters.num_workers = workers
    st = sv.Solve(m)
    if st == cp_model.OPTIMAL:
        best = int(round(sv.ObjectiveValue()))
        drop = [CRIBPOS[t] for t in range(24) if not sv.Value(ok[t])]
        return 24 - best, drop, "opt"
    if st == cp_model.FEASIBLE:
        best = int(round(sv.ObjectiveValue())); bound = int(sv.BestObjectiveBound())
        drop = [CRIBPOS[t] for t in range(24) if not sv.Value(ok[t])]
        return (24 - best, 24 - bound), drop, "partial"
    return None, None, "fail"

def periodic(p):
    return lambda i: ([(i % p, 1)], 0)

if __name__ == "__main__":
    import sys, time
    for p in [7, 12, 5, 3]:
        t0 = time.time(); r = emin(periodic(p), CRIBPT, K4C, "Q3", "VIG", workers=4)
        print(p, r, f"{time.time()-t0:.2f}s")
