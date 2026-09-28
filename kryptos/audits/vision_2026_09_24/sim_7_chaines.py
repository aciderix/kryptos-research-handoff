"""sim_7_chaines.py — chaînes de doublets à l écart 7 (i, i+7, i+14) sous l autoclé sur le clair à l écart 7 (24/09/2026).
Dans cette famille, un doublet chiffré en i dépend de l égalité des pas clairs en i−7 et i : les doublets à 7 d écart sont liés.
K4 a une chaîne de trois doublets (18, 25, 32). 20 000 chiffrés par famille, clairs anglais des textes versés."""
import numpy as np, glob, re
rng=np.random.default_rng(11)
txt=""
for f in glob.glob(__import__("os").path.join(__import__("os").path.dirname(__import__("os").path.abspath(__file__)),"../../sources/docs_utilisateur_2026_09_24/texte/*.txt")):
    if "Binary" in f: continue
    txt+=open(f,errors="ignore").read()
E=np.array([ord(x)-65 for x in re.sub("[^A-Z]","",txt.upper())]); L=97
def eng(n=L): s=rng.integers(0,len(E)-n); return E[s:s+n].copy()
def stats(c):
    d=set(i for i in range(L-1) if c[i]==c[i+1])
    D=max(sum(1 for i in d if i%7==r) for r in range(7)) if len(d)>=4 else 0
    chain=any((i in d and i+7 in d and i+14 in d) for i in range(L-15))
    C=int((c[7:]==c[:-7]).sum())
    return len(d),D,chain,C
def perm(): return rng.permutation(26)
def ptauto(mode,alpha):
    def f():
        s=perm() if alpha=="rand" else np.arange(26); inv=np.argsort(s)
        p=eng(); ps=s[p]; pr=rng.integers(0,26,7); k=np.concatenate([pr,ps[:-7]])
        cs=(ps+k)%26 if mode=="VIG" else ((k-ps)%26 if mode=="BEAU" else (ps-k)%26)
        return inv[cs]
    return f
fams={"uniforme":lambda: rng.integers(0,26,L),
      "Vigenère période 7":lambda: (eng()+np.resize(rng.integers(0,26,7),L))%26}
for m in ("VIG","BEAU","VARB"):
    for a in ("AZ","rand"): fams[f"autoclé clair écart 7 {m} {a}"]=ptauto(m,a)
K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
print("K4",stats(np.array([ord(x)-65 for x in K4])))
N=20000
print(f"{'famille':38s} doubl  P(D7>=5) P(chaîne 3) P(C7>=9) P(D7>=5 & C7>=9)")
for n,f in fams.items():
    A=[stats(f()) for _ in range(N)]
    nd=np.mean([a[0] for a in A]); pD=np.mean([a[1]>=5 for a in A]); pc=np.mean([a[2] for a in A]); pC=np.mean([a[3]>=9 for a in A]); pj=np.mean([a[1]>=5 and a[3]>=9 for a in A])
    print(f"{n:38s} {nd:5.2f}  {pD:.4f}   {pc:.4f}     {pC:.4f}   {pj:.5f}")
