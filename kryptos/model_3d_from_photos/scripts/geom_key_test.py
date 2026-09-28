#!/usr/bin/env python3
# Cle GEOMETRIQUE, 1:1 preserve : le decalage de chaque lettre vient de sa position physique.
import csv, math, statistics as st
AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KRYP="KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIB={}
for i,ch in enumerate("EASTNORTHEAST"): CRIB[21+i]=ch
for i,ch in enumerate("BERLINCLOCK"):   CRIB[63+i]=ch
POS=sorted(CRIB)

# charge grille
cells={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    cells[(int(d['row']),int(d['col']))]=d
# mapping K4 -> cellules physiques : row24 col27..30 puis rows 25,26,27 col0..30
seq=[(24,c) for c in range(27,31)]+[(25,c) for c in range(31)]+[(26,c) for c in range(31)]+[(27,c) for c in range(31)]
assert len(seq)==97, len(seq)
# verifie 1:1 : pos63-73 doit exister
geo=[]  # par position K4 : dict de features
for i,(r,c) in enumerate(seq):
    d=cells.get((r,c))
    if d is None:  # cellule manquante -> abandon mapping propre
        geo.append({'row':r,'col':c,'arc':0.0,'z':0.0,'x':0.0,'y':0.0}); continue
    geo.append({'row':r,'col':c,'arc':float(d['arc_s_m']),'z':float(d['z_from_base_m']),
                'x':float(d['x_m']),'y':float(d['y_m'])})
missing=sum(1 for i,(r,c) in enumerate(seq) if (r,c) not in cells)
print(f"cellules manquantes dans le mapping : {missing}/97")

def I(a): return {c:i for i,c in enumerate(a)}
def decrypt(keystream, alph, mode):
    Im=I(alph); n=len(alph); out=[]
    for i,c in enumerate(K4):
        ci=Im[c]; ki=keystream[i]%n
        pi=(ci-ki)%n if mode=="VIG" else (ki-ci)%n if mode=="BEA" else (ci+ki)%n
        out.append(alph[pi])
    return "".join(out)
def sc(pt): return sum(1 for p in POS if pt[p]==CRIB[p])

# familles de keystream geometriques (indices de decalage 0..25)
def q(vals):  # quantise une valeur continue en 0..25 par rang
    order=sorted(range(len(vals)), key=lambda i:vals[i])
    ks=[0]*len(vals)
    for rank,i in enumerate(order): ks[i]=int(rank*26/len(vals))%26
    return ks
arcs=[g['arc'] for g in geo]; zs=[g['z'] for g in geo]
fams={
 "col":[g['col']%26 for g in geo],
 "row":[g['row']%26 for g in geo],
 "row+col":[(g['row']+g['col'])%26 for g in geo],
 "row-col":[(g['row']-g['col'])%26 for g in geo],
 "col-row":[(g['col']-g['row'])%26 for g in geo],
 "2col":[(2*g['col'])%26 for g in geo],
 "arc_rank":q(arcs),
 "z_rank":q(zs),
 "arc+z":[ (q(arcs)[i]+q(zs)[i])%26 for i in range(97)],
}
# variante "tableau a la position" : keystream = lettre du tableau keyed a (row+col)
tab_rc=[ I(KRYP)[KRYP[(g['row']+g['col'])%26]] for g in geo ]  # = (row+col)%26, deja couvert
# variante lettre-tableau en index AZ
fams["tableauKRYP@rc_asAZ"]=[ I(AZ)[KRYP[(g['row']+g['col'])%26]] for g in geo ]

res=[]
for fname,ks in fams.items():
    for alph,an in [(AZ,"AZ"),(KRYP,"KRYP")]:
        for m in ["VIG","BEA","VAR"]:
            res.append((sc(decrypt(ks,alph,m)),fname,an,m))
res.sort(reverse=True)
print("\n=== cles geometriques : TOP 12 ===")
for r in res[:12]: print(f"  {r[0]:2d}/24  {r[1]:<18} {r[2]:<4} {r[3]}")

# null : keystream aleatoire par position (famille contrainte -> ref binomiale)
import random; random.seed(5); rr=[]
for _ in range(20000):
    ks=[random.randrange(26) for _ in range(97)]
    rr.append(sc(decrypt(ks,KRYP,"VIG")))
print(f"\nnull (keystream aleatoire/position): moyenne {st.mean(rr):.2f}  max {max(rr)}  P(>=6)={sum(1 for v in rr if v>=6)/len(rr):.4f}")
print(f"meilleur geometrique = {res[0][0]}/24  (significatif si >> {max(rr)})")
