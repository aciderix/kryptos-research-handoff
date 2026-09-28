"""Per-lag null rate for the plaintext-autokey survivors (unknown alphabet)."""
import json
from k4_algebraic import autokey, rand_ct, K4, ANCH
out = {}
for mode, lags in (("beau", (6, 7, 8, 10)), ("vig", (32, 33, 34, 35))):
    for L in lags:
        n = 60
        hits = sum(1 for s in range(n) if autokey(rand_ct(20000 + s), ANCH, mode, L, "PT"))
        cons = sum(1 for i in ANCH if i - L in ANCH)
        out[f"{mode}_L{L}"] = {"K4": bool(autokey(K4, ANCH, mode, L, "PT")),
                               "n_constraints": cons, "random_sat_rate": f"{hits}/{n}"}
        print(mode, L, out[f"{mode}_L{L}"], flush=True)
json.dump(out, open("results_autokey_pt_lags_null.json", "w"), indent=1)
