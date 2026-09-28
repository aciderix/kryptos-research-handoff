"""Gromark with DATE primers (Gemini relay, 23/09).  Primers fixed in advance:
1986, 1989, 8689, 8986, 19861, 19891, 19869, 19898, 861989, 891986, 19861989, 19891986.
Key: digits of primer, then k[i] = k[i-n] + k[i-n+1] mod 10 (standard Gromark chain).
Alphabets: Quagmire I, II, III, IV (all unknown alphabets FREE: any of 26!), Vig/Beau.
Tests: exact (24/24) and one error tolerated (23/24: each anchor dropped in turn).
Null: 50 random ciphertexts per cell (exact)."""
import sys, os, json
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "algebraic_elimination_2026_09_22"))
from k4_algebraic import Model, _alph, _relation, K4, ANCH, rand_ct

PRIMERS = ["1986","1989","8689","8986","19861","19891","19869","19898",
           "861989","891986","19861989","19891986"]

def chain(primer, n=97):
    k = [int(c) for c in primer]
    while len(k) < n:
        k.append((k[-len(primer)] + k[-len(primer)+1]) % 10)
    return k

def sat(ct, primer, mode, q, anch=ANCH):
    k = chain(primer)
    m = Model(); P, C = _alph(m, q)
    for i, pt in anch.items():
        _relation(m, P, C, ct[i], pt, [], mode, key_const=k[i])
    return m.solve()

out = []
for q in ("I","II","III","IV"):
    for mode in ("vig","beau"):
        for pr in PRIMERS:
            ex = sat(K4, pr, mode, q)
            slip = [i for i in ANCH if sat(K4, pr, mode, q, {j:c for j,c in ANCH.items() if j != i})]
            nul = sum(sat(rand_ct(3000+s), pr, mode, q) for s in range(50))
            r = {"q":q,"mode":mode,"primer":pr,"exact":ex,"one_slip_positions":slip,"null_exact":nul}
            out.append(r); print(json.dumps(r), flush=True)
json.dump(out, open("results.json","w"), indent=1)
