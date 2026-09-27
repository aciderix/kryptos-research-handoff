import bpy, numpy as np, csv
from mathutils import Vector
from mathutils.bvhtree import BVHTree
MPU=0.02935
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
z=cow[:,2]; zmid=(z.min()+z.max())/2
# tableau letter positions
TAB=[]
for d in csv.DictReader(open("grid_tableau_hi.csv")):
    TAB.append(np.array([float(d['x_m']),float(d['y_m']),float(d['z_m'])])/MPU)
def panel_of(loc):
    x,y=loc[0],loc[1]
    dB=abs(np.hypot(x-cB[0],y-cB[1])-cB[2]); dA=abs(np.hypot(x-cA[0],y-cA[1])-cA[2])
    return "tableau" if dA<dB else "cipher"
# light candidates: tableau arc center, cipher arc center, inflection, petrified tree
tree=np.array([-90.91,-44.62,zmid])  # from space_and_scale center (model units)
lights={"tableau_center":np.array([cA[0],cA[1],zmid]),
        "cipher_center":np.array([cB[0],cB[1],zmid]),
        "inflection":np.array([(cA[0]+cB[0])/2,(cA[1]+cB[1])/2,zmid]),
        "petrified_tree":tree}
for lname,Lp in lights.items():
    hitc=0
    for p in TAB:
        d=p-Lp; d=d/np.linalg.norm(d)
        h=bvh.ray_cast(Vector(p)+0.5*Vector(d),Vector(d),500.0)
        if h[0] is not None and panel_of(h[0])=="cipher": hitc+=1
    print(f"light={lname:16s}: tableau-rays hitting CIPHER panel = {hitc}/{len(TAB)}")
