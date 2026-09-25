# Relais DeepSeek n° 2 (« Recoupement général : ce que la communauté n'a pas vu », 25/09/2026) : vérification point par point

**Statut :** `EXPLORATION RESULT`. Aucun signal. Le texte reformule surtout la base 7 §9 et §12 (autre session). Ses ajouts sont faux, invérifiables ou relèvent de la numérologie. La seule proposition testable (§3 ci-dessous) est faite, et ne rouvre rien.

**Outil.** Tout le calcul est en C + OpenMP : gcc 13.3, libgomp, 4 threads, AVX-512, compilation `gcc -O3 -march=native -fopenmp`. Le moteur commun de la branche est `../moteur_2026_09_25/k4x.c`.

## Point par point

| n° | Proposition | Vérification | Verdict |
|---|---|---|---|
| 1 | Les 8 signes raturés sous VTTMZFPK correspondent exactement à LINCLOCK (66–73) | C'est le rapprochement de la base 7 §12.2, fait par l'autre session. La table de reconnaissance existe (`../erreurs_multiples_2026_09_24/results_huit_signes.txt`) | **Déjà au dépôt.** Seule action : la photo demandée à Paradigm |
| 2 | Doublets en colonne 4 des blocs de 7 ⇒ clé à pas 7 ; pas de clé 1, 1, 5 = Fibonacci | Mécanisme déjà décrit (base 7 §12.1). **Les « pas de clé » 1, 1, 5 ne viennent pas de la clé.** À un doublet, c_i = c_{i+1} impose k_i − k_{i+1} = σ(p_{i+1}) − σ(p_i) : ce sont les écarts des lettres claires N → O, S → T, I → N dans l'alphabet choisi. En A–Z Vigenère : 1, 1, 5 ; en Beaufort A–Z : 25, 25, 21 ; en KRYPTOS : 12, 24, 4. Les familles « à pas 7 » ont été testées : T11, T11b, T16, T24–T26, alphabets à mot-clé (`../motcle_pas7_2026_09_24/` §1 : au moins 7 erreurs) | **Fibonacci : artefact** du choix de A–Z et des mots des cribs. Le reste est déjà testé |
| 3 | Les auto-chiffrements 32 (S → S) et 73 (K → K) : rejouer T18 et T11b en supposant que l'un est une lettre **recopiée**, les autres contraintes étant maintenues | **Déjà fait**, trois fois : T19 (T18 rouvert par une copie en 73 en Vigenère, en 32 en variante, mais 78 à 97 % des chiffrés aléatoires rouvrent aussi avec une erreur) ; T20 modèle B « copie » (T11b : la plupart des cas rouverts passent par 32, au niveau du hasard). **Complément du jour, T27**, pour la seule forme qui a du pouvoir, c'est-à-dire avec les alphabets à mot-clé (voir plus bas) : à l'écart 7, il faut **au moins 10 erreurs** ; **aucun** cas n'est sauvé par une copie en 32 ou 73, ni pour K4, ni pour les 15 témoins | **Rien ne rouvre** |
| 4 | Le fragment de 97 lettres est un modèle réduit de K4 | Le tableau **mélange deux fragments**. Les lettres doublées de remplissage (SS, LLL, II, EE) et la décimation en 16 colonnes sont ceux du fragment de **Zola** (512 lettres), pas du petit fragment de 97 lettres (base 7 §9.3–9.4). « K4 a 6 doublets comme SHADOW a 6 lettres » : numérologie. La période 6 est éliminée pour tout alphabet, même avec plusieurs erreurs (T22). Ce que le fragment apporte vraiment est déjà noté : le format de 97 lettres, un clair tronqué, environ 4 % d'erreurs | **Rien de nouveau ; une confusion factuelle** |
| 5 | Le « 1-2-3 » de Sanborn = décalages des lignes du tableau ⇒ clé construite par décalages successifs | Une clé qui avance d'un cran à chaque lettre, ou par bloc, est la clé progressive (éliminée pour tout alphabet le 22/09) ou la famille T11 (s·j par bloc). « Le masque change la base » : non formulé, donc non testable | **Déjà couvert** |
| 6 | « Compass cipher » ; les 8 signes seraient 8 directions | Seule une photo des signes peut le dire (base 7 §12.5 : numérologie pour « 66–67° » et « 8 K = 8 directions ») | **Invérifiable** en l'état |
| 7 | Retirer SS en 42 : 5 doublets sur 5 en colonne 4, p = 1/7⁵ ≈ 6 × 10⁻⁵ | **Deux fautes.** (a) La colonne est choisie après coup : la probabilité que 5 doublets donnés tombent dans **une même** classe est 7 × (1/7)⁵ = 1/2401, pas 1/16 807. (b) On ne retire pas l'exception parce qu'elle gêne : la statistique honnête reste « au moins 5 sur 6 dans une classe », déjà calculée (p ≈ 6 × 10⁻⁴ pour le module 7, ≈ 0,02 en balayant le module, base 7 §7.2). Rien ne désigne 42 comme une erreur ; hors cribs, rien ne peut le tester | **Faux** |
| 8 | Chercher si les pas de clé des paires à l'écart 7 forment une suite | Une différence de clé n'est calculable que si les deux positions sont dans les cribs. Parmi les 8 paires, une seule l'est : 65/72 (P → R et P → C). C'est justement la paire qui interdit toute substitution qui ne dépend que de i mod 7. Pour les autres, le clair est inconnu | **Non calculable** |
| 9 | Les 8 K de K4 marqueraient les positions de clé nulle | Il y a bien 8 K (2, 31, 45, 52, 73, 77, 86, 93). Mais en Vigenère ou en variante, quel que soit l'alphabet, clé nulle ⇔ chiffré = clair. **En 31, le chiffré K couvre le clair A** : la clé n'y est pas nulle. Seul 73 est K → K. 4 des 8 K sont ≡ 3 (mod 7), ce qui arrive au hasard avec p ≈ 0,1 | **Réfuté par le crib** |

