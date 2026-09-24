"""Null for the only one-slip survivors on K4 (Quagmire I and II, period 13): exact compatibility rate of random
ciphertexts at p = 13 (no slip needed)."""
import json, os, sys
from multiprocessing import Pool
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "algebraic_elimination_2026_09_22"))
from k4_algebraic import ANCH, periodic, rand_ct

def w(a):
    i, q = a
    return i, q, periodic(rand_ct(9000 + i), ANCH, "vig", 13, q)

if __name__ == "__main__":
    with Pool(4) as p:
        r = p.map(w, [(i, q) for i in range(20) for q in ("I", "II")])
    out = {q: sum(1 for i, qq, ok in r if qq == q and ok) / 20 for q in ("I", "II")}
    json.dump(out, open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "results_null_p13.json"), "w"))
    print("taux de compatibilité exacte au hasard, p=13:", out)
