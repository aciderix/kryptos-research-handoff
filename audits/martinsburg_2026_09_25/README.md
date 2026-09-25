# Martinsburg (IRS, Sanborn 1999) : le binaire est de l'ASCII 7 bits (25/09/2026)

**Question** (base 6 §2.9, « ouvert ») : les 17 lignes binaires du mur de cuivre de Martinsburg sont-elles un « changement de base » propre à Sanborn, qui éclairerait le « masque » de K4 ?

**Données** : relevé de D. Wilson (2003), `sources/docs_utilisateur_2026_09_24/BinarySystems.txt` (même fichier dans le groupe, *Other Sanborn Artwork/Binary Systems*). À gauche, 17 lignes de noms (7 de présidents, 10 de secrétaires au Trésor) ; à droite, 17 lignes binaires alignées à droite, coupées à gauche.

**Méthode** (`decode.py`) : largeur de 5 à 8 bits ; A = 0, A = 1 ou ASCII ; ordre des bits normal ou inversé ; complément ; lecture en miroir ; tous les décalages ; comparaison à chaque ligne de noms, à l'endroit et à l'envers, avec ou sans espaces et virgules.

**Résultat** (`ascii7.py`, `results_ascii7.txt`) : **les 17 lignes sont de l'ASCII 7 bits ordinaire**, sans aucune transformation. Chaque ligne binaire code le début de la ligne de noms qui lui fait face, **virgules gardées, espaces retirés** :

| ligne | lu | noms |
|---|---|---|
| 1 | ASHINGTO | WASHINGTON, ADAMS… |
| 2 | ACKSON,V | JACKSON, VAN BUREN… |
| 4 | RFIELD, | GARFIELD, ARTHUR… |
| 8 | MILTON, | HAMILTON, WOLCOTT… |
| 13 | N,WINDOM | (SHER)MAN, WINDOM… |
| 17 | NNEDY,C | KENNEDY, CONNALLY… |

(les 17 lignes dans le fichier de résultats). Une seule anomalie : ligne 9, AUFORD au lieu de AWFORD (CRAWFORD), soit **un seul bit** (U = 1010101, W = 1010111) : erreur de gravure ou de relevé.

**Portée pour K4.**
- Le « changement de base » réellement exécuté par Sanborn est un **code public** (ASCII), pas un chiffre. Rien ne suggère une recette pour K4.
- Les 5 bits de poids faible de l'ASCII des majuscules sont exactement le codage A = 1 ; le XOR et l'addition sur ce codage sont déjà testés pour K4 (`../binary_xor_2026_09_23/`).
- Détail : en 2022, Sanborn disait n'avoir « aucune idée de ce qu'est l'ASCII » (base 7 §11.6) ; le codage de 1999 a sans doute été fait par un fabricant ou un assistant.
- Même dans ce travail soigné, une erreur d'un bit s'est glissée : un exemple de plus d'erreurs d'exécution dans les œuvres de Sanborn.
