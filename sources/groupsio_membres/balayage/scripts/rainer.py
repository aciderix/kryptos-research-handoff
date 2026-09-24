import random, itertools
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
PT={i:c for i,c in zip(range(21,34),"EASTNORTHEAST")}; PT.update({i:c for i,c in zip(range(63,74),"BERLINCLOCK")})
def ok(ct,cls):
    # cls: fonction position -> classe ; chaque classe = substitution simple quelconque (bijection partielle)
    m={}; inv={}
    for i,p in PT.items():
        k=cls(i); c=ct[i]
        if m.get((k,c),p)!=p or inv.get((k,p),c)!=c: return False
        m[(k,c)]=p; inv[(k,p)]=c
    return True
def patterns():
    for per in range(1,13):
        for nc in (2,3):
            # toutes les affectations des résidus à nc classes (à renommage près, grossièrement)
            for a in itertools.product(range(nc),repeat=per):
                if a[0]!=0 or len(set(a))!=nc: continue
                yield per,nc,a
P=list(patterns()); print("motifs",len(P))
res={}
for per,nc,a in P:
    if ok(K4,lambda i:a[i%per]): res.setdefault((per,nc),[]).append("".join(map(str,a)))
print("rainer 0011 :", ok(K4,lambda i:(0,0,1,1)[i%4]), " décalage 1100/0110/1001 :", [ok(K4,lambda i,s=s:(0,0,1,1)[(i+s)%4]) for s in range(4)])
for k,v in sorted(res.items()): print("K4 compatible période",k[0],"classes",k[1],":",len(v),"motifs", v[:6])
random.seed(3); tot=0; n=200
cnt={}
for _ in range(n):
    ct="".join(random.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZ") for _ in range(97))
    for per,nc,a in P:
        if ok(ct,lambda i:a[i%per]): cnt[(per,nc)]=cnt.get((per,nc),0)+1
print("chiffrés aléatoires : nombre moyen de motifs compatibles par (période, classes) :", {k:round(v/n,2) for k,v in sorted(cnt.items())})
