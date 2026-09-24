import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(12)
N=20000
S=np.array([rng.permutation(c) for _ in range(N)]+[c])   # dernière ligne = K4
onehot=np.zeros((S.shape[0],97,26)); 
for L in range(26): onehot[:,:,L]=(S==L)
tot=onehot.sum(1)  # (N+1,26) identique pour tous
stats={}
for k in range(2,11):
    g=np.arange(97)*k//97
    size=np.bincount(g)
    O=np.stack([onehot[:,g==j,:].sum(1) for j in range(k)],1)  # (N+1,k,26)
    E=tot[:,None,:]*size[None,:,None]/97
    chi=((O-E)**2/np.where(E>0,E,1)).sum((1,2))
    stats[f"blocs{k}"]=chi
# dispersion
pos=np.arange(97)
disp=np.zeros(S.shape[0])
for L in range(26):
    m=onehot[:,:,L]; n=m.sum(1)
    mu=(m*pos).sum(1)/np.maximum(n,1); var=(m*(pos-mu[:,None])**2).sum(1)/np.maximum(n,1)
    disp+=np.where(n>=3,-np.sqrt(var),0)
stats["dispersion"]=disp
# p-values (rang) pour chaque stat et chaque texte
P={}
for name,v in stats.items():
    order=np.argsort(np.argsort(-v))   # rang 0 = plus grand
    P[name]=(order+1)/len(v)
    print(f"{name}: p(K4) = {P[name][-1]:.4f}")
minp=np.min(np.stack(list(P.values())),0)
print("p minimale K4 =", minp[-1], "; fraction des mélanges avec p minimale <= :", np.mean(minp[:-1]<=minp[-1]))
