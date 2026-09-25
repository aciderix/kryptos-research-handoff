#!/usr/bin/env python3
"""Preuve algébrique : l'autoclé sur le clair à l'écart 7, alphabet mixte σ commun (Q3),
est INCOMPATIBLE avec les cribs de K4, sans aucune erreur, dans les trois conventions.
Aucun mot-clé, aucun budget d'erreur : contradiction sur σ (une permutation) déduite des seules 24 lettres.

Modèle. Domaine σ : x_i = σ(clair_i), y_i = σ(chiffré_i).
  VIG : y_i = x_i + x_{i-7}     BEAU : y_i = x_{i-7} - x_i     VAR : y_i = x_i - x_{i-7}
Les deux cribs (21–33 et 63–73) sont distants de 42 = 6×7 : chaque classe mod 7 contient donc
des positions des DEUX blocs, et les chaînes de récurrence les relient. On rassemble toutes les
égalités que les cribs imposent à σ, puis on teste si une égalité σ(X)=σ(Y) (X≠Y) ou σ(X)=0
est dans l'espace engendré (mod 2 ET mod 13, donc mod 26). Si oui, aucune permutation ne convient.
"""
import itertools
CT="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
P={}
for i,c in enumerate("EASTNORTHEAST"): P[21+i]=c
for i,c in enumerate("BERLINCLOCK"): P[63+i]=c
V=lambda c:ord(c)-65
from collections import defaultdict

def rows_for(mode, drop=()):
    Pd={k:v for k,v in P.items() if k not in drop}
    cls=defaultdict(list)
    for i in sorted(Pd): cls[i%7].append(i)
    rows=[]
    for r,ii in cls.items():
        i0=ii[0]
        for j in ii[1:]:
            n=(j-i0)//7; vec=[0]*26
            if mode=="VIG":   # x_i=y_i-x_{i-7} ; x_j = Σ(-1)^{n-t} y_{i0+7t} + (-1)^n x_{i0}
                vec[V(Pd[j])]-=1; vec[V(Pd[i0])]+=(-1)**n
                for t in range(1,n+1): vec[V(CT[i0+7*t])]+=(-1)**(n-t)
            elif mode=="VAR": # y_i=x_i-x_{i-7} ; x_i=x_{i-7}+y_i (même chaîne, signe +)
                vec[V(Pd[j])]-=1; vec[V(Pd[i0])]+=1
                for t in range(1,n+1): vec[V(CT[i0+7*t])]+=1
            else:             # BEAU y_i=x_{i-7}-x_i ; x_i=x_{i-7}-y_i ; x_j=x_{i0}-Σ y (sans alternance)
                vec[V(Pd[j])]-=1; vec[V(Pd[i0])]+=1
                for t in range(1,n+1): vec[V(CT[i0+7*t])]-=1
            rows.append([x%26 for x in vec])
    return rows

def rref(M,p):
    M=[r[:] for r in M]; piv=[]; r=0
    for c in range(26):
        pr=next((k for k in range(r,len(M)) if M[k][c]%p),None)
        if pr is None: continue
        M[r],M[pr]=M[pr],M[r]; inv=pow(M[r][c],-1,p) if p>2 else 1
        M[r]=[(x*inv)%p for x in M[r]]
        for k in range(len(M)):
            if k!=r and M[k][c]%p:
                f=M[k][c]; M[k]=[(M[k][j]-f*M[r][j])%p for j in range(26)]
        piv.append(c); r+=1
    return M[:r]

def inspan(vec,M,p):
    return len(rref(M,p))==len(rref(M+[[x%p for x in vec]],p))

def contradiction(rows):
    for X in range(26):
        e=[1 if c==X else 0 for c in range(26)]
        if inspan(e,rows,2) and inspan(e,rows,13): return ("σ(%c)=0"%(65+X))
    for X,Y in itertools.combinations(range(26),2):
        e=[1 if c==X else 25 if c==Y else 0 for c in range(26)]
        if inspan(e,rows,2) and inspan(e,rows,13): return ("σ(%c)=σ(%c)"%(65+X,65+Y))
    return None

if __name__=="__main__":
    for mode in ("VIG","VAR","BEAU"):
        c=contradiction(rows_for(mode))
        print(f"{mode:4s}: contradiction = {c}  -> {'IMPOSSIBLE' if c else 'possible'}")
    print("\nRobustesse à une erreur (retrait d'une lettre de crib) :")
    for mode in ("VIG","BEAU"):
        survive=[d for d in P if contradiction(rows_for(mode,(d,))) is None]
        print(f"  {mode}: retraits qui lèvent la contradiction : {survive if survive else 'AUCUN (≥2 erreurs nécessaires)'}")
    # pour VIG, quelle PAIRE de retraits ?
    ok=[ (a,b) for a,b in itertools.combinations(sorted(P),2) if contradiction(rows_for("VIG",(a,b))) is None]
    print("  VIG: paires de retraits levant la contradiction :",ok[:12],"..." if len(ok)>12 else "", f"({len(ok)} paires)")
