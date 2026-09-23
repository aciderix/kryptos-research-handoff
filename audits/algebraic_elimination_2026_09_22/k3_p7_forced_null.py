"""Are the FORCED plaintext letters of the period-7 K3-route cells more English than chance?

Score = mean log English unigram frequency of the forced letters outside the cribs.
Null  = the same score for random ciphertexts that are ALSO compatible with the same cell
        (rejection sampling), analysed with exactly the same code.
"""
import json
import math
import random
import sys

import k3_p7_forced as F
from k3_multiplicative import q3_periodic, triples_A, triples_B
from k4_algebraic import AZ, ANCH

FREQ = dict(zip(AZ, [8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4, 6.7, 7.5,
                     1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074]))


def score(txt):
    letters = [c for j, c in enumerate(txt) if c != "." and j not in ANCH]
    return sum(math.log(FREQ[c] / 100) for c in letters) / len(letters) if letters else None


def with_ct(ct97, fn):
    saved = F.K4
    F.K4 = ct97
    try:
        return fn()
    finally:
        F.K4 = saved


def main(cell_idx, n_null=15):
    model, mult, order, mode = F.CELLS[cell_idx]
    k4_txt, k4_f = F.analyse(model, mult, order, mode)
    r = random.Random(1234 + cell_idx)
    nulls, tried = [], 0
    while len(nulls) < n_null and tried < 5000:
        tried += 1
        ct = "".join(r.choice(AZ) for _ in range(97))
        tr = triples_A("?" + ct, ANCH, mult, order)[0] if model == "A" else triples_B(ct, ANCH, mult, order)
        if not q3_periodic(tr, mode, 7):
            continue
        txt, f = with_ct(ct, lambda: F.analyse(model, mult, order, mode))
        nulls.append({"forced": f, "score": score(txt), "text": txt})
    s_k4 = score(k4_txt)
    better = sum(1 for n in nulls if n["score"] is not None and n["score"] >= s_k4)
    out = {"cell": F.CELLS[cell_idx], "K4": {"forced": k4_f, "score": s_k4, "text": k4_txt},
           "null_n": len(nulls), "null_tried": tried, "null_scores": [n["score"] for n in nulls],
           "null_forced": [n["forced"] for n in nulls],
           "fraction_null_at_least_as_english": better / len(nulls) if nulls else None}
    json.dump(out, open(f"results_k3_p7_forced_null_{cell_idx}.json", "w"), indent=1)
    print(json.dumps({k: v for k, v in out.items() if k != "null_scores"}), flush=True)


if __name__ == "__main__":
    main(int(sys.argv[1]))
