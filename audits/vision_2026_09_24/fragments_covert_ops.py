"""Deux chiffres de Sanborn postérieurs à Kryptos, vérifiés (base 7 §9.3 et §9.4).

Sources publiques : pages « COFzola » et « COFeng » du site SciRealm, et kryptos.yak.net/41.
Messages du groupe : 29/12/2003 (Zola) et 01/11/2005 (petit fragment anglais).

1. Fragment « Covert Operations » du restaurant Zola, 16 lignes. Transposition en 16 colonnes lues de droite à
   gauche ; les lettres répétées en fin de ligne servent de remplissage.
2. Petit fragment anglais, 97 lettres, vu dans l'atelier de Sanborn le 29/10/2005. Méthode de K1 : Quagmire III,
   alphabet KRYPTOS, clé SHADOW. On relève les erreurs de chiffrement.
"""
from math import comb

KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
KI = {c: i for i, c in enumerate(KA)}

# ---------- 1. Zola ----------
ZOLA = """DINEHYODTTSATEOSLNSSEBDBHRTSODULL EGTTPECLEEHTFESOATPSAYOAUMEWTBAWW
NHOWSOUNPBDEHRSLDESESNLKPLEHEIWYY PHSRADDSEICADSEDEOIEGWRPRNIDOTWEE
ELBAEAEALGSTOHASRELPWSACEHIHRATPP EAEPEECEFAVEWTCEDLFLHMHATNEWAEHR
IIOIAPSSHDMHYNEANTUEDTHFASOOWCDTT ITLDJYOANIMLEUTECLGAUAWRDMEERMHLL
OEASSNEACANDEBEYUISATDDSODTORERSSS LIEPCNTAREHOERELDNSRLOHNNAARMEMOO
SSAEETAIETDCODAWEENCAFTRRIALVTILLLL ERDARIWODNREEHNYNTTUELHNAYRADESIII
OEHTDLATRBIRULOTTDEICETEKRVBNOFSSS HTNOADESNEEETWPEIHWNNLVTEADFIEAEEE
NEIADIEISCWOLRSMGCDGLKRNSHIRAPSEEE XAEAEELGAITETAADXNNITTARNNHULRGTT""".split()
print("Zola : longueurs des lignes", [len(x) for x in ZOLA])
print("  remplissage retiré (au-delà de 32) :", [x[32:] for x in ZOLA])
ct = "".join(x[:32] for x in ZOLA)
grid = [ct[i:i + 16] for i in range(0, len(ct), 16)]
pt = "".join("".join(r[c] for r in grid) for c in range(15, -1, -1))
print("  512 lettres ; clair (16 colonnes lues de droite à gauche) :")
print("  ", pt[:120], "…", pt[-60:])

# ---------- 2. Petit fragment anglais, 97 lettres ----------
ROWS = ["RAJIRBAPKMQJDZKHQYZQVJTEL", "QEEDJNLFZXREKUOWTZZGGQYZ",
        "UQUGMDPEZETZZOLMDLIHHQYZ", "MFJFZDYHHDJWJMEBFEEHWJSE"]
C = "".join(ROWS)
P = "UNCOVERINGINTENTIONSISTHESTRONGPOINTOFHUMANESPIONAGEBUTITISANEXAGGERATIONTOSAYTHATONLYESPIONAGECO"
KEY = "SHADOW"
print("\nPetit fragment : lignes", [len(r) for r in ROWS], "=", len(C), "lettres ; clair", len(P))
dec = "".join(KA[(KI[c] - KI[KEY[i % 6]]) % 26] for i, c in enumerate(C))
print("  déchiffré tel que gravé :", dec)
errs = []
for i in range(97):
    exp = KA[(KI[P[i]] + KI[KEY[i % 6]]) % 26]
    if exp != C[i]:
        d = (KI[C[i]] - KI[exp]) % 26
        errs.append((i, P[i], KEY[i % 6], exp, C[i], d if d <= 13 else d - 26))
print("  erreurs (position 0-base, clair, lettre de clé, chiffré attendu, gravé, écart dans l'alphabet KRYPTOS) :")
for e in errs:
    print("   ", e)
# trois des quatre écarts sont égaux : probabilité pour des erreurs d'écart uniforme parmi 25 valeurs non nulles
q = 1 / 25
p3 = sum(comb(4, k) * q ** (k - 1) * (1 - q) ** (4 - k) for k in (3, 4))  # au moins 3 égaux, valeur commune libre
print(f"  P(au moins 3 des 4 écarts égaux | écarts au hasard) ≈ {p3:.4f}")
print("  lecture : aux positions 9 et 87, la clé D a été remplacée par la lettre suivante de SHADOW (O) ;")
print("  en 22, le clair T a été recopié sans chiffrement (gravé T = clair T, soit la clé K) ; en 91, clé M au lieu de H.")
# que resterait-il des cribs de K4 avec le même taux d'erreurs ?
p0 = comb(97 - 24, 4) / comb(97, 4)
print(f"\nSi K4 avait 4 erreurs sur 97 placées au hasard : P(aucune dans les 24 lettres des cribs) = {p0:.2f}")
