import bpy, numpy as np, math, csv
from mathutils import Vector
from mathutils.bvhtree import BVHTree
MPU=0.02935
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
cribset=set(range(21,34))|set(range(63,74))

# ---- recover Option A transform: model(x,y)->world(E,N) ----
cow2=np.load("copper_verts.npy")[:,:2].astype(float)
def centroid(fn):
    xs=[];ys=[]
    for d in csv.DictReader(open(fn)): xs.append(float(d['x_m'])/MPU); ys.append(float(d['y_m'])/MPU)
    return np.array([np.mean(xs),np.mean(ys)])
tabc=centroid("grid_tableau_hi.csv")
c2=cow2.mean(0); X=cow2-c2; cov=X.T@X/len(X); w,v=np.linalg.eigh(cov); prin=v[:,np.argmax(w)]
t=X@prin; tipA=cow2[t.argmin()]; tipB=cow2[t.argmax()]
tipNE,tipSW=(tipA,tipB) if np.linalg.norm(tabc-tipA)<np.linalg.norm(tabc-tipB) else (tipB,tipA)
a=tipNE-tipSW; target=np.array([math.sin(math.radians(50.6)),math.cos(math.radians(50.6))])
ang=math.atan2(target[1],target[0])-math.atan2(a[1],a[0])
M=np.array([[math.cos(ang),-math.sin(ang)],[math.sin(ang),math.cos(ang)]])  # option A, det=+1
# world axes expressed in MODEL coords: North_model = M^T @ (0,1); East_model = M^T @ (1,0)
North_m=M.T@np.array([0,1.0]); East_m=M.T@np.array([1.0,0])
North=np.array([North_m[0],North_m[1],0.0]); East=np.array([East_m[0],East_m[1],0.0]); Up=np.array([0,0,1.0])
print("North_model=",np.round(North,3)," East_model=",np.round(East,3))

# ---- BVH copper+tree ----
bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
verts=[];tris=[]
def add(nm):
    o=bpy.data.objects[nm]; me=o.data; Mm=np.array(o.matrix_world)
    n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
    W=(np.c_[co,np.ones(n)]@Mm.T)[:,:3]; base=len(verts); verts.extend(Vector(p) for p in W)
    for p in me.polygons:
        vs=[base+i for i in p.vertices]
        for i in range(1,len(vs)-1): tris.append((vs[0],vs[i],vs[i+1]))
add("Kryptos.Part.CopperSheet"); add("Kryptos.Part.PetrifedTree")
bvh=BVHTree.FromPolygons(verts,tris)
rows={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    r=int(d['row']); rows.setdefault(r,{})[int(d['col'])]=np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU
pos=[(24,27+i) for i in range(4)]+[(r,c) for r in (25,26,27) for c in range(31)]
P=[rows[r][c] for (r,c) in pos]
suns={"Inaug 15h":(228.4,19.9),"Inaug 12h":(182.3,35.3),"Berlin 13h":(197.2,32.3)}
for sname,(az,alt) in suns.items():
    azr,altr=math.radians(az),math.radians(alt)
    sundir=East*math.sin(azr)*math.cos(altr)+North*math.cos(azr)*math.cos(altr)+Up*math.sin(altr); sundir/=np.linalg.norm(sundir)
    sh=[bvh.ray_cast(Vector(p)+0.3*Vector(sundir),Vector(sundir),2000.0)[0] is not None for p in P]
    idx=[i for i,s in enumerate(sh) if s]; ov=len(set(idx)&cribset)
    ncrib=len(idx&cribset) if False else ov
    non=len([i for i in idx if i not in cribset])
    print(f"{sname}: shadowed {len(idx)}/97 | crib-shadowed {ov}/24 ({ov/24*100:.0f}%) vs non-crib {non}/73 ({non/73*100:.0f}%)")
print("\nInterprétation: si crib% ~ non-crib% et couverture haute -> pas de masque (artefact). Masque réel = crib% >> non-crib% avec couverture modérée.")
