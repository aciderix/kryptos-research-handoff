"""Vérification explicite (Z/26, alphabet σ quelconque commun, sans décalage) : combien de lettres
de crib faut-il retirer pour que l'autoclé sur le clair à l'écart 7 devienne compatible ?
On cherche σ par CP-SAT, puis on REJOUE la récurrence lettre par lettre sur le vrai chiffré pour
chaque classe mod 7 : aucune équation n'est supposée, tout est recalculé.
Réponse à audits/autocle_algebre_2026_09_25 (qui compte σ(X) = 0 comme une contradiction, alors
qu'une permutation envoie toujours une lettre sur 0)."""
from ortools.sat.python import cp_model
from t36_autocle_groupes import CT, PT, AZ, LAG

def chains(known):
    out = []
    for r in range(LAG):
        cl = sorted(p for p in known if p % LAG == r)
        out += list(zip(cl, cl[1:]))
    return out

def step(x_prev, y, mode):          # x_i en fonction de x_{i-7} et y_i
    return {"VIG": y - x_prev, "VAR": x_prev + y, "BEAU": x_prev - y}[mode] % 26

def find_sigma(mode, removed):
    known = {p: c for p, c in PT.items() if p not in removed}
    M = cp_model.CpModel()
    s = {ch: M.NewIntVar(0, 25, ch) for ch in AZ}
    M.AddAllDifferent(list(s.values()))
    for a, b in chains(known):
        m = (b - a) // LAG
        # développement symbolique de la récurrence
        coef = {known[a]: 1}
        for t in range(1, m + 1):
            y = CT[a + LAG * t]
            if mode == "VIG":
                coef = {k: -v for k, v in coef.items()}; coef[y] = coef.get(y, 0) + 1
            elif mode == "VAR":
                coef[y] = coef.get(y, 0) + 1
            else:
                coef[y] = coef.get(y, 0) - 1
        coef[known[b]] = coef.get(known[b], 0) - 1
        q = M.NewIntVar(-400, 400, f"q{a}_{b}")
        M.Add(sum(v * s[k] for k, v in coef.items()) == 26 * q)
    S = cp_model.CpSolver(); S.parameters.max_time_in_seconds = 60; S.parameters.num_workers = 4
    st = S.Solve(M)
    if st not in (cp_model.OPTIMAL, cp_model.FEASIBLE):
        return None
    return {ch: S.Value(s[ch]) for ch in AZ}

def replay(sig, mode, removed):
    """Rejoue la récurrence sur le vrai chiffré ; renvoie les rangs de crib non respectés."""
    known = {p: c for p, c in PT.items() if p not in removed}
    bad = []
    for r in range(LAG):
        cl = sorted(p for p in known if p % LAG == r)
        if not cl: continue
        x = sig[known[cl[0]]]; i = cl[0]
        while i + LAG <= cl[-1]:
            i += LAG
            x = step(x, sig[CT[i]], mode)
            if i in known and x != sig[known[i]]:
                bad.append(i); x = sig[known[i]]
    return bad

if __name__ == "__main__":
    for mode in ("VIG", "VAR", "BEAU"):
        print(mode, "sans retrait :", "compatible" if find_sigma(mode, ()) else "IMPOSSIBLE")
        ok = []
        for p in sorted(PT):
            sig = find_sigma(mode, (p,))
            if sig is not None:
                assert replay(sig, mode, (p,)) == [], (mode, p)
                ok.append(p)
        print(f"   un seul retrait suffit en : {ok if ok else 'aucun'}")
        if ok:
            sig = find_sigma(mode, (ok[-1],))
            print("   exemple (retrait", ok[-1], ") σ =", "".join(sorted(AZ, key=lambda c: sig[c])),
                  "| rejoué sur le chiffré :", replay(sig, mode, (ok[-1],)) or "aucun écart")
