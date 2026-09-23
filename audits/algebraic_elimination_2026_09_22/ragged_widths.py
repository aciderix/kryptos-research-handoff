"""Ragged-grid column routes on the 97 letters (widths 7, 14, 21: NSA 1992 remark and Bean's
width-21 anomaly), not covered by the full-grid multiplicative family.

Closed list: width in {7, 14, 21}; column order natural (and KRYPTOS-keyed for width 7);
each column read bottom->top (K3) or top->bottom; used as encryption or its inverse;
substitution before or after; Quagmire III, unknown alphabet, periodic key p = 1..26.
Null: 10 random ciphertexts per configuration, same pipeline. Positive control included.
"""
import json
import random
import sys

from k4_algebraic import AZ, ANCH, K4, trans_periodic, rand_ct, synth


def route(n, w, bottom_up, keyed):
    rows = (n + w - 1) // w
    order = sorted(range(w), key=lambda c: ("KRYPTOS"[c], c)) if keyed else list(range(w))
    out = []
    for c in order:
        col = [r * w + c for r in range(rows) if r * w + c < n]
        out += col[::-1] if bottom_up else col
    return out          # out[t] = in[perm[t]]


def invert(p):
    q = [0] * len(p)
    for t, s in enumerate(p):
        q[s] = t
    return q


def configs():
    for w in (7, 14, 21):
        for keyed in ((False, True) if w == 7 else (False,)):
            for bu in (True, False):
                base = route(97, w, bu, keyed)
                for d, perm in (("fwd", base), ("inv", invert(base))):
                    yield dict(width=w, keyed=keyed, bottom_up=bu, direction=d), perm


def main():
    rk = random.Random(1)
    kw = [rk.randrange(26) for _ in range(10)]
    pc = {}
    for meta, perm in list(configs())[:4]:
        for order in ("TS", "ST"):
            ct = synth(21, lambda j, Pa, pt: kw[j % 10], "vig", perm=perm, order=order)
            pc[f"w{meta['width']}-{meta['direction']}-bu{meta['bottom_up']}-{order}"] = [
                trans_periodic(ct, ANCH, "vig", 10, perm, order), trans_periodic(ct, ANCH, "vig", 7, perm, order)]
    print("positive controls [p10, p7] (want [True, False]):", pc, flush=True)
    shard, nsh = (int(sys.argv[1]), int(sys.argv[2])) if len(sys.argv) > 2 else (0, 1)
    jobs = [(meta, perm, order, mode) for meta, perm in configs() for order in ("TS", "ST")
            for mode in ("vig", "beau")][shard::nsh]
    rows = []
    for meta, perm, order, mode in jobs:
        if True:
            if True:
                k4 = [p for p in range(1, 27) if trans_periodic(K4, ANCH, mode, p, perm, order)]
                null = [sum(1 for p in range(1, 14) if trans_periodic(rand_ct(3000 + s), ANCH, mode, p, perm, order))
                        for s in range(10)]
                row = dict(meta, order=order, mode=mode, K4_sat=k4,
                           K4_short=sum(1 for p in k4 if p <= 13), null_short_mean=sum(null) / 10)
                rows.append(row)
                print(row, flush=True)
    tot_k4 = sum(r["K4_short"] for r in rows)
    tot_null = sum(r["null_short_mean"] for r in rows)
    out = {"positive_controls": pc, "rows": rows, "K4_short_hits_total": tot_k4, "expected_under_null": tot_null}
    json.dump(out, open(f"results_ragged_widths_{shard}of{nsh}.json", "w"), indent=1)
    print("TOTAL short-period hits: K4", tot_k4, "expected", round(tot_null, 1))


if __name__ == "__main__":
    main()
