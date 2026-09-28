"""Témoins pour l'écart 7 : e_min de l'autoclé sur le chiffré pour 60 chiffrés aléatoires complets, 9 variantes."""
import json, random
from t23_autocle_chiffre import emin_ak
rnd = random.Random(23)
for kr in ("sig", "AZ", "KA"):
    for mode in ("VIG", "BEAU", "VARB"):
        h = {}
        for z in range(60):
            rc = [rnd.randrange(26) for _ in range(97)]
            e = emin_ak(rc, 7, mode, kr, timeout=60)[0]
            h[str(e)] = h.get(str(e), 0) + 1
        print(json.dumps(dict(L=7, key=kr, mode=mode, null=h)), flush=True)
