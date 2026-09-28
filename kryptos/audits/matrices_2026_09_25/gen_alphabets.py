"""gen_alphabets.py — alphabets « en matrice » : alphabet à mot-clé écrit en lignes de largeur w (2..13), puis relu
par colonnes. Lectures : colonnes de gauche à droite ou de droite à gauche, de haut en bas ou de bas en haut,
et colonnes dans l'ordre alphabétique des lettres du mot-clé (colonnar à clé, si w = nombre de lettres du mot).
L'alphabet relu à l'envers est traité par les programmes (option rev). Doublons retirés.
Sortie : alphas.txt (un alphabet de 26 lettres par ligne) et alphas_labels.txt (même ordre)."""
import sys
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
def kw_alpha(w):
    s = []
    for c in w + AZ:
        if c not in s: s.append(c)
    return "".join(s)
def reads(a, w, kw):
    rows = [a[i:i + w] for i in range(0, 26, w)]
    cols = [[r[c] for r in rows if c < len(r)] for c in range(w)]
    out = {}
    for lr in (0, 1):
        order = range(w) if lr == 0 else range(w - 1, -1, -1)
        for ud in (0, 1):
            out[f"w{w}{'LR' if lr == 0 else 'RL'}{'down' if ud == 0 else 'up'}"] = "".join("".join(cols[c] if ud == 0 else cols[c][::-1]) for c in order)
    u = []
    for c in kw:
        if c not in u: u.append(c)
    if len(u) == w:
        order = sorted(range(w), key=lambda c: u[c])
        out[f"w{w}keyed"] = "".join("".join(cols[c]) for c in order)
    return out
words = [l.strip() for l in open(sys.argv[1]) if l.strip()]
seen = set(); A = open("alphas.txt", "w"); Lb = open("alphas_labels.txt", "w")
for wd in words:
    a = kw_alpha(wd)
    for w in range(2, 14):
        for lab, al in reads(a, w, wd).items():
            assert len(al) == 26 and len(set(al)) == 26
            if al in seen or al[::-1] in seen: continue
            seen.add(al); A.write(al + "\n"); Lb.write(f"{wd} {lab}\n")
print(len(seen))
