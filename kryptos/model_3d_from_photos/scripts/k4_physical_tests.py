import csv, numpy as np, random
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
assert len(K4)==97
# physical layout on copper (from stencil): width=31 grid, row-major continuous.
# K4 occupies: last 4 cols of one row (OBKR) + 3 full rows of 31.
# grid (prow,pcol): OBKR at cols 27..30 of row0; then rows1..3 cols0..30
pos=[]  # (i, prow, pcol)
for i in range(4): pos.append((i,0,27+i))
k=4
for r in (1,2,3):
    for c in range(31):
        pos.append((k,r,c)); k+=1
assert k==97
prow=[p[1] for p in pos]; pcol=[p[2] for p in pos]

# ---- attach measured 3D coords from detected grid rows 24..27 (cipher CSV) ----
rows={}
with open("grid_cipher_hi.csv") as f:
    for d in csv.DictReader(f):
        r=int(d["row"]); rows.setdefault(r,{})[int(d["col"])]=(float(d["x_m"]),float(d["y_m"]),float(d["z_m"]))
detmap={0:24,1:25,2:26,3:27}  # physical K4 row -> detected row index
coords=[]
for (i,pr,pc) in pos:
    dr=detmap[pr]; xyz=rows.get(dr,{}).get(pc)
    coords.append(xyz)
have=sum(1 for c in coords if c)
print("K4 letters with measured 3D coords:",have,"/97")

# ---- écart-7 coincidences and doublets ----
e7=[i for i in range(90) if K4[i]==K4[i+7]]
dbl=[i for i in range(96) if K4[i]==K4[i+1]]
print("écart-7 coincidences (i where K4[i]==K4[i+7]):",e7,"count",len(e7))
print("doublets (i where K4[i]==K4[i+1]):",dbl,"count",len(dbl))
print("doublet letters:",[(i,K4[i]) for i in dbl])
# physical placement of these
def place(i):
    p=pos[i]; return f"i={i}({K4[i]}) row{p[1]} col{p[2]}"
print("-- écart-7 pairs physical --")
for i in e7:
    a=pos[i]; b=pos[i+7]
    print(f"  {K4[i]}=={K4[i+7]} : row{a[1]}c{a[2]} <-> row{b[1]}c{b[2]}  drow={b[1]-a[1]} dcol={b[2]-a[2]}")
print("-- doublets physical --")
for i in dbl:
    a=pos[i]; b=pos[i+1]
    print(f"  {K4[i]}{K4[i+1]} : row{a[1]}c{a[2]} -> row{b[1]}c{b[2]}")

# ---- mod-7 phase of doublets vs physical column ----
print("doublet index mod 7:",[i%7 for i in dbl])
print("doublet physical col:",[pos[i][2] for i in dbl])
print("doublet physical col mod 7:",[pos[i][2]%7 for i in dbl])

# ---- shuffle test: is the écart-7 count special? (linear index, replicates base14 as sanity) ----
rng=random.Random(42); N=200000; base=sum(1 for i in range(90) if K4[i]==K4[i+7])
cnt=0; L=list(K4)
for _ in range(N):
    rng.shuffle(L)
    c=sum(1 for i in range(90) if L[i]==L[i+7])
    if c>=base: cnt+=1
print(f"shuffle p(écart7>={base}) = {cnt/N:.4f}  (base14 reported ~0.004 on full K4)")
