# Audit « recherche de procédés » (25/09/2026, soir) : chercher le mécanisme, pas la clé

**Ce qui est nouveau.** Personne n'avait cherché automatiquement parmi les **procédés** de chiffrement. Ni dans ce dépôt, ni dans les archives du groupe : la seule trace est un algorithme génétique sur des clés, Materna 2019. On fait ici deux choses :
1. une **borne générale** : combien de doublets un chiffre déchiffrable peut-il produire, quelle que soit sa clé ;
2. une **recherche exhaustive** dans une grammaire de procédés manuels déchiffrables (environ 20 000 procédés), notés sur leur ressemblance avec K4 et étalonnés par des témoins et deux contrôles positifs.

| Fichier | Rôle |
|---|---|
| `borne_doublets.py`, `results_borne_doublets.txt` | borne sur le taux de doublets (anglais Gutenberg, 16,8 M lettres) |
| `recherche_procedes.c` | grammaire, simulation, note de vraisemblance, signature jointe (C/OpenMP) |
| `results_recherche_K4.txt` | K4, σ = KRYPTOS, un terme |
| `results_recherche_K4_deux_termes.txt` | K4, combinaisons de deux termes |
| `results_recherche_K4_AZ.txt` | K4, σ = A–Z |
| `temoins.txt`, `results_recherche_temoins*.txt` | 12 K4 mélangés (un terme), 8 (deux termes) |
| `ctl_procede*.txt`, `results_ctl_procede*.txt` | deux contrôles positifs (faux K4 de procédé connu) |
| `t35_fautes_de_recopie.c`, `results_t35.txt` | T35 : le « 7 » vu comme des fautes de recopie d'une feuille de 7 colonnes |
| `geometrie_14x7.py`, `amas_14x7.py` et leurs résultats | géométrie des répétitions sur la feuille 14 × 7, sans hypothèse de clé (§5) |
| `agrandir_signes_nova.py` | agrandit les 8 signes de la feuille NOVA depuis `Personal Folders/pi/K4 NOVA.jpg` (image non versionnée) |

## 1. La borne : ce qu'un chiffre déchiffrable peut faire

**Argument.** Si le chiffre est déchiffrable lettre à lettre, alors, pour un état donné (clé, texte déjà chiffré), la lettre chiffrée en i + 1 est une bijection de la lettre claire en i + 1. Il y a donc **une seule** lettre claire, notée f(contexte), qui produit c_{i+1} = c_i. D'où :

> P(doublet) = P(p_{i+1} = f(contexte)).

Un doublet est donc une « prédiction » de la lettre claire suivante. Le taux dépend de ce que l'état « sait » du clair.

| Ce que l'état sait (famille de chiffres) | Taux maximal | P(≥ 5 doublets sur 14) | Avec les cribs (NO, ST, IN) | P(≥ 5 sur 14) |
|---|---|---|---|---|
| clé seule, alphabet A–Z ou KRYPTOS (Vigenère, Quagmire, clé quelconque) | 6,3–6,7 % | 0,1 % | **0 % (impossible)** | 0 |
| clé seule, alphabet GIRASOL | 7,1 % | 0,2 % | 7,0 % | 0,2 % |
| clé seule, meilleur des 101 086 alphabets à mot-clé | 13,6 % | 3 % | 9,9 % | 0,9 % |
| clé seule, alphabets **quelconques** par position (bijection libre) | 17,1 % | 7 % | 16,0 % | 6 % |
| clé dépendant de la lettre claire précédente (type autoclé), fonction libre | 22,6 % | 19 % | 21,9 % | 17 % |
| clé dépendant des deux lettres claires précédentes | 30,8 % | 44 % | 28,2 % | 36 % |

Colonne « avec les cribs » : même relation entre les colonnes 4 et 5 à toutes les lignes (une seule f), contrainte à donner NO → QQ, ST → SS et IN → TT. Si la relation change d'une ligne à l'autre, chaque doublet de crib ne vaut plus que 1/26 avec une clé tirée au hasard, et le taux des autres cases reste sous les bornes de la colonne « taux maximal ».

