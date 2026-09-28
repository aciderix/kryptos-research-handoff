#!/usr/bin/env python3
"""E12 — calibration des mécanismes du livre sur de vrais textes (voir experiments/E12_mechanism_calibration).
Usage : python3 tools/e12_calibration.py WORDS.txt [NSIM_EN] [NSIM_OTHER]
WORDS.txt : mots-clés candidats (un par ligne ; ceux d'au moins 5 lettres distinctes sont utilisés)."""
import sys,os,random,collections,statistics as st,math
ROOT=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AL="ABCDEFGHIKLMNOPQRSTUVWXYZ"; IDX={c:i for i,c in enumerate(AL)}
R=random.Random(12)
WORDS=[w.strip().upper() for w in open(sys.argv[1]) if len(set(w.strip().upper().replace('J','I')))>=5 and w.strip().isalpha()]
NEN=int(sys.argv[2]) if len(sys.argv)>2 else 2000; NOT=int(sys.argv[3]) if len(sys.argv)>3 else 300
LANGS=['en','fr','de','it','es','la','nl','eo']
TXT={l:open(os.path.join(ROOT,'data','heldout',l+'.txt')).read().replace('J','I') for l in LANGS}

# ---------- chiffré réel ----------
d=''.join(c for c in open(os.path.join(ROOT,'data','ciphertext_1939.txt')).read() if c.isdigit())[:392]
REAL=[((0 if d[2*i]=='6' else 1 if d[2*i]=='7' else 2 if d[2*i]=='8' else 3 if d[2*i]=='9' else 4)*5+int(d[2*i+1])-1) for i in range(196)]

def features(seq):
    c=collections.Counter(seq); v=sorted(c.values(),reverse=True)
    t1=len(c); t3=sum(x>=11 for x in v); t4=sum(3<=x<=8 for x in v)
    best=99; excl=False
    for j in range(14):
        rest=[seq[i] for i in range(196) if i%14!=j]; dj=len(set(rest))
        if dj<best: best=dj; excl=set(seq)-set(rest)!=set()
    return dict(T1=t1,T2=best,T3=t3,T4=t4,T5=excl), v+[0]*(25-len(v))
FR,PR=features(REAL)

def kw_square():
    w=R.choice(WORDS).replace('J','I'); seq=[]
    for ch in w+AL:
        if ch in IDX and ch not in seq: seq.append(ch)
    return seq           # 25 lettres, position = case (0..24, lignes 6-0 × colonnes 1-5)
def letters(l,n):
    t=TXT[l]; o=R.randrange(len(t)-4*n-10); return t[o:o+4*n]

# ---------- mécanismes : renvoient au moins 196 symboles (cases 0..24) ----------
def M1(l):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; return [cell[c] for c in letters(l,196)][:196]
def M6(l,k,pol):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; out=[]; used=set(); src=iter(letters(l,196))
    rare=[cell[c] for c in 'QXZKV']
    while len(out)<196:
        if len(out)%k==k-1:
            if pol=='a': out.append(R.randrange(25))
            elif pol=='b': out.append(R.choice(rare))
            else: out.append(R.choice(sorted(used)) if used else R.randrange(25))
        else:
            s=cell[next(src)]; used.add(s); out.append(s)
    return out
