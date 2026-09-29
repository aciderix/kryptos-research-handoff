# K15 — Chiffrement par blocs avec **clé commune**, scores additionnés sur les 6 blocs — PRÉ-INSCRIPTION

Rédigée et commitée avant tout calcul (2026-09-29).

## Idée
Les 6 blocs (lettre soulignée + point ; 166, 169, 162, 169, 169, 144 lettres) sont la seule structure indépendante de l'habillage
(K10-K11). Un auteur qui chiffre bloc par bloc réutilise normalement la même clé. Sous cette hypothèse, on **additionne** l'évidence
des 6 blocs : le score MI (invariant par substitution) retrouve la puissance du texte entier, alors qu'il était sans puissance bloc
par bloc (K07) ; et des familles jamais testées par bloc deviennent accessibles.

## Familles (même clé et mêmes paramètres pour les 6 blocs ; chaque bloc chiffré indépendamment)
- **C** : colonnes à clé, largeur w = 5..20 (lignes incomplètes admises) — par bloc, seule la largeur 13/12 avait été testée (K03).
- **U** : même clé deux fois (Übchi) par bloc, w = 5..15.
- **D** : double transposition par bloc, deux clés distinctes, w1, w2 = 5..13.
- **F** : grille tournante 13×13 commune à S2, S4, S5 (blocs de 169 lettres ; S6 = 12×12 exclu faute de grille commune).
- **R** : routes du catalogue K06 (même route, même largeur, même sens pour tous les blocs).
Scores : **MI(1) additionné** (tables de paires des 6 blocs cumulées ; substitution quelconque, toute langue) et **quadrigrammes
allemands moyens** (lettres intactes). Outils : `tools/k15_blocs.c`, `tools/k15_routes.py`.

## Contrôles (avant le texte réel)
Pour chaque famille × score : 10 textes allemands réservés découpés en blocs de mêmes tailles, chiffrés bloc par bloc avec une clé
commune aléatoire (et une substitution aléatoire pour le score MI) ; paramètres non fournis ; succès = ≥ 90 % des lettres voisines
recollées (dans chaque bloc). Exigé ≥ 7/10, sinon « sans puissance » (non interprété).
Nuls : lettres mélangées **à l'intérieur de chaque bloc**, même recherche (30 ; 100 pour R).

## Décision
Répond si le meilleur score dépasse tous les nuls et atteint un niveau de langue (MI additionné ≥ 0,7 ; quadrigrammes ≥ −10,5) ;
revendication seulement sur texte lisible.
