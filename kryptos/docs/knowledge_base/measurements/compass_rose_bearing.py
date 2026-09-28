"""Independent measurement (two photos: official CIA photo, and Jim Gillogly's 1999 photo) of the engraved needle bearing on the Kryptos compass rose.
Input: official CIA photo kryptos_sculpture_2_lg.jpg (980x1470), point coordinates read by eye.
Method: homography mapping the four cardinal letters (assumed equidistant from the rose centre,
at N/E/S/W) to a unit square, then bearing of the needle in the rose frame.
Sensitivity: each image point perturbed by +/- 15 px (Monte Carlo) to bound reading error."""
import numpy as np, random, math
# pixel (x, y) in the CIA photo, read by eye
L = {"S": (170, 625), "W": (705, 585), "N": (855, 1080), "E": (165, 1160)}
needle_light_tip = (557, 552)   # light half, pointing towards the lodestone
needle_dark_end  = (362, 1210)  # dark half, opposite end
dst = {"N": (0, 1), "E": (1, 0), "S": (0, -1), "W": (-1, 0)}
def H_from(pts):
    A=[]
    for k,(x,y) in pts.items():
        u,v=dst[k]
        A.append([-x,-y,-1,0,0,0,u*x,u*y,u]); A.append([0,0,0,-x,-y,-1,v*x,v*y,v])
    _,_,Vt=np.linalg.svd(np.array(A,float)); return Vt[-1].reshape(3,3)
def ap(H,p):
    q=H@np.array([p[0],p[1],1.0]); return q[:2]/q[2]
def bearing(v):  # clockwise from N
    return math.degrees(math.atan2(v[0],v[1]))%360
def measure(Lp,tip,end):
    H=H_from(Lp); t=ap(H,tip); e=ap(H,end)
    return bearing(t-e), bearing(e-t)   # direction of light tip, of dark end
b_tip,b_end=measure(L,needle_light_tip,needle_dark_end)
print(f"light tip bearing (rose frame): {b_tip:.1f} deg ; dark end: {b_end:.1f} deg")
r=random.Random(0); tips=[]; ends=[]
for _ in range(4000):
    j=lambda p:(p[0]+r.uniform(-15,15),p[1]+r.uniform(-15,15))
    a,b=measure({k:j(v) for k,v in L.items()},j(needle_light_tip),j(needle_dark_end)); tips.append(a); ends.append(b)
tips.sort(); ends.sort()
print(f"95% range light tip: {tips[100]:.1f}-{tips[3900]:.1f} ; dark end: {ends[100]:.1f}-{ends[3900]:.1f}")
print("reference: WSW=247.5, SW=225, ENE=67.5, NE=45")


# --- Second, independent photo: Jim Gillogly 1999, https://www.voynich.net/Kryptos/compass1.jpg (1280x960)
L2 = {"S": (590, 290), "W": (1052, 382), "N": (918, 745), "E": (355, 610)}
b_tip2, b_end2 = measure(L2, (950, 332), (472, 688))
print(f"Gillogly 1999 photo: light tip {b_tip2:.1f} deg ; dark end {b_end2:.1f} deg")
