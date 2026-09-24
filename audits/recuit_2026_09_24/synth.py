"""synth.py — chiffrés synthétiques du même modèle que sa.c (contrôles positifs).
Clair : fenêtre de 97 lettres d'un livre du corpus, cribs EASTNORTHEAST (21) et BERLINCLOCK (63) insérés.
Alphabet(s) : alphabet à mot-clé (mot tiré d'une liste) ; clé : m[i mod p] + a[seg(i)] aléatoires.
Option errs : nombre d'erreurs (lettres chiffrées remplacées au hasard) dont au moins une dans les cribs si possible."""
import random, re, sys, glob
CORP = "/tmp/claude-0/-home-user-kryptos-research-handoff/001fee97-849c-56c2-808b-4e071a32a106/scratchpad/corpus/"
_txt = None
def text():
    global _txt
    if _txt is None:
        t = open(CORP + "pg2600.txt", errors="ignore").read() + open(CORP + "pg1661.txt", errors="ignore").read()
        _txt = re.sub("[^A-Z]", "", t.upper())
    return _txt
WORDS = ["PALIMPSEST", "ABSCISSA", "KRYPTOS", "SHADOW", "MEDUSA", "BERLIN", "COMPASS", "LODESTONE", "MAGNETIC", "LATITUDE",
         "INVISIBLE", "SCULPTURE", "TUTANKHAMEN", "EGYPT", "CLOCK", "WEBSTER", "LANGLEY", "SANBORN", "SCHEIDT", "GIRASOL"]
def kalpha(w):
    s = []
    for ch in w + "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
        if ch not in s: s.append(ch)
    return "".join(s)
def make(rng, q, mode, p, W, o, errs=0, n=97):
    T = text(); st = rng.randrange(len(T) - n); pt = list(T[st:st + n])
    pt[21:34] = list("EASTNORTHEAST"); pt[63:74] = list("BERLINCLOCK")
    A1 = kalpha(rng.choice(WORDS)); A2 = kalpha(rng.choice(WORDS))
    if q == "Q3": A2 = A1
    if q == "Q2": A1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    if q == "Q1": A2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    m = [rng.randrange(26) for _ in range(p)]; ns = (n - 1 + o) // W + 1 if W else 1
    a = [0] + [rng.randrange(26) for _ in range(ns - 1)]
    ct = []
    for i, ch in enumerate(pt):
        k = m[i % p] + a[(i + o) // W if W else 0]; x = A1.index(ch)
        y = (x + k) % 26 if mode == "VIG" else (k - x) % 26 if mode == "BEAU" else (x - k) % 26
        ct.append(A2[y])
    cribs = list(range(21, 34)) + list(range(63, 74))
    for e in range(errs):
        i = rng.choice(cribs) if e == 0 else rng.randrange(n)
        ct[i] = chr(65 + (ord(ct[i]) - 65 + 1 + rng.randrange(25)) % 26)
    return "".join(pt), "".join(ct)
if __name__ == "__main__":
    rng = random.Random(int(sys.argv[1]))
    pt, ct = make(rng, sys.argv[2], sys.argv[3], int(sys.argv[4]), int(sys.argv[5]), int(sys.argv[6]), int(sys.argv[7]) if len(sys.argv) > 7 else 0)
    print(pt); print(ct)
