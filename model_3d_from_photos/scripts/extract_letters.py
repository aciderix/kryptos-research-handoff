import bpy, numpy as np, math, csv
from mathutils import Vector
from mathutils.bvhtree import BVHTree
from PIL import Image
from scipy import ndimage

bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")
obj=bpy.data.objects["Kryptos.Part.CopperSheet"]; me=obj.data
n=len(me.vertices); co=np.empty((n,3)); me.vertices.foreach_get("co",co.reshape(-1))
M=np.array(obj.matrix_world); cow=(np.c_[co,np.ones(n)]@M.T)[:,:3]
x,y,z=cow[:,0],cow[:,1],cow[:,2]; zmin,zmax=z.min(),z.max(); H=zmax-zmin
verts=[Vector(p) for p in cow]; tris=[]
for p in me.polygons:
    vs=list(p.vertices)
    for i in range(1,len(vs)-1): tris.append((vs[0],vs[i],vs[i+1]))
bvh=BVHTree.FromPolygons(verts,tris)
cA,cB=np.load("circles.npy"); mA=np.load("assign.npy")

# real-world scale: sculpture copper width ~20 ft; model arc heights etc in model units.
# We'll report both model units and a ft estimate using height=~12 ft as anchor.
FT_PER_UNIT = 12.0/H   # assume screen height ~12 ft

def angular_window(th):
    nb=720; h=np.zeros(nb,bool)
    idx=((th+math.pi)/(2*math.pi)*nb).astype(int)%nb; h[idx]=True
    hh=np.r_[h,h]; runs=[]; j=0
    while j<2*nb:
        if not hh[j]:
            k=j
            while k<2*nb and not hh[k]: k+=1
            runs.append((j,k-j)); j=k
        else: j+=1
    gap=max((r for r in runs if r[1]<nb),key=lambda r:r[1])
    a0=-math.pi+2*math.pi*((gap[0]+gap[1])%nb)/nb
    width=2*math.pi*(nb-gap[1])/nb
    return a0,width

def process(cx,cy,R,mask,cols,name):
    th=np.arctan2(y[mask]-cy,x[mask]-cx); a0,width=angular_window(th)
    # pad the window slightly to avoid clipping edge column
    pad=math.radians(4); a0-=pad; width+=2*pad
    arc=R*width; rows=int(cols*H/arc)
    img=np.zeros((rows,cols),np.uint8); Ro=R+8.0; maxd=18.0
    for j in range(cols):
        ang=a0+width*(j+0.5)/cols; ca,sa=math.cos(ang),math.sin(ang); d=Vector((-ca,-sa,0))
        ox=cx+Ro*ca; oy=cy+Ro*sa
        for i in range(rows):
            zz=zmax-H*(i+0.5)/rows
            if bvh.ray_cast(Vector((ox,oy,zz)),d,maxd)[0] is not None: img[i,j]=255
    Image.fromarray(img).save(f"final_{name}.png")
    # holes (letters) = 0 pixels; merge intra-letter fragments via dilation, then label
    holes=(img==0)
    dil=ndimage.binary_dilation(holes,iterations=3)
    lab,nlab=ndimage.label(dil)
    sizes=ndimage.sum(np.ones_like(lab),lab,range(1,nlab+1))
    border_labels=set(lab[0,:]).union(lab[-1,:]).union(lab[:,0]).union(lab[:,-1])-{0}
    # centroid computed on ORIGINAL holes weighted, within each dilated label
    cents=ndimage.center_of_mass(holes,lab,range(1,nlab+1))
    letters=[]
    for k in range(1,nlab+1):
        if k in border_labels: continue
        s=sizes[k-1]
        if s<40 or s>3000: continue   # filter specks and big frames
        ci,cj=cents[k-1]
        if not (np.isfinite(ci) and np.isfinite(cj)): continue
        ang=a0+width*(cj+0.5)/cols; zz=zmax-H*(ci+0.5)/rows
        wx=cx+R*math.cos(ang); wy=cy+R*math.sin(ang)
        s_arc=R*(ang-a0)   # flattened horizontal (model units)
        letters.append((s_arc, zz, wx,wy,zz, s))
    print(name,"raster",img.shape,"arc_len",round(arc,1),"raw_comps",nlab,"letters",len(letters))
    return letters,dict(cx=cx,cy=cy,R=R,a0=a0,width=width,arc=arc,rows=rows,cols=cols)

Lc,gA=process(cB[0],cB[1],cB[2],~mA,1000,"cipher")   # B = ciphertext panel
Lt,gt=process(cA[0],cA[1],cA[2], mA,1000,"tableau")  # A = tableau panel

def group_rows(letters):
    zs=np.array(sorted(set(round(l[1],1) for l in letters)))
    # cluster z into rows
    zvals=np.array([l[1] for l in letters]); order=np.argsort(-zvals)
    # simple: sort by z desc, split where gap> median letter height
    ls=sorted(letters,key=lambda l:-l[1])
    rows=[]; cur=[ls[0]];
    for a,b in zip(ls,ls[1:]):
        if abs(a[1]-b[1])>2.0: rows.append(cur); cur=[b]
        else: cur.append(b)
    rows.append(cur)
    for r in rows: r.sort(key=lambda l:l[0])  # left to right by arc
    return rows

for letters,label in ((Lc,"cipher"),(Lt,"tableau")):
    rows=group_rows(letters)
    with open(f"letters_{label}.csv","w",newline="") as f:
        w=csv.writer(f); w.writerow(["row","col","arc_s_units","z_units","x","y","z","arc_s_ft","z_ft","blob_px"])
        for ri,r in enumerate(rows):
            for cj,l in enumerate(r):
                s_arc,zz,wx,wy,wz,sz=l
                w.writerow([ri,cj,round(s_arc,3),round(zz,3),round(wx,3),round(wy,3),round(wz,3),
                            round(s_arc*FT_PER_UNIT,4),round((zz-zmin)*FT_PER_UNIT,4),int(sz)])
    print(label,"rows",len(rows),"row_lengths",[len(r) for r in rows])
print("FT_PER_UNIT",round(FT_PER_UNIT,5),"height_units",round(H,2))
