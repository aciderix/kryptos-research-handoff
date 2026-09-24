"""Pre-registered (23/09): Sanborn's own words - 'top plate' / 'bottom plate' (1990), 'pull up one layer,
then you can come to the next', copper = 'paper' perforated by text (1989), projection born from two Kryptos
plates brought together (2009). Hypothesis: the TOP plate (K1-K2, rows 1-14) is the template laid on the
BOTTOM plate (K3-K4, rows 15-28); the top-plate letter over each K4 letter is its key.
Variants fixed in advance: SLIDE (top row t over row t+14) or FOLD (hinge at the junction: row t over row 29-t);
columns by index (j<->j) or justified (same relative x). Relations: direct, Vigenere, Beaufort, variant
Beaufort, in AZ and KA. Criterion: crib letters reproduced out of 24 (chance ~0.9 per relation).
Row text: CIA / Wikipedia transcription of the cipher side, 28 rows.
"""
import json
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
ROWS = """EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJ
YQTQUXQBQVYUVLLTREVJYQTMKYRDMFD
VFPJUDEEHZWETZYVGWHKKQETGFQJNCE
GGWHKK?DQMCPFQZDQMMIAGPFXHQRLG
TIMVMZJANQLVKQEDAGDVFRPJUNGEUNA
QZGZLECGYUXUEENJTBJLBQCRTBJDFHRR
YIZETKZEMVDUFKSJHKFWHKUWQLSZFTI
HHDDDUVH?DWKBFUFPWNTDFIYCUQZERE
EVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDX
FLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKF
FHQNTGPUAECNUVPDJMQCLQUMUNEDFQ
ELZZVRRGKFFVOEEXBDMVPNFQXEZLGRE
DNQFMPNZGLFLPMRJQYALMGNUVPDXVKP
DQUMEBEDMHDAFMJGZNUPLGEWJLLAETG
ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIA
CHTNREYULDSLLSLLNOHSNOSMRWXMNE
TPRNGATIHNRARPESLNNELEBLPIIACAE
WMTWNDITEENRAHCTENEUDRETNHAEOE
TFOLSEDTIWENHAEIOYTEYQHEENCTAYCR
EIFTBRSPAMHHEWENATAMATEGYEERLB
TEEFOASFIOTUETUAEOTOARMAEERTNRTI
BSEDDNIAAHTTMSTEWPIEROAGRIEWFEB
AECTDDHILCEIHSITEGOEAOSDDRYDLORIT
RKLMLEHAGTDHARDPNEOHMGFMFEUHE
ECDMRIPFEIMEHNLSSTTRTVDOHW?OBKR
UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO
TWTQSJQSSEKZZWATJKLUDIAWINFBNYP
VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR""".split()
assert len(ROWS) == 28
CRIB = {21 + j: p for j, p in enumerate("EASTNORTHEAST")}
CRIB.update({63 + j: p for j, p in enumerate("BERLINCLOCK")})

k4pos = []  # (k, row(1-based), col)
k = 0
for r in (25, 26, 27, 28):
    row = ROWS[r - 1]
    start = row.index("?") + 1 if r == 25 else 0
    for j in range(start, len(row)):
        k4pos.append((k, r, j)); k += 1
K4 = "".join(ROWS[r - 1][j] for _, r, j in k4pos)
assert K4.startswith("OBKRUOX") and len(K4) == 97


def rels(P, C, T, A):
    if T not in A:
        return {}
    p, c, t = A.index(P), A.index(C), A.index(T)
    return {"vig": (c - t) % 26 == p, "beau": (t - c) % 26 == p, "varbeau": (c + t) % 26 == p}


res = {}
for mode in ("SLIDE", "FOLD"):
    for cols in ("index", "justified"):
        cnt, under = {"direct": 0}, []
        for k, r, j in k4pos:
            tr = r - 14 if mode == "SLIDE" else 29 - r
            top = ROWS[tr - 1]; n = len(ROWS[r - 1]); m = len(top)
            jj = j if cols == "index" else int((j + 0.5) / n * m)
            T = top[jj] if jj < m else " "
            under.append(T)
            if k in CRIB:
                cnt["direct"] += CRIB[k] == T
                for A, an in ((AZ, "AZ"), (KA, "KA")):
                    for name, ok in rels(CRIB[k], K4[k], T, A).items():
                        cnt[f"{name}_{an}"] = cnt.get(f"{name}_{an}", 0) + ok
        res[f"{mode}_{cols}"] = {"hits_out_of_24": cnt, "template_letters_over_K4": "".join(under)}
print(json.dumps(res, indent=1))
