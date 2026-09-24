# « Changer la base du langage » : codage 5 bits + XOR / addition mod 32 (relais Gemini, 23/09)

Paramètres fixés avant calcul (`binary_xor.py`) : codages A=1…26, A=0…25, Baudot ITA2 ; opérations XOR, addition et soustraction mod 32 ;
clé périodique p = 1–48 ; cohérence exacte sur les 24 lettres des cribs ; témoin : 200 chiffrés aléatoires.
Hors des périodes triviales 27–29 (où les cribs ne partagent aucun résidu), ce montage n'est **pas** couvert par les éliminations Quagmire : un XOR n'est pas un décalage.

**Résultats**
- Tous les codages et opérations : seules les périodes triviales 27–29 passent, **sauf ITA2 + XOR, p = 30** (hasard : 3,5 % à cette période).
- Coïncidences attendues au hasard sur l'ensemble des 9 cases : **0,54 par chiffré** ⇒ un passage isolé est banal.
- Déchiffrement de K4 avec la clé p = 30 : `AYK#W#ZCII#UES…NN#IJQ##XS…` ; **13 codes non-lettres sur 76** positions (un vrai clair Baudot n'en aurait aucun).
- ⇒ **éliminé** : pas de clé périodique 5 bits (XOR ou addition) pour p ≤ 48, hors 27–29.
- Les flux de clé bruts (24 valeurs) sont dans `results.json` ; aucun motif visible.
