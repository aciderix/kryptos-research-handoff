"""Global control for #14a: the same scan (1024 configurations x p = 1..12) on random ciphertexts."""
import json, sys
from multiprocessing import Pool
from tests import perms14, tperm, rand_ct
P = perms14()
CONF = [(pm, o, md) for pm in P.values() for o in ("TS", "ST") for md in ("vig", "beau")]
def scan(seed):
    ct = rand_ct(52000 + seed); cnt = {8: 0, 10: 0, 12: 0}
    for pm, o, md in CONF:
        for p in range(1, 13):
            if tperm(ct, pm, o, md, p) is True:
                for k in cnt:
                    if p <= k: cnt[k] += 1
    return seed, cnt
if __name__ == "__main__":
    with Pool(3) as pool, open("null14a.jsonl", "w") as f:
        for s, c in pool.imap_unordered(scan, range(6)):
            f.write(json.dumps({"seed": s, "counts": c}) + "\n"); f.flush()
