"""Row+column key, Beaufort (completes the registry entry left 'not completed').
key(i) = a[i // w] + b[i % w]  (unknown a, b), Quagmire III with ANY alphabet, exact on the 24 crib letters.
Usage: python3 one_width.py W  -> prints one JSON line (K4 result; null on 10 random ciphertexts only if K4 is SAT)."""
import sys, json, time
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import rowcol, K4, ANCH, rand_ct
w = int(sys.argv[1]); t = time.time()
k4 = rowcol(K4, ANCH, "beau", w)
out = {"w": w, "K4": k4, "sec_K4": round(time.time() - t, 1)}
if k4:
    out["null_sat"] = sum(bool(rowcol(rand_ct(6000 + s), ANCH, "beau", w)) for s in range(10)); out["n_null"] = 10
print(json.dumps(out), flush=True)
