"""t21_wheatstone_une_erreur.py — l'élimination de T15 (Wheatstone à cadran extérieur à mot-clé) résiste-t-elle à UNE erreur ?
(audit « vision », 24/09/2026 ; suite de T19–T20)

Dans le cryptographe de Wheatstone, la position de l'aiguille intérieure ne dépend que du CLAIR (pas cumulés) : une lettre
de chiffré fausse en e ne se propage pas, elle retire seulement la paire (case intérieure, lettre chiffrée) de e.
Modèles : A = erreur quelconque en une position de crib ; B = « copie » (chiffré = clair : 32 et 73 pour K4).
Même liste de cadrans que T15 (vocabulaire des textes + liste thématique), deux sens, décalage d entre cribs libre.
Témoins : 300 cadrans tirés au hasard × 2 sens × chiffrés aléatoires.
"""
import glob, os, re, random, sys
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIBS = [(21, "EASTNORTHEAST"), (63, "BERLINCLOCK")]
here = os.path.dirname(os.path.abspath(__file__))
# même vocabulaire que T15 (on relit la liste thématique dans le fichier de T15 pour ne pas la dupliquer)
src = open(os.path.join(here, "t15_wheatstone_motcle.py")).read()
THEME = re.search(r'THEME = """(.*?)"""', src, re.S).group(1).split()
words = set(THEME)
roots = [os.path.join(here, "../../sources/docs_utilisateur_2026_09_24/texte")]
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

def rels_of(ring, sign):
    ring = ring + " "; idx = {ch: i for i, ch in enumerate(ring)}; out = []
    for _, w in CRIBS:
        pos, o = 0, [0]
        for a, b in zip(w, w[1:]):
            s = (sign * (idx[b] - idx[a])) % 27
            if s == 0: s = 27
            pos += s; o.append(pos % 26)
        out.append(o)
    return out

def conflicts(pairs):
    """positions en conflit (même case, lettres différentes ; même lettre, cases différentes)"""
    byr, byc = {}, {}
    for r, c, e in pairs:
        byr.setdefault(r, []).append((c, e)); byc.setdefault(c, []).append((r, e))
    bad = set()
    for lst in byr.values():
        if len({c for c, _ in lst}) > 1: bad.update(e for _, e in lst)
    for lst in byc.values():
        if len({r for r, _ in lst}) > 1: bad.update(e for _, e in lst)
    return bad

def fix1(pairs, allowed):
    """0 si cohérent ; sinon ensemble des positions dont le retrait (une seule) suffit, parmi `allowed`"""
    bad = conflicts(pairs)
    if not bad: return 0, set()
    ok = {e for e in bad if e in allowed and not conflicts([p for p in pairs if p[2] != e])}
    return 1, ok

def verdict(ct, ring, sign, copies):
    """(exact, A, B) avec au plus une erreur ; copies = positions où chiffré = clair"""
    rel = rels_of(ring, sign)
    p1 = [(r, ct[CRIBS[0][0] + j], CRIBS[0][0] + j) for j, r in enumerate(rel[0])]
    p2 = [(r, ct[CRIBS[1][0] + j], CRIBS[1][0] + j) for j, r in enumerate(rel[1])]
    allpos = {e for _, _, e in p1 + p2}
    n1, s1 = fix1(p1, allpos); n2, s2 = fix1(p2, allpos)
    if (n1 and not s1) or (n2 and not s2) or (n1 and n2): return False, False, False
    E = A = B = False
    for d in range(26):
        pairs = p1 + [((r + d) % 26, c, e) for r, c, e in p2]
        n, s = fix1(pairs, allpos)
        if n == 0: return True, True, True
        if s:
            A = True
            if s & copies: B = True
        if B: break
    return E, A, B

def copies_of(ct):
    return {st + j for st, w in CRIBS for j, ch in enumerate(w) if ct[st + j] == ch}

if __name__ == "__main__":
    print(f"{len(rings)} cadrans distincts")
    ck = copies_of(K4); print("positions « copie » de K4 :", sorted(ck))
    k = [0, 0, 0]; hits = []
    for ring in rings:
        for sign in (1, -1):
            v = verdict(K4, ring, sign, ck)
            for i in range(3): k[i] += v[i]
            if v[1]: hits.append((ring, sign, v))
    print(f"K4 : cadrans×sens compatibles exact {k[0]}, ≤1 erreur quelconque {k[1]}, ≤1 copie {k[2]} (sur {2 * len(rings)})")
    for ring, sign, v in hits[:30]:
        print(f"  {ring} sens {'+' if sign > 0 else '-'} : exact {int(v[0])}, A {int(v[1])}, B {int(v[2])}")
    rnd = random.Random(3)
    sample = rnd.sample(rings, 300); z = [0, 0, 0]; nz = 0
    for rg in sample:
        for sign in (1, -1):
            ct = "".join(rnd.choice(AZ) for _ in range(97))
            v = verdict(ct, rg, sign, copies_of(ct)); nz += 1
            for i in range(3): z[i] += v[i]
    tot = 2 * len(rings)
    print(f"témoins ({nz} tirages) : taux {z[0] / nz:.4f} / {z[1] / nz:.4f} / {z[2] / nz:.4f} ; "
          f"attendu sur {tot} : {tot * z[0] / nz:.1f} / {tot * z[1] / nz:.1f} / {tot * z[2] / nz:.1f}")
    # contrôle positif : vrai Wheatstone (cadran à mot-clé de la liste), puis UNE lettre de crib fausse
    def enc(pt, ring, inner, sign):
        ring = ring + " "; po = ring.index(" "); pi = 0; out = []
        for ch in pt:
            s = (sign * (ring.index(ch) - po)) % 27
            if s == 0: s = 27
            po = ring.index(ch); pi = (pi + s) % 26; out.append(inner[pi])
        return "".join(out)
    ok = 0
    for trial in range(20):
        rg = rnd.choice(rings); inner = "".join(rnd.sample(AZ, 26)); sign = rnd.choice((1, -1))
        pt = [rnd.choice(AZ) for _ in range(97)]
        for st, w in CRIBS: pt[st:st + len(w)] = list(w)
        pt = "".join(pt)  # lettres répétées admises : le dispositif fait alors un tour complet (pas de 27)
        ct = list(enc(pt, rg, inner, sign)); e = rnd.choice([st + j for st, w in CRIBS for j in range(len(w))])
        ct[e] = rnd.choice([x for x in AZ if x != ct[e]]); ct = "".join(ct)
        ok += verdict(ct, rg, sign, copies_of(ct))[1]
    print(f"contrôle positif (une lettre de crib fausse) : {ok}/20 reconnus avec au plus une erreur")
