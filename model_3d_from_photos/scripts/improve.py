import bpy, numpy as np, math, csv, json
from mathutils import Vector
from mathutils.bvhtree import BVHTree
from PIL import Image
from scipy import ndimage
from scipy.signal import find_peaks

bpy.ops.wm.open_mainfile(filepath="KryptosSculpture3DPrintable.Public.blend")

# ---------- full-scene spatial inventory ----------
scene_inv={}
allmin=np.array([1e9]*3); allmax=np.array([-1e9]*3)
for o in bpy.data.objects:
    if o.type!='MESH': continue
    mw=o.matrix_world
    cs=np.array([ (mw@Vector(c)).to_tuple() for c in o.bound_box])
    mn=cs.min(0); mx=cs.max(0); ctr=(mn+mx)/2
    scene_inv[o.name]=dict(dim=[round(v,2) for v in (mx-mn)],
                           center=[round(v,2) for v in ctr],
                           min=[round(v,2) for v in mn], max=[round(v,2) for v in mx])
    if not o.name.startswith("Kryptos.AllInOne") and "AllExcept" not in o.name:
        allmin=np.minimum(allmin,mn); allmax=np.maximum(allmax,mx)
scene_inv["_SCENE_TOTAL(parts)"]=dict(dim=[round(v,2) for v in (allmax-allmin)],
                                      min=[round(v,2) for v in allmin],max=[round(v,2) for v in allmax])

# ---------- copper extraction (higher res) ----------
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

def angular_window(th):
    nb=1440; h=np.zeros(nb,bool); idx=((th+math.pi)/(2*math.pi)*nb).astype(int)%nb; h[idx]=True
    hh=np.r_[h,h]; runs=[]; j=0
    while j<2*nb:
        if not hh[j]:
            k=j
            while k<2*nb and not hh[k]: k+=1
            runs.append((j,k-j)); j=k
        else: j+=1
    gap=max((r for r in runs if r[1]<nb),key=lambda r:r[1])
    a0=-math.pi+2*math.pi*((gap[0]+gap[1])%nb)/nb; width=2*math.pi*(nb-gap[1])/nb
    return a0,width

def raster(cx,cy,R,mask,cols,pad_deg):
    th=np.arctan2(y[mask]-cy,x[mask]-cx); a0,width=angular_window(th)
    pad=math.radians(pad_deg); a0-=pad; width+=2*pad
    arc=R*width; rows=int(cols*H/arc)
    img=np.zeros((rows,cols),np.uint8); Ro=R+8.0; maxd=18.0
    for j in range(cols):
        ang=a0+width*(j+0.5)/cols; ca,sa=math.cos(ang),math.sin(ang); d=Vector((-ca,-sa,0))
        ox=cx+Ro*ca; oy=cy+Ro*sa
        for i in range(rows):
            zz=zmax-H*(i+0.5)/rows
            if bvh.ray_cast(Vector((ox,oy,zz)),d,maxd)[0] is not None: img[i,j]=255
    return img,dict(cx=cx,cy=cy,R=R,a0=a0,width=width,zmin=zmin,zmax=zmax,H=H,rows=rows,cols=cols,arc=arc)

def gridify(img,g):
    holes=(img==0).astype(float); rows_img,cols_img=holes.shape
    P=ndimage.gaussian_filter1d(holes.sum(1),sigma=rows_img/28/6)
    rp=rows_img/28.0
    rpk,_=find_peaks(P,distance=rp*0.6,height=P.max()*0.15)
    letters=[]; half=int(rp*0.44)
    for pr in rpk:
        band=holes[max(0,pr-half):min(rows_img,pr+half),:]
        prof=ndimage.gaussian_filter1d(band.sum(0),sigma=rp/6.5)
        cpk,_=find_peaks(prof,distance=rp*0.5,height=prof.max()*0.12)
        for cj in cpk:
            ang=g['a0']+g['width']*(cj+0.5)/cols_img; zz=g['zmax']-g['H']*(pr+0.5)/rows_img
            wx=g['cx']+g['R']*math.cos(ang); wy=g['cy']+g['R']*math.sin(ang)
            letters.append((pr,cj,g['R']*(ang-g['a0']),zz,wx,wy,zz))
    return letters,rpk

out={}
for name,(c,mask) in (("cipher",(cB,~mA)),("tableau",(cA,mA))):
    img,g=raster(c[0],c[1],c[2],mask,1400,6.0)
    Image.fromarray(img).save(f"hi_{name}.png")
    letters,rpk=gridify(img,g)
    from collections import Counter
    rc=Counter(l[0] for l in letters); lens=[rc[p] for p in rpk]
    out[name]=(letters,g,rpk,lens)
    print(name,"rows",len(rpk),"total",len(letters),"lens",lens)

json.dump({"scene_inventory_model_units":scene_inv,
           "copper_geometry":{k:{kk:(round(vv,3) if isinstance(vv,float) else vv)
                                 for kk,vv in out[k][1].items()} for k in out}},
          open("space_and_scale.json","w"),indent=2)

# scale: anchor screen lettered height. Use copper H as ~ real screen height.
# Provide two candidate anchors.
ANCHORS={"screen_height_12ft":12.0/H, "screen_height_11ft":11.0/H}
FT=ANCHORS["screen_height_12ft"]; MPU=FT*0.3048
for name in out:
    letters,g,rpk,lens=out[name]
    byrow={}
    for l in letters: byrow.setdefault(l[0],[]).append(l)
    with open(f"grid_{name}_hi.csv","w",newline="") as f:
        w=csv.writer(f); w.writerow(["row","col","x","y","z","arc_s_units","x_m","y_m","z_m","arc_s_m","z_from_base_m"])
        for ri,pr in enumerate(sorted(byrow)):
            for cj,l in enumerate(sorted(byrow[pr],key=lambda t:t[2])):
                _,_,s_arc,zz,wx,wy,wz=l
                w.writerow([ri,cj,round(wx,3),round(wy,3),round(wz,3),round(s_arc,3),
                            round(wx*MPU,4),round(wy*MPU,4),round(wz*MPU,4),
                            round(s_arc*MPU,4),round((zz-g['zmin'])*MPU,4)])
print("ANCHORS ft/unit:",{k:round(v,5) for k,v in ANCHORS.items()},"m/unit(12ft):",round(MPU,5))
print("SCENE TOTAL parts (units):",scene_inv["_SCENE_TOTAL(parts)"]["dim"],
      "-> meters:",[round(v*MPU,2) for v in scene_inv["_SCENE_TOTAL(parts)"]["dim"]])
