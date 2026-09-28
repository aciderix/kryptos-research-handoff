import numpy as np, itertools
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
w=21
bg={}
for i in range(97-w): bg.setdefault(K4[i]+K4[i+w],[]).append(i)
pairs=[(k,v) for k,v in bg.items() if len(v)>1]
tot=0
for k,v in pairs:
    n=len(v)*(len(v)-1)//2; tot+=n
    print(k, v, "paires:",n)
print("total",tot)
dbl=[i for i in range(96) if K4[i]==K4[i+1]]
# une paire verticale répétée (i,j) est "due aux doublets" si (i,j)=(d,d+1) pour deux doublets empilés
