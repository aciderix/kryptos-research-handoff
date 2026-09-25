# Base documentaire 2 — Ce qui a déjà été testé (pour ne pas le refaire)

**Version :** 2026-09-23 (première consolidation)
**Règle d'usage :** avant tout calcul, chercher ici la famille concernée. Si elle y figure, lire la **portée exacte** (alphabets, périodes, alignement, textes). Un nouveau test n'est justifié que s'il sort **explicitement** de cette portée, et dit en quoi.

Rappels de vocabulaire (repris de `docs/family_registry.md`) :
- **Éliminé sous portée** : impossible ou sans signal *dans les hypothèses précisées*, pas en général.
- **Registre incomplet** : un script existe mais son résultat n'est pas archivé. Ce n'est pas une élimination.
- **Revendication seulement** : une « solution » publique existe, sans méthode vérifiable.

Sources :
- **Dépôt** : `exhaustion_log.json` (1 044 entrées : 732 actives, 310 épuisées, 2 remplacées), `docs/family_registry.md`, `MEMORY.md`, `audits/`.
- **Campagne algébrique du 22/09** : `audits/algebraic_elimination_2026_09_22/`, rapport `docs/k4_mechanism_reasoning_2026_09_22.md`.
- **Communauté** : références en fin de document.

---

## 1. Faits établis qui encadrent tout le reste

