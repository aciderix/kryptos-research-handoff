# 10 — Registre complet des tentatives, au 25/09/2026

**But.** Tout ce qui a été tenté dans ce dépôt, avec son résultat, en un seul endroit. Cela couvre les tests sur K4, mais aussi les recherches documentaires, les outils et les raisonnements corrigés en route. Ce registre complète la base 9 (synthèse) et la base 2 (portée exacte de chaque test sur K4).

**Comment le lire.**
- Une ligne par tentative. La colonne « Où » renvoie au détail.
- Positions comptées à partir de 0. « Témoins » : chiffrés de contrôle passés dans le même test. « Contrôle positif » : faux K4 fabriqué avec le procédé testé, que le test doit retrouver.
- **Résultats possibles** :

| Résultat | Sens |
|---|---|
| **Réussi** | Un fait établi, un texte décodé, un outil validé, une source retrouvée |
| **Éliminé** | Le procédé est incompatible avec K4 dans la portée indiquée |
| **Sans signal** | K4 se comporte comme ses témoins : ni éliminé, ni soutenu |
| **Non concluant** | Le test ou l'outil n'a pas assez de puissance, ou n'est pas allé au bout |
| **Bloqué** | Source ou accès impossible depuis l'environnement |
| **Non fait** | Protocole écrit mais pas exécuté, ou décision de ne pas le faire |
| **Corrigé** | Une affirmation antérieure du dépôt s'est révélée fausse et a été rectifiée |

**Aucune tentative n'a produit de clair de K4.**

---

## A. Héritage du dépôt d'origine (jusqu'au 21/09/2026)

Ces résultats viennent du dépôt source (`jcolinpatrick/kryptos`). Ils sont repris de `MEMORY.md`, `exhaustion_log.json` et `docs/`, sans avoir été rejoués ici, sauf mention.

