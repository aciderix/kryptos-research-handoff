import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(11)
rows=np.array([0]*4+[1]*31+[2]*31+[3]*31)
def chi(a, groups, G):
    s=0.0
    for L in range(26):
        m=(a==L); n=m.sum()
        if n==0: continue
        for g in range(G):
            e=n*(groups==g).mean(); o=(m&(groups==g)).sum(); s+=(o-e)**2/e
    return s
def span_stat(a):
    # somme sur les lettres (>=3 occurrences) de l'écart-type des positions (petit = regroupé)
    s=0.0
    for L in range(26):
        p=np.where(a==L)[0]
        if len(p)>=3: s+=p.std()
    return s
halves=np.array([0]*49+[1]*48); thirds=np.arange(97)*3//97; quarters=np.arange(97)*4//97
tests=[("lignes physiques (4,31,31,31)",lambda a:chi(a,rows,4)),("moitiés",lambda a:chi(a,halves,2)),("tiers",lambda a:chi(a,thirds,3)),("quarts",lambda a:chi(a,quarters,4)),("dispersion des positions (petit=regroupé)",lambda a:-span_stat(a))]
N=20000
S=[rng.permutation(c) for _ in range(N)]
for name,f in tests:
    k=f(c); z=np.array([f(s) for s in S])
    print(f"{name}: K4 = {abs(k):.2f} ; P(mélange >= K4) = {np.mean(z>=k):.4f}")
# O's in the first 35
from math import comb
print("5 O dans les 35 premières : ", comb(35,5)/comb(97,5))
for L in "OURKT":
    print(L, [i for i,x in enumerate(K4) if x==L])