| Fait | Portée | Source |
|---|---|---|
| Aucune clé **périodique** (Vigenère, Beaufort, Beaufort variant, en AZ ou KA) ne passe sous alignement direct pour les périodes testables ; les contraintes de Bean ne laissent que 624 suites de clé possibles aux 24 positions | alignement direct (H1), additif | dépôt `MEMORY.md` (Bean, contraintes linéaires 2026-04-10) ; Bean 2021 |
| Périodes 27–29 : éliminées formellement | H1 | dépôt `e_period_27_29_formal` |
| **Quagmire III avec n'importe quel alphabet mélangé** (26! possibles), sens Vigenère, périodique 1–26, 31–40, 42–52 : impossible | H1 | campagne 22/09 (nouveau par rapport au registre, qui testait des alphabets de listes) |
| **Transposition pure** : exclue (la fréquence des lettres ne correspond pas à de l'anglais) | toute transposition sans substitution | dépôt `e_audit_05_scytale_cylinder`, `docs/two_systems_landscape.md` |
| **Autoclave** (sur clair ou chiffré, tout amorçage) : structurellement impossible | H1, alphabets AZ/KA | dépôt `e06_autokey`, `e_autokey_bidir_extended`, `e_s_69`, `e_s_85`, `e_autokey_bootstrap_00` |
| **Hill** 2×2 (et variantes par anomalies) : éliminé algébriquement | H1 | dépôt `e04_hill_cipher`, `e_audit_05_hill_2x2_lyar`, `k4_algebraic_eliminations` ; Numberworld (dét. insoluble) |
| **Bifide, trifide, fractionnements** : famille éliminée par preuves structurelles | périodes testées | dépôt `e_frac_21`, `e_s_09`, `e_s_42`, `e_s_44` |
| **Playfair**, **César**, **affine/monoalphabétique**, **Atbash** : éliminés | — | dépôt `e_playfair_01_full_disproof`, `disprove_caesar_rot`, `e_affine_mono_disproof`, `e_atbash_03_crib_drag` |
| **Insérer des nulles** ne sauve pas une clé périodique | H1 + nulles | dépôt `e_solve_10_null_proof` ; confirmé avec alphabet inconnu, p ≤ 11, campagne 22/09 |

## 2. Par famille

### 2.1 Substitutions polyalphabétiques et tableaux

| Ce qui a été testé | Portée | Statut | Où |
|---|---|---|---|
| Vigenère/Beaufort périodiques avec mots-clés thématiques | listes de mots-clés, AZ/KA | éliminé sous portée | dépôt `e_poly_01..03`, `e_rerun_01/02`, `e_tableau_20` |
| Quagmire III/IV, balayage d'indicateurs | 6 alphabets × 10 clés × 26 indicateurs | éliminé (0/24) | `e_aaa_indicator_sweep_07` |
| Quagmire III, axe « tableau » (PALIMPSEST, ABSCISSA, LATITUDE, MAGNETIC, COMPASS) | 22 périodes × 3 indicateurs | éliminé sous portée | `f_quagmire_tableau_axis_completion_2026_06_09` |
| Quagmire III masqué (nulles arbitraires) | 915 masques × 18 mots × p 3–12 | éliminé | `masked_quagmire_iii_probe_2026_05_25` |
| Tableaux KA non standard, navigation algébrique | — | éliminé sous portée | `e_ka_01`, `e_ka_02`, `e_tableau_nav_001_algebraic` |
| Alphabets étendus (27–30 symboles), Vigenère homophone | community (Kimmo) | éliminé | `e_extended_alphabet_vig_27/28_30`, `e_homophonic_vigenere_duplet` |
| Chaocipher | 142 000 paires d'alphabets de mots-clés | bruit (7/24) | `e_chaocipher_exhaustive_01` |
| Deux couches additives (KA dedans, AZ dehors) | thématique | éliminé sous portée | `e_two_layer_kainner_01` |
| Quagmire I, II, III, IV **avec alphabets inconnus** | p 1–52 | III-Vig éliminé ; I, II, IV **non tranchables** par 24 lettres | campagne 22/09 §2.2 |
| **Quagmire I, II, IV avec alphabets À MOT-CLÉ** (préfixe ≤ 12 lettres puis le reste dans l'ordre ; tout mot-clé, sans liste), Vig/Beau, p 1–52. Motivé par les feuilles de Sanborn (AAA 6/8) et les « keywords » (AP 1991) | exact (SAT), témoins positifs 350/350 | éliminé sauf aux périodes longues où le hasard passe aussi ; la seule case « sévère » (p = 19) déchiffre en charabia : **coïncidence** | `audits/keyword_alphabet_2026_09_23/` (23/09) |
| Sélecteur limité aux lettres de KRYPTOS/PALIMPSEST/ABSCISSA | 6 conventions | impossible (≥ 12 lignes distinctes nécessaires) | campagne 22/09 §2.1 |

### 2.2 Clés structurées et générateurs

| Ce qui a été testé | Portée | Statut | Où |
|---|---|---|---|
| Clé ligne + colonne (« ID BY ROWS », codes matriciels 2D) | algébrique ; **précision 23/09 : avec un alphabet QUELCONQUE (QIII)**, éliminé seulement aux largeurs Vig [6, 9, 15, 19, 21, 38, 42, 45] et Beau [5, 6, 9, 10, 19, 20, 21, 38, 40, 42] (CP-SAT, `audits/rowcol_beau_2026_09_23/`) ; ailleurs compatible mais indécidable (hasard 60–100 %) | éliminé (alphabets fixes) ; **partiel** (alphabet libre) | dépôt `e_s_10_additive_grid_key`, `e_solve_17`, `e_solve_19` ; **doublon** : campagne 22/09 §2.1 |
| Clés progressives, polynomiales, Fibonacci, récurrences | Bean | éliminé | `e_s_90`, `e_s_84`, `e_s_50`, `e_recurrence_00` |
| Gromark / clé numérique en chaîne | + colonnes largeur 7 | éliminé sous portée | `e_s_67` ; carnet utilisateur (forme canonique, bases 3–12) |
| **Gromark, base 10, amorce de 5 chiffres, alphabets clair et chiffré libres** | espace entier examiné (inégalités + base de Gröbner) | **39 amorces compatibles** avec les 24 lettres (dont 26717 ≡ 84393). Plaintexts « proches de l'anglais » trouvés par recuit, aucun convaincant. **Non éliminé** | Bean 2021, §3 ; code https://github.com/RichardBean/k4testing |
| Gromark, autres bases/longueurs | base 10 à 4 chiffres : 3 amorces (3301, 6740, 9903) ; base 8 à 5 chiffres : 4 amorces (dont 00351, 00537, période 84) | ouvert | Bean 2021 |
| ✔ **Désaccord résolu (23/09)** | Les deux résultats sont justes, leurs portées diffèrent. Le carnet fixe l'alphabet clair à A–Z : c'est impossible (clair R en 27 et clair C en 72 donnent tous deux P, d'où k72 − k27 ≡ 15, hors de portée de chiffres 0–9 ; Bean le note aussi). Bean laisse les **deux** alphabets libres : 39 amorces, **reproduites** avec son `gt.c`. Le registre du dépôt (`two_systems_landscape.md`, « Gromark STRUCTURAL, zéro amorce ») n'est valable qu'avec des alphabets fixés | `audits/gromark_scope_2026_09_23/` |
| Gromark base 10, amorce de 5, **un côté fixé** par un alphabet de l'œuvre : normal, KRYPTOS, mot-clé PALIMPSEST ou ABSCISSA, ou leurs inverses ; l'autre côté libre | 100 000 amorces × 16 conventions, plus les couples AZ/KA | **éliminé (0)** ; contrôles positifs 5/5. **Mais** 0 amorce sur 200 alphabets aléatoires aussi : fixer un côté suffit presque toujours à exclure. Le Gromark ne survit qu'avec **deux alphabets étrangers à l'œuvre connue**, cas que 24 lettres ne tranchent pas | `audits/gromark_scope_2026_09_23/` |
| **Gromark amorcé par des dates** (1986, 1989 et 10 combinaisons, jusqu'à 8 chiffres), **tout alphabet** (QI–IV), Vig/Beau, exact et une erreur tolérée | 96 cas ; contrôle : amorces de Bean retrouvées | exact : **0** ; une erreur : 1 cas, au niveau du hasard (10 %) ⇒ **éliminé** | `audits/gromark_dates_2026_09_23/` (23/09) |
| **Clé par paliers** (constante sur m lettres puis +s ; m 1–13, toutes phases, s 1–25), QI/II/III **tout alphabet**, Vig/Beau ; motivé par « KKKL » dans la clé de Beaufort A–Z | **0 / 13 650** ⇒ **éliminé** ; « KKK » : 10,7 % des chiffrés aléatoires ont un triplé dans l'une des 12 clés (23/09) | `audits/stepped_key_2026_09_23/` |
| Clé interrompue, pas dépendant des données | 8 modèles | éliminé | `e_interrupted_key_vig_01` |
| Chiffre soviétique en trois étapes, VIC, Ubchi | — | éliminé sous portée | `e_soviet_threestep_01`, `e_full_vic_pipeline_k4`, `e_ubchi_null_insertion_01` |
| Wichmann-Hill (PRNG) | graines 0–50 | éliminé | Numberworld |

### 2.3 Clés courantes (running key)

| Source de clé | Portée | Statut | Où |
|---|---|---|---|
| Textes K1, K2, K3 (clairs) | AZ/KA, puis alphabets mélangés de mots-clés (672 M de vérifications), puis avec transposition largeur 1–10 (47 G) | éliminé | `blitz_plaintext_archaeology2`, `e_audit_07_k3_running_key`, `e_aaa_runkey_bijection_08b/08c` ; **doublon partiel** : campagne 22/09 (avec *tout* alphabet) |
| K1–K3 + routes de K3 et grilles 7/14/21 | 7 272 combinaisons, AZ/KA | éliminé (max 8/24 = hasard) | campagne 22/09 `route_runkey_fixed.py` |
| Carter, *The Tomb of Tut-ankh-Amen* (vol. 1, Gutenberg) | décalages, 52 routes | éliminé sous portée | `f_non_direct_alignment_carter_tape`, `f_carter_gutenberg_running_key_nondirect` |
| Corpus égyptiens, textes thématiques, italien/espagnol | + colonnes largeur 7 | éliminé (0 sur 17 milliards) | `e_s_103`, `e_diana_constant_rk_01`, `e_egypt_01`, `e_team_italian_spanish_scan` |
| Texte Morse K0 | — | éliminé sous portée | `e_k0_running_key_01` |
| **Inscriptions de Langley** (devise Jean 8:32 du hall, avec/sans référence ; Memorial Wall), toutes phases | alphabets fixés : max 5/24 ; tout alphabet (QIII) : 0 phase ⇒ **éliminé** (23/09) | `audits/langley_texts_2026_09_23/` |
| Clé courante générique « livre » | — | **ouvert** (non réfutable sans connaître le texte) | Numberworld ; MEMORY (clé courante « rétrogradée ») |

### 2.3 bis Diagnostic statistique de Bean (2021)

| Constat | Valeur | Conséquence tirée par Bean |
|---|---|---|
| Bigrammes verticaux répétés en largeur 21 (11 sur 76) | 1 permutation aléatoire sur 6 750 | propriété probablement « causale » du chiffrement |
| « Différences mineures » (Materna) : lettres claires de {K,R,Y,P,T,O,S} proches de leur chiffré dans l'alphabet standard | 1 sur 5 520 | alphabet chiffré « proche » de A–Z (mot-clé ?) |
| Distances entre les lettres chiffrées d'une même lettre claire (moyenne 3,6 ; 10 sur 13 < 5) | 1 sur 240 / 1 sur 310 | **correspondance un-à-un, pas de transposition** |
| Playfair sérié en largeur 21 et 7 | exclus par les cribs (ZT, BQ) | — |
| Hill | argument contre : 97 est premier | — |

### 2.4 Transpositions et routes

> **Note (23/09, corrigée le soir) :** Sanborn affirme la correspondance lettre à lettre (CNN 2019), mais **recule** la même année (déjeuner de mars 2019 : il ne s'engage que sur « 97 caractères » et « BERLIN au 64ᵉ »). La transposition **pure** reste impossible (comptes de lettres) ; une transposition **locale + substitution** n'est défavorisée que par la statistique de Bean, pas par une parole fiable.

| Ce qui a été testé | Portée | Statut | Où |
|---|---|---|---|
| Grilles et routes rectangulaires | jusqu'à 20×20 | éliminé sous portée | `e_grid_route_20x20`, `e_route_definitive` |
| Colonnes simples, doubles, Myszkowski, AMSCO, Nihiliste | largeurs usuelles | éliminé sous portée | `dragnet_v4`, `e_s_06`, `e_s_19`, `e_frac_46..48`, `e_s_53` |
| Transposition puis substitution périodique (« TABP ») | 6 165 transpositions + 252 840 composées, AZ/KA, p 1–50 | éliminé | `f_tabp_*` |
| Alignement libre (cribs n'importe où) | 6 millions de configurations, et multicouche | éliminé sous portée | `f_free_alignment_classical`, `f_solver_free_alignment` |
| **Route de K3 appliquée à K4** | rotation style K3 | bruit (4/24) | `e_cfm_07_k3_rotational`, `e_hybrid_04_reverse_k3`, `e_s_58` ; **doublon élargi** : campagne 22/09 §2.5 (les 102 variantes exactes par multiplication modulaire, tout alphabet, p 1–26) |
| **Serpentin (boustrophédon)** par lignes ou colonnes, largeurs 7–31, avant ou après la substitution, **cribs = positions du clair**, Quagmire III tout alphabet, p 1–26 | K4 au niveau du hasard ou en dessous (témoin global : 5 chiffrés aléatoires sur 6 ont autant ou plus de cases compatibles) ; déchiffrements en charabia ⇒ **éliminé en pratique** (23/09) | `audits/serpentine_2026_09_23/` |
| Colonnes KRYPTOS + Quagmire III, alphabet inconnu | p 1–26 | pas de signal | campagne 22/09 §2.3 |
| Grille 98 = 14×7 avec le « ? » | — | éliminé sous portée | dépôt `e_s_130_checkpoint_98char`, `e_s_03`, `e_s_04` ; **doublon** : campagne 22/09 §2.4 |
| Routes selon la rose des vents | 576 configurations | bruit (4/24) | `e_compass_route_01` |
| Grilles irrégulières de largeur 7, 14, 21 (97 lettres), lecture par colonnes vers le haut ou vers le bas, directe ou inverse, + Quagmire III, alphabet inconnu, p 1–26 | 48 des 80 configurations (1 lot sur 4 perdu lors d'un redémarrage, non relancé) | pas de signal : 11 compatibilités à période courte pour ≈ 19 attendues par hasard | campagne 22/09, `ragged_widths.py`, `results_ragged_widths_summary.json` |
| Grilles tournantes, grilles de Cardan, Fleissner | — | bruit | `e_s_18`, `e_s_70`, `blitz_rotation_180`, `blitz_grille_*` |
| Carré latin (Swagman), bandes (strip cipher) | — | bruit | `e_swagman_01`, `blitz_strip_*` |
| **Réglettes M-138 découpées dans le tableau KRYPTOS** (lignes ou colonnes) | chaque réglette = KA décalé ⇒ équivaut à un décalage constant par bloc de 30 ; les T des positions 24 et 28 (même bloc) donnent V et R ⇒ **impossible** (23/09, sans calcul). Réglettes aléatoires : invérifiable (au-delà de 112 bits) | raisonnement |
| Paires digraphiques 8×13 → 31×3 | — | revendication non validée | Nash Associates (2025) |

### 2.5 Masques, nulles, stéganographie

| Ce qui a été testé | Statut | Où |
|---|---|---|
| Palette {B,G,I,K,O,W,Z} et masques de nulles dérivés | **RETIRÉ** (artefact) | `C-PALETTE-01`, `stego_mechanism/*` |
| Masques de nulles géométriques, par colonnes, grille 28×31 | éliminé sous portée | `e_two_sys_06..08` |
| Morse : « E » en surnombre comme masque | **registre incomplet** (scripts jamais exécutés) | F-10 |
| **Tailles des groupes de E du Morse (26 E) comme clé numérique** (4 ordres, toutes phases, QI/II/IV tout alphabet ; QIII déjà mort à p = 11) | **éliminé** : K4 0, témoin 0/50 (23/09) ; règle « intervalles » non définie | `audits/morse_E_key_2026_09_23/` |
| **« Miroir » Morse** (lettres lues à l'envers, A↔N, B↔V…) avant/après un Quagmire ; C, J, Z sans image | QIII : aucune p ≤ 12 ; seul phénomène sévère p = 19 (distance 38), déchiffrement en charabia ⇒ **éliminé** (23/09) | `audits/morse_mirror_2026_09_23/` |
| **Substitution partitionnée** (clé selon position ET lettre claire ∈ KRYPTOS ou non ; anomalie Materna), QIII tout alphabet, p 1–26 | compatible seulement là où le hasard passe, sauf p = 19 (phénomène distance 38, charabia) ⇒ **aucun signal** (23/09) | `audits/langley_texts_2026_09_23/` |
| Masques issus des anomalies physiques | bruit (5/24) | `e_team_anomaly_extraction` |

### 2.6 Idées thématiques et physiques

| Ce qui a été testé | Statut | Où |
|---|---|---|
| Horloge de Berlin (Mengenlehreuhr, Weltzeituhr) comme route ou sélecteur | registre du dépôt actif (non conclu) ; mruckman1 : 0/48 masques UTC | `thematic/berlin_clock/*` ; [MR] |
| Mots-clés « Operation Gold », tunnel de Berlin, cryptonymes CIA | éliminé sous portée | `e_opgold_*`, `e_cryptonym_crib_screen_01` |
| Coordonnées, dates, abscisse | éliminé sous portée | `e_s_60`, `e_roman_03b/04b`, `e_k2_coords_transposition` |
| Fautes d'orthographe (deltas) comme clé | éliminé | `e02_misspelling_deltas`, `e_bespoke_03` |
| Différences Antipodes / Kryptos | éliminé sous portée | `e_antipodes_11/12` |
| Superposition cuivre ↔ tableau (lettre en face) | forme naïve : 1/24. **23/09 :** l'écran replié comme un livre et le miroir donnent 0 à 2 sur 24. **Fermeture générale** : toute superposition à lignes parallèles, lettre du tableau = clé (quels que soient décalage, appariement des lignes, miroir), exige une clé en progression ±1 dans KA sous EASTNORTHEAST (qui tient sur une seule ligne). Aucune convention ne la donne : **classe éliminée**. Restent ouvertes : superposition tournée, gabarit de sélection, gabarit extérieur | carnet utilisateur ; F-07 ; `audits/fold_overlay_2026_09_23/` |
| **Batterie positionnelle déterministe** : lettre du tableau au dos (40 géométries : replié, miroirs, rotation 180°, décalages de ligne, alignements linéaires début/fin) → 15 fonctions simples (rang, − étiquette de ligne, ± colonne/ligne/position, distance au bord…) → clé ; alphabets fixés A–Z/KRYPTOS, 12 conventions | K4 6/24 ; hasard P(≥ 6) = 56 % ⇒ **éliminé** (23/09) | `audits/positional_battery_2026_09_23/` |
| **Lettre du tableau au dos, même position, comme sélecteur** (clé = g(lettre), g inconnue ; FOLD/miroir, d = −2…+2), QIII/QIV tout alphabet | QIII au niveau du hasard (11 contre 10,5) ; QIV : 25 % des chiffrés aléatoires passent tout, comme K4 ⇒ **aucun signal, peu décidable** (23/09) | `audits/tableau_lookup_2026_09_23/` |
| **Plaque du haut (K1–K2) posée sur la plaque du bas (K3–K4)**, glissée ou rabattue, colonnes par rang ou justifiées, lettre du dessus = clé (7 relations) | 0 à 3 sur 24 : **éliminé** (23/09) | `audits/plate_overlay_2026_09_23/` ; antériorité partielle `e_k3k4_overlay_k1k2` |
| **Texte intermédiaire de la feuille de K3 (24 × 14) comme clé**, K4 empilé dessous (7 × 14, « ? » des deux façons) ; 4 sens de lecture, tous décalages | alphabets fixés : max 6/24 ; tout alphabet : 0 ; témoin 0/8 ⇒ **éliminé** (23/09) | `audits/k3_chart_stack_2026_09_23/` |
| **Texte CHIFFRÉ de K3 comme clé de K4**, linéaire (tout décalage) ou ligne par ligne (d = 1–11), Quagmire I–IV tout alphabet, clé en A–Z/KRYPTOS/même alphabet, Vig/Beau | K4 : 0 ; témoin aléatoire : 0/0/4/68/0 ⇒ **éliminé** (23/09) | `audits/k3ct_key_2026_09_23/` |
| **Une erreur de Sanborn tolérée dans les cribs** (23/24) : Quagmire III tout alphabet p 1–13 ; clés suivies K1–K3 tout alphabet | **toujours éliminé** : les éliminations classiques résistent à une erreur | `audits/one_slip_2026_09_23/` |
| Mot-clé **recommencé à chaque ligne de 31** (feuille ou cuivre), Quagmire I–III, p 1–13, exact et avec une erreur | bruit (K4 : 3 cases sur 26 ; hasard : 2 en moyenne) ; la case p = 8 Beaufort meurt avec les alphabets de Sanborn | `audits/one_slip_2026_09_23/` |
| **Cryptographe de Wheatstone** (cadran à deux aiguilles), extérieur AZ ou KA, **intérieur quelconque** | **éliminé exactement**, même sur EASTNORTHEAST seul ; contrôle 20/20 | `audits/wheatstone_2026_09_23/` ; antériorité échantillonnée `e_wheatstone_clock_01` |
| **Mots visibles sur l'œuvre** (39 : Morse K0, KRYPTOS, HILL, YAR/DYAHR, rose des vents, PALIMPSEST, ABSCISSA) comme alphabet à mot-clé et/ou clé périodique, montage MEDUSA inclus, QI–QIV, 3 modes ; et comme amorces de Gromark | K4 10/24 ; hasard P(≥ 10) = 7 % ; Gromark 0 ⇒ **éliminé** (23/09) | `audits/visible_words_2026_09_23/` |
| **Codage 5 bits (A1Z26, A0Z25, Baudot ITA2) + XOR / addition mod 32**, clé périodique p 1–48 (« changer la base », Scheidt 2020) | seul cas non trivial : ITA2 XOR p = 30, niveau du hasard (0,54 attendu), déchiffrement avec 13 codes non-lettres ⇒ **éliminé** (23/09) | `audits/binary_xor_2026_09_23/` |
| **Transposition de bits** (5 bits), une partie des bits laissée en place, substitution simple avant ou après (J. DeSeve, 2007) | Contrôle **à la main**, pas encore à la machine. La variante de DeSeve (bits 1–2 fixés, puis substitution) tombe sur deux paires des cribs : F (E/O) et P (R/C). Tout autre choix de bits fixes, en A = 0 ou A = 1, tombe sur trois paires. Reste la transposition de **tous** les bits, qui produit des valeurs sans lettre (26–31) : peu plausible | base 7 §10.6 (protocole) |
| **Chaque crib seul** (aucun lien entre les deux) : clés nécessaires avec alphabet fixé (A–Z/KRYPTOS, 3 conventions) ; périodique QIII tout alphabet par crib | clés = charabia (aucun mot-clé) ; Vig éliminé pour toute période < longueur du crib (sauf p = 12, hasard 86 %) ; Beau au niveau du hasard ⇒ **aucun signal** (23/09) | `audits/cribs_separately_2026_09_23/` |
| **Crib décalé** (lettre sautée ou ajoutée entre les cribs, décalage de phase s = −3…+3), Quagmire I–IV tout alphabet, p 1–26 | QIII : p ≤ 12 éliminé pour tout s ; les cases QI/II/IV « sévères » relèvent toutes d'un seul phénomène (distance 38 entre cribs), au niveau du hasard (≈ 10 %) ⇒ **aucun signal** (23/09) | `audits/crib_shift_2026_09_23/` |
| **Catalogue « 20 hypothèses »** (relais 23/09, nuit) : Polybe 5×5 impossible (K4 a I **et** J) ; coordonnées de K2 comme clé ; permutations modulaires sur 97 (affines toutes, multiplicatives, forme K3, exponentielles/log) avant/après substitution ; générateur congruentiel ; colonnes DYAHR/YAR ; « ID BY ROWS » + lecture K3 ; partition îlots fermés ; décalage par ligne physique | tous **éliminés ou au niveau du hasard** ; permutations mod 97 tout alphabet : K4 **en dessous** du témoin global (23/09) | `audits/twenty_hypotheses_2026_09_23/` |
| **Textes physiques de la sculpture comme clé courante** (tableau avec/sans étiquettes, panneau chiffré, Morse), **13 parcours 1D** (lignes, colonnes, boustrophédons, diagonales, sens inverses) et **2D sur cylindre** (K4 à sa place, clé en (ligne+dr, col+dc), miroirs, rotation) ; σ quelconque + 24 conventions à alphabets fixés | 57 617 placements : **0** ; fixés max 8/24 = hasard ⇒ **éliminé** (24/09) | `audits/vision_2026_09_24/` T1 |
| **Addition en chaîne** (4 règles, base 10 amorces 2–6, base 26 amorces 2–5), QIII σ quelconque | **0** (hasard 0) ⇒ **éliminé** ; en QIV, K4 = hasard : **les 39 amorces de Bean = 40 attendues pour des clés de chiffres aléatoires** (24/09) | `audits/vision_2026_09_24/` T2 |
| **Deux mots-clés superposés** k = a[i mod p1] + b[i mod p2] (cellule OPEN de `two_systems_landscape.md`), alphabets A–Z/KRYPTOS, 12 conventions | **éliminé pour p1+p2 ≤ 24** (aucun couple compatible) ; indécidable au-delà (K4 = hasard) (24/09) ; **alphabet quelconque** (p1+p2 ≤ 20, 54 couples) : éliminés VIG 28, VARB 28, BEAU 18 (petits couples : tous les (2, p2) en VIG et VARB, et (3,4), (4,5), (5,9) partout ; listes dans l’audit) ; les compatibles (VIG 20, BEAU 23, VARB 19, dont KRYPTOS+PALIMPSEST 7+10) le sont au niveau du hasard (30–100 % des témoins), 0 sévère ; 6/13/7 non tranchés (limite) | `audits/vision_2026_09_24/` T3, T4 |
| **Pliure au centre** (cribs en miroir i ↔ 96−i) : clé = clair en miroir | **éliminé** (test puissant, témoin 0/200) ; demi-pliures et chiffré réfléchi : au niveau du hasard (24/09) | `audits/vision_2026_09_24/` T5 |
| **Journal de fouilles de Carter (1922)** et **inscriptions du hall de la CIA** (directeurs, Donovan, Dulles, *Book of Honor*, relevés NSA 1991) comme clé courante | **0** partout ⇒ **éliminé** (24/09) | `audits/vision_2026_09_24/` T6 |
| **Textes « enfouis » ou « fondateurs » du site** comme clé courante : directive Truman du 22/01/1946 (scellée dans la boîte de cuivre de la pierre angulaire de la CIA), principes du concours artistique CIA de 1988, lettre de Sanborn de 1989 ; à l'endroit et à l'envers, tous décalages | **0** partout (σ quelconque) ; alphabets fixés max 8/24, comme les témoins ⇒ **éliminé** (24/09) | `audits/vision_2026_09_24/` T7 ; base 7 §4 |
| **« Chaîne de masquage » proposée sur le groupe en 2017** : développement décimal de 1/563 (période 281), comme clé numérique additive (un chiffre, ou paires mod 26), tous décalages | **0** compatible (σ quelconque) ; fixés max 6/24, comme les témoins ⇒ **éliminé** pour l'emploi direct (24/09) | `audits/vision_2026_09_24/` T8 ; base 7 §7.3 |
| **Addition en chaîne à amorce de 7** (« fonction régénérative », Scheidt 2011 ; doublets mod 7) : base 10, amorces de 7 chiffres (10⁷), règles R1–R4 de T2 ; base 26, amorce **KRYPTOS** (rangs A–Z ou KRYPTOS, endroit/envers) | Quagmire III tout alphabet : **0** ; Quagmire IV : comptes égaux à l'attendu i.i.d. (≈ 3 800) ; amorce KRYPTOS : 0, fixés max 5/24 ⇒ **éliminé** (QIII) (24/09) | `audits/vision_2026_09_24/` T2 (option 7), T9 |
| **« Weltzeituhr Error Key »** (2025) : lignes 1–2 par décalages de César tirés de OBKR, ligne 3 par initiales de villes de l'horloge | **Impossible** : FLRV→EAST exige 1, 11, 25, 2 ; NYP→BER exige 12, 20 ; la ligne 3 est ajustée au crib (24/09) | base 7 §7.5 |
| **Maquette GIRASOL de 1988** comme source de clé : tableau GIRASOL (15 et 26 lignes), table ABC inversée, bloc chiffré 7 × 21 et son clair ; 13 parcours 1D, 2D sur cylindre avec K4 à sa place physique et en lignes de 21 ; alphabets fixés A–Z, KA et GIRASOL | **54 188 placements : 0** (σ quelconque) ; fixés max 8/24, témoins 8–10/24 ⇒ **éliminé** (24/09). Le bloc de démonstration de la maquette se lit en Vigenère A–Z, clé **RUG** | `audits/vision_2026_09_24/` T10 ; base 7 §8 |
| **Toute substitution qui dépend de la position modulo 7** (alphabets quelconques, même sans lien entre eux ; y compris « par ligne de 21 ») | **Impossible sans calcul** : en 65 et 72 (même classe, même ligne de 21), chiffré P les deux fois, clair R puis C (24/09) | base 8 §5.3 ; `stats_7.py` |
| **Autoclé sur le chiffré à l'écart 7**, additive, alphabet quelconque (hypothèse NSA pour l'« intervalle 7 ») | **Impossible sans calcul** : c[i] = c[i−7] exige clair[i] = « 0 » ; or c'est le cas en 22 (A) et en 72 (C) (24/09) | base 8 §5.3 |
| **Deux alphabets alternés par paires** (« 00++00++ », Rainer, groups.io) ; plus largement, 2 ou 3 substitutions quelconques réparties selon i mod p, p ≤ 12 | 0011 : **incompatible** avec les cribs ; seuls 4 motifs de période 11 à 3 classes passent, contre 6,7 en moyenne pour un chiffré aléatoire ⇒ **éliminé / indécidable** (24/09) | base 7 §8 |
| **Modèle à deux couches d'E. Cruz** (2022) : substitution monoalphabétique puis décalage périodique de 7 (A–Z) | Ajusté sur EASTNORTHEAST seul (14 inconnues pour 13 lettres) ; **incompatible avec BERLINCLOCK**, comme l'auteur l'a lui-même constaté (paire 65/72) ⇒ **éliminé** sous sa forme unique | base 7 §8 |
| **« Percée » de S. Perry (2013)** et « familles BERLIN » de R. Thompson (2015) : Quagmire III de clé EMUFPHZLRFA + lecture par sauts de 38 depuis 77 | BERLIN en 63–68 par effet de recherche multiple (l'auteur lui-même attendait 1 à 2 coïncidences ; LONDON apparaît aussi) ; l'intermédiaire n'a ni EASTNORTHEAST (21–33 : JAILWDCCNZFHS) ni CLOCK (69–73 : PEEKR) ⇒ **sans valeur** (24/09) | base 7 §8 |
| **Disque tourné une fois par bloc de 7** (T11) : clé = décalage libre par bloc de 7, à toute phase, + motif fixé dans le bloc (pas constant s = 0..25, dont KRYPTOS lu dans KA ; KRYPTOS lu dans A–Z) ; couvre la clé progressive avec saut tous les 7 ; alphabet quelconque | **588 cas : 0 compatible** (1,7 attendu au hasard ; contrôle positif reconnu) ⇒ **éliminé** (24/09) | `audits/vision_2026_09_24/` T11 |
| **Autoclé mixte** (T12) : lettre-clé (chiffré ou clair, L avant) lue dans un alphabet fixe (A–Z, KRYPTOS, inverses), tableau à alphabet quelconque ; L = 1–96 | 112 cas compatibles sur 1 080, contre **211 attendus** au hasard ; tous au niveau des témoins ⇒ **aucun signal** (24/09) | `audits/vision_2026_09_24/` T12 |
| **Chiffre de Hill par blocs** (T13) : n = 2–8, toute phase, coefficients {0,1}, {−1,0,1}, {0,1,2}, trois numérotations, deux sens | n = 2–6 : **0 compatible** ; 7 × 7 binaire (essai de Bean 2018) **incompatible partout** ; seuls passent des cas lâches, comme les témoins ⇒ **éliminé** pour les petits coefficients (24/09). Une matrice quelconque n'est pas testable avec 24 lettres | `audits/vision_2026_09_24/` T13 |
| **Disque tourné par bloc de n lettres** (T11b), n = 2–30, toute phase, pas s·j, alphabet quelconque | 0 compatible pour n ≥ 5 ; n = 2–4 au niveau du hasard ⇒ **éliminé / aucun signal** (24/09) | `audits/vision_2026_09_24/` T11b |
| **Clé transposée comme le texte de K3** (T16) : mot-clé (L = 1–14) écrit en lignes de 7, relu par colonnes dans l'un des 5 040 ordres (dont KRYPTOS), ou l'inverse ; alphabet quelconque | **L ≤ 7 : 0 compatible** ; L = 8–14 : moins de compatibilités que pour un chiffré aléatoire ⇒ **éliminé (L ≤ 7) / aucun signal** (24/09) | `audits/vision_2026_09_24/` T16 |
| **Autoclé « vers l'avant »** (T17) : clé = clair ou chiffré L positions plus loin, lue dans σ ou dans un alphabet fixe ; L = 1–96 | 179 cas compatibles sur 1 380, contre 199 attendus ⇒ **aucun signal** ; à l'écart 7, indécidable (35–70 % des témoins passent) (24/09) | `audits/vision_2026_09_24/` T17 |
| **Autoclé sur le clair avec chaînes entre cribs** (T18) : relation linéaire propagée de L en L à travers le clair inconnu ; L = 1–96, clé avant ou après, alphabet quelconque | **Écart 7, sens classique (hypothèse NSA 1992) : impossible dans les 3 conventions** (29 à 57 % des témoins passent) ; le Beaufort, laissé compatible le 22/09, tombe grâce aux chaînes. Tous écarts : au niveau du hasard ⇒ **éliminé (écart 7) / aucun signal** (24/09). **Réserve T19** : avec une seule erreur de chiffrement (une équation de chaîne retirée), K4 redevient compatible dans les 3 conventions, comme 78 à 97 % des témoins ⇒ élimination valable **sans erreur seulement** | `audits/vision_2026_09_24/` T18, T19 |
| **Robustesse de T18 à une erreur** (T19) : chaque équation de chaîne retirée à tour de rôle (une lettre mal chiffrée ne se propage pas en autoclé sur le clair) ; écart 7, 3 conventions, alphabet quelconque | K4 compatible après un retrait : Vigenère 1/17, Beaufort 4/17, variante 1/17 ; témoins avec au plus une erreur : 78 %, 97 %, 93 % ⇒ **T18 non robuste** ; motif : le petit fragment de Sanborn (97 lettres, méthode de K1) a 4 erreurs (base 7 §9.4) | `audits/vision_2026_09_24/` T19 |
| **Autoclé sur le clair, alphabets à mot-clé, erreurs = équations de chaîne fausses** (T27, 25/09) : 202 172 alphabets, 5 types, 3 lectures de la clé, 3 conventions, L = 1–48 ; modèle « copie » en 32 et 73 | Écart 7 : **au moins 10 erreurs** (témoins 9–10) ; aucun cas sauvé par une copie ; tous écarts : 2, comme les témoins ⇒ **éliminé** pour les alphabets à mot-clé. Corrige le comptage de `kwautokey.c` et de `k4x -e a` (une erreur y comptait plusieurs fois) | `audits/relais_2026_09_25/` |
| **Trifide (3 × 3 × 3) à toute PHASE** (T28, 25/09) : périodes 2–40, blocs commençant n'importe où (dont 4, début de la première ligne pleine du cuivre) ; cube quelconque | **0 compatible** sur 819 cas (témoins 0/200, contrôle positif 20/20) ; en simulation, pas de concentration des doublets ⇒ **éliminé**, confirme E-S-09 pour toutes les phases | `audits/relais_2026_09_25/` T28 |
| **Disque tourné de temps en temps, toute phase** (T29, 25/09) : k = m[(i−φ) mod 7] + a[(i−φ) div 7], φ = 0–6 ; 101 086 alphabets à mot-clé, 5 types | K4 : au moins 5 erreurs, comme les 20 témoins ⇒ **éliminé** (alphabets de Sanborn) | `audits/relais_2026_09_25/` T29 |
| **Clé périodique avec saut de phase dans un crib** (T30–T31, 25/09) : sauts ±1 à ±3 en x = 22–73 (un ou deux) ; p ≤ 13 | Alphabet quelconque : 115 cas compatibles pour 491 attendus ; alphabets à mot-clé : au moins 3 erreurs, comme les témoins ⇒ **aucun signal** | `audits/relais_2026_09_25/` T30, T31 |
| **Clé courante en allemand, français, latin** (25/09), alphabets à mot-clé | p = 0,76, 0,24, 0,71 (contrôle allemand retrouvé) ⇒ **aucun signal** | `audits/relais_2026_09_25/` |
| **Clé progressive, alphabets à mot-clé** (T32, 25/09) : mot-clé de p lettres qui avance de s à chaque tour, p = 2–26, s = 1–25, toute origine du tour | p ≤ 11 : plus de 5 erreurs ; p = 12–13 : 5 (témoins 3–5) ⇒ **aucun signal** | `audits/relais_2026_09_25/` T32 |
| **Toute clé numérique 0–9 (Gronsfeld, Gromark toute amorce, dates, coordonnées), alphabets à mot-clé** (T33, 25/09) | Au mieux 3 valeurs de clé hors 0–9 sur 24 (témoins 2–5) ⇒ **aucun signal** | `audits/relais_2026_09_25/` T33 |
| **Fautes de recopie d'une feuille de 7 colonnes** (T35 : lettres répétées = copies ; masque M = {22, 26, 33, 68, 72} désigné sans ajustement), clés périodiques p = 1–26 | Alphabet quelconque : 16 cas compatibles, aucun à p = 7 (masques au hasard : 18,9). Alphabets à mot-clé : p = 7 exige encore 7 erreurs ; Σ e_min (p ≤ 13) moins bon que 193 masques au hasard sur 200 ⇒ **éliminé** avec une clé simple dessous | `audits/procedes_2026_09_25/` T35 |
| **Borne des doublets** (tout chiffre déchiffrable ; anglais Gutenberg) | Un doublet = une « prédiction » de la lettre claire suivante. **Clé indépendante du texte, quelle qu'elle soit** : taux ≤ 6,7 % (A–Z, KRYPTOS), 13,6 % (meilleur mot-clé), 17 % (alphabets libres) ⇒ 5 doublets sur 14 en colonne 4 avec ceux des cribs : 0 / 0,9 % / ≤ 6 %. Seule une clé dépendant du clair précédent et accordée à l'anglais y parvient ⇒ **aucune famille de clés n'explique les doublets alignés** | `audits/procedes_2026_09_25/` |
| **Recherche automatique de procédés** (≈ 20 000 procédés manuels déchiffrables, un ou deux termes, KRYPTOS et A–Z, 5 000 simulations chacun, deux contrôles positifs réussis) | Les meilleurs ne font que forcer SS en 32 à partir du crib (coïncidence arithmétique attendue). **Aucun ne produit la concentration** (≤ 11 % de doublets en colonne 4 contre 36 % ; signature jointe ≤ 2 × 10⁻⁶) | `audits/procedes_2026_09_25/` |
| **Simulateur Sanborn** : 52 procédés manuels × 10⁶ faux K4 (anglais, cribs insérés) ; ressemblance aux 97 lettres (écart 7, écarts 14/21, doublets par classe mod 7, doublets des cribs) | Le « 7 » de K4 ne relie que des voisins (écarts 14–49 au hasard) : **aucune clé périodique**. Seule l'autoclé sur le clair à l'écart 7 en **forme Vigenère** reproduit ce profil (4,9 % contre 0,29 % au hasard) ; Beaufort et Variante non. **Aucun procédé positionnel ne concentre les doublets** ; signature complète ≤ 10⁻⁵ | `audits/simulateur_2026_09_25/` |
| **Autoclé sur le clair, écart 7, Vigenère, deux alphabets à mot-clé** (clair σ, chiffré τ, décalage ; Quagmire IV, T34) | 5,3 × 10¹¹ paires, recherche exacte sur 17 équations de chaîne, clair entier lu. **K4 : au moins 7 équations fausses** (témoins 5–7), clairs illisibles ; contrôle positif retrouvé (0 fausse). Éliminé, même avec le taux d'erreur de Sanborn (7 fausses ou plus : 0,2 %) | `audits/simulateur_2026_09_25/` T34 |
| **Moteur unique, 30 témoins K4 mélangé** (T1, 25/09) : p7 pure / + ligne du cuivre / + ligne de 7, blocs n = 5–14, T16, autoclés, clé courante, conventions par ligne ; **1 395 188 alphabets** (mot-clé linéaires + matrices de largeur 2–13) × 5 types × 3 conventions | K4 dans la distribution des témoins pour les 11 familles (p = 0,03 à 1). Seul écart, autoclé à 3 erreurs (EUODOS), déchiffré : charabia (−7,08) | `audits/moteur_2026_09_25/` §1 |
| **Dictionnaire élargi** (T2 : 430 167 mots, noms propres, 34 000 villes, 258 termes thématiques) et **deux mots-clés** (2 960 415 alphabets) | Aucun signal (p = 0,13 à 1) | `audits/moteur_2026_09_25/` §2, §7 |
| **Clé périodique p = 1–14 avec le modèle d'erreur observé chez Sanborn** (glissement, copie, décalage commun ; T3), 1,4 M alphabets ; p ≤ 26 pour Q1–Q4 (T6) | Contrôle : les 4 erreurs du petit fragment sont reconnues. K4 : e_min ≥ 5 (p ≤ 14), jamais ≤ 3 erreurs « à la Sanborn » ⇒ **éliminé** ; p = 15–26 au niveau des témoins | `audits/moteur_2026_09_25/` §3, §7 |
| **Transposition à colonnes + substitution périodique** (TABP, hypothèse NSA 1992, K3 + K1) : 271 138 ordres (toutes permutations w ≤ 8, mots w = 9–20), routes K3, p ≤ 13, KRYPTOS/A–Z | K4 : e_min ≥ 5, témoins pareil ; contrôle retrouvé ⇒ **éliminé** dans cette portée | `audits/moteur_2026_09_25/` §4 |
| **Quagmire IV à deux alphabets à mot-clé** (497 × 497 thématiques ; 76 × 101 086) : p7, blocs, T16 | Aucun signal | `audits/moteur_2026_09_25/` §4 |
| **Attaque sur le texte entier depuis les clés EXACTES des cribs** (T5 : énumération par cycles du graphe des lettres, puis recuit) ; p7 + ligne du cuivre, alphabet quelconque | Contrôle retrouvé (y compris avec 2 erreurs). K4 : e_min = 2 **prouvé** (T24 disait « 1 ou 2 ») ; avec 32 et 66 retirées, 582 924 clés, meilleur texte −6,38 (charabia) | `audits/moteur_2026_09_25/` §5 |
| **Ligne + colonne (« ID BY ROWS », largeur 2–48) et deux mots superposés (p1 < p2 ≤ 26), alphabets à mot-clé** (T6 ; seules les 190 configurations à redondance ≥ 3) ; autoclé vers l'avant | 1,4 M alphabets : K4 **moins** compatible que les 30 témoins (12 815 cas contre 13 134–22 469) ; autoclé vers l'avant p = 0,32–0,97 ⇒ **aucun signal** | `audits/moteur_2026_09_25/` §6–7 |
| **Clé en récurrence linéaire (LFSR, ordre 1–5)** et **« Fibonacci au pas 7 »** (relais DeepSeek n° 3 et 4) | Aucune récurrence ne couvre les deux fragments de clé (101 086 alphabets) ; colonne mod 7 en récurrence d'ordre 2 : 0 cas (tous coefficients en A–Z/KRYPTOS ; Fibonacci pur, tous alphabets à mot-clé) ⇒ **éliminés** | `audits/relais_2026_09_25/` 5ᵉ–7ᵉ parties |
| **Éliminations rejouées avec une erreur** (T20–T21) : modèle A (erreur quelconque) et modèle B « à la Sanborn » (lettre recopiée en 32 ou 73, ou clé de la lettre voisine) | Périodique : rien ne rouvre pour p ≤ 11 ; p = 12–15 rien trouvé, quelques variantes non tranchées (conforme au registre). **T11, T11b (n = 5–30), T13 (Hill n = 2–6) : au niveau du hasard** avec une erreur. T16 (L ≤ 7) : 6 à 24 cas rouverts, bien sous le hasard (≈ 170 à 650). **T15 (Wheatstone à mot-clé) : toujours 0**, contrôle 20/20 ⇒ seules T15 et la clé périodique courte sont des éliminations robustes | `audits/vision_2026_09_24/` T20, T21 |
| **Nombre minimal d'erreurs, clé périodique** (T22) : p = 1–26, Quagmire I, II, III (VIG, BEAU), IV, alphabets quelconques ; combien de lettres des cribs faut-il déclarer fausses ? 200 témoins par cas | K4 n'est **jamais** plus proche d'un chiffre périodique que le hasard (P(témoin ≤ K4) 0,5 à 1), sauf p = 19 (égalité de Bean, distance 38) ; période 7 : 4 erreurs (Q3 VIG), contre 2,6 au hasard ⇒ **éliminé pour tout nombre plausible d'erreurs** (24/09, soir) | `audits/erreurs_multiples_2026_09_24/` T22 |
| **Autoclé sur le chiffré avec erreurs** (T23), hypothèse NSA 1992 : clé = chiffré L avant, lu dans σ, en A–Z ou en KRYPTOS ; VIG, BEAU, VARB | écart 7 : **au moins 5 erreurs** sur 24 dans les 9 variantes ; écarts 1–15 : jamais moins de 4 ⇒ **éliminé de façon robuste** (24/09, soir) | `audits/erreurs_multiples_2026_09_24/` T23 |
| **Période 7 + décalage d'alphabet à chaque ligne du cuivre** (T24) : les deux conflits de la période 7 (65/72, 24/66) enjambent une fin de ligne | K4 : 1 erreur (Q4, en 28 ; Q3 BEAU) ; témoins Q3 : 26 % compatibles sans erreur ⇒ **trop lâche, aucun signal** (24/09, soir) | `audits/erreurs_multiples_2026_09_24/` T24 |
| **« On retourne la feuille »** (T25) : période 7, convention (VIG/BEAU/VARB) fixée par ligne du cuivre, par ligne de 7 ou en alternance | au mieux 2 erreurs (cuivre), 1 (lignes de 7, 4 motifs sur plus de 500, comme les témoins), 3 (alternance) ⇒ **aucun signal** (24/09, soir) | `audits/erreurs_multiples_2026_09_24/` T25 |
| **Alphabets à mot-clé × clé à pas 7** (`kwsweep`) : 237 988 alphabets (59 497 mots × 4 formes) × Q3/Q2/Q1/Q4a/Q4b × 3 conventions ; clé = période 7 + décalage par ligne du cuivre, par ligne de 7, ou pure | au mieux **7 à 10 erreurs** sur 24, comme les témoins ⇒ **éliminé** pour les alphabets à mot-clé (24/09, nuit) | `audits/motcle_pas7_2026_09_24/` §1 |
| **Autoclé sur le clair, L = 1–13, alphabets à mot-clé** (`kwautokey`) : le clair entier est déterminé depuis les cribs, puis noté ; inclut l'hypothèse NSA (écart 7) | 120 M cas : au mieux **4 erreurs**, meilleur texte −6,38 (charabia) ; témoin : 4 erreurs, −6,52 ; contrôle positif retrouvé (−4,29) ⇒ **éliminé** (24/09, nuit) | idem §2 |
| **Clé courante tirée d'un texte anglais inconnu, un alphabet à mot-clé** (`kwrunkey`) : les deux fragments de clé (13 + 11 lettres) doivent être de l'anglais | K4 −4,98, sous la médiane des témoins « K4 mélangé » (−5,16 à −4,58) ; une vraie clé anglaise dépasse −4,98 dans 99,5 % des cas (84 % avec une erreur). **Deux alphabets à mot-clé** (1,1 G cas) : K4 −4,44, 2ᵉ sur 9 avec les témoins mélangés ; puissance ≈ 80 % ⇒ **éliminé** pour l'anglais continu (24/09, nuit) | idem §3 |
| **Autoclé sur le chiffré, L = 1–96, alphabets à mot-clé** (`kwcak`) : clair déterminé par le chiffré, puis noté | 1 G cas : au mieux 10 lettres des cribs sur 24 (témoin 9), texte en charabia ⇒ **éliminé**, y compris aux écarts longs où T23 est sans puissance (24/09, nuit) | idem §4 |
| **« EGYPTXQUNDIG »** (Reddit, 2026) : clé QUXXLA + TUTEMF tirée des anomalies et d'initiales, alphabet KRYPTOS puis A–Z | calcul juste, mais clé de 12 lettres pour 12 lettres (masque jetable) et choix libres ; aux cribs, la méthode devrait donner RDUMRIYWOYNKY / ELYOIECBAQK, ce qu'aucune de ses sources ne fournit ⇒ **sans valeur** (24/09) | base 7 §12.3 |
| **Doublets = glissements de copie** (T14) : retrait des lettres 26, 33, 68 ou 25, 32, 67 des cribs, clé périodique p = 1–22, Quagmire III tout alphabet | Vigenère : rien de rouvert (retrait 26, 33, 68) ; seuls passent des cas au niveau du hasard, et p = 19 (retrait 25, 32, 67), porté par l'égalité de Bean, à ≤ 1/26 pour un seul cas sur 198 ⇒ **aucun signal** (24/09) | `audits/vision_2026_09_24/` T14 |
| **Cryptographe de Wheatstone à cadran extérieur à mot-clé** (T15) : 704 880 cadrans (vocabulaire des textes lus + liste thématique), deux sens, cadran intérieur quelconque ; motif : un doublet chiffré y signifie deux clairs voisins sur le cadran | **0 compatible** (témoins 0/600 ; contrôle positif 20/20) ⇒ **éliminé** pour les cadrans à mot-clé (24/09). Cadran quelconque : indécidable | `audits/vision_2026_09_24/` T15 ; extension de `audits/wheatstone_2026_09_23/` |
| **Architecture « xor399 »** (2026, dossier d'un membre) : un masque unique à l'écart 399 | **Incohérente** : sur les cribs, le flux est plusieurs-à-plusieurs (S ← E, S, R, O ; L → V, Y) ⇒ **éliminé** (24/09) | base 7 §8 |
| **Algorithme « T » de Todd / Jose Luis** (2011, 2020) : clé = chiffré + départ − 4·rang (A–Z), puis Vigenère KRYPTOS | Colle à NYP → ELY et VTT → OIE avec deux départs libres (3 lettres chacun), échoue sur CLOCK ; « mots » trouvés en balayant 26 départs × 13 pas : recherche multiple ⇒ **sans valeur** (24/09) | base 7 §8 |
| **« Solution » communautaire de 2025** : 6 classes (Vigenère/Beaufort/variante selon (i mod 2)·3 + i mod 3) × roues de période 17 | **Non falsifiable** : ppcm(6,17) = 102 > 97, donc chaque position a sa propre clé (masque jetable) ; le clair est choisi, pas déduit ; les contrôles d'un membre montrent que la méthode ne retrouve pas un crib retiré (24/09) | base 7 §3 |
| **M-94 à 25 disques, génératrice fixe** (diapositives NSA 2014) | **impossible** (positions 21 et 71 : E→F et O→F sur le même disque) (24/09) | `audits/vision_2026_09_24/stats_quick.py` |
| Géométrie mesurée de l'écran | **jamais testée** (donnée absente) | [MR] ; `current_experimental_frontier.md` |

### 2.7 « Solutions » revendiquées publiquement (non validées)

| Revendication | Problème relevé | Où |
|---|---|---|
| SolveKryptos (7×14, cartes auxiliaires) | les cartes auxiliaires sont ajustées sur le clair proposé | F-06 |
| « Genie Engine » (A. Glanz), horloge de Berlin | réplication indépendante manquante | F-05 |
| Nash Associates (digraphes) | explicitement incomplète | F-03 |

## 3. Ce qui reste réellement ouvert

> **Mise à jour du 24/09 (base 6).** Scheidt (1991, 2015) n'a jamais vérifié K4 : Sanborn l'a chiffré seul, probablement fin 1989–1990 (après la chute du Mur), avec des erreurs possibles. Scheidt dit aussi que le **changement de base** a été envisagé puis **écarté**, et que le **masque** est la première étape. *Corrigé le 24/09 (base 7 §2.11 et §11.6)* : ce qui a été écarté est la base **mathématique** (base 2, base 16), et le masque change la « base de langue » ; la « première étape » dont parle Scheidt est celle de l'analyste, et l'ordre du masque n'est pas dit. Le « ? » avant OBKR n'est **pas codé** (IMG_1340 : lignes 4, 8, 10, 25).

> **Mise à jour du 23/09 (source primaire retrouvée).** « Deux systèmes… indice majeur » (Sanborn, inauguration 1990) porte sur **la plaque du bas, K3 + K4**, pas sur K4 seul (base 1, §4 quinquies). Les familles « deux couches » (substitution + transposition, etc.) ne reposent donc plus que sur le « masquage » de Scheidt (2020) et « plus d'une étape » (2015). Elles ne sont ni plus ni moins réfutées qu'avant, mais **moins prioritaires** qu'un système unique inconnu, sans le tableau.

Les familles ci-dessous sont ouvertes parce que **l'information manque**, pas parce que personne n'y a pensé :

1. **Géométrie physique mesurée** de l'écran K4 (lignes, espacements, « ? », « YAR », « L » en trop) et de ses rapports au tableau et au site (rose des vents, magnétite). Voir base 1, §8–9.
2. **Masques Morse (F-10)** : jamais réellement exécutés dans le dépôt, mais ils exigent une transcription vérifiée de K0.
3. **Superposition physique (F-07) et opérations sur la charte (F-08)** : il faut le plan coté.
4. **Clé courante tirée d'un texte non identifié** : impossible à réfuter avec 24 lettres **si l'alphabet est quelconque** (campagne 22/09 §5). *Fermée le 24/09 pour les alphabets à mot-clé* (un ou deux alphabets) si la clé est de l'anglais continu : `audits/motcle_pas7_2026_09_24/` §3.
5. **Quagmire I, II, IV à période moyenne** : 24 lettres ne suffisent pas mathématiquement.
6. **Gromark à deux alphabets inconnus** (39 amorces de Bean) : même situation. Il ne devient testable que si une source fixe les alphabets ; les alphabets de l'œuvre ne marchent pas (23/09).

## 4. Mes tests du 22/09 : lesquels étaient des redites ?

| Test | Nouveau ? |
|---|---|
| Quagmire III périodique avec **tout** alphabet mélangé | **Probablement nouveau dans cette forme** (le registre utilisait des listes de mots-clés) |
| Ligne+colonne, autoclave, progressive (alphabets fixes) | **Redite** (E-S-10, e_autokey_*, E-S-90) |
| Périodicité robuste aux nulles | **Redite** (E-SOLVE-10), élargie à tout alphabet |
| Clé courante K1–K3 | **Redite** (E-AUDIT-07, bijection 08b/08c), élargie à tout alphabet |
| Grille 98 = 14×7 | **Redite** (E-S-130) |
| Route de K3 sur K4 | **Redite élargie** (E-CFM-07), avec en plus la formulation exacte par multiplication modulaire qui ferme la famille |
| Route de K3 + clé courante des textes de la sculpture | Nouveau comme combinaison, sans signal |

## Références communautaires

- [MR] mruckman1, *K4 synthesis* (63 verdicts, « toutes les approches déterministes et légitimes sont épuisées » selon l'auteur ; restent la géométrie mesurée et un éventuel 5ᵉ crib) : https://github.com/mruckman1/k4_cipher/blob/main/docs/K4_synthesis.md
- rayborg, *kryptos-k4-dashboard* (registre de 100 méthodes, majoritairement ouvertes, outillage) : https://github.com/rayborg/kryptos-k4-dashboard
- Numberworld, *K4 abnormalities and solution attempts* (Hill, clé courante, Wichmann-Hill ; anomalie des lettres doubles en 7×14) : https://www.numberworld.blog/2017/03/kryptos-cipher-part-2.html
- Numberworld, *Part 4* (Quagmire + permutation par coordonnées) : https://www.numberworld.blog/2020/07/kryptos-cipher-part-4.html
- R. Bean, *Cryptodiagnosis of “Kryptos K4”*, HistoCrypt 2021 : https://richardbean.id.au/papers/03_Bean_HistoCrypt2021.pdf (intégré le 23/09)
- Kryptos Beyond K4 (blog) : https://kryptosfan.wordpress.com/
- Elonka Dunin : https://www.elonka.com/kryptos/
