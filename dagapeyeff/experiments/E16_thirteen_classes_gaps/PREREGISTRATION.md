# E16 — « 13 classes » : trous identifiés à la relecture du livre (routes, nulles régulières, clés K1/K2, largeurs 13/14) — PRÉ-INSCRIPTION

Rédigée et commitée avant tout calcul de E16 sur le vrai chiffré (2026-09-28). Motivation : `docs/03_relecture_du_livre.md`
(T1-T3, § 2 bis et 2 ter). Hypothèse testée partout : **H13** = anglais, lettres fusionnées en 13 classes équilibrées
(E ; TZ AQ OX IK NV SB HP RY DG LF CW UM pour les contrôles, comme E14-E15), classes → 13 symboles, géométrie **B**
(182 symboles, colonne 14 = nulles ; H13 exige exactement 13 symboles, la géométrie A en a 18).

## Outil
`tools/e16_h13.c` : reprend **sans modification** la statistique invariante R d'E04 (répétitions de 3-5-grammes) et le
solveur de paires d'E15 (Viterbi 2 lettres/symbole, recuit sur l'appariement, 6 départs × 150 000 itérations,
quadgrammes joints `qg_en.bin`). Seuls les générateurs de candidats sont nouveaux.

## Cellules (fermées)
| Cellule | Candidats (déchiffrement appliqué aux 182 symboles lus par lignes) | Étage 1 | Étage 2 |
|---|---|---|---|
| **R** routes (T3) | 64 routes d'E01 (8 diédrales × lignes/colonnes × simple/boustrophédon — dont la route « chinoise » p. 126 —, 16 diagonales, 16 spirales) sur la grille 14×13, × 2 sens = 128 | R, on garde les 10 meilleurs | paires sur les 10 |
| **N** nulles régulières (T2, p. 111) | suppression d'une position sur k (k = 3, 4, 5 ; phase 0 à k−1) dans la suite des 182, **sans** transposition = 12 | — | paires sur les 12 |
| **K** clés récurrentes d'E03 (T1) | K1 = 5 3 7 13 8 6 1 4 9 2 11 12 10 et K2 = 4 9 2 11 7 13 8 6 1 12 10 3 5, convention d'E03 (clair(r, c) = S[(K(c)−1)·14 + r], W = 13, H = 14), et variante clé inverse = 4 | — | paires sur les 4 |
| **F** colonnaire complète à clé libre, W = 13 (H = 14) et W = 14 (H = 13), conventions E et I (T1) | recuit sur R dans l'espace des ordres de colonnes | voir *Admissibilité F* | paires sur les 10 meilleurs |

## Contrôles (avant tout calcul réel)
- R : 10 textes anglais réservés (182 lettres), fusion H13, symboles aléatoires, route et sens aléatoires.
- N : 10 textes, fusion H13, k et phase aléatoires ; les nulles insérées sont tirées uniformément parmi les 13 symboles
  utilisés (comme dans le chiffré : aucun symbole propre aux nulles hors colonne 14) ; longueur totale 182.
- K : sans objet (clés imposées) ; la référence est le null.
- F : 10 textes par (W, convention), clé aléatoire.
Succès d'un contrôle : ≥ 80 % des lettres lues justes (décalage ≤ 16 toléré). **Admissibilité** d'une cellule : ≥ 6/10.

### Admissibilité F (puissance d'abord ; E04 § 5-6 montre qu'un recuit sur 13!/14! crée de la structure sur du hasard)
Étage 1 seul sur les contrôles et sur 10 mélanges du chiffré réel : F n'est lancée sur le réel que si le R atteint sur
les contrôles dépasse le **maximum** des R atteints sur les mélanges dans ≥ 8/10 cas **et** que ≥ 6/10 contrôles sont
lus après l'étage 2. Sinon F est déclarée **sans puissance** (non concluant), sans calcul réel.

## Null
Chiffré réel mélangé (182 symboles), même pipeline par cellule : 5 mélanges (R, N, K), 10 (F, étage 1).

## Critères de succès (tous requis, par cellule)
1. qoff réel > max(null) + 0,5 **et** ≥ min(qoff des contrôles lus) − 0,5 ;
2. lecture anglaise continue ≥ 80 % (jugée après le critère 1) ;
3. stabilité : même candidat et même appariement à une 2ᵉ graine ;
4. vérification directe exacte (chaque lettre lue appartient au symbole observé, transposition réappliquée).
Sinon : **négatif** pour la cellule, à la puissance mesurée.

## Portée annoncée
Un négatif exclut H13 (appariement « fréquente + rare ») avec ces transpositions ; il ne dit rien des appariements
non équilibrés, des autres langues (le latin était aussi compatible en E13), ni de T4-T7 (Richelieu, code numérique,
message court, Wolseley), qui ne sont pas testés ici.
