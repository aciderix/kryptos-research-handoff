import bpy, numpy as np, math
from mathutils import Vector
from mathutils.bvhtree import BVHTree
from PIL import Image

bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
obj=bpy.data.objects["Kryptos.Part.CopperSheet"]; me=obj.data
n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
M=np.array(obj.matrix_world); cow=(np.c_[co,np.ones(n)]@M.T)[:,:3]
x,y,z=cow[:,0],cow[:,1],cow[:,2]; zmin,zmax=z.min(),z.max()
verts=[Vector(p) for p in cow]; tris=[]
for p in me.polygons:
    vs=list(p.vertices)
    for i in range(1,len(vs)-1): tris.append((vs[0],vs[i],vs[i+1]))
bvh=BVHTree.FromPolygons(verts,tris)
cA,cB=np.load("circles.npy"); mA=np.load("assign.npy")

def angular_window(th):
    # occupancy histogram over 360 bins, find longest empty run = outside gap
    nb=360; h=np.zeros(nb,bool)
    idx=((th+math.pi)/(2*math.pi)*nb).astype(int)%nb
    h[idx]=True
    # find longest run of False in circular array
    best_len=0;best_start=0;cur=0;cur_start=0
    hh=np.r_[h,h]
    i=0
    runs=[]
    # scan 2*nb for circular runs of False
    j=0
    while j<2*nb:
        if not hh[j]:
            k=j
            while k<2*nb and not hh[k]: k+=1
            runs.append((j,k-j)); j=k
        else: j+=1
    # pick longest run with length<nb
    gap=max((r for r in runs if r[1]<nb), key=lambda r:r[1])
    gap_start,gap_len=gap
    arc_start_bin=(gap_start+gap_len)%nb
    arc_len_bins=nb-gap_len
    a0=-math.pi+2*math.pi*arc_start_bin/nb
    width=2*math.pi*arc_len_bins/nb
    return a0,width

def rasterize(cx,cy,R,mask,cols,label):
    th=np.arctan2(y[mask]-cy,x[mask]-cx)
    a0,width=angular_window(th)
    arc=R*width; rows=int(cols*(zmax-zmin)/arc)
    img=np.zeros((rows,cols),dtype=np.uint8)
    Ro=R+8.0; maxd=18.0
    for j in range(cols):
        ang=a0+width*(j+0.5)/cols
        ca,sa=math.cos(ang),math.sin(ang); d=Vector((-ca,-sa,0.0))
        ox=cx+Ro*ca; oy=cy+Ro*sa
        for i in range(rows):
            zz=zmax-(zmax-zmin)*(i+0.5)/rows
            hit=bvh.ray_cast(Vector((ox,oy,zz)),d,maxd)
            if hit[0] is not None: img[i,j]=255
    print(label,"arc_deg",round(math.degrees(width),1),"arc_len",round(arc,1),"img",img.shape)
    return img
for (c,mask,cols,lab,fn) in ((cA,mA,900,"A(tableau)","stencilA2.png"),
                             (cB,~mA,900,"B(cipher)","stencilB2.png")):
    img=rasterize(c[0],c[1],c[2],mask,cols,lab); Image.fromarray(img).save(fn)
