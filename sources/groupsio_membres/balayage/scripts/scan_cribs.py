# Confronte aux cribs chaque chaine de 97-98 lettres des textes des membres (24/09/2026).
# Scan members' text files for 97/98-letter candidate decrypts and score them against the cribs
import os, re, json, collections
ROOT = "/home/user/kryptos-research-handoff/sources/groupsio_membres/textes"
TREE = "/home/user/kryptos-research-handoff/sources/groupsio_membres/inventaire/tree.json"
CT = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
C1, C2 = "EASTNORTHEAST", "BERLINCLOCK"
dates = {}
for x in json.load(open(TREE)):
    if x.get("type") != "Folder":
        dates[x["path"].rstrip("/") + "/" + x["name"]] = x.get("date", "")
        dates.setdefault(x["name"], x.get("date", ""))
spaced = re.compile(r'^(?:[A-Za-z] ){20,}[A-Za-z]$')
res = []
nfiles = ncand = 0
for dp, dn, fn in os.walk(ROOT):
    for f in fn:
        p = os.path.join(dp, f)
        rel = p[len(ROOT):]
        orig = rel[:-4] if rel.endswith(".txt") else rel
        date = dates.get(orig, "") or dates.get(os.path.basename(orig), "")
        try:
            txt = open(p, encoding="utf-8", errors="ignore").read()
        except Exception:
            continue
        nfiles += 1
        seen = set()
        for line in txt.replace("\r", "\n").split("\n"):
            s = line.strip()
            if spaced.match(s): s = s.replace(" ", "")
            for run in re.findall(r'[A-Za-z]{97,98}', s) + ([s] if re.fullmatch(r'[A-Za-z?]{97,99}', s) else []):
                u = re.sub(r'[^A-Za-z]', '', run).upper()
                if len(u) not in (97, 98) or u in seen: continue
                seen.add(u)
                for off in ((0,) if len(u) == 97 else (0, 1)):
                    v = u[off:off + 97]
                    if v == CT: continue
                    ncand += 1
                    m1 = sum(v[21 + i] == C1[i] for i in range(13))
                    m2 = sum(v[63 + i] == C2[i] for i in range(11))
                    if m1 + m2 >= 6 or m1 >= 5 or m2 >= 5:
                        res.append((m1 + m2, m1, m2, date, rel, v))
res.sort(key=lambda r: (-r[0], r[3]))
print("files", nfiles, "candidates", ncand, "flagged", len(res))
json.dump(res, open("crib_hits.json", "w"))
hist = collections.Counter(min(r[0], 24) for r in res)
print("score histogram (flagged):", sorted(hist.items()))
