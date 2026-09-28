"""Deterministic positional battery (protocol: README.md)."""
import json, random, sys
import numpy as np
sys.path.insert(0, "../fold_overlay_2026_09_23"); sys.path.insert(0, "../plate_overlay_2026_09_23")
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
exec(open("../plate_overlay_2026_09_23/plate_overlay.py").read().split("def ")[0])       # ROWS = list of the 28 cipher rows
CIPHER_ROWS = list(ROWS)
exec(open("../fold_overlay_2026_09_23/fold_overlay.py").read().split("res = {}")[0])      # ROWS(25-28 dict), K4, CRIB, tab_row, positions
assert CIPHER_ROWS and len(CIPHER_ROWS) == 28
POS = positions()                           # (k, r, j) for the 97 K4 letters
CI = sorted(CRIB)                           # 24 crib indices
PT = [CRIB[i] for i in CI]

def tab(r, fb):
    return tab_row(r, fb) if 1 <= r <= 28 else None

def geom_grid(flipv, mirror, d, fb):
    H, L = {}, {}
    for k, r, j in POS:
        rr = (29 - r if flipv else r) + d
        row = tab(rr, fb)
        if row is None: return None
        jj = len(row) - 1 - j if mirror else j
        H[k] = row[jj] if 0 <= jj < len(row) else None
        L[k] = row[0] if 2 <= rr <= 27 else None
    return H, L

def geom_linear(from_end, count_q, header):
    cipher = "".join(CIPHER_ROWS)
    if not count_q: cipher = cipher.replace("?", "")
    tabl = "".join(tab(r, False) for r in range(1 if header else 2, 29))
    # absolute index of each K4 letter in the cipher text
    idx = []; n = 0
    start = len(cipher) - 97
    for k in range(97): idx.append(start + k)
    H, L = {}, {}
    for k in range(97):
        a = idx[k] if not from_end else len(tabl) - 97 + k
        H[k] = tabl[a] if 0 <= a < len(tabl) else None
        L[k] = None
    return H, L

GEOMS = {}
for flipv in (False, True):
    for mirror in (False, True):
        for d in range(-3, 4):
            for fb in (False, True):
                g = geom_grid(flipv, mirror, d, fb)
                if g: GEOMS[f"grid flipv={flipv} mirror={mirror} d={d:+d} fb={fb}"] = g
for fe in (False, True):
    for cq in (False, True):
        for hd in (False, True):
            GEOMS[f"linear from_end={fe} count_q={cq} header={hd}"] = geom_linear(fe, cq, hd)

ROWCOL = {k: (r, j) for k, r, j in POS}
def keys_for(H, L, X):
    out = {}
    def x(c): return X.index(c) if c and c in X else None
    rows = {}
    for name in ("H", "H-L", "L-H", "H+col", "H-col", "H+row", "H-row", "H+i", "H-i", "L", "col", "row", "i", "col+row", "edge"):
        v = []
        for i in CI:
            r, j = ROWCOL[i]; h = x(H[i]); l = x(L[i])
            val = {"H": h, "H-L": None if h is None or l is None else h - l, "L-H": None if h is None or l is None else l - h,
                   "H+col": None if h is None else h + j, "H-col": None if h is None else h - j,
                   "H+row": None if h is None else h + r, "H-row": None if h is None else h - r,
                   "H+i": None if h is None else h + i, "H-i": None if h is None else h - i,
                   "L": l, "col": j, "row": r, "i": i, "col+row": j + r, "edge": min(j, 30 - j)}[name]
            v.append(-99 if val is None else val % 26)
        out[name] = np.array(v)
    return out

KEYSETS = []
for gname, (H, L) in GEOMS.items():
    for Xn, X in (("AZ", AZ), ("KA", KA)):
        for fname, kv in keys_for(H, L, X).items():
            KEYSETS.append((gname, Xn, fname, kv))
KEYMAT = np.array([k for *_, k in KEYSETS])            # (n_keysets, 24)
PTI = {"AZ": np.array([AZ.index(c) for c in PT]), "KA": np.array([KA.index(c) for c in PT])}

def scan(ct, want_best=False):
    best, arg = -1, None
    for pa in ("AZ", "KA"):
        for ca in ("AZ", "KA"):
            C = np.array([(AZ if ca == "AZ" else KA).index(ct[i]) for i in CI]); P = PTI[pa]
            for mode, req in (("vig", (C - P) % 26), ("beau", (C + P) % 26), ("var", (P - C) % 26)):
                sc = (KEYMAT == req).sum(1); m = int(sc.max())
                if m > best:
                    best = m; arg = (pa, ca, mode, KEYSETS[int(sc.argmax())][:3])
    return (best, arg) if want_best else best

if __name__ == "__main__":
    print("geometries:", len(GEOMS), " keysets:", len(KEYSETS), " conventions: 12")
    b, arg = scan(K4, True)
    print("K4 best:", b, "/24 at", arg)
    rnd = random.Random(21)
    nulls = [scan("".join(rnd.choice(AZ) for _ in range(97))) for _ in range(200)]
    dist = {v: nulls.count(v) for v in sorted(set(nulls))}
    print("null max distribution:", dist, " P(>= K4) =", sum(v >= b for v in nulls) / 200)
    json.dump({"K4_best": b, "K4_arg": arg, "null_max_dist": dist, "n_geoms": len(GEOMS), "n_keysets": len(KEYSETS)},
              open("results.json", "w"), indent=1, default=str)
