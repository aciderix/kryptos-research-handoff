"""K4-only answers for the two slow families (the full run with nulls is k4_algebraic.py)."""
import json, time
from k4_algebraic import progressive, rowcol, K4, ANCH
out = {}
for name, fn in (("progressive", progressive), ("rowcol", rowcol)):
    for mode in ("vig", "beau"):
        t = time.time()
        out[f"{name}_{mode}"] = [x for x in range(2, 49) if fn(K4, ANCH, mode, x)]
        print(name, mode, out[f"{name}_{mode}"], f"{time.time()-t:.0f}s", flush=True)
json.dump(out, open("results_k4_only_prog_rowcol.json", "w"), indent=1)
