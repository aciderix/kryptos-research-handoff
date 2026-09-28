import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(3)
def colpairs(a,p):
    # nombre de paires de lettres égales dans une même colonne (i ≡ j mod p), normalisé
    s=0; n=0
    for r in range(p):
        col=a[r::p]; cnt=np.bincount(col,minlength=26); s+=(cnt*(cnt-1)//2).sum(); n+=len(col)*(len(col)-1)//2
    return s/n
N=20000; S=[rng.permutation(c) for _ in range(N)]
print("p  IC_col(K4)  moyenne  P(>=)")
for p in range(1,22):
    k=colpairs(c,p); z=np.array([colpairs(s,p) for s in S])
    print(f"{p:2d}  {k:.4f}  {z.mean():.4f}  {np.mean(z>=k):.4f}")
