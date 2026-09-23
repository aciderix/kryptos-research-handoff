"""Family-level null for k3_multiplicative.py, by sampling: draw random cells
(model, multiplier, order, sense, period 1..13) and test each on random ciphertexts.
The pooled SAT rate times the number of cells gives the number of short-period hits
expected for a ciphertext with NO structure, to compare with the count observed for K4.

Run: python3 k3_multiplicative_family_null.py <seed> ; aggregate with --aggregate
"""
import glob
import json
import random
import sys
from math import gcd

from k3_multiplicative import group_mod, q3_periodic, rand_ct, triples_A, triples_B
from k4_algebraic import ANCH


def cells():
    GA = group_mod([2, 7], 99)
    MB = [m for m in range(1, 98) if gcd(m, 98) == 1]
    return [(md, m, o, sn, p) for md, ms in (("A", GA), ("B", MB)) for m in ms
            for o in ("TS", "ST") for sn in ("vig", "beau") for p in range(1, 14)]


def sample(seed, n_cells=40, n_ct=8):
    r = random.Random(seed)
    trials = hits = 0
    for model, mult, order, mode, p in r.sample(cells(), n_cells):
        for k in range(n_ct):
            ct97 = rand_ct(r.randrange(10**9))
            tr = triples_A("?" + ct97, ANCH, mult, order)[0] if model == "A" else \
                triples_B(ct97, ANCH, mult, order)
            hits += q3_periodic(tr, mode, p)
            trials += 1
    return {"seed": seed, "trials": trials, "sat": hits}


def aggregate():
    rows = []
    for f in sorted(glob.glob("results_k3_multiplicative_shard*of4.json")):
        d = json.load(open(f))
        for model in ("modelA", "modelB"):
            for r in d.get(model, []):
                r["model"] = model[-1]
                rows.append(r)
    k4_hits = sum(1 for r in rows for p in r["K4_sat"] if p <= 13)
    nulls = [json.load(open(f)) for f in sorted(glob.glob("results_k3_family_null_seed*.json"))]
    trials = sum(n["trials"] for n in nulls)
    rate = sum(n["sat"] for n in nulls) / trials if trials else None
    n_cells = len(cells())
    rare = [dict(model=r["model"], mult=r["mult"], order=r["order"], mode=r["mode"], p=p, null=v)
            for r in rows for p, v in r.get("null_rate_short", {}).items() if int(v.split("/")[0]) <= 1]
    out = {"n_configs": len(rows), "K4_short_period_hits": k4_hits,
           "n_cells_short_periods": n_cells,
           "null_pooled_sat_rate": rate, "null_trials": trials,
           "expected_hits_under_null": rate * n_cells if rate is not None else None, "K4_hits_with_null_rate_le_1_of_30": rare}
    json.dump(out, open("results_k3_multiplicative_summary.json", "w"), indent=1)
    print(json.dumps(out, indent=1))


if __name__ == "__main__":
    if sys.argv[1] == "--aggregate":
        aggregate()
    else:
        seed = int(sys.argv[1])
        out = sample(60000 + seed)
        json.dump(out, open(f"results_k3_family_null_seed{seed}.json", "w"))
        print(out)
