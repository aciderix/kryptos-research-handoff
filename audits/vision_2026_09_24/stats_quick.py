# stats_quick.py — deux contrôles rapides (24/09/2026)
import random
CT="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
PT={i:c for i,c in zip(range(21,34),"EASTNORTHEAST")}; PT.update({i:c for i,c in zip(range(63,74),"BERLINCLOCK")})
def ic(s):
    from collections import Counter
    n=len(s); return sum(v*(v-1) for v in Counter(s).values())/(n*(n-1))
# 1) IC des 21 premières lettres (0,0667) : probabilité pour 21 lettres tirées DANS K4 (même composition), fenêtre fixe et meilleure fenêtre
ic0=ic(CT[:21]); random.seed(1); N=20000
p_fixed=sum(ic(''.join(random.sample(CT,21)))>=ic0 for _ in range(N))/N
best_k4=max(ic(CT[i:i+21]) for i in range(0,97-20))
p_best=0
for _ in range(2000):
    s=list(CT); random.shuffle(s); s=''.join(s)
    if max(ic(s[i:i+21]) for i in range(77))>=ic0: p_best+=1
print(f"IC(0..20)={ic0:.4f}; P(21 lettres de K4 au hasard >= )={p_fixed:.3f}; meilleure fenêtre de 21 dans K4={best_k4:.4f}; P(meilleure fenêtre d'un K4 mélangé >= IC(0..20))={p_best/2000:.3f}")
# 2) M-94 / réglettes à disques en ordre fixe, génératrice FIXE : position i -> disque (i mod n) ; une même lettre claire ou chiffrée
#    sur le même disque doit être cohérente (bijection par disque). n = 25 (M-94) et 1..40
for n in list(range(20,31)):
    ok=True; bad=None
    for r in range(n):
        m={}; inv={}
        for i in PT:
            if i%n!=r: continue
            p,c=PT[i],CT[i]
            if m.get(p,c)!=c or inv.get(c,p)!=p: ok=False; bad=(r,i); break
            m[p]=c; inv[c]=p
        if not ok: break
    print(f"n={n}: {'compatible' if ok else 'IMPOSSIBLE (disque %d, position %d)'%bad}")
