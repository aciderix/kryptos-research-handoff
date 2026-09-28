"""Summary over the COMPLETED parts of k3_multiplicative.py (shards 2 and 3 fully; shard 1 model A
from its log, where every configuration with a short-period hit is printed). Shard 0 did not finish."""
import ast, glob, json
rows = []
for f in ("results_k3_multiplicative_shard2of4.json", "results_k3_multiplicative_shard3of4.json"):
    d = json.load(open(f))
    for m in ("modelA", "modelB"):
        for r in d[m]:
            rows.append(dict(r, model=m[-1]))
n_cells = len(rows) * 13
lines = open("logs/k3_multiplicative_s1.log").read().splitlines()
cut = next(i for i, l in enumerate(lines) if l.startswith("Model A:"))
s1A = [dict(ast.literal_eval(l[2:]), model="A") for l in lines[:cut] if l.startswith("A ")]
rows += s1A
n_cells += 60 * 13
hits = sum(1 for r in rows for p in r["K4_sat"] if p <= 13)
nulls = [json.load(open(f)) for f in sorted(glob.glob("results_k3_family_null_seed*.json"))]
tr = sum(n["trials"] for n in nulls); sat = sum(n["sat"] for n in nulls)
rare = [dict(model=r["model"], mult=r["mult"], order=r["order"], mode=r["mode"], p=int(p), null=v)
        for r in rows for p, v in r.get("null_rate_short", {}).items() if int(v.split("/")[0]) <= 1]
out = {"coverage": "shards 2,3 complete (both models); shard 1 model A; shard 0 and shard 1 model B not finished",
       "n_configurations_covered": len(rows) and (n_cells // 13),
       "n_short_period_cells": n_cells, "K4_short_period_hits": hits, "K4_rate": hits / n_cells,
       "null_trials": tr, "null_sat": sat, "null_rate": sat / tr,
       "expected_hits_under_null": sat / tr * n_cells,
       "K4_hits_with_null_rate_le_1_of_30": rare}
json.dump(out, open("results_k3_multiplicative_partial_summary.json", "w"), indent=1)
print(json.dumps(out, indent=1))
