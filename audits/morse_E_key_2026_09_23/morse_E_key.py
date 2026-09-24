"""Rule B (Gemini relay, 23/09): the sizes of the isolated-E groups of the Morse (K0) as a numeric key.
Groups (base 1, Rumkin transcription): SHADOW 2 before/2 after; FORCES 5 after; LUCID 3 after;
MEMORY 1 after; POSITION 1 after; DIGETAL 1 before/3 after; VIRTUALLY 2 before/1 attached; INVISIBLE 5 before (total 26).
Orders (fixed): Rumkin reading order, physical order (base 1), and both reversed.
Key: periodic (period 11), every phase; also key+1 variant is absorbed by the free alphabet.
Quagmire I, II, IV (III is already eliminated at p=11 for any key), any alphabets, Vig/Beau, exact.
Null: 50 random ciphertexts."""
import sys, json
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import Model, _alph, _relation, K4, ANCH, rand_ct
ORDERS = {"rumkin": [2, 2, 5, 3, 1, 1, 1, 3, 2, 1, 5],
          "physical": [2, 1, 5, 1, 3, 2, 2, 5, 3, 1, 1]}
ORDERS.update({k + "_rev": v[::-1] for k, v in list(ORDERS.items())})
assert all(sum(v) == 26 for v in ORDERS.values())
def sat(ct, key, ph, q, mode):
    m = Model(); P, C = _alph(m, q)
    for i, pt in ANCH.items():
        _relation(m, P, C, ct[i], pt, [], mode, key_const=key[(i + ph) % len(key)])
    return m.solve()
out = []
for name, key in ORDERS.items():
    for q in ("I", "II", "IV"):
        for mode in ("vig", "beau"):
            k4 = [ph for ph in range(11) if sat(K4, key, ph, q, mode)]
            nul = sum(any(sat(rand_ct(900 + t), key, ph, q, mode) for ph in range(11)) for t in range(50))
            out.append(dict(order=name, q=q, mode=mode, K4_phases=k4, null_any_phase=nul))
            print(name, q, mode, "K4 phases SAT:", k4, " null (any phase):", nul, "/50", flush=True)
json.dump(out, open("results.json", "w"), indent=1)
