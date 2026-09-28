import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
KA="KRYPTOSABCDEFGHIJLMNQUVWXZ"
pos=list(range(21,34))+list(range(63,74)); PT="EASTNORTHEAST"+"BERLINCLOCK"
AZi={ch:i for i,ch in enumerate("ABCDEFGHIJKLMNOPQRSTUVWXYZ")}; KAi={ch:i for i,ch in enumerate(KA)}
def keys(ct,alpha,mode):
    out=[]
    for i,p in zip(pos,PT):
        c=alpha[ct[i]]; q=alpha[p]
        out.append((c-q)%26 if mode=="VIG" else (c+q)%26)   # VARB = -VIG : même IC
    return np.array(out)
def ic(k):
    cnt=np.bincount(k,minlength=26); return (cnt*(cnt-1)).sum()/(len(k)*(len(k)-1))
def local(k):
    # paires de valeurs égales à distance <= 3 dans un même crib
    s=0
    for a in range(len(k)):
        for b in range(a+1,min(a+4,len(k))):
            if (a<13)==(b<13) and k[a]==k[b]: s+=1
    return s
conv=[(n,al,m) for n,al in (("AZ",AZi),("KA",KAi)) for m in ("VIG","BEAU")]
for n,al,m in conv:
    k=keys(K4,al,m); print(n,m,"clé:",list(k),"IC=%.4f"%ic(k),"égalités proches:",local(k))
rng=np.random.default_rng(1); letters=list(K4); N=200000
best_k=max(ic(keys(K4,al,m)) for n,al,m in conv)
hit=0; hitB=0; kB=ic(keys(K4,AZi,"BEAU"))
for _ in range(N):
    ct="".join(rng.choice(letters,97))
    v=[ic(keys(ct,al,m)) for n,al,m in conv]
    hit+= max(v)>=best_k; hitB+= v[1]>=kB
print(f"IC max K4 (4 conventions) = {best_k:.4f} ; P(chiffré aléatoire >=) = {hit/N:.4f} ; Beaufort AZ seul : {hitB/N:.4f}")
