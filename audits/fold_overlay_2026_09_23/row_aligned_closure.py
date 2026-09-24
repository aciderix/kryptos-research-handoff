"""Why NO row-aligned overlay of the tableau can work (any horizontal shift, any row pairing, mirrored or not).
Along a tableau row, letters follow the KA order (or reverse order if mirrored): the key read under 13
consecutive cipher letters of one row is KA[s], KA[s+-1], ... EASTNORTHEAST (K4 21-33) lies inside cipher
row 26. So for such an overlay, the implied key at 21..33 must be a +1 or -1 progression in KA.
Check every convention (P = f(C, T)); if none gives a +-1 progression, the whole class is closed."""
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
C = "FLRVQQPRNGKSS"; P = "EASTNORTHEAST"
out = {}
for an, A in (("AZ", AZ), ("KA", KA)):
    for name, f in (("vig", lambda p, c: (c - p) % 26), ("beau", lambda p, c: (c + p) % 26),
                    ("varbeau", lambda p, c: (p - c) % 26)):
        key = [A[f(A.index(p), A.index(c))] for p, c in zip(P, C)]
        ka = [KA.index(k) for k in key]
        steps = [(b - a) % 26 for a, b in zip(ka, ka[1:])]
        out[f"{name}_{an}"] = ("".join(key), steps, all(s == 1 for s in steps) or all(s == 25 for s in steps))
for k, v in out.items():
    print(k, v[0], "pas KA:", v[1], "progression +-1:", v[2])
