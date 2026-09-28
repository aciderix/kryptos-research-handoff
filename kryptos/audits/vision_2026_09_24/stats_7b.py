# stats_7b.py — correctif du 24/09 : la largeur 21 n'est PAS indépendante des deux autres signaux DANS K4.
# Sur ses 11 bigrammes verticaux répétés, QZ (25/26) et ZT (46/47) sont les doublets QQ, ZZ, TT empilés (espacés de 21),
# et PK (65/72) vient de deux coïncidences à l'écart 7 (65/72, 86/93). On ne garde donc que doublets × écart,
# avec une loi nulle de référence (200 000 mélanges) et 1 000 000 de mélanges testés, module m balayé de 2 à 24.
import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(777)
M=list(range(2,25)); pos=np.arange(96)
def stats(S):
    dbl=(S[:,1:]==S[:,:-1]); nd=dbl.sum(1); D={}; C={}
    for m in M:
        cnt=np.stack([dbl[:,pos%m==r].sum(1) for r in range(m)],1)
        D[m]=np.where(nd>=4,cnt.max(1)/np.maximum(nd,1),0.0); C[m]=(S[:,m:]==S[:,:-m]).sum(1)
    return D,C
# 1) loi nulle de référence (200 000) pour les p marginales
R=np.array([rng.permutation(c) for _ in range(200000)])
DR,CR=stats(R)
ref={m:(np.sort(DR[m]),np.sort(CR[m])) for m in M}
def pv(v,refs): return 1-np.searchsorted(refs,v,side='left')/len(refs)
def score(S):
    D,C=stats(S)
    return np.min(np.stack([pv(D[m],ref[m][0])*pv(C[m],ref[m][1]) for m in M]),0)
k=score(c[None,:])[0]; print("score K4",k)
hits=0; N=0
for _ in range(10):
    T=np.array([rng.permutation(c) for _ in range(100000)]); s=score(T); hits+=int((s<=k).sum()); N+=len(s)
print(f"doublets × écart, module 2–24 : {hits}/{N} = {hits/N:.2e}")
