from decimal import Decimal, getcontext
import math, random
getcontext().prec=260
AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
P="THECOMPASSROSEISHEREXEASTNORTHEASTTHISISYOURPOSITIONXCOMMISSIONBERLINCLOCKWHICHISNORTHEASTOFHEREX"
C="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
N=97
kal=lambda w:''.join(dict.fromkeys(w+AZ))
def keyv(a,m):
    iv={c:i for i,c in enumerate(a)};o=[]
    for i in range(N):
        cv=iv[C[i]];pv=iv[P[i]]
        o.append((cv-pv)%26 if m=='VIG' else (cv+pv)%26 if m=='BEA' else (pv-cv)%26)
    return o
# base-26 fractional digits of a Decimal in (0,1), D digits
def b26(fr,D):
    out=[]
    for _ in range(D):
        fr*=26; d=int(fr); out.append(d); fr-=d
    return out
def isqrt_frac_b26(n,D):
    # floor(sqrt(n)*26^D) via isqrt, then take fractional base-26 digits
    scaled=math.isqrt(n*(26**(2*D)))
    ip=scaled//(26**D); frac=scaled-(ip*(26**D))
    out=[]
    for k in range(D-1,-1,-1):
        p=26**k; d=frac//p; out.append(d); frac-=d*p
    return out
PI=Decimal("3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745")
E =Decimal("2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193200305992181741359662904357290033429526059563073813232862794349076323382988075319525101901157383418793070215408914")
consts={}
consts['pi']=b26(PI-3,150)
consts['e']=b26(E-2,150)
for name,n in (('sqrt2',2),('sqrt3',3),('sqrt5',5)):
    consts[name]=isqrt_frac_b26(n,150)
# phi=(1+sqrt5)/2 ; frac = (sqrt5-1)/2
s5=math.isqrt(5*(26**300)); # floor(sqrt5 * 26^150)
val=(s5-(26**150))//2  # (sqrt5-1)/2 * 26^150 approx
frac=val
phid=[]
for k in range(149,-1,-1):
    p=26**k; d=frac//p; phid.append(d%26); frac-=d*p
consts['phi']=phid
def matches(key,seq):
    best=(0,-1)
    for off in range(0,len(seq)-N+1):
        m=sum(1 for i in range(N) if key[i]==seq[off+i]%26)
        if m>best[0]:best=(m,off)
    return best
print("=== keystream vs base-26 of constants (best matches /97) ===")
res=[]
for an,a in {'AZ':AZ,'KRYPTOS':kal('KRYPTOS')}.items():
  for m in ('VIG','BEA','VAR'):
    key=keyv(a,m)
    for cn,seq in consts.items():
        mm,off=matches(key,seq); res.append((mm,an,m,cn,off))
res.sort(reverse=True)
for mm,an,m,cn,off in res[:8]: print(f"  match={mm}/97  key[{an},{m}] vs {cn}@off{off}")
# null: random keys vs same constants
random.seed(1); nm=0
for _ in range(2000):
    k=[random.randrange(26) for _ in range(N)]
    b=max(matches(k,seq)[0] for seq in consts.values())
    nm=max(nm,b)
print(f"NULL (random key, best over constants, 2000 trials): max match={nm}/97")