**Lecture.**
- **Toute clé indépendante du texte** (périodique, courante, tirée d'une matrice, choisie au hasard, de n'importe quelle longueur) rend improbable la concentration de K4, soit 5 doublets sur les 14 cases de la colonne 4.
  - Avec A–Z ou KRYPTOS et une relation de clé fixe entre les colonnes 4 et 5, les cribs la rendent même impossible (GIRASOL : 0,2 %).
  - Même avec des alphabets totalement libres, on reste à 6 % au mieux. Ce résultat couvre d'un coup toutes les familles de clés, testées ou non.
- **Pour rendre ces doublets probables**, l'état doit « deviner » la lettre claire suivante à partir des précédentes : I → N, S → T, N → O (P(N|I) = 26 %, P(T|S) = 17 %, P(O|N) = 9 %). Il faudrait une clé qui dépend du clair **et** des alphabets accordés aux successions de l'anglais. C'est très peu vraisemblable pour un chiffre fait à la main.

## 2. La recherche de procédés

**Grammaire.** Un chiffre positionnel et déchiffrable, dans l'alphabet σ (KRYPTOS, ou A–Z) :
- σc = σp + k (Vigenère), k − σp (Beaufort) ou σp − k (Variante) ;
- k = base + terme, où la base est une période 7, une clé par ligne de 7, une clé courante anglaise ou une clé progressive ;
- le terme est la lettre claire ou chiffrée située L rangs avant (L = 1–8, 14), lue en σ, en A–Z ou dans un alphabet à mot-clé ρ tiré au hasard, avec un signe ± ;
- il s'applique à toutes les colonnes, à une seule, ou à toutes sauf une.

On écarte les procédés qui laissent une colonne sans clé, où le clair passerait tel quel. Cela fait **19 776 procédés à un terme**, puis leurs combinaisons à deux termes. Chaque procédé est simulé 5 000 fois sur de l'anglais avec les cribs à leur place.

**Note.** On calcule la log-vraisemblance des comptes de K4 sous les taux du procédé, puis on la compare à une clé courante (quasi hasard). Les comptes retenus sont :
- les doublets par colonne mod 7 ;
- les coïncidences aux écarts 7, 14 et 21 ;
- les trois doublets des cribs ;
- les paires égales dans chaque colonne (écarte les procédés qui laissent voir le clair).

