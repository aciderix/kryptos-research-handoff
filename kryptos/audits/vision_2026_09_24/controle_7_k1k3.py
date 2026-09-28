"""controle_7_k1k3.py — le « 7 » existe-t-il aussi dans K1, K2, K3 ? (contrôle, 24/09/2026)
Mêmes statistiques que pour K4 (coïncidences par écart, doublets par classe de résidus), contre 4 000 mélanges de chaque texte.
Si le « 7 » venait d une habitude de Sanborn (feuilles en groupes de 7) ou de la gravure, il apparaîtrait ailleurs."""
import numpy as np
rows=open(__import__("os").path.join(__import__("os").path.dirname(__import__("os").path.abspath(__file__)),"data/panel_wikipedia.txt")).read().split()
full="".join(rows)
i_k3=full.index("ENDYAHR"); i_k4=full.index("OBKRUOX")
k1k2=full[:i_k3]; K1=k1k2[:63].replace("?",""); K2=k1k2[63:].replace("?",""); K3=full[i_k3:i_k4].replace("?",""); K4=full[i_k4:]
print(len(K1),len(K2),len(K3),len(K4))
rng=np.random.default_rng(3)
def dstat(a,m):
    d=[i for i in range(len(a)-1) if a[i]==a[i+1]]
    if len(d)<4: return 0.0,len(d)
    return max(sum(1 for i in d if i%m==r) for r in range(m))/len(d),len(d)
def cstat(a,m): return int((a[m:]==a[:-m]).sum())
for name,s in (("K1",K1),("K2",K2),("K3",K3),("K4",K4)):
    a=np.array([ord(x)-65 for x in s]); n=len(a)
    S=[rng.permutation(a) for _ in range(4000)]
    # profil des coïncidences : 3 écarts les plus excédentaires
    ex=[]
    for m in range(1,min(49,n//2)):
        k=cstat(a,m); z=np.array([cstat(t,m) for t in S]); ex.append((np.mean(z>=k),m,k,z.mean()))
    ex.sort()
    print(name,"n=",n,"| écarts les plus excédentaires :",[(m,k,round(mu,1),round(p,4)) for p,m,k,mu in ex[:4]])
    k7=cstat(a,7); z7=np.array([cstat(t,7) for t in S])
    f,nd=dstat(a,7); zf=np.array([dstat(t,7)[0] for t in S])
    print(f"   écart 7 : {k7} (moy {z7.mean():.1f}, P {np.mean(z7>=k7):.3f}) ; doublets {nd}, part max mod 7 = {f:.2f} (P {np.mean(zf>=f):.3f})")
    for m in range(2,15):
        f,nd=dstat(a,m); zf=np.array([dstat(t,m)[0] for t in S[:2000]]); p=np.mean(zf>=f)
        if p<0.05: print(f"   doublets mod {m} : part {f:.2f} sur {nd}, P {p:.3f}")
