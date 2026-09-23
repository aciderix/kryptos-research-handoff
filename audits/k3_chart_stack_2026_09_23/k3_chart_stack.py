"""K3 chart (24x14, INTERMEDIATE text of K3's double transposition) + K4 (7x14 with '?') = 31x14 (relay, 23/09).
Stacking K4 under the chart at the same width 14 = linear alignment of the chart text with K4, so testing EVERY offset
of the chart text covers every row pairing and both '?' conventions.  Key text variants (fixed in advance):
chart row-major, row-major reversed, column-major (top->bottom), column-major bottom->top (K3's own read-out direction).
Models: (a) fixed alphabets P/C in {AZ,KA}, key letter read in the plaintext alphabet, Vig/Beau/var -> crib matches /24;
        (b) Quagmire III ANY alphabet, key letter in the same alphabet (K1/K2 style), Vig/Beau, exact (runkey engine).
Also: letter statistics of K4 (for the 'Cardan grille extracting letters from a longer English text' idea)."""
import sys, json, random
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import runkey, K4, ANCH, AZ, KA, rand_ct
CHART = """ILNTAYESTATHCW BLHMHEHAROIEEH ISIWNTHONRSLEO OLTETYMFTEHMHD ELAAEOAERIILUV TSGCRIPEEPEKET PDNADESTEWCRFR
CLRIUARBAELTMT OUPIEHBLMTIIFT EYTPNNTRRSHRGS HEELEEFEAMDOMS RRNBTWIEOTDLHL SNMECIEYTTTDON LTXLHTRGOHCYEH NHWEADCEEAERNE
HCRNREYTAAADPM OAMNNSAAUIBDDI RISLELTMTNESRE HAOSEOCAEDFOAF ANNETFNTUDWAHP YHSPITEATEEEDI DSHRDEENOSIOTR NYOANOHEIBRGGM
EDNRWEQWFIGEAD""".split()
assert len(CHART) == 24 and all(len(r) == 14 for r in CHART)
V = {"rowmajor": "".join(CHART)}
V["rowmajor_rev"] = V["rowmajor"][::-1]
V["colmajor_down"] = "".join(CHART[r][c] for c in range(14) for r in range(24))
V["colmajor_up"] = "".join(CHART[r][c] for c in range(14) for r in range(23, -1, -1))
out = {}
for name, T in V.items():
    best = (0, None); anyal = []
    for o in range(-21, len(T) - 73):                 # every offset keeping the 24 crib letters inside the key text
        if not all(0 <= i + o < len(T) for i in ANCH): continue
        for pa in (AZ, KA):
            for ca in (AZ, KA):
                for md in ("vig", "beau", "var"):
                    sc = 0
                    for i, pt in ANCH.items():
                        c, p, k = ca.index(K4[i]), pa.index(pt), pa.index(T[i + o])
                        sc += {"vig": (p + k) % 26 == c, "beau": (k - p) % 26 == c, "var": (p - k) % 26 == c}[md]
                    if sc > best[0]: best = (sc, (o, "AZ" if pa == AZ else "KA", "AZ" if ca == AZ else "KA", md))
        for md in ("vig", "beau"):
            if runkey(K4, ANCH, md, T, o): anyal.append((o, md))
    out[name] = {"fixed_best": best, "anyalphabet_compatible": anyal}
    print(name, "| fixed alphabets best:", best, "| any alphabet (QIII) compatible offsets:", anyal, flush=True)
# null for (b): same scan on 8 random ciphertexts (parallel); results saved first
json.dump(out, open("results.json", "w"), indent=1)
from multiprocessing import Pool
def one(s):
    ct = rand_ct(8800 + s)
    return any(runkey(ct, ANCH, md, T, o) for T in V.values() for o in range(-21, len(T) - 73)
               if all(0 <= i + o < len(T) for i in ANCH) for md in ("vig", "beau"))
from collections import Counter
cnt = Counter(K4); ic = sum(v * (v - 1) for v in cnt.values()) / (97 * 96)
vow = sum(cnt[c] for c in "AEIOU") / 97
print(f"K4: IC = {ic:.4f} (English ~0.066, random ~0.038); vowels = {vow:.2f} (English ~0.38); E = {cnt['E']}, T = {cnt['T']}", flush=True)
out.update({"K4_IC": ic, "K4_vowel_share": vow})
json.dump(out, open("results.json", "w"), indent=1)
if __name__ == "__main__":
    with Pool(4) as p: v = p.map(one, range(8))
    print("null: random ciphertexts with ANY compatible offset:", sum(v), "/8", flush=True)
    out["null_any_offset_of_8"] = sum(v); json.dump(out, open("results.json", "w"), indent=1)
