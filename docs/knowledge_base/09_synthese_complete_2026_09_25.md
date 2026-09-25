# 09 — Synthèse complète au 25/09/2026, après relecture de tout le dépôt

**Ce document remplace la base 8 comme point d'entrée.** Il a été écrit après une relecture intégrale :
- le README, le document de reprise (`HANDOFF_NEXT_AGENT.md`) et la mémoire amont (`MEMORY.md`) ;
- les bases 1 à 8 ;
- les documents de méthode de `docs/` ;
- les rapports de tous les audits (21/09 → 25/09).

Il ne rapporte rien de nouveau par le calcul. Il dit ce qui est établi, ce qui est éliminé et avec quelle solidité, ce qui reste ouvert, et il corrige ce que la relecture a fait apparaître (§7).

**Conventions.**
- Positions comptées **à partir de 0**, sauf mention.
- Niveaux de source : **P** = pièce d'époque ou parole directe ; **S** = témoin qui rapporte (notes de dîner, mémoire) ; **T** = compilation.
- Les *lectures* sont des interprétations, signalées comme telles.
- « Témoins » : chiffrés de contrôle passés dans le même test (K4 mélangé, sauf mention). « Contrôle positif » : faux K4 fabriqué avec le procédé testé, que le test doit retrouver.

**Statut.** Aucune solution, aucun clair, aucune revendication de percée.

**Liste complète des tentatives**, avec leur résultat (réussi, éliminé, sans signal, non concluant, bloqué, non fait, corrigé) : [`10_registre_des_tentatives.md`](10_registre_des_tentatives.md). Ce document-ci en donne la lecture d'ensemble.

---

## 0. L'essentiel en dix points

1. **K4 compte 97 lettres. Le « ? » qui les précède n'est pas chiffré.** Quatre sources indépendantes le disent, et une cinquième est compatible (§1). Les cribs sont aux rangs 21–33 (EASTNORTHEAST) et 63–73 (BERLINCLOCK) **du clair**, ce que l'acheteur du clair a fait vérifier par programme en 2026.
2. **Deux auteurs.** Scheidt a conçu le procédé ; Sanborn l'a « changé » (« I fucked with it », 2025) et a chiffré K4 seul, fin 1989–1990. **Personne n'a jamais redéchiffré K4** : ni Sanborn, ni Scheidt.
3. **Sanborn fait des erreurs de chiffrement, et elles sont systématiques** : 4 sur 97 dans un petit chiffre d'atelier de même longueur, 3 dans K1–K2, 2 types répétés dans sa maquette de 1988. Avec ce taux, les 24 lettres des cribs n'ont qu'environ 31 % de chances d'être toutes justes. **Toute élimination doit dire si elle tient avec une ou deux erreurs.**
4. **Un seul signal statistique résiste aux corrections : le « 7 ».** Cinq des six doublets sont en position ≡ 4 (mod 7), et il y a neuf coïncidences à l'écart 7 pour 3,3 attendues. Ensemble : p ≈ 2 × 10⁻⁴ brut, **10⁻³ à 10⁻² après correction** du choix fait après coup. Très improbable, pas exclu.
5. **Ce « 7 » ne relie que des voisins.** L'écart 7 est en excès ; les écarts 14 à 49 sont au niveau du hasard. Ce n'est donc **pas une période**, quelle que soit la clé.
6. **Aucune clé fixée à l'avance ne rend les doublets alignés probables** (borne du 25/09, §5.2). C'est vrai quelle que soit la famille : période, clé courante, objet extérieur, horloge, matrice, pochoir. Sur environ 20 000 procédés manuels simulés, aucun ne les reproduit.
7. **Les deux moitiés du « 7 » n'ont pas le même statut.**
   - L'excès à l'écart 7 s'explique par un seul procédé : l'autoclé sur le clair à l'écart 7, forme Vigenère (l'hypothèse de la NSA en 1992). Il est éliminé pour les alphabets de Sanborn, même avec ses erreurs.
   - Les doublets alignés ne s'expliquent par aucune clé : c'est un hasard, une intervention faite en regardant le clair, ou une étape qui n'est pas lettre à lettre.
8. **Avec les alphabets que Sanborn employait** (alphabets à mot-clé, en ligne ou en matrice), **toutes les familles de clés faisables à la main testées sont éliminées ou au niveau des témoins**. Cela couvre plus de 3 millions d'alphabets, les deux mots-clés, les erreurs de Sanborn et le déchiffrement complet des 97 lettres.
9. **Les 24 lettres ne suffisent pas à trancher le reste** : alphabets quelconques, clés choisies à la main, étape qui n'est pas lettre à lettre. Il faut une donnée nouvelle.
10. **Ce qui trancherait** :
    - le chiffré de **K5** (outil prêt : `audits/k5_depth/`) ;
    - une photo lisible des **8 signes raturés** sous LINCLOCK sur la feuille de travail ;
    - la **description de la méthode** détenue par l'acheteur.

---

## 1. Le texte : ce qui est établi

| Fait | Sources | Solidité |
|---|---|---|
| Chiffré : `OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR` | cuivre ; transcriptions concordantes | **Établi** |
| **97 lettres, « ? » non chiffré** | (1) note de Sanborn IMG_1340 : « ? or like 4, 8, 10, 25 ‹ ? › coded » : ce sont les lignes des quatre « ? », mais le mot devant « coded » est illisible (« not » probable, non certain ; base 5, 25/09) : indice compatible, pas preuve (P) ; (2) son fichier pour le NYT, 2010 : « ? » non numéroté, O = 1 … R = 97 (P) ; (3) la NSA compte le « ? » dans K3 : 436 + 337 + 97 (P) ; (4) Sanborn, NPR 2005 : « les 97 derniers caractères », donné comme indice (P) ; (5) test de vérification : positions citées jusqu'à 96–97, jamais au-delà (S, 2005 et 2009). Le « 98 » de Scheidt (2005) compte très probablement le « ? » | **Établi** |
| Cribs **EASTNORTHEAST** en 21–33 et **BERLINCLOCK** en 63–73, rangs **du clair** | indices de 2010, 2014, 2020 (P) ; contrôle par programme du clair saisi par Sanborn : 97 lettres, A–Z, cribs à leur rang (Paradigm, 2026, S) | **Établi** |
| Correspondance **lettre à lettre** (la lettre gravée n° i donne la lettre claire n° i) | Sanborn 2011 (« 1-1 ») et 2013 (N = B, Y = E… « déchiffre ») (S) ; CNN 2019 (« un-à-un », cité par Bean) ; statistique de Bean (une même lettre claire donne des chiffrés proches) | **Probable, avec réserve** : Sanborn « recule dès qu'on précise 1:1 » (E. Dunin, 2023) ; Scheidt hésite (2013) |
| Deux **auto-chiffrements** : 32 (S → S) et 73 (K → K) | cribs | Fait. Dans toute famille additive, ils imposent une clé nulle en 32 et 73, et portent une grande part du pouvoir éliminatoire |
| **Égalité de Bean** : même lettre chiffrée P pour R (27) et R (65), écart 38 | cribs | Fait |
| **Aucune transposition pure** n'est possible : les cribs contiennent 3 E, le chiffré de K4 n'en a que 2 | comptage | Fait |
| Avec toute clé additive, les contraintes de Bean ne laissent que **624 suites de clé** possibles aux 24 positions | dépôt d'origine (`MEMORY.md`) | Fait (sous l'hypothèse lettre à lettre) |
| Le K4 d'*Antipodes* (1997) est **identique lettre pour lettre** à celui de Kryptos | transcription de J. Wilson, vérifiée (base 5) | Fait : aucune variante à exploiter |
| K4 compte **8 K** (2, 31, 45, 52, 73, 77, 86, 93) ; seul 73 est un auto-chiffrement | NSA 2014 ; cribs | Fait |
| Mise en page : 4 + 31 + 31 + 31 ; EASTNORTHEAST tient dans la ligne 26 ; BER / LINCLOCK coupé entre les lignes 27 et 28 | cuivre | Fait |
| Le panneau du bas (K3, « ? », K4) fait **exactement 14 × 31 = 434** caractères ; la feuille de travail NOVA porte K3 + « ? » + K4 sur 31 colonnes et 14 lignes | transcription ; reconstitution « All Charts » (S) | Fait (cuivre) ; probable (feuille) |
| **8 signes raturés** sous VTTMZFPK (66–73, soit LINCLOCK), **déjà masqués au tournage de NOVA (2006)**, sous correcteur sur les photos de 2026 | images NOVA du dépôt ; reconstitution « All Charts » ; message d'un membre (2026) | Fait ; nature inconnue |
| **K5** : 97 caractères, « codage semblable », BERLINCLOCK au même rang que dans K4 | lettre de Sanborn, 12/11/2025 (P, transcription) | Déclaration |

