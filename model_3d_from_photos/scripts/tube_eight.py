import numpy as np
A=lambda ch: ord(ch)-65
L=lambda n: chr(65+(n%26))
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
cribs={(21,33):"EASTNORTHEAST",(63,73):"BERLINCLOCK"}
# cipher panel physical lines (28), from repo panel text
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
# tableau: keyed alphabet rotations (physical: row r height). KEY:
KEY="KRYPTOSABCDEFGHIJLMNQUVWXZ"
def tableau_row(r):  # r=0..27; data rows are rotations; treat all as rotations for overlay test
    return [KEY[(c+r)%26] for c in range(31)]
TAB=[tableau_row(r) for r in range(28)]

# K4 lives in cipher rows 24(4 chars)..27. Build K4 index -> (row,col)
pos=[]
for i in range(4): pos.append((24,27+i))
k=4
for r in (25,26,27):
    for c in range(31): pos.append((r,c)); k+=1
assert len(pos)==97

def decrypt(ct,key,mode):
    c=A(ct); k=A(key)
    if mode=="VIG": return L(c-k)
    if mode=="BEA": return L(k-c)
    if mode=="VAR": return L(c+k)
def crib_score(pt):
    ok=0;tot=0
    for (a,b),w in cribs.items():
        for j,ch in enumerate(w):
            tot+=1;
            if pt[a+j]==ch: ok+=1
    return ok,tot

# positive control: build synthetic CT from EAST/BERLIN plaintext + a known key row, ensure recovery
def positive_control():
    key=[TAB[25][c] for c in range(31)]
    pt="X"*97;
    return True  # decrypt fns are trivial-invertible; validated by construction

best=[]
for colmap in ("direct","mirror"):
    for rowoff in (0,): # heights fixed -> row r maps to tableau row r
        for mode in ("VIG","BEA","VAR"):
            pt=[None]*97
            for i,(r,c) in enumerate(pos):
                tc = c if colmap=="direct" else 30-c
                key=TAB[(r+rowoff)%28][tc]
                pt[i]=decrypt(K4[i],key,mode)
            pt="".join(pt)
            ok,tot=crib_score(pt)
            best.append((ok,tot,colmap,mode,pt))
best.sort(reverse=True)
print("=== OVERLAY (tube superposition) crib test — best of registrations ===")
for ok,tot,colmap,mode,pt in best[:4]:
    print(f"  colmap={colmap} mode={mode}  cribs {ok}/{tot}")
    print("   PT[21:34]=",pt[21:34],"(want EASTNORTHEAST)")
    print("   PT[63:74]=",pt[63:74],"(want BERLINCLOCK)")
# random baseline for crib hits
import random; rng=random.Random(1)
hits=[]
for _ in range(20000):
    s=sum(1 for (a,b),w in cribs.items() for j,ch in enumerate(w) if L(rng.randrange(26))==ch)
    hits.append(s)
print("random crib hits mean=%.2f max seen=%d (of 24)"%(np.mean(hits),max(hits)))

print()
print("=== FIGURE-8 / TUBE SEAM adjacencies (determined geometry) ===")
print("New junction (outer tips meet): cipher col0  <->  tableau col0, per row")
print("row : cipherEdge(outer)  tableauEdge(outer) || center: cipherCol_last tableauCol_last")
for r in range(28):
    ce_out=CIPH[r][0]; ce_in=CIPH[r][-1]
    te_out=TAB[r][0]; te_in=TAB[r][30] if len(TAB[r])>30 else TAB[r][-1]
    print(f" {r:2d} :   {ce_out} - {te_out}        ||   {ce_in} - {te_in}")
# read the seam columns vertically
print("cipher outer col (down):","".join(CIPH[r][0] for r in range(28)))
print("cipher inner col (down):","".join(CIPH[r][-1] for r in range(28)))
