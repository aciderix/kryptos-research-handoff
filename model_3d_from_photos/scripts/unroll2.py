import bpy, numpy as np, math
from mathutils import Vector
from mathutils.bvhtree import BVHTree
from PIL import Image

bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
obj=bpy.data.objects["Kryptos.Part.CopperSheet"]; me=obj.data
n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
M=np.array(obj.matrix_world); cow=(np.c_[co,np.ones(n)]@M.T)[:,:3]
x,y,z=cow[:,0],cow[:,1],cow[:,2]; zmin,zmax=z.min(),z.max()

verts=[Vector(p) for p in cow]
tris=[]
for p in me.polygons:
    vs=list(p.vertices)
    for i in range(1,len(vs)-1): tris.append((vs[0],vs[i],vs[i+1]))
bvh=BVHTree.FromPolygons(verts,tris)

def fit_circle(px,py):
    A=np.c_[2*px,2*py,np.ones(len(px))]; b=px**2+py**2
    c,*_=np.linalg.lstsq(A,b,rcond=None)
    cx,cy=c[0],c[1]; R=math.sqrt(c[2]+cx**2+cy**2); return cx,cy,R
# init split by y, fit, then reassign by nearest circle, iterate
ymid=(y.min()+y.max())/2
cA=fit_circle(x[y>=ymid],y[y>=ymid]); cB=fit_circle(x[y<ymid],y[y<ymid])
for _ in range(6):
    dA=np.abs(np.hypot(x-cA[0],y-cA[1])-cA[2])
    dB=np.abs(np.hypot(x-cB[0],y-cB[1])-cB[2])
    mA=dA<=dB
    cA=fit_circle(x[mA],y[mA]); cB=fit_circle(x[~mA],y[~mA])
print("circle A",[round(v,2) for v in cA],"n",int(mA.sum()))
print("circle B",[round(v,2) for v in cB],"n",int((~mA).sum()))

def unwrap_span(th):
    th=np.sort(th); gaps=np.diff(th)
    # largest gap = outside of arc; rotate so arc is contiguous
    k=np.argmax(gaps); gap=gaps[k]
    start=th[k+1]  # arc starts after biggest gap
    rel=(th-start)%(2*math.pi)
    return start, rel.max()  # start angle, angular width

def rasterize(cx,cy,R,mask,cols=760,label=""):
    th=np.arctan2(y[mask]-cy,x[mask]-cx)
    start,width=unwrap_span(th)
    arc=R*width; rows=int(cols*(zmax-zmin)/arc)
    img=np.zeros((rows,cols),dtype=np.uint8)
    for j in range(cols):
        ang=start+width*(j+0.5)/cols
        d=Vector((-math.cos(ang),-math.sin(ang),0.0))
        ox=cx+2*R*math.cos(ang); oy=cy+2*R*math.sin(ang)
        for i in range(rows):
            zz=zmax-(zmax-zmin)*(i+0.5)/rows
            if bvh.ray_cast(Vector((ox,oy,zz)),d)[0] is not None:
                img[i,j]=255
    print(label,"arc_deg",round(math.degrees(width),1),"arc_len",round(arc,1),
          "img",img.shape)
    return img
imgA=rasterize(*cA,mA,label="A"); Image.fromarray(imgA).save("stencilA.png")
imgB=rasterize(*cB,~mA,label="B"); Image.fromarray(imgB).save("stencilB.png")
np.save("assign.npy",mA); np.save("circles.npy",np.array([cA,cB]))
