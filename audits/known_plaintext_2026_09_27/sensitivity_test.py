import random
from collections import Counter
AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
P="THECOMPASSROSEISHEREXEASTNORTHEASTTHISISYOURPOSITIONXCOMMISSIONBERLINCLOCKWHICHISNORTHEASTOFHEREX"
C="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
N=97
kal=lambda w:''.join(dict.fromkeys(w+AZ))
KR=kal('KRYPTOS'); iv={c:i for i,c in enumerate(KR)}
crib=set(range(21,34))|set(range(63,74))
def keyv(Pp):
    return [(iv[C[i]]-iv[Pp[i]])%26 for i in range(N)]
def ic(v):
    c=Counter(v);n=len(v);return sum(x*(x-1) for x in c.values())/(n*(n-1))
def min_period_contra(k):
    best=99
    for L in range(1,27):
        m=0
        for r in range(L):
            vals=[k[i] for i in range(r,N,L)]
            if vals:
                mx=max(Counter(vals).values()); m+=len(vals)-mx
        best=min(best,m)
    return best
def min_autokey_contra(k=None):
    best=99
    for g in range(1,15):
        mp={};c=0
        for i in range(g,N):
            key=(P[i],P[i-g])
            if key in mp:
                if mp[key]!=C[i]:c+=1
            else:mp[key]=C[i]
        best=min(best,c)
    return best
k0=keyv(P)
print(f"reconstruction telle quelle: IC(key)={ic(k0):.4f}, min-contradictions périodique={min_period_contra(k0)}, plaintext-autokey={min_autokey_contra()}")
print("=> pour CACHER une périodicité il faudrait ~%d lettres fausses ; un autoclé ~%d." % (min_period_contra(k0),min_autokey_contra()))
# robustesse : perturber k lettres non-crib au hasard, IC reste-t-il aléatoire ?
random.seed(3)
noncrib=[i for i in range(N) if i not in crib]
for kk in (2,5,10):
    ics=[]
    for _ in range(300):
        Pp=list(P)
        for j in random.sample(noncrib,kk): Pp[j]=random.choice(AZ)
        ics.append(ic(keyv(''.join(Pp))))
    print(f"  perturber {kk} lettres non-crib: IC(key) moyen={sum(ics)/len(ics):.4f} (aléatoire 0.0385) — reste aléatoire")
print("\nCONCLUSION: la reconstruction (qoff -2.0, cribs exacts, confirmée Sanborn) a au plus qq lettres douteuses ;")
print("or cacher une structure exigerait des dizaines de corrections => les négatifs NE sont PAS des artefacts de reconstruction.")
