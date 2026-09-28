# E12 — Calibration : les mécanismes du livre, appliqués à de vrais textes, produisent-ils le profil observé ? — PRÉ-INSCRIPTION

Rédigée et commitée avant le calcul (2026-09-28). Répond à la relecture critique d'E11 : E11 ne rejette le clair
linguistique que pour « une paire = une lettre » ; il faut calibrer les mécanismes **documentés** (`docs/02_mecanismes_du_livre.md`).

## Traits observés (cibles, définis d'avance)
T1 symboles distincts sur 196 = 18 ; T2 symboles distincts sur la zone de 182 (colonne la plus « rare » retirée,
voir T5) = 13 ; T3 nombre de symboles d'effectif ≥ 11 (sur 196) = 12 ; T4 nombre d'effectif 3-8 (sur 196) = 1 ;
T5 : il existe une colonne (sur 14) qui contient **tous** les symboles absents du reste. Statistique de profil :
D (E11) contre le profil observé.

## Mécanismes simulés (sur textes réels de 8 langues, textes réservés `data/heldout/`)
Chacun produit 196 paires rangées en grille 14×14 (lecture par lignes ; la transposition ne change pas les comptes).
- M1 : substitution injective (témoin).
- M6 : insertion d'une nulle toutes les k lettres (k = 3, 4, 5) après chiffrement ; nulles tirées (a) uniformément
  parmi les 25 cases, (b) parmi les cases des lettres rares du carré, (c) parmi les cases déjà utilisées par le clair.
- M7 : Playfair (carré à mot-clé tiré des mots du livre) puis coordonnées.
- M11 : Vigenère sur l'alphabet de 25 lettres, clé de longueur 3-8, puis coordonnées.
- M12 : homophones de voyelles (chaque voyelle répartie sur 2-3 cases), + une factice entre les mots.
- M15 : omissions aléatoires de 5 % des lettres.
- M4v : Wolseley « numéro » : carré à mot-clé (mots du livre), cases appariées par symétrie centrale ; chaque
  lettre est codée par la case **canonique** de sa paire (13 classes).
- M8 : fractionnation (coordonnées du carré transposées une par une par une colonnaire à mot-clé, largeur 5-11),
  paires recomposées : on mesure aussi le taux de paires respectant l'alternance.
Tirages : 2 000 par (mécanisme, variante) en anglais, 300 par autre langue.

## Tests structurels sur le chiffré (sans simulation de langue)
S1 indépendance ligne × colonne de la table 5×5 (G-test, p par permutation des symboles) — attendue sous M8.
S2 pour chacune des 14 colonnes retirée : nombre de symboles distincts restants et ajustement uniforme (E11).
S3 contingence position × symbole : le symbole dépend-il de l'indice de colonne (1-13) ou de ligne (1-14) ?
(G-test, p par permutation des positions.)

## Décision (par mécanisme)
- « Peut produire le profil » si P(T1 ≤ 18 **et** T3 ≥ 12 **et** T4 ≤ 1) ≥ 0,01 **ou** si la p-valeur du test de
  profil D dépasse 0,05, dans au moins une langue et une variante.
- Sinon « ne peut pas le produire » (à la puissance des tirages).
- Si **aucun** mécanisme documenté ne le produit, la conclusion d'E11 se renforce (sans devenir une preuve : des
  mécanismes non documentés restent possibles) ; si l'un le produit, il devient la piste prioritaire (pré-inscription
  d'une attaque dédiée).
