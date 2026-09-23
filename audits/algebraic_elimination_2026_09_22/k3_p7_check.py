"""Follow-up of the four K3-route cells that are SAT for K4 at period 7 with null rate 0/30.
(1) high-resolution null (300 random ciphertexts) for each of those cells;
(2) look-elsewhere: over ALL 408 configurations at p = 7, K4 SAT count vs expected count
    = sum of per-configuration null rates (20 random ciphertexts each)."""
import json, sys, time
from math import gcd
from k3_multiplicative import group_mod, q3_periodic, rand_ct, triples_A, triples_B
from k4_algebraic import ANCH, K4

def tr_for(model, mult, order, ct97):
    return triples_A("?" + ct97, ANCH, mult, order)[0] if model == "A" else triples_B(ct97, ANCH, mult, order)

part = sys.argv[1]
if part == "cells":
    cells = [("A", 91, "ST", "vig"), ("B", 81, "TS", "vig"), ("B", 81, "ST", "vig"), ("A", 85, "ST", "vig"),
             ("A", 58, "ST", "vig")]
    out = {}
    for model, mult, order, mode in cells:
        k4 = q3_periodic(tr_for(model, mult, order, K4), mode, 7)
        h = sum(q3_periodic(tr_for(model, mult, order, rand_ct(90000 + s)), mode, 7) for s in range(300))
        out[f"{model}-m{mult}-{order}-{mode}-p7"] = {"K4": k4, "null": f"{h}/300"}
        print(out, flush=True)
    json.dump(out, open("results_k3_p7_cells.json", "w"), indent=1)
else:
    shard, nsh = map(int, part.split("/"))
    GA = group_mod([2, 7], 99); MB = [m for m in range(1, 98) if gcd(m, 98) == 1]
    confs = [(md, m, o, sn) for md, ms in (("A", GA), ("B", MB)) for m in ms for o in ("TS", "ST")
             for sn in ("vig", "beau")][shard::nsh]
    k4hits = 0; exp = 0.0; rows = []
    for c in confs:
        k = bool(q3_periodic(tr_for(c[0], c[1], c[2], K4), c[3], 7))
        r = sum(q3_periodic(tr_for(c[0], c[1], c[2], rand_ct(70000 + s)), c[3], 7) for s in range(20)) / 20
        k4hits += k; exp += r; rows.append([*c, k, r])
    out = {"n": len(confs), "K4_hits_p7": k4hits, "expected_p7": exp, "rows": rows}
    json.dump(out, open(f"results_k3_p7_family_{shard}of{nsh}.json", "w"), indent=1)
    print(shard, out["n"], k4hits, round(exp, 2), flush=True)
