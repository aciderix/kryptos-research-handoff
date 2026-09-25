# Audit « autoclé, preuve algébrique » (25/09/2026, nuit)

**Statut :** `EXPLORATION RESULT`. Un résultat exact, négatif, qui ferme proprement la seule piste que le simulateur retenait.

## Pourquoi ce test

Le simulateur (`../simulateur_2026_09_25/`) a montré que, parmi 52 procédés manuels, **un seul** reproduit l'empreinte « écart 7 » de K4 (excès à l'écart 7, rien à 14/21) : l'**autoclé sur le clair à l'écart 7, forme Vigenère**. C'est l'hypothèse de la NSA de 1992. T34 l'avait « éliminée » avec deux alphabets à mot-clé (≥ 7 équations fausses), mais par une recherche, pas par une preuve.

Ici on prouve, **sans mot-clé et sans budget d'erreur**, que ce procédé est incompatible avec les cribs, dans les trois conventions, pour un alphabet mixte σ **quelconque** commun aux deux côtés (Quagmire III).

## L'argument (`preuve.py`)

Dans le domaine de σ, on pose x_i = σ(clair_i), y_i = σ(chiffré_i). L'autoclé à l'écart 7 s'écrit :
- **VIG** : y_i = x_i + x_{i−7} ⟹ x_i = y_i − x_{i−7}
- **VAR** : y_i = x_i − x_{i−7} ⟹ x_i = x_{i−7} + y_i
- **BEAU** : y_i = x_{i−7} − x_i ⟹ x_i = x_{i−7} − y_i

**Le fait géométrique clé.** Les deux cribs (21–33 et 63–73) sont distants de **42 = 6 × 7**. Chaque classe modulo 7 contient donc des positions des **deux** blocs :

| classe | positions de crib |
|---|---|
| 0 | 21, 28, 63, 70 |
| 1 | 22, 29, 64, 71 |
| 2 | 23, 30, 65, 72 |
| 3 | 24, 31, 66, 73 |
| 4 | 25, 32, 67 |
| 5 | 26, 33, 68 |
| 6 | 27, 69 |

À l'intérieur d'une classe, la récurrence relie tous les x connus. Chaque paire de cribs d'une même classe donne donc une **équation linéaire sur σ seul** (les x et y y sont des σ de lettres connues). On rassemble les 31 équations, et on teste si une égalité σ(X) = σ(Y) (X ≠ Y) ou σ(X) = 0 appartient à l'espace qu'elles engendrent, modulo 2 **et** modulo 13 (donc modulo 26). Si oui, aucune permutation σ ne peut convenir.

## Résultat

| Convention | Contradiction forcée | Verdict |
|---|---|---|
| Vigenère | σ(L) = 0 | **impossible** |
| Variante | σ(L) = 0 | **impossible** |
| Beaufort | σ(K) = σ(S) | **impossible** |

**Le cœur en clair (Vigenère).** Les positions 32 (S → S) et 73 (K → K) sont des **auto-chiffrements** (chiffré = clair). En autoclé Vigenère, c_i = σ⁻¹(σ(p_i) + σ(p_{i−7})), donc un auto-chiffrement impose σ(p_{i−7}) = 0. En 32, la lettre 7 rangs avant est N ⟹ σ(N) = 0 ; en 73, c'est L ⟹ σ(L) = 0. Les chaînes de classe propagent ces deux contraintes jusqu'à une incompatibilité complète.

## Robustesse aux erreurs de Sanborn

En admettant qu'une ou deux lettres de crib soient mal chiffrées (modèle base 7 §9.4) :
- **Vigenère** : une seule erreur ne suffit **jamais**. Il en faut **deux**, et seulement sur quatre paires : **(25, 66), (25, 73), (32, 66), (32, 73)** — chaque fois une position du premier bloc et une du second, toujours autour des auto-chiffrements 32 et 73.
- **Beaufort** : une erreur suffit, mais seulement en 24, 28, 31, 32, 67 ou 70.

Même dans ces cas rescapés, le déchiffrement complet reste du charabia (recherche de σ libre + recuit, `../moteur_2026_09_25/k4sa.c` et T34) : la levée de la contradiction ne fait pas apparaître d'anglais.

## Portée et conséquences

- **Ce qui est prouvé** : l'autoclé sur le clair à l'écart 7, avec **un** alphabet mixte commun, est exclu sans erreur, pour tout σ. C'est le procédé unique que le simulateur désignait.
- **Ce qui restait déjà exclu ailleurs** : deux alphabets à mot-clé (T34, ≥ 7 erreurs) ; alphabet standard (hypothèse NSA littérale, T18–T19).
- **Ce que cela dit du « 7 »** : si l'excès à l'écart 7 de K4 vient d'une autoclé, il faut soit deux erreurs exactement placées autour de 32 et 73, soit un alphabet lu différemment des deux côtés — et aucune de ces portes ne donne d'anglais. L'empreinte « écart 7 » n'est donc pas une autoclé sur le clair propre. Cela renforce la lecture de la base 9 : le « 7 » ne relie que des voisins et n'est pas une période ; son origine reste le hasard, une intervention manuelle, ou une étape qui n'est pas lettre à lettre.
- **Les positions 32 et 73 sont le verrou.** Ce sont les deux seuls auto-chiffrements de K4 (clair = chiffré), et ce sont eux qui portent l'essentiel du pouvoir éliminatoire, ici comme dans T19–T21. Si l'une des deux est une erreur de recopie de Sanborn, une grande partie du paysage change — mais il en faudrait deux, très précises, pour l'autoclé Vigenère.

## Reproduire

```
python3 preuve.py
```
Sortie : les trois contradictions, puis les retraits d'erreur qui les lèvent. Aucune dépendance, calcul exact en quelques secondes.
