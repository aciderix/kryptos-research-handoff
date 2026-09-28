# Audit « autoclé, preuve algébrique » (25/09/2026, nuit)

**Statut :** `EXPLORATION RESULT`. Un résultat exact et négatif, sans erreur et avec un alphabet commun au clair et au chiffré. Il ne ferme pas la version à deux alphabets libres, ni la version avec une erreur (voir la correction plus bas et `../autocle_groupes_2026_09_25/`).

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

À l'intérieur d'une classe, la récurrence relie tous les x connus. Chaque paire de cribs d'une même classe donne donc une **équation linéaire sur σ seul** (les x et y y sont des σ de lettres connues). On rassemble les 31 équations, et on teste si une égalité σ(X) = σ(Y) (X ≠ Y) appartient à l'espace qu'elles engendrent, modulo 2 **et** modulo 13 (donc modulo 26). Si oui, aucune permutation σ ne peut convenir.

## Résultat

| Convention | Contradiction forcée | Verdict |
|---|---|---|
| Vigenère | σ(B) = σ(Z) (et σ(N) = σ(L), voir ci-dessous) | **impossible** |
| Variante | σ(A) = σ(H) | **impossible** |
| Beaufort | σ(K) = σ(S) | **impossible** |

**Le cœur en clair (Vigenère).** Les positions 32 (S → S) et 73 (K → K) sont des **auto-chiffrements** (chiffré = clair). En autoclé Vigenère, c_i = σ⁻¹(σ(p_i) + σ(p_{i−7})), donc un auto-chiffrement impose σ(p_{i−7}) = 0. En 32, la lettre 7 rangs avant est N ⟹ σ(N) = 0 ; en 73, c'est L ⟹ σ(L) = 0. D'où σ(N) = σ(L) : deux lettres sur la même valeur, contradiction immédiate. (Avec un décalage d, les deux valent −d : même conclusion.)

## Robustesse aux erreurs de Sanborn

En admettant qu'une ou deux lettres de crib soient mal chiffrées (modèle base 7 §9.4) :
- **Vigenère** : **une** erreur suffit, en 66 ou en 73 (l'auto-chiffrement K → K).
- **Variante** : une erreur suffit, en 25 ou en 32 (l'auto-chiffrement S → S).
- **Beaufort** : une erreur suffit, en 24, 28, 31, 32, 67 ou 70.

> **Correction (25/09, nuit ; `../autocle_groupes_2026_09_25/verif_une_erreur.py`).** La première version affirmait qu'en Vigenère « une seule erreur ne suffit jamais, il en faut deux », sur (25, 66), (25, 73), (32, 66) ou (32, 73). C'était un artefact : le script comptait aussi « σ(X) = 0 » comme une contradiction, alors qu'une permutation envoie toujours une lettre sur 0. Après correction, le calcul algébrique et le solveur exact (CP-SAT, AllDifferent sur les 26 lettres) donnent les mêmes rangs. On a aussi rejoué la récurrence lettre par lettre sur le vrai chiffré avec un σ trouvé, sans aucun écart. Exemple, Vigenère avec le rang 73 retiré : σ = NLKWEXSJPHGTVDIRUAMCQOYBZF. Le résultat « impossible sans erreur » est inchangé ; c'est aussi celui de T18.

Même dans ces cas rescapés, le déchiffrement complet reste du charabia (recherche de σ libre + recuit, `../moteur_2026_09_25/k4sa.c` et T34) : la levée de la contradiction ne fait pas apparaître d'anglais.

## Portée et conséquences

- **Ce qui est prouvé** : l'autoclé sur le clair à l'écart 7, avec **un** alphabet mixte commun, est exclu sans erreur, pour tout σ. Le simulateur désignait ce procédé avec un alphabet du chiffré **libre** ; cette version à deux alphabets quelconques reste compatible (T36b).
- **Ce qui restait déjà exclu ailleurs** : deux alphabets à mot-clé (T34, ≥ 7 erreurs) ; alphabet standard (hypothèse NSA littérale, T18–T19).
- **Ce que cela dit du « 7 »** : si l'excès à l'écart 7 de K4 vient d'une autoclé, il faut soit une erreur sur un auto-chiffrement (73 en Vigenère), soit un alphabet lu différemment des deux côtés. Avec une erreur, les 24 lettres ne tranchent plus (T19 ; T36 : 65 % des témoins font aussi bien en Vigenère). Avec deux alphabets libres, K4 est compatible sans erreur (T36b). L'empreinte « écart 7 » n'est donc pas une autoclé sur le clair propre. Cela renforce la lecture de la base 9 : le « 7 » ne relie que des voisins et n'est pas une période ; son origine reste le hasard, une intervention manuelle, ou une étape qui n'est pas lettre à lettre.
- **Les positions 32 et 73 sont le verrou.** Ce sont les deux seuls auto-chiffrements de K4 (clair = chiffré), et ce sont eux qui portent l'essentiel du pouvoir éliminatoire, ici comme dans T19–T21. Si l'une des deux est une erreur de recopie de Sanborn, une grande partie du paysage change : pour l'autoclé Vigenère, une erreur en 73 suffit.

## Reproduire

```
python3 preuve.py
```
Sortie : les trois contradictions, puis les retraits d'erreur qui les lèvent. Aucune dépendance, calcul exact en quelques secondes.
