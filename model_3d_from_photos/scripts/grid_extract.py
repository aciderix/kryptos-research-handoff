import numpy as np, math, csv
from PIL import Image
from scipy import ndimage
from scipy.signal import find_peaks

# reuse the two final stencils produced by extract_letters.py: final_cipher.png / final_tableau.png
# and geometry params recomputed identically here.
import json
geo=json.load(open("geo.json"))

def process(name):
    img=np.array(Image.open(f"final_{name}.png"))
    holes=(img==0).astype(float)
    rows_img,cols_img=holes.shape
    g=geo[name]
    # ---- row bands via horizontal projection ----
    P=holes.sum(axis=1)
    Ps=ndimage.gaussian_filter1d(P,sigma=rows_img/28/6)
    rowpitch=rows_img/28.0
    peaks,_=find_peaks(Ps,distance=rowpitch*0.6,height=Ps.max()*0.15)
    # ---- per row, find letter columns ----
    letters=[]
    halfband=int(rowpitch*0.42)
    for ri,pr in enumerate(peaks):
        i0=max(0,pr-halfband); i1=min(rows_img,pr+halfband)
        band=holes[i0:i1,:]
        prof=band.sum(axis=0)
        letterpitch=rowpitch  # letters roughly as wide as tall
        profs=ndimage.gaussian_filter1d(prof,sigma=letterpitch/6)
        cpk,_=find_peaks(profs,distance=letterpitch*0.55,height=profs.max()*0.18)
        for cj in cpk:
            ang=g['a0']+g['width']*(cj+0.5)/cols_img
            zz=g['zmax']-g['H']*(pr+0.5)/rows_img
            wx=g['cx']+g['R']*math.cos(ang); wy=g['cy']+g['R']*math.sin(ang)
            s_arc=g['R']*(ang-g['a0'])
            letters.append((ri,cj,s_arc,zz,wx,wy,zz))
    # counts per row
    from collections import Counter
    rc=Counter(l[0] for l in letters)
    lens=[rc[i] for i in range(len(peaks))]
    FT=12.0/g['H']
    with open(f"grid_{name}.csv","w",newline="") as f:
        w=csv.writer(f); w.writerow(["row","col","arc_s_units","z_units","x","y","z","arc_s_ft","z_ft"])
        # assign col index per row ordered by arc
        byrow={}
        for l in letters: byrow.setdefault(l[0],[]).append(l)
        for ri in sorted(byrow):
            for cj,l in enumerate(sorted(byrow[ri],key=lambda t:t[2])):
                _,_,s_arc,zz,wx,wy,wz=l
                w.writerow([ri,cj,round(s_arc,3),round(zz,3),round(wx,3),round(wy,3),round(wz,3),
                            round(s_arc*FT,4),round((zz-g['zmin'])*FT,4)])
    print(name,"rows",len(peaks),"total_letters",len(letters),"row_lengths",lens)
    return letters
process("cipher"); process("tableau")