## T27 — autoclé sur le clair, alphabets à mot-clé, erreurs comptées correctement (`t27_autocle_motcle_chaines.c`)

**Défaut trouvé dans deux programmes antérieurs.**
- `../motcle_pas7_2026_09_24/kwautokey.c` et la famille `a` de `../moteur_2026_09_25/k4x.c` propagent le clair depuis la **première** lettre de crib de chaque classe, sans ré-ancrage.
- En autoclé, une lettre de chiffré fausse corrompt toute la suite de sa chaîne. **Une seule erreur peut donc compter plusieurs fois.**

**Méthode juste.** On ré-ancre la chaîne à chaque lettre de crib. Entre deux positions de crib consécutives a < b d'une même classe, on propage depuis le clair connu en a et l'on compare en b. Chaque équation (a → b) n'est faussée que par une erreur dans ]a, b], et changer c_b suffit à la réparer. Donc **nombre minimal d'erreurs = nombre d'équations fausses**.
- Contrôle positif n° 4 : une lettre fausse en 42, au milieu d'une chaîne. Le nouveau comptage donne 1 erreur, l'ancien 2.
- `k4x.c` est corrigé en une ligne (ré-ancrage). Après correction, la famille `a` donne pour K4 **4 erreurs**, pour 4 à 5 chez les témoins (p = 0,29). Les conclusions de ces audits ne changent pas.

**Portée.**
- 202 172 alphabets distincts : mots du dictionnaire et mots thématiques, sous 4 formes.
- 5 types (Q3, Q2, Q1, Q4a, Q4b) ; lettre-clé lue en σ, en A–Z ou en KRYPTOS ; VIG, BEAU, VARB.
- Écarts L = 1 à 48, seulement les cas à au moins 8 équations : 228 656 532 cas par chiffré, environ 12 s sur 4 threads.
- Pour les cas à 0 ou 1 erreur avec L ≤ 13, on déchiffre les 97 lettres et on note l'anglais (quadrigrammes).

**Contrôles positifs.**
- PALIMPSEST, écart 7, VIG, **une copie en 73** : retrouvé seul à 1 erreur, marqué « copie », clair anglais −3,99.
- Même contrôle avec une lettre fausse en 42 : retrouvé à 1 erreur, −4,07.

