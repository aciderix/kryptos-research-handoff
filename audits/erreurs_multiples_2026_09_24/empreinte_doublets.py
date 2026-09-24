"""empreinte_doublets.py — les doublets d'un Quagmire périodique se concentrent sur certaines phases de la clé.
(audit « erreurs multiples », 24/09/2026)

1. Petit fragment de Sanborn (97 lettres, KRYPTOS, clé SHADOW, période 6) et K1–K2 : doublets par phase de clé.
   Mécanisme : un doublet c_i = c_{i+1} se produit quand σ(p_{i+1}) − σ(p_i) = k_i − k_{i+1}. Si la différence
   de clé à une phase tombe sur une différence fréquente des bigrammes anglais dans l'alphabet, cette phase
   accumule les doublets.
2. Conséquence pour K4 : si ses 5 doublets en phase 4 (mod 7) viennent d'une différence de clé fixe d, les trois
   doublets des cribs (NO→QQ, ST→SS, IN→TT) imposent σ(O)−σ(N) = σ(N)−σ(I) = σ(T)−σ(S) = d dans l'alphabet du clair.
   Filtre appliqué à tous les alphabets à mot-clé tirés du corpus (et à une liste thématique).
3. Taux de doublets ρ(d) = part des bigrammes anglais de différence d : KRYPTOS, A–Z, GIRASOL, meilleur alphabet.
Usage : empreinte_doublets.py <dossier du corpus>
"""
import re, glob, sys, random, math, collections
CORP = sys.argv[1] if len(sys.argv) > 1 else "corpus"
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"; AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KI = {c: i for i, c in enumerate(KA)}

print("== 1. Doublets par phase de clé dans les chiffres périodiques de Sanborn ==")
ROWS = ["RAJIRBAPKMQJDZKHQYZQVJTEL", "QEEDJNLFZXREKUOWTZZGGQYZ", "UQUGMDPEZETZZOLMDLIHHQYZ", "MFJFZDYHHDJWJMEBFEEHWJSE"]
FC = "".join(ROWS); FP = "UNCOVERINGINTENTIONSISTHESTRONGPOINTOFHUMANESPIONAGEBUTITISANEXAGGERATIONTOSAYTHATONLYESPIONAGECO"
panel = "".join(open("../vision_2026_09_24/data/panel_wikipedia.txt").read().split())
K1C = panel[:63]; K2C = panel[63:869 - 97 - 1 - 336].replace("?", "")
def dec(c, key): return "".join(KA[(KI[x] - KI[key[i % len(key)]]) % 26] for i, x in enumerate(c))
for name, c, key, p in (("fragment (SHADOW)", FC, "SHADOW", FP), ("K1 (PALIMPSEST)", K1C, "PALIMPSEST", None), ("K2 (ABSCISSA)", K2C, "ABSCISSA", None)):
    p = p or dec(c, key)
    d = [i for i in range(len(c) - 1) if c[i] == c[i + 1]]
    ph = collections.Counter(i % len(key) for i in d)
    print(f"{name} : {len(c)} lettres, {len(d)} doublets (≈ {(len(c) - 1) / 26:.1f} attendus au hasard) ; par phase {dict(sorted(ph.items()))}")
    for j in sorted(ph):
        dj = (KI[key[j]] - KI[key[(j + 1) % len(key)]]) % 26
        print(f"   phase {j} ({key[j]}→{key[(j + 1) % len(key)]}, différence {dj}) : clairs {[p[i:i + 2] for i in d if i % len(key) == j]}")

print("\n== 2. Filtre : σ(O)−σ(N) = σ(N)−σ(I) = σ(T)−σ(S) (alphabets à mot-clé) ==")
words = collections.Counter()
for f in glob.glob(CORP + "/*.txt"):
    for w in re.findall(r"[A-Za-z]+", open(f, errors="ignore").read()):
        if 3 <= len(w) <= 15: words[w.upper()] += 1
