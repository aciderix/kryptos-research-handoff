"""Exact structural test of the Wheatstone cryptograph (1867) on K4, for ANY inner alphabet.

Why (sources): Sanborn's own list 'Beaufort cipher, Compass cipher, Morse code, Alphabet code' (AAA papers,
IMG_1569); BERLIN CLOCK; the Morse/telegraph entrance (Wheatstone co-invented the electric telegraph);
'not mathematical', letter-for-letter (Sanborn 2019).
Device: outer ring = plain order (26 letters + 1 blank = 27 positions), inner ring = 26-letter alphabet
(unknown). The long hand is turned clockwise to each plaintext letter (a repeated letter = a full turn);
the short hand advances the same number of steps on the 26-ring. Short-hand position = cumulative steps mod 26.
Within a crib, the steps are fixed by the known plaintext; the absolute start of each crib is unknown
(absorbed, for the first crib, by the unknown inner alphabet; for the second, an offset d in 0..25).
Consistency: each inner position holds one ciphertext letter and vice versa.
Variants: outer order AZ or KA (blank after the last letter), clockwise (+) or counter-clockwise (-),
repeated letter = full turn (27 steps) or 0 steps.
"""
import json
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
CRIBS = [(21, "EASTNORTHEAST"), (63, "BERLINCLOCK")]


def positions(word, outer, sign, rep):
    """relative short-hand positions (mod 26) along a crib, first letter = 0."""
    ring = outer + " "                      # 27 positions
    pos, out = 0, [0]
    for a, b in zip(word, word[1:]):
        s = (sign * (ring.index(b) - ring.index(a))) % 27
        if s == 0:
            s = 27 if rep == "full" else 0
        pos += s
        out.append(pos % 26)
    return out


res = {}
for on, outer in (("AZ", AZ), ("KA", KA)):
    for sign in (1, -1):
        for rep in ("full", "zero"):
            ok_d = []
            rels = [positions(w, outer, sign, rep) for _, w in CRIBS]
            for d in range(26):
                inner = {}
                back = {}
                good = True
                for (st, w), rel, off in zip(CRIBS, rels, (0, d)):
                    for j, r in enumerate(rel):
                        x = (r + off) % 26
                        c = K4[st + j]
                        if inner.setdefault(x, c) != c or back.setdefault(c, x) != x:
                            good = False; break
                    if not good:
                        break
                if good:
                    ok_d.append(d)
            # also: first crib alone
            inner, back, first_ok = {}, {}, True
            for j, r in enumerate(rels[0]):
                c = K4[CRIBS[0][0] + j]
                if inner.setdefault(r, c) != c or back.setdefault(c, r) != r:
                    first_ok = False
            res[f"outer={on},sign={'+' if sign > 0 else '-'},repeat={rep}"] = {
                "EASTNORTHEAST_alone_consistent": first_ok, "offsets_d_consistent_both": ok_d}
print(json.dumps(res, indent=1))
