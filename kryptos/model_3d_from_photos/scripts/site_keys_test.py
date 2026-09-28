#!/usr/bin/env python3
# Test: clés de flux tirees des mots GRAVES sur le site Kryptos + clair de K1-K3
# Vigenere (Vig/Beau/Var) x alphabets (AZ, KRYPTOS) x tous decalages
# + Autocle ecart-7 (amorce=motcle ; flux = clair OU chiffre)
# Score = nb de lettres justes sur les 24 cribs. Controle positif OBLIGATOIRE.

AZ   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
KRYP = "KRYPTOSABCDEFGHIJLMNQUVWXZ"

K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
# cribs 0-indexes
CRIB = {}
for i,ch in enumerate("EASTNORTHEAST"): CRIB[21+i]=ch   # 21..33
for i,ch in enumerate("BERLINCLOCK"):   CRIB[63+i]=ch   # 63..73
CRIBPOS = sorted(CRIB)
assert len(CRIBPOS)==24

def idx(alph): return {c:i for i,c in enumerate(alph)}

def vig_decrypt(ct, key, alph, mode):
    I=idx(alph); n=len(alph)
    out=[]
    for i,c in enumerate(ct):
        k=key[i%len(key)]
        ci=I[c]; ki=I[k]
        if mode=="VIG":  pi=(ci-ki)%n          # C=P+K -> P=C-K
        elif mode=="BEA":pi=(ki-ci)%n          # C=K-P -> P=K-C
        else:            pi=(ci+ki)%n          # variant: C=P-K -> P=C+K
        out.append(alph[pi])
    return "".join(out)

def autokey_decrypt(ct, primer, alph, mode, lag, stream):
    # keystream[i] = primer[i] for i<lag(<=len primer) else derived from pos i-lag
    # stream='P': keystream from recovered plaintext ; 'C': from ciphertext
    I=idx(alph); n=len(alph); L=len(primer)
    P=[None]*len(ct); ks=[None]*len(ct)
    for i,c in enumerate(ct):
        if i<lag:
            k = primer[i % L]                    # amorce (repetee si primer<lag)
        else:
            src = (P[i-lag] if stream=='P' else ct[i-lag])
            k = src
        ci=I[c]; ki=I[k]
        if mode=="VIG":  pi=(ci-ki)%n
        elif mode=="BEA":pi=(ki-ci)%n
        else:            pi=(ci+ki)%n
        P[i]=alph[pi]; ks[i]=k
    return "".join(P)

def score(pt):
    return sum(1 for p in CRIBPOS if pt[p]==CRIB[p])

# ---------- CONTROLE POSITIF : K1 avec PALIMPSEST ----------
K1CT="EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD"
K1PT="BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION"
ok=None
for alph,an in [(AZ,"AZ"),(KRYP,"KRYP")]:
    for mode in ["VIG","BEA","VAR"]:
        d=vig_decrypt(K1CT,"PALIMPSEST",alph,mode)
        if d==K1PT: ok=(an,mode)
print("CONTROLE POSITIF K1/PALIMPSEST ->", ok, "(doit etre non-None)")
if ok is None:
    print("!! controle echoue, arret"); raise SystemExit

# ---------- CANDIDATS ----------
K1W = ["BETWEEN","SUBTLE","SHADING","ABSENCE","LIGHT","LIES","NUANCE","IQLUSION","ILLUSION",
       "PALIMPSEST","SHADOW","SHADE","SUBTLESHADING","ABSENCEOFLIGHT"]
K2W = ["INVISIBLE","TOTALLYINVISIBLE","POSSIBLE","EARTHS","MAGNETIC","FIELD","MAGNETICFIELD",
       "INFORMATION","GATHERED","TRANSMITTED","UNDERGROUND","UNDERGRUUND","UNKNOWN","LOCATION",
       "LANGLEY","BURIED","SOMEWHERE","EXACT","ONLY","WW","WEBSTER","WILLIAMWEBSTER","LASTMESSAGE",
       "THIRTYEIGHT","DEGREES","MINUTES","SECONDS","NORTH","WEST","IDBYROWS","ABSCISSA","EARTHSMAGNETICFIELD"]
K3W = ["SLOWLY","DESPARATLY","DESPERATELY","REMAINS","PASSAGE","DEBRIS","DOORWAY","REMOVED",
       "TREMBLING","HANDS","BREACH","UPPERLEFT","CORNER","WIDENING","CANDLE","PEERED","HOTAIR",
       "CHAMBER","FLAME","FLICKER","PRESENTLY","DETAILS","ROOM","EMERGED","MIST","ANYTHING",
       "CANYOUSEEANYTHING","SEEANYTHING","TREMBLINGHANDS"]
SITE=["KRYPTOS","BERLIN","CLOCK","BERLINCLOCK","EAST","NORTHEAST","EASTNORTHEAST","SHADOWFORCES",
      "FORCES","LUCID","MEMORY","LUCIDMEMORY","VIRTUALLY","VIRTUALLYINVISIBLE","TISYOURPOSITION",
      "POSITION","DIGETAL","INTERPRETATU","SOS","RQ","SANBORN","SCHEIDT","EDSCHEIDT","CIA",
      "NORTHEASTBERLIN","BERLINWALL","WALL","COMPASS","LODESTONE"]
CANDS = list(dict.fromkeys(K1W+K2W+K3W+SITE))
print(f"{len(CANDS)} candidats")

# ---------- BALAYAGE ----------
results=[]
for key in CANDS:
    for alph,an in [(AZ,"AZ"),(KRYP,"KRYP")]:
        for mode in ["VIG","BEA","VAR"]:
            # Vigenere tous decalages
            for off in range(len(key)):
                rk=key[off:]+key[:off]
                s=score(vig_decrypt(K4,rk,alph,mode))
                results.append((s,key,an,mode,f"VIG off{off}"))
            # Autocle ecart-7, amorce=key, flux clair et chiffre
            for stream in ["P","C"]:
                s=score(autokey_decrypt(K4,key,alph,mode,7,stream))
                results.append((s,key,an,mode,f"AK7-{stream}"))

results.sort(reverse=True)
print("\n=== TOP 15 ===")
for r in results[:15]:
    print(f"  {r[0]:2d}/24  {r[1]:<18} {r[2]:<4} {r[3]:<3} {r[4]}")

# baseline aleatoire : cle = mot aleatoire meme longueur
import random
random.seed(1)
base=[]
for _ in range(3000):
    L=random.randint(4,12); rk="".join(random.choice(AZ) for _ in range(L))
    base.append(score(vig_decrypt(K4,rk,KRYP,"VIG")))
import statistics as st
print(f"\nbaseline aleatoire (3000 cles VIG/KRYP): moyenne {st.mean(base):.2f}  max {max(base)}  ecart-type {st.pstdev(base):.2f}")
top=results[0]
z=(top[0]-st.mean(base))/st.pstdev(base)
print(f"meilleur = {top[0]}/24  -> z = {z:.2f}  (>3 = signal ; sinon bruit)")
