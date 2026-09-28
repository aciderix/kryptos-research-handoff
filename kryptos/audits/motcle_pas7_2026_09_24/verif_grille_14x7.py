CT="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
KA="KRYPTOSABCDEFGHIJLMNQUVWXZ"
P={}
for s,w in ((21,"EASTNORTHEAST"),(63,"BERLINCLOCK")):
    for j,c in enumerate(w): P[s+j]=c
cell=lambda i:((i%14)%7, i//14)       # case de la matrice de clé 7×7 (ligne mod 7, colonne)
cells={}
for i in P: cells.setdefault(cell(i),[]).append(i)
print("cases de clé partagées par deux lettres des cribs :", [v for v in cells.values() if len(v)>1])
print("cases de clé touchées :", len(cells), "sur 49")
key={i:KA[(KA.index(CT[i])-KA.index(P[i]))%26] for i in P}
for c in (1,2,4,5):
    print("colonne",c,":", "".join(key.get(14*c+r,"_") for r in range(14)))
print("clé 66-73 :", "".join(key[i] for i in range(66,74)))
print("doublets (ligne, colonne) :", [(i%14,i//14) for i in (18,25,32,42,46,67)])
