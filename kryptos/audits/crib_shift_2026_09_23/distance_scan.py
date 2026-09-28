"""Minimal phenomenon: key equal for crib letters at cross-crib distance d (only those pairs),
Quagmire I-IV any alphabets, Vig.  K4 vs 200 random ciphertexts, d = 30..52."""
import sys, json
from multiprocessing import Pool
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import Model, _alph, _relation, K4, ANCH, rand_ct
C1 = range(21, 34); C2 = range(63, 74)

def sat(ct, d, q):
    m = Model(); P, C = _alph(m, q); n = 0
    for i in C1:
        if i + d in C2:
            for j in (i, i + d):
                _relation(m, P, C, ct[j], ANCH[j], [(1, f"k{i}")], "vig"); n += 1
    return m.solve() if n else None

def job(a):
    d, q = a
    return d, q, sat(K4, d, q), sum(bool(sat(rand_ct(40000 + t), d, q)) for t in range(200))

if __name__ == "__main__":
    with Pool(4) as p: R = p.map(job, [(d, q) for d in range(30, 53) for q in ("II", "IV")])
    for d, q, k, n in R: print(f"d={d} Q{q} pairs={sum(1 for i in C1 if i+d in C2)} K4={k} null={n}/200")
    exp = sum(n / 200 for d, q, k, n in R if q == "IV")
    print("expected chance passes over d (QIV):", round(exp, 2), " K4 passes:", sum(bool(k) for d, q, k, n in R if q == "IV"))
    json.dump(R, open("distance_scan.json", "w"))
