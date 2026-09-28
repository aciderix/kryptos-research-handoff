"""Null for gromark_keyword_alphabets.py: how many base-10 primers fit when one side is a RANDOM
alphabet (other side free)? Tells whether 0 for Sanborn's alphabets is informative."""
import json, random, sys
from gromark_fixed_side import PAIRS, key, consistent, AZ

KEYS = [key([int(d) for d in f"{n:05d}"]) for n in range(100000)]
rng = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 0)
rows = []
for t in range(int(sys.argv[2]) if len(sys.argv) > 2 else 100):
    A = "".join(rng.sample(AZ, 26))
    side = t % 2
    c = sum(consistent(k, PAIRS, A if side == 0 else None, A if side == 1 else None) for k in KEYS)
    rows.append(c)
print(json.dumps({"trials": len(rows), "alphabets_with_any_primer": sum(r > 0 for r in rows),
                  "total_primers": sum(rows), "counts": rows}))
