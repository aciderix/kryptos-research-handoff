import bpy, numpy as np, math, csv, random
from mathutils import Vector
from mathutils.bvhtree import BVHTree

MPU=0.02935
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
cribs=[(21,33,"EASTNORTHEAST"),(63,73,"BERLINCLOCK")]
A=lambda c: ord(c)-65; Lf=lambda n: chr(65+(n%26))

# --- load mesh + BVH (model units) ---
bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
obj=bpy.data.objects["Kryptos.Part.CopperSheet"]; me=obj.data
n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
M=np.array(obj.matrix_world); cow=(np.c_[co,np.ones(n)]@M.T)[:,:3]
verts=[Vector(p) for p in cow]; tris=[]
for p in me.polygons:
    vs=list(p.vertices)
    for i in range(1,len(vs)-1): tris.append((vs[0],vs[i],vs[i+1]))
bvh=BVHTree.FromPolygons(verts,tris)
cA,cB=np.load("circles.npy")  # tableau, cipher centers (model units)

# --- K4 letter positions (model units) from meters CSV, rows 24-27 ---
rows={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    r=int(d['row']); rows.setdefault(r,{})[int(d['col'])]=np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU
# K4 index -> (row,col)
pos=[]
for i in range(4): pos.append((24,27+i))
for r in (25,26,27):
    for c in range(31): pos.append((r,c))
P=[rows[r][c] for (r,c) in pos]  # 97 positions

# tableau letters: reconstruct grid + centroids (model units) from tableau CSV
trows={}
for d in csv.DictReader(open("grid_tableau_hi.csv")):
    r=int(d['row']); trows.setdefault(r,{})[int(d['col'])]=np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU
# flat list of tableau centroids with (r,c)
tcent=[];
for r in trows:
    for c in trows[r]: tcent.append((r,c,trows[r][c]))
tc_xyz=np.array([t[2] for t in tcent])
KEY="KRYPTOSABCDEFGHIJLMNQUVWXZ"
def tableau_letter(r,c): return KEY[(c+r)%26]  # standard keyed rotation (approx physical)

# --- cast along horizontal radial normal from cipher-arc center cB ---
def normal_at(p):
    d=np.array([p[0]-cB[0],p[1]-cB[1],0.0]); d/=np.linalg.norm(d); return d
def cast(p,direction):
    o=Vector((p[0],p[1],p[2])); d=Vector(direction)
    hit=bvh.ray_cast(o+0.5*d, d, 400.0)  # skip own panel by starting a bit off
    return hit  # (location, normal, index, dist)

def which_panel(loc):
    # nearest tableau centroid distance vs cipher: use assignment by nearest arc
    xy=np.array([loc[0],loc[1]])
    dB=abs(np.hypot(xy[0]-cB[0],xy[1]-cB[1])-cB[2])
    dA=abs(np.hypot(xy[0]-cA[0],xy[1]-cA[1])-cA[2])
    return "tableau" if dA<dB else "cipher"

results={}
for sign,lbl in ((+1,"out"),(-1,"in")):
    hits=0; tab_hits=0; mapping=[]
    for i,p in enumerate(P):
        nrm=normal_at(p)*sign
        h=cast(p,nrm)
        if h[0] is None: mapping.append(None); continue
        hits+=1
        if which_panel(h[0])=="tableau":
            tab_hits+=1
            loc=np.array([h[0][0],h[0][1],h[0][2]])
            j=np.argmin(((tc_xyz-loc)**2).sum(1))
            r,c,_=tcent[j]; mapping.append((r,c))
        else: mapping.append(("cipher",))
    results[lbl]=(hits,tab_hits,mapping)
    print(f"normal {lbl}: hits={hits}/97 tableau_hits={tab_hits}")

# If enough tableau hits, build key and test cribs
def test_mapping(mapping,mode):
    pt=[None]*97; used=0
    for i in range(97):
        m=mapping[i]
        if not m or len(m)!=2: pt[i]="."; continue
        r,c=m; key=tableau_letter(r,c); used+=1
        cc=A(K4[i]); k=A(key)
        pt[i]=Lf(cc-k) if mode=="VIG" else Lf(k-cc)
    pt="".join(pt)
    ok=tot=0
    for a,b,w in cribs:
        for j,ch in enumerate(w):
            tot+=1
            if pt[a+j]==ch: ok+=1
    return pt,ok,tot,used
for lbl in results:
    hits,tab_hits,mapping=results[lbl]
    if tab_hits<10:
        print(f"[{lbl}] too few tableau hits ({tab_hits}) -> normals of the two panels do NOT face each other; overlay-by-normal geometrically absent.")
        continue
    for mode in ("VIG","BEA"):
        pt,ok,tot,used=test_mapping(mapping,mode)
        print(f"[{lbl}/{mode}] cribs {ok}/{tot} (used {used} mapped)  EAST->{pt[21:34]} BERLIN->{pt[63:74]}")
