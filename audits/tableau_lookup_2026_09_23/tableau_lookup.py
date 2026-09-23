"""Relay (23/09): 'the tableau letter H(i) at the SAME physical position is not the shift itself but a selector':
key(i) = g(H(i)) with g an UNKNOWN map letter -> shift (26 free values, one per tableau letter, never per position).
Layout: cipher rows 25-28 (31 wide), tableau rows from ../fold_overlay_2026_09_23 (FOLD j<->j, FRONT_MIRROR j<->len-1-j,
footer with/without blank), row offset d = -2..+2.  Encryption: Quagmire III (any alphabet) and IV (any two alphabets),
Vig/Beau, exact on the 24 crib letters.  Null: 50 random ciphertexts on the same positions."""
import sys, json
sys.path.insert(0, "../algebraic_elimination_2026_09_22"); sys.path.insert(0, "../fold_overlay_2026_09_23")
from k4_algebraic import Model, _alph, _relation, AZ, rand_ct
src = open("../fold_overlay_2026_09_23/fold_overlay.py").read().split("res = {}")[0]
exec(src)                                    # ROWS, K4, CRIB, tab_row, positions
POS = positions()
def H(mode, fb, d):
    out = {}
    for k, r, j in POS:
        rr = r + d
        if not 1 <= rr <= 28: return None
        row = tab_row(rr, fb); jj = j if mode == "FOLD" else len(row) - 1 - j
        out[k] = row[jj] if 0 <= jj < len(row) else " "
    return out
def sat(ct, h, q, md):
    m = Model(); P, C = _alph(m, q)
    for i, pt in CRIB.items():
        if h[i] == " ": continue
        _relation(m, P, C, ct[i], pt, [(1, "g" + h[i])], md)
    return m.solve()
res = []
for mode in ("FOLD", "FRONT_MIRROR"):
    for fb in (False, True):
        for d in range(-2, 3):
            h = H(mode, fb, d)
            if h is None: continue
            letters = [h[i] for i in CRIB if h[i] != " "]
            rep = len(letters) - len(set(letters))
            for q in ("III", "IV"):
                for md in ("vig", "beau"):
                    k4 = sat(K4, h, q, md)
                    nul = sum(sat(rand_ct(5000 + t), h, q, md) for t in range(50))
                    r = dict(mode=mode, footer_blank=fb, d=d, repeats=rep, q=q, md=md, K4=k4, null=nul)
                    res.append(r); print(json.dumps(r), flush=True)
json.dump(res, open("results.json", "w"), indent=1)
inf = [r for r in res if r["null"] < 40]
print("informative cells (null<80%):", len(inf), " K4 compatible among them:", sum(r["K4"] for r in inf))
