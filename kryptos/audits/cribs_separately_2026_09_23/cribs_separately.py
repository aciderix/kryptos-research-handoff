"""Each crib ALONE (relay 23/09: 'two independent systems/keys').
(1) With a fixed alphabet (K1/K2 style: tableau alphabet for plain, cipher and key), the key each crib needs is fully
    determined -> printed for AZ and KRYPTOS, Vig / Beau / variant Beaufort.  A keyword (Egypt, Berlin...) would be readable.
(2) Each crib alone, Quagmire III ANY alphabet, periodic key p: smallest periods compatible, K4 vs 50 random ciphertexts."""
import sys, json
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import periodic, K4, AZ, KA, rand_ct
C1 = {21 + j: c for j, c in enumerate("EASTNORTHEAST")}; C2 = {63 + j: c for j, c in enumerate("BERLINCLOCK")}
out = {"keystreams": {}}
for name, cr in (("EASTNORTHEAST", C1), ("BERLINCLOCK", C2)):
    for an, A in (("AZ", AZ), ("KRYPTOS", KA)):
        for md in ("vig", "beau", "var"):
            ks = ""
            for i, p in cr.items():
                c, q = A.index(K4[i]), A.index(p)
                ks += A[{"vig": (c - q) % 26, "beau": (c + q) % 26, "var": (q - c) % 26}[md]]
            out["keystreams"][f"{name} {an} {md}"] = ks
            print(f"{name:14} {an:8} {md:5} clé nécessaire : {ks}")
out["periodic_alone"] = {}
for name, cr in (("EASTNORTHEAST", C1), ("BERLINCLOCK", C2)):
    for md in ("vig", "beau"):
        rows = []
        for p in range(1, len(cr)):
            k4 = periodic(K4, cr, md, p, "III")
            nul = sum(periodic(rand_ct(1200 + s), cr, md, p, "III") for s in range(50))
            rows.append((p, k4, nul))
        out["periodic_alone"][f"{name} {md}"] = rows
        print(name, md, "périodes compatibles (témoin /50) :", [(p, n) for p, k, n in rows if k],
              "| éliminées :", [p for p, k, n in rows if not k])
json.dump(out, open("results.json", "w"), indent=1)
