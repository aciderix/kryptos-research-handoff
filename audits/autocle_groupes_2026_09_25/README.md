# Audit T36 (25/09/2026, nuit) : autoclé sur le clair à l'écart 7 calculée dans « autre chose » que les 26 lettres

**Question** (relais « cinq angles morts », point 3 ; base 7 §12.17). L'autoclé sur le clair à l'écart 7, forme Vigenère, est le seul procédé manuel qui rend naturel le profil du « 7 » (simulateur, base 7 §12.10). Elle est éliminée avec les alphabets de Sanborn (T27, T34). Le relais demande de la refaire sans passer par un alphabet de lettres : coordonnées d'une grille, chiffres, codes binaires. C'est l'idée de Scheidt, « changer la base de langue… pas vers une autre langue, vers autre chose » (2020).

| Fichier | Rôle |
|---|---|
| `t36_autocle_groupes.py`, `results_t36.txt`, `results_t36.json` | T36 : 23 groupes, 3 conventions, K4 + contrôle positif + 20 témoins (CP-SAT) |
| `t36_controles.py`, `results_t36_controles.txt` | contrôles positifs pour les groupes où aucun texte anglais ne reste dans les 26 lettres ; taux de fermeture de l'anglais |
| `t36_tau_libre.py`, `results_t36_tau_libre.txt` | T36b : codage du chiffré τ indépendant du codage du clair σ |

Lancement : `CORPUS=…/corpus_all.txt python3 t36_autocle_groupes.py 30 20`, puis `python3 t36_controles.py 3` et `python3 t36_tau_libre.py 30 20`. Le corpus anglais vient de `../erreurs_multiples_2026_09_24/corpus_get.sh` (Gutenberg). Il faut `pip install ortools`.

## 1. Ce que « autre chose » peut vouloir dire, mathématiquement

Une autoclé additive a besoin d'une addition, donc d'un **groupe** G : coordonnées ajoutées case par case (Polybe, trifide), codes de 5 bits combinés par XOR (Baudot), nombres modulo n. On code chaque lettre par un élément de G (injection σ), puis on calcule dans G :

- VIG : σ(c_i) = σ(p_i) + σ(p_{i−7}) + d ;
- BEAU : σ(c_i) = σ(p_{i−7}) − σ(p_i) + d ;
- VAR : σ(c_i) = σ(p_i) − σ(p_{i−7}) + d.

Deux remarques règlent une grande partie de la question avant tout calcul.

1. **Avec exactement 26 symboles, « autre chose » n'existe pas.** Tout groupe abélien d'ordre 26 est cyclique (26 = 2 × 13 est sans facteur carré), donc isomorphe à Z/26. Une grille, une roue ou des coordonnées à 26 cases, avec une addition, reviennent donc à Z/26 et à un alphabet σ **quelconque**. C'est exactement T18 (tout alphabet, sans erreur : éliminé) et T19 (avec une erreur : non concluant).
2. **Avec plus de 26 symboles, le chiffre sort des lettres.** Si G a n > 26 éléments, la somme de deux lettres codées tombe souvent sur un élément qui n'est l'image d'aucune lettre. Le chiffré de K4, lui, ne contient que les 26 lettres. Nous avons mesuré la part de textes anglais de 97 lettres qui restent dans les 26 lettres jusqu'au bout (2 000 essais par cas, `t36_controles.py`) :

| Ordre de G | Textes anglais qui restent dans les 26 lettres |
|---|---|
| 26 | 100 % |
| 27 (Z27, Z3×Z9, Z3³) | 3 à 5 % |
| 28 (Z28, Z2×Z14) | 0,05 à 0,35 % |
| 29 à 36 (et XOR 5 bits, 32 éléments) | **0 %** |

Au-delà de 28 symboles, Sanborn aurait dû refaire son clair à presque chaque ligne, ou graver des signes qui ne sont pas des lettres. Une petite marge de 27 ou 28 symboles reste pensable (un « ? » ou un signe en plus), mais elle ne tient qu'une fois sur 20 (27 symboles) à une fois sur 300 à 2 000 (28 symboles).

