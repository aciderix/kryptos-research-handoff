"""Vérification de deux remarques trouvées dans les dossiers personnels du groupe (balayage du 24/09, base 7 §9).

1. Caveney (2017) : les digrammes « à une lettre d'écart » répétés de K4 (K?U, G?U, U?U) contiennent tous U,
   et toutes les occurrences de U y participent. Statistique choisie après coup : on mesure sa rareté
   sur des mélanges de K4, à l'écart 2 seul puis au meilleur des écarts 2, 3, 4.
2. ad_cooper (2008) : trigrammes « ABA » (OXO, TWT, PVT…) ; on les compte contre les mélanges.
"""
import numpy as np
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
rng=np.random.default_rng(8)
def skip_prop(s,lag=2):
    # paires (s[i], s[i+lag]) répétées ; existe-t-il une lettre X présente dans toutes, et dont toutes les occurrences y participent ?
    pairs={}
    for i in range(len(s)-lag): pairs.setdefault((s[i],s[i+lag]),[]).append(i)
    rep={k:v for k,v in pairs.items() if len(v)>1}
    if len(rep)<3: return False,len(rep)
    for X in set(s):
        if all(X in k for k in rep):
            cover=set()
            for k,v in rep.items():
                for i in v:
                    if s[i]==X: cover.add(i)
                    if s[i+lag]==X: cover.add(i+lag)
            occ=[i for i,c in enumerate(s) if c==X]
            if set(occ)<=cover: return True,len(rep)
    return False,len(rep)
print("K4 écart 2 :",skip_prop(K4))
L=list(K4); N=200000; h=0; hl=0
for _ in range(N):
    rng.shuffle(L); s="".join(L)
    h+=skip_prop(s)[0]
    hl+=any(skip_prop(s,lag)[0] for lag in (2,3,4))
print(f"mélanges : propriété à l'écart 2 : {h/N:.5f} ; à un écart 2, 3 ou 4 : {hl/N:.5f}")
# motif ad_cooper : trigrammes « ABA » (palindromes de 3)
aba=[i for i in range(95) if K4[i]==K4[i+2] and K4[i]!=K4[i+1]]
print("trigrammes ABA dans K4 :",[(i,K4[i:i+3]) for i in aba])
z=[]
for _ in range(20000):
    rng.shuffle(L); s="".join(L); z.append(sum(1 for i in range(95) if s[i]==s[i+2] and s[i]!=s[i+1]))
z=np.array(z); print(f"nombre ABA : K4 {len(aba)} ; moyenne mélanges {z.mean():.2f} ; P(>=) {np.mean(z>=len(aba)):.3f}")
