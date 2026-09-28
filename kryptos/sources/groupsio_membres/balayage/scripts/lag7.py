import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4]); rng=np.random.default_rng(14)
def lags(a): return np.array([(a[:,d:]==a[:,:-d]).sum(1) for d in range(1,49)]).T
N=100000; S=np.array([rng.permutation(c) for _ in range(N)])
L=lags(S); k=lags(c[None,:])[0]
print("K4 par écart:", dict(zip(range(1,49),k)))
print("écart 7 : K4",k[6],"; moyenne",L[:,6].mean().round(2),"; P(>=)",np.mean(L[:,6]>=k[6]))
# excès max standardisé sur tous les écarts
mu=L.mean(0); sd=L.std(0)
zk=((k-mu)/sd).max(); zs=((L-mu)/sd).max(1)
print("z max K4",zk.round(2),"à l'écart",np.argmax((k-mu)/sd)+1,"; P(z max mélange >=)",np.mean(zs>=zk))
print("paires à l'écart 7 :",[(i,K4[i]) for i in range(90) if K4[i]==K4[i+7]])
# multiples de 7
m7=sum(k[d-1] for d in (7,14,21,28,35,42)); m7s=sum(L[:,d-1] for d in (7,14,21,28,35,42))
print("écarts multiples de 7 (7..42): K4",m7,"moyenne",m7s.mean().round(1),"P",np.mean(m7s>=m7))