## 2. Méthode

- **Équations de chaîne** (comme T18 et T34). Dans chaque classe modulo 7, entre deux rangs de crib consécutifs a < b = a + 7m, la relation se propage à travers le clair inconnu. Elle donne une équation entre lettres connues seulement : 24 rangs de crib − 7 classes = **17 équations**.
- **Relaxation.** Les lettres claires intermédiaires ne sont pas obligées d'être des lettres (elles peuvent tomber hors des 26 images). Une incompatibilité est donc une vraie élimination.
- **Résolution exacte** par CP-SAT (OR-Tools 9.15) : une variable par composante et par lettre, `AllDifferent` sur l'index de l'élément, symétrie de translation fixée. Le solveur maximise le nombre d'équations vraies, et **e_min = 17 − ce maximum**. Tous les cas ont été résolus à l'optimum (preuve d'optimalité, pas d'heuristique).
- **23 groupes** : Z26 (référence, doit redonner T18/T19), Z27, Z3×Z9, Z3³, Z28, Z2×Z14, Z29, Z30, Z31, Z32, Z2×Z16, Z4×Z8, Z2×Z2×Z8, Z2×Z4×Z4, Z2³×Z4, Z2⁵ (XOR sur 5 bits), Z33, Z34, Z35, Z36, Z2×Z18, Z3×Z12, Z6×Z6. Cela couvre notamment le carré 6 × 6 (Z6×Z6), le trifide (Z3³), le Baudot (Z2⁵) et un carré 5 × 6 (Z30, isomorphe à Z5×Z6). Un carré de Polybe 5 × 5 est impossible : le chiffré de K4 contient à la fois I et J.
- **Trois conventions** (VIG, BEAU, VAR), sauf pour les groupes où x = −x (XOR) : elles y coïncident, on ne fait que VIG. Total : **67 cas**.
- **Témoins** : 20 K4 mélangés par cas (1 340 résolutions). **Contrôles positifs** : un faux K4 fabriqué avec le procédé (anglais, cribs insérés) quand l'anglais reste dans les 26 lettres. Sinon, `t36_controles.py` fabrique un clair lettre par lettre qui y reste, 3 par cas.

## 3. Résultats

**Contrôles positifs** : **197 sur 197** retrouvés à 0 équation fausse. Dans 4 cas sur 201, le générateur n'a pas trouvé de texte qui reste dans les lettres (Z36 VAR, Z2×Z18 VAR, Z3×Z12 VAR). Les contrôles de `results_t36.txt` (Z26 à Z29 VIG) sont tous retrouvés à 0.

**K4** : au moins une équation fausse dans **les 67 cas**, à l'optimum prouvé.

