#!/usr/bin/env python3
"""E13 — hypothèse « 13 classes » (un symbole = deux lettres). Voir experiments/E13_thirteen_classes.
Usage : python3 tools/e13_classes.py WORDS.txt FREQ_DIR
FREQ_DIR contient freq_<l>.txt (25 log-probabilités A..Z sans J, issues des corpus d'ENTRAÎNEMENT)."""
import sys,os,random,collections,math
import numpy as np
ROOT=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AL="ABCDEFGHIKLMNOPQRSTUVWXYZ"; IDX={c:i for i,c in enumerate(AL)}
R=random.Random(13); NR=np.random.default_rng(13)
WORDS=sorted({w.strip().upper().replace('J','I') for w in open(sys.argv[1]) if w.strip().isalpha() and len(set(w.strip().upper().replace('J','I')))>=5})
FD=sys.argv[2]; LANGS=['en','fr','de','it','es','la','nl','eo']
d=''.join(c for c in open(os.path.join(ROOT,'data','ciphertext_1939.txt')).read() if c.isdigit())[:392]
pairs=[d[2*i:2*i+2] for i in range(196)]; B=[pairs[i] for i in range(196) if i%14!=13]
OBS=np.array(sorted(collections.Counter(B).values(),reverse=True)+[0]*(25-len(set(B))),dtype=float)
print("profil observé (colonnes 1-13, 182) :",OBS[:13].astype(int).tolist())

def samples(l,n,N=182):
    t=open(os.path.join(ROOT,'data','heldout',l+'.txt')).read().replace('J','I'); S=np.zeros((n,25))
    for k in range(n):
        o=R.randrange(len(t)-N-1)
        for ch in t[o:o+N]: S[k,IDX[ch]]+=1
    return S
def Mof(cls):  # cls[lettre] -> classe 0..12 ; matrice 25×25 (colonnes > 12 vides)
    M=np.zeros((25,25));
    for L in range(25): M[L,cls[L]]=1
    return M
def pval(S,cls):
    C=-np.sort(-(S@Mof(cls)),axis=1); E=C.mean(axis=0); D=lambda q:((q-E)**2/(E+1)).sum(axis=-1)
    do=D(OBS); ds=D(C); return do,(1+(ds>=do).sum())/(len(ds)+1),E
def balanced(l):
    lp=[float(x) for x in open(os.path.join(FD,f'freq_{l}.txt'))]; order=sorted(range(25),key=lambda L:-lp[L])
    cls=[0]*25; cls[order[0]]=0; rest=order[1:]
    for k in range(12): cls[rest[k]]=k+1; cls[rest[-1-k]]=k+1
    return cls
def wolseley(w):
    seq=[]
    for ch in w+AL:
        if ch in IDX and IDX[ch] not in seq: seq.append(IDX[ch])
    cls=[0]*25
    for p,L in enumerate(seq): cls[L]=min(p,24-p)
    return cls
def randpart():
    L=list(range(25)); R.shuffle(L); cls=[0]*25; cls[L[0]]=12
    for k in range(12): cls[L[1+2*k]]=k; cls[L[2+2*k]]=k
    return cls

print("\n== Q1(a) partition équilibrée optimale (glouton fréquente + rare)")
for l in LANGS:
    S=samples(l,2000 if l=='en' else 300); do,p,E=pval(S,balanced(l))
    print(f"  {l} : D={do:.2f} p={p:.4f} ; profil attendu {np.round(E[:13],1).tolist()}")
print("\n== Q1(c) partitions aléatoires (200, anglais)")
S_en=samples('en',2000); ps=[pval(S_en,randpart())[1] for _ in range(200)]
print(f"  fraction p>0,05 : {np.mean(np.array(ps)>0.05):.3f} ; médiane p {np.median(ps):.4f}")
print(f"\n== Q1(b) partitions de Wolseley ({len(WORDS)} mots du livre, anglais, 2000 extraits communs)")
res=[]
for w in WORDS:
    do,p,_=pval(S_en,wolseley(w)); res.append((p,do,w))
res.sort(reverse=True); pv=np.array([r[0] for r in res])
print(f"  fraction p>0,05 : {np.mean(pv>0.05):.4f} ; p>0,01 : {np.mean(pv>0.01):.4f} ; meilleurs :")
for p,do,w in res[:15]: print(f"    {w:18s} p={p:.4f} D={do:.2f}")
# autres langues pour les 30 meilleurs mots
print("  (30 meilleurs mots, autres langues : p max)")
for l in LANGS[1:]:
    S=samples(l,300); best=max((pval(S,wolseley(w))[1],w) for _,_,w in res[:30]); print(f"    {l} : {best[1]} p={best[0]:.4f}")

print("\n== Q2 puissance du test de dépendance des bigrammes (classes équilibrées, anglais non transposé, n=182)")
def G_bigr(seq):
    n=len(seq); bg=collections.Counter(zip(seq,seq[1:])); m=collections.Counter(seq)
    return 2*sum(o*math.log(o/(m[a]*m[b]*(n-1)/n/n)) for (a,b),o in bg.items())
t=open(os.path.join(ROOT,'data','heldout','en.txt')).read().replace('J','I'); cls=balanced('en'); rej=0; NT=300
for _ in range(NT):
    o=R.randrange(len(t)-200); seq=[cls[IDX[c]] for c in t[o:o+182]]; g=G_bigr(seq); s=seq[:]; ge=0
    for _ in range(200):
        R.shuffle(s); ge+= G_bigr(s)>=g
    rej+= (1+ge)/201<0.05
print(f"  fraction d'extraits où la dépendance est détectée (p<0,05) : {rej/NT:.3f}")
