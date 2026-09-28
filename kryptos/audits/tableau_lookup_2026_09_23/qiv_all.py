import sys, json
from multiprocessing import Pool
exec(open("tableau_lookup.py").read().split("res = []")[0])
CELLS = [(mo, fb, d, md) for mo in ("FOLD", "FRONT_MIRROR") for fb in (False, True) for d in range(-2, 3)
         for md in ("vig", "beau") if H(mo, fb, d) is not None]
HS = {c: H(c[0], c[1], c[2]) for c in CELLS}
def all_pass(ct): return all(sat(ct, HS[c], "IV", c[3]) for c in CELLS)
def one(t): return all_pass(rand_ct(70000 + t))
if __name__ == "__main__":
    print("QIV cells:", len(CELLS), " K4 passes all:", all_pass(K4))
    with Pool(4) as p: v = p.map(one, range(200))
    print("random ciphertexts passing ALL QIV cells:", sum(v), "/200")
    json.dump({"cells": len(CELLS), "K4_all": all_pass(K4), "null_all": sum(v)}, open("qiv_all.json", "w"))
