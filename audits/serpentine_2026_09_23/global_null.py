"""Global control for the serpentine scan: the SAME full scan (400 cells x p = 1..20) run on random ciphertexts;
compare the total number of compatible (cell, p) pairs with K4's.  Same conflict budget as serpentine3."""
import json, sys
from multiprocessing import Pool
from serpentine3 import tp, CELLS, snake, rand_ct
def scan_one(seed):
    ct = rand_ct(90000 + seed); cnt = {8: 0, 12: 0, 16: 0, 20: 0}; und = 0
    for (w, k, o, m) in CELLS:
        perm = snake(w, k)
        for p in range(1, 21):
            v = tp(ct, m, p, perm, o)
            if v is None: und += 1
            elif v:
                for P in cnt:
                    if p <= P: cnt[P] += 1
    return seed, cnt, und
if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 6
    with Pool(3) as pool, open("global_null.jsonl", "a") as f:
        for seed, cnt, und in pool.imap_unordered(scan_one, range(n)):
            f.write(json.dumps({"seed": seed, "counts": cnt, "undecided": und}) + "\n"); f.flush()
            print(seed, cnt, und, flush=True)
