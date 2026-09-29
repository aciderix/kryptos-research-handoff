# K13 — Transpositions historiques non encore couvertes : nihiliste, Myszkowski, AMSCO — PRÉ-INSCRIPTION

Rédigée et commitée avant tout calcul (2026-09-29).

## Recensement (déjà couvert, pour mémoire)
Routes (colonnes, zigzags/boustrophédon, diagonales, spirales, toutes largeurs : K06, K07), colonnes à clé (carrés : K03 ;
texte entier 5-40 colonnes, ± substitution : K07), double transposition en colonnes (3-9 : K08), grille tournante (K05), barrière et
décimation (K09). **Trous restants** couverts ici :
- **N — transposition nihiliste** : carré q×q, lignes **et** colonnes permutées par la même clé, lecture en lignes ou en colonnes.
- **M — Myszkowski** : colonnes à clé avec rangs répétés ; les colonnes de même rang sont lues ensemble ligne par ligne.
- **A — AMSCO** : cases alternées de 1 et 2 lettres (départ 1 ou 2), colonnes lues dans l'ordre de la clé.

## Portées et scores
- N : blocs S2, S4, S5 (q = 13), S6 (q = 12) ; score quadrigrammes allemands (lettres intactes) — le MI est sans puissance sur les
  blocs (K07).
- M : texte entier (979), largeurs 5-15 ; A : texte entier, largeurs 3-12, départ 1 ou 2. Deux scores : quadrigrammes allemands
  (lettres intactes) et MI(1) (substitution quelconque en plus, toute langue).
Recherche : recuit / montée avec redémarrages (`tools/k13_transpositions.c`), effort fixé sur les contrôles puis gelé.

## Contrôles (avant le texte réel)
Pour chaque famille et chaque score : 10 textes allemands réservés (sous substitution aléatoire pour le score MI), paramètres et
clés aléatoires dans la portée, paramètres **non fournis** à la recherche ; succès = ≥ 90 % des lettres voisines recollées ; exigé
≥ 8/10, sinon la combinaison famille × score est déclarée **sans puissance** (rapporté, non interprété).
Nuls : lettres de la suite réelle mélangées, même recherche (30 pour le texte entier, 50 pour les blocs).

## Décision
Une suite répond si son meilleur score dépasse tous ses nuls **et** atteint un niveau de langue (quadrigrammes ≥ −10,5 ; MI ≥ 0,7).
Aucune revendication sans texte lisible. Un négatif exclut la famille dans sa portée, pas ses variantes hors portée.
