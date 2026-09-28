"""'Change the base of the language' (Scheidt 2020) as 5-bit coding + XOR / add mod 32.
Fixed in advance: codings A1Z26, A0Z25, ITA2 Baudot (letters shift); ops XOR, ADD mod 32
(C = P op K); periodic key p = 1..48; exact consistency on the 24 crib letters.
Null: 200 random ciphertexts.  Also prints the raw 24-value key stream."""
import random, json
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
CRIBS = {21: "EASTNORTHEAST", 63: "BERLINCLOCK"}
ANCH = {s + j: c for s, w in CRIBS.items() for j, c in enumerate(w)}
ITA2 = dict(zip("ETAOINSHRDLUCMFPGWYBVKXJQZ",   # ITA2 letter codes (bit order b1..b5, LSB = b1)
    [0b00001, 0b10000, 0b00011, 0b11000, 0b00110, 0b01100, 0b00101, 0b10100, 0b01010, 0b01001,
     0b10010, 0b00111, 0b01110, 0b11100, 0b01101, 0b10110, 0b11010, 0b10011, 0b10101, 0b11001,
     0b11110, 0b01111, 0b11101, 0b01011, 0b10111, 0b10001]))
CODES = {"A1Z26": {c: i + 1 for i, c in enumerate(AZ)}, "A0Z25": {c: i for i, c in enumerate(AZ)}, "ITA2": ITA2}
OPS = {"XOR": lambda c, p: c ^ p, "ADD32": lambda c, p: (c - p) % 32, "SUB32": lambda c, p: (p - c) % 32}

def stream(ct, code, op):
    return {i: OPS[op](CODES[code][ct[i]], CODES[code][pt]) for i, pt in ANCH.items()}

def periods_ok(ks):
    ok = []
    for p in range(1, 49):
        seen, good = {}, True
        for i, k in ks.items():
            if seen.setdefault(i % p, k) != k: good = False; break
        if good: ok.append(p)
    return ok

if __name__ == "__main__":
    assert len(set(ITA2.values())) == 26
    out = {}
    rnd = random.Random(5)
    nulls = ["".join(rnd.choice(AZ) for _ in range(97)) for _ in range(200)]
    for code in CODES:
        for op in OPS:
            ks = stream(K4, code, op); ok = periods_ok(ks)
            nm = [len(periods_ok(stream(n, code, op))) for n in nulls]
            frac = {p: sum(p in periods_ok(stream(n, code, op)) for n in nulls) / 200 for p in ok}
            out[f"{code}/{op}"] = {"K4_periods": ok, "null_frac_at_those_periods": frac,
                                   "keystream": [ks[i] for i in sorted(ks)]}
            print(code, op, "K4 periods:", ok, "null frac:", frac)
            print("   key:", [ks[i] for i in sorted(ks)])
    json.dump(out, open("results.json", "w"), indent=1)
