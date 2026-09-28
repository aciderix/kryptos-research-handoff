"""Gromark (ACA) with Sanborn's KRYPTOS alphabet fixed on one side, all base-10 five-digit primers.

Pre-registered (23/09, before running): family = ACA Gromark, key k_0..k_96 from a 5-digit primer
by k_i = k_{i-5} + k_{i-4} mod 10 (Hall 1969 / Bean 2021, gt.c), encryption idx_C(ct) = idx_P(pt) + k (mod 26).
Four side conventions, each with ONE alphabet fixed from the sculpture and the other left free:
  P=KA / C free, P free / C=KA, P=AZ / C free (standard ACA), P free / C=AZ.
Also both fixed: (KA,KA), (KA,AZ), (AZ,KA), (AZ,AZ).
Criterion: consistency with the 24 crib letters (0-indexed 21-33 EASTNORTHEAST, 63-73 BERLINCLOCK).
With one side fixed, the other alphabet's indices are forced; consistent = no letter gets two indices
and no index gets two letters. Positive control: a synthetic K4 built with a known primer is recovered.
"""
import json, random, sys

CT = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIBS = {21: "EASTNORTHEAST", 63: "BERLINCLOCK"}
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
PAIRS = [(i + j, p, CT[i + j]) for i, w in CRIBS.items() for j, p in enumerate(w)]


def key(primer, n=97, base=10):
    k = list(primer)
    while len(k) < n:
        k.append((k[-5] + k[-4]) % base)
    return k


def consistent(k, pairs, P=None, C=None):
    """P/C: fixed alphabet string or None (free). Returns True if some bijection fits."""
    if P and C:
        return all((P.index(p) + k[i]) % 26 == C.index(c) for i, p, c in pairs)
    if P:   # forced cipher index for each ciphertext letter
        fwd, back = {}, {}
        for i, p, c in pairs:
            x = (P.index(p) + k[i]) % 26
            if fwd.setdefault(c, x) != x or back.setdefault(x, c) != c:
                return False
        return True
    if C:   # forced plain index for each plaintext letter
        fwd, back = {}, {}
        for i, p, c in pairs:
            x = (C.index(c) - k[i]) % 26
            if fwd.setdefault(p, x) != x or back.setdefault(x, p) != p:
                return False
        return True
    raise ValueError


CONVS = {"P=KA,C=free": (KA, None), "P=free,C=KA": (None, KA), "P=AZ,C=free": (AZ, None),
         "P=free,C=AZ": (None, AZ), "P=KA,C=KA": (KA, KA), "P=KA,C=AZ": (KA, AZ),
         "P=AZ,C=KA": (AZ, KA), "P=AZ,C=AZ": (AZ, AZ)}


def scan(pairs):
    out = {name: [] for name in CONVS}
    for n in range(100000):
        pr = [int(d) for d in f"{n:05d}"]
        k = key(pr)
        for name, (P, C) in CONVS.items():
            if consistent(k, pairs, P, C):
                out[name].append(f"{n:05d}")
    return out


def positive_control(seed):
    rng = random.Random(seed)
    pr = [rng.randrange(10) for _ in range(5)]
    k = key(pr)
    pt = [rng.choice(AZ) for _ in range(97)]
    for i, w in CRIBS.items():
        pt[i:i + len(w)] = w
    C = "".join(rng.sample(AZ, 26))
    ct = "".join(C[(KA.index(p) + k[i]) % 26] for i, p in enumerate(pt))
    pairs = [(i + j, p, ct[i + j]) for i, w in CRIBS.items() for j, p in enumerate(w)]
    ok = consistent(k, pairs, KA, None)
    return "".join(map(str, pr)), ok


if __name__ == "__main__":
    ctrl = [positive_control(s) for s in range(5)]
    res = scan(PAIRS)
    summary = {name: {"count": len(v), "primers": v[:50]} for name, v in res.items()}
    bean = open(sys.argv[1]).read().split() if len(sys.argv) > 1 else []
    bean39 = sorted({l for l in bean if len(l) == 5 and l.isdigit()})
    out = {"positive_controls_P=KA_C=free": ctrl, "results": summary, "bean_gt_10_5_primers": bean39}
    print(json.dumps(out, indent=1))
