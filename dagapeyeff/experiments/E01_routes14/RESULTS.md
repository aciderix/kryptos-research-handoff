# E01 — RÉSULTATS (2026-09-28) : **NÉGATIF**

Pré-inscription : `PREREGISTRATION.md` (+ Amendement 1, décidé sur les contrôles avant tout run réel).
Solveur : `tools/e01_solver.c` ; quadgrammes joints `tools/build_qg_joint.c` (Gutenberg 13,6 M lettres).
Réglages identiques partout : 8 redémarrages × 60 000 itérations par cellule, 256 cellules par recherche.
Journaux bruts : `logs/`.

## 1. Contrôles positifs (RÉSULTAT)
| Géométrie | Récupérés (≥ 90 % des lettres) | qoff des contrôles |
|---|---|---|
| (A) 14×14 | **10/10** (196/196 lettres à chaque fois) | −9,17 … −9,72 |
| (B) 14×13, colonne 14 nulle | **10/10** (182/182) | −9,23 … −9,88 |

Textes tirés d'*Alice* (hors corpus d'entraînement), carré aléatoire, route et sens aléatoires. La route
« trouvée » diffère parfois de la route vraie (ex. `d3-row/dir0` ↔ `d1-row/dir1`) : ce sont des routes
**équivalentes** (même clair) ; le critère porte sur les lettres. Le solveur est donc capable, dans cette
famille, de retrouver un anglais planté de cette longueur. Min des contrôles récupérés : **−9,875**.

## 2. Null (RÉSULTAT)
20 permutations aléatoires des 196 paires réelles, même recherche (256 cellules) :
min −12,794 ; médiane −12,70 ; **max −12,435**.
(Les fichiers `null_seed1/2.out` affichent une ligne finale « max null = −9.000 » : bogue d'initialisation de
l'affichage, corrigé depuis dans le source ; le maximum est recalculé ici à partir des 20 lignes.)

## 3. Vrai chiffré (RÉSULTAT)
- Meilleure cellule : géométrie **(B)**, route `diag-anti-c3-zz`, sens 0, **qoff = −12,733**.
- Meilleure cellule en géométrie (A) : −12,855.
- Clair de la meilleure cellule (pour mémoire, **pas** un candidat) :
  `TEEASDUODIDNTLEDHELARESTHRLTPLRSIUODOUTATDAPERUPRUNORUSTSOH…`

## 4. Critères pré-inscrits
| # | Critère | Mesure | Verdict |
|---|---|---|---|
| 1 | qoff > max(null) + 0,5 = −11,935 **et** ≥ min(contrôles) − 0,5 = −10,375 | −12,733 | **ÉCHEC** (sous le max du null ; rang ≈ 6/21, sous la médiane du null) |
| 2 | ≥ 80 % d'anglais lisible | non évalué (le critère 1 conditionne l'examen) ; à l'œil : aucun | ÉCHEC |
| 3 | prédiction colonne 14 (A) | sans objet : la meilleure cellule est en (B) | — |
| 4 | ré-enchiffrement exact | 0/196 paires différentes | réussi (mais trivial : une substitution injective se ré-enchiffre toujours ; ce critère n'a de valeur que joint aux autres) |
| 5 | stabilité ≥ 6/8 | **0/8** (recoupement 36 à 103 lettres sur 182) | ÉCHEC |

## 5. Conclusion
- **RÉSULTAT** : dans la famille fermée E01 (64 routes × 2 sens × 2 géométries, carré de Polybe inconnu), le
  vrai chiffré ne se distingue **pas** d'un chiffré mélangé ; il score même un peu *moins* bien que la médiane
  des mélanges. Le solveur retrouve pourtant 20/20 messages plantés de même mécanisme.
- **Ce que cela exclut** (à puissance du test près) : Polybe + une route de cette liste, colonne 14 comprise
  comme fin du message ou comme colonne de nulles. Ceci **confirme, avec contrôles et null**, les négatifs
  antérieurs de Pelling (16 diagonales) et numberworld (lignes↔colonnes), et les étend (spirales, boustrophédons,
  les deux sens, géométrie à nulles).
- **Ce que cela n'exclut pas** : un ordre de colonnes **à clé** (transposition colonnaire), une double
  transposition, des nulles ailleurs qu'en colonne 14, une erreur d'enchiffrement, une autre langue.
- Bogue de ma vérification directe détecté et corrigé pendant le run (l'inverse du carré était pollué par les
  symboles absents) ; le solveur lui-même n'était pas concerné ; le run réel a été refait avec la même graine.

## 6. Observation qui motive E02 (OBSERVATION, pas encore un résultat)
Les 5 symboles les plus rares (≤ 3 occurrences ; le suivant en a 9) occupent la case 14 de **8 des 14 lignes**
imprimées (lignes 3, 4, 7, 8, 9, 10, 12, 14) ; les 6 autres lignes finissent par des symboles courants. C'est
exactement ce que produit une **transposition colonnaire à clé de largeur 14** avec un message de 188 lettres
et 8 bourrages en fin de dernière ligne : chaque colonne lue devient une ligne imprimée, la dernière case de
chaque ligne est la dernière ligne du clair. → pré-inscription E02.
