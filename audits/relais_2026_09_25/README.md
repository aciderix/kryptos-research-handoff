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

## Quatrième partie (25/09, suite) : les dernières familles de clés, avec les alphabets de Sanborn

| Test | Famille | K4 | Témoins | Verdict |
|---|---|---|---|---|
| **T32** `t32_progressive_motcle.c` | **Clé progressive** : mot-clé de p lettres qui avance de s à chaque tour (p = 2–26, s = 1–25, tour commençant n'importe où). Avec un alphabet libre, la famille était trop lâche (compatible pour p = 8, 10–13) | p ≤ 11 : plus de 5 erreurs ; p = 12–13 : 5 | 3 à 5 à p = 12–13 ; même niveau ailleurs | **Aucun signal** ; contrôle positif (PALIMPSEST, p = 8, pas 3) retrouvé à 0 erreur |
| **T33** `t33_cle_chiffres.py` | **Toute clé faite de chiffres 0–9** : Gronsfeld, Gromark quelle que soit l'amorce, dates, coordonnées de K2, heures. Filtre : les 24 valeurs de clé imposées par les cribs doivent être dans 0–9 (au hasard, ≈ 10⁻¹⁰ par alphabet) | au mieux 3 valeurs hors 0–9 | 2 à 5 (30 témoins) | **Aucun signal** pour toute clé numérique avec un alphabet à mot-clé. Avec une constante ajoutée (fenêtre de 10 valeurs n'importe où), le filtre n'a plus de pouvoir (témoins 0–2, K4 1) |

**Où en est-on.** Avec les alphabets que Sanborn a toujours employés (mots-clés, dont KRYPTOS, GIRASOL, A–Z), **toutes** les familles de clés « faisables à la main » testées sont éliminées ou au niveau du hasard :
- période, avec ou sans erreurs ni sauts de phase ;
- clé de 7 décalée par bloc ou par ligne, à toute phase ;
- clé progressive ;
- autoclés sur le clair et sur le chiffré ;
- clé courante en anglais, allemand, français ou latin ;
- clé numérique de toute sorte ;
- clé transposée à la K3 ;
- convention changée par ligne.