**Résultats** (`results_t27.txt`).

| | Écart 7, erreurs minimales | Tous écarts, minimum | Sauvés par une copie en 32 ou 73 |
|---|---|---|---|
| K4 | **10** (sur 17 équations) | 2 (3 cas) | **0** |
| 10 témoins K4 mélangé | 9 à 10 | 2 à 3 | 0 |
| 5 témoins uniformes | 9 à 10 | 1 à 2 | 0 |

⇒ L'hypothèse de la NSA (autoclé sur le clair à l'écart 7) est **exclue pour les alphabets à mot-clé**. Il faudrait au moins 10 lettres fausses sur 24, et une copie en 32 ou 73 n'y change rien. Sur l'ensemble des écarts, K4 est au niveau du hasard. Avec un alphabet **libre**, T19 reste valable : une seule erreur rouvre la famille, comme pour un texte aléatoire, donc le test y est sans pouvoir.

---

# Recoupements du 25/09 : ce que la communauté n'a pas vu ? Pistes essayées et mesurées

**Démarche.** Partir des faits les plus solides, et chercher **un seul mécanisme** qui les explique ensemble :
- le « 7 » : doublets alignés et coïncidences à l'écart 7, p ≈ 2 × 10⁻⁴ ;
- l'absence de clé périodique : 65/72, et K4 est *moins* compatible avec la période 7 que le hasard (T22) ;
- l'échec de toutes les clés à pas 7 avec les alphabets de Sanborn.

Chaque idée est mesurée avant d'être retenue.

## 1. Un chiffre FRACTIONNANT à période 7, décalé sur la mise en page (T28, `t28_trifide.c`) : éliminé
- **Idée.** Un trifide de Delastelle (27 symboles) de période 7 produit un « 7 » sans clé périodique.
  - Si ses blocs commencent en 4, début de la première ligne pleine de K4 sur le cuivre, les doublets 18, 25, 32, 46 et 67 sont tous en tête de bloc.
  - En tête de bloc, les lettres chiffrées 0 et 1 sont faites de la même coordonnée de six lettres claires.
  - Chaque crib contient alors un bloc entier : NORTHEA et INCLOCK.
- **Antériorité.** Le registre déclare le trifide éliminé « pour toutes les périodes » (E-S-09, E-S-42b, E-S-44), probablement avec des blocs commençant à la première lettre. On teste ici **toutes les phases**.
- **Test exact.** Cube quelconque ; union-find sur les 81 trits, puis recherche d'un cube bijectif ; périodes 2 à 40, toutes phases (819 cas).
  - **K4 : 0 compatible** ; témoins 0/200 par cas ; contrôle positif (P = 7, φ = 4) 20/20.
- **Simulation** (200 000 textes anglais, cubes aléatoires) : le trifide de période 7 ne concentre **pas** les doublets sur une phase (0,48 à 0,61 par phase), et il ne donne que 4,1 coïncidences à l'écart 7, contre 9 pour K4.
- ⇒ **Piste fermée**, et l'antériorité est confirmée pour toutes les phases.

## 2. Clé lue verticalement sur la feuille 31 × 14, et « 1-2-3 »
- **Déjà éliminé** : clé = chiffré de K3 à la même colonne, d = 1 à 11 lignes plus haut (`../k3ct_key_2026_09_23/`), et texte de la feuille de K3 dans tous les sens (`../k3_chart_stack_2026_09_23/`).
- Le « 1-2-3 » de Sanborn (03/03/2019) concerne les lettres désalignées de K1–K3 : « no, those letters were intended to refer to "1-2-3" ». Ce n'est pas un indice sur K4.

## 3. Un motif de répétition jamais mesuré : KZ, TJ, DI « comprimés » (`motif_kz_tj_di.c`)
- **Fait.** Trois bigrammes se répètent dans le même ordre, régulièrement espacés aux deux endroits :
  - K45 Z46 · T50 J51 · D55 I56, soit un pas de 5 ;
  - K77 Z78 · T80 J81 · D83 I84, soit un pas de 3.
  - Les distances entre les deux occurrences diminuent : 32, 30, 28. Le groupe DI tombe les deux fois en fin de ligne d'une grille de 14 (colonne 13).
  - B. Briere (2009) avait noté la « répétition symétrique » sans la mesurer.
