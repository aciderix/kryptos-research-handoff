#!/usr/bin/env python3
"""H1 (échange chef+MECA) : contraindre l'alphabet libre par le PRIOR de Bean « alphabet proche de A-Z ».
Classe = permutations à distance de transposition <= k de l'identité (A-Z + k swaps). ÉNUMÉRABLE (k<=2 : 50k).
Ni mot-clé (éliminé T27/T34) ni libre (intractable T36b). Attaque = ÉNUMÉRATION (pas de recherche) :
pour chaque σ (et τ) de la classe, on DÉRIVE κ des cribs, on déchiffre les 97 lettres, on compte les erreurs
de crib (tolérance Sanborn 1-2) et on score les quadrigrammes hors cribs. Contrôle positif + témoins K4-mélangé.

Réserve d'antériorité (honnête) : T18 élimine l'autoclé écart-7 pour TOUT alphabet SANS erreur -> H1 ne vit
qu'avec >=1 erreur ; et le prior « proche de A-Z » est FAIBLE (base 09 §4 : 0.3-5.6% corrigé). H1 teste si,
MALGRÉ ça, la classe énumérable contient un clair anglais pour K4 au-dessus des témoins.

Usage: h1_bean_prior.py qg.bin CT [mode=tie|indep] [kmax] [emax]
"""
import sys, struct
from itertools import combinations

N, LAG = 97, 7
CE = list(range(21, 34)); CB = list(range(63, 74)); CRIBPOS = CE + CB
CRIBPT = "EASTNORTHEAST" + "BERLINCLOCK"
PTAT = {p: (ord(CRIBPT[i]) - 65) for i, p in enumerate(CRIBPOS)}
ISCRIB = [1 if i in set(CRIBPOS) else 0 for i in range(N)]

def load_qg(path):
    d = open(path, "rb").read()
    return struct.unpack("<%df" % (len(d) // 4), d)

def class_within_k(k):
    seen = {tuple(range(26))}
    frontier = [tuple(range(26))]
    for _ in range(k):
        nf = []
        for p in frontier:
            for i, j in combinations(range(26), 2):
                q = list(p); q[i], q[j] = q[j], q[i]; q = tuple(q)
                if q not in seen:
                    seen.add(q); nf.append(q)
        frontier = nf
    return list(seen)

def derive_kappa_and_decrypt(CT, sig, tau):
    """sig,tau : tuples (sig[letter]=value). Dérive κ par classe depuis le 1er crib, déchiffre les 97."""
    siginv = [0] * 26
    for L, v in enumerate(sig): siginv[v] = L
    taul = [tau[ord(c) - 65] for c in CT]   # tau(ct[i])
    byc = {}
    for p in CRIBPOS: byc.setdefault(p % LAG, []).append(p)
    kappa = {}
    for r, ps in byc.items():
        f = min(ps); idxs = list(range(r, f + 1, LAG))
        coefK = 0; const = 0
        for pos_i, pos in enumerate(idxs):
            tval = taul[pos]
            if pos_i == 0: const = tval; coefK = -1
            else: const = tval - const; coefK = -coefK
        target = sig[PTAT[f]]
        kappa[r] = ((target - const) * (1 if coefK == 1 else -1)) % 26
    x = [0] * N
    for i in range(N):
        kk = kappa[i % LAG] if i < LAG else x[i - LAG]
        x[i] = (taul[i] - kk) % 26
    return [siginv[x[i]] for i in range(N)]

def crib_errors(pt):
    return sum(1 for p in CRIBPOS if pt[p] != PTAT[p])

def qoff(qg, pt):
    s = 0.0; n = 0
    for i in range(N - 3):
        if not any(ISCRIB[i + d] for d in range(4)):
            s += qg[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; n += 1
    return s / n if n else 0.0

def attack(qg, CT, mode, kmax, emax, base="ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
    # base[L] = valeur (position) de la lettre L dans l'alphabet mixte de base (A-Z ou KRYPTOS-keyed).
    bp = [base.index(chr(65 + L)) for L in range(26)]
    raw = class_within_k(kmax)
    # compose : alphabet = base perturbé par k swaps de valeurs -> sig[L] = swap[bp[L]]
    cls = [tuple(swap[bp[L]] for L in range(26)) for swap in raw]
    best = None
    if mode == "tie":
        for sig in cls:
            pt = derive_kappa_and_decrypt(CT, sig, sig)
            e = crib_errors(pt)
            if e <= emax:
                q = qoff(qg, pt)
                if best is None or q > best[0]: best = (q, e, "".join(chr(65 + c) for c in pt))
    else:  # indep : σ dans la classe, τ dans la classe (peut être gros)
        for sig in cls:
            for tau in cls:
                pt = derive_kappa_and_decrypt(CT, sig, tau)
                e = crib_errors(pt)
                if e <= emax:
                    q = qoff(qg, pt)
                    if best is None or q > best[0]: best = (q, e, "".join(chr(65 + c) for c in pt))
    return best, len(cls)

if __name__ == "__main__":
    qg = load_qg(sys.argv[1]); CT = sys.argv[2]
    mode = sys.argv[3] if len(sys.argv) > 3 else "tie"
    kmax = int(sys.argv[4]) if len(sys.argv) > 4 else 2
    emax = int(sys.argv[5]) if len(sys.argv) > 5 else 2
    base = sys.argv[6] if len(sys.argv) > 6 else "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    best, nc = attack(qg, CT, mode, kmax, emax, base)
    if best:
        print("mode=%s kmax=%d emax=%d classe=%d : MEILLEUR qoff=%.4f erreurs=%d\n  PT=%s" % (mode, kmax, nc, emax, best[0], best[1], best[2]))
    else:
        print("mode=%s kmax=%d emax=%d classe=%d : AUCUN alphabet ne passe <=%d erreurs de crib" % (mode, kmax, nc, emax, emax))
