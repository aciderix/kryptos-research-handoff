import bpy, numpy as np, math, csv
from mathutils import Vector
from mathutils.bvhtree import BVHTree
MPU=0.02935
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
cribset=set(range(21,34))|set(range(63,74))

bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
# BVH from copper + tree (shadow casters)
tris=[]; verts=[]
def add(objname):
    o=bpy.data.objects[objname]; me=o.data; M=np.array(o.matrix_world)
    n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
    w=(np.c_[co,np.ones(n)]@M.T)[:,:3]; base=len(verts)
    verts.extend(Vector(p) for p in w)
    for p in me.polygons:
        vs=[base+i for i in p.vertices]
        for i in range(1,len(vs)-1): tris.append((vs[0],vs[i],vs[i+1]))
add("Kryptos.Part.CopperSheet"); add("Kryptos.Part.PetrifedTree")
bvh=BVHTree.FromPolygons(verts,tris)

cA,cB=np.load("circles.npy")
# K4 letter positions (model units)
rows={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    r=int(d['row']); rows.setdefault(r,{})[int(d['col'])]=np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU
pos=[(24,27+i) for i in range(4)]+[(r,c) for r in (25,26,27) for c in range(31)]
P=[rows[r][c] for (r,c) in pos]

Nm=np.array([cA[0]-cB[0],cA[1]-cB[1],0.0]); Nm/=np.linalg.norm(Nm)   # model dir of real North
Up=np.array([0,0,1.0])
suns={"Inaug15h(az228,alt20)":(228.4,19.9),"Inaug12h(az182,alt35)":(182.3,35.3),
      "Berlin13h(az197,alt32)":(197.2,32.3),"Berlin15h(az228,alt19)":(227.5,18.9)}
def shadowed(P, sundir):
    out=[]
    for i,p in enumerate(P):
        o=Vector(p)+0.3*Vector(sundir)
        h=bvh.ray_cast(o,Vector(sundir),2000.0)
        out.append(h[0] is not None)
    return out
def report(name,sh):
    idx=[i for i,s in enumerate(sh) if s]
    overlap=len(set(idx)&cribset)
    word="".join(K4[i] if s else "." for i,s in enumerate(sh))
    print(f"  {name}: shadowed={len(idx)}/97  crib-overlap={overlap}/24  cribpos-shadowed={sorted(set(idx)&cribset)}")
    return idx

for h in (+1,-1):
    Em=np.array([Nm[1]*h,-Nm[0]*h,0.0])  # East (handedness h)
    print(f"\n=== handedness h={h} (East={np.round(Em,2)}) ===")
    for sname,(az,alt) in suns.items():
        azr,altr=math.radians(az),math.radians(alt)
        sundir=Em*math.sin(azr)*math.cos(altr)+Nm*math.cos(azr)*math.cos(altr)+Up*math.sin(altr)
        sundir/=np.linalg.norm(sundir)
        sh=shadowed(P,sundir); report(sname,sh)

# north sweep: does ANY orientation make shadowed set match cribs? (inaug 15h)
print("\n=== north sweep (Inaug 15h) : best crib-overlap vs orientation ===")
best=(-1,)
az,alt=228.4,19.9; azr,altr=math.radians(az),math.radians(alt)
for h in (+1,-1):
    for rot in range(0,360,5):
        rr=math.radians(rot)
        Nr=np.array([math.cos(rr)*Nm[0]-math.sin(rr)*Nm[1], math.sin(rr)*Nm[0]+math.cos(rr)*Nm[1],0]); Nr/=np.linalg.norm(Nr)
        Er=np.array([Nr[1]*h,-Nr[0]*h,0.0])
        sundir=Er*math.sin(azr)*math.cos(altr)+Nr*math.cos(azr)*math.cos(altr)+Up*math.sin(altr); sundir/=np.linalg.norm(sundir)
        sh=shadowed(P,sundir); idx=set(i for i,s in enumerate(sh) if s)
        ov=len(idx&cribset); frac=ov/max(1,len(idx))
        if ov>best[0]: best=(ov,len(idx),h,rot,frac)
print("  best crib-overlap:",best,"(overlap, nshadow, handed, rot_deg_from_diagramN, frac)")
