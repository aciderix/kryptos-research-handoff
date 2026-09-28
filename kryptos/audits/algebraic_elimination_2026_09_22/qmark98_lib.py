"""Hypothesis Q98: the carved '?' between K3 and K4 belongs to K4 (K3's plaintext already
encodes its own question mark as the final 'Q'), making K4 a 98-symbol block = 14 x 7.

Closed candidate set (K3 vocabulary, no free parameter): 7-column grid (14 rows),
row-major fill, columns read in KRYPTOS-keyed order or plain order, top-down or
bottom-up, used as encryption (fwd) or its inverse (inv); the '?' is a null placed at
the start or at the end of the 98-symbol plaintext.
FREE VALIDATION: a candidate is kept only if the null lands at carved position 0,
where the '?' actually is. Survivors are then tested algebraically against Quagmire III
with an UNKNOWN alphabet and periodic key p = 1..26, both layer orders.

Run: python3 qmark98.py
"""
import json
import random
from k4_algebraic import K4, ANCH, AZ, trans_periodic

CT98 = "?" + K4


def columnar(n, w, keyed, bottom_up):
    order = sorted(range(w), key=lambda c: ("KRYPTOS"[c], c)) if keyed else list(range(w))
    perm = []
    for c in order:
        col = list(range(c, n, w))
        perm += col[::-1] if bottom_up else col
    return perm                     # out[t] = in[perm[t]]


def invert(perm):
    inv = [0] * len(perm)
    for t, s in enumerate(perm):
        inv[s] = t
    return inv


cands = []
for keyed in (True, False):
    for bu in (False, True):
        base = columnar(98, 7, keyed, bu)
        for dname, perm in (("fwd", base), ("inv", invert(base))):
            for null in ("start", "end"):
                nidx = 0 if null == "start" else 97
                lands = perm.index(nidx)
                cands.append(dict(keyed=keyed, bottom_up=bu, direction=dname, null=null,
                                  null_lands_at=lands, perm=perm))
surv = [c for c in cands if c["null_lands_at"] == 0]
_msg=(f"{len(cands)} candidates, {len(surv)} put the null at carved position 0:")
for c in surv:
    pass



def survivors():
    return surv
