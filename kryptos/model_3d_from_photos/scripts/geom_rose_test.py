#!/usr/bin/env python3
# TEST de l'idee "rose des vents" (transcript ChatGPT 2026-09-27) : le VECTEUR cellule-tableau ->
# cellule-chiffre (cross-panneau) donne un azimut par position ; ce flux d'azimuts est-il la CLE ?
# Addendum 12 a deja refute col/row/arc/z (features INTRA-panneau). Ici : feature CROSS-panneau
# (vecteur entre les deux panneaux superposables 28x31). Controle : cribs + null par shuffle.
import csv, math, random, statistics as st
AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KRYP="KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
CRIB={}
for i,ch in enumerate("EASTNORTHEAST"): CRIB[21+i]=ch
for i,ch in enumerate("BERLINCLOCK"):   CRIB[63+i]=ch
POS=sorted(CRIB)
D=lambda p:{ (int(r['row']),int(r['col'])):r for r in csv.DictReader(open(p)) }
cip=D("../data/master_cipher_letters.csv"); tab=D("../data/letters_tableau_panel.csv")
seq=[(24,c) for c in range(27,31)]+[(25,c) for c in range(31)]+[(26,c) for c in range(31)]+[(27,c) for c in range(31)]
assert len(seq)==97
# vecteur tableau->chiffre par position (x_m,y_m), azimut deg 0..360
az=[]; miss=0
for (r,c) in seq:
    a=tab.get((r,c)); b=cip.get((r,c))
    if not a or not b: az.append(None); miss+=1; continue
    dx=float(b['x_m'])-float(a['x_m']); dy=float(b['y_m'])-float(a['y_m'])
    az.append((math.degrees(math.atan2(dy,dx)))%360)
print(f"positions sans appariement: {miss}/97 ; azimut moyen={st.mean([a for a in az if a is not None]):.2f} deg")
vals=[a for a in az if a is not None]; print(f"azimut min={min(vals):.1f} max={max(vals):.1f} (etendue={max(vals)-min(vals):.1f} deg)")
def I(a): return {c:i for i,c in enumerate(a)}
def decrypt(ks,alph,mode):
    Im=I(alph); n=len(alph); out=[]
    for i,c in enumerate(K4):
        ci=Im[c]; ki=ks[i]%n
        pi=(ci-ki)%n if mode=="VIG" else (ki-ci)%n if mode=="BEA" else (ci+ki)%n
        out.append(alph[pi])
    return "".join(out)
def sc(pt): return sum(1 for p in POS if pt[p]==CRIB[p])
# familles de keystream tirees de l'azimut
def sect16(a): return int(((a%360)/22.5))%16 if a is not None else 0
def rankks(v,mod):
    idx=[i for i in range(97) if az[i] is not None]
    order=sorted(idx,key=lambda i:az[i]); ks=[0]*97
    for rk,i in enumerate(order): ks[i]=int(rk*mod/len(idx))%mod
    return ks
fams={
 "sector16":[sect16(a) for a in az],                     # 0..15 direct
 "sector16*? asAZ":[ (sect16(a))%26 for a in az],
 "deg/ (360/26)":[ int(((a or 0)%360)/(360/26))%26 for a in az],
 "az_rank26":rankks(az,26),
 "ENE/WSW pair (3/11)":[ (3 if (a is not None and abs(((a-67.5+180)%360)-180)<11.25) else 11) for a in az ],
}
res=[]
for fn,ks in fams.items():
    for alph,an in [(AZ,"AZ"),(KRYP,"KRYP")]:
        for m in ["VIG","BEA","VAR"]:
            res.append((sc(decrypt(ks,alph,m)),fn,an,m))
# offsets constants ENE=3 / WSW=11 (Caesar) - test explicite du transcript
for k in (3,11,23,15):  # +/-3, +/-11 mod26
    for alph,an in [(AZ,"AZ"),(KRYP,"KRYP")]:
        for m in ["VIG","BEA","VAR"]:
            res.append((sc(decrypt([k]*97,alph,m)),f"const{k}",an,m))
res.sort(reverse=True)
print("\n=== rose/azimut cross-panneau : TOP 12 ===")
for r in res[:12]: print(f"  {r[0]:2d}/24  {r[1]:<20} {r[2]:<4} {r[3]}")
# NULL : on garde la meme famille sector16 mais on permute l'affectation position->azimut
random.seed(7); best_null=[]
azc=[a for a in az]
for _ in range(20000):
    perm=azc[:]; random.shuffle(perm)
    ksn=[sect16(a)%26 for a in perm]
    s=max(sc(decrypt(ksn,AZ,"VIG")),sc(decrypt(ksn,KRYP,"VIG")))
    best_null.append(s)
print(f"\nnull (azimut permute, sector16, VIG): moyenne {st.mean(best_null):.2f} max {max(best_null)} P(>={res[0][0]})={sum(1 for v in best_null if v>=res[0][0])/len(best_null):.4f}")
print(f"meilleur reel = {res[0][0]}/24 ; significatif seulement si >> {max(best_null)}")
