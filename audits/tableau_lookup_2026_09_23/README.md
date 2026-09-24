# Lettre du tableau à la même position physique comme SÉLECTEUR de clé (relais « deux couches », 23/09)
Hypothèse : la lettre du tableau H(i), au dos à la même position (écran replié FOLD, ou vu en miroir FRONT_MIRROR ; pied de tableau avec ou sans blanc ;
décalage de ligne d = −2…+2), n'est pas elle-même le décalage mais une **entrée** : clé(i) = g(H(i)), g inconnue (une valeur par lettre du tableau, jamais par position).
C'est plus général que la « fermeture » du 23/09 (`../fold_overlay_2026_09_23/`), où la lettre du tableau *était* la clé.
Chiffrement : Quagmire III (tout alphabet) et IV (deux alphabets quelconques), Vig/Beau ; exact sur les 24 lettres ; témoins 50 chiffrés aléatoires.

**Résultats**
- Puissance faible (hasard 12–54 % par case) : sous les cribs, les lettres du tableau ne se répètent que 9 à 10 fois, et g absorbe le reste.
- QIII : K4 compatible dans 11 cases sur 24, pour **10,5 attendues** au hasard.
- QIV : K4 compatible dans les 24 cases ; mais **49 chiffrés aléatoires sur 200 (25 %) le sont aussi, dans toutes** (`qiv_all.py`).
- ⇒ **aucun signal**, et famille **peu décidable** avec 24 lettres : on ne peut ni la confirmer ni l'exclure fermement. Il faudrait plus de clair connu (K5, nouveaux cribs).