def M7(l):  # Playfair (règles p. 123-124 : même colonne -> en dessous, même ligne -> à droite, rectangle)
    sq=kw_square(); pos={ch:(i//5,i%5) for i,ch in enumerate(sq)}; t=letters(l,120); pairs=[]; i=0
    while i<len(t)-1:
        a,b=t[i],t[i+1]
        if a==b: pairs.append((a,'X' if a!='X' else 'Z')); i+=1
        else: pairs.append((a,b)); i+=2
    out=[]
    for a,b in pairs:
        (ra,ca),(rb,cb)=pos[a],pos[b]
        if ca==cb: a2,b2=sq[((ra+1)%5)*5+ca],sq[((rb+1)%5)*5+cb]
        elif ra==rb: a2,b2=sq[ra*5+(ca+1)%5],sq[rb*5+(cb+1)%5]
        else: a2,b2=sq[ra*5+cb],sq[rb*5+ca]
        out+= [IDX[a2],IDX[b2]]  # lettres chiffrées -> cases d'un carré de Polybe (ordre alphabétique, bijectif)
    return out[:196]
def M11(l):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; L=R.randint(3,8); key=[R.randrange(25) for _ in range(L)]
    return [cell[AL[(IDX[c]+key[i%L])%25]] for i,c in enumerate(letters(l,196))][:196]
def M12(l):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; hom={'E':[cell['E'],cell['Q']],'T':[cell['T'],cell['X']],'A':[cell['A'],cell['Z']],'O':[cell['O'],cell['K']]}
    out=[]
    for c in letters(l,196):
        if c in 'QXZK': c='C'
        out.append(R.choice(hom[c]) if c in hom else cell[c])
    return out[:196]
def M15(l):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; return [cell[c] for c in letters(l,220) if R.random()>0.05][:196]
def M4v(l):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; return [min(cell[c],24-cell[c]) for c in letters(l,196)][:196]
def M8(l,stats):
    sq=kw_square(); cell={ch:i for i,ch in enumerate(sq)}; co=[]
    for c in letters(l,98): co+= [('r',cell[c]//5),('c',cell[c]%5)]
    W=R.randint(5,11); key=list(range(W)); R.shuffle(key); H=-(-len(co)//W)
    co+= [('n',R.randrange(5))]*(H*W-len(co))
    cols=[[co[r*W+j] for r in range(H)] for j in range(W)]; stream=[x for j in key for x in cols[j]]
    out=[]; okpairs=0
    for i in range(0,len(stream)-1,2):
        (ta,a),(tb,b)=stream[i],stream[i+1]; okpairs+= (ta=='r' and tb=='c'); out.append(a*5+b)
    stats.append(okpairs/(len(stream)//2)); return out[:196]

MECHS=[('M1 injectif',lambda l:M1(l))]
for k in (3,4,5):
    for pol,nm in (('a','uniformes'),('b','lettres rares'),('c','cases déjà utilisées')):
        MECHS.append((f'M6 nulle 1/{k} ({nm})',(lambda k,pol: lambda l:M6(l,k,pol))(k,pol)))
MECHS+=[('M7 Playfair',M7),('M11 Vigenère L3-8',M11),('M12 homophones',M12),('M15 omissions 5%',M15),('M4v Wolseley-numéro',M4v)]
M8stats=[]; MECHS.append(('M8 fractionnation',lambda l:M8(l,M8stats)))

def Dstat(prof,E): return sum((prof[r]-E[r])**2/(E[r]+1) for r in range(25))
print("RÉEL :",FR,"profil",PR[:18])
print(f"{'mécanisme':32s} {'langue':3s} {'T1 moy':>6s} {'T2 moy':>6s} {'T3 moy':>6s} {'T4 moy':>6s} {'P(T1<=18&T3>=12&T4<=1)':>24s} {'p profil':>8s}")
for name,f in MECHS:
    for l in LANGS:
        n=NEN if l=='en' else NOT; F=[]; P=[]
        for _ in range(n):
            s=f(l); fe,pr=features(s); F.append(fe); P.append(pr)
        E=[st.mean(p[r] for p in P) for r in range(25)]; do=Dstat(PR,E); ds=[Dstat(p,E) for p in P]; pv=(1+sum(x>=do for x in ds))/(n+1)
        joint=sum(f_['T1']<=18 and f_['T3']>=12 and f_['T4']<=1 for f_ in F)/n
        print(f"{name:32s} {l:3s} {st.mean(x['T1'] for x in F):6.1f} {st.mean(x['T2'] for x in F):6.1f} {st.mean(x['T3'] for x in F):6.1f} {st.mean(x['T4'] for x in F):6.1f} {joint:24.4f} {pv:8.4f}",flush=True)
if M8stats: print("M8 : fraction moyenne de paires respectant l'alternance (ligne, colonne) :",round(st.mean(M8stats),3),"; fraction des chiffrés où TOUTES la respectent :",sum(x==1.0 for x in M8stats)/len(M8stats))

# ---------- tests structurels sur le chiffré ----------
def G_indep(counts):  # table 5x5
    N=sum(sum(r) for r in counts); rs=[sum(r) for r in counts]; cs=[sum(counts[i][j] for i in range(5)) for j in range(5)]
    return 2*sum(counts[i][j]*math.log(counts[i][j]*N/(rs[i]*cs[j])) for i in range(5) for j in range(5) if counts[i][j]>0)
def table(seq):
    t=[[0]*5 for _ in range(5)]
    for s in seq: t[s//5][s%5]+=1
    return t
# S1 : indépendance ligne × colonne, p par tirage multinomial sous l'hypothèse produit (marges fixées)
T=table(REAL); g=G_indep(T); rs=[sum(r) for r in T]; cs=[sum(T[i][j] for i in range(5)) for j in range(5)]
rows=[i for i in range(5) for _ in range(rs[i])]; colsl=[j for j in range(5) for _ in range(cs[j])]; ge=0
for _ in range(5000):
    R.shuffle(colsl); t=[[0]*5 for _ in range(5)]
    for a,b in zip(rows,colsl): t[a][b]+=1
    ge+= G_indep(t)>=g
print(f"S1 indépendance ligne×colonne : G={g:.1f} ; p={(1+ge)/5001:.4f}  (faible p ⇒ PAS un produit ligne×colonne ⇒ contre la fractionnation M8)")
# S2 : chaque colonne retirée
for j in range(14):
    rest=[REAL[i] for i in range(196) if i%14!=j]; print(f"S2 colonne {j+1:2d} retirée : {len(set(rest))} symboles distincts")
# S3 : contingence position × symbole (colonnes 1-13 ; lignes 1-14), p par permutation des positions
def G_ct(labels,syms):
    c=collections.Counter(zip(labels,syms)); a=collections.Counter(labels); b=collections.Counter(syms); N=len(syms)
    return 2*sum(o*math.log(o*N/(a[x]*b[y])) for (x,y),o in c.items())
idx=[i for i in range(196) if i%14!=13]; syms=[REAL[i] for i in idx]
for nm,lab in (('colonne',[i%14 for i in idx]),('ligne',[i//14 for i in idx])):
    g0=G_ct(lab,syms); s=syms[:]; ge=0
    for _ in range(3000):
        R.shuffle(s); ge+= G_ct(lab,s)>=g0
    print(f"S3 dépendance symbole ~ {nm} (colonnes 1-13) : G={g0:.1f} ; p={(1+ge)/3001:.4f}")
