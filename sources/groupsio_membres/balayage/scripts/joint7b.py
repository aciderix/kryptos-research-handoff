import numpy as np, itertools
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(99)
def vert(a,w):
    # nombre de paires (i<j) avec (a[i],a[i+w]) == (a[j],a[j+w])
    code=a[:,:-w]*26+a[:,w:]
    out=np.zeros(a.shape[0],int)
    for x in range(676):
        n=(code==x).sum(1); out+=n*(n-1)//2
    return out
print("K4 bigrammes verticaux répétés, largeurs 7,14,21,28:",[int(vert(c[None,:],w)[0]) for w in (7,14,21,28)])
N=100000
S=np.vstack([c[None,:],np.array([rng.permutation(c) for _ in range(N)])])
M=list(range(2,25))
dbl=(S[:,1:]==S[:,:-1]); nd=dbl.sum(1); pos=np.arange(96)
def pv(x): ref=np.sort(x[1:]); return 1-np.searchsorted(ref,x,side='left')/N
PD,PC,PV={},{},{}
for m in M:
    cnt=np.stack([dbl[:,pos%m==r].sum(1) for r in range(m)],1)
    PD[m]=pv(np.where(nd>=4,cnt.max(1)/np.maximum(nd,1),0.0))
    PC[m]=pv((S[:,m:]==S[:,:-m]).sum(1))
    w=3*m
    PV[m]=pv(vert(S,w)) if w<=48 else np.ones(N+1)
print("m=7 : p doublets",PD[7][0],"p écart 7",PC[7][0],"p bigrammes verticaux largeur 21",PV[7][0])
for name,f in [("D×C",lambda m:PD[m]*PC[m]),("D×C×V(3m)",lambda m:PD[m]*PC[m]*PV[m]),("D×V(3m)",lambda m:PD[m]*PV[m]),("C×V(3m)",lambda m:PC[m]*PV[m])]:
    arr=np.stack([f(m) for m in M]); sc=arr.min(0); best=M[int(np.argmin(arr[:,0]))]
    print(f"{name}: K4 {sc[0]:.2e} (m={best}) ; P(mélange <= K4) = {np.mean(sc[1:]<=sc[0]):.5f}")
# seuls, avec balayage de m
for name,P in [("D",PD),("C",PC),("V(3m)",PV)]:
    arr=np.stack([P[m] for m in M]); sc=arr.min(0)
    print(f"{name} seul, balayage m=2..24: K4 {sc[0]:.2e} ; corrigé {np.mean(sc[1:]<=sc[0]):.4f}")
