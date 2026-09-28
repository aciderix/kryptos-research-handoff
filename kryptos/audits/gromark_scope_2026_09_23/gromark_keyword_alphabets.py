"""Follow-up (pre-registered 23/09): Gromark base 10, 5-digit primers, with ONE side fixed to a
keyword-mixed alphabet built from Sanborn's only documented keywords (KRYPTOS, PALIMPSEST, ABSCISSA)
or the standard alphabet; the other side free. Keyword alphabet = keyword letters (duplicates dropped)
then the rest of A-Z in order, and its reverse. Same criterion as gromark_fixed_side.py.
"""
import json
from gromark_fixed_side import PAIRS, key, consistent, AZ


def kw(word):
    s = []
    for ch in word + AZ:
        if ch not in s:
            s.append(ch)
    return "".join(s)


ALPHAS = {}
for w in ["KRYPTOS", "PALIMPSEST", "ABSCISSA"]:
    ALPHAS[w] = kw(w)
    ALPHAS[w + "_rev"] = kw(w)[::-1]
ALPHAS["AZ"] = AZ
ALPHAS["AZ_rev"] = AZ[::-1]

res = {}
for name, A in ALPHAS.items():
    for side in ("P", "C"):
        hits = [f"{n:05d}" for n in range(100000)
                if consistent(key([int(d) for d in f"{n:05d}"]), PAIRS, A if side == "P" else None,
                              A if side == "C" else None)]
        res[f"{side}={name}"] = {"alphabet": A, "count": len(hits), "primers": hits[:50]}
print(json.dumps(res, indent=1))
