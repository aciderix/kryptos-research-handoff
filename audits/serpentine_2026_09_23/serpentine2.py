"""Same test as serpentine.py, reorganised for speed (rule fixed before looking at new results):
phase 1: K4 only, all cells (resumes from results.jsonl); phase 2: 10 random-ciphertext controls ONLY for
periods p <= 20 where K4 is compatible (p > 20: controls not computed; long periods are known to be undecidable)."""
import json, os
from multiprocessing import Pool
from serpentine import snake, trans_periodic, K4, ANCH, rand_ct
done = {}
if os.path.exists("results.jsonl"):
    for l in open("results.jsonl"):
        if l.startswith("{"):
            r = json.loads(l); done[(r["w"], r["kind"], r["order"], r["mode"])] = r
CELLS = [(w, k, o, m) for w in range(7, 32) for k in ("rowsLR", "rowsRL", "colsDown", "colsUp")
         for o in ("TS", "ST") for m in ("vig", "beau")]
def phase1(c):
    w, k, o, m = c; perm = snake(w, k)
    return c, [p for p in range(1, 27) if trans_periodic(K4, ANCH, m, p, perm, o)]
def phase2(a):
    (w, k, o, m), p = a; perm = snake(w, k)
    return (w, k, o, m), p, sum(trans_periodic(rand_ct(4000 + s), ANCH, m, p, perm, o) for s in range(10))
if __name__ == "__main__":
    todo = [c for c in CELLS if c not in done]
    res = {c: done[c]["K4_sat_p"] for c in done}
    with Pool(3) as pool:
        for c, sp in pool.imap_unordered(phase1, todo, chunksize=2): res[c] = sp
        print("phase 1 done:", len(res), "cells", flush=True)
        nulls = {c: dict(done[c]["null_of_10"]) for c in done}
        need = [(c, p) for c, sp in res.items() for p in sp if p <= 20 and str(p) not in nulls.get(c, {})]
        for c, p, n in pool.imap_unordered(phase2, need, chunksize=2): nulls.setdefault(c, {})[str(p)] = n
    out = [dict(w=c[0], kind=c[1], order=c[2], mode=c[3], K4_sat_p=res[c],
                null_of_10={k: v for k, v in nulls.get(c, {}).items()}) for c in CELLS]
    json.dump(out, open("results_all.json", "w"), indent=1)
    sev = [(o["w"], o["kind"], o["order"], o["mode"], int(p)) for o in out for p, n in o["null_of_10"].items()
           if int(p) in o["K4_sat_p"] and n <= 1]
    print("cells:", len(out), " K4-compatible short periods (p<=12):",
          sorted({(o["w"], o["kind"], o["order"], o["mode"], p) for o in out for p in o["K4_sat_p"] if p <= 12}))
    print("severe (K4 compatible, controls <= 1/10):", sev)
