"""K3 CIPHERTEXT as key for K4 (relais Gemini, 23/09).  Scope fixed in advance:
 (1) linear: key = K3CT[i+o], every offset o keeping the 24 anchors inside K3CT;
 (2) row-aligned (same plate, 'column by column'): key of K4 letter at (row r, col c)
     = cipher letter at (row r-d, col c), d = 1..11 (rows 15-25 of the bottom plate).
Models: (A) QIII, any alphabet, key letter read in the same alphabet (K1/K2 style);
        (B) key letter -> number by A-Z or KRYPTOS index; alphabets QI-QIV free.
Vig and Beau.  Exact 24/24.  Null: same scan on 20 random ciphertexts."""
import sys, json, random
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
sys.path.insert(0, "../plate_overlay_2026_09_23")
from k4_algebraic import Model, _alph, _relation, ANCH, AZ, KA, K4
exec(open("../plate_overlay_2026_09_23/plate_overlay.py").read().split("def ")[0])
K3CT = "".join(ROWS[14:24]) + ROWS[24][:26]          # rows 15-25 up to '?'
K3CT = K3CT.replace("?", "")
def k4pos(i):
    return (25, 27 + i) if i < 4 else (26 + (i - 4) // 31, (i - 4) % 31)

def keyletters(mode_, par):
    if mode_ == "lin":
        o = par; return {i: K3CT[i + o] for i in ANCH} if all(0 <= i + o < len(K3CT) for i in ANCH) else None
    d = par; out = {}
    for i in ANCH:
        r, c = k4pos(i); rr = r - d
        if rr < 15 or c >= len(ROWS[rr - 1]) or ROWS[rr - 1][c] == "?": return None
        out[i] = ROWS[rr - 1][c]
    return out

def sat(ct, kl, model, mode):
    m = Model()
    if model == "A":
        P, C = _alph(m, "III")
        for i, pt in ANCH.items(): _relation(m, P, C, ct[i], pt, [(1, "P" + kl[i])], mode)
        return m.solve()
    q, alpha = model
    P, C = _alph(m, q)
    for i, pt in ANCH.items(): _relation(m, P, C, ct[i], pt, [], mode, key_const=alpha.index(kl[i]))
    return m.solve()

MODELS = ["A"] + [(q, a) for q in ("I", "II", "III", "IV") for a in (AZ, KA)]
PARAMS = [("lin", o) for o in range(-21, len(K3CT))] + [("row", d) for d in range(1, 12)]
def scan(ct):
    hits = []
    for kind, par in PARAMS:
        kl = keyletters(kind, par)
        if kl is None: continue
        for mdl in MODELS:
            for mode in ("vig", "beau"):
                if sat(ct, kl, mdl, mode):
                    hits.append((kind, par, mdl if mdl == "A" else mdl[0] + ("AZ" if mdl[1] == AZ else "KA"), mode))
    return hits

if __name__ == "__main__":
    print("K3CT length", len(K3CT), "valid params", sum(1 for k, p in PARAMS if keyletters(k, p)))
    h = scan(K4); print("K4 hits:", h, flush=True)
    rnd = random.Random(7); nulls = []
    for s in range(20):
        ct = "".join(rnd.choice(AZ) for _ in range(97)); nulls.append(len(scan(ct)))
    print("null hits per random ct:", nulls)
    json.dump({"K4_hits": h, "null_hits": nulls, "K3CT_len": len(K3CT)}, open("results.json", "w"), indent=1)
