"""Pre-registered (23/09): the only parameter-free overlay of the two faces of the Kryptos screen.

Facts used: cipher side = 28 rows (CIA transcription), K4 = rows 25-28 (31 chars each, row 25 ends '?OBKR').
Tableau = 28 rows: header ABCD..ZABCD, rows A..Z (label + KA rotated by row + 4 wrap letters; row N has the
extra L), footer. Tableau is cut to be read from the BACK; folding the screen like a book puts it behind the
cipher face, readable from the front, row r over row r, reading positions j over j (outer edges aligned).
Variants (fixed in advance, no tuning): FOLD (j<->j), FRONT-MIRROR (tableau seen from the front without
folding: j <-> len-1-j), footer with/without leading blank. For each, the tableau letter T under a K4 letter
C is related to the crib plaintext P by: P = T (direct), or Vigenere / Beaufort / variant Beaufort with
T as key, in AZ and in KA. Criterion: crib letters reproduced out of 24; chance ~ 24/26.
"""
import json
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
ROWS = {25: "ECDMRIPFEIMEHNLSSTTRTVDOHW?OBKR", 26: "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO",
        27: "TWTQSJQSSEKZZWATJKLUDIAWINFBNYP", 28: "VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"}
K4 = "".join(ROWS[r] for r in (25, 26, 27, 28)).split("?")[1]
assert K4.startswith("OBKR") and len(K4) == 97
CRIB = {21 + j: p for j, p in enumerate("EASTNORTHEAST")}
CRIB.update({63 + j: p for j, p in enumerate("BERLINCLOCK")})


def tab_row(r, footer_blank):
    if r in (1, 28):
        return (" " if footer_blank else "") + AZ + "ABCD"
    i = r - 2
    row = AZ[i] + KA[i:] + KA[:i] + (KA[i:] + KA[:i])[:4]
    if AZ[i] == "N":
        row += "L"
    return row


def positions():
    out, k = [], 0
    for r in (25, 26, 27, 28):
        for j, ch in enumerate(ROWS[r]):
            if r == 25 and j < 27:
                continue
            if ch == "?":
                continue
            out.append((k, r, j)); k += 1
    return out


def rel(P, C, T, A):
    p, c, t = A.index(P), A.index(C), A.index(T)
    return {"vig": (c - t) % 26 == p, "beau": (t - c) % 26 == p, "varbeau": (c + t) % 26 == p}


res = {}
for mode in ("FOLD", "FRONT_MIRROR"):
    for fb in (False, True):
        pos = positions(); cnt = {"direct": 0}
        under = []
        for k, r, j in pos:
            row = tab_row(r, fb)
            jj = j if mode == "FOLD" else len(row) - 1 - j
            T = row[jj] if 0 <= jj < len(row) else " "
            under.append(T)
            if k in CRIB and T != " ":
                cnt["direct"] += CRIB[k] == T
                for A, an in ((AZ, "AZ"), (KA, "KA")):
                    for name, ok in rel(CRIB[k], K4[k], T, A).items():
                        cnt[f"{name}_{an}"] = cnt.get(f"{name}_{an}", 0) + ok
        res[f"{mode}{'_footerblank' if fb else ''}"] = {"hits_out_of_24": cnt,
                                                          "tableau_under_K4": "".join(under)}
print(json.dumps(res, indent=1))
