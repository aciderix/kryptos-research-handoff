"""Visible words on Kryptos as alphabet keyword and/or periodic key (protocol: README.md)."""
import json, random
import numpy as np
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
CRIBS = {21: "EASTNORTHEAST", 63: "BERLINCLOCK"}
POS = [s + j for s, w in CRIBS.items() for j in range(len(w))]
PT = [c for s, w in CRIBS.items() for c in w]
WORDS = [w.strip() for w in open("inventory.txt") if w.strip()]

def kw(w):
    seen = []
    for c in w + AZ:
        if c not in seen: seen.append(c)
    return "".join(seen)

ALPHS = {"AZ": AZ, "KA": kw("KRYPTOS")}
for w in WORDS:
    a = kw(w)
    if a not in ALPHS.values(): ALPHS["kw:" + w] = a
NAMES = list(ALPHS); IDX = {n: np.array([[a.index(c) for c in AZ]]) for n, a in ALPHS.items()}
# key candidates: (word, phase) -> letters at crib positions
KEYS = [(w, ph) for w in WORDS for ph in range(len(w))]
KL = np.array([[AZ.index(w[(p + ph) % len(w)]) for p in POS] for w, ph in KEYS])   # A-Z letter ids

def scan(ct, top=0):
    cl = np.array([AZ.index(ct[p]) for p in POS]); pl = np.array([AZ.index(c) for c in PT])
    best, hits = 0, []
    for pn in NAMES:
        Pa = IDX[pn][0]; p = Pa[pl]
        for cn in NAMES:
            Ca = IDX[cn][0]; c = Ca[cl]
            for mode, r in (("vig", (c - p) % 26), ("beau", (c + p) % 26), ("var", (p - c) % 26)):
                for kmode, kv in (("AZ", KL), ("P", Pa[KL]), ("C", Ca[KL])):
                    sc = (kv == r).sum(1)
                    m = sc.max()
                    if m > best: best = m
                    if top and m >= top:
                        for i in np.nonzero(sc >= top)[0]:
                            hits.append((int(sc[i]), pn, cn, mode, kmode, *KEYS[i]))
    return best, hits

if __name__ == "__main__":
    print("alphabets", len(NAMES), "key candidates", len(KEYS))
    b, _ = scan(K4)
    nulls = []
    rnd = random.Random(11)
    for s in range(20):
        ct = "".join(rnd.choice(AZ) for _ in range(97)); nulls.append(int(scan(ct)[0]))
    _, hits = scan(K4, top=max(b - 1, 1))
    hits.sort(reverse=True)
    print("K4 best", b, "/24 ; null best per ct", nulls, "max", max(nulls))
    for h in hits[:15]: print(h)
    json.dump({"K4_best": int(b), "null_best": nulls, "top_K4": hits[:50]}, open("results.json", "w"), indent=1, default=str)
