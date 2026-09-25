"""ascii7.py — lecture des 17 lignes binaires de Martinsburg en ASCII 7 bits (décalage choisi pour chaque ligne :
celui qui donne le plus de caractères imprimables A–Z, espace et virgule)."""
import re
src = open("../../sources/docs_utilisateur_2026_09_24/BinarySystems.txt").read()
part = src[src.index("Right-hand side in binary:"):src.index("Line 1:")]
B = [re.sub("[^01]", "", l) for l in part.splitlines() if "..." in l]
names_txt = src[src.index("Left-hand side engraved"):src.index("Right-hand side")]
N = [l.strip() for l in names_txt.splitlines()[3:] if l.strip() and l.strip()[0].isalpha()]
ok = set("ABCDEFGHIJKLMNOPQRSTUVWXYZ ,")
for li, bits in enumerate(B):
    cands = []
    for off in range(7):
        s = "".join(chr(int(bits[i:i + 7], 2)) for i in range(off, len(bits) - 6, 7))
        cands.append((sum(c in ok for c in s), off, s))
    sc, off, s = max(cands)
    rest = len(bits) - off - 7 * len(s)
    print(f"{li + 1:2d} décalage {off} ({sc}/{len(s)} lisibles, {rest} bits en trop à droite) : {s!r:40s} | noms : {N[li][:45]}")