THEME = "KRYPTOS PALIMPSEST ABSCISSA SHADOW MEDUSA BERLIN CLOCK COMPASS LODESTONE MAGNETIC EGYPT CAIRO WELTZEITUHR ALEXANDERPLATZ GIRASOL SANBORN SCHEIDT LANGLEY WEBSTER TUTANKHAMEN CARTER NORTHEAST EAST IQLUSION UNDERGRUUND DESPARATLY DIGETAL VIRTUALLY INVISIBLE LUCID MEMORY POSITION HILL YAR DYAHR".split()
for w in THEME: words[w] += 0
def alph(w, cont):
    s = []
    for c in w:
        if c not in s: s.append(c)
    if not cont: rest = [c for c in AZ if c not in s]
    else: i = AZ.index(s[-1]); rest = [AZ[(i + 1 + j) % 26] for j in range(26) if AZ[(i + 1 + j) % 26] not in s]
    return "".join(s + rest)
def passes(a):
    x = {c: i for i, c in enumerate(a)}
    d = (x["N"] - x["I"]) % 26
    return d if d == (x["O"] - x["N"]) % 26 == (x["T"] - x["S"]) % 26 else None
tot = hit = 0; thits = []
for w in words:
    for cont in (0, 1):
        a = alph(w, cont); tot += 1; d = passes(a)
        if d is not None:
            hit += 1
            if w in THEME: thits.append((w, "suite" if cont else "standard", d, a))
print(f"alphabets (sens direct ; le sens inverse donne le même verdict) : {tot}, compatibles {hit} ({100 * hit / tot:.2f} %)")
print("mots thématiques compatibles :", thits)
print("KRYPTOS :", passes(KA), " A–Z :", passes(AZ))

print("\n== 3. Taux de doublets ρ(d) ==")
txt = "".join(re.sub("[^A-Z]", "", open(f, errors="ignore").read().upper()) for f in sorted(glob.glob(CORP + "/*.txt"))[:12])
big = [[0] * 26 for _ in range(26)]
for a, b in zip(txt, txt[1:]): big[ord(a) - 65][ord(b) - 65] += 1
T = sum(map(sum, big)); fq = [[x / T for x in r] for r in big]
def rho(al):
    x = {c: i for i, c in enumerate(al)}; r = [0] * 26
    for a in range(26):
        for b in range(26): r[(x[chr(65 + b)] - x[chr(65 + a)]) % 26] += fq[a][b]
    return r
for n, al in (("A–Z", AZ), ("KRYPTOS", KA), ("GIRASOL", "GIRASOLBCDEFHJKMNPQTUVWXYZ"), ("BERLIN (suite)", "BERLINOPQSTUVWXYZACDFGHJKM")):
    r = rho(al); top = sorted(range(26), key=lambda d: -r[d])[:4]
    print(f"{n:15s} différences les plus fréquentes : {[(d, round(100 * r[d], 1)) for d in top]} (en %)")
def best(constrained, seed, iters=300000):
    rnd = random.Random(seed)
    al = list("INOST") + [c for c in AZ if c not in "INOST"] if constrained else list(AZ)
    if constrained: rest = al[5:]; rnd.shuffle(rest); al = al[:5] + rest
    else: rnd.shuffle(al)
    def sc(al):
        x = {c: i for i, c in enumerate(al)}
        if constrained and not ((x["N"] - x["I"]) % 26 == (x["O"] - x["N"]) % 26 == (x["T"] - x["S"]) % 26 == 1): return -1
        return sum(fq[ord(al[i]) - 65][ord(al[(i + 1) % 26]) - 65] for i in range(26))
    cur = sc(al)
    for it in range(iters):
        T = 0.01 * (1 - it / iters) + 1e-5; i, j = rnd.randrange(26), rnd.randrange(26); al[i], al[j] = al[j], al[i]; s = sc(al)
        if s >= cur or rnd.random() < math.exp((s - cur) / T): cur = s
        else: al[i], al[j] = al[j], al[i]
    return cur
print(f"meilleur alphabet possible (pas 1) : {100 * max(best(False, s) for s in range(3)):.1f} % ; avec la contrainte I-N-O / S-T : {100 * max(best(True, s) for s in range(3)):.1f} %")
from math import comb
for r in (0.0385, 0.07, 0.143):
    pv = sum(comb(14, k) * r ** k * (1 - r) ** (14 - k) for k in range(5, 15))
    print(f"P(≥ 5 doublets sur les 14 cases de la phase 4 | taux {100 * r:.1f} %) = {pv:.2g}")
