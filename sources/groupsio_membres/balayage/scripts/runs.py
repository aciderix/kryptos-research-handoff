import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
c=np.array([ord(x)-65 for x in K4])
def maxrun(a, lags):
    # a: (N,97); returns (N,) max run length of equal consecutive lag-d differences, excluding diff 0? report both
    N,L=a.shape; best=np.zeros(N,int); bestnz=np.zeros(N,int)
    for d in lags:
        D=(a[:,d:]-a[:,:-d])%26   # (N, L-d)
        run=np.ones(N,int); r=np.ones(N,int); rnz=np.ones(N,int); runnz=np.zeros(N,int)
        # run of equal values in D
        cur=np.ones(N,int); curnz=np.where(D[:,0]!=0,1,0)
        mx=cur.copy(); mxnz=curnz.copy()
        for j in range(1,D.shape[1]):
            eq=D[:,j]==D[:,j-1]
            cur=np.where(eq,cur+1,1)
            curnz=np.where(D[:,j]!=0, np.where(eq,curnz+1,1), 0)
            mx=np.maximum(mx,cur); mxnz=np.maximum(mxnz,curnz)
        best=np.maximum(best,mx); bestnz=np.maximum(bestnz,mxnz)
    return best,bestnz
rng=np.random.default_rng(7)
for lags,name in [(range(4,5),"lag 4 seul"),(range(1,9),"lags 1-8"),(range(1,49),"lags 1-48")]:
    b,bn=maxrun(c[None,:],lags)
    N=100000
    R=rng.integers(0,26,(N,97))
    S=np.array([rng.permutation(c) for _ in range(N)])
    rb,rbn=maxrun(R,lags); sb,sbn=maxrun(S,lags)
    print(f"{name}: K4 max run (tous écarts) = {b[0]} ; (écart ≠ 0) = {bn[0]}")
    print(f"   aléatoire uniforme : P(run≠0 >= {bn[0]}) = {np.mean(rbn>=bn[0]):.4f} ; K4 mélangé : {np.mean(sbn>=bn[0]):.4f}")
    print(f"   (tous écarts) uniforme P(>= {b[0]}) = {np.mean(rb>=b[0]):.4f} ; mélangé {np.mean(sb>=b[0]):.4f}")
# where in K4
for d in range(1,49):
    D=(c[d:]-c[:-d])%26
    j=0
    while j<len(D):
        k=j
        while k+1<len(D) and D[k+1]==D[j]: k+=1
        if k-j+1>=4 and D[j]!=0: print("lag",d,"start",j,"len",k-j+1,"diff",D[j],K4[j:k+1+d])
        j=k+1
