"""maquette_1988_erreurs.py — le bloc chiffré de démonstration de la maquette GIRASOL de 1988 (7 × 21), relu entièrement.
Relevé : ../vision_2026_09_24/data/girasol_ct7x21.txt (P. Kiesel, d'après les relevés de 2015). Vigenère A–Z, clé RUG (période 3).
La reconstitution notait « dernière ligne altérée au relevé ». En fait, la dernière ligne se lit : il manque une lettre au chiffré
(le T de « THE »), et toute la suite de la clé glisse d'un rang. On relève aussi toutes les lettres fausses et leur type.
"""
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
ct = "".join(open("../vision_2026_09_24/data/girasol_ct7x21.txt").read().split())
key = "RUG"
d = lambda c, k: AZ[(AZ.index(c) - AZ.index(k)) % 26]
e = lambda p, k: AZ[(AZ.index(p) + AZ.index(k)) % 26]
# déchiffrement avec un glissement de phase de +1 à partir de la position 133 (lettre omise après « WITHOUT »)
pt = "".join(d(c, key[(i + (1 if i >= 133 else 0)) % 3]) for i, c in enumerate(ct))
for r in range(7):
    print(r, ct[21 * r:21 * r + 21], pt[21 * r:21 * r + 21])
print("\nlecture : CODES MAY BE DIVIDED INT(O) TWO DIFFERENT CLASSES NAMELY SUBSTITUTIONAL AND TRANSPOSITIONAL TYPES THE")
print("          TRANSPOSITIONAL BEING THE HARDEST TO DEC(I)PHER W(I)THOUT (T)HE KEY ETRANS…")
# lettres fausses : clair attendu -> chiffré attendu, et ce qui est gravé
attendu = {20: "O", 121: "I", 127: "I"}
for i, p in attendu.items():
    k = key[i % 3]
    print(f"position {i} : clair {p}, clé {k} : chiffré attendu {e(p, k)}, relevé {ct[i]} → se lit {d(ct[i], k)}")
print("\n=> 121 et 127 : la même erreur deux fois (clair I, clé U : B au lieu de C, une case trop à gauche) ;")
print("   133 : une lettre omise ; la suite de la clé glisse d'un rang (comme l'X omis de K2) ;")
print("   20 : INTU au lieu de INTO (faute du clair, ou lecture du relevé).")
