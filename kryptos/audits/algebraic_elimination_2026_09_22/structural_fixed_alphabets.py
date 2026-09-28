"""Determined structural tests on the 24 anchors with the FIXED alphabets AZ and KA.

No key search: each test asks whether a key GENERATOR can produce the 24 implied
key values, for the three additive conventions (Vigenere, Beaufort, variant
Beaufort) in the standard (AZ) and KRYPTOS (KA) alphabets.

Run: python3 structural_fixed_alphabets.py
"""
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
CT = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
PT = {s + j: c for s, w in {21: "EASTNORTHEAST", 63: "BERLINCLOCK"}.items() for j, c in enumerate(w)}
POS = sorted(PT)
CONV = [(an, a, m) for an, a in (("AZ", AZ), ("KA", KA)) for m in ("vig", "beau", "vbeau")]


def keystream(alpha, mode):
    out = {}
    for i in POS:
        c, p = alpha.index(CT[i]), alpha.index(PT[i])
        out[i] = {"vig": (c - p) % 26, "beau": (c + p) % 26, "vbeau": (p - c) % 26}[mode]
    return out


def consistent_rowcol(k, w):
    """key = a[row] + b[col] ; union-find with potentials mod 26. Returns (ok, n_cycles)."""
    R = max(POS) // w + 1
    par, pot = list(range(R + w)), [0] * (R + w)

    def find(x):
        if par[x] == x:
            return x, 0
        r, p = find(par[x])
        par[x] = r
        pot[x] = (pot[x] + p) % 26
        return r, pot[x]
    cycles = 0
    for i in POS:
        u, v, d = i // w, R + i % w, k[i]          # a_r - (-b_c) = k
        ru, pu = find(u)
        rv, pv = find(v)
        if ru == rv:
            if (pu - pv) % 26 != d:
                return False, cycles
            cycles += 1
        else:
            par[ru] = rv
            pot[ru] = (d - pu + pv) % 26
    return True, cycles


def main():
    union = set("KRYPTOS") | set("PALIMPSEST") | set("ABSCISSA")
    for an, a, m in CONV:
        k = keystream(a, m)
        name = f"{an}-{m}"
        print(f"\n== {name}: key at anchors = "
              f"{''.join(a[k[i]] for i in POS[:13])} / {''.join(a[k[i]] for i in POS[13:])}")
        rc = [(w, n) for w in range(2, 49) for ok, n in [consistent_rowcol(k, w)] if ok and n > 0]
        print("  T1 row+col key, widths 2-48, informative & consistent:", rc)
        ak = []
        for L in range(1, 97):
            for src in ("CT", "PT"):
                n, bad = 0, False
                for i in POS:
                    j = i - L
                    if j < 0:
                        continue
                    if src == "CT":
                        s = a.index(CT[j])
                    elif j in PT:
                        s = a.index(PT[j])
                    else:
                        continue
                    n += 1
                    if s != k[i]:
                        bad = True
                        break
                if not bad and n >= 4:
                    ak.append((src, L, n))
        print("  T2 autokey any lag (>=4 checks) survivors:", ak)
        pg = []
        for p in range(2, 49):
            for s in range(26):
                seen, n, bad = {}, 0, False
                for i in POS:
                    v, r = (k[i] - s * (i // p)) % 26, i % p
                    if r in seen:
                        n += 1
                        if seen[r] != v:
                            bad = True
                            break
                    else:
                        seen[r] = v
                if not bad and n >= 3:
                    pg.append((p, s, n))
        print("  T3 progressive key p<=48, any step (>=3 checks) survivors:", pg)
        blocks = ([i for i in POS if i < 50], [i for i in POS if i > 50])
        ic = [p for p in range(1, 13)
              if all(k[i] == k[i + p] for b in blocks for i in b if i + p in b)]
        print("  T4 periodic p<=12 checked inside each crib only (null/omission-robust):", ic)
        print(f"  T5 distinct key rows among 24 anchors: {len(set(k.values()))}; "
              f"anchors whose row is outside KRYPTOS|PALIMPSEST|ABSCISSA letters: "
              f"{sum(1 for i in POS if a[k[i]] not in union)}/24")


if __name__ == "__main__":
    main()
