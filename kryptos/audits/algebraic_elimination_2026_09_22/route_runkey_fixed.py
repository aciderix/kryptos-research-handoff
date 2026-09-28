"""K3-style routes x running key taken from the sculpture's own texts, FIXED alphabets (AZ, KA).

Pure arithmetic (no solver). For every route (identity, the 102 K3 multiplicative routes of
k3_multiplicative.py, the ragged 7/14/21 routes of ragged_widths.py), layer order, alphabet,
additive sense and source text, count at the best offset how many of the 24 anchors are
reproduced. A real mechanism reproduces 24/24; the null distribution (random ciphertexts through
the identical pipeline) shows what "best offset" reaches by chance.

Texts: K1/K2/K3 plaintexts, K1 and K3 ciphertexts DERIVED from their plaintexts with the published
methods (K1: Quagmire III KA/PALIMPSEST; K3: the verified double route), the three concatenated.
K2 ciphertext is not used (the carved K2 differs from a clean re-encryption; not reconstructed here).
"""
import json
import random
from math import gcd

import numpy as np

from k3_multiplicative import group_mod, k3_route
from k4_algebraic import AZ, ANCH, K1PT, K2PT, K3PT, K4, KA
from ragged_widths import configs as ragged_configs

K1CT = "".join(KA[(KA.index(c) + KA.index("PALIMPSEST"[i % 10])) % 26] for i, c in enumerate(K1PT))
K3CT = "".join(k3_route(k3_route(list(K3PT), 42), 14))
TEXTS = {"K1PT": K1PT, "K2PT": K2PT, "K3PT": K3PT, "K1CT": K1CT, "K3CT": K3CT,
         "K123PT": K1PT + K2PT + K3PT}


def layouts(ct97):
    """yield name, list of (anchor PT index j, CT letter, key index TS, key index ST)."""
    J = sorted(ANCH)
    yield "identity", [(j, ct97[j], j, j) for j in J]
    for m in group_mod([2, 7], 99):
        inv = pow(m, -1, 99)
        src = [(inv * (t + 1)) % 99 - 1 for t in range(98)]
        q = src[0]
        pos = {i: t for t, i in enumerate(src)}
        ct98 = "?" + ct97
        yield f"A-m{m}", [(j, ct98[pos[j if j < q else j + 1]], pos[j if j < q else j + 1] - 1, j) for j in J]
    for m in [m for m in range(1, 98) if gcd(m, 98) == 1]:
        inv = pow(m, -1, 98)
        src = [(inv * (t + 1)) % 98 - 1 for t in range(97)]
        pos = {i: t for t, i in enumerate(src)}
        yield f"B-m{m}", [(j, ct97[pos[j]], pos[j], j) for j in J]
    for meta, perm in ragged_configs():
        pos = {s: t for t, s in enumerate(perm)}
        name = "R-w{width}-k{keyed}-bu{bottom_up}-{direction}".format(**meta)
        yield name, [(j, ct97[pos[j]], pos[j], j) for j in J]


def best(ct97):
    res = []
    for name, lay in layouts(ct97):
        for an, A in (("AZ", AZ), ("KA", KA)):
            ix = {c: i for i, c in enumerate(A)}
            c = np.array([ix[x[1]] for x in lay]); p = np.array([ix[ANCH[x[0]]] for x in lay])
            need = {"vig": (c - p) % 26, "beau": (c + p) % 26, "vbeau": (p - c) % 26}
            for order, col in (("TS", 2), ("ST", 3)):
                kidx = np.array([x[col] for x in lay])
                for tn, T in TEXTS.items():
                    R = np.array([ix[x] for x in T])
                    offs = np.arange(-kidx.min(), len(T) - kidx.max())
                    if len(offs) == 0:          # text too short for this key span
                        continue
                    G = R[kidx[None, :] + offs[:, None]]          # offsets x 24
                    for mode, nd in need.items():
                        sc = (G == nd[None, :]).sum(1)
                        k = int(sc.argmax())
                        res.append((int(sc[k]), name, an, order, tn, mode, int(offs[k])))
    return res


def positive_control():
    """Synthetic K4: route B-m5, key = K3PT from offset 40 along the plaintext (ST), KA Vigenere."""
    r = random.Random(9)
    pt = [r.choice(AZ) for _ in range(97)]
    for j, ch in ANCH.items():
        pt[j] = ch
    sub = [KA[(KA.index(pt[j]) + KA.index(K3PT[j + 40])) % 26] for j in range(97)]
    ct = [None] * 97
    for j in range(97):
        ct[(5 * (j + 1)) % 98 - 1] = sub[j]
    top = max(best("".join(ct)))
    return top


def main():
    assert K3CT.startswith("ENDYAHROHNLSRHEOCPTEOIBI")
    pc = positive_control()
    print("positive control (want 24, B-m5, KA, ST, K3PT, vig, offset 40):", pc, flush=True)
    assert pc[0] == 24
    k4 = best(K4)
    k4.sort(reverse=True)
    null_max = []
    for s in range(20):
        r = random.Random(500 + s)
        null_max.append(max(x[0] for x in best("".join(r.choice(AZ) for _ in range(97)))))
    out = {"n_cells": len(k4), "K4_top10": k4[:10], "K4_max": k4[0][0],
           "null_max_over_family": null_max,
           "K4_count_24": sum(1 for x in k4 if x[0] == 24)}
    json.dump(out, open("results_route_runkey_fixed.json", "w"), indent=1)
    print(json.dumps(out, indent=1))


if __name__ == "__main__":
    main()
