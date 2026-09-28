import json, sys
from tests import tperm, columnar_perm, N, rand_ct, K4
def count(ct):
    n = 0
    for key in ("DYAHR", "YAR"):
        perm = columnar_perm(N, key)
        for order in ("TS", "ST"):
            for md in ("vig", "beau"):
                n += sum(1 for p in range(1, 27) if tperm(ct, perm, order, md, p))
    return n
k4 = count(K4); rnd = [count(rand_ct(31000 + s)) for s in range(10)]
print("#19 cases compatibles : K4 =", k4, "; 10 chiffrés aléatoires =", sorted(rnd), "; aléatoires >= K4 :", sum(r >= k4 for r in rnd), "/10")
json.dump({"K4": k4, "random": rnd}, open("null19.json", "w"))
