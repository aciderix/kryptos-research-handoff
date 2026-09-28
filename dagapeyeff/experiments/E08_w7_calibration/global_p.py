import re,statistics as st,collections
import os; SC=os.path.join(os.path.dirname(os.path.abspath(__file__)),'logs')
real={('E',2):9,('E',3):13,('E',4):16,('E',5):23,('E',6):26,('E',7):46,('E',8):42,('E',9):46,
      ('I',2):3,('I',3):18,('I',4):13,('I',5):8,('I',6):15,('I',7):14,('I',8):12,('I',9):10}   # journaux E04 (réel, B)
null=collections.defaultdict(list)  # (fam,W) -> liste par mélange
for fam in "EI":
    t=open(f"{SC}/e08_glob_{fam}.out").read()
    blocks=t.split("NULL #")
    cur=[]
    for line in t.splitlines():
        m=re.match(r"\s+W=(\d+) : Rmax=(\d+)",line)
        if m: cur.append((int(m.group(1)),int(m.group(2))))
        if line.startswith("NULL #"):
            for w,r in cur: null[(fam,w)].append(r)
            cur=[]
n=min(len(v) for v in null.values()); print("mélanges :",n)
mu={k:st.mean(v[:n]) for k,v in null.items()}; sd={k:st.pstdev(v[:n]) for k,v in null.items()}
zr={k:(real[k]-mu[k])/sd[k] for k in real}
zmax_real=max(zr.values()); kbest=max(zr,key=zr.get)
print("z réels :",{f"{k[0]}{k[1]}":round(v,2) for k,v in sorted(zr.items())})
print("z max réel = %.2f (cellule %s%d)"%(zmax_real,*kbest))
zm=[max((null[k][i]-mu[k])/sd[k] for k in real) for i in range(n)]
ge=sum(z>=zmax_real for z in zm); print("p_global = (1+%d)/(%d+1) = %.4f"%(ge,n,(1+ge)/(n+1)))
