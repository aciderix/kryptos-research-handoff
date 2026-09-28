"""t23_autocle_chiffre.py — autoclé sur le CHIFFRÉ à l'écart L, avec erreurs (e_min exact, CP-SAT).

Modèle : la clé en i est la lettre chiffrée i − L, lue
  - 'sig' : dans le même alphabet inconnu σ (convention de K1–K2 : σ(c_i) = σ(p_i) + σ(c_{i−L})) ;
  - 'AZ'  : par son rang A–Z ;  'KA' : par son rang dans KRYPTOS…
Tableau à alphabet σ quelconque ; conventions VIG, BEAU, VARB. Tout le chiffré est gravé : la clé est connue
aux 24 positions, seul σ est inconnu. Une erreur de chiffrement en j ne casse que l'équation en j (la suite
chiffre avec la lettre gravée), donc « e erreurs » = « e équations retirées ».
Hypothèse NSA (1992) : L = 7. Éliminée sans erreur par 22 (A) / 72 (C) ; jamais testée avec erreurs.
Sortie : e_min de K4 pour L = 1..96, et témoins (chiffré aléatoire complet : la clé change aussi).
"""
import sys, random, json
from ortools.sat.python import cp_model
from emin_cpsat import CT, CRIBPOS, CRIBPT
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"

def emin_ak(ct, L, mode, keyread, timeout=60):
    m = cp_model.CpModel()
    s = [m.NewIntVar(0, 25, f"s{x}") for x in range(26)]
    m.AddAllDifferent(s)
    ok = []
    for t, i in enumerate(CRIBPOS):
        j = i - L
        if j < 0: continue
        p, c, k = CRIBPT[t], ct[i], ct[j]
        o = m.NewBoolVar(f"o{t}"); ok.append((t, o))
        key = s[k] if keyread == "sig" else (k if keyread == "AZ" else KA.index(chr(65 + k)))
        if mode == "VIG":  e = s[c] - s[p] - key
        elif mode == "BEAU": e = s[c] + s[p] - key
        else: e = s[c] - s[p] + key
        z = m.NewIntVar(-3, 3, f"z{t}")
        m.Add(e == 26 * z).OnlyEnforceIf(o)
    m.Maximize(sum(o for _, o in ok))
    sv = cp_model.CpSolver(); sv.parameters.max_time_in_seconds = timeout; sv.parameters.num_workers = 1
    st = sv.Solve(m)
    n = len(ok)
    if st != cp_model.OPTIMAL: return None, None, n
    best = int(round(sv.ObjectiveValue()))
    drop = [CRIBPOS[t] for t, o in ok if not sv.Value(o)]
    sig = [sv.Value(x) for x in s]
    return n - best, drop, n, sig

if __name__ == "__main__":
    NN = int(sys.argv[1]) if len(sys.argv) > 1 else 50
    Ls = list(map(int, sys.argv[2].split(","))) if len(sys.argv) > 2 else list(range(1, 22))
    ct = [ord(x) - 65 for x in CT]
    rnd = random.Random(7)
    for L in Ls:
        for keyread in ("sig", "AZ", "KA"):
            for mode in ("VIG", "BEAU", "VARB"):
                r = emin_ak(ct, L, mode, keyread)
                e, drop, n = r[0], r[1], r[2]
                hist = {}
                for z in range(NN):
                    rc = [rnd.randrange(26) for _ in range(97)]
                    ez = emin_ak(rc, L, mode, keyread)[0]
                    hist[ez] = hist.get(ez, 0) + 1
                le = sum(v for k2, v in hist.items() if k2 is not None and e is not None and k2 <= e)
                print(json.dumps(dict(L=L, key=keyread, mode=mode, n=n, emin=e, drop=drop,
                                      null=dict(sorted(hist.items())), p_le=le / NN)), flush=True)
