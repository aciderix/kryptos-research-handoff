"""ctl_t34.py — contrôle positif de T34 : faux K4 chiffré par autoclé sur le clair à l'écart 7, forme Vigenère,
alphabet du clair σ ≠ alphabet du chiffré τ (deux alphabets à mot-clé), décalage d, amorce aléatoire ; cribs à leur place."""
import random, re, sys
random.seed(int(sys.argv[2]) if len(sys.argv) > 2 else 11)
al = open(sys.argv[1], "rb").read(); n = len(al) // 26
A = lambda k: al[k*26:(k+1)*26].decode()
sa, ta = A(random.randrange(n)), A(random.randrange(n)); d = random.randrange(26)
txt = re.sub("[^A-Z]", "", open(sys.argv[3], errors="ignore").read().upper())
s = random.randrange(len(txt) - 200); p = list(txt[s:s+97])
p[21:34] = "EASTNORTHEAST"; p[63:74] = "BERLINCLOCK"
sg = {c: i for i, c in enumerate(sa)}; tinv = {i: c for i, c in enumerate(ta)}
pr = [random.randrange(26) for _ in range(7)]
c = "".join(tinv[(sg[p[i]] + (pr[i] if i < 7 else sg[p[i-7]]) - d) % 26] for i in range(97))
print(c); print("".join(p), sa, ta, d, file=sys.stderr)
