# E02 — RÉSULTATS (2026-09-28) : **NÉGATIF**

Pré-inscription : `PREREGISTRATION.md` (commit antérieur à tout run réel). Solveur : `tools/e02_solver.c`
(recherche emboîtée clé/carré ; TKF=8, 8 redémarrages × 6 000 propositions × 1 500 itérations internes).
Journaux : `logs/`.

## 1. Contrôles positifs (RÉSULTAT)
10 textes d'*Alice* (188 lettres + 8 bourrages), carré et clé aléatoires, partition vraie fournie :
**8/10 récupérés** (188/188 ×7, 187/188 ×1) — seuil pré-inscrit (≥ 8/10) atteint.
qoff des récupérés : −9,06 … −9,87 (min **−9,867**). Les 2 échecs finissent eux-mêmes à −10,44 et −10,81,
c.-à-d. **nettement hors du null** : un vrai message, même mal résolu, se verrait.

## 2. Null (RÉSULTAT)
20 chiffrés, 182 paires hors colonne 14 mélangées (colonne 14 fixe ⇒ même partition) :
min −12,296 ; médiane ≈ −12,10 ; **max −11,677**.

## 3. Vrai chiffré (RÉSULTAT)
| Cellule | Graine | Meilleur qoff | Clé trouvée | Stabilité |
|---|---|---|---|---|
| principale (écriture par lignes) | 41 | **−12,099** | 13 2 11 1 5 6 14 12 4 9 7 3 8 10 | 0/8 (8 à 38 lettres communes sur 188) |
| principale | 45 | −12,289 | 13 5 2 1 6 11 14 12 8 7 10 4 3 9 | — |
| principale | 46 | −12,263 | 5 11 6 2 1 13 10 4 3 14 12 7 8 9 | — |
| variante boustrophédon | 42 | −12,298 | 10 4 7 12 9 14 8 3 1 6 5 11 2 13 | 0/8 |
| clé libre 14! (critère 3) | 43 | −12,101 | 14 13 5 9 6 4 12 10 11 1 7 3 8 2 | 0/8 |

Null propre à la variante boustrophédon (10 mélanges, colonne 14 fixe) : min −12,313 ; médiane ≈ −12,13 ;
max −11,956. Le réel (−12,298) est **sous la médiane** ⇒ variante boustrophédon également **négative**.
(Null réduit à 10 au lieu de 20 : sans conséquence ici, le réel n'atteint même pas la médiane.)

## 4. Critères pré-inscrits
| # | Critère | Mesure | Verdict |
|---|---|---|---|
| 1 | qoff > max(null) + 0,5 = −11,177 **et** ≥ −9,867 − 0,5 = −10,367 | −12,099 (meilleur des 3 runs) | **ÉCHEC** — au niveau de la **médiane** du null |
| 2 | ≥ 80 % d'anglais lisible | non examiné (critère 1 non franchi) ; aucun texte à l'œil | ÉCHEC |
| 3 | la recherche à clé libre retombe sur le même clair | non | ÉCHEC — **mais sans puissance** : à clé libre, le solveur ne retrouve **aucun** contrôle (0/3). Ce critère n'apporte donc rien ici |
| 4 | ré-enchiffrement exact | 0/196 différences, 0 collision | réussi (trivial sans les autres) |
| 5 | stabilité ≥ 6/8 | 0/8 ; 3 graines → 3 clés différentes | ÉCHEC |

## 5. Conclusion
- **RÉSULTAT** : l'hypothèse « Polybe à carré inconnu + colonnaire à clé de largeur 14, écriture par lignes,
  lecture des colonnes de haut en bas, message de 188 lettres + 8 bourrages (partition tirée de la colonne 14) »
  ne produit **rien de distinguable du hasard**, alors que le même solveur retrouve 8/10 messages plantés et
  que ses échecs mêmes sortent du null. Répétée 3 fois (puissance ≈ 1 − 0,2³ ≈ 99 % si les échecs sont
  indépendants), la cellule principale reste au niveau du null.
- Équivalence notée : l'écriture « miroir » (chaque ligne de droite à gauche) est la même famille à un
  ré-étiquetage de la clé près, avec la même partition ⇒ **couverte**.
- **L'observation « 8 lignes finissent par un symbole rare » reste une OBSERVATION** : l'explication simple
  (colonnaire largeur 14 à bourrage final) est réfutée à la puissance du test. Restent : colonne 14 = nulles
  (hypothèse B) suivies d'une transposition à clé ; double transposition ; erreur d'enchiffrement.
- **Limite d'outil** (RÉSULTAT sur contrôles) : le recuit emboîté ne résout pas une clé libre de 14 colonnes
  jointe à un carré inconnu. Toute cellule suivante à clé libre exige d'abord un solveur plus puissant, validé
  sur contrôles (piste : à carré fixé, l'ordre optimal des colonnes se calcule **exactement** par programmation
  dynamique sur les sous-ensembles, 2¹⁴ × 14 états).