### 1.1 L'œuvre, le site et la fabrication
- **La rose des vents.** L'aiguille gravée pointe vers la magnétite. Mesurée sur deux photos indépendantes (CIA ; Gillogly, 1999), elle est orientée à 246–247° / 66–67° dans le repère de la rose : **axe OSO–ENE**. Le NE est exclu. C'est la direction du premier crib (EASTNORTHEAST), gravée trente ans avant l'indice de 2020.
  - Le cadran lui-même n'est pas orienté nord-sud : « it's offset » (Sanborn, NOVA 2006). La NSA notait en 1991 « 230°–240° ». L'orientation par rapport au vrai nord n'a jamais été mesurée.
  - Trois études de rose dessinées par Sanborn (IMG_1518, papiers AAA, photo de KryptosBot ; mesure `measurements/rose_studies_img1518.py`) montrent l'aiguille à 63°, 55° et 66°, entre NE et ENE. Deux sont à 1 à 4° de l'aiguille gravée. Si ces études sont celles de Kryptos, l'orientation ENE est un choix dessiné (base 5, 25/09).
  - L'aiguille et la magnétite sont un motif ancien de Sanborn (sculptures de 1980), pas un indice créé pour K4. Elles **ne fixent aucun paramètre** de chiffrement.
- **Le Morse (K0)** est réparti sur **deux dalles**. La dalle de la rose porte « T IS YOUR / POSITION » (contre la magnétite) et « DIGETAL INTERPRETATI ». Le U de YOUR est bien un U : la transcription de Rumkin (G) était fausse. L'ordre de lecture « de l'entrée à la cour » qui circule n'est qu'un ordre de liste. Les plaques de l'entrée **ont bougé** (constat de la CIA, 2008).
- **« Deux systèmes, un indice majeur »** (inauguration, 5/11/1990) porte sur **la plaque du bas, K3 + K4**, pas sur K4 seul. Sanborn : « I used that table to encipher **the top plate** ». Lettre de 1989 : le texte se déchiffre « en partie avec le tableau, en partie avec un système potentiellement redoutable ».
- **Chronologie de fabrication** (manuscrit de livre de Sanborn, vers 2009, AAA 6/9) :
  - le tableau a été découpé **avant** que le clair existe ;
  - le clair était encore « sans cesse révisé » à la mi-1989 ;
  - K3 et K4 ont été découpés par **une seule personne**, « pratiquement sans erreur ». Le risque porte donc sur la feuille de chiffrement, pas sur la découpe ;
  - Webster n'a reçu qu'une clé **partielle** ;
  - les photos officielles de 1990 cachent **volontairement** une partie du texte.
- **Les « textes brouillés » de 2025** sont le clair découpé par phrases et remélangé pour la CIA en 1989. Ce n'est pas une étape du chiffrement.
- **Le lot vendu en 2025** comprend le clair manuscrit, « le système de codage original de K4 », les clairs de découpe, les chartes de K1–K3, un K1 et un « K4 alternatif » de 1988 (devenu K5), et les textes brouillés. Aucune photo des pièces 1 à 7 n'a été publiée.
- **Exclu faute de source** : « THE COMPASS ROSE IS HERE », qui vient du clair reconstruit de SolveKryptos, et « la clé est KOMITET » (message anonyme).

---

## 2. Ce que disent les auteurs

### 2.1 Qui a fait quoi
- **Scheidt a conçu le procédé** ; Sanborn l'a **changé** (appel à E. Dunin, 14/08/2025, S) : « It is a solid system, and **I fucked with it**. » — « I can't say. » (*Financial Times*, 2025, P cité).
- Scheidt a fait tourner **son** procédé « une fois, deux fois » ; « Jim prétend avoir fait autre chose par-dessus » (2015, P).
- **K4 n'a jamais été relu** :
  - Sanborn en 2003 : « personne n'a testé le code, pas même Scheidt » (S) ;
  - Scheidt en 2006 : « je n'ai pas relu le code lettre à lettre » (S) ;
  - Scheidt en 2015 : il n'a jamais déchiffré les 97 lettres gravées (P).
- Chronologie : clair définitif postérieur au 9/11/1989 ; K4 chiffré par Sanborn seul, fin 1989–1990. Une première version de 1988 (« K4 alternatif », avec sa propre charte) est devenue K5.

### 2.2 Le procédé
- **Plus d'une étape** (Scheidt 2015) ; « cinq ou six » techniques pour tout Kryptos (Sanborn 2005). Morse, Vigenère et transposition en font trois : il en reste deux ou trois pour K4.
- **Côté Scheidt, un « masque »** :
  - il « retire le biais » (2007) et l'avantage de l'anglais (2004) ;
  - il « change la base de langue, pas vers une autre langue, vers autre chose » (2020) ;
  - « un peu de stéganographie » (2004) ; une « fonction régénérative » (2011) ; un code « réfléchissant », pas « symétrique » (2007) ;
  - un résultat « comme un masque jetable », mais obtenu autrement (2020).
  - **Ordre non dit.** La lecture « le masque est la première étape » (réunion de 2015) était fautive : la « première étape » est celle de l'analyste, qui compte les lettres. Scheidt refuse de répondre sur l'ordre (« vous vous approchez… je diffère »).
  - Ce qui a été **refusé** : changer la base **mathématique** (base 2, base 16) et les langues rares.
