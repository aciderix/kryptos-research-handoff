"""t15_wheatstone_motcle.py — cryptographe de Wheatstone, cadran extérieur À MOT-CLÉ (audit « vision », 24/09/2026)

Extension de audits/wheatstone_2026_09_23 (cadrans A–Z et KRYPTOS seulement, incompatibles).
Motif : dans ce dispositif, deux lettres chiffrées identiques de suite (doublet) apparaissent exactement quand la
lettre claire suivante est la voisine immédiate de la précédente sur le cadran (26 pas). Les doublets des cribs
(NO→QQ, ST→SS, IN→TT) exigent donc un cadran où O–N–I et T–S se suivent ; ni A–Z ni KRYPTOS ne le font.
Cadran extérieur : mot-clé (lettres sans répétition) + reste de l'alphabet dans l'ordre, puis le blanc (27 cases).
Sens de rotation : horaire ou inverse (équivaut au cadran lu à l'envers). Cadran intérieur : QUELCONQUE (26!).
Décalage inconnu d entre les deux cribs (0..25). Juge exact : chaque case intérieure porte une seule lettre chiffrée.
Mots-clés : vocabulaire des textes versés au dépôt (sources/…/texte) et d'une liste thématique ; si le vocabulaire des
dossiers du groupe est disponible localement (non versé), il est ajouté.
Témoins : pour chaque mot-clé retenu, 200 chiffrés aléatoires (taux de compatibilité attendu).
"""
import glob, os, re, random
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIBS = [(21, "EASTNORTHEAST"), (63, "BERLINCLOCK")]
THEME = """KRYPTOS PALIMPSEST ABSCISSA BERLIN CLOCK BERLINCLOCK EASTNORTHEAST SANBORN SCHEIDT WEBSTER LANGLEY GIRASOL
SHADOW SHADOWFORCES ILLUSION IQLUSION NUANCE LUCID MEDUSA UNDERGROUND LAYERTWO CARTER HOWARDCARTER TUTANKHAMUN EGYPT
COMPASS WHEATSTONE MENGENLEHREUHR WELTZEITUHR ALEXANDERPLATZ URANIA DOORWAY CANDLE CHAMBER MAGNETIC POSITION VIRTUALLY
INVISIBLE DIGITAL INTERPRETATION TIME WALL BRANDENBURG CHECKPOINT CHARLIE BURIED LOCATION MESSAGE LODESTONE PETRIFIED
QUARTZ COPPER GRANITE ANTIPODES CYRILLIC PROJECTOR KGB CIA NSA OSS DONOVAN DULLES CASEY GATES AGENCY SECRET CODE CIPHER""".split()
words = set(THEME)
roots = [os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../sources/docs_utilisateur_2026_09_24/texte")]
extra = "/tmp/claude-0/-home-user-kryptos-research-handoff/6f413935-69f8-5daa-ba44-4ada97bf49d9/scratchpad/gio/txt"
if os.path.isdir(extra): roots.append(extra)
for r in roots:
    for f in glob.glob(r + "/**/*.txt", recursive=True):
        try: t = open(f, errors="ignore").read()
        except Exception: continue
        for w in re.findall(r"[A-Za-z]{3,16}", t): words.add(w.upper())
def keyed(w):
    seen = []
    for ch in w + AZ:
        if ch not in seen: seen.append(ch)
    return "".join(seen)
rings = sorted({keyed(w) for w in words})

def compatible(ct, ring, sign):
    ring = ring + " "
    rels = []
    for _, w in CRIBS:
        pos, out = 0, [0]
        for a, b in zip(w, w[1:]):
            s = (sign * (ring.index(b) - ring.index(a))) % 27
            if s == 0: s = 27
            pos += s; out.append(pos % 26)
        rels.append(out)
    # premier crib seul
    inner, back = {}, {}
    for j, r in enumerate(rels[0]):
        c = ct[CRIBS[0][0] + j]
        if inner.setdefault(r, c) != c or back.setdefault(c, r) != r: return False
    for d in range(26):
        inn, bk, good = dict(inner), dict(back), True
        for j, r in enumerate(rels[1]):
            x = (r + d) % 26; c = ct[CRIBS[1][0] + j]
            if inn.setdefault(x, c) != c or bk.setdefault(c, x) != x: good = False; break
        if good: return True
    return False

def doublet_ok(ring, sign):
    ring = ring + " "
    need = [("N", "O"), ("S", "T"), ("I", "N")]
    return all((sign * (ring.index(b) - ring.index(a))) % 27 == 26 for a, b in need)

print(f"{len(words)} mots, {len(rings)} cadrans distincts")
nd = 0; hits = []
for ring in rings:
    for sign in (1, -1):
        if doublet_ok(ring, sign): nd += 1
        if compatible(K4, ring, sign): hits.append((ring, sign))
print(f"cadrans où les trois doublets des cribs sont possibles (O–N–I et T–S voisins) : {nd}")
print(f"cadrans compatibles avec K4 (deux cribs, cadran intérieur quelconque) : {len(hits)}")
rnd = random.Random(1)
for ring, sign in hits[:50]:
    null = sum(compatible("".join(rnd.choice(AZ) for _ in range(97)), ring, sign) for _ in range(200))
    print(f"  {ring} sens {'+' if sign > 0 else '-'} : témoins {null}/200")
# taux global au hasard : 300 chiffrés aléatoires contre 2000 cadrans tirés au hasard dans la liste
sample = rnd.sample(rings, min(2000, len(rings)))
tot = sum(compatible("".join(rnd.choice(AZ) for _ in range(97)), r, s) for r in sample[:300] for s in (1, -1))
print(f"témoin global : {tot} compatibilités pour 300 cadrans × 2 sens sur des chiffrés aléatoires")

# contrôle positif : faux K4 chiffré par un vrai Wheatstone (cadran extérieur à mot-clé tiré au hasard, intérieur aléatoire)
def wheatstone_encrypt(pt, ring, inner, sign):
    ring = ring + " "; pos_out = ring.index(" "); pos_in = 0; out = []
    for ch in pt:
        s = (sign * (ring.index(ch) - pos_out)) % 27
        if s == 0: s = 27
        pos_out = ring.index(ch); pos_in = (pos_in + s) % 26; out.append(inner[pos_in])
    return "".join(out)
ok = 0
for trial in range(20):
    ring = keyed(rnd.choice(sorted(words))); inner = "".join(rnd.sample(AZ, 26)); sign = rnd.choice((1, -1))
    pt = [rnd.choice(AZ) for _ in range(97)]
    for st, w in CRIBS: pt[st:st + len(w)] = list(w)
    pt = "".join(pt)
    # pas de lettre claire répétée de suite (convention du dispositif : on l'évite en pratique)
    pt = "".join(ch if i == 0 or ch != pt[i - 1] else ("Q" if ch != "Q" else "X") for i, ch in enumerate(pt))
    ok += compatible(wheatstone_encrypt(pt, ring, inner, sign), ring, sign)
print(f"contrôle positif : {ok}/20 faux K4 reconnus")
