# K4 — chercher le mécanisme, pas la clé

**Date :** 2026-09-22
**Statut :** `EXPLORATION RESULT — ÉLIMINATIONS ALGÉBRIQUES SOUS SCOPE`. Aucune solution, aucun clair, aucun signal anormal revendiqué.
**Code et résultats bruts :** `audits/algebraic_elimination_2026_09_22/`

---

## 1. Le changement de méthode : l'algèbre plutôt que l'énumération

La force brute pose la question « *cette clé-ci* donne-t-elle de l'anglais ? » et la répète des milliards de fois. C'est une mauvaise question pour K4 : les clés candidates viennent de listes de mots, les alphabets mélangés aussi, et un score d'anglais sur 97 lettres est un juge faible.

La question d'un cryptanalyste de 1989 est différente : « *ce type de machine* peut-il, avec **n'importe quel** réglage, avoir produit les 24 lettres connues ? ». Friedman raisonnait ainsi (isomorphes, symétrie de position) : on ne cherche pas la clé, on cherche les **relations que le mécanisme impose** entre chiffré et clair, et on vérifie si les données les respectent.

Concrètement, chaque hypothèse de mécanisme est traduite en contraintes :

- l'alphabet mélangé est une **inconnue** : les 26! permutations sont couvertes, pas seulement les alphabets tirés d'un mot-clé ;
- les valeurs de clé sont des inconnues ;
- un solveur SAT exact répond **oui** (au moins un réglage colle aux 24 lettres) ou **non** (aucun réglage, quel qu'il soit).

Un « non » élimine le mécanisme entier en un seul calcul, pour toutes les clés et tous les alphabets à la fois. Un « oui » ne prouve rien : il dit seulement que les 24 lettres ne suffisent pas à trancher.

### Garde-fous (appliqués à chaque famille)

| Contrôle | Rôle | Résultat |
|---|---|---|
| **K1 réel** (Quagmire III, alphabet KRYPTOS, clé PALIMPSEST) avec 24 lettres d'ancrage de même forme | le solveur doit retrouver la vraie période | période 10 (et 20) trouvées ; 7, 8, 9, 11 rejetées |
| **Contrôle positif synthétique** par famille : un faux K4 fabriqué avec le mécanisme testé (alphabet et clé aléatoires) | le test doit dire « oui » quand le mécanisme est vrai | « oui » pour toutes les familles dont le calcul est terminé (après correction d’un bug du générateur Q-I/Q-II, voir §6) |
| **Contrôle nul** : 20 chiffrés aléatoires (5 pour la clé courante) avec les mêmes positions d'ancrage | mesure la fréquence des « oui » par pur hasard, donc la puissance du test | reporté dans chaque tableau |
| **Recoupement** : les périodes Quagmire III ont d'abord été résolues avec Z3, puis avec l'encodage SAT exact | un second outil doit donner la même réponse | résultats identiques |

L'hypothèse d'alignement est toujours explicite. **H1** : la lettre gravée n° *i* se déchiffre en la lettre claire n° *i*. C'est l'hypothèse qu'implique la manière dont Sanborn a formulé ses indices (« NYPVTT devient BERLIN »).

---

## 2. Ce que disent les 24 lettres (sous H1)

### 2.1 Alphabets fixes (AZ et KRYPTOS), trois conventions additives

`structural_fixed_alphabets.py` teste des *générateurs* de clé, pas des clés, pour Vigenère, Beaufort et Beaufort variant, en alphabet AZ et KA :

| Générateur de clé | Paramètres couverts | Survivants |
|---|---|---|
| ligne + colonne : `k = a[ligne] + b[colonne]` (« ID BY ROWS ») | largeurs 2–48 | **aucun** |
| autoclave (clé = chiffré ou clair *L* positions avant) | tous les décalages 1–96 | **aucun** |
| clé progressive : mot-clé de période *p* décalé de *s* à chaque tour | *p* ≤ 48, tous les *s* | **aucun** |
| clé périodique vérifiée **à l'intérieur de chaque crib seulement** | *p* ≤ 12 | **aucun** |

Le dernier test est important : il reste valable **même si des lettres ont été omises ou insérées** entre les cribs ou avant. C'est exactement le type d'erreur documenté chez Sanborn en K2 : une lettre omise qui décale la clé et transforme `XLAYERTWO` en `IDBYROWS`.

**Sélecteur parmi les lettres des mots-clés connus.** Sur les 24 positions, la clé utilise au moins **12 lignes distinctes** du tableau (Beaufort AZ) et jusqu'à 16 (Vigenère KA). Un sélecteur qui pioche uniquement dans les lettres de KRYPTOS (7), PALIMPSEST (8) ou ABSCISSA (5) est donc impossible. Même leur réunion (14 lettres : A B C E I K L M O P R S T Y) laisse entre 5 et 13 positions sans ligne disponible selon la convention.

### 2.2 Alphabets inconnus : toutes les permutations possibles

Résultats du solveur SAT exact (`k4_algebraic.py`). « K4 compatible » liste les paramètres pour lesquels **au moins un** alphabet et une clé collent aux 24 lettres. La colonne « hasard » donne la moyenne pour un chiffré aléatoire.

| Mécanisme (alphabet inconnu) | Paramètres | K4 compatible (Vig) | K4 compatible (Beau) | Moyenne pour un texte aléatoire (Vig/Beau) |
|---|---|---|---|---|
| **Quagmire III** (système de K1/K2), clé périodique | p = 1–52 | **27, 28, 29, 30, 41** | 23, 26, 27–33, 46, 47, 49, 51, 52 | 20,5 / 17,4 |
| Quagmire I (clair mélangé) | p = 1–52 | 13, 16, 19, 23, 24, 26–33, 35, 38, 39, 41, 46–49, 51, 52 | idem (équivalent) | 19,4 |
| Quagmire II (chiffré mélangé) | p = 1–52 | 16, 19, 20, 23, 24, 26–33, 37–40, 46–49, 51, 52 | idem (équivalent) | 22,3 |
| Quagmire IV (deux alphabets indépendants) | p = 1–52 | 8, 13, 16, 19, 20, 23, 24, 26–33, 35, 37–41, 46–49, 51, 52 | idem | 28,4 |
| Quagmire III, période avec **saut de phase libre entre les cribs** | p = 1–12 | **12 seulement** | 8, 11, 12 | 3,9 / 3,5 |
| Quagmire III, **clé courante = clair de K1** | 11 décalages | **aucun** | **aucun** | 0 / 0 |
| Quagmire III, **clé courante = clair de K2** | 318 décalages | **aucun** | **aucun** | 0 / 0 |
| Quagmire III, **clé courante = clair de K3** | 284 décalages | **aucun** | **aucun** | 0 / 0 |
| Quagmire III, autoclave sur le chiffré | L = 1–96 | 37, 41, 46, 60, 64, 67, 69–71 | 22 décalages | 18,7 / 15,3 |
| Quagmire III, autoclave sur le clair | L = 1–96 | 32–39, 46, 48–50 | 6, 7, 8, 10, 32, 33, … | 15,9 / 14,0 |
| Quagmire III, clé progressive (pas libre) | p = 2–48 | 8, 10–13, 16–18, 20, 22–37, 39–41, 43, 44, 46–48 | 11–13, 15–18, 22–37, 39, 41, 43–48 | contrôle nul non terminé (voir §6) |
| Quagmire III, clé ligne + colonne | largeurs 2–48 | 39 largeurs sur 47, dont 2–5 | calcul non terminé | non calculé : famille trop lâche (voir ci-dessous) |

Lecture :

- **Le système exact de K1/K2 (Quagmire III, sens Vigenère), avec n'importe quel alphabet mélangé et n'importe quelle clé, est impossible pour toutes les périodes 1 à 26**, et aussi 31–40 et 42–52. Il ne reste que 27–30 et 41. Or 27–29 ne sont contraintes par *aucune* paire d'ancrage (aucun écart entre lettres connues n'est multiple de 27, 28 ou 29), et 30 par une seule paire : « compatible » n'y signifie rien. La période 41 est compatible, mais **30 % des chiffrés aléatoires** le sont aussi (12/40) : ce n'est pas un indice.
- Les registres existants testaient Quagmire III avec des alphabets **tirés de listes de mots-clés**. Ici, tous les alphabets sont couverts d'un coup.
- **Même avec des nulles ou des omissions libres entre les cribs**, une clé de période ≤ 11 est impossible en Quagmire III-Vigenère.
- La **clé courante tirée des clairs K1, K2 ou K3** est éliminée pour tout alphabet et tout décalage. Le test est puissant : aucun chiffré aléatoire n'est compatible, alors que le contrôle positif l'est.
- Les autoclaves « compatibles » ne sont pas des indices : décalage par décalage, entre 33 % et 98 % des chiffrés aléatoires le sont aussi (`results_autokey_pt_lags_null.json`).
- **Mise à jour du 24/09 (audit « vision », T18).** L'autoclave sur le clair se propage de L en L à travers le clair inconnu par une relation **linéaire** dans σ. Deux positions de crib distantes d'un multiple de L sont donc reliées par une équation qui ne fait intervenir que des lettres chiffrées connues, ce que ce tableau n'utilisait pas. Avec ces chaînes, **l'écart 7 (hypothèse de la NSA en 1992) devient impossible en Vigenère, Beaufort et variante**, alors que 29 à 57 % des chiffrés aléatoires restent compatibles. Sur l'ensemble des écarts, K4 reste au niveau du hasard (`audits/vision_2026_09_24/t18_autocle_chaines.c`). **Réserve (T19)** : l'élimination suppose zéro erreur de chiffrement. En retirant une seule équation de chaîne, ce qui revient à une lettre mal chiffrée, K4 redevient compatible dans les trois conventions (`t19_une_erreur.c`).
- Avec un alphabet inconnu, la **clé progressive** (mot-clé qui glisse d'un pas libre à chaque tour) reste compatible pour la plupart des périodes, dont 8, 10, 11 et 12. Le pas libre ajoute juste assez de liberté pour que 24 lettres ne suffisent plus. Sans contrôle nul terminé, **aucune conclusion** n'est tirée. Avec les alphabets fixes AZ et KA (§2.1), elle est en revanche éliminée partout.
- La **clé ligne + colonne** avec alphabet inconnu est compatible même en largeur 2. C'est structurel : chaque ligne a sa propre valeur libre, donc avec peu de colonnes presque chaque paire de positions reçoit un décalage libre. La famille n'est testable qu'avec un alphabet fixé, et elle est alors éliminée (§2.1).
- Quagmire I, II et IV ont trop de degrés de liberté pour 24 lettres : la plupart des périodes moyennes restent ouvertes, et un texte aléatoire est compatible avec 19 à 28 périodes. **Les 24 lettres ne suffisent pas à trancher ces familles.** Il faut un paramètre fixé par une source.

### 2.3 La couche K3 : colonnes KRYPTOS + Quagmire III, alphabet inconnu

On teste la combinaison littérale « K3 + K1/K2 », c'est-à-dire la « LAYER TWO » : transposition à colonnes clé KRYPTOS (7 colonnes), dans les deux sens et les deux ordres de couches, puis Quagmire III périodique avec alphabet inconnu, périodes 1–26.

| Variante | K4 compatible (Vig) | K4 compatible (Beau) | Moyenne pour un texte aléatoire |
|---|---|---|---|
| transposition puis substitution, sens direct | 15, 19, 22, 23, 25, 26 | 15, 22, 25, 26 | 3,3 / 2,9 |
| substitution puis transposition, sens direct | 18, 23, 26 | 18, 23, 26 | 5,7 / 5,0 |
| transposition inverse puis substitution | **aucune** | 16, 20, 23 | 2,2 / 3,3 |
| substitution puis transposition inverse | 25, 26 | 14, 18, 21, 26 | 3,9 / 5,2 |

Aucune période courte (≤ 13) ne survit. Les survivantes sont des périodes longues et peu contraintes, en nombre comparable au hasard. **Pas de signal.**

### 2.4 L'hypothèse du « ? » : K4 ferait 98 symboles, soit 14 × 7

Le raisonnement est détaillé au §4. Le test porte sur une liste fermée de 16 grilles : 7 colonnes, remplissage par lignes, colonnes lues dans l'ordre KRYPTOS ou dans l'ordre naturel, de haut en bas ou de bas en haut, transposition directe ou inverse, nulle « ? » au début ou à la fin. On garde seulement les grilles qui **placent le « ? » en position gravée 0**, là où il se trouve réellement. Il en reste 4 : clé KRYPTOS ou non, directe ou inverse, toujours lecture de haut en bas et nulle au début.

| Grille survivante | Ordre | K4 compatible (Vig) | K4 compatible (Beau) | Moyenne pour un texte aléatoire (Vig/Beau) |
|---|---|---|---|---|
| KRYPTOS, directe | transposition puis substitution | 26 | 15, 19, 23, 25 | 4,4 / 3,5 |
| KRYPTOS, directe | substitution puis transposition | **aucune** | 18, 23, 25, 26 | 7,0 / 4,8 |
| KRYPTOS, inverse | transposition puis substitution | 23 | 19, 25 | 2,9 / 1,7 |
| KRYPTOS, inverse | substitution puis transposition | **aucune** | 22, 24, 25, 26 | 6,3 / 4,6 |
| sans clé, directe | transposition puis substitution | 18, 26 | 15, 18, 23, 26 | 7,6 / 4,5 |
| sans clé, directe | substitution puis transposition | 25, 26 | 13, 14, 21, 25, 26 | 8,4 / 5,1 |
| sans clé, inverse | transposition puis substitution | **aucune** | 16, 20, 26 | 2,6 / 3,7 |
| sans clé, inverse | substitution puis transposition | 18, 25, 26 | 13, 16, 24, 26 | 5,2 / 6,0 |

Le contrôle positif passe : un faux K4 de 98 symboles construit avec chacune des 4 grilles et une clé de période 10 est retrouvé à la période 10 et rejeté à 7 (16/16, `results_qmark98_positive_control.json`).

**Conclusion : la version la plus simple de l'hypothèse « 98 = 14 × 7 + Quagmire III périodique » est éliminée pour les périodes 1–12, avec tout alphabet.** Les périodes longues qui restent ne sont pas plus nombreuses que pour le hasard. L'idée du « ? » n'est pas morte pour autant : la vraie procédure de K3 (double rotation de grille) n'est pas une simple transposition à colonnes, et c'est elle qu'il faut adapter (§5).

À noter : la « validation gratuite » par la position du « ? » est plus faible qu'annoncé. Toute lecture de haut en bas qui commence par la colonne 0 place une nulle initiale en tête. Le seul fait non trivial est que K, première lettre de KRYPTOS, est aussi la première de ses lettres dans l'ordre alphabétique : la grille clée se comporte donc ici comme la grille naturelle.

### 2.5 La vraie méthode de K3, généralisée exactement

**Fait établi (vérifié sur les vrais textes de K3).** La route de K3 se décrit ainsi : écrire le clair en lignes de 42, lire les colonnes de gauche à droite, chacune de bas en haut, puis réécrire en lignes de 14 et relire de la même façon. C'est **exactement** une multiplication des positions :

> (position chiffrée + 1) = 192 × (position claire + 1) mod 337, avec 192 = 8 × 24 (le nombre de lignes des deux grilles) et 337 = 336 + 1, qui est premier.

Plus généralement, un passage dans une grille pleine de *r* lignes multiplie (position + 1) par *r* modulo *n* + 1. C'est la raison profonde de l'observation connue « K3 = prendre une lettre sur 192 ». Conséquence : « appliquer la méthode de K3 à K4 » n'est pas un espace flou de grilles et de sens de lecture, c'est une **liste fermée de multiplicateurs** :

- **Modèle A (le « ? » fait partie de K4, 98 symboles, modulo 99).** Les grilles pleines ont 2, 7, 14 ou 49 lignes, et ce nombre de lignes est aussi le multiplicateur d'un passage. Avec un nombre quelconque de passages, les multiplicateurs engendrés par 2 et 7 sont **les 60 multiplicateurs inversibles modulo 99**, c'est-à-dire toutes les décimations. La position gravée du « ? » (en tête) détermine alors où se trouvait la nulle dans le clair : ce n'est pas un paramètre libre.
- **Modèle B (97 lettres, modulo 98).** Aucune grille pleine n'existe (97 est premier). La généralisation naturelle est la décimation x → m·x mod 98, avec m premier avec 98 (42 multiplicateurs).

Chaque permutation est combinée à Quagmire III (alphabet inconnu, clé périodique 1–26, Vigenère et Beaufort, substitution avant ou après la transposition), soit (60 + 42) × 2 × 2 = 408 configurations. Les contrôles positifs passent (12/12 : un faux K4 construit avec un multiplicateur est retrouvé à la vraie période et rejeté à une fausse).

**Résultat : pas de signal.** Le jugement porte sur les périodes courtes (1–13), les seules que les 24 lettres contraignent vraiment.

- **Vue d'ensemble** (`results_k3_multiplicative_partial_summary.json`). Sur les 260 configurations entièrement calculées (3 380 cellules configuration × période), K4 est compatible dans **96 cellules (2,8 %)**. Des chiffrés aléatoires passés dans les mêmes cellules tirées au sort le sont dans **3,4 %** des cas (43/1 280, `results_k3_family_null_seed*.json`), soit environ 114 attendues. **Aucun excès.**
- **Le cas de la période 7.** Quelques cellules à période 7 (la longueur de KRYPTOS) étaient compatibles pour K4 et pour 0 texte aléatoire sur 30. Examen complet (`k3_p7_check.py`) :
  - Sur les 408 configurations à p = 7, K4 est compatible dans **8**, pour environ **2,2 attendues**. Cette espérance est estimée avec 20 tirages par cellule, donc sous-estimée : de nombreuses cellules à taux réel d'environ 1 % apparaissent à 0/20. Avec 300 tirages, les 5 cellules suivies ont un taux de hasard de 0,7 à 1,7 %.
  - **La période 7 a été choisie après avoir vu les données**, parmi 13 périodes courtes testées. Sur l'ensemble de ces 13 périodes, il n'y a aucun excès. Ce n'est pas un signal pré-enregistré.
  - **Le test décisif : les lettres prédites** (`k3_p7_forced.py`). Pour chaque cellule, le solveur établit quelles lettres du clair **hors cribs** sont forcées, c'est-à-dire identiques pour tous les alphabets et toutes les clés compatibles. Une vraie solution doit donner de l'anglais. Les deux cellules les plus contraintes donnent du charabia : multiplicateur 58 (58 lettres forcées : `LNZZLJ…ZXI…WXHZD…BLNDKUFSZN…`) et multiplicateur 85 (52 lettres forcées : `…JZRTBPKSYW BERLINCLOCK FLXG…`). Les deux autres (multiplicateurs 81 et 91) ne forcent que 15 à 21 lettres. Pour eux, on compare le caractère « anglais » des lettres forcées (log-fréquence moyenne) à celui de chiffrés aléatoires compatibles avec la même cellule (15 chacun, obtenus par rejet) : **47 % et 67 % des textes aléatoires font au moins aussi bien que K4** (`results_k3_p7_forced_null_0.json`, `_1.json`). Les hits à la période 7 sont donc du hasard.

**Route de K3 + clé courante tirée des textes de la sculpture** (`route_runkey_fixed.py`, arithmétique pure, alphabets réellement employés par Sanborn : normal et KRYPTOS). Les routes testées sont l'identité, les 102 routes multiplicatives et les 20 routes irrégulières de largeur 7, 14 et 21 (§2.6). Chacune est combinée aux deux ordres de couches, à Vigenère, Beaufort et Beaufort variant, et à six textes sources : clairs de K1, K2 et K3, chiffrés de K1 et K3 reconstruits, et la concaténation des trois clairs. Tous les décalages sont essayés, soit **7 272 combinaisons**. Un faux K4 construit ainsi est retrouvé à 24/24. Pour K4, la meilleure combinaison ne reproduit que **8 lettres sur 24**. Des chiffrés aléatoires passés dans la même famille atteignent 7 à 9 (20 essais). **Éliminé.**

**Conclusion : « méthode de K3 (toutes ses variantes pleines, avec ou sans le « ? ») + système de K1/K2 avec clé périodique courte » ne rend pas compte de K4, pour aucun alphabet.** Si K4 réutilise la route de K3, la seconde couche n'est pas une Quagmire III à clé courte. Il faut alors une clé longue ou non périodique, et on retombe sur le besoin d'un objet extérieur (§3).

---

## 3. La déduction centrale

Mis bout à bout, ces résultats disent quelque chose de net :

> **Sous H1, aucun générateur de clé « autonome » ne survit.** Ni période courte, ni progression, ni ligne/colonne, ni autoclave, ni clé tirée des textes de K1–K3, ni sélecteur limité aux lettres des mots-clés connus. Et cela vaut pour *tout* alphabet mélangé dans le cadre K1/K2.

Aux 24 positions, la clé se comporte comme des lettres tirées au hasard : au moins 12 lignes distinctes, sans relation arithmétique. Pour un concepteur travaillant à la main en 1989, il n'existe que deux façons d'obtenir cela de manière reproductible :

1. **La clé vient d'un objet extérieur** : un texte (livre, document, inscription) ou un dispositif physique (horloge, grille, position sur la sculpture). Les 24 lettres n'y donnent accès que si l'on connaît l'objet. C'est la version forte de l'hypothèse du « sélecteur » du carnet précédent. Mais nos résultats montrent qu'un sélecteur **libre** n'est pas réfutable avec 24 lettres. Seul un sélecteur **entièrement fixé par une source** peut être testé.
2. **H1 est fausse au niveau des lettres** : les positions ont été déplacées (transposition, nulles, grille) et les indices de Sanborn désignent des positions du **clair**, pas une correspondance lettre à lettre. Dans ce cas, les « suites de clé » calculées depuis 1999 n'existent tout simplement pas, et tous les tests de périodicité faits sous H1, les nôtres compris, portent sur un objet fictif.

> **Correction (23/09).** L'argument ci-dessous en faveur de (2) est **affaibli par une source primaire** que je n'avais pas : Sanborn lui-même a déclaré (documentaire CNN, 2019, cité par Bean 2021) que « BERLINCLOCK en clair correspond directement à NYPVTTMZFPK, c'est une correspondance un-à-un ». Bean le confirme statistiquement. L'hypothèse (1) redevient la plus probable, sous la forme d'une **clé fabriquée par une étape préalable**, simple et mémorisable à partir de mots-clés (Scheidt 2011), comme le Gromark. Voir `docs/knowledge_base/`.

Argument initial, conservé pour mémoire : le rasoir d'Occam penche vers (2). Sanborn a montré en K3 qu'il transpose. K2 se termine (après correction) par **« LAYER TWO »**. Scheidt parle d'un anglais d'abord « masqué ». Enfin, un concepteur qui voulait une clé impossible à reconstituer n'avait pas besoin des 24 lettres pour la protéger. En revanche, s'il y a une transposition, les cribs ne révèlent rien tant qu'on n'a pas trouvé *la bonne permutation*. C'est cohérent avec des indices donnés au compte-gouttes (2010, 2014, 2020) sans que personne n'avance.

L'argument de Bean pour H1 (répétitions compatibles avec un chiffrement direct) reste valable, mais il est statistique, pas démonstratif. Il ne suffit pas à exclure (2).

---

## 4. Ce qui manque dans le raisonnement : l'élément à chercher

Le problème change de forme. Avec la méthode algébrique, **tester une transposition candidate coûte une seule question** (« existe-t-il un alphabet et une clé périodique qui, après cette permutation, collent aux 24 lettres ? »), couvrant tous les alphabets et toutes les clés. L'espace à explorer n'est plus « transpositions × alphabets × clés », mais seulement « transpositions ». Et une transposition *faite à la main* se décrit en quelques mots : dimensions, sens de remplissage, ordre de lecture, mot-clé.

L'élément manquant est donc probablement **une règle de parcours** : la façon dont Sanborn a adapté la grille de K3 à K4. Trois indices « matériels » la contraignent, sans rien ajuster sur les résultats :

1. **97 est premier.** Aucune grille rectangulaire ne le divise. Un concepteur qui transpose doit donc compléter (nulles), ou accepter une grille irrégulière.
2. **Le « ? » gravé entre K3 et K4 n'est pas nécessaire à K3.** Le clair de K3 code déjà son propre point d'interrogation par la lettre finale `Q` (`CANYOUSEEANYTHINGQ`). S'il appartient à K4, K4 compte **98 = 14 × 7** symboles : une grille parfaite pour le mot-clé **KRYPTOS (7 lettres)**, et exactement les nombres 7 et 14 relevés par l'analyse NSA de 1992. La position du « ? » (en tête) devient alors une **prédiction gratuite** : une transposition candidate doit l'y placer. C'est une validation qu'on ne peut pas ajuster.
3. **Les deux cribs commencent sur des multiples de 21** (positions 21 et 63, en comptant à partir de 0). En largeur 21 (ou 7), les deux fragments débutent en colonne 0 des lignes 1 et 3. C'est la même famille de nombres (7, 14, 21) que celle de la NSA et de l'anomalie de Bean en largeur 21. Une coïncidence reste possible (trois largeurs vérifient cette propriété : 3, 7, 21). Mais pour un concepteur qui compose son clair ligne par ligne dans une grille de 7 ou 21, c'est exactement ce qu'on s'attend à voir.

Ces trois indices désignent la même famille : **grilles de 7 colonnes, avec ou sans le « ? »**, qui est précisément la famille de K3.

---

## 5. Programme proposé (déterminé, sans force brute)

Chaque étape est une liste **fermée**, écrite avant tout résultat. Chaque candidat coûte une requête SAT et passe par les mêmes contrôles.

1. ~~Reconstituer exactement la procédure de K3 et tester toutes ses adaptations à 97 et 98 symboles.~~ **Fait (§2.5)** : la route de K3 est une multiplication modulaire, la famille est close (102 permutations), et elle est négative avec Quagmire III à clé courte. **Suite naturelle** : la même famille close combinée à une clé courante tirée des textes de la sculpture (test sans faux positif). Environ 250 000 requêtes, faisable en quelques heures sur une machine plus large.
2. **Même chose pour les grilles de largeur 21** (anomalie de Bean) et de largeur 7 non clés : remplissage par lignes, lecture par colonnes, dans les deux sens.
3. **Clé courante issue d'objets publics de la sculpture**, toujours avec alphabet inconnu : textes chiffrés de K1–K3, Morse K0, lignes du tableau lues dans les sens naturels. Le test est puissant : zéro faux positif sur texte aléatoire. Il faut d'abord établir les transcriptions exactes à partir de sources primaires.
4. Seulement si une étape donne un « oui » **plus rare que le hasard** : reconstruire la solution complète, puis vérifier qu'elle prédit des lettres hors cribs lisibles. Sinon, conserver le négatif avec sa portée exacte.

Ce qui ne peut **pas** se décider avec les données publiques, et qu'il ne faut pas faire semblant de tester : un sélecteur libre, une clé tirée d'un livre inconnu, Quagmire IV à période moyenne. Pour ces familles, 24 lettres sont mathématiquement insuffisantes (le hasard est compatible dans la majorité des cas).

---

## 6. Limites et incidents

- **Z3 et délais.** Un premier encodage Z3 (entiers modulo 26) a renvoyé `unknown` (délai dépassé) sur un contrôle positif. Il pouvait donc manquer des solutions sans le signaler. Tous les résultats ci-dessus proviennent de l'encodage **booléen exact** (python-sat / CaDiCaL), qui n'a pas de délai. Z3 n'a servi qu'au recoupement des périodes Quagmire I–IV, où il n'a renvoyé aucun `unknown`.
- **Bug corrigé dans un contrôle.** Le générateur synthétique utilisait par erreur un alphabet mélangé du côté censé être l'identité pour Quagmire I et II. Leurs contrôles positifs échouaient donc à tort. Après correction, les 8 variantes retrouvent la vraie période et rejettent une fausse (`positive_controls_periodic_fixed.json`). Q-I et Q-II ont été relancés, et les fichiers de résultats portent les contrôles corrigés.
- **Calculs non terminés.** Les passes complètes « clé progressive » et « ligne + colonne » avec alphabet inconnu, 20 contrôles nuls chacune, ont été arrêtées faute de temps : les preuves d'impossibilité sur chiffrés aléatoires sont lentes. Les passes ciblées (`prog_rowcol_targeted.py`) ont aussi été arrêtées avant la fin. Seules les réponses pour K4 sont archivées (`results_k4_only_progressive.json`, `results_k4_only_rowcol.json` en Vigenère seulement). En conséquence, ces deux familles restent `OPEN` avec un alphabet inconnu (elles sont éliminées avec AZ et KA).
- **Route de K3, calcul partiel.** Deux des quatre lots de `k3_multiplicative.py` (modèle B du lot 1 et lot 0 entier) ont été arrêtés après plusieurs heures, bloqués sur des instances SAT difficiles. Le bilan porte sur 260 des 408 configurations. En revanche, l'examen de la période 7 couvre les 408.
- **Grilles irrégulières 7/14/21** (`ragged_widths.py`) : 48 des 80 configurations calculées. Le 4ᵉ lot a été perdu lors d'un redémarrage du conteneur et n'a pas été relancé, les campagnes de calcul étant arrêtées à la demande de l'utilisateur. Résultat : 11 compatibilités à période courte pour environ 19 attendues par hasard, contrôles positifs corrects. **Pas de signal.**
- **Transcriptions.** Les clairs de K1–K3 utilisés comme clés courantes viennent de la solution publique usuelle, avec les fautes gravées conservées (`IQLUSION`, `UNDERGRUUND`, `DESPARATLY`). Une erreur de transcription entre les deux cribs pourrait faire échouer un bon décalage. Les résultats « aucun » valent pour ces transcriptions.
- **Portée.** Toutes les éliminations sont conditionnelles à H1 (ou à la transposition précisée) et aux conventions Vigenère/Beaufort de la famille Quagmire. « Compatible » ne vaut jamais indice, sauf si c'est nettement plus rare que pour le hasard.
- **Antériorité (révisée le 23/09).** Un inventaire systématique du registre (`docs/knowledge_base/02_tested_refuted.md` §4) montre que plusieurs tests de cette campagne étaient des **redites** : clé ligne+colonne (E-S-10), robustesse aux nulles (E-SOLVE-10), clé courante K1–K3 (E-AUDIT-07, bijection 08b/08c), grille 98 = 14×7 (E-S-130), route de K3 sur K4 (E-CFM-07). L'apport propre se limite à la couverture de **tous** les alphabets mélangés et à la formulation fermée de la route de K3. Le registre contient aussi deux scripts Z3 (`e_z3_periodic_feasibility`, `e_z3_multilayer_feasibility`) marqués `last_run: never`, qui portent sur des alphabets fixes. Aucune entrée ne documente une élimination sur **tous** les alphabets mélangés. Des solveurs publics ont pu le faire ailleurs : cette note ne revendique pas une première mondiale.