| Groupe | VIG : K4 / témoins à 0 / témoins ≤ K4 | BEAU | VAR |
|---|---|---|---|
| Z26 (référence T18/T19) | 1 / 35 % / 65 % | 1 / 45 % / 95 % | 1 / 35 % / 85 % |
| Z27 | 1 / 30 % / 55 % | 1 / 45 % / 85 % | 1 / 45 % / 85 % |
| Z3×Z9 | 1 / 15 % / 65 % | 1 / 60 % / 100 % | 1 / 30 % / 80 % |
| Z3³ (trifide) | **2** / 0 % / 80 % | 1 / 5 % / 65 % | 1 / 5 % / 65 % |
| Z28 | 1 / 35 % / 85 % | 1 / 40 % / 95 % | 1 / 70 % / 100 % |
| Z2×Z14 | 1 / 40 % / 95 % | 1 / 60 % / 100 % | 1 / 50 % / 90 % |
| Z29 | 1 / 35 % / 75 % | 1 / 45 % / 75 % | 1 / 60 % / 90 % |
| Z30 (carré 5 × 6) | 1 / 45 % / 70 % | 1 / 55 % / 90 % | 1 / 60 % / 100 % |
| Z31 | 1 / 30 % / 85 % | 1 / 60 % / 100 % | 1 / 60 % / 100 % |
| Z32 | 1 / 15 % / 55 % | 1 / 65 % / 100 % | 1 / 65 % / 100 % |
| Z2×Z16 | 1 / 40 % / 65 % | 1 / 55 % / 90 % | 1 / 30 % / 80 % |
| Z4×Z8 | 1 / 25 % / 75 % | 1 / 35 % / 100 % | 1 / 45 % / 100 % |
| Z2×Z2×Z8 | 1 / 10 % / 60 % | 1 / 30 % / 70 % | 1 / 55 % / 100 % |
| Z2×Z4×Z4 | 1 / 20 % / 60 % | 1 / 40 % / 90 % | 1 / 35 % / 95 % |
| Z2³×Z4 | **2** / 5 % / 85 % | 1 / 10 % / 40 % | 1 / 5 % / 70 % |
| Z2⁵ (XOR 5 bits, Baudot) | **2** / 0 % / 30 % | — | — |
| Z33 | 1 / 40 % / 85 % | 1 / 55 % / 95 % | 1 / 70 % / 95 % |
| Z34 | 1 / 35 % / 70 % | 1 / 45 % / 90 % | 1 / 60 % / 100 % |
| Z35 | 1 / 30 % / 75 % | 1 / 60 % / 85 % | 1 / 40 % / 95 % |
| Z36 | 1 / 25 % / 75 % | 1 / 65 % / 95 % | 1 / 65 % / 90 % |
| Z2×Z18 | 1 / 25 % / 75 % | 1 / 70 % / 100 % | 1 / 55 % / 100 % |
| Z3×Z12 | 1 / 40 % / 60 % | 1 / 65 % / 90 % | 1 / 35 % / 90 % |
| Z6×Z6 (carré 6 × 6) | 1 / 20 % / 65 % | 1 / 50 % / 85 % | 1 / 55 % / 95 % |

Moyennes sur les 67 cas : 40 % des témoins sont compatibles sans erreur, et 83 % font au moins aussi bien que K4.

**T36b : codage du chiffré indépendant** (τ ≠ σ, les deux libres ; 6 groupes × 3 conventions, 20 témoins). C'est le cas que désigne le simulateur : la lettre-clé est lue dans le codage du clair, le codage du chiffré est libre (base 7 §12.10). **K4 est compatible sans erreur dans les 16 cas**, et les témoins aussi : 55 à 95 % d'entre eux, selon le cas, sont à 0 (Z26 VIG : 75 %).

## 4. Verdict

- **Sans erreur, l'autoclé sur le clair à l'écart 7 est éliminée dans tous les groupes testés**, avec un même codage pour le clair, la clé et le chiffré. Cela vaut pour les 3 conventions, les nombres modulo 26 à 36, les coordonnées de grille (5 × 6, 6 × 6, 3 × 3 × 3) et le XOR sur 5 bits. Pour l'ordre 26, c'est un théorème (Z/26 seul), déjà couvert par T18.
- **Avec une erreur, aucun pouvoir de décision** : K4 fait comme 30 à 100 % des témoins. C'est la même situation que T19 dans Z/26. Le changement de « base » n'ajoute rien de décidable.
- **Avec deux codages libres (T36b), aucun pouvoir de décision, même sans erreur.** Le coin ouvert du procédé désigné est donc un peu plus large que ce qu'écrivait la base 9 (§6, point 1). Il ne comprend pas seulement « un alphabet libre et une erreur », mais aussi « deux alphabets libres, sans erreur ». Seul le texte entier (97 lettres) ou un nouveau clair connu peut trancher ; les 24 lettres des cribs n'y suffisent pas.
- **La fermeture sur 26 lettres** rend les groupes de plus de 28 éléments très peu plausibles pour un texte gravé en A–Z seulement.

**Ce qui n'est pas couvert.** Les structures non additives (tables quelconques de 26 × 26, qui ne sont plus des groupes), et les groupes non abéliens. Pour ces familles, 24 lettres ne décident rien : leur espace de clés est bien plus grand que celui d'un alphabet libre, qui est déjà indécidable avec une erreur.