- **Mesure** (2 millions de mélanges de K4) : **p ≈ 1,4 × 10⁻³** pour « trois bigrammes répétés, en progression arithmétique aux deux endroits ». La statistique est définie après coup ; le facteur à payer pour ce choix est inconnu.
- **Aucun mécanisme connu** ne la produit. Dans une clé additive, il faudrait les mêmes clairs **et** les mêmes clés aux deux endroits, avec une lecture de la clé « un sur deux » : c'est l'allure d'une grille ou d'un masque qui prélève les mêmes lettres avec un autre pas.
- ⇒ **Curiosité à garder comme contrôle**, pas un signal.

## 4. GIRASOL : la résonance des doublets, recadrée
- **Rappel** (base 7 §12.1). Si les doublets de la phase 4 viennent d'un écart de clé fixe, l'alphabet du clair doit vérifier σ(O) − σ(N) = σ(T) − σ(S) = σ(N) − σ(I). Seuls **0,54 %** des alphabets à mot-clé le font.
- **GIRASOL** (le tableau de la maquette de 1988) le fait, avec un écart de **15** pour les trois paires. KRYPTOS et A–Z ne le font pas.
- **Recadrage.** L'autre session estimait p ≈ 0,07, en comparant à 80 mots thématiques. Mais l'ensemble des alphabets **que Sanborn a réellement employés** est fixé d'avance et petit : KRYPTOS, GIRASOL, et A–Z si l'on veut. Qu'un sur deux ou trois passe un filtre à 0,54 % donne **p ≈ 0,01 à 0,016**. C'est faible, mais cela relie deux faits indépendants : les doublets de K4 et l'alphabet de 1988.
- **Ce que GIRASOL ne donne pas.** Aucune structure de clé :
  - avec une clé de période 7 en Beaufort GIRASOL, il faudrait 14 erreurs sur 24 ;
  - les familles à pas 7, les autoclés et la clé courante avec GIRASOL tombent déjà (`../motcle_pas7_2026_09_24/`, où GIRASOL fait partie des mots thématiques).
- **Deux prédictions vérifiables**, si l'hypothèse « écart de clé fixe en phase 4, alphabet GIRASOL » est juste :
  1. Les doublets hors cribs, BB (18–19) et ZZ (46–47), cachent des bigrammes clairs d'écart 15 en GIRASOL. Parmi eux, en anglais courant : **IN, ST, NO, OU, HI, VE, PL, MS, UD**.
  2. **Les 8 signes sous LINCLOCK**, s'ils sont des lettres de clé en GIRASOL, se lisent :
     - Vigenère **MQABTLDG** ;
     - Beaufort **IUDXONZR** ;
     - variante **FCXTBUPG**.
     - Ces lignes s'ajoutent à la table de reconnaissance de `../erreurs_multiples_2026_09_24/results_huit_signes.txt`.

## Bilan honnête
- Aucun moment Eurêka **vérifié**. Les deux idées qui pouvaient expliquer le « 7 » d'un coup, le fractionnement décalé et la clé verticale, sont fermées par des tests exacts.
- Il reste :
  - une curiosité mesurée : KZ, TJ, DI (p ≈ 1,4 × 10⁻³, après coup) ;
  - un lien recadré, faible mais réel : GIRASOL passe la contrainte des doublets (p ≈ 0,01 parmi les alphabets de Sanborn) ;
  - deux prédictions précises pour le jour où l'on pourra lire les 8 signes.

---

# Troisième partie (25/09, suite) : modèles « à la Sanborn », testés exactement

Chaque modèle ci-dessous est un **geste plausible d'un artiste qui chiffre à la main**. Chacun a un contrôle positif (chiffré synthétique retrouvé) et des témoins « K4 mélangé ».

