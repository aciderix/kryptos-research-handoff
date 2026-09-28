import numpy as np, csv, math
MPU=0.02935
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
cribs=[(21,33,"EASTNORTHEAST"),(63,73,"BERLINCLOCK")]
A=lambda c: ord(c)-65; Lf=lambda n: chr(65+(n%26))
KEY="KRYPTOSABCDEFGHIJLMNQUVWXZ"
tabletter=lambda r,c: KEY[(c+r)%26]

# K4 cipher centroids (model units)
rows={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    r=int(d['row']); rows.setdefault(r,{})[int(d['col'])]=np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU
pos=[(24,27+i) for i in range(4)]+[(r,c) for r in (25,26,27) for c in range(31)]
CIP=np.array([rows[r][c] for (r,c) in pos])   # 97x3
# tableau centroids + identity
TAB=[]; TABid=[]
for d in csv.DictReader(open("grid_tableau_hi.csv")):
    r=int(d['row']); c=int(d['col'])
    TAB.append(np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU); TABid.append(tabletter(r,c))
TAB=np.array(TAB)

center=np.vstack([CIP,TAB]).mean(0)
extent=np.linalg.norm(np.vstack([CIP,TAB]).ptp(0))
def project(pts,eye):
    view=center-eye; view=view/np.linalg.norm(view)
    up=np.array([0,0,1.0]); right=np.cross(view,up); right/=np.linalg.norm(right); up=np.cross(right,view)
    rel=pts-eye
    z=rel@view
    x=(rel@right)/z; y=(rel@up)/z   # perspective divide
    return np.c_[x,y],z

def decrypt(ci,key,mode):
    return Lf(A(ci)-A(key)) if mode=="VIG" else Lf(A(key)-A(ci))
def crib_score(pt):
    ok=tot=0
    for a,b,w in cribs:
        for j,ch in enumerate(w):
            tot+=1
            if pt[a+j]==ch: ok+=1
    return ok,tot

rng=np.random.default_rng(0)
best=(-1,)
# sweep eye points on spheres of several radii, full sphere
tried=0; overlap_seen=0
for R in (1.5,2.5,4.0):
    r_eye=R*extent
    for _ in range(4000):
        u=rng.normal(size=3); u/=np.linalg.norm(u); eye=center+r_eye*u
        cip2,zc=project(CIP,eye); tab2,zt=project(TAB,eye)
        # for each cipher letter, nearest tableau in projection
        pt=[]; used=0
        # scale threshold by projected spread
        thr=0.02* (np.abs(cip2).max()+1e-9)
        for i in range(97):
            dd=((tab2-cip2[i])**2).sum(1)
            j=int(dd.argmin())
            if dd[j]<thr*thr:
                used+=1; pt.append(None); pt[-1]=(TABid[j])
            else: pt.append(None)
        if used<20:
            tried+=1; continue
        overlap_seen+=1
        for mode in ("VIG","BEA"):
            dec=[]
            for i in range(97):
                dec.append(decrypt(K4[i],pt[i],mode) if pt[i] else ".")
            dec="".join(dec)
            ok,tot=crib_score(dec)
            if ok>best[0]: best=(ok,tot,used,mode,R,dec)
        tried+=1
print(f"viewpoints tried={tried}, with >=20 panel overlap={overlap_seen}")
if best[0]>=0:
    ok,tot,used,mode,R,dec=best
    print(f"BEST: cribs {ok}/{tot} (overlap {used}/97, mode {mode}, R={R})")
    print("  EAST[21:34]=",dec[21:34]," BERLIN[63:74]=",dec[63:74])
else:
    print("no viewpoint produced >=20 cipher/tableau overlaps -> panels do not visually superimpose from any tested eye point")
# random crib-hit baseline
hits=[sum(1 for a,b,w in cribs for j,ch in enumerate(w) if Lf(rng.integers(26))==ch) for _ in range(20000)]
print("random crib hits: mean %.2f max %d /24"%(np.mean(hits),max(hits)))
