#!/usr/bin/env python3
"""Recalibration de l'excès de coïncidences à l'écart 7 dans K4 (leçon d'E08, dossier dagapeyeff/).
Coïncidence à l'écart g : nombre de i tels que c[i] == c[i+g]. Null : 20 000 permutations de K4 (fréquences
conservées). p local (écart 7) et p global (plus grand z sur les écarts 1-48, look-elsewhere)."""
import random,statistics as st
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
assert len(K4)==97
G=range(1,49)
def coinc(s): return [sum(s[i]==s[i+g] for i in range(len(s)-g)) for g in G]
obs=coinc(K4); R=random.Random(97); NS=20000; sims=[]
for _ in range(NS):
    l=list(K4); R.shuffle(l); sims.append(coinc(l))
mu=[st.mean(s[k] for s in sims) for k in range(len(G))]; sd=[st.pstdev(s[k] for s in sims) for k in range(len(G))]
z=[(obs[k]-mu[k])/sd[k] for k in range(len(G))]
k7=6; ploc=(1+sum(s[k7]>=obs[k7] for s in sims))/(NS+1)
zmax=max(z); kmax=z.index(zmax); zm=[max((s[k]-mu[k])/sd[k] for k in range(len(G))) for s in sims]
pglob=(1+sum(x>=z[k7] for x in zm))/(NS+1)
print(f"écart 7 : observé {obs[k7]} ; attendu {mu[k7]:.2f} (σ {sd[k7]:.2f}) ; z = {z[k7]:.2f} ; p local = {ploc:.4f}")
print(f"plus grand z sur les écarts 1-48 : écart {G[kmax]} (z = {zmax:.2f})")
print(f"p global (un écart quelconque parmi 1-48 atteint z ≥ {z[k7]:.2f}) = {pglob:.4f}")
print("écarts avec z ≥ 2 :",[(G[k],obs[k],round(z[k],2)) for k in range(len(G)) if z[k]>=2])
