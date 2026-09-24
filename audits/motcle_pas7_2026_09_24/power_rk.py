"""Puissance du test « clé courante anglaise » : score de vrais fragments anglais (13 lettres à la position 21 et
11 lettres à la position 63 d'une même fenêtre de 97 lettres), avec la table de quadrigrammes de build_qg."""
import re, random, struct, sys, array
qg = array.array('f'); qg.frombytes(open(sys.argv[1], 'rb').read())
books = sys.argv[2:]
T = "".join(re.sub("[^A-Z]", "", open(b, errors="ignore").read().upper()) for b in books)
rnd = random.Random(1)
def sc(s):
    v = [ord(c) - 65 for c in s]
    return sum(qg[((v[i] * 26 + v[i + 1]) * 26 + v[i + 2]) * 26 + v[i + 3]] for i in range(len(v) - 3))
xs = []
for _ in range(20000):
    st = rnd.randrange(len(T) - 97); w = T[st:st + 97]
    xs.append((sc(w[21:34]) + sc(w[63:74])) / 18)
xs.sort()
for thr in (-4.40, -4.58, -4.98, -5.2, -5.5):
    print(f"part des vraies clés anglaises sous {thr} : {sum(x < thr for x in xs) / len(xs):.3f}")
print("médiane", xs[len(xs) // 2], "1er centile", xs[len(xs) // 100])
for nerr in (1, 2):
    ys = []
    for _ in range(20000):
        st = rnd.randrange(len(T) - 97); w = list(T[st:st + 97])
        for _ in range(nerr):
            i = rnd.choice(list(range(21, 34)) + list(range(63, 74))); w[i] = chr(65 + (ord(w[i]) - 65 + 1 + rnd.randrange(25)) % 26)
        w = "".join(w); ys.append((sc(w[21:34]) + sc(w[63:74])) / 18)
    print(f"{nerr} erreur(s) : part sous -4.98 = {sum(y < -4.98 for y in ys) / len(ys):.3f} ; sous -4.40 = {sum(y < -4.40 for y in ys) / len(ys):.3f}")
