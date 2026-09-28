"""sim_7.py — quelles familles de chiffrement reproduisent la signature « 7 » de K4 ? (24/09/2026)
Clairs anglais = textes de sources/docs_utilisateur_2026_09_24/texte (transcriptions, notes Carter).
Pour chaque famille : 4 000 chiffrés de 97 lettres ; on compte les doublets, la part dans la meilleure classe mod 7,
les coïncidences à l écart 7 et les bigrammes verticaux répétés en largeur 21, comparés aux valeurs de K4."""
import numpy as np, glob, re, os
rng=np.random.default_rng(5)
txt=""
for f in glob.glob(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../sources/docs_utilisateur_2026_09_24/texte/*.txt")):
    if "Binary" in f: continue
    txt+=open(f,errors="ignore").read()
E=np.array([ord(x)-65 for x in re.sub("[^A-Z]","",txt.upper())])
print("corpus",len(E))
L=97
def eng(n=L): s=rng.integers(0,len(E)-n); return E[s:s+n].copy()
def stats(c):
    d=[i for i in range(L-1) if c[i]==c[i+1]]
    D=max(sum(1 for i in d if i%7==r) for r in range(7)) if len(d)>=4 else 0
    C=int((c[7:]==c[:-7]).sum())
    code=c[:-21]*26+c[21:]; _,cnt=np.unique(code,return_counts=True); V=int((cnt*(cnt-1)//2).sum())
    return len(d),D,C,V
def vig(p,k): return (p+k)%26
fams={}
fams["hasard uniforme"]=lambda: rng.integers(0,26,L)
fams["Vigenère clé courante anglaise"]=lambda: vig(eng(),eng())
fams["Vigenère période 7"]=lambda: vig(eng(),np.resize(rng.integers(0,26,7),L))
def ptauto():
    p=eng(); k=np.concatenate([rng.integers(0,26,7),p[:-7]]); return vig(p,k)
fams["autoclé sur le clair, écart 7"]=ptauto
def ctauto():
    p=eng(); c=np.zeros(L,int); pr=rng.integers(0,26,7)
    for i in range(L): c[i]=(p[i]+(pr[i] if i<7 else c[i-7]))%26
    return c
fams["autoclé sur le chiffré, écart 7"]=ctauto
def rowcol():
    R=rng.integers(0,26,14); Cc=rng.integers(0,26,7); k=np.array([R[i//7]+Cc[i%7] for i in range(L)]); return vig(eng(),k)
fams["clé ligne + colonne, largeur 7"]=rowcol
def prog():
    kw=rng.integers(0,26,7); s=rng.integers(1,26); k=np.array([kw[i%7]+s*(i//7) for i in range(L)]); return vig(eng(),k)
fams["clé progressive période 7"]=prog
def two_layer():
    return vig(vig(eng(),eng()),np.resize(rng.integers(0,26,7),L))
fams["clé courante + clé période 7"]=two_layer
def trans_then_run():
    p=eng(); order=rng.permutation(7); cols=[p[j::7] for j in range(7)]; t=np.concatenate([cols[j] for j in order]); return vig(t,eng())
fams["transposition colonnes (7) puis clé courante"]=trans_then_run
def run_then_rows():
    # clé courante, puis écrit en colonnes de hauteur 14 et lu par lignes de 7 (transposition finale)
    c=vig(eng(),eng()); M=np.full(98,-1); idx=0
    order=rng.permutation(7); grid=-np.ones((14,7),int); k=0
    for j in order:
        for r in range(14):
            if r*7+j<L: grid[r,j]=c[k]; k+=1
    return grid.flatten()[:L]
fams["clé courante puis colonnes→lignes (7)"]=run_then_rows
def p7_then_rows():
    c=vig(eng(),np.resize(rng.integers(0,26,7),L)); order=rng.permutation(7); grid=-np.ones((14,7),int); k=0
    for j in order:
        for r in range(14):
            if r*7+j<L: grid[r,j]=c[k]; k+=1
    return grid.flatten()[:L]
fams["période 7 puis colonnes→lignes (7)"]=p7_then_rows
K=np.array([ord(x)-65 for x in "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"])
print("K4 :",stats(K),"(doublets, doublets dans la meilleure classe mod 7, coïncidences écart 7, bigrammes verticaux largeur 21)")
Ns=4000
print(f"{'famille':48s} doubl. D7>=5 C7moy C7>=9 V21moy V21>=11 D&C&V")
for name,f in fams.items():
    A=np.array([stats(f()) for _ in range(Ns)])
    j=np.mean((A[:,1]>=5)&(A[:,2]>=9)&(A[:,3]>=11))
    print(f"{name:48s} {A[:,0].mean():5.2f} {np.mean(A[:,1]>=5):6.4f} {A[:,2].mean():5.2f} {np.mean(A[:,2]>=9):6.4f} {A[:,3].mean():5.2f} {np.mean(A[:,3]>=11):7.4f} {j:.4f}")
