import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
KA="KRYPTOSABCDEFGHIJLMNQUVWXZ"
rng=np.random.default_rng(5)
for name,al in (("A-Z",{c:i for i,c in enumerate("ABCDEFGHIJKLMNOPQRSTUVWXYZ")}),("KA",{c:i for i,c in enumerate(KA)})):
    c=np.array([al[x] for x in K4])
    def band(a,lo,hi):
        d=(a[:,1:]-a[:,:-1])%26; s=np.minimum(d,26-d); return ((s>=lo)&(s<=hi)).sum(1)
    S=np.array([rng.permutation(c) for _ in range(100000)])
    for lo,hi in ((1,1),(1,2),(1,3),(1,5),(6,10),(11,13)):
        k=band(c[None,:],lo,hi)[0]; z=band(S,lo,hi)
        print(f"{name} |Δ| dans [{lo},{hi}] : K4 {k} ; moyenne {z.mean():.1f} ; P(<=) {np.mean(z<=k):.3f} ; P(>=) {np.mean(z>=k):.3f}")
    # colonne 4->5 des blocs de 7
    d=[(c[i+1]-c[i])%26 for i in range(4,96,7)]; print(name,"col4→5 :",[min(x,26-x) for x in d])