| Tentative | Portée | Résultat | Où |
|---|---|---|---|
| Journal d'antériorité | 1 044 entrées : 732 actives, 310 épuisées, 2 remplacées | Référence | `exhaustion_log.json` |
| Contraintes de Bean (égalité, inégalités, 101 contraintes linéaires) | toute clé additive, alignement direct | **Réussi** : exactement 624 suites de clé possibles aux 24 positions | `MEMORY.md` |
| Clés périodiques Vigenère, Beaufort, variante, A–Z et KRYPTOS | périodes testables, alignement direct | **Éliminé** | base 2 §1 |
| Transposition pure, toute permutation | — | **Éliminé** : les cribs demandent 3 E, K4 n'en a que 2 | base 2 §1 ; base 1 |
| Autoclés (clair, chiffré, Quagmire II), alphabets A–Z/KRYPTOS | tout amorçage | **Éliminé** (liste « à ne pas relancer ») | `MEMORY.md` §5 |
| Hill 2 × 2, fractionnements (bifide, trifide, ADFGVX, damier), Playfair, César, affine, Atbash | — | **Éliminé** | base 2 §1 |
| Nulles insérées + clé périodique | — | **Éliminé** | base 2 §1 |
| Quagmire III/IV à mots-clés thématiques, indicateurs, tableaux KA non standard, alphabets étendus, Vigenère homophone | listes de mots-clés | **Éliminé** sous portée | base 2 §2.1 |
| Chaocipher | 142 000 paires d'alphabets à mot-clé | **Sans signal** (7/24) ; l'attaque par chaînes est ensuite jugée non admissible | base 2 §2.1 ; `MEMORY.md` §5 |
| Clés progressives, polynomiales, Fibonacci, récurrences linéaires et affines d'ordre 1–8 | — | **Éliminé** | base 2 §2.2 ; `docs/invariants.md` §10 |
| Gromark « zéro amorce » | alphabets fixés | **Éliminé** avec alphabets fixés seulement (voir 23/09) | base 2 §2.2 |
| Chiffre soviétique en trois étapes, VIC, Ubchi, clé interrompue, Wichmann-Hill (graines 0–50) | — | **Éliminé** sous portée | base 2 §2.2 |
| Clés courantes : K1–K3 (jusqu'à 47 G de vérifications avec transposition), Carter (livre), corpus égyptiens, italien, espagnol, Morse K0 | — | **Éliminé** sous portée | base 2 §2.3 |
| Grilles et routes jusqu'à 20 × 20 ; colonnes simples, doubles, Myszkowski, AMSCO, Nihiliste ; grilles tournantes, Cardan, Fleissner ; carré latin, bandes | — | **Éliminé** sous portée ou **sans signal** | base 2 §2.4 |
| TABP : transposition puis substitution périodique, v1 à v3 | 6 165 transpositions + 252 840 composées, A–Z/KA, p = 1–50 | **Éliminé** (tous au niveau du bruit après correction) | `MEMORY.md` §1 |
| Cadre de composition v1–v3 | 105 000 branches, 37 campagnes | **Sans signal** (max 6/24) | `MEMORY.md` |
| Campagne cartésienne à deux couches du 12/04/2026 | 206 448 profils (masques, projections × couches presque neutres) | **Sans signal** ; ne couvre pas les couches internes fortement mélangeantes | `docs/two_systems_landscape.md` |
| Perturbation du chiffré, étape A : une lettre de K4 changée | 10 465 764 réglages, 719 mots-clés | **Éliminé** (0 passage de Bean) ; étape B **non faite** | `MEMORY.md` |
| Routes selon la rose des vents ; horloge de Berlin comme route ; mots « Operation Gold », cryptonymes ; coordonnées et dates ; fautes d'orthographe comme clé ; différences Antipodes/Kryptos | — | **Éliminé** ou **sans signal** ; l'horloge de Berlin reste un registre non conclu | base 2 §2.6 |
| Palette {B,G,I,K,O,W,Z} et masques de nulles dérivés | — | **Corrigé** : artefact, ligne retirée | base 2 §2.5 |
| Masques Morse des « E » en surnombre (F-10) | — | **Non fait** : scripts jamais exécutés (`Last run: never`) | `docs/family_registry.md` |
| Recherche par quadrigrammes d'une composition mono + transposition + clé courante (E-FRAC-54) | — | **Non concluant** : aucun score ne bat le hasard adverse à 97 lettres ; piste arrêtée | `MEMORY.md` §2 |
| Anomalies « Stehle Δ5 » et « lettres de KRYPTOS proches » | réexamen amont | **Réussi** (revérifiées), mais aucun prédicat dur | `MEMORY.md` |

---

## B. 21/09 : audits documentaires et de fiabilité (sans calcul sur K4)

| Tentative | Résultat | Où |
|---|---|---|
| Audit de la « solution » SolveKryptos (7 × 14, cartes auxiliaires) | **Réussi** comme contrôle : le clair proposé est arithmétiquement cohérent, mais les cartes sont ajustées à rebours ; revendication seulement | `audits/k4_audit_001_solvekryptos/` |
| « L » en trop du tableau comme superposition | **Non concluant** : pas une recette complète | `audits/k4_audit_002_extra_l_overlay/` |
| Largeur 21, saut 5, lecture 8 | **Non concluant** : pas une recette complète | `audits/k4_audit_003_width21_skip5_read8/` |
| Fiabilité du registre amont | **Corrigé** : la campagne « L en trop » étiquetée Beaufort était du Vigenère ; trois campagnes sans résultat archivé reclassées « registre incomplet » | `audits/prior_art_reliability_2026_09_21/` |
| Cohérence du registre des familles | **Corrigé** : F-10 (masques Morse) n'était pas éliminé | `audits/registry_coherence_2026_09_21/` |
| Cellules ouvertes F-02, F-07, F-08, F-10, mécanismes physiques | **Non concluant** : aucune recette admissible ; informations manquantes listées | `audits/open_cells_2026_09_21/` |
| Authentification des scans d'archives (IMG_1236, 1238, 1555, 1340, 1569…) | Scans réels, mais provenance non authentifiable à cette date | `audits/documentary_authentication_2026_09_21/` |
| Recherche documentaire générale (deux systèmes, masque, 1986/1989, orientation) | Contraintes réelles, aucune recette | `docs/documentary_research_2026_09_21.md` |

---

## C. 22/09 : éliminations algébriques (SAT, tout alphabet) et exploration 01

| Tentative | Portée | Résultat | Où |
|---|---|---|---|
| Quagmire III périodique, **tout alphabet** | p = 1–52 | **Éliminé** pour p = 1–26, 31–40, 42–52 ; 27–30 et 41 sans contrainte ou au niveau du hasard | `docs/k4_mechanism_reasoning_2026_09_22.md` §2.2 |
| Quagmire I, II, IV, tout alphabet | p = 1–52 | **Non concluant** : 24 lettres ne suffisent pas | idem |
| Quagmire III, clé périodique vérifiée **dans chaque crib seulement** | p ≤ 12, résiste aux lettres omises | **Éliminé** pour p ≤ 11 | idem |
| Clé courante = clairs de K1, K2, K3, tout alphabet, tout décalage | — | **Éliminé** (test puissant) | idem |
| Autoclés, clé progressive, ligne + colonne, alphabets A–Z et KA | tous paramètres | **Éliminé** | idem §2.1 |
| Les mêmes avec alphabet inconnu | — | **Non concluant** (trop lâches, ou contrôles nuls non terminés) | idem §6 |
| Sélecteur limité aux lettres de KRYPTOS, PALIMPSEST, ABSCISSA | — | **Éliminé** (au moins 12 lignes distinctes nécessaires) | idem §2.1 |
| Colonnes KRYPTOS + Quagmire III, tout alphabet, 4 ordres | p = 1–26 | **Sans signal** | idem §2.3 |
| Grilles de 7 avec le « ? » (98 = 14 × 7) + Quagmire III | p = 1–12 | **Éliminé** ; l'argument du « ? » est ensuite tombé (IMG_1340) | idem §2.4 |
| Route de K3 : formule exacte (position + 1) × 192 mod 337 | — | **Réussi** : la route est une multiplication ; famille fermée de 102 permutations | idem §2.5 |
| Route de K3 (102 permutations) + Quagmire III | 260 des 408 configurations | **Sans signal** ; cas de la période 7 examiné : hasard | idem §2.5 |
| Route de K3 + clé courante des textes de la sculpture | 7 272 combinaisons | **Éliminé** (max 8/24, comme le hasard) | idem §2.5 |
| Grilles irrégulières de largeur 7, 14, 21 | 48 des 80 configurations | **Sans signal** ; un lot perdu | idem §6 |
| Exploration 01 : 5 transformations fixées, 40 contrôles | — | **Sans signal** | `audits/exploration_01_2026_09_22/` |

---

## D. 23/09 : bases documentaires 1 à 5 et audits courts

### D.1 Recherches documentaires
| Tentative | Résultat | Où |
|---|---|---|
| Orientation de l'aiguille gravée de la rose des vents (homographie sur deux photos indépendantes, CIA et Gillogly 1999) | **Réussi** : 246,0° / 66,0° et 247,3° / 67,3° : axe **OSO (vers la magnétite) – ENE**, dans le repère de la rose ; le NE est exclu | base 1 §2 |
| Orientation de la rose par rapport au vrai nord | **Non fait** : donnée absente ; Sanborn dit le cadran « décalé » (2006) ; NSA 1991 : 230–240° | base 1 §2 ; base 6 |
| Morse K0 : lecture des plaques de Gillogly, U de YOUR | **Réussi** : concorde avec Rumkin, sauf YOUR (U, et non G : Rumkin se trompait) | base 1 §2 |
| Ordre physique des plaques Morse « de l'entrée à la cour » | **Corrigé** : c'est un ordre de liste recopié de site en site ; le sens réel du parcours reste inconnu | base 1 ; base 3 |
| Origine de la phrase « deux systèmes… indice majeur » | **Réussi** : propos de Sanborn à l'inauguration (5/11/1990) ; elle porte sur **la plaque du bas, K3 + K4** | base 1 §4 quinquies |
| Date d'inauguration | **Corrigé** : 5 novembre 1990, et non le 3 | base 1 §2 |
| Textes « brouillés » de l'archive de 2025 | **Corrigé** : c'est le clair découpé par phrases pour la CIA, pas une étape du chiffrement | base 1 §4 bis |
| Entretien d'histoire orale de 2009 (Archives of American Art) | **Réussi** : lu en entier | base 1 §4 bis |
| *The Cryptogram* 1991, lettre de Sanborn de 1989, discours de Webster, article de Yandle | **Réussi** : « en partie avec le tableau, en partie avec un système potentiellement redoutable » ; deux enveloppes, dont les **mots-clés** | base 1 §4 ter |
| Photos RR Auction (12, puis 31) | **Réussi** : transcription de l'inauguration, contrat, plaque d'essai, brouillons (le Q d'IQLUSION absent du brouillon) | base 1 §4 quinquies |
| Photos KryptosBot des papiers de Sanborn | **Réussi** pour IMG_1340 (voir 24/09), 1410, 1555, 1569 ; IMG_1236 et « 7 × 88 » rattachés à d'autres projets ; les autres lues le 25/09 (voir F) | base 1 §4 sexies ; base 5 |
| Fonds Sanborn des Archives of American Art | **Réussi** : numérisé et accessible (EDAN) ; 16 dossiers lus, dont le manuscrit de 2009, la dédicace, la presse de 1990–1991, les feuilles russes | base 5 |
| Site aaa.si.edu et web.archive.org en direct | **Bloqué** (403) ; contourné par l'EDAN et les copies fournies | base 5 |
| Dossier « Stencil Patterns, circa 1988 » | **Bloqué** : scellé par le donateur (AAA, 2025) | base 5 ; base 7 §11.2 |
| Cassette audio de la dédicace (1990) ; addition de 2026 ; numérique non traité | **Bloqué** : non numérisés | base 5 |
| Feuilles de chiffrement russes de Sanborn (1990–1991) | **Réussi** : Quagmire II, alphabet ТЕНЬ, clé МЕДУЗА ; 22 lettres sur 23 retrouvées ; coquilles de clé sur la feuille | base 5 §6/8 |
| *Cyrillic Projector* : clé ЙБАСГТ | **Réussi** : c'est МЕДУЗА lue à travers l'alphabet ТЕНЬ | base 5 |
| K4 d'*Antipodes* | **Réussi** : identique lettre pour lettre à celui de Kryptos | base 5 |
| « 7 × 88 on a side » | **Réussi** : note de composeur sur un essai de police, sans rapport avec K4 | base 5 §6/18 |
| Tableau KRYPTOS tracé à la main, trois cases entourées | Observé : clés G et Q absentes de PALIMPSEST et ABSCISSA ; origine inconnue | base 5 §6/18 |
| Textes codés de *Code Room* / *Covert Obsolescence* (1991, clé MEDUSA) comme source sur la méthode | **Non fait** : piste documentaire ouverte | base 5 §16/2 n° 5 |
| Relais d'IA (Gemini, ChatGPT) sur la rose, le Morse, le « Compass cipher », la boîte 6 | Vérifiés un par un : quelques faits justes (MEDUSA visible sur l'œuvre), beaucoup de faux ou d'invérifiables | bases 1 et 5 |
| Affirmations exclues | « THE COMPASS ROSE IS HERE » (clair reconstruit de SolveKryptos) et « KOMITET » (message anonyme) : **exclus** | base 1 §7 |

### D.2 Tests sur K4 (23/09)
| Tentative | Résultat | Où |
|---|---|---|
| Quagmire I, II, IV à alphabets à mot-clé (préfixe ≤ 12 lettres) | **Éliminé**, sauf aux périodes longues où le hasard passe ; p = 19 déchiffre en charabia | `keyword_alphabet_2026_09_23` |
| Une erreur tolérée dans les cribs (Quagmire III tout alphabet p ≤ 13 ; clés courantes K1–K3) | **Éliminé** : les éliminations classiques résistent à une erreur | `one_slip_2026_09_23` |
| Mot-clé recommencé à chaque ligne de 31 | **Sans signal** | idem |
| Crib décalé par une lettre sautée ou ajoutée (s = −3…+3), Quagmire I–IV tout alphabet | **Sans signal** | `crib_shift_2026_09_23` |
| Chaque crib seul, sans lien | **Sans signal** | `cribs_separately_2026_09_23` |
| Wheatstone, cadran extérieur A–Z ou KRYPTOS, intérieur quelconque | **Éliminé** (contrôle 20/20) | `wheatstone_2026_09_23` |
| Gromark : pourquoi 0 amorce ici et 39 chez Bean | **Réussi** : 39 amorces reproduites avec le code de Bean ; la différence tient aux alphabets fixés ou libres | `gromark_scope_2026_09_23` |
| Gromark, un côté fixé par un alphabet de l'œuvre | **Éliminé** (0) | idem |
| Gromark amorcé par des dates (1986, 1989…) | **Éliminé** | `gromark_dates_2026_09_23` |
| Clé par paliers (« KKKL » dans la clé Beaufort) | **Éliminé** (0/13 650) ; le triplé KKK est banal (10,7 %) | `stepped_key_2026_09_23` |
| Superposition cuivre ↔ tableau (écran replié, miroir, toutes les superpositions à lignes parallèles) | **Éliminé** | `fold_overlay_2026_09_23` |
| Plaque du haut posée sur la plaque du bas | **Éliminé** | `plate_overlay_2026_09_23` |
| Batterie positionnelle (40 géométries × 15 fonctions) | **Éliminé** | `positional_battery_2026_09_23` |
| Lettre du tableau au dos comme sélecteur | **Sans signal** | `tableau_lookup_2026_09_23` |
| Feuille de K3 empilée sur K4 ; chiffré de K3 comme clé | **Éliminé** | `k3_chart_stack_2026_09_23`, `k3ct_key_2026_09_23` |
| Inscriptions de Langley comme clé courante | **Éliminé** | `langley_texts_2026_09_23` |
| Substitution partitionnée (lettres de KRYPTOS à part) | **Sans signal** | idem |
| Mots visibles sur l'œuvre (39) comme mots-clés ou amorces | **Éliminé** | `visible_words_2026_09_23` |
| Groupes de E du Morse comme clé numérique (règle B) | **Éliminé** ; règle A (intervalles) **non faite**, car mal définie | `morse_E_key_2026_09_23` |
| Miroir du Morse appliqué à K4 | **Éliminé** | `morse_mirror_2026_09_23` |
| Codage 5 bits + XOR ou addition mod 32 | **Éliminé** | `binary_xor_2026_09_23` |
| Serpentin, largeurs 7–31, cribs = positions du clair | **Éliminé** en pratique | `serpentine_2026_09_23` |
| Ligne + colonne en Beaufort, tout alphabet (CP-SAT) | **Éliminé** pour 10 largeurs ; ailleurs indécidable | `rowcol_beau_2026_09_23` |
| Catalogue « 20 hypothèses » (Polybe, coordonnées de K2, permutations mod 97, générateur congruentiel, DYAHR…) | **Éliminé** ou **sans signal** | `twenty_hypotheses_2026_09_23` |
| Réglettes M-138 découpées dans le tableau | **Éliminé** par raisonnement (positions 24 et 28) | base 2 §2.4 |
| Décalage constant « 6 points » (N → ENE) ; transposition en spirale ; autoclé à décalage constant | **Éliminé** | base 1 §4 sexies |

---

## E. 24/09 : documents versés, groupe kryptos.groups.io, tests T1 à T25

### E.1 Recherches documentaires
| Tentative | Résultat | Où |
|---|---|---|
| Documents versés par l'utilisateur (NSA 1991–1992 et 2014, ABC 1991, NOVA 2006, réunion de 2015, Carter, Martinsburg) | **Réussi** : lus en entier ; Scheidt n'a jamais vérifié K4 ; « rugosité à l'intervalle 7 » (NSA 1992) ; plan orienté de la cour (NSA 1991) | base 6 |
| IMG_1340 « 4, 8, 10, 25 » | **Réussi** : ce sont les lignes des quatre « ? ». **Corrigé le 25/09** : le mot devant « coded » est illisible ; indice compatible, pas preuve | base 6 ; audit `vision` §1 ; base 5 (25/09) |
| Fichiers du groupe (1 780 fichiers inventoriés, 460 Mo lus) | **Réussi** : fichier NYT de Sanborn (« ? » non numéroté), pièces NSA 1993–1999, transcription Scheidt 2020, comptes rendus de dîners | base 7 §1–§3 |
| 81 clairs complets revendiqués par des membres | Vérifiés : **aucun** n'est dérivé par une méthode reproductible ; la « solution » de 2025 est un masque jetable | base 7 §3 |
| Archive des messages 2003–2018 (20 250 messages) | **Réussi** : témoignages directs, antériorité des doublets (2002), petit fragment de 97 lettres | base 7 §7–§9 |
| Balayages des dossiers personnels (258 fichiers, puis 270 observations et 195 affirmations chiffrées) | **Réussi** : chaque affirmation chiffrable mesurée ; rien de caché qui résoudrait K4 | base 7 §8–§9 |
| Petit fragment *Covert Operations* (97 lettres) | **Réussi** : décodé (méthode de K1, clé SHADOW) ; 4 erreurs de chiffrement, 3 du même type | base 7 §9.4 |
| Fragment de Zola (512 lettres) | **Réussi** : transposition en 16 colonnes, lettres doublées de remplissage | base 7 §9.3 |
| « Chaîne de masquage » de 2017 | **Réussi** : identifiée comme 1/563 (puis testée, T8) | base 7 §7.3 |
| Compilation des entretiens (375 pages, 1989–2020) | **Réussi** : NPR 1999 (systèmes « spatiaux »), Big Techday 2013, « matrice d'origine » attribuée à Sanborn | base 7 §10 |
| Messages 2018–2026 (5 900, par l'API, lecture seule) ; 48 803 chaînes de membres confrontées aux cribs | **Réussi** : Paradigm, lettre de 2025, K5 ; aucun membre n'avait approché les cribs sans le voir | base 7 §11 |
| Brouillons de K3 (photo 21 de la vente) | **Réussi** : 342 = 336 + 6 ; plan « 11 lignes / 3 lignes 93 » exact | base 7 §11.6 |
| Feuille NOVA de K3 + K4 : 8 signes sous LINCLOCK | Fait relevé ; **lecture bloquée** (signes masqués dès 2006) | base 7 §11.6, §12.2, §12.14 |
| Documentaire de Paradigm (YouTube), fiche RR Auction (Cloudflare), NYT 12/06/2026 | **Bloqué** | base 7 §11.6 |
| Mémo Donovan de 1944 (boîte de la pierre angulaire) ; plaque de dédicace du bâtiment de 1988 | **Bloqué** : texte fiable introuvable ; non testés | base 7 §4 ; `langley_texts` |
| Vidéos et modèles 3D de membres, fichiers de plus de 15 Mo, formules des tableurs, longs fils 2018–2026 | **Non fait** (priorité basse, méthodes proposées) | base 7 §3, §7.5, §11.5 |
| Martinsburg (binaire de l'IRS) | **Non concluant** le 24/09 ; **réussi** le 25/09 (ASCII 7 bits) | base 6 ; `martinsburg_2026_09_25` |
| Demande de photo des 8 signes à Paradigm ; demande aux Archives of American Art | **Non fait** : brouillons prêts, envoi laissé à l'utilisateur | base 5 ; `docs/veille_k5_et_demande_archives.md` |

### E.2 Statistiques mesurées
| Mesure | Résultat | Où |
|---|---|---|
| Largeur 21 (11 bigrammes verticaux) | p ≈ 0,09 après correction, non indépendante | audit `vision` §3 |
| IC des positions 0–20 | **Non significatif** | idem |
| Doublets ≡ 4 (mod 7) | p ≈ 0,0006, 0,02 corrigé | idem |
| Écart 7 (9 coïncidences) | p ≈ 0,005, 0,06 corrigé | idem |
| Les deux ensemble, module libre | **Réussi** : p ≈ 2 × 10⁻⁴ (premier chiffre de 10⁻⁵ **corrigé**) | idem ; base 7 §8.2 |
| Contrôle K1–K3 | **Réussi** : aucun « 7 » ailleurs sur la sculpture | idem |
| IC par colonne de 7 | p = 0,24 : pas de période | idem |
| Clé Beaufort A–Z répétée localement | p ≈ 0,004 corrigé (curiosité) | idem |
| Propriété U de Caveney ; regroupements (Improvidus) ; Stehle Δ5 ; ABA ; 336 coïncidences | U : 1,8 × 10⁻⁴ (curiosité) ; regroupements expliqués par les doublets ; Stehle 0,0065 corrigé ; ABA et 336 ordinaires | base 7 §8–§9 |
| Simulation de 10 familles (`sim_7.py`) | Aucune ne concentre les doublets (≤ 3 %) | base 7 §8.2 |
| Empreinte des doublets par phase de clé dans les chiffres de Sanborn | **Réussi** : 7 doublets sur 2 des 6 phases du petit fragment ; filtre GIRASOL (p ≈ 0,07, non significatif) | base 7 §12.1 |

### E.3 Tests sur K4 (T1 à T25, `audits/vision_2026_09_24/`, `audits/erreurs_multiples_2026_09_24/`)
| Test | Famille | Résultat |
|---|---|---|
| T1 | Clé écrite sur la sculpture, 13 parcours et 2D sur cylindre (57 617 placements) | **Éliminé** |
| T2 | Addition en chaîne (Gromark, VIC), Quagmire III | **Éliminé** ; en Quagmire IV, les 39 amorces de Bean sont exactement le hasard |
| T2 (7) | Amorces de 7 chiffres | **Éliminé** (Quagmire III) |
| T3, T4 | Deux mots-clés superposés, alphabets fixés puis quelconques | **Éliminé** pour les petits couples ; au-delà indécidable |
| T5 | Pliure au centre | Pliure complète **éliminée** ; demi-pliures **sans signal** |
| T6 | Journal de Carter, inscriptions du hall de la CIA | **Éliminé** |
| T7 | Textes « enfouis » : directive Truman, principes CIA 1988, lettre de 1989 | **Éliminé** |
| T8 | Suite de 1/563 | **Éliminé** |
| T9 | Chaîne en base 26 amorcée par KRYPTOS | **Éliminé** |
| T10 | Maquette GIRASOL de 1988 comme source de clé (54 188 placements) | **Éliminé** ; le bloc de démonstration se lit en Vigenère, clé RUG |
| T11 | Disque tourné par bloc de 7 | **Éliminé** sans erreur ; **sans signal** avec une erreur (T20) |
| T11b | Disque par bloc de 2 à 30 | idem |
| T12 | Autoclé mixte | **Sans signal** |
| T13 | Hill par blocs, petits coefficients | **Éliminé** sans erreur ; **sans signal** avec une erreur (T21) |
| T14 | Doublets vus comme glissements de copie | **Sans signal** |
| T15 | Wheatstone à cadran à mot-clé (704 880 cadrans) | **Éliminé**, robuste à une erreur (T21) |
| T16 | Clé transposée à la manière de K3 | **Éliminé** pour L ≤ 7 ; sous le hasard avec une erreur |
| T17 | Autoclé « vers l'avant » | **Sans signal** |
| T18 | Autoclé sur le clair, chaînes entre cribs, tout alphabet | Écart 7 **éliminé sans erreur** |
| T19 | T18 avec une erreur | **Non concluant** : K4 redevient compatible, comme 78 à 97 % des témoins |
| T20, T21 | Toutes les éliminations rejouées avec une erreur | Seuls T15 et la clé périodique courte tiennent |
| T22 | Nombre minimal d'erreurs, clé périodique p = 1–26, Q1–Q4 | **Éliminé** pour tout nombre plausible d'erreurs |
| T23 | Autoclé sur le chiffré avec erreurs | **Éliminé**, robuste |
| T24 | Période 7 + décalage par ligne du cuivre | **Sans signal** (trop lâche) |
| T25 | « On retourne la feuille » : convention par ligne | **Sans signal** |
| — | Toute substitution qui dépend de i mod 7 ; autoclé sur le chiffré à l'écart 7 | **Éliminé** sans calcul (paires 65/72 et 22/72) |
| — | M-94 à génératrice fixe | **Éliminé** |
| — | Transposition de bits (DeSeve) | Forme de DeSeve **éliminée** à la main ; transposition de tous les bits **non faite** (peu plausible) |
| — | Modèle d'E. Cruz, « 00++00++ », xor399, algorithme T, « Weltzeituhr Error Key », S. Perry, EGYPTXQUNDIG | **Éliminé** ou **sans valeur** |

### E.4 Alphabets à mot-clé (24/09, nuit, `audits/motcle_pas7_2026_09_24/`)
| Test | Résultat |
|---|---|
| Clé à pas 7 (période 7, par ligne du cuivre, par ligne de 7), 237 988 alphabets | **Éliminé** (au moins 7 erreurs, comme les témoins) |
| Autoclé sur le clair, L = 1–13, clair entier déchiffré | **Éliminé** |
| Clé courante tirée d'un texte anglais inconnu, un ou deux alphabets | **Éliminé** (puissance 99,5 % sans erreur, ≈ 80 % avec deux alphabets) |
| Autoclé sur le chiffré, tous écarts | **Éliminé** |
| Attaque par recuit sur les 97 lettres (`audits/recuit_2026_09_24/`) | **Non concluant** : l'outil échoue à son contrôle positif ; aucun essai sur K4 |

---

## F. 25/09 : nouveaux alphabets, moteur, simulateur, procédés

| Test | Famille | Résultat | Où |
|---|---|---|---|
| Alphabets en matrice | 1 294 102 alphabets ; clé courante, autoclés, pas 7 | **Sans signal** | `matrices_2026_09_25` |
| T11/T11b rejugés avec alphabets à mot-clé | blocs n = 5–14 | **Éliminé** (au moins 7 erreurs) | `blocs_motcle_2026_09_25` |
| Martinsburg | 17 lignes binaires | **Réussi** : ASCII 7 bits ordinaire, une erreur d'un bit ; rien pour K4 | `martinsburg_2026_09_25` |
| T26 | Clé ligne + colonne en largeur 7, alphabet libre (relais DeepSeek) ; variantes Fibonacci et « boussole » | **Non concluant** (sans pouvoir) ; boussole au hasard | base 7 §12.5 |
| T27 | Autoclé sur le clair, alphabets à mot-clé, erreurs comptées par équation | **Éliminé** (écart 7 : au moins 10 erreurs) ; défaut de comptage de `kwautokey.c` et `k4x.c` **corrigé** | `relais_2026_09_25` |
| T28 | Trifide à toute phase, périodes 2–40 | **Éliminé** | idem |
| T29 | Disque tourné de temps en temps | **Éliminé** (alphabets de Sanborn) | idem |
| T30, T31 | Clé périodique avec saut de phase | **Sans signal** | idem |
| — | Clé courante en allemand, français, latin | **Sans signal** | idem |
| — | KZ, TJ, DI « comprimés » | Mesuré : p ≈ 1,4 × 10⁻³ (curiosité) | idem |
| — | GIRASOL et la résonance des doublets | p ≈ 0,01 parmi les alphabets de Sanborn ; aucune structure de clé | idem |
| — | Maquette de 1988 relue | **Réussi** : une lettre omise et deux fois la même erreur de table | idem |
| T32 | Clé progressive, alphabets à mot-clé | **Sans signal** | idem |
| T33 | Toute clé numérique 0–9 | **Sans signal** | idem |
| Moteur T0 | Moteur C/OpenMP unique | **Réussi** : 35 à 300 fois plus rapide, contrôles positifs par famille | `moteur_2026_09_25` |
| Moteur T1 | 30 témoins sur 1,4 million d'alphabets | **Sans signal** ; seul écart (EUODOS) déchiffré en charabia | idem |
| Moteur T2 | Dictionnaire élargi (430 167 mots), deux mots-clés (2,96 millions) | **Sans signal** | idem |
| Moteur T3 | Modèle d'erreur de Sanborn validé sur le fragment ; clé périodique p ≤ 14 | **Éliminé** (au moins 5 erreurs) | idem |
| Moteur T4 | TABP (hypothèse NSA n° 2) ; Quagmire IV à deux mots-clés | **Éliminé** / **sans signal** | idem |
| Moteur T5 | Attaque sur le texte entier depuis les clés exactes des cribs | **Réussi** sur le contrôle ; K4 : charabia ; 275 couples d'erreurs **non faits** | idem |
| Moteur T6 | Familles indécidables avec alphabets à mot-clé (ligne + colonne, deux mots, autoclé vers l'avant, Q1–Q4 p ≤ 26) | **Sans signal** | idem |
| Simulateur | 52 procédés × 10⁶ faux K4 | **Réussi** : profil « voisin » ; seule l'autoclé Vigenère le produit ; aucun procédé ne concentre les doublets | `simulateur_2026_09_25` |
| T34 | Autoclé Vigenère à l'écart 7, deux alphabets à mot-clé (5,3 × 10¹¹ paires) | **Éliminé** (au moins 7 équations fausses sur 17) | idem |
| Borne des doublets | tout chiffre déchiffrable | **Réussi** : aucune clé fixée d'avance ne rend les doublets probables | `procedes_2026_09_25` |
| Recherche de procédés | ≈ 20 000 procédés, deux contrôles positifs | **Sans signal** : aucun ne reproduit la concentration | idem |
| T35 | Fautes de recopie + clé simple | **Éliminé** | idem |
| Géométrie en rangs de 7, amas | — | **Réussi** : relations orthogonales seulement ; hypothèse de mise en page pour le « ? » | idem ; base 9 §7 |
| LFSR d'ordre 1–5 | 101 086 alphabets | **Éliminé** | `relais_2026_09_25` 5ᵉ partie |
| « Fibonacci au pas 7 » (colonnes mod 7 en récurrence d'ordre 2) | — | **Éliminé** | idem 7ᵉ partie |
| Agrandissement des images NOVA des 8 signes | deux agrandissements indépendants | **Bloqué** : taches opaques, signes déjà masqués en 2006 | idem 6ᵉ partie ; `procedes` §4 |
| T36 | Autoclé sur le clair à l'écart 7 dans 23 groupes d'ordre 26 à 36 (grilles 5 × 6, 6 × 6, trifide, XOR 5 bits), même codage partout ; 3 conventions | **Éliminé sans erreur** (67 cas sur 67, optimum prouvé ; 197/197 contrôles retrouvés) ; **non concluant** avec une erreur (83 % des témoins font aussi bien) | `autocle_groupes_2026_09_25` |
| T36b | Idem, codage du chiffré indépendant du codage du clair (les deux libres) | **Non concluant** : K4 compatible sans erreur, comme 55 à 95 % des témoins | idem |
| Relais « cinq angles morts » (25/09, nuit) | 105 = 15 × 7, place des cribs, autoclé hors alphabet, doublets, cadran | Place des cribs : **faux** (base 4 juste) ; 105 : calcul circulaire, lecture déjà notée, sans effet ; doublets et cadran : déjà connus ou éliminés ; autoclé : T36 | base 7 §12.17 |


### F bis. 25/09 : relecture intégrale de kryptosbot.com (archive, findings, research-questions)
| Tentative | Résultat | Où |
|---|---|---|
| Lecture des trois pages et des **42 photos** de la page d'archive (14 seulement le 23/09) | **Réussi** : toutes vues, passages manuscrits agrandis | base 5, complément du 25/09 |
| Relecture agrandie d'IMG_1340 | **Corrigé** : le mot devant « coded » est illisible ; « section » confirmé (et non « seeding ») | idem, A.1 |
| Orientation de l'aiguille sur trois études de rose dessinées (IMG_1518) | **Réussi** : 63°, 55° et 66°, entre NE et ENE ; deux proches de l'aiguille gravée (66–67°) | `measurements/rose_studies_img1518.py` |
| Tableau aux lettres entourées (IMG_1223–1224), « piste vivante » du projet amont | Déjà analysé le 23/09 (6/18) ; relu : clés G/G, Q/Q, B/V (et non A/W) ; aucune paire des cribs | base 5, A.3 |
| Carte sur transparent (IMG_1221) | **Réussi** : carte du projet d'Alexandria de Sanborn et Urban (non réalisé), pas de Kryptos | idem, A.4 |
| « He lied » (IMG_1384) | **Réussi** : coordonnées à 37° de la jaquette du *Da Vinci Code* ; réaction probable à Dan Brown | idem, A.5 |
| « ? = J » (IMG_1531) | **Éliminé** (98 caractères, contraire au fichier NYT et au contrôle de Paradigm) | idem, A.6 |
| *Secret Past* (1992), copie du texte de Kryptos (IMG_1485) | Piste documentaire **non faite** | idem, A.7 |
| Carnet relié : croquis du site, vocabulaire du renseignement, clair « copper Veil » (IMG_1580–1582) | Relevé ; date et œuvre inconnues ; rien de testable | idem, A.8 |
| Lettre de l'avocat sur *The Lost Symbol* (2009) | Lue sur le montage ; l'attribution du paragraphe du chapitre 53 est de KryptosBot | idem, A.9 |
| État amont (findings) : Stehle ≈ 1/205, largeur 21 indépendante des W, 13 302 candidats sans survivant, Mengenlehreuhr et Weltzeituhr sans signal | Repris ; cohérent avec nos mesures | idem, C |
| Rapport amont `docs/REAL_K4_CURRENT_POSITION.md` | **Non fait** : hors de notre périmètre d'accès | — |

---

## G. Outils : validés, en échec, corrigés

| Outil | Résultat | Où |
|---|---|---|
| Solveur SAT exact (python-sat / CaDiCaL), 22/09 | **Réussi** ; Z3 abandonné (réponses « unknown ») | `algebraic_elimination_2026_09_22` |
| Générateur de contrôles Quagmire I/II | **Corrigé** : alphabet mélangé du mauvais côté | `docs/k4_mechanism_reasoning_2026_09_22.md` §6 |
| Passes SAT progressive et ligne + colonne avec alphabet inconnu | **Non concluant** : contrôles nuls non terminés | idem |
| Moteur exact en C (`k4lib.h`), 24/09 | **Réussi** : 2 000/2 000 contrôles positifs, ≈ µs par test | audit `vision` §0 |
| e_min par CP-SAT | **Réussi** ; version C abandonnée sur les cas lâches | `erreurs_multiples_2026_09_24` |
| Recuit simulé, recuit sur mot-clé libre | **Non concluant** : échec du contrôle ; fabrique de l'anglais apparent | `recuit_2026_09_24` |
| Témoins uniformes pour les tests qui notent un texte | **Corrigé** : remplacés par des témoins K4 mélangé | `motcle_pas7_2026_09_24` §3 |
| Comptage des erreurs d'autoclé | **Corrigé** (T27) | `relais_2026_09_25` |
| Moteur C/OpenMP, simulateur, recherche exacte par tiroirs (T34) | **Réussi** | audits du 25/09 |
| Témoins de T34 | Arrêtés après 16 tirages (lenteur) | `simulateur_2026_09_25` |
| Recherche de procédés, première version | **Corrigé** : des procédés « fuyants » (colonne sans clé) dominaient ; exclus. Second contrôle positif ajouté avec un procédé différent | `procedes_2026_09_25` |
| Outil `k5_depth` | **Réussi** : autotesté ; prêt pour K5 | `audits/k5_depth/` |

---

## H. Raisonnements corrigés en route

| Affirmation d'abord faite | Correction | Où |
|---|---|---|
| Le « ? » appartient à K4 (98 = 14 × 7, « prédiction gratuite ») | Le « ? » n'est pas chiffré ; K4 = 97 | IMG_1340 ; base 7 apport 1 |
| « Deux systèmes » = K4 en deux couches | La phrase porte sur K3 + K4 | base 1 §4 quinquies |
| Scheidt : « le masque est la première étape » | C'est la première étape de l'analyste ; l'ordre n'est pas dit | base 7 §11.6 |
| « Changer la base » : option refusée | Refus de la base mathématique ; le masque change la base de langue | base 7 §2.11 |
| « Matrice d'origine… tous les shifts » : de Scheidt | De Sanborn | base 7 §10.1 |
| Doublets mod 7 : Gillogly 2005 | Stehle ≈ 2002, publié en 2003 | base 7 §9.1 |
| « Le 7 conjoint » à 10⁻⁵ | 2 × 10⁻⁴ (la largeur 21 était comptée deux fois) | base 7 §8.2 |
| T18 élimine l'hypothèse NSA | Seulement sans erreur (T19) ; l'élimination robuste vient de T27 et T34 (alphabets de Sanborn) | base 7 §9.5 |
| Serpentin écarté par raisonnement | Testé proprement, éliminé en pratique | `serpentine_2026_09_23` |
| Anomalie du U de YOUR | C'est un U : Rumkin se trompait | base 1 §2 |
| Cap vers la Weltzeituhr = ENE | 44,4° = NE | base 1 §2 |
| Clé « lue dans une matrice de 7 colonnes » comme candidat pour les doublets | Écartée par la borne des doublets | base 9 §7 |
| Rectangle 14 × 7 « avec le ? » tenu pour la feuille de travail | Hypothèse de mise en page | base 9 §7 |
| Qui a exécuté K4 : Scheidt (*Museum & Arts* 1990) ou Sanborn (WaPo 1999) | Tranché pour Sanborn (Scheidt 1991 et 2015) | base 5 ; base 6 |
| Martinsburg : « aucune convention simple » | ASCII 7 bits | `martinsburg_2026_09_25` |

---

## I. Ce qui n'a pas été fait, et pourquoi

| Piste | Raison | État |
|---|---|---|
| Lire les 8 signes sous LINCLOCK | Masqués dès 2006 ; original chez Paradigm | Demande non envoyée (choix de l'utilisateur) |
| Test en profondeur avec K5 | K5 non publié | Outil prêt |
| Lettre retirée ou ajoutée entre les cribs (familles liées à la position) | Priorité basse depuis le contrôle de Paradigm (97 lettres, cribs aux rangs du cuivre) | Protocole écrit (base 7 §10.3) |
| Transposition de tous les bits | Peu plausible (valeurs 26–31 sans lettre) | Protocole écrit (base 7 §10.6) |
| Masques Morse, règle des « intervalles » (F-10) | Règle non définie ; transcription et géométrie insuffisantes | Ouvert |
| Géométrie mesurée de l'écran et du site ; orientation de la rose par rapport au nord | Données absentes | Ouvert |
| Superposition tournée, gabarit de sélection, gabarit extérieur (F-07) | Plan coté absent | Ouvert |
| Textes de *Code Room* / *Covert Obsolescence* | Relevés publics non cherchés | Piste documentaire ouverte |
| Déclassification de « The Fourth Breakthrough? » (NSA 1999) | Demande administrative | Non faite |
| Perturbation du chiffré, étape B (amont) | — | Non faite |
| 275 couples d'erreurs de l'attaque sur le texte entier | Faible priorité | Non faits |
| Autoclé Vigenère à l'écart 7 avec deux alphabets libres, sur les 97 lettres | Les cribs ne tranchent pas (T36b) ; il faut noter le clair entier, et le recuit a échoué à son contrôle | Ouvert |
| Passes SAT inachevées (route de K3 : 148 configurations ; grilles irrégulières : 32 ; contrôles nuls progressive et ligne + colonne) | Calculs arrêtés | Non repris |
| 26 photos KryptosBot, OCR des dossiers AAA 9/4 et 9/5, vidéos et longs fils du groupe | Priorité basse | Non faits |

---

## J. Bilan en une phrase

Tout ce que les 24 lettres des cribs permettent de trancher, avec les alphabets de Sanborn et ses erreurs, l'a été : aucune famille ne passe. Ce qui reste demande une donnée nouvelle : K5, les 8 signes ou la description de la méthode.