- **Côté Sanborn, des matrices qu'on tourne** :
  - K1–K3 : des matrices « qu'il fallait tourner » ; K4 : « tourner et retourner », « à la machine à laver » (2019, S) ;
  - Big Techday 2013 (P), en montrant ses feuilles : « à peu près la façon dont je l'ai fait » ; on **retourne la feuille, on la met à l'envers**, on l'**éclaire** ;
  - NPR 1999 (P) : des systèmes « **spatiaux** », à motifs, de lumière et d'ombre, absents de K1–K3, « peut-être » dans K4 ;
  - 2003 (S) : c'est **Sanborn**, et non Scheidt, qui s'étonne que personne n'ait retrouvé « la matrice d'origine » ni ne l'ait passée par « tous les shifts ».
- **La presse de 1990–1991** (AAA 16/2 et 6/18) :
  - l'autre moitié du texte est codée « dans un **système moderne créé pour le projet** par un cryptographe expert » (*Washington Post*, 14/01/1990) ;
  - « trois ou quatre » systèmes, de complexité croissante (*Museum & Arts*, 1990) ;
  - AP (mars 1991) : le « quadrant inférieur droit » (K4) est « un tout autre jeu, **de codes multiples** » ; certaines parties « pourraient ne **jamais** être déchiffrées sans ce que sait Webster » ; même déchiffré, « le message n'aura pas de sens » ; deux enveloppes, la traduction et « **les mots-clés** nécessaires pour casser le code ».
- **Autres paroles sur la méthode** :
  - Scheidt 2003 (S) : les clés sont « **dissimulées sur la sculpture** », retrouvables grâce aux indices présents ; « la clé n'est pas forcément le mot-clé… **la clé est l'algorithme** ».
  - Scheidt 2005 : il faut « **d'abord trouver la technique** » ; quatre procédés en tout, « deux semblables, deux différents ».
  - Scheidt 2015 : « Jim a dit que **quelque chose qui était clé n'est plus là** » ; des mots à double sens (« coca-cola ») ; « K4 n'est peut-être pas quelque chose de ce genre, c'est en ça qu'il est unique ».
  - Sanborn 2006 : « si je faisais des erreurs, **c'était une bonne chose** », cela rendait le déchiffrement plus dur.
  - Sanborn 2025 : « **Qui dit que c'est même une solution mathématique ?** »
  - Sanborn 2005 (WSJ) : « la clé la plus évidente de la sculpture, personne ne l'a remarquée ».
- **La pratique personnelle de Sanborn** juste après Kryptos (feuilles russes de 1990–1991, AAA 6/8) : une grille à trois lignes clair / clé / chiffré, comme la feuille de K1–K2 ; un **Quagmire II** (alphabet ТЕНЬ, clé МЕДУЗА), retrouvé sur 22 lettres sur 23 ; des coquilles de clé sur la feuille même. Aucune transposition, aucun autre procédé.
- **Contraintes de pratique, toutes concordantes** : crayon et papier ; « résoluble par un humain », « un procédé très long n'est probablement pas le bon » (Scheidt 2011) ; « cryptographie classique, pas quelque chose de bizarre », « facile à utiliser » (2006) ; mémorisable ; deux enveloppes remises à Webster, le message et « les mots-clés » (1990).
- Sanborn n'a « aucune idée de ce qu'est l'ASCII ou le XOR » (2022, S). Le binaire de son œuvre de Martinsburg (1999) est de l'ASCII 7 bits ordinaire, sans chiffrement (audit `martinsburg_2026_09_25`).
- La méthode existe **par écrit** : Paradigm détient une description, « aussi appelée la clé » (Sanborn 2025).

### 2.3 Le clair
- **Anglais**, écrit par Sanborn seul, postérieur à la chute du Mur.
- **Plusieurs phrases**, probablement séparées par des **X** : « toutes mes sections de base sont séparées par des X » (Sanborn 2006, S) ; « plus d'une phrase » (Scheidt 2015).
- Thèmes : une direction (ENE), une **horloge publique** de Berlin (Weltzeituhr), l'Égypte en 1986, le Mur en 1989, « délivrer un message » ; une énigme qui mène « dans plusieurs directions ».
- **Longueurs réglées en coupant le clair** : les brouillons de K3 (photo de la vente) montrent le plan « 11 Lines | 342 » et « 3 lines 93 ». Sanborn coupe « slowly » (6 lettres) pour que K3, le « ? » et K4 tiennent en 14 × 31. Le petit fragment de 97 lettres a un clair **coupé au milieu d'un mot**. Un clair candidat peut donc omettre un petit mot, ou finir au milieu d'un mot.
- K4 se résout sans K1–K3 ; K5 ne se résout pas sans K4.
- Dans un carnet qui contient aussi des croquis du site de Kryptos, Sanborn écrit sous le titre « copper "Veil" » des idées de clair pour une pièce de cuivre codée : des phrases courtes et impératives, une fuite par bribes (« Grab the rope… Tunnel… Run… Hold your breath… Catch your breath » ; IMG_1580–1582). Date et œuvre inconnues. *Lecture* : un clair décousu, fait de bribes, comme le dit Scheidt en 2015 (base 5, 25/09).

### 2.4 Ce que pensait la NSA
- 1992 : « légère propriété à l'intervalle 7 » ; hypothèses : autoclé sur le clair, ou alphabets suivis d'une transposition ; « KRYPTOS joue très probablement un rôle intégral ».
- 1998 : « étant donné la cryptographie soupçonnée », 97 lettres sont trop courtes pour un effort raisonnable.
- 1999 : la section « III. The Fourth Breakthrough? » est **caviardée** pour secret de défense, encore en 2023.

