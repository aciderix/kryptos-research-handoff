import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
rng=np.random.default_rng(13)
def stat_blocks(s,k):
    c=np.array([ord(x)-65 for x in s]); n=len(c); g=np.arange(n)*k//n; size=np.bincount(g); tot=np.bincount(c,minlength=26)
    chi=0
    for j in range(k):
        O=np.bincount(c[g==j],minlength=26); E=tot*size[j]/n; m=E>0; chi+=((O-E)[m]**2/E[m]).sum()
    return chi
def pval(s,k,N=20000):
    v=stat_blocks(s,k); l=list(s); z=0
    for _ in range(N):
        rng.shuffle(l); z+=stat_blocks(l,k)>=v
    return z/N
# 1) sans les doublets (on retire la 2e lettre de chaque doublet)
nod="".join(x for i,x in enumerate(K4) if not (i>0 and K4[i-1]==x))
print(len(nod), nod)
for k in (4,6,9): print("sans doublets, blocs",k,":",pval(nod,k,5000))
# 2) répétitions à courte distance
c=np.array([ord(x)-65 for x in K4])
for d in range(1,13):
    print(d, int((c[d:]==c[:-d]).sum()), end=" | ")
print()
N=20000; S=np.array([rng.permutation(c) for _ in range(N)])
for D in (3,5,8,12):
    k=sum(int((c[d:]==c[:-d]).sum()) for d in range(1,D+1))
    z=np.array([sum(int((s[d:]==s[:-d]).sum()) for d in range(1,D+1)) for s in S[:5000]])
    print(f"répétitions à distance 1..{D}: K4 {k}, moyenne mélanges {z.mean():.2f}, P(>=) {np.mean(z>=k):.4f}")
