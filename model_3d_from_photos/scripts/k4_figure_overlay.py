import json, numpy as np
from PIL import Image, ImageDraw, ImageFont
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
# physical grid rows for K4 (width 31): row0 has OBKR at cols27-30; rows1-3 full
grid=[[None]*31 for _ in range(4)]
for i in range(4): grid[0][27+i]=K4[i]
k=4
for r in (1,2,3):
    for c in range(31): grid[r][c]=K4[k]; k+=1
e7=[0,7,12,15,32,45,65,76,86]; dbl=[18,25,32,42,46,67]
def rc(i):
    if i<4: return 0,27+i
    j=i-4; return 1+j//31, j%31
dblset={rc(i) for i in dbl}|{rc(i+1) for i in dbl}
# draw
cell=34; W=31*cell+40; H=4*cell+70
img=Image.new("RGB",(W,H),"white"); d=ImageDraw.Draw(img)
try: fnt=ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf",20)
except: fnt=ImageFont.load_default()
try: sm=ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",11)
except: sm=fnt
for c in range(31):
    d.text((20+c*cell+6,6),f"{c}",fill="#888",font=sm)
    if c%7==0 and c>0: d.line([(20+c*cell,24),(20+c*cell,24+4*cell)],fill="#d33",width=1)
for r in range(4):
    for c in range(31):
        g=grid[r][c]
        if g is None: continue
        x=20+c*cell; y=28+r*cell
        if (r,c) in dblset: d.rectangle([x,y,x+cell,y+cell],fill="#ffe08a")
        d.rectangle([x,y,x+cell,y+cell],outline="#ccc")
        d.text((x+8,y+6),g,fill="black",font=fnt)
# mark écart-7 same-row links
for i in e7:
    a=rc(i); b=rc(i+7)
    if a[0]==b[0]:
        y=28+a[0]*cell+cell//2
        d.line([(20+a[1]*cell+cell//2,y),(20+b[1]*cell+cell//2,y)],fill="#2a7",width=3)
d.text((20,H-34),"Jaune = doublets  |  lignes vertes = écart-7 (même rangée, +7 col)  |  traits rouges = colonnes multiples de 7",fill="#333",font=sm)
d.text((20,H-18),"Largeur physique = 31 (mesurée). Rangée 1 : doublets BB,QQ,SS aux colonnes 14,21,28 (pas de 7).",fill="#333",font=sm)
img.save("deliverable/k4_physical_grid_annotated.png"); print("figure saved")

# ---- overlay registration of the two panels ----
geo=json.load(open("geo.json"))
c=geo["cipher"]; t=geo["tableau"]
def stats(g,cols=31,rows=28):
    return dict(R=g["R"], arc=g["R"]*g["width"], col_pitch=g["R"]*g["width"]/cols,
               row_pitch=g["H"]/rows)
sc=stats(c); st=stats(t)
print("cipher  R=%.2f arc=%.1f col_pitch=%.3f row_pitch=%.3f"%(sc["R"],sc["arc"],sc["col_pitch"],sc["row_pitch"]))
print("tableau R=%.2f arc=%.1f col_pitch=%.3f row_pitch=%.3f"%(st["R"],st["arc"],st["col_pitch"],st["row_pitch"]))
print("col_pitch ratio=%.3f  row_pitch identical=%.3f"%(st["col_pitch"]/sc["col_pitch"], st["row_pitch"]/sc["row_pitch"]))
