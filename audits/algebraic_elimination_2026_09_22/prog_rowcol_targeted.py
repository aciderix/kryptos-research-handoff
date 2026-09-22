"""Targeted null rates for short-period progressive survivors + K4-only rowcol answers."""
import json, sys
from k4_algebraic import progressive, rowcol, rand_ct, K4, ANCH
out = {}
if sys.argv[1] == "prog":
    for mode, ps in (("vig", (8, 10, 11, 12)), ("beau", (11, 12))):
        for p in ps:
            n = 30
            hits = sum(1 for s in range(n) if progressive(rand_ct(30000 + s), ANCH, mode, p))
            out[f"{mode}_p{p}"] = {"K4": bool(progressive(K4, ANCH, mode, p)), "random_sat_rate": f"{hits}/{n}"}
            print(mode, p, out[f"{mode}_p{p}"], flush=True)
    json.dump(out, open("results_progressive_short_null.json", "w"), indent=1)
else:
    for mode in ("vig", "beau"):
        out[mode] = [w for w in range(2, 49) if rowcol(K4, ANCH, mode, w)]
        print("rowcol", mode, out[mode], flush=True)
    json.dump(out, open("results_k4_only_rowcol.json", "w"), indent=1)
