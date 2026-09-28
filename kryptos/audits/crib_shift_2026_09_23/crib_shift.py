"""Piste 4 (relais Gemini, 23/09): a letter skipped/added between the cribs on the copper.
The key index of BERLINCLOCK is shifted by s = -3..+3 relative to EASTNORTHEAST
(i.e. key index = i + s for i >= 63).  Quagmire I-IV, ANY alphabet(s), periodic p = 1..26,
Vig/Beau, exact.  Null: 20 random ciphertexts per cell (same shift)."""
import sys, json
from multiprocessing import Pool
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import periodic, K4, ANCH, rand_ct

def job(a):
    q, mode, s, p = a
    idx = lambda i: i + s if i >= 63 else i
    k4 = periodic(K4, ANCH, mode, p, q, idx)
    nul = sum(periodic(rand_ct(700 + t), ANCH, mode, p, q, idx) for t in range(20))
    return {"q": q, "mode": mode, "shift": s, "p": p, "K4": k4, "null": nul}

if __name__ == "__main__":
    jobs = [(q, m, s, p) for q in ("I", "II", "III", "IV") for m in ("vig", "beau")
            for s in (-3, -2, -1, 0, 1, 2, 3) for p in range(1, 27)]
    with Pool(4) as pool: R = pool.map(job, jobs)
    json.dump(R, open("results.json", "w"), indent=1)
    for q in ("I", "II", "III", "IV"):
        for m in ("vig", "beau"):
            for s in (-3, -2, -1, 0, 1, 2, 3):
                rs = [r for r in R if (r["q"], r["mode"], r["shift"]) == (q, m, s)]
                sev = [r["p"] for r in rs if r["K4"] and r["null"] <= 1]
                print(f"Q{q} {m} s={s:+d} K4 SAT p={[r['p'] for r in rs if r['K4']]}  severe(null<=1/20)={sev}")
