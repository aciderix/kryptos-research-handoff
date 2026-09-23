"""Morse 'mirror' (reverse the dots/dashes of each letter) applied to K4 before decryption.
Mirror: A<->N B<->V D<->U F<->L G<->W Q<->Y; palindromic letters fixed; C,J,Z have no mirror
-> variant (a) left unchanged, variant (b) the crib position 70 (Z) dropped.
Applied to: ciphertext only, plaintext only, both.  Then periodic Quagmire III (same unknown alphabet
both sides, the K1/K2 system) and Quagmire I-IV any alphabet, p = 1..26, Vig/Beau, exact.
Null: 20 random ciphertexts per cell (same transform)."""
import sys, json
from multiprocessing import Pool
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import periodic, K4, ANCH, rand_ct
MIR = {"A":"N","N":"A","B":"V","V":"B","D":"U","U":"D","F":"L","L":"F","G":"W","W":"G","Q":"Y","Y":"Q"}
m = lambda s: "".join(MIR.get(c, c) for c in s)
def job(a):
    side, var, q, mode, p = a
    anch = {i: (m(c) if side in ("pt", "both") else c) for i, c in ANCH.items() if not (var == "b" and i == 70)}
    tr = (lambda s: m(s)) if side in ("ct", "both") else (lambda s: s)
    k4 = periodic(tr(K4), anch, mode, p, q)
    nul = sum(periodic(tr(rand_ct(800 + t)), anch, mode, p, q) for t in range(20))
    return dict(side=side, var=var, q=q, mode=mode, p=p, K4=k4, null=nul)
if __name__ == "__main__":
    jobs = [(s, v, q, md, p) for s in ("ct", "pt", "both") for v in ("a", "b") for q in ("III", "I", "II", "IV")
            for md in ("vig", "beau") for p in range(1, 27)]
    with Pool(4) as pool: R = pool.map(job, jobs)
    json.dump(R, open("results.json", "w"), indent=1)
    for s in ("ct", "pt", "both"):
        for v in ("a", "b"):
            for q in ("III", "I", "II", "IV"):
                for md in ("vig", "beau"):
                    rs = [r for r in R if (r["side"], r["var"], r["q"], r["mode"]) == (s, v, q, md)]
                    print(f"{s:4} {v} Q{q:3} {md}: K4 SAT p={[r['p'] for r in rs if r['K4']]} severe={[r['p'] for r in rs if r['K4'] and r['null'] <= 1]}")
    sev = [r for r in R if r["null"] <= 1]
    print("severe cells:", len(sev), " K4 SAT among them:", sum(r["K4"] for r in sev), " expected by chance:", round(sum(r["null"] / 20 for r in sev), 1))
