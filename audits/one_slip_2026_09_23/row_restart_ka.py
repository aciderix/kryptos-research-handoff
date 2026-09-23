"""Determined check of the row-restart one-slip hits with Sanborn's own alphabets fixed (KA, and AZ).
Quagmire III Beaufort / Vigenere: key value at crib position j = f(ct, pt); key index = col(j) mod p.
Print, per key index, the key letters demanded by the crib positions. Consistent (up to one slip) => key readable."""
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
ANCH = {21 + j: c for j, c in enumerate("EASTNORTHEAST")}
ANCH.update({63 + j: c for j, c in enumerate("BERLINCLOCK")})
ALIGN = {"WS": lambda i: i % 31, "PANEL": lambda i: (i + 27) % 31}
for an, A in (("KA", KA), ("AZ", AZ)):
    for al, idx in ALIGN.items():
        for p in (8, 13):
            for mode in ("beau", "vig"):
                cols = {}
                for j, pt in ANCH.items():
                    c, q = A.index(K4[j]), A.index(pt)
                    k = (c + q) % 26 if mode == "beau" else (c - q) % 26
                    cols.setdefault(idx(j) % p, []).append((j, A[k]))
                bad = sum(len(v) - max(sum(1 for _, x in v if x == y) for _, y in v) for v in cols.values())
                if bad <= 1:
                    print(an, al, p, mode, "INCOHÉRENCES:", bad, {i: [x for _, x in v] for i, v in sorted(cols.items())})
                else:
                    print(an, al, p, mode, "incohérences:", bad)
