"""run_families.py — e_min de K4 et des témoins pour une liste de familles (parallèle).
Usage : python3 run_families.py <groupe> <ntémoins> <sortie.jsonl>
Groupes : periodic, ...
Chaque ligne : famille, type, convention, e_min(K4), retrait optimal trouvé, histogramme des témoins, P(témoin ≤ K4).
"""
import sys, json, random, time
from multiprocessing import Pool
from emin_cpsat import emin, CRIBPT, K4C, CRIBPOS
import families as FM

def job(args):
    name, q, mode, nnull, seed = args
    fam = FM.get(name)
    e, drop, st = emin(fam, CRIBPT, K4C, q, mode, timeout=120)
    rnd = random.Random(seed)
    hist = {}
    part = 0
    for z in range(nnull):
        cc = [rnd.randrange(26) for _ in range(24)]
        ez, _, stz = emin(fam, CRIBPT, cc, q, mode, timeout=60)
        if stz != "opt": part += 1; ez = ez[0] if isinstance(ez, tuple) else ez
        hist[ez] = hist.get(ez, 0) + 1
    ek = e if not isinstance(e, tuple) else e[0]
    le = sum(v for k, v in hist.items() if k is not None and k <= ek)
    return dict(family=name, q=q, mode=mode, emin=e, drop=drop, status=st,
                null=dict(sorted((str(k), v) for k, v in hist.items())), nnull=nnull, null_partial=part,
                p_le=le / nnull)

if __name__ == "__main__":
    grp, nnull, out = sys.argv[1], int(sys.argv[2]), sys.argv[3]
    specs = FM.group(grp)
    jobs = [(n, q, m, nnull, 1000 + k) for k, (n, q, m) in enumerate(specs)]
    t0 = time.time()
    with Pool(4) as pool, open(out, "w") as f:
        for r in pool.imap_unordered(job, jobs):
            f.write(json.dumps(r, ensure_ascii=False) + "\n"); f.flush()
            print(r["family"], r["q"], r["mode"], "K4 e_min", r["emin"], r["drop"], "P(≤)", round(r["p_le"], 3), r["null"], f"{time.time()-t0:.0f}s", flush=True)
