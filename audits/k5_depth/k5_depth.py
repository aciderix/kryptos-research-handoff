"""Ready-to-run 'messages in depth' check for the day K5's ciphertext is published.
Usage: python3 k5_depth.py K5CIPHERTEXT            (97 letters; '?' allowed)
Basis (base 1): K5 = 97 characters, shares coded words AT THE SAME POSITIONS as K4 (Sanborn 2025);
said to be an 'alternative K4' from 1988 with its own chart.
Tests (all parameter-free):
 1. positions where K4 and K5 carry the SAME cipher letter; runs of >= 3 = candidate shared word
    under the same key AND same alphabets (chance: ~97/26 = 3.7 isolated coincidences);
 2. difference D = K4 - K5 (mod 26) in A-Z and in KRYPTOS order; under a shared additive keystream
    D = PT4 - PT5, so at the 24 known K4 letters we get K5's plaintext directly -> printed for reading;
 3. the same for Beaufort (D' = K4 + K5 relation) ;
 4. random control: the same statistics for 1000 shuffled K5 (how unusual are the equal-letter runs?)."""
import sys, random
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
CRIB = {21 + j: c for j, c in enumerate("EASTNORTHEAST")} | {63 + j: c for j, c in enumerate("BERLINCLOCK")}

def runs(a, b):
    eq = [i for i in range(97) if a[i] == b[i]]
    out, cur = [], []
    for i in eq:
        if cur and i == cur[-1] + 1: cur.append(i)
        else:
            if len(cur) >= 3: out.append(cur)
            cur = [i]
    if len(cur) >= 3: out.append(cur)
    return eq, out

def main(k5):
    k5 = "".join(c for c in k5.upper() if c.isalpha() or c == "?")
    assert len(k5) == 97, f"K5 has {len(k5)} characters, expected 97"
    eq, rr = runs(K4, k5)
    print(f"1. equal cipher letters at {len(eq)} positions (chance ~3.7): {eq}")
    print(f"   runs >= 3 (candidate shared words): {rr}")
    rnd = random.Random(1); big = 0
    for _ in range(1000):
        s = list(k5); rnd.shuffle(s)
        if any(len(r) >= max([len(x) for x in rr] or [3]) for r in runs(K4, "".join(s))[1]): big += 1
    print(f"   control: shuffled K5 reaching the same longest run: {big}/1000")
    for name, A in (("A-Z", AZ), ("KRYPTOS", KA)):
        for mode in ("vig", "beau"):
            pt5 = []
            for i in range(97):
                if i in CRIB and k5[i] in A:
                    if mode == "vig":   # C = P + K  => P5 = P4 - C4 + C5
                        v = (A.index(CRIB[i]) - A.index(K4[i]) + A.index(k5[i])) % 26
                    else:               # C = K - P  => P5 = P4 + C4 - C5
                        v = (A.index(CRIB[i]) + A.index(K4[i]) - A.index(k5[i])) % 26
                    pt5.append(A[v])
                else:
                    pt5.append(".")
            s = "".join(pt5)
            print(f"2-3. shared keystream, {name}, {mode}: K5 plaintext at crib positions: {s[21:34]} ... {s[63:74]}")

if __name__ == "__main__":
    main(sys.argv[1])
