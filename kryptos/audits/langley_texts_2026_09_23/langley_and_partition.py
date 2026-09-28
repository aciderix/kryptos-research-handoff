"""(1) Inscriptions at CIA headquarters as running key (repeated to 97, every phase):
    motto (John 8:32, lobby of the Original HQ Building), with/without 'JOHN VIII XXXII'; Memorial Wall inscription.
    (a) fixed alphabets: P/C in {AZ, KA}, key letter read in the plaintext alphabet, Vig/Beau/var -> crib matches /24;
    (b) Quagmire III ANY alphabet, key letter read in the same alphabet (K1/K2 style), Vig/Beau, exact.
(2) Partitioned substitution (Materna phenomenon): key(i) = k[class(pt_i)][i mod p], class = pt letter in KRYPTOS or not;
    Quagmire III any alphabet, p = 1..26, Vig/Beau, exact; null 20 random ciphertexts where K4 is compatible."""
import sys, json, random
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import Model, _alph, _relation, runkey, K4, ANCH, AZ, KA, rand_ct
TEXTS = {"motto": "ANDYESHALLKNOWTHETRUTHANDTHETRUTHSHALLMAKEYOUFREE",
         "motto_john": "ANDYESHALLKNOWTHETRUTHANDTHETRUTHSHALLMAKEYOUFREEJOHNVIIIXXXII",
         "memorial_wall": "INHONOROFTHOSEMEMBERSOFTHECENTRALINTELLIGENCEAGENCYWHOGAVETHEIRLIVESINTHESERVICEOFTHEIRCOUNTRY"}
out = {}
for name, T in TEXTS.items():
    best = (0, None)
    for ph in range(len(T)):
        R = (T * 5)[ph:ph + 200]
        for pa in (AZ, KA):
            for ca in (AZ, KA):
                for mode in ("vig", "beau", "var"):
                    sc = 0
                    for i, pt in ANCH.items():
                        c, p, k = ca.index(K4[i]), pa.index(pt), pa.index(R[i])
                        sc += {"vig": (p + k) % 26 == c, "beau": (k - p) % 26 == c, "var": (p - k) % 26 == c}[mode]
                    if sc > best[0]: best = (sc, (ph, "AZ" if pa == AZ else "KA", "AZ" if ca == AZ else "KA", mode))
    anyalpha = [(ph, md) for ph in range(len(T)) for md in ("vig", "beau") if runkey(K4, ANCH, md, (T * 5)[ph:ph + 200], 0)]
    out[name] = {"fixed_best": best, "anyalphabet_QIII_compatible": anyalpha}
    print(name, "fixed alphabets best:", best, "| any alphabet QIII compatible (phase, mode):", anyalpha, flush=True)
# (2) partitioned substitution
KS = set("KRYPTOS")
def part(ct, mode, p):
    m = Model(); P, C = _alph(m, "III")
    for i, pt in ANCH.items():
        _relation(m, P, C, ct[i], pt, [(1, f"k{'K' if pt in KS else 'O'}{i % p}")], mode)
    return m.solve()
res = []
for mode in ("vig", "beau"):
    for p in range(1, 27):
        k4 = part(K4, mode, p)
        nul = sum(part(rand_ct(3300 + s), mode, p) for s in range(20)) if k4 else None
        res.append({"mode": mode, "p": p, "K4": k4, "null_of_20": nul})
        if k4: print("partition", mode, "p", p, "K4 compatible, null", nul, "/20", flush=True)
print("partition: K4 compatible periods:", [(r["mode"], r["p"]) for r in res if r["K4"]])
out["partition"] = res
json.dump(out, open("results.json", "w"), indent=1)
