# E06 — Lecture verticale des colonnes imprimées, ordres à clé des lignes paires et impaires — PRÉ-INSCRIPTION

Rédigée et commitée avant tout run de E06 sur le vrai chiffré (2026-09-28). Statut : cellule **exploratoire**
destinée à expliquer une OBSERVATION (pas une hypothèse historique documentée).

## Observation à expliquer (E04, § 6)
En géométrie B, la colonnaire de largeur 7, clé `7 3 6 1 4 2 5`, donne R = 46 (null ≤ 41 sur 46 mélanges) et deux
6-grammes répétés. Cette clé équivaut à lire chaque **colonne imprimée verticalement**, sur les lignes impaires
(1-indexées) dans l'ordre 7, 11, 3, 9, 13, 5, 1, colonne après colonne, puis sur les lignes paires dans l'ordre
8, 12, 4, 10, 14, 6, 2 (même ordre des paires de lignes). E05 : aucune des 6 autres langues n'en tire un texte
(résultats partiels au moment de la rédaction).

## Famille P (fermée) — `tools/e04_scan.c`, FAM=P
Grille imprimée (géométrie B : 14 × 13 après retrait de la colonne 14 ; A : 14 × 14). Lignes 0-indexées paires
lues dans l'ordre sE (7! possibilités), impaires dans l'ordre sO (7!) ⇒ **7!² ≈ 2,54·10⁷ clés par mode** :
- P0 : toutes les colonnes sur les lignes paires, puis toutes sur les impaires (généralise la largeur 7 : sE = sO) ;
- P1 : colonne par colonne, paires puis impaires ; P2 : colonne par colonne, impaires puis paires.
Puis substitution de Polybe. Balayage exhaustif (étage 1 : R), TOP 50 par mode, étage 2 (8 × 40 000).

## Questions et critères
**Q1 (indépendante de la langue)** : la généralisation renforce-t-elle la structure ? Mesure : R max réel par mode
vs R max de 5 mélanges (même famille). « Structure renforcée » si R réel > max(null) dans le mode P0 **et** si
l'écart (en nombre de σ du null) dépasse celui de la largeur 7 seule (≈ 3). Sinon, l'observation reste isolée.
**Q2 (déchiffrement)** : étage 2 en anglais, français, allemand, italien, espagnol, latin, néerlandais.
Critères de succès (par langue) identiques à E05 : qoff > max(null) + 0,5 **et** ≥ min(contrôles) − 0,5 ;
lisibilité ≥ 80 % ; ré-enchiffrement exact ; stabilité.

## Contrôles et null
- Positifs : anglais 10 (B) + 5 (A) ; chaque autre langue 5 (B), textes réservés ; mode, sE, sO aléatoires.
  Admissibilité ≥ 6/10 (anglais B), ≥ 3/5 ailleurs.
- Null : 5 mélanges (B, anglais : donne aussi R max par mode pour Q1) ; 3 mélanges par autre langue (B) ;
  3 mélanges (A, anglais).

## Mise au point
Contrôle de fonctionnement (anglais, B, 1 texte, réglages réduits) : retrouvé 182/182 ; R(vrai) = 143.
