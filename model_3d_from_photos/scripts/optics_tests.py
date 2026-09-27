import numpy as np, csv, math
MPU=0.02935
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
cribs=[(21,33,"EASTNORTHEAST"),(63,73,"BERLINCLOCK")]
A=lambda c: ord(c)-65; Lf=lambda n: chr(65+(n%26))
KEY="KRYPTOSABCDEFGHIJLMNQUVWXZ"; tabletter=lambda r,c: KEY[(c+r)%26]

# --- load K4 letters with geometry (model units) ---
cip={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    r=int(d['row']); cip.setdefault(r,{})[int(d['col'])]=dict(
        x=float(d['x_m'])/MPU,y=float(d['y_m'])/MPU,z=float(d['z_m'])/MPU,s=float(d['arc_s_units']))
posrc=[(24,27+i) for i in range(4)]+[(r,c) for r in (25,26,27) for c in range(31)]
K=[cip[r][c] for (r,c) in posrc]           # 97 dicts
Kxyz=np.array([[k['x'],k['y'],k['z']] for k in K])
Ks=np.array([k['s'] for k in K]); Kz=Kxyz[:,2]
# tableau letters with geometry + identity
TAB=[]
for d in csv.DictReader(open("grid_tableau_hi.csv")):
    r=int(d['row']); c=int(d['col'])
    TAB.append(dict(s=float(d['arc_s_units']),z=float(d['z_m'])/MPU,r=r,c=c,L=tabletter(r,c)))
Tarr=np.array([[t['s'],t['z']] for t in TAB]); TL=[t['L'] for t in TAB]
cA,cB=np.load("circles.npy")

def decrypt(ci,key,mode): return Lf(A(ci)-A(key)) if mode=="VIG" else Lf(A(key)-A(ci))
def cribscore(pt):
    ok=tot=0
    for a,b,w in cribs:
        for j,ch in enumerate(w):
            tot+=1
            if a+j<len(pt) and pt[a+j]==ch: ok+=1
    return ok,tot
rng=np.random.default_rng(0)
randbase=[sum(1 for a,b,w in cribs for j,ch in enumerate(w) if Lf(rng.integers(26))==ch) for _ in range(20000)]
print("random crib baseline: mean %.2f max %d /24"%(np.mean(randbase),max(randbase)))

# ===== IDÉE 5 : coaxial cylinder wrap (washing machine) =====
# both arcs -> one cylinder; pair K4 letter with tableau letter by (arc_s,height), direct & flipped
smax_t=Tarr[:,0].max()
print("\n=== IDÉE 5 : cylindre coaxial ===")
for flip in (False,True):
    for mode in ("VIG","BEA"):
        pt=[]
        for i in range(97):
            s = (smax_t-Ks[i]) if flip else Ks[i]
            d=((Tarr[:,0]-s)**2 + (Tarr[:,1]-Kz[i])**2)
            j=int(d.argmin()); key=TL[j]
            pt.append(decrypt(K4[i],key,mode))
        pt="".join(pt); ok,tot=cribscore(pt)
        print(f"  flip={flip} {mode}: cribs {ok}/{tot}  EAST={pt[21:34]} BERLIN={pt[63:74]}")

# ===== #5 : geometric reading orders -> écart-7 signal =====
def e7(seq): return sum(1 for i in range(len(seq)-7) if seq[i]==seq[i+7])
print("\n=== #5 : ordres de lecture géométriques -> écart-7 ===")
carved=e7(K4); print("  carved order écart-7 =",carved)
# baseline distribution of écart-7 over random permutations of K4
perm_e7=[e7("".join(K4[i] for i in rng.permutation(97))) for _ in range(20000)]
import numpy as _np
mu,sd=_np.mean(perm_e7),_np.std(perm_e7)
print(f"  random-permutation écart-7: mean {mu:.2f} sd {sd:.2f} (carved {carved} -> z={(carved-mu)/sd:.2f})")
keys={
 "z_desc": -Kz, "z_asc": Kz,
 "angle_cB": np.arctan2(Kxyz[:,1]-cB[1],Kxyz[:,0]-cB[0]),
 "x": Kxyz[:,0], "y": Kxyz[:,1], "arc_s": Ks,
 "dist_infl": np.hypot(Kxyz[:,0]-(cA[0]+cB[0])/2,Kxyz[:,1]-(cA[1]+cB[1])/2),
 "dist_tabctr": np.hypot(Kxyz[:,0]-cA[0],Kxyz[:,1]-cA[1]),
}
for name,kv in keys.items():
    order=np.argsort(kv,kind="stable")
    seq="".join(K4[i] for i in order)
    e=e7(seq); z=(e-mu)/sd
    flag=" <==" if abs(z)>3 else ""
    print(f"  order {name:12s}: écart-7 = {e:2d}  (z={z:+.2f}){flag}")
