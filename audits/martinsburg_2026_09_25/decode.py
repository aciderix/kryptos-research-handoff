"""decode.py — Martinsburg (IRS, Sanborn 1999) : les 17 lignes binaires codent-elles les 17 lignes de noms ?
Relevé D. Wilson 2003 (sources/docs_utilisateur_2026_09_24/BinarySystems.txt). Lignes binaires alignées à droite,
coupées à gauche. On essaie : largeur b = 5..8 bits, valeur de A = 0, 1 ou 65 (ASCII), bits de poids fort en tête ou
en queue, complément, chaîne de bits inversée (miroir), alignement sur la fin ou sur tout décalage ; on compare le texte
décodé à la ligne de noms correspondante, lue à l'endroit ou à l'envers, avec ou sans espaces et virgules."""
import re, itertools
src = open("../../sources/docs_utilisateur_2026_09_24/BinarySystems.txt").read()
part = src[src.index("Right-hand side in binary:"):src.index("Line 1:")]
B = [re.sub("[^01]", "", l) for l in part.splitlines() if "..." in l]
names_txt = src[src.index("Left-hand side engraved"):src.index("Right-hand side")]
N = [l.strip() for l in names_txt.splitlines()[3:] if l.strip() and l.strip()[0].isalpha()]
print(len(B), "lignes binaires ;", len(N), "lignes de noms")
def forms(s):
    t = re.sub(r"\[[^\]]*\]", "?", s)
    return {"lettres": re.sub("[^A-Z?]", "", t), "espaces": re.sub("[^A-Z? ]", "", t), "ponct": re.sub("[^A-Z?, ]", "", t)}
def dec(bits, b, base, rev_bits, charset):
    out = []
    for i in range(0, len(bits) - b + 1, b):
        chunk = bits[i:i + b]
        if rev_bits: chunk = chunk[::-1]
        v = int(chunk, 2) - base
        out.append(charset.get(v, "."))
    return "".join(out)
best = []
for li, bits0 in enumerate(B):
    for comp, mirror in itertools.product((0, 1), (0, 1)):
        bits = bits0
        if comp: bits = "".join("1" if c == "0" else "0" for c in bits)
        if mirror: bits = bits[::-1]
        for b in (5, 6, 7, 8):
            for base, cs in ((0, {i: chr(65 + i) for i in range(26)} | {26: " ", 27: ","}), (1, {i: chr(65 + i) for i in range(26)} | {-1: " "}), (65, {i: chr(65 + i) for i in range(26)} | {32 - 65: " ", 44 - 65: ","})):
                if base == 65 and b < 7: continue
                for rb in (0, 1):
                    for off in range(b):
                        d = dec(bits[off:], b, base, rb, cs)
                        letters = sum(c.isalpha() for c in d)
                        best.append((letters / max(1, len(d)), li, comp, mirror, b, base, rb, off, d))
best.sort(reverse=True)
for x in best[:15]: print(round(x[0], 2), x[1:8], x[8][:40])

print("\n== comparaison aux noms (toutes lignes, tous alignements) ==")
T = {}
for j, n in enumerate(N):
    for k, v in forms(n).items():
        T[(j, k)] = v; T[(j, k + "-env")] = v[::-1]
res = []
for (frac, li, comp, mirror, b, base, rb, off, d) in best:
    if len(d) < 6: continue
    for key, t in T.items():
        for sh in range(-len(d) + 4, len(t) - 3):
            m = n_ = 0
            for i, c in enumerate(d):
                j = sh + i
                if 0 <= j < len(t) and t[j] != "?":
                    n_ += 1; m += c == t[j]
            if n_ >= 6: res.append((m / n_, m, n_, li, key, comp, mirror, b, base, rb, off, sh, d))
res.sort(reverse=True)
for r in res[:12]: print(round(r[0], 2), r[1:12], r[12][:30])
