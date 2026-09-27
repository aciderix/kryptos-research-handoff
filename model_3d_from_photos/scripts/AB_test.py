#!/usr/bin/env python3
# A : clairs ENTIERS K1/K2/K3 (direct + inverse) comme flux de cle pour K4
# B : "erreurs" de Sanborn (IQLUSION/UNDERGRUUND/DESPARATLY, ?, lettres fautives) comme cle
AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KRYP="KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIB={}
for i,ch in enumerate("EASTNORTHEAST"): CRIB[21+i]=ch
for i,ch in enumerate("BERLINCLOCK"):   CRIB[63+i]=ch
POS=sorted(CRIB)
def I(a): return {c:i for i,c in enumerate(a)}
def vig(ct,key,alph,mode):
    Im=I(alph);n=len(alph);o=[]
    for i,c in enumerate(ct):
        ci=Im[c];ki=Im[key[i%len(key)]]
        pi=(ci-ki)%n if mode=="VIG" else (ki-ci)%n if mode=="BEA" else (ci+ki)%n
        o.append(alph[pi])
    return "".join(o)
def ak7(ct,pr,alph,mode,stream):
    Im=I(alph);n=len(alph);L=len(pr);P=[None]*len(ct)
    for i,c in enumerate(ct):
        k=pr[i%L] if i<7 else (P[i-7] if stream=='P' else ct[i-7])
        ci=Im[c];ki=Im[k]
        P[i]=alph[(ci-ki)%n if mode=="VIG" else (ki-ci)%n if mode=="BEA" else (ci+ki)%n]
    return "".join(P)
def sc(pt): return sum(1 for p in POS if pt[p]==CRIB[p])

# clairs
K1="BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION"
K2="ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLETHEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHISTHEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATIONONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTXLAYERTWO"
K3="SLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWERPARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPERLEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEEREDINTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLYDETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ"

def run(label, keys):
    res=[]
    for name,key in keys:
        for alph,an in [(AZ,"AZ"),(KRYP,"KRYP")]:
            for m in ["VIG","BEA","VAR"]:
                step=1 if len(key)<=40 else max(1,len(key)//120)
                for off in range(0,len(key),step):
                    rk=key[off:]+key[:off]
                    res.append((sc(vig(K4,rk,alph,m)),name,an,m,f"VIGoff{off}"))
                for s in ["P","C"]:
                    res.append((sc(ak7(K4,key,alph,m,s)),name,an,m,f"AK7{s}"))
    res.sort(reverse=True)
    print(f"\n=== {label} : TOP 10 ===")
    for r in res[:10]: print(f"  {r[0]:2d}/24  {r[1]:<16} {r[2]:<4} {r[3]:<3} {r[4]}")
    return res[0][0]

# A
Akeys=[("K1",K1),("K1rev",K1[::-1]),("K2",K2),("K2rev",K2[::-1]),("K3",K3),("K3rev",K3[::-1]),
       ("K1K2K3",K1+K2+K3),("K3K2K1",K3+K2+K1)]
bestA=run("A  clairs entiers", Akeys)

# B  erreurs
Bkeys=[("QU","QU"),("QUA","QUA"),("QUUA","QUUA"),("LOE","LOE"),("errlet","QUAE"),
       ("corr","LOEE"),("IQLUSION","IQLUSION"),("UNDERGRUUND","UNDERGRUUND"),
       ("DESPARATLY","DESPARATLY"),("QUUAQ","QUUAQ"),
       ("qmarks","QQQ"),  # placeholders de "?" (non-lettres) -> ignore de fait
       ("errwords","IQLUSIONUNDERGRUUNDDESPARATLY")]
bestB=run("B  erreurs Sanborn", Bkeys)

# null familial (7000 essais, 150 reps) pour reference
import random,statistics as st
random.seed(3);Im=I(KRYP);mx=[]
for _ in range(150):
    b=0
    for _ in range(7000):
        L=random.randint(4,20);rk="".join(random.choice(AZ) for _ in range(L))
        b=max(b,sc(vig(K4,rk,KRYP,"VIG")))
    mx.append(b)
print(f"\nnull familial max/7000 : moyenne {st.mean(mx):.2f}  95e centile {sorted(mx)[142]}  max {max(mx)}")
print(f"A meilleur={bestA}  B meilleur={bestB}  -> signal seulement si NETTEMENT au-dessus de ~{st.mean(mx):.1f}")
