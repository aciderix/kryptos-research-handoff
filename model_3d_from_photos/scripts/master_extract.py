import csv, numpy as np
from PIL import Image, ImageDraw, ImageFont

CIPH=["EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJ","YQTQUXQBQVYUVLLTREVJYQTMKYRDMFD",
"VFPJUDEEHZWETZYVGWHKKQETGFQJNCE","GGWHKK?DQMCPFQZDQMMIAGPFXHQRLG",
"TIMVMZJANQLVKQEDAGDVFRPJUNGEUNA","QZGZLECGYUXUEENJTBJLBQCRTBJDFHRR",
"YIZETKZEMVDUFKSJHKFWHKUWQLSZFTI","HHDDDUVH?DWKBFUFPWNTDFIYCUQZERE",
"EVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDX","FLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKF",
"FHQNTGPUAECNUVPDJMQCLQUMUNEDFQ","ELZZVRRGKFFVOEEXBDMVPNFQXEZLGRE",
"DNQFMPNZGLFLPMRJQYALMGNUVPDXVKP","DQUMEBEDMHDAFMJGZNUPLGEWJLLAETG",
"ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIA","CHTNREYULDSLLSLLNOHSNOSMRWXMNE",
"TPRNGATIHNRARPESLNNELEBLPIIACAE","WMTWNDITEENRAHCTENEUDRETNHAEOE",
"TFOLSEDTIWENHAEIOYTEYQHEENCTAYCR","EIFTBRSPAMHHEWENATAMATEGYEERLB",
"TEEFOASFIOTUETUAEOTOARMAEERTNRTI","BSEDDNIAAHTTMSTEWPIEROAGRIEWFEB",
"AECTDDHILCEIHSITEGOEAOSDDRYDLORIT","RKLMLEHAGTDHARDPNEOHMGFMFEUHE",
"ECDMRIPFEIMEHNLSSTTRTVDOHW?OBKR","UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO",
"TWTQSJQSSEKZZWATJKLUDIAWINFBNYP","VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"]
truelens=[len(l) for l in CIPH]
print("true line lengths:",truelens,"sum",sum(truelens))

# load detected letters
det={}
for d in csv.DictReader(open("grid_cipher_hi.csv")):
    r=int(d['row']); det.setdefault(r,[]).append(d)
for r in det: det[r].sort(key=lambda d:float(d['arc_s_units']))
detlens=[len(det[r]) for r in range(28)]
print("detected line lengths:",detlens,"sum",sum(detlens))

# section boundaries in the continuous 869 stream (cumulative), mark K4 = last 97
cum=np.cumsum([0]+truelens)
total=cum[-1]
k4_start_global=total-97
# question-mark global indices
qglobal=[]
gi=0
for r,l in enumerate(CIPH):
    for c,ch in enumerate(l):
        if ch=='?': qglobal.append(gi)
        gi+=1
# crib global positions: cribs are within K4 (0-indexed in K4)
cribs=[(21,33,"EASTNORTHEAST"),(63,73,"BERLINCLOCK")]
crib_global=set()
for a,b,w in cribs:
    for j in range(a,b+1): crib_global.add(k4_start_global+j)

# build master rows (best-effort identity alignment)
master=[]; gi=0; conf_rows=0
for r in range(28):
    line=CIPH[r]; ds=det[r]
    exact = (len(ds)==len(line))
    if exact: conf_rows+=1
    n=min(len(ds),len(line))
    for c in range(len(line)):
        ch=line[c]
        d=ds[c] if c<len(ds) else None
        glob=cum[r]+c
        sect = "K4" if glob>=k4_start_global else ("pre-K4")
        rec=dict(gidx=glob,row=r,col=c,letter=ch,
                 section=sect,is_crib=glob in crib_global,is_q=(ch=='?'),
                 conf="exact" if exact else "approx",
                 x_m=float(d['x_m']) if d else None, y_m=float(d['y_m']) if d else None,
                 z_m=float(d['z_m']) if d else None, arc_s=float(d['arc_s_units']) if d else None)
        master.append(rec)
print("rows with exact identity alignment:",conf_rows,"/28")
with open("master_cipher_letters.csv","w",newline="") as f:
    w=csv.DictWriter(f,fieldnames=["gidx","row","col","letter","section","is_crib","is_q","conf","x_m","y_m","z_m","arc_s"])
    w.writeheader()
    for rec in master: w.writerow(rec)
print("wrote master_cipher_letters.csv (",len(master),"letters )")

# ---- spacing regularity (per row, arc spacing between consecutive letters) ----
print("\n=== SPACING per row (arc units) ===")
allgaps=[]
for r in range(28):
    ss=[float(d['arc_s_units']) for d in det[r]]
    g=np.diff(sorted(ss))
    allgaps+=list(g)
    print(f" row{r:2d} n={len(ss):2d} gap mean={g.mean():.2f} std={g.std():.2f} min={g.min():.2f} max={g.max():.2f}")
allgaps=np.array(allgaps)
print(f"ALL gaps: mean={allgaps.mean():.3f} std={allgaps.std():.3f} CV={allgaps.std()/allgaps.mean():.3f}")

# ---- residual vs ideal grid: fit per-row linear position vs col index ----
print("\n=== GRID residuals (letter arc pos vs ideal linear) ===")
maxres=[]
resid_grid=np.full((28,33),np.nan)
for r in range(28):
    ss=np.array([float(d['arc_s_units']) for d in det[r]])
    idx=np.arange(len(ss))
    A=np.c_[idx,np.ones_like(idx)]
    coef,*_=np.linalg.lstsq(A,ss,rcond=None)
    pred=A@coef; res=ss-pred
    for c in range(len(ss)): resid_grid[r,c]=res[c]
    maxres.append(np.abs(res).max())
    if np.abs(res).max()>1.5:
        print(f" row{r:2d} pitch={coef[0]:.2f} maxdev={np.abs(res).max():.2f} (letters shifted from ideal)")
print("median row max-residual:",round(float(np.nanmedian(maxres)),3),"units; letter box ~",round(allgaps.mean(),2),"units")

# residual heatmap
H=28; Wc=33; cell=26
img=Image.new("RGB",(Wc*cell+60,H*cell+40),"white"); dr=ImageDraw.Draw(img)
vmax=np.nanmax(np.abs(resid_grid))
for r in range(28):
    for c in range(33):
        v=resid_grid[r,c]
        if np.isnan(v): continue
        t=v/vmax
        R=int(255*max(0,t)); B=int(255*max(0,-t)); G=int(255*(1-abs(t)))
        dr.rectangle([50+c*cell,20+r*cell,50+c*cell+cell,20+r*cell+cell],fill=(255-B,255-R-B+G if False else G+ (255-R-B)//1,255-R),outline="#eee")
dr.text((5,5),"Residus position lettre vs grille ideale (rouge=+, bleu=-). Ecran chiffre.",fill="#333")
img.save("deliverable/cipher_grid_residuals.png"); print("saved residual heatmap")
