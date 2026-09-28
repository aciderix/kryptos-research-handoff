#!/usr/bin/env python3
"""Tests du profil de fréquences (indépendants de la transposition et du carré).
Usage : python3 tools/profile_tests.py CORPUS_DIR   (CORPUS_DIR contient train_<l>.txt ou corpus_big.txt, A-Z)
Compare le profil rang-fréquence des paires (géométries A : 196, B : 182 sans la colonne 14) à des échantillons de
même longueur de 8 langues (substitution monoalphabétique inconnue ⇒ seul le profil trié compte) et à un tirage
uniforme sur k symboles. Statistique D = Σ_r (obs_r − E_r)² / (E_r + 1), p calibré par simulation."""
import sys,os,random,collections,statistics as st
ROOT=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
d=''.join(c for c in open(os.path.join(ROOT,'data','ciphertext_1939.txt')).read() if c.isdigit())[:392]
p=[d[2*i:2*i+2] for i in range(196)]
seqs={'A':p,'B':[p[i] for i in range(196) if i%14!=13]}
CD=sys.argv[1]
langs=[('anglais','corpus_big.txt')]+[(n,f'train_{l}.txt') for n,l in [('français','fr'),('allemand','de'),('italien','it'),('espagnol','es'),('latin','la'),('néerlandais','nl'),('espéranto','eo')]]
def sprof(seq): c=sorted(collections.Counter(seq).values(),reverse=True); return c+[0]*(25-len(c))
def test(obs,sims):
    E=[st.mean(s[r] for s in sims) for r in range(25)]; D=lambda q: sum((q[r]-E[r])**2/(E[r]+1) for r in range(25))
    do=D(obs); ds=[D(s) for s in sims]; return do,(1+sum(x>=do for x in ds))/(len(ds)+1)
R=random.Random(3)
for g,seq in seqs.items():
    N=len(seq); obs=sprof(seq); print(f"géométrie {g} (N={N}) profil : {obs[:16]} ; symboles distincts {sum(x>0 for x in obs)}")
    for name,f in langs:
        path=os.path.join(CD,f)
        if not os.path.exists(path): path=os.path.join(CD,'lang',f)
        t=open(path).read(5000000).replace('J','I')
        do,pv=test(obs,[sprof(t[o:o+N]) for o in (R.randrange(len(t)-N-1) for _ in range(3000))]); print(f"  {name:12s} D={do:6.1f} p={pv:.4f}")
    for k in (13,14,15,18):
        do,pv=test(obs,[sprof([R.randrange(k) for _ in range(N)]) for _ in range(3000)]); print(f"  uniforme k={k:2d} D={do:6.1f} p={pv:.4f}")