| Test | Modèle | Portée | K4 | Témoins | Verdict |
|---|---|---|---|---|---|
| **T29** `t29_pas7_phases.c` | « Disque tourné de temps en temps » : clé de 7 + décalage libre par bloc de 7, blocs commençant **à n'importe quelle phase** (kwsweep et k4x ne testaient que la phase 0). Ce modèle explique à lui seul les coïncidences à l'écart 7, les doublets à phase fixe et le conflit 65/72 | 101 086 alphabets à mot-clé × 5 types × VIG/BEAU (inverses et VARB couverts) × 7 phases | **au moins 5 erreurs** | au moins 5 (20 témoins) | **Éliminé** pour les alphabets de Sanborn |
| **T30** `t30_saut_de_phase.c` | Clé périodique où Sanborn « perd sa place » : saut de phase de ±1 à ±3, **y compris dans un crib** (le registre ne couvrait que les sauts entre les cribs) | p = 2–13, x = 22–73, alphabet **quelconque**, 3 conventions ; 11 232 cas | compatible dans 115 cas | **491 attendus** | Sous le hasard. Le seul groupe rare (p = 8, Beaufort, saut +2 entre 64 et 67 ; 1 à 3 % des témoins de même égalité de Bean, `t30b_bean.c`) est une fluctuation parmi des milliers de cas |
| **T31** `t31_saut_motcle.c` | Même modèle (un ou deux sauts, dont un dans chaque crib), **alphabets de Sanborn** | p = 2–13, sauts ±1 à ±3, 13 735 configurations × 101 086 alphabets | au moins 3 erreurs (5 cas, une seule configuration, alphabets sans lien avec Sanborn) | 3 à 4 (et plus) | Au niveau du hasard |
| **Clé courante non anglaise** (k4x `-e r` avec d'autres tables) | Clé tirée d'un texte **allemand** (thème Berlin), **français** ou **latin** ; alphabets à mot-clé | quadrigrammes : 11 livres allemands, 10 français, 2 latins ; contrôle positif allemand retrouvé (−4,26) | −5,12 / −4,80 / −5,06 | p = 0,76 / 0,24 / 0,71 | **Aucun signal** (`results_cle_courante_langues.txt`) |

## La maquette de 1988, relue en entier (`maquette_1988_erreurs.py`)

Le bloc de démonstration (7 × 21, Vigenère A–Z, clé RUG) avait une « dernière ligne altérée au relevé ». **Elle ne l'est pas.** On y trouve trois erreurs de Sanborn :
1. **Une lettre omise** en 133, le T de « (T)HE KEY ». Toute la suite de la clé glisse d'un rang, et la dernière ligne se lit « WITHOUT HE KEY ETRANS… ». C'est l'erreur de l'X omis de K2 (« charabia »), vingt ans avant qu'il l'avoue.
2. **La même erreur de table, deux fois** (121 et 127) : clair I avec la clé U donne **B** au lieu de C, une case trop à gauche. D'où « DECHPHER » et « WHTHOUT ».
3. « INTU » pour « INTO » (position 20) : faute du clair, ou lecture du relevé.

Avec le petit fragment de 97 lettres (4 erreurs, dont 3 du même écart), cela fait deux chiffres manuels de Sanborn, et dans les deux ses erreurs sont **systématiques** : même case mal lue, même glissement de clé, lettre omise.

## Bilan de la troisième partie
- **Quatre modèles « artistiques » nouveaux**, que les tests antérieurs ne couvraient pas, sont éliminés ou au niveau du hasard : phases de blocs libres, sauts de phase dans les cribs, clé courante en allemand, français ou latin.
- **Fait documentaire nouveau** : la maquette de 1988 porte les mêmes types d'erreurs que les chiffres ultérieurs de Sanborn (lettre omise avec glissement de clé, erreur de table répétée).
- **Défaut corrigé en cours de route** : dans T30 et T31, l'indice de clé était d'abord réduit modulo 26 avant de l'être modulo p. Le contrôle positif de T30 passait quand même, car il était construit avec la même erreur ; celui de T31 a révélé le défaut. Tous les résultats ci-dessus sont ceux de la version corrigée.