### 2.5 Les contradictions apparentes, et comment elles se résolvent
- « Base changée, refusée » (2015) contre « le masque change la base de langue » (2020) : le refus porte sur la base **mathématique** ; le masque, sur la **langue**.
- « Scheidt n'a jamais déchiffré K4 » contre « une fois, deux fois » : il a fait tourner son procédé, pas les 97 lettres gravées.
- Fautes voulues (2013) contre fautes accidentelles (2019–2020) : les deux existent. Le dépôt au Copyright Office (2010) montre que le Q de K1 et le U en trop de K2 ne sont **pas** dans le clair de Sanborn ; DESPARATLY y est.
- « 1:1 » (2011, 2013, 2019) contre le recul de Sanborn quand on précise (2023) : non résolu. C'est l'une des trois portes que les 24 lettres laissent ouvertes (§6).
- Qui a exécuté K4 : « encoded for Sanborn » par l'ancien de la CIA (*Museum & Arts*, 1990) contre « avec le système en main, Sanborn s'est mis à chiffrer » (*Washington Post*, 1999). Tranché pour Sanborn par Scheidt lui-même (1991 : « voilà comment on fait, et c'était tout » ; 2015 : il n'a jamais déchiffré K4).
- Clé « complète » remise à Webster (« this is the key and the actual text », 1990) contre clé « partielle » (manuscrit, vers 2009 ; « il n'a pas eu toute la banane », 2006) : Sanborn a lui-même présenté son récit de 1990 comme incomplet (« need to know »).

---

## 3. Les erreurs de Sanborn : taux et types

| Pièce | Erreurs | Types |
|---|---|---|
| Petit fragment *Covert Operations* (97 lettres, méthode de K1, clé SHADOW) | **4 sur 97** (positions 9, 22, 87, 91) | 3 du même écart : **glissement de clé** d'un cran (9, 87) et lettre claire **recopiée sans chiffrement** (22) ; p ≈ 0,006 au hasard pour trois écarts égaux |
| K1–K2 (≈ 430 lettres) | 3 | Q d'IQLUSION (clé) ; U d'UNDERGRUUND (gravure) ; X retiré à la fin de K2, pour la mise en page (volontaire, effets non prévus) |
| Maquette GIRASOL de 1988 | plusieurs | une **lettre omise** (T de « THE KEY »), qui fait glisser la clé ; **deux fois la même erreur de table** |
| Martinsburg (1999) | 1 bit | U au lieu de W |
| *Cyrillic Projector* | plusieurs | attribuées à un assistant, qui a ensuite **corrigé le clair** pour coller au chiffré fautif (2003, S) |

**Conséquences.**
- Avec 4 erreurs sur 97 placées au hasard, la probabilité qu'aucune ne touche les 24 lettres des cribs est d'environ **0,31**.
- Le modèle d'erreur est **étroit** : glissement de clé, copie du clair, mauvaise ligne de table. Seules 32 et 73 peuvent être des copies (chiffré = clair).
- **Règle** : toute élimination exacte indique sa tenue avec une erreur quelconque et avec une erreur « à la Sanborn ». Elle compte les erreurs par **équation**, avec ré-ancrage aux cribs (défaut corrigé le 25/09 : une même erreur d'autoclé était comptée plusieurs fois).
- **Avant l'unique soumission** au vérificateur de Paradigm : corriger en anglais les lettres manifestement fautives, sans ajuster la méthode.

---

## 4. Les signaux statistiques, triés

| Signal | Brut | Corrigé | Indépendant ? | Statut |
|---|---|---|---|---|
| **Doublets** BB 18, QQ 25, SS 32, ZZ 46, TT 67 **≡ 4 (mod 7)** ; SS 42 fait exception | p ≈ 0,0006 (module 7) | p ≈ 0,02 (modules 3–16) | oui | **Solide**. Connu depuis 2002–2003 (Stehle, Gillogly ; chiffré par Matson en 2009) |
| **Coïncidences à l'écart 7** : c[i] = c[i+7] neuf fois (3,3 attendues) | p ≈ 0,005 | p ≈ 0,06 (écarts 2–24) | oui (corrélation < 0,02 avec les doublets) | **Solide**. Probablement la « propriété 7 » de la NSA (1992) |
| **Les deux ensemble**, module libre de 2 à 24 | **p ≈ 2 × 10⁻⁴** (198 sur un million) | **10⁻³ à 10⁻²** en payant le choix après coup | — | Le seul signal fort |
| **Profil** : écarts 14, 21, 28, 35, 42, 49 = 2, 3, 4, 1, 3, 0 coïncidences | au hasard | — | — | **Pas une période** : le « 7 » relie des voisins |
| **Géométrie en rangs de 7** : relation « case du dessus » en excès sur toutes les colonnes ; doublets tous dans la même paire de colonnes ; **aucune diagonale** (2 et 3 contre 2,8) ; 3 amas de lettres identiques contre 0,5 (p = 0,011) | — | — | même information que les deux lignes précédentes | Trace **orthogonale**, invisible sur le cuivre (31 de large). Voir la réserve du §7, n° 1 |
| Largeur 21 : 11 bigrammes verticaux répétés | p = 0,0003 | **p ≈ 0,09** (largeurs 2–48) | **non** : 3 des 11 paires viennent des doublets et de l'écart 7 | Réel mais redondant. Le « 1 sur 6 750 » de l'ancien dépôt est dépassé |
| Lettres de KRYPTOS presque en place (9 sur 10 à ±2) | p ≈ 0,0004 | 0,3 % à 5,6 % selon le seuil | — | Suggestif. Le « 1 sur 5 520 » de l'ancien dépôt est dépassé |
| Pas de transposition : une même lettre claire donne des chiffrés proches (distance moyenne 3,6) | — | — | — | Appui statistique au lettre à lettre (Bean) |
| Clé Beaufort A–Z localement répétée : JLJODEGKUKKKL \| OCGGBGOKTRU | p ≈ 0,001 | **p ≈ 0,004** (4 conventions) | — | Curiosité, statistique choisie après coup. Le triplé KKK seul est banal (10,7 %) |
| Propriété U de Caveney : les 3 digrammes « à une lettre d'écart » répétés contiennent tous U, et les 6 U y participent | p ≈ 1,8 × 10⁻⁴ | 6,5 × 10⁻⁴ (écarts 2–4) | — | Curiosité non expliquée ; coût réel du choix après coup inconnu (×10 à ×100) |
| KZ, TJ, DI « comprimés » (pas de 5 puis de 3) | p ≈ 1,4 × 10⁻³ | — | — | Curiosité, après coup |
| Stehle : c[i+4] − c[i] = 5 cinq fois en 55–63 | p ≈ 0,0002 | p ≈ 0,0065 (tous écarts et différences) ; ≈ 1/205 selon la remesure amont d'août 2026 (écarts 1–30) | — | Curiosité locale |
| Miroir des cribs autour de 48 (11 paires) | fait géométrique | — | — | La pliure complète est éliminée (T5) |
| Regroupement des lettres (Improvidus) | p ≈ 0,018 corrigé | 0,06–0,17 sans les doublets | non | **Expliqué** par les doublets |
| IC des positions 0–20 (0,0667, « anglais ») | 2,4 % | 42 % des K4 mélangés ont une fenêtre aussi forte | — | **Non significatif** |

**Contrôle.** Ni K1, ni K2, ni K3 n'ont de « 7 » : leurs excès sont à leurs propres périodes (20 pour K1 ; 8, 16, 40 pour K2 ; 39, 20 pour K3). Le « 7 » est propre à K4.

**Toute solution devra expliquer**, ou rendre fortuits : le « 7 » (écart 7 et doublets), puis les curiosités (U, KZ-TJ-DI, clé Beaufort répétée).

---

## 5. Ce qui est éliminé, et avec quelle solidité

### 5.1 Tableau par solidité

**0. Héritage du dépôt d'origine** (repris de `MEMORY.md` et du journal d'antériorité, 1 044 entrées ; non rejoué ici sauf mention)
- Transposition pure (comptage des lettres) ; Hill 2 × 2, fractionnements, Playfair, César, affine, Atbash ; clés périodiques en A–Z et KRYPTOS ; autoclés en A–Z et KRYPTOS.
- Chaocipher (142 000 paires, bruit) ; VIC, Ubchi, chiffre soviétique en trois étapes ; clé interrompue ; Wichmann-Hill ; récurrences linéaires et affines d'ordre 1 à 8.
- Grilles et routes jusqu'à 20 × 20 ; Myszkowski, AMSCO, Nihiliste ; grilles tournantes, Cardan, Fleissner ; carré latin, bandes.
- TABP (transposition puis substitution périodique, trois versions) ; cadre de composition (105 000 branches, max 6/24) ; campagne cartésienne à deux couches (206 448 profils).
- Perturbation du chiffré, étape A : 10 465 764 réglages, une lettre de K4 changée, 0 passage (l'étape B n'a pas été faite).
- Palette {B,G,I,K,O,W,Z} : artefact, ligne retirée.

**A. Éliminé pour tout alphabet (26! couverts), et robuste aux erreurs**
- Clé **périodique**, p = 1 à 26, Quagmire I à IV : K4 n'est jamais plus proche que le hasard, quel que soit le nombre d'erreurs admis (sauf p = 19, égalité de Bean) ; à p = 7, il l'est **moins** (T22, CP-SAT).
- **Autoclé sur le chiffré** : 3 à 5 erreurs requises aux écarts 1–23 (T23), alors que le taux de Sanborn en prévoit environ une.
- **Clé courante tirée des clairs de K1–K3** (SAT, tout alphabet, tout décalage) : impossible, même avec une erreur tolérée (`one_slip`).

**B. Éliminé avec les alphabets de Sanborn**
Portée : alphabets à mot-clé en ligne (237 988, puis 684 730 avec le dictionnaire élargi), en matrice (1,3 à 1,4 million), paires de mots-clés (2,96 millions) ; témoins K4 mélangé ; contrôles positifs ; déchiffrement complet quand la clé est déterminée.
- **Autoclé sur le clair, écart 7** (hypothèse NSA 1992), **y compris avec deux alphabets à mot-clé indépendants** (T34, 5,3 × 10¹¹ paires) : au moins 7 équations fausses sur 17, contre 5 à 7 pour les témoins ; environ 2 attendues avec le taux d'erreur de Sanborn. Aux autres écarts : au niveau des témoins (T27).
- **Clé courante tirée d'un texte anglais inconnu** : les deux fragments de clé devraient être de l'anglais ; puissance 99,5 % sans erreur, 84 % avec une (un alphabet) ; ≈ 80 % avec deux alphabets.
- Autoclé sur le chiffré à **tous** les écarts (déchiffrement complet).
- Clés à pas 7 : période 7, décalage par ligne du cuivre ou par ligne de 7 (au moins 7 erreurs requises).
- Disque tourné par bloc de 5 à 14 (au moins 7 erreurs), de temps en temps (T29), clé périodique avec saut de phase (T30–T31).
- Clé périodique affectée d'**au plus 3 erreurs « à la Sanborn »**, p ≤ 14 (modèle validé sur le vrai fragment).
- Clé progressive (T32) ; toute clé numérique 0–9 : Gronsfeld, Gromark quelle que soit l'amorce, dates, coordonnées (T33) ; LFSR d'ordre 1 à 5 ; « Fibonacci au pas 7 » ; ligne + colonne ; deux mots superposés ; autoclé « vers l'avant » ; Quagmire I, II, IV jusqu'à p = 26 ; transposition puis substitution (hypothèse NSA n° 2).
- Clé courante en allemand, en français ou en latin.
- **Wheatstone** à cadran extérieur à mot-clé, cadran intérieur quelconque (T15, 1,4 million de cadrans ; robuste à une erreur, T21).
- Fautes de recopie depuis une feuille de 7 colonnes, avec une clé simple dessous (T35).

**C. Éliminé seulement « sans erreur », avec un alphabet quelconque**
Avec une seule lettre mal chiffrée, ces tests tombent au niveau du hasard (T19–T21) :
- T11 et T11b (disque tourné par bloc) ;
- T13 (Hill par blocs à petits coefficients) ;
- T18 (autoclé sur le clair à l'écart 7, chaînes entre cribs) ;
- T36 : la même autoclé calculée dans « autre chose » que Z/26, c'est-à-dire 23 groupes d'ordre 26 à 36 (coordonnées de grille 5 × 6 et 6 × 6, trifide, XOR sur 5 bits) avec un même codage partout. Au moins une équation fausse dans les 67 cas, mais 83 % des témoins font aussi bien avec une erreur. Avec exactement 26 symboles, un autre groupe n'existe pas (tout groupe abélien d'ordre 26 est Z/26) ; au-delà de 28, l'anglais ne reste presque jamais dans les 26 lettres ;
- toute substitution qui ne dépend que de i mod 7, même par ligne de 21 : en 65 et 72, P chiffre R puis C. L'argument repose sur ces deux lettres. La clé de période 7, elle, reste éliminée avec erreurs (A).

T16 (clé transposée à la manière de K3) perd son zéro strict, mais reste sous le hasard. L'autoclé sur le chiffré à l'écart 7 était aussi éliminée sans erreur par deux lettres (A en 22 et C en 72 devraient valoir 0) ; elle l'est désormais avec erreurs (A).

**D. Sources de clé fixées, toutes négatives**
- Textes de la sculpture lus en 13 parcours et en 2D sur cylindre (T1, 57 617 placements) ; tableau GIRASOL de 1988 (T10) ; mots visibles sur l'œuvre ; groupes de E du Morse ; miroir du Morse.
- Superpositions physiques : écran replié ou en miroir, plaque du haut posée sur la plaque du bas, lettre du tableau au dos comme clé ou comme sélecteur, batterie de 40 géométries × 15 fonctions ; réglettes M-138 découpées dans le tableau.
- Gromark amorcé par des dates ; Gromark avec un alphabet de l'œuvre ; clé par paliers.
- Carter (livre et journal de fouilles), inscriptions du hall de la CIA et de Langley, directive Truman de la boîte de cuivre, cahier des charges de 1988, lettre de Sanborn de 1989 (T6, T7) ; suite de 1/563 (T8) ; chiffré de K3 comme clé ; feuille de K3 empilée sur K4.

**E. Structures éliminées sous l'hypothèse lettre à lettre**
- Route de K3 (multiplicative, 102 permutations) + clé périodique courte ; grilles 98 = 14 × 7 avec le « ? » (sans appui documentaire depuis IMG_1340) ; serpentins.
- Fractionnements (bifide, trifide, dont le trifide décalé sur la mise en page, T28) ; transposition de bits (forme de DeSeve fermée par les cribs ; la transposition de tous les bits reste peu plausible) ; XOR 5 bits ; M-94.
- Pliure complète (T5, F1).

**F. « Solutions » publiées.** Aucune n'est dérivée de façon reproductible. Sur 81 clairs complets trouvés dans les dossiers des membres, et 48 803 déchiffrements confrontés aux cribs, rien n'approchait les cribs avant leur publication. La plus élaborée (2025) est un masque jetable : chaque position a sa propre clé.

**G. Indécidable avec 24 lettres.** Le hasard y est compatible dans la majorité des cas : Quagmire IV et clés composées longues avec alphabets quelconques, demi-pliures, Gromark à deux alphabets, ligne + colonne avec alphabet quelconque, clé courante avec alphabet quelconque, sélecteur libre ; autoclé sur le clair à l'écart 7 avec **deux** codages libres, un pour le clair et un pour le chiffré (T36b : K4 compatible sans erreur, comme 55 à 95 % des témoins).

### 5.2 La borne des doublets (25/09) : pourquoi aucune famille de clés ne suffit
- Dans un chiffre qu'on peut déchiffrer lettre à lettre, à état donné, **une seule** lettre claire produit c[i+1] = c[i]. Un doublet revient donc à « prédire » la lettre claire suivante.
- Sur de l'anglais, avec une clé **indépendante du texte** (n'importe laquelle, fixée d'avance), le taux maximal de doublets est de 6,3 à 6,7 % en A–Z ou KRYPTOS, 13,6 % avec le meilleur des 101 086 alphabets à mot-clé, 17 % avec des alphabets quelconques.
- Pour 5 doublets sur les 14 cases d'une colonne, avec les trois des cribs, cela donne : **0** en A–Z ou KRYPTOS ; 0,9 % avec le meilleur mot-clé ; **6 % au plus** avec des alphabets libres.
- Seule une clé qui dépend du clair précédent **et** qui est accordée aux successions de l'anglais (I → N, S → T, N → O) atteint 17 à 36 %. C'est invraisemblable pour un chiffre manuel.
- **Conséquence** : aucune famille de clés, testée ou à venir, ne rend probables les doublets alignés. La question « quelle clé ? » ne peut pas les expliquer.

---

## 6. Où en est la question du « 7 »

**Ce qui est acquis.**
- Le « 7 » est dans les **positions du chiffré** (écart 7, doublets ≡ 4 mod 7), pas dans une période de clé.
- Il est propre à K4 (K1–K3 n'en ont pas).
- L'excès à l'écart 7 et les doublets sont deux faits **indépendants** sous le hasard.

**Les deux moitiés du signal.**
1. **L'excès à l'écart 7.** Sur 52 procédés manuels simulés (un million de faux K4 chacun), un seul le rend naturel : l'autoclé sur le clair à l'écart 7, **forme Vigenère**, lettre-clé lue dans l'alphabet du clair (4,9 % des tirages, contre 0,29 % au hasard et 0,48 % pour une période 7). En Beaufort ou en Variante, l'excès disparaît : c[i] = c[i+7] équivaut à p[i−7] = p[i+7] seulement en Vigenère.
   - Éliminée pour les alphabets de Sanborn, même avec ses erreurs (T27, T34).
   - Éliminée pour **tout** alphabet s'il n'y a **aucune** erreur (T18). Avec une erreur, les 24 lettres n'ont plus aucune puissance (78 à 97 % des témoins passent).
   - **Seul coin non fermé** : cette autoclé avec un alphabet qui n'est pas construit sur un mot-clé, et au moins une erreur, **ou** avec deux alphabets libres (clair et chiffré), même sans erreur (T36b). Les recherches libres sur 97 lettres ne tranchent pas (audit « recuit »).
   - Changer de « base » (coordonnées, chiffres, XOR, groupes d'ordre 27 à 36) ne rouvre rien : même constat que dans Z/26 (T36, `audits/autocle_groupes_2026_09_25/`).
2. **Les doublets alignés.** Aucune clé ne les explique (borne, §5.2). Aucun des ≈ 20 000 procédés déchiffrables simulés ne les reproduit (au plus 11 % de doublets en colonne 4, contre 36 % pour K4 ; signature jointe ≤ 2 × 10⁻⁶). L'autoclé du point 1 ne les concentre pas non plus : dans le simulateur, la signature complète reste ≤ 10⁻⁵ pour tout procédé positionnel. Seules les transpositions finales concentrent les doublets (1–2 %), et l'alignement des cribs les exclut.

**Il reste trois explications.** Les 24 lettres ne permettent pas de choisir entre elles :
- **le hasard**, à 10⁻³–10⁻² après correction ;
- **une intervention faite en regardant le clair**, par exemple des lettres de clé choisies pour faire un motif. *Lecture* : c'est compatible avec « I fucked with it » et avec le goût de Sanborn pour les « codes qui dépendent entièrement de motifs » ;
- **une étape qui n'est pas lettre à lettre**. *Lecture* : c'est compatible avec le recul de Sanborn sur « 1:1 » et avec le « masque » de Scheidt.

---

## 7. Ce que la relecture corrige ou nuance

1. **Le rectangle 14 × 7 « avec le ? » est une hypothèse de mise en page, pas un fait.** Le « ? » n'est pas chiffré (§1), et la seule feuille connue fait 31 colonnes. Ce qui ne dépend pas du « ? » :
   - l'excès à l'écart 7 ;
   - l'absence de diagonales ;
   - le fait que les doublets tombent tous dans **une même paire de colonnes** de 7 (colonnes 4–5 en comptant depuis O).

   Seule l'affirmation « les doublets sont **en bout de ligne** » exige le décalage d'un rang que donne le « ? ». Sans lui, les doublets sont en colonnes 4–5. La base 7 (§12.15), la base 4 et la base 8 sont nuancées en ce sens. Le rapprochement avec les lettres doublées de fin de ligne du fragment de Zola ne tient donc que sous cette hypothèse.
2. **La base 8 (§5, n° 3) proposait comme candidat une clé lue dans une matrice de 7 colonnes tournée ou retournée.** La borne des doublets (§5.2) l'écarte comme explication des doublets : une telle clé ne regarde pas le clair. Elle pourrait encore expliquer l'excès à l'écart 7, mais seule l'autoclé Vigenère le produit en simulation.
3. **Le dilemme du 22/09** (`docs/k4_mechanism_reasoning_2026_09_22.md` §3) opposait une clé tirée d'un objet extérieur à une étape qui n'est pas lettre à lettre. Si les doublets ne sont pas un hasard, la borne élimine la première branche **comme explication des doublets**. Restent l'étape qui n'est pas lettre à lettre, ou une clé qui dépend du clair ou d'un choix fait en le regardant.
4. **Chiffres de l'ancien dépôt dépassés** (`docs/procedural_anomaly_recipes.md` §4, `docs/invariants.md` §7) :
   - largeur 21 « 1 sur 6 750, STRONG » : p ≈ 0,09 après correction, et non indépendante ;
   - lettres de KRYPTOS « 1 sur 5 520 » : 0,3 à 5,6 % contre des ensembles de 7 lettres au hasard ;
   - IC des positions 0–20 « peut-être un autre chiffre » : non significatif.
5. **L'argument « 98 = 14 × 7, le ? est une prédiction gratuite »** (`docs/k4_mechanism_reasoning_2026_09_22.md` §4, n° 2) n'a plus d'appui documentaire depuis IMG_1340. Ses tests (grilles de 7 avec le « ? ») restent valables comme éliminations.
6. **« K5 ⇒ chiffre positionnel »** (`docs/invariants.md` §8) reste une hypothèse. Elle devient **testable dès la publication de K5** : si K5 porte NYPVTTMZFPK en 63–73, la clé ne dépend que de la position ; sinon, elle dépend aussi d'une charte, du texte ou d'une autoclé.
7. **« Aucune expérience admissible »** (`docs/current_experimental_frontier.md`, 21/09) décrit l'état du 21/09. Depuis, l'utilisateur a demandé des tests. Plus de 35 ont été faits, chacun avec une portée écrite d'avance, des témoins et un contrôle positif ; leurs négatifs sont conservés avec leur portée.
8. **Phrases périmées corrigées pendant cette relecture** :
   - base 2 (mise à jour du 24/09) : « le masque est la première étape » ;
   - audit « vision » (§1 et §4) : « changer la base » présenté comme refusé sans nuance ; « le masque d'abord » ; Martinsburg « non décodé » (c'est de l'ASCII 7 bits) ;
   - base 8 (§7) : phrase dupliquée ;
   - README : puces mélangées par une fusion.

---

## 8. Leçons de méthode

1. **Témoins mélangés.** Pour tout test qui note un texte, des témoins tirés uniformément sont trop bas. Seuls les témoins « K4 mélangé », qui gardent ses fréquences, sont justes. Avec eux, plusieurs « signaux » de clé courante sont devenus banals.
2. **Contrôle positif d'abord.** Un outil qui ne retrouve pas un faux K4 fabriqué avec le procédé testé ne conclut rien. L'attaque par recuit libre a échoué à son contrôle : aucun essai n'a été fait sur K4 avec elle.
3. **La puissance vient de la restriction.** Avec un alphabet quelconque, 24 lettres ne tranchent presque rien. Avec les alphabets que Sanborn employait, les mêmes familles deviennent décidables. Les recherches libres (alphabet quelconque, mot-clé quelconque) fabriquent de l'anglais apparent sur n'importe quel chiffré.
4. **Compter les erreurs juste** : par équation, avec ré-ancrage aux cribs ; indiquer la tenue à une erreur ; signaler quand une élimination repose sur 32 ou 73.
5. **Payer le choix après coup.** Une statistique choisie en regardant K4 se corrige du nombre de statistiques possibles (facteur 10 à 100). Plusieurs anomalies célèbres n'y résistent pas.
6. **Ne pas compter deux fois.** La largeur 21 recoupait les doublets et l'écart 7 ; le premier chiffre conjoint (10⁻⁵) était faux.
7. **Distinguer ce qui est calculé de ce qui est lu.** Les « 8 lettres de clé » des positions 66–73 sont calculées à partir du crib ; elles n'apportent aucune contrainte tant que personne n'a lu les signes.
8. **Antériorité.** Les doublets mod 7 sont connus depuis 2002–2003. Chercher dans l'archive du groupe avant de présenter une piste comme nouvelle.
9. **Relais d'IA.** Les relais Gemini (23–24/09) et DeepSeek (25/09) reformulaient surtout la base, avec des erreurs de compte : 8 paires à l'écart 7 au lieu de 9 ; « 8 dernières lettres » pour les premières de la dernière ligne ; lettres de clé calculées depuis le crib prises pour une lecture. Chaque point a été vérifié. Les tests qui en sont sortis (audits du 23/09, T26, T27, LFSR, Fibonacci au pas 7, T36) sont tous négatifs. Le relais « cinq angles morts » (25/09, nuit ; base 7 §12.17) affirmait que la place des cribs sur le cuivre était fausse : c'est lui qui se trompait, en mettant le « ? » en tête de la ligne 25.
10. **Changer de question.** « Quelle famille passe les 24 lettres ? » est la boucle de la communauté depuis 35 ans. « Quel procédé fabrique un texte qui ressemble à K4 sur ses 97 lettres ? » a donné en un jour la borne des doublets et le profil du « 7 ».

---

## 9. Questions ouvertes, et ce qui les trancherait

| Question | Ce qui la trancherait | État |
|---|---|---|
| Le procédé de K4 | Le chiffré de **K5** : même rang pour BERLINCLOCK, « codage semblable ». Premier test immédiat : lettres de K5 en 63–73. Puis attaque en profondeur (`audits/k5_depth/`, autotesté ; protocole dans `docs/veille_k5_et_demande_archives.md`) | K5 non publié (Paradigm : « in the future ») |
| Les **8 signes** sous LINCLOCK : lettres de clé, clair LINCLOCK, début d'une 15ᵉ ligne (K4 d'abord long de 105 lettres), note ? | Photo de l'original en lumière transmise, rasante ou infrarouge (Paradigm). Table de reconnaissance prête (`audits/erreurs_multiples_2026_09_24/results_huit_signes.txt`) | Illisibles dans toutes les copies disponibles, masqués dès 2006. Demande non envoyée (choix de l'utilisateur) |
| La **description écrite** de la méthode | Publication par Paradigm | Au coffre |
| Le « 7 » : hasard, intervention ou étape qui n'est pas lettre à lettre ? | K5, les 8 signes ou la méthode. Les 24 lettres ne suffisent pas | Ouvert, recadré (§6) |
| Autoclé Vigenère à l'écart 7 avec un alphabet **non** construit sur un mot-clé et au moins une erreur, ou avec deux alphabets libres sans erreur (T36b) | Plus de clair connu (K5) ; une attaque sur 97 lettres qui passe son contrôle positif dans cette famille | Seul coin non fermé du procédé désigné par le profil. Le calcul dans un autre groupe que Z/26 n'y change rien (T36) |
| Alphabets non construits sur un mot-clé ; clés choisies à la main (liste de mots, pochoir) | Une source qui fixe l'alphabet ou la clé : « Stencil Patterns, circa 1988 », dossier scellé par le donateur aux Archives of American Art | Hors d'atteinte des 24 lettres |
| Une lettre retirée ou ajoutée entre les cribs | Protocole écrit (base 7 §10.3) | Priorité basse : clair de 97 lettres avec les cribs aux rangs du cuivre ; longueurs réglées sur le papier |
| Transposition de tous les bits (forme extrême de DeSeve) | Protocole écrit (base 7 §10.6) | Peu plausible (valeurs 26–31 sans lettre) |
| Attaque sur le texte entier, 275 couples d'erreurs non parcourus (moteur T5) | Calcul | Non fait ; faible priorité |
| L'analyse NSA de K4 (1998–1999) | Demande de déclassification visant « The Fourth Breakthrough? » | Classifiée |
| Le « big hint » donné à la CIA vers 1997–1998 | Archives CIA ; témoignage | Jamais publié |
| Curiosités U, KZ-TJ-DI, clé Beaufort répétée | Une solution devra les expliquer ou les rendre fortuites | Conservées comme contrôles |
| La méthode de Sanborn dans ses œuvres de 1991 (*Code Room*, *Covert Obsolescence*, clé MEDUSA : « je suis resté aux systèmes de la période Kryptos ») | Relevés publics de leurs textes codés | Piste documentaire jamais instruite |
| *Secret Past* (1992) : une œuvre de Sanborn dont la moitié « duplique le texte de Kryptos » (liste de la galerie Drysdale, IMG_1485) | Retrouver l'œuvre ou une photo, et comparer son K4 et son « ? » au cuivre | Piste nouvelle (25/09), non instruite ; *Antipodes* (1997) a un K4 identique |
| Orientation réelle de la rose des vents ; géométrie mesurée de l'écran et du site ; sens de lecture des dalles Morse | Relevé sur place, vue aérienne nette | Données absentes. Aucune ne fixerait seule un paramètre |
| Masques des « E » du Morse (règle des intervalles) | Une règle définie d'avance et une transcription vérifiée | Règle non définie ; la règle des groupes est éliminée |

---

## 10. Guide du dépôt

### 10.1 Bases documentaires (`docs/knowledge_base/`)
| Base | Contenu |
|---|---|
| **09** (ce document) | Synthèse complète au 25/09 ; point d'entrée |
| **10** | Registre complet des tentatives : tests, recherches documentaires, outils, raisonnements corrigés, avec leur résultat |
| 08 | Synthèse du 24/09, complétée le 25/09 ; tableaux de recoupement des sources |
| 01 | Corpus de l'artiste : tout ce que Sanborn et Scheidt ont dit, par date |
| 02 | Tout ce qui a été testé et réfuté, avec la portée de chaque test |
| 03 | Kryptos lu comme un parcours, du Morse de l'entrée à la cour |
| 04 | Les phénomènes de K4 (lettres de KRYPTOS, « 7 », pas de transposition) et relecture critique |
| 05 | Audit des sources primaires (papiers de Sanborn aux Archives of American Art) |
| 06 | Documents versés par l'utilisateur le 24/09 (NSA 1991–1992, NOVA 2006, réunion de 2015…) |
| 07 | Fichiers et archive du groupe kryptos.groups.io (2003–2026), balayages, tests T7–T36, relais |

### 10.2 Audits (`audits/`)
- **21/09** : fiabilité du registre amont, cellules ouvertes, authenticité documentaire, trois audits de solutions publiées (`k4_audit_001` à `003`). État : « le blocage est documentaire ».
- **22/09** : éliminations algébriques par SAT avec alphabets inconnus (`algebraic_elimination_2026_09_22`, raisonnement dans `docs/k4_mechanism_reasoning_2026_09_22.md`) ; exploration 01.
- **23/09** : une vingtaine d'audits courts, un par piste relayée : erreur tolérée (`one_slip`), crib décalé, chaque crib seul, alphabets à mot-clé, Wheatstone, Gromark (portée et dates), superpositions de plaques, pliure, serpentin, textes de Langley, chiffré de K3 comme clé, Morse (E, miroir), XOR 5 bits, ligne + colonne Beaufort, clé par paliers, tableau comme sélecteur, batterie positionnelle, mots visibles, catalogue « 20 hypothèses ».
- **24/09** : `vision` (T1–T21, statistiques revues), `erreurs_multiples` (e_min, empreinte des doublets, 8 signes), `motcle_pas7` (alphabets à mot-clé, clé courante anglaise), `recuit` (attaque libre : ne passe pas son contrôle).
- **25/09** : `matrices` (alphabets en matrice), `blocs_motcle`, `martinsburg` (ASCII 7 bits), `relais` (T27–T33, sept parties), `moteur` (C/OpenMP, T0–T6), `simulateur` (52 procédés, T34), `procedes` (borne des doublets, ≈ 20 000 procédés, T35, géométrie), `autocle_groupes` (T36 : autoclé à l'écart 7 dans 23 groupes ; T36b : deux codages libres).
- **Outil prêt** : `k5_depth` (attaque en profondeur K4/K5, sans paramètre libre).

### 10.3 Documents de méthode (`docs/`)
- Protocole, registre des familles et politique de preuve : `research_protocol.md`, `family_registry.md`, `claims_ladder.md`, `EVIDENCE_POLICY.md`, `search_policy.md`.
- Faits de base amont (anglais) : `kryptos_ground_truth.md`, `invariants.md`. Voir le §7, n° 4 et 6, pour les chiffres et hypothèses dépassés.
- Paysage amont : `two_systems_landscape.md`, `composition.md`, `procedural_anomaly_recipes.md`.
- Feuilles de Sanborn : `k3_chart_layout_and_route_2026_09_19.md`, `nyt_k1k2_chart_physical_layout_2026_09_19.md`.
- Veille K5 et brouillon de demande aux Archives of American Art : `veille_k5_et_demande_archives.md`.

### 10.4 Sources et règles
- `sources/docs_utilisateur_2026_09_24/` : documents versés par l'utilisateur.
- `sources/aaa_sanborn_papers/` : exports PDF des dossiers numérisés du fonds Sanborn (Archives of American Art, Git LFS), à citer comme le demande le Smithsonian.
- `sources/groupsio_membres/` : copie des fichiers réservés aux membres du groupe. **Ne pas republier.** Le dépôt est privé depuis le 24/09. S'il redevient public, ce dossier doit d'abord être retiré de tout l'historique git.
- Aucun identifiant de compte n'est écrit dans le dépôt. Les adresses des membres sont remplacées par « [courriel] ».
- On n'utilise **ni** le clair acheté par Paradigm **ni** son vérificateur payant pour ajuster une méthode : une seule soumission, à la toute fin.

---

## 11. Pour reprendre

1. Lire ce document, puis la base 2 avant tout calcul, pour ne pas refaire un test.
2. Ne plus tester de famille de clés contre les 24 lettres **sans raison documentaire nouvelle**. La borne des doublets et les balayages du 25/09 couvrent ce que les cribs peuvent dire.
3. Surveiller K5 (Paradigm, groupe, presse) et lancer `audits/k5_depth/k5_depth.py` le jour de sa publication.
4. Si une photo de la feuille K3 + K4 devient disponible : lire d'abord les 8 signes, et les comparer à la table de reconnaissance.
5. Toute nouvelle piste passe par la même porte : portée écrite d'avance, contrôle positif, témoins K4 mélangé, tenue à une ou deux erreurs, antériorité vérifiée dans l'archive du groupe.
