"""Positive control for wheatstone_exact.py: encipher a random plaintext carrying the cribs with the device
(random inner alphabet, outer AZ or KA, clockwise, repeat = full turn) and check the test accepts it."""
import random
from wheatstone_exact import AZ, KA, CRIBS, positions


def encipher(pt, outer, inner, sign=1, rep="full"):
    ring = outer + " "
    out, small, prev = [], 0, " "
    for ch in pt:
        s = (sign * (ring.index(ch) - ring.index(prev))) % 27
        if s == 0:
            s = 27 if rep == "full" else 0
        small = (small + s) % 26
        out.append(inner[small]); prev = ch
    return "".join(out)


def accepted(ct, outer, sign=1, rep="full"):
    rels = [positions(w, outer, sign, rep) for _, w in CRIBS]
    for d in range(26):
        inner, back, good = {}, {}, True
        for (st, w), rel, off in zip(CRIBS, rels, (0, d)):
            for j, r in enumerate(rel):
                x, c = (r + off) % 26, ct[st + j]
                if inner.setdefault(x, c) != c or back.setdefault(c, x) != x:
                    good = False; break
            if not good: break
        if good: return True
    return False


ok = 0
for seed in range(20):
    r = random.Random(seed)
    pt = [r.choice(AZ) for _ in range(97)]
    for st, w in CRIBS: pt[st:st + len(w)] = w
    outer = AZ if seed % 2 else KA
    ct = encipher("".join(pt), outer, "".join(r.sample(AZ, 26)))
    ok += accepted(ct, outer)
print(f"contrôle positif : {ok}/20 chiffrés de Wheatstone reconnus")
