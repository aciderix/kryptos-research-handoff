import numpy as np, csv, math
from PIL import Image, ImageDraw, ImageFont
MPU=0.02935
cA,cB=np.load("circles.npy")  # tableau, cipher arc centers (model units)
cow=np.load("copper_verts.npy")[:, :2]   # top-view footprint (model units)
# centroids of the two panels (letters)
def centroid(fn):
    xs=[];ys=[]
    for d in csv.DictReader(open(fn)):
        xs.append(float(d['x_m'])/MPU); ys.append(float(d['y_m'])/MPU)
    return np.array([np.mean(xs),np.mean(ys)])
tabc=centroid("grid_tableau_hi.csv"); cipc=centroid("grid_cipher_hi.csv")
tree=np.array([-90.91,-44.62])
# East = tableau concave = from tableau centroid toward its arc center cA
East=np.array([cA[0]-tabc[0],cA[1]-tabc[1]]); East/=np.linalg.norm(East)
def render(h,fname):
    # North = East rotated +90° for h=+1 (CCW), or -90° for reflection handling
    if h==+1: North=np.array([-East[1],East[0]])
    else:     North=np.array([ East[1],-East[0]])
    def toimg(p):  # world: x=East comp, y=North comp
        v=np.array(p); return np.array([v@East, v@North])
    pts=np.array([toimg(p) for p in cow])
    # also key features
    feats={"tableau(Vsq)":toimg(tabc),"cipher(crypto)":toimg(cipc),"tree":toimg(tree),
           "cA":toimg(cA[:2]),"cB":toimg(cB[:2])}
    mn=pts.min(0)-20; mx=pts.max(0)+40
    W=700; scale=W/(mx[0]-mn[0]); H=int((mx[1]-mn[1])*scale)+120
    img=Image.new("RGB",(W+120,H+40),"white"); d=ImageDraw.Draw(img)
    def px(q): return (int((q[0]-mn[0])*scale)+20, int((mx[1]-q[1])*scale)+60)  # y up=north
    for p in pts:
        x,y=px(p); d.point((x,y),fill="#bbb")
    for name,q in feats.items():
        x,y=px(q)
        col="#c00" if "tableau" in name else ("#06c" if "cipher" in name else "#093")
        d.ellipse([x-5,y-5,x+5,y+5],fill=col); d.text((x+7,y-6),name,fill=col)
    # north/east arrows top-right
    ax,ay=W+40,90
    d.line([(ax,ay),(ax,ay-45)],fill="black",width=3); d.text((ax-5,ay-62),"N",fill="black")
    d.line([(ax,ay),(ax+45,ay)],fill="black",width=3); d.text((ax+48,ay-6),"E",fill="black")
    d.text((10,10),f"Modèle calé Nord=haut (tableau concave=Est), chiralité h={h}",fill="black")
    d.text((10,H+18),"Attendu (plan NSA): tableau au N, cipher au S, arbre au SW, lecture vers E",fill="#444")
    img.save(fname)
    # print bearings for check
    def bearing(v):
        return (math.degrees(math.atan2(v@East, v@North)))%360
    print(f"h={h}: tableau@{bearing(tabc-((cA[:2]+cB[:2])/2)):.0f}° cipher@{bearing(cipc-((cA[:2]+cB[:2])/2)):.0f}° tree@{bearing(tree-((cA[:2]+cB[:2])/2)):.0f}° | cipher_concave@{bearing(cB[:2]-cipc):.0f}°(att.~270 W)")
render(+1,"align_h+1.png"); render(-1,"align_h-1.png")
print("saved align_h+1.png align_h-1.png")
