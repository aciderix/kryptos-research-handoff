"""K4 seul : e_min de l'autoclé sur le chiffré pour tous les écarts L = 1..96, 9 variantes."""
import json
from t23_autocle_chiffre import emin_ak
from emin_cpsat import CT
ct = [ord(x) - 65 for x in CT]
for L in range(1, 97):
    for kr in ("sig", "AZ", "KA"):
        for mode in ("VIG", "BEAU", "VARB"):
            r = emin_ak(ct, L, mode, kr, timeout=120)
            print(json.dumps(dict(L=L, key=kr, mode=mode, n=r[2], emin=r[0], drop=r[1])), flush=True)
