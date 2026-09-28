"""t24_perline.py — clé de période p + décalage de l'alphabet à chaque ligne (cuivre : lignes de 31, K4 en colonne 27).
Compatibilité exacte (e_min) de K4 et des témoins, Quagmire I–IV, VIG/BEAU."""
import sys, random, json
from emin_cpsat import emin, CRIBPT, K4C
import families as FM
NN = int(sys.argv[1]) if len(sys.argv) > 1 else 100
ps = list(map(int, sys.argv[2].split(","))) if len(sys.argv) > 2 else list(range(1, 15))
W, o = (int(sys.argv[3]), int(sys.argv[4])) if len(sys.argv) > 4 else (31, 27)
rnd = random.Random(11)
for p in ps:
    for q, mode in FM.QM:
        fam = FM.get2(f"perline:{p}:{W}:{o}")
        e, drop, st = emin(fam, CRIBPT, K4C, q, mode, timeout=60)
        hist = {}
        for z in range(NN):
            cc = [rnd.randrange(26) for _ in range(24)]
            ez, _, s2 = emin(fam, CRIBPT, cc, q, mode, timeout=30)
            if isinstance(ez, tuple): ez = ez[0]
            hist[ez] = hist.get(ez, 0) + 1
        le = sum(v for k, v in hist.items() if k is not None and k <= (e if not isinstance(e, tuple) else e[0]))
        print(json.dumps(dict(p=p, W=W, o=o, q=q, mode=mode, emin=e, drop=drop, st=st, null={str(k): v for k, v in sorted(hist.items(), key=lambda x: (x[0] is None, x[0]))}, p_le=le / NN)), flush=True)