Pour les meilleurs procédés, la signature jointe (profil de l'écart 7, au moins 5 doublets en colonne 4, les 3 doublets des cribs) est estimée directement sur 0,5 à 2 millions de tirages.

**Contrôles positifs.**
1. Faux K4 fabriqué par « clé par ligne − chiffré précédent, sauf colonne 5 » : le procédé est retrouvé en tête, à égalité avec son équivalent Beaufort, qui a les mêmes statistiques.
2. Faux K4 fabriqué par « période 7 + clair[i−3] en colonne 4 » : la famille « période 7 » est reconnue en tête ; le détail du terme ne l'est pas, ce qui est normal sur 97 lettres.

**Résultats sur K4.**

| | Meilleur procédé | ΔlogL | Doublets colonne 4 | P(≥ 5 doublets col. 4) | P(signature jointe) |
|---|---|---|---|---|---|
| σ = KRYPTOS, un terme | clé par ligne de 7, − chiffré[i−1], sauf colonne 5 | 7,9 | 10,8 % | 1,1 × 10⁻³ | 0 sur 2 × 10⁶ |
| σ = KRYPTOS, deux termes | le même, + clair[i−5] | 7,8 | 10,8 % | — | — |
| σ = A–Z, un terme | clé par ligne de 7, − clair[i−7], sauf une colonne | 8,6 | 11,2 % | 2,0 × 10⁻³ | ≤ 2 × 10⁻⁶ |
| témoins (K4 mélangé : 12 à un terme, 8 à deux termes) | — | 1,2 à 4,8 (un terme) ; 0,6 à 4,8 (deux) | — | — | — |

**Pourquoi ces procédés sortent en tête, et pourquoi cela ne prouve rien.**
- Dans ces procédés, la clé **s'annule** dans la condition de doublet de la colonne 4. Le doublet ne dépend alors que du clair, et les cribs le prédisent.
  - Au rang 1 (KRYPTOS) : la somme alternée de ORTHEAST (26–33) dans l'alphabet KRYPTOS vaut exactement 0, donc SS en 32 est forcé.
  - Au rang 1 (A–Z) : autoclé à l'écart 7, avec S − N = T − O = 5, donc SS en 32 est forcé. C'est la « chaîne 18–25–32 » déjà notée (base 7 §8.2).
- Chaque procédé de ce type a environ 1 chance sur 26 de « prédire » un doublet de crib donné. Sur des milliers de procédés, en trouver qui prédisent **un** doublet est attendu. Les meilleurs en prédisent un ; aucun ne prédit les trois.
- Les témoins mélangés ont perdu les doublets de K4 : l'écart de note mesure les traits de K4, pas un mécanisme.
- Surtout, **aucun procédé ne produit la concentration**. Au mieux 11 % de doublets en colonne 4 (K4 : 36 %), 0,2 % de chances d'en avoir 5, et une signature jointe pratiquement jamais obtenue.

## 3. Bilan

- **Borne générale (nouveau).** Aucun chiffre à clé indépendante du texte ne rend probables les doublets alignés de K4. C'est impossible avec A–Z ou KRYPTOS et une relation fixe entre les colonnes 4 et 5, et cela reste à 6 % au plus avec des alphabets quelconques. Il n'est donc pas utile de chercher les doublets dans une nouvelle famille de clés.
- **Recherche de procédés (nouveau).** Parmi environ 20 000 procédés manuels déchiffrables, aucun ne reproduit l'empreinte. Les meilleurs ne font que prédire le doublet SS en 32 à partir du crib, par une coïncidence arithmétique attendue.
- **Il reste trois explications, toutes hors d'un algorithme de chiffrement « propre ».**
  1. **Le hasard** : environ 10⁻³ à 10⁻² après correction.
  2. **Un choix délibéré** : Sanborn aurait fixé des lettres de clé pour créer des doublets visibles, comme motif. Il emploie déjà des lettres doublées de remplissage dans le fragment Zola. Mais 3 doublets correspondent à des lettres claires connues, ce ne sont donc pas des nulles.
  3. **Un élément qui n'est pas lettre à lettre** : homophones, nulles, correspondance « non 1:1 » à laquelle Sanborn ne s'est jamais laissé enfermer.
- **Conséquence pratique.** Les doublets alignés ne sont pas une empreinte de clé à retrouver. Ce sont soit du bruit, soit la trace d'une intervention manuelle.

## 4. T35 : et si le « 7 » était une faute de recopie ? (hors cribs dans l'idée, cribs pour le test)

**Idée.** Aucun chiffre déchiffrable ne produit l'empreinte de K4 (§1–§2). Une **recopie à la main** depuis une feuille de 7 colonnes la produit, en revanche, sans effort :
- l'œil qui glisse sur la case du dessus recopie la lettre 7 rangs avant. Cela donne des répétitions à l'écart 7, jamais à 14 : c'est exactement le profil « de proche en proche » ;
- la lettre précédente répétée au même endroit de chaque ligne (pli, blanc entre deux groupes) donne des doublets dans une seule colonne.

C'est le seul modèle rencontré qui explique les deux traits à la fois. Il désigne **sans ajustement** les lettres fautives : la seconde de chaque répétition. Dans les cribs, cela fait M = {22, 26, 33, 68, 72}. Le chiffre sous-jacent pourrait alors être simple, par exemple une période 7 (feuille de 7 colonnes, KRYPTOS).

**Test.** On retire M des cribs, il reste 19 lettres, puis on reteste les clés périodiques p = 1–26 de deux façons : (A) alphabet quelconque, Quagmire III, solveur exact ; (B) 101 088 alphabets à mot-clé, 5 types. On compare avec 200 masques de 5 positions de crib tirés au hasard.

| | Masque M | Masques au hasard |
|---|---|---|
| (A) cas compatibles sur 78, alphabet quelconque | 16 (aucun à p = 7 ; surtout p ≥ 14) | 18,9 en moyenne ; 41 sur 60 font au moins aussi bien |
| (B) erreurs encore nécessaires à p = 7, alphabets à mot-clé | 7 | 5 à 7 (surtout 6) |
| (B) Σ e_min pour p ≤ 13 | 87 | 83,6 en moyenne ; 193 sur 200 font au moins aussi bien |

**Résultat.** Retirer les lettres désignées par le modèle n'aide **pas**. M fait même un peu moins bien que des masques au hasard. Le modèle « fautes de recopie + clé simple » est donc **éliminé** : avec ou sans ces lettres, aucune clé périodique ne s'ajuste. Des fautes de recopie restent possibles, mais le chiffre qui est dessous n'est pas simple, et on ne gagne rien.

## 5. Pas une question de familles : la forme des répétitions sur la feuille 14 × 7

Tout ce qui suit se fait sans aucune hypothèse de clé. Avec le « ? » en tête, K4 fait **98 = 14 × 7** caractères, soit un rectangle plein. Les comparaisons portent sur 100 000 mélanges des lettres de K4.

**Directions de voisinage** (`geometrie_14x7.py`). Deux relations seulement dépassent le hasard, toutes deux orthogonales :
- lettre = lettre du dessus : 9 contre 3,25 (p = 0,005). Les 9 cas se répartissent sur les colonnes 0, 1, 1, 2, 3, 3, 4, 5, 6 ;
- dernière paire de chaque ligne : 5 doublets contre 0,5.

Le reste est au hasard : diagonales 2 et 3 (contre 2,8), deux cases plus loin 2 et 2 (contre 2,5 et 3,0).

**Amas** (`amas_14x7.py`). Les lettres identiques forment **3 amas d'au moins 3 cases**, contre 0,5 attendu (p = 0,011), et 27 cases appartiennent à des amas d'au moins 2, contre 11,9 (p = 0,002) :
- O O O, en colonne 1, lignes 0 à 2 ;
- B en équerre, (1,6) (2,5) (2,6) ;
- S en équerre, (4,5) (4,6) (5,5).

Les deux équerres touchent le bord droit. Ces amas redisent les mêmes répétitions sous une autre forme : ce n'est pas un signal indépendant.

**Comparaison avec le fragment Zola.** Sanborn y répète la dernière lettre d'une ligne chiffrée pour l'allonger jusqu'à la marge (…ULL, …AWW, …RSSS). Ces lettres sont **ajoutées**, puis retirées avant de déchiffrer.
- Dans K4, cela ne tient pas : le clair enregistré fait 97 lettres, avec les cribs aux mêmes rangs que le chiffré. Un ajout avant le rang 21 décalerait tout.
- Et les secondes lettres des doublets de crib (26, 33, 68) portent de vraies lettres claires (O, T, N).
- Le lien avec Zola se réduit donc à une **habitude de la main** : doubler une lettre en bout de ligne. Dans K4, elle ne serait pas une lettre ajoutée mais une lettre **remplacée**, donc une perte d'information.

**Lecture.**
- Le « 7 » décrit une feuille de 14 × 7 qui commençait sans doute par le « ? », et invisible sur le cuivre. Sur cette feuille, chaque case tient à la case du dessus, la dernière case de chaque ligne tient à sa voisine de gauche, et rien ne passe en diagonale.
- Aucune clé appliquée lettre à lettre ne produit cela (borne du §1). Une recopie distraite non plus : elle aurait des diagonales et se concentrerait en début de ligne.
- Restent une opération faite **ligne à ligne sur la feuille**, qui n'est pas lettre à lettre (masque, retouche), ou le hasard (10⁻³ à 10⁻² après correction).
- Retirer les lettres concernées ne rend aucune clé simple compatible (T35, §4). Le mécanisme ne se reconstituera donc pas à partir des 97 lettres seules.

## 6. Observation du 25/09 (nuit) : sur EASTNORTHEAST, le chiffré n'est que le clair décalé de ±2

- En Vigenère A–Z, la clé des positions 21–33 vaut, en décalage signé, **1, 11, −1, 2, 3, 2, −2, −2, 6, 2, 10, 0, −1**. Neuf lettres sur 13 sont chiffrées à ±2 rangs de leur clair (E→F, S→R, T→V, O→Q, R→P, T→R, E→G, S→S, T→S).
- Sur BERLINCLOCK : 12, −6, −2, 10, 11, 6, 10, −12, −9, −13, 0, soit **2 sur 11**, le niveau du hasard.
- **Significativité** (K4 mélangé, 3 000 tirages) : p nominal 2 × 10⁻⁵. En corrigeant pour la recherche (alphabets A–Z ou KRYPTOS de chaque côté, VIG et VARB, BEAU centré n'importe où, seuils ±1, ±2, ±3, deux cribs), **p = 0,006**.
- C'est l'effet E0b de Materna et Bean (lettres de KRYPTOS « presque en place »), vu autrement : il ne concerne pas les lettres de KRYPTOS, mais **toute la zone du premier crib**, et disparaît sur le second.

**Conséquence testable** (`../moteur_2026_09_25/viterbi_pm2.c`). Si la zone est chiffrée par de petits décalages, le chiffré voisin devrait redonner de l'anglais en décalant chaque lettre d'au plus ±2. On cherche le meilleur anglais par programmation dynamique sur les quadrigrammes.
- **Contrôle** (anglais + clé aléatoire dans [−2, 2], 40 lettres) : −3,66 par quadrigramme, contre −3,97 en moyenne pour les témoins ; p = 0,02. Le test a peu de puissance, car 5 choix par lettre suffisent à fabriquer du pseudo-anglais.
- **K4**, positions 0–63 : −4,04 (« NDISTOWNHISMASONHEADVENTUPONTOHISTORY… »), témoins −4,19 en moyenne, **p = 0,08**. Fenêtres 0–21, 21–34 et 34–63 : p = 0,26, 0,17 et 0,22.
- Sur le crib, EASTNORTHEAST n'est pas retrouvé : quatre de ses décalages (+11, +10, +6, +3) sortent de ±2.

**Lecture.** L'écart est réel au niveau de 0,6 % après correction. Mais il ne se prolonge pas de façon détectable hors du crib. Le projet a regardé des centaines de statistiques ; un p de 0,006 y est attendu de temps en temps. À garder comme un fait à expliquer : clé proche de A sur 21–33, mais pas sur 63–73.

**Localisation sans clair (25/09, nuit).** Idée : une zone chiffrée par des décalages de ±2 garde dans ses lettres chiffrées la trace des fréquences de l'anglais (lettres autour de E, T, A, O). On fait glisser une fenêtre de 13 lettres sur K4 et on note sa vraisemblance sous « anglais brouillé de ±2 ».
- Résultat : 21–33 donne +1,36, 63–73 donne 0,03. Des fenêtres voisines de 24–44 sont légèrement positives, et 80–92 aussi.
- Mais 68 % des K4 mélangés ont une fenêtre qui fait au moins aussi bien : **le test n'a aucune puissance**. Sur 13 lettres, les fréquences seules ne distinguent rien.
- La zone du phénomène ne peut donc pas être localisée à partir du chiffré seul.
- **Explication écartée** : une clé en « période 7 + décalage par ligne du cuivre » presque nulle sur la ligne 26 (celle d'EASTNORTHEAST). Elle imposerait une clé constante sur chaque ligne de BERLINCLOCK, alors qu'on lit 12, −6, −2 sur la ligne 27 et 10, 11, 6, 10, −12, −9, −13, 0 sur la ligne 28.
