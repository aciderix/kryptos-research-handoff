"""Relay (23/09): Beaufort A-Z keystream of EASTNORTHEAST ends K K K L -> 'key held for a few letters, then steps'.
(1) How unusual is a run of >= 3 identical key letters? Same 12 keystreams (2 cribs x AZ/KA x Vig/Beau/var) on random ciphertexts.
(2) Stepped key, general form: key(i) = a + s * floor((i - phi) / m)  (constant on blocks of m letters, then +s),
    m = 1..13, phi = 0..m-1, s = 1..25, a unknown; Quagmire I, II, III with ANY alphabet(s); Vig/Beau; exact on the 24 crib letters.
    Null: 20 random ciphertexts for every K4-compatible configuration."""
import sys, json, random
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import Model, _alph, _relation, K4, ANCH, AZ, KA, rand_ct
C1 = list(range(21, 34)); C2 = list(range(63, 74))
def streams(ct):
    out = []
    for cr in (C1, C2):
        for A in (AZ, KA):
            for md in ("vig", "beau", "var"):
                ks = []
                for i in cr:
                    c, q = A.index(ct[i]), A.index(ANCH[i])
                    ks.append({"vig": (c - q) % 26, "beau": (c + q) % 26, "var": (q - c) % 26}[md])
                out.append(ks)
    return out
def has_run3(ks): return any(ks[j] == ks[j + 1] == ks[j + 2] for j in range(len(ks) - 2))
k4run = [has_run3(s) for s in streams(K4)]
rnd = random.Random(3); N = 20000
hits = sum(any(has_run3(s) for s in streams("".join(rnd.choice(AZ) for _ in range(97)))) for _ in range(N))
print(f"(1) K4: triplé dans {sum(k4run)} des 12 clés ; textes aléatoires avec au moins un triplé dans l'une des 12 clés : {hits}/{N} = {hits/N:.1%}", flush=True)
def sat(ct, q, md, m, phi, s):
    M = Model(); P, C = _alph(M, q)
    for i, pt in ANCH.items():
        _relation(M, P, C, ct[i], pt, [(1, "a")], md, key_const=s * ((i - phi) // m))
    return M.solve()
res = []
for q in ("III", "II", "I"):
    for md in ("vig", "beau"):
        for m in range(1, 14):
            for phi in range(m):
                for s in range(1, 26):
                    if sat(K4, q, md, m, phi, s):
                        nul = sum(sat(rand_ct(700 + t), q, md, m, phi, s) for t in range(20))
                        res.append(dict(q=q, mode=md, m=m, phi=phi, s=s, null_of_20=nul)); print("   compatible:", res[-1], flush=True)
print("(2) configurations compatibles avec K4 :", len(res), "sur", 3 * 2 * 91 * 25)
json.dump({"K4_run3_in_12_streams": sum(k4run), "random_any_run3_rate": hits / N, "stepped_compatible": res}, open("results.json", "w"), indent=1)
