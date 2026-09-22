"""Positive control for qmark98.py: a synthetic 98-symbol ciphertext built WITH each surviving
grid (null '?' at plaintext start) + Quagmire III (random alphabet, period-10 key) must be SAT
at p=10 and UNSAT at p=7."""
import json
import random
from k4_algebraic import ANCH, AZ, trans_periodic
from qmark98_lib import survivors

res = {}
for c in survivors():
    for order in ("TS", "ST"):
        for mode in ("vig", "beau"):
            r = random.Random(77)
            Pa = r.sample(range(26), 26)
            inv = {v: k for k, v in enumerate(Pa)}
            key = [r.randrange(26) for _ in range(10)]
            pt = [r.choice(AZ) for _ in range(97)]
            for i, ch in ANCH.items():
                pt[i] = ch
            pt98 = ["?"] + pt
            sg = 1 if mode == "vig" else -1

            def sub(ch, j):
                return ch if ch == "?" else AZ[inv[(sg * Pa[AZ.index(ch)] + key[j % 10]) % 26]]
            perm = c["perm"]
            if order == "TS":
                mid = [pt98[perm[t]] for t in range(98)]
                ct = "".join(sub(mid[t], t) for t in range(98))
            else:
                s = [sub(pt98[i], i) for i in range(98)]
                ct = "".join(s[perm[t]] for t in range(98))
            assert ct[0] == "?"
            anch = {i + 1: ch for i, ch in ANCH.items()}
            tag = f"{c['keyed']}-{c['direction']}-{order}-{mode}"
            res[tag] = {"p10": trans_periodic(ct, anch, mode, 10, perm, order),
                        "p7": trans_periodic(ct, anch, mode, 7, perm, order)}
            print(tag, res[tag], flush=True)
json.dump(res, open("results_qmark98_positive_control.json", "w"), indent=1)