Il reste trois possibilités, qu'aucun test sur les 24 lettres des cribs ne peut trancher :
1. **L'alphabet n'est pas un alphabet à mot-clé** (tableau tourné ou retourné d'une façon non standard, pochoir). Avec un alphabet libre, la plupart de ces familles deviennent trop lâches pour être jugées.
2. **La correspondance n'est pas lettre à lettre.** E. Dunin (2023) : Sanborn parle de rangs dans le clair et « recule dès qu'on précise 1:1 ». Les compositions avec transposition sont largement couvertes par le registre (`docs/two_systems_landscape.md`, preuve de Bean pour toute permutation avec une clé périodique), mais pas pour toute clé.
3. **Une clé choisie à la main, sans générateur** (liste de mots, pochoir posé sur le tableau : « Stencil Patterns, circa 1988 », dossier scellé par Sanborn aux AAA).

## Cinquième partie (25/09, soir) : relais DeepSeek n° 3, « les 8 signes sous LINCLOCK sont la clé »

**La thèse.** Les 8 signes raturés sous VTTMZFPK (feuille NOVA 2006) seraient la clé des positions 66–73 : KLGKORNA en Vigenère A–Z, OIECBAQK en Vigenère KRYPTOS, etc. Ils élimineraient d'un coup la période 7, les deux autoclés à l'écart 7, les clés constante, progressive, LCG et Fibonacci. Ils donneraient un « F » en ASCII par la parité, et la graine d'un registre à décalage (LFSR).

**Vérification point par point.**

| Affirmation | Vérifié | Verdict |
|---|---|---|
| Table des clés en 66–73 (6 variantes) | identique à `../erreurs_multiples_2026_09_24/huit_signes.py` (base 7 §12.2) | **exact, déjà au dépôt** |
| « Les 8 signes sont la clé » | ces 8 lettres sont **calculées à partir du crib** (chiffré gravé + clair LINCLOCK). Personne n'a lu les signes | **raisonnement circulaire**. La table dit ce qu'on lirait *si* les signes étaient des lettres de clé ; elle ne dit pas qu'ils le sont |
| « Test à 8 lettres plus fort que les cribs » | c'est une réécriture de 8 des 24 lettres de crib | **aucune information nouvelle** |
| Période 7 impossible (K25 − K26 = 1, K32 − K33 = 1, K67 − K68 = 5) | vrai en Vigenère A–Z : K25 = 3, K32 = 0, K67 = 11 devraient être égaux | **exact mais connu** (Bean) ; T22 élimine toute période ≤ 26 pour **tout** alphabet, même avec erreurs |
| Autoclé sur le clair à l'écart 7 impossible (K73 = A ≠ P66 = L) | vrai en A–Z et en KRYPTOS | **connu** (T18, NSA). Une seule équation, et elle porte sur 73 (K → K), la lettre qui pourrait être une copie « à la Sanborn ». Donc aussi fragile à une erreur que T18 (T19). L'élimination robuste vient de T27 et T34 |
| Autoclé sur le chiffré à l'écart 7 impossible | même équation isolée | **connu** (T23 : au moins 5 erreurs, tous alphabets) |
| Période 14 « encore possible, K(11) = L, K(12) = G » | **faux** : 67 mod 14 = 11 veut dire K67 = K11, où K11 est inconnu. Les signes ne donnent pas K11 | erreur d'indice ; p = 14 déjà éliminée (T22) |
| Clés constante, progressive, LCG, Fibonacci | vrai | connu (T32 : progressive avec alphabets à mot-clé, aucun signal) |
| Parité → 01000110 = « F », « invariant structurel des trois conventions » | l'invariance est **arithmétique** : 26 est pair, donc C − P, C + P et P − C ont la même parité. En KRYPTOS on obtient 0xF4, qui n'est pas une lettre. L'ordre des bits et le codage (A = 0) sont choisis après coup | **numérologie** : environ une chance sur 10 de tomber sur une lettre majuscule, et beaucoup de lectures possibles |
| Le « pas » entre colonnes 4 et 5 dérive (1, 1, 5) : clé 2D, grille 14 × 7 | vrai en A–Z. Avec un alphabet à mot-clé, l'écart devient σ(O) − σ(N), σ(T) − σ(S), σ(N) − σ(I) (empreinte des doublets, base 7 §12.1). La « grille 14 × 7 » avec une valeur par ligne et une par colonne est la famille R (décalage par ligne de 7) | déjà testé : alphabets à mot-clé, au moins 6 erreurs, comme les témoins (`../moteur_2026_09_25/`, T1) ; alphabet libre, 75 % des témoins compatibles (trop lâche) ; T29 à toute phase, éliminé |
| Récurrence d'ordre 3 ou plus, LFSR à 8 états | **testé ici** | voir ci-dessous |

**Test nouveau : la clé suit-elle une récurrence linéaire (LFSR) ?** (`rec_lfsr.c`)
- **Modèle.** k_n = a₁k_{n−1} + … + a_r k_{n−r} + c (mod 26), ordre r = 1 à 5, coefficients inconnus.
- **Résolution.** Exacte, par élimination de Gauss dans GF(2) et GF(13) (restes chinois). Sur les deux fragments de clé donnés par les cribs (21–33 et 63–73), sur chacun seul, et sur les 8 positions 66–73 seules.
- **Portée.** 101 086 alphabets à mot-clé × 2 sens × 5 types × 3 conventions, 30 témoins K4 mélangé ; A–Z et KRYPTOS avec 2 000 témoins.
- **Contrôle positif** (clé k_n = 3k_{n−1} + 5k_{n−2} + 7k_{n−3} + 11, Quagmire III KRYPTOS) : retrouvé, p = 0,02 sur 50 témoins.
- **K4, deux fragments ensemble** : **aucune récurrence d'ordre 1 à 5**, avec aucun des 3 millions de cas (témoins : 0 aussi). Une récurrence qui engendrerait toute la clé est donc éliminée pour les alphabets à mot-clé.
- **Fragments isolés, ou 66–73 seul.** Il reste assez d'inconnues pour que des récurrences locales existent. K4 en a autant que les témoins (p = 0,16 à 1).
- **Le seul écart apparent.** En A–Z et KRYPTOS, sur 66–73 à l'ordre 3, K4 compte 17 cas, contre au plus 29 pour 2 000 témoins (p = 0,007). Examiné (`rec_decode.c`) :
  - les 17 cas se réduisent à **4 récurrences distinctes**, car en A–Z les types Q1/Q2/Q3 sont le même chiffre, et le sens inverse permute les conventions ;
  - 34 % des témoins ont au moins une récurrence ;
  - aucune ne reproduit BER (63–65), ni a fortiori EASTNORTHEAST ;
  - prolongées au-delà de 73 (lecture « graine de registre »), elles donnent du charabia. Exemple en A–Z : « TENWOULKKZMJVGAIAQRHUDT ». Le « TENWOUL » initial ne tient pas.

**Bilan.** Rien de neuf dans la thèse, sinon le test LFSR, qui est négatif.
- Les éliminations annoncées sont exactes, mais elles étaient connues et découlent des cribs, pas des signes.
- Les signes restent **non lus**. Leur nature n'est pas établie : lettres de clé, lettres retirées, ou annotation du clair.
- Seule une photo de l'original trancherait. L'utilisateur a choisi de ne pas la demander.

## Sixième partie (25/09, soir) : réponse de DeepSeek (« lire les signes sans Paradigm ») : vérifiée sur les images

**Propositions et vérifications.**

1. **« 8 lettres de clé connues contraindraient la clé ; des familles éliminées pourraient rouvrir. »**
   - **Faux.** Si les signes sont des lettres de clé, leurs valeurs sont **forcément** celles de la table (clé = chiffré − clair, déjà connus en 66–73), à la convention près.
   - Elles ne peuvent donc ni rouvrir une famille éliminée par les cribs ni en éliminer une autre : c'est la même information.
   - Leur seul apport possible serait d'**identifier la convention et l'alphabet**. Par exemple, lire OIECBAQK désignerait Quagmire III KRYPTOS en Vigenère. Il faudrait pour cela les lire.
   - Le « test de cohérence des signes avec les structures de clé » (§4 de la réponse) est lui aussi exactement le test sur les cribs restreint à 66–73. `rec_lfsr.c` l'a déjà fait, fragment « 8 signes ».

2. **« Les signes étaient lisibles en 2006 (rushes NOVA) ; le correcteur n'est arrivé qu'en 2026. »** Vérifié sur les images du dépôt :
   - `sources/groupsio_membres/fichiers/Personal Folders/pi/K4 NOVA.jpg` (« I circled the point of interest ») : image extraite de la vidéo NOVA, 1209 × 553. Agrandie et contrastée (`nova_8_taches_agrandi.png`), elle montre une rangée oblique d'environ 8 **taches sombres pleines**. On ne peut pas lire leur forme.
   - `.../pi/number 3.jpg` (« Jims K4 notes from NOVA ») et `.../Brandon's Files/K3K4 Video Stills.pdf` (2013, images à 04:35 et 04:36 de la vidéo de 12:36) : même rangée à côté de « TOP #4 » et du visage souriant. Brandon écrit : « *The 8 blobs here could be magic marker or wax pen drawn over the plastic sheet to conceal whatever is underneath* ».
   - ⇒ **Dès 2006, les signes étaient des taches opaques qui masquaient quelque chose.** Ce n'étaient pas des caractères lisibles. Le correcteur vu en 2026 n'a rien fait disparaître de lisible. Les rushes NOVA ne peuvent pas les révéler, sauf à trouver une image nettement meilleure, ce qui semble peu probable.
3. **« Les photos RR Auction de 2025 montrent la feuille avant le correcteur. »**
   - **Non.** La maison a annoncé qu'« aucune photo ne sera fournie des pièces 1 à 7 », c'est-à-dire tout le matériel de travail de K4 (base 1 §4 quinquies).
   - Les photos publiées (plaque d'essai, contrat, badge, atelier, discours) ne montrent pas cette feuille.
4. **« Les dossiers AAA 6/18 et 6/19 peuvent contenir la feuille. »**
   - **Déjà lus en entier** (6/18 : 17 images ; 6/19 : 124/124 ; base 5).
   - 6/19 ne contient que des propositions de tiers. 6/18 contient des chartes et de la presse, pas la feuille 31 × 14.
5. **« Les signes ne sont peut-être pas des lettres. »** D'accord : on ne peut pas savoir, puisqu'ils sont masqués.
   - Mais l'exemple avancé est faux : il y a **9** coïncidences à l'écart 7, et non 8 (positions 0, 7, 12, 15, 32, 45, 65, 76, 86). DeepSeek oublie la position 7.
   - Chiffres, flèches, lettres de clé, LINCLOCK recopié, lettres retirées : aucune hypothèse n'est testable tant que le dessous n'est pas visible.

**Bilan.** La réponse a raison sur l'essentiel : on ne connaît pas les signes, et toute table en 66–73 est une hypothèse. Mais les trois sources qu'elle propose ne peuvent pas les montrer :
- NOVA 2006 : déjà masqués ;
- RR Auction : pièces de travail non photographiées ;
- AAA 6/18–6/19 : lus, absents.

Seul l'original, en lumière transmise ou en infrarouge sous l'encre, les montrerait. **Fait nouveau pour la base** : les signes étaient déjà recouverts en 2006. Sanborn (ou quelqu'un d'autre) les a donc cachés avant le tournage.

## Septième partie (25/09, soir) : relais DeepSeek n° 4, « les doublets suivent Fibonacci »

**L'observation.** Cinq des six doublets (18, 25, 32, 46, 67) sont espacés de 7, 7, 14, 21, soit 7 × (1, 1, 2, 3) : des écarts de Fibonacci. Le sixième (42) est écarté.

**Le calcul est exact. Mais la probabilité n'est pas « très faible ».**
- Il y a 14 cases ≡ 4 (mod 7). Parmi les 2 002 façons d'y placer 5 doublets, **15** ont des écarts de Fibonacci : p = 0,75 %.
- On a cherché une loi *après* avoir vu les positions. En admettant n'importe quelle loi simple (Fibonacci, arithmétique, doublement, pas constant), p ≈ **2 %**. Et le doublet 42 a été écarté pour que la loi tienne.
- Trois des cinq doublets (25, 32, 67) sont **dans les cribs**. Ils sont doublets parce que le clair y porte NO, ST, IN. Leur place est donc celle des cribs, et non un choix du chiffreur.
- **Le mécanisme proposé ne produit pas l'effet.** Une clé qui suit une récurrence dans chaque colonne fixe des *valeurs*. Un doublet apparaît là où la différence de clé coïncide avec une différence de bigramme du clair, donc là où le texte le veut. Aucune loi sur la clé ne place les doublets à des rangs de Fibonacci.

**Test exact de la famille proposée** (`fib7.c`). Dans chaque colonne i ≡ r (mod 7), la clé x_n = k[r + 7n] suit une même récurrence :
- ordre 2 : x_n = a·x_{n−1} + b·x_{n−2} + c ;
- ordre 3 : + d·x_{n−3}. Cela couvre k[i] = k[i−7] + k[i−14] − k[i−21] et k[i] = 2k[i−7] − k[i−21], les deux formules du relais.

Chaque colonne a sa graine libre. On teste exactement, par élimination de Gauss dans GF(2) et GF(13), qu'une graine reproduit les 2 à 4 valeurs de clé données par les cribs, puis la même chose en retirant une lettre (une erreur).

| Portée | Contrôle positif | K4 | Témoins K4 mélangé |
|---|---|---|---|
| Ordre 2, **tous** les coefficients (26³), A–Z et KRYPTOS × 5 types × 3 conventions × 2 sens | 20 cas exacts (témoins 0/200) | **0**, et 0 avec une erreur | 0/200 |
| Ordre 3, coefficients dans {0, ±1, ±2}, constante libre | — | **0** sans erreur ; 39 avec une erreur | 90 % des témoins compatibles avec une erreur (p = 0,08) : famille trop lâche à ce niveau |
| Fibonacci pur (a = b = 1), constante libre, 101 086 alphabets à mot-clé × 5 types × 3 conventions × 2 sens | — | **0** | 0/30 |

**Bilan.** La famille « Fibonacci au pas 7 » est **éliminée**, pour A–Z et KRYPTOS avec n'importe quels coefficients d'ordre 2, et pour tous les alphabets à mot-clé dans le cas Fibonacci pur.
- L'ordre 3 est éliminé sans erreur. Avec une erreur admise, il est trop lâche pour trancher.
- La régularité 7, 7, 14, 21 est une coïncidence du niveau de 1 à 2 %, portée pour moitié par l'emplacement des cribs.

## Huitième partie (25/09, nuit) : « cinq angles morts » (relais externe) : vérifiés

| Affirmation | Vérification | Verdict |
|---|---|---|
| **2. Les cribs sont mal placés sur le cuivre** (« ? » en tête de la ligne 25, EASTNORTHEAST coupé par une fin de ligne, BERLINCLOCK entier sur la ligne 27) | K3 compte 336 lettres et commence en tête de la ligne 15 : 10 lignes pleines plus 26 cases de la ligne 25. Le « ? » est en colonne 26, OBKR (0–3) finit la ligne 25, et les 93 lettres suivantes remplissent exactement les lignes 26–28. Donc EASTNORTHEAST = ligne 26, colonnes 17–29 ; BER termine la ligne 27 ; LINCLOCK ouvre la ligne 28 (colonnes 0–7) ; EKCAR termine la ligne 28 (26–30). Confirmé par le plan de Sanborn (« 11 Lines », « 3 lines 93 ») et par la reconstitution NOVA (ligne 11 : « …VDOHW?OBKR ») | **Faux.** Le relais oublie les 26 dernières lettres de K3 sur la ligne 25. La base 4 est juste, et les 8 signes sont bien sous LINCLOCK |
| **1. K4 faisait 105 lettres (15 × 7) ; les 8 signes sont sa fin coupée** | L'arithmétique proposée (342 + 1 + 105 = 448 = 434 + 14) mélange deux choses. Le brouillon « 11 Lines \| 342 » concerne K3 **avant** la coupe de « slowly » : 342 − 6 = 336, et 336 + 1 + 4 = 341 = 11 × 31. Le cadre « 3 lines 93 » montre que la fin de K4 faisait **déjà 93 lettres** quand la mise en page a été planifiée. Une fin de 8 lettres sur une 15ᵉ ligne tomberait bien sous les colonnes 0–7, sous LINCLOCK : c'est la proposition de S. P. (22/06/2026), déjà en base 7 §11.6. Mais le clair contrôlé par Paradigm fait 97 lettres | **Possible pour la mise en page, non établi.** Surtout, cela ne change rien aux familles en i mod 7 : K4 commence au même rang. **Seule T16 dépend de la longueur de la grille** : rejouée sur 105 = 15 × 7 (§ ci-dessous) |
| **3. Refaire l'autoclé à l'écart 7 dans « une autre base »** | La citation porte à faux : Scheidt dit que le **changement de base a été envisagé puis écarté** (base 2 §3, base 6). Une grille de 25 cases (Polybe) est exclue, car K4 contient les 26 lettres. Un tableau quelconque laisse les cribs sans contrainte (invérifiable). Seules arithmétiques fermées sur 26 lettres : Z26 (T27, T34) et la grille 2 × 13 / 13 × 2 (ligne mod 2, colonne mod 13) | **Testé** (`../moteur_2026_09_25/k4coord.c`) : 1,4 M alphabets × 2 grilles × 3 lectures de la clé × 3 conventions, 10 équations directes. Contrôle : 0 fausse (témoins 4–6). **K4 : au mieux 3 fausses sur 10, témoins 3–4 (p = 0,36). Éliminé** |
| **4. Les écarts de clé −1, −1, −5 aux doublets prouvent une clé non périodique, qui suit le clair** | Exact en A–Z (déjà chez Bean, base 2). La période 7 est éliminée pour **tout** alphabet (T22). Qu'une clé qui dépend du clair explique ces doublets, la borne des doublets l'a mesuré (`../procedes_2026_09_25/`) : 20 000 procédés, dont les autoclés, n'en produisent pas la concentration | Connu ; rien de nouveau à tester |
| **5. Le L en plus « not coded » (IMG_1340), cadrans d'orientation, réglettes** | IMG_1340 porte « Extra L at end of line, Bottom chart section » ; le mot devant « coded » concerne les quatre « ? », et il est illisible (relecture KryptosBot, base 5). Rien ne relie ce L à « HILL ». Disques et cadrans tournés : T11/T11b et T15 (Wheatstone à mot-clé, éliminé et robuste) | Lecture de source inexacte ; familles déjà couvertes |

**T16 sur une grille de 105 = 15 × 7** (clé de 1 à 7 lettres écrite en lignes de 7 et relue par colonnes, à la manière de K3, sur les 105 cases d'un K4 d'origine) : contrôle retrouvé (e = 0, témoins 8–9). Résultat sur 1,4 M alphabets (`../moteur_2026_09_25/res_T16_grille105.txt`) : **K4 au mieux 7 erreurs, témoins 6–8 (p = 0,87). Aucun signal** : la grille de 105 ne fait pas mieux que celles de 97 et 98.

## Neuvième partie (25/09, nuit) : relais Gemini, « sept détails fondamentaux »

| Point | Vérification | Verdict |
|---|---|---|
| 1. Aux doublets, le pas de clé compense la pente des bigrammes (−1, −1, −5 en A–Z) | Exact, mais c'est la définition même d'un doublet dans un chiffre additif. Déjà chez Bean (base 2), et au cœur de la borne des doublets (`../procedes_2026_09_25/`). L'« intervention manuelle » est l'une des trois explications retenues en base 9 | Connu |
| 2. 8 signes sous VTTMZFPK, gabarit P/K/C, table des clés | Mise en page exacte (NYP finit la ligne 27, VTTMZFPK ouvre la 28). Table identique à `huit_signes.py` (base 7 §12.2). Mais les signes étaient **déjà des taches opaques en 2006** (§ 6ᵉ partie, base 7 §12.12) | Connu ; illisible |
| 3. 11 lignes / 342, 3 lignes / 93, coupe de « slowly » | Exact : c'est notre base 7 §11.6 (336 + 1 + 4 = 341 = 11 × 31 ; 93 = 3 × 31) | Connu |
| 4. Dossier « Stencil Patterns, circa 1988 » scellé ; IMG_1555 « Code Breaker » sur « Coded » = pochoir physique | Le dossier 6/13 est bien retiré de l'inventaire (base 5). IMG_1555 est lu en amont comme la **métaphore des œuvres *Filter Media*** (plans superposés, projection de lumière), pas comme un pochoir de chiffrement. Si K4 dépend d'un gabarit physique inconnu, les 24 lettres ne peuvent pas le reconstituer : c'est la troisième porte ouverte de la base 9 | Connu ; invérifiable sans le dossier |
| 5. Aiguille à 66°, cadran « offset » d'environ 15° vers l'ouest, correction de cap | « It's offset » (NOVA 2006) : exact. Mais **les 15° n'ont jamais été mesurés** : c'est l'estimation « à l'œil » d'un internaute (~345°, base 1). Un décalage fixe de l'alphabet ou de la clé est absorbé par tous nos tests, qui essaient toutes les valeurs de clé et toutes les rotations (Q3) ; clé et convention de Beaufort comprises | Déjà couvert ; mesure absente |
| 6. « Réfléchissant » = involutif = Beaufort ; masques binaires « autant de 1 que de 0 » | Beaufort est testé dans **toutes** les familles (convention BEAU). XOR 5 bits testé (23/09). Le « changement de base » de 2020 décrit le masque ; celui de 2015 (base mathématique) est **écarté** par Scheidt lui-même (bases 2 et 7) | Connu |
| 7. Lettres entourées D (5, 7), T (8, 24), K (14, 14), H (20, 22) ; lignes +3, +6, +6 ; K au « centre » de la diagonale | Lu dans le tableau KRYPTOS (ligne = lettre-clé, colonne = lettre claire, rangs à partir de 1), chaque marque est une recherche cohérente : **clé T + clair S → D ; clé A + clair W → T ; clé G + clair G → K ; clé N + clair U → H**. Aucune de ces paires (S→D, W→T, G→K, U→H) n'apparaît dans les cribs de K4 ni dans K1–K3. K est sur la diagonale clé = clair (2 × G = 26 ≡ 0), en 14ᵉ position sur 26, donc pas au centre exact. Quatre points donnent trois écarts : « +3, +6, +6 » n'est pas une régularité mesurable. Le retournement éclairé (Munich 2013) a été testé : superpositions cuivre ↔ tableau (éliminées), convention retournée par ligne (T25, aucun signal) | Pas de lien avec K4 |

**Bilan.** Les points 1 à 6 sont déjà dans nos bases ; deux affirmations sont inexactes (15° « mesurés », « base » de 2020 prise pour une base de calcul). Le point 7 est décodé ci-dessus : ce sont quatre recherches ordinaires dans le tableau, sans rapport avec les cribs.
