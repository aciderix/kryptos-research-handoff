import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(2024)
N=100000
S=np.vstack([c[None,:],np.array([rng.permutation(c) for _ in range(N)])])   # ligne 0 = K4
M=list(range(3,17))
dbl=(S[:,1:]==S[:,:-1])            # (N+1,96) doublet commençant en i
nd=dbl.sum(1)
pos=np.arange(96)
D={}; C={}; V={}
for m in M:
    cnt=np.stack([dbl[:,pos%m==r].sum(1) for r in range(m)],1)
    D[m]=np.where(nd>=4, cnt.max(1)/np.maximum(nd,1), 0.0)
    C[m]=(S[:,m:]==S[:,:-m]).sum(1)
    # bigrammes verticaux répétés à la largeur 3m (21 pour m=7) : paires (i,i+1) identiques à (i+w,i+w+1)
    w=3*m
    V[m]=((S[:,w:-1]==S[:,:-w-1])&(S[:,w+1:]==S[:,1:-w])).sum(1) if w+1<97 else np.zeros(N+1,int)
def pv(x):   # p unilatérale de chaque texte sous la loi nulle (lignes 1..N)
    ref=np.sort(x[1:]); return 1-np.searchsorted(ref,x,side='left')/N
PD={m:pv(D[m]) for m in M}; PC={m:pv(C[m]) for m in M}; PV={m:pv(V[m]) for m in M}
for m in (7,):
    print(f"m=7 : doublets p={PD[7][0]:.5f} ; écart 7 p={PC[7][0]:.5f} ; largeur 21 p={PV[7][0]:.5f}")
    print("corrélation nulle D7/C7 :",np.corrcoef(D[7][1:],C[7][1:])[0,1].round(3)," D7/V7:",np.corrcoef(D[7][1:],V[7][1:])[0,1].round(3))
for name,combo in [("doublets × écart m",lambda m:PD[m]*PC[m]),("doublets × écart m × largeur 3m",lambda m:PD[m]*PC[m]*PV[m])]:
    sc=np.min(np.stack([combo(m) for m in M]),0)
    best=M[int(np.argmin([combo(m)[0] for m in M]))]
    print(f"{name}: K4 score min = {sc[0]:.2e} (m={best}) ; P(mélange <= K4) = {np.mean(sc[1:]<=sc[0]):.4f}")
