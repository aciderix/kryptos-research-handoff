# Audit « vision » (24/09/2026) : relier ce qui est dispersé, et tester ce qui ne l'a jamais été

**Demande :** relire tout le dépôt, faire des liens que personne n'a faits, sortir des sentiers battus, tester de l'inédit avec du code rapide.
**Statut :** `EXPLORATION RESULT`. Aucun clair, aucune solution, aucun signal au-dessus du hasard. Les négatifs sont conservés avec leur portée.
**Lectures associées :** `docs/knowledge_base/06_documents_2026_09_24.md` (documents versés le même jour) et `docs/knowledge_base/07_groupsio_2026_09_24.md` (fichiers du groupe kryptos.groups.io).

## 0. Outillage (C, sans dépendance)

| Fichier | Rôle |
|---|---|
| `k4lib.h` | noyau exact. `eng_num` : clé **numérique** connue, alphabet σ **inconnu** (26! couverts), même σ côté clair et chiffré (Quagmire III). Composantes connexes + placement injectif, ~µs par test. `eng_let` : clé en **lettres**, lettre-clé lue dans le même σ inconnu (convention K1–K2). CSP avec propagation et symétrie σ → u·σ. VIG `C=P+k`, BEAU `C=k−P`, VARB `C=P−k` |
| `lincsp.h` | solveur de contraintes linéaires mod 26 avec σ « toutes différentes » (clés à plusieurs paramètres) |
| `selftest.c` | contrôles : 2 000/2 000 témoins positifs retrouvés par chaque moteur ; 0/2 000 clés aléatoires compatibles avec K4 ; clé courante « clair de K1 » : 0 (comme le dépôt) |
| `data/*.txt` | panneau chiffré et tableau, transcription Wikipédia (28 lignes chacun) |

Compilation : `gcc -O2 -march=native -o t1 t1_sculpture_keys.c` (idem pour les autres). Toutes les sorties brutes sont dans `results_*.txt`.

## 1. Liens documentaires (sans calcul)

1. **IMG_1340 est lisible : « 4, 8, 10, 25 » = les lignes des quatre « ? ».** Sur le panneau chiffré (transcription Wikipédia, `data/panel_wikipedia.txt`), les quatre points d'interrogation sont **exactement** aux lignes 4, 8, 10 et 25. La note de Sanborn se lit donc : « 3. Extra L at end of line, bottom chart section. 4. **? (lignes 4, 8, 10, 25) not coded** ». Le « ? » avant OBKR **n'est pas chiffré** : K4 compte 97 lettres. Toute la piste « 98 = 14 × 7 » perd son appui documentaire. Et le point 3 décrit le L en trop à la fin de la ligne N, qui est **la première ligne de la plaque du bas** du tableau (confirmé par la NSA le 14/11/1991 : « the underline shows where the upper and lower copper plates are bolted together », sous la ligne M). Les deux « anomalies » sont des **particularités expliquées** par l'artiste, pas des indices.
2. **Chronologie du chiffrement de K4.** Voir `06_documents_2026_09_24.md` §3 : le clair définitif de K4 (chute du Mur, 9/11/1989) est postérieur à la tournée de mi-1989 ; Sanborn a « réécrit d'une autre façon » ; Scheidt n'a jamais vérifié K4. ⇒ K4 a été chiffré **seul par Sanborn, fin 1989–1990** ; le « K4 alternatif de 1988 » (K5) en est vraisemblablement la première version.
3. **« Changer la base » était une option refusée** (Scheidt 2015 : « I was asked not to do »). Sanborn 1991 : « plain English ».
4. **PALIMPSEST** (clé de K1), 2ᵉ sens (Webster's Third, cité par la NSA en 1992) : plaque de laiton **gravée au revers**. C'est le tableau de Kryptos.

## 2. Tests exacts nouveaux (paramètres fixés avant calcul ; aucun score d'anglais)

| Test | Question | Portée | K4 | Témoin | Verdict |
|---|---|---|---|---|---|
| **T1** `t1_sculpture_keys.c` | Une **clé écrite sur la sculpture**, lue dans un sens géométrique ? | Sources : tableau (avec/sans étiquettes), panneau chiffré (« ? » retirés ou comptés), Morse K0 (2 ordres). **13 parcours 1D** (lignes ↔, inversé, boustrophédons, colonnes ↕ et justifiées, diagonales) × tous décalages ; **2D sur cylindre** : K4 à sa place physique, clé = lettre en (ligne+dr, colonne+dc), source telle quelle / miroirs / rotation, tous dr, dc. Juges : σ quelconque (L3, NA, NK) et 24 conventions à alphabets fixés | **57 617 placements : 0 compatible** ; alphabets fixés max 8/24 | 10 chiffrés aléatoires : 0 ; max 7–8/24 | **éliminé** |
| **T2** `t2_chain.c` | Clé par **addition en chaîne** (Gromark, VIC) | Règles k[i]=k[i−n]+k[i−n+1], k[i−n]+k[i−1], k[i−n]−k[i−n+1], k[i−n]+k[i−n+1]+k[i−1] ; base 10 (amorces 2–6 chiffres) et base 26 (2–5 lettres, 11,9 M) ; QIII σ quelconque ; QIV deux σ | **QIII : 0 partout** | QIII : 0 | **éliminé (QIII)** |
| | (même test, QIV) | base 10, amorce 5 (Gromark de Bean) | **39 amorces** (= Bean, reproduit) | **clés de chiffres i.i.d. : 40,0 attendues** | **les 39 amorces de Bean sont exactement le hasard** : le Gromark à deux alphabets n'a aucun appui statistique |
| **T3** `t3_compound.c` | **Deux mots-clés superposés** (k = a[i mod p1] + b[i mod p2]) : cellule marquée OPEN « non énumérée » dans `docs/two_systems_landscape.md` | p1 1–30, p2 p1+1–48, alphabets A–Z/KRYPTOS, 12 conventions, système linéaire exact mod 2 et mod 13 | 0 couple « sévère » ; **aucun couple compatible avec p1+p2 ≤ 24** | 200 chiffrés : moyenne 7 118 cases compatibles (K4 : 7 113) ; 0,41 case sévère attendue | **éliminé partout où 24 lettres tranchent** (p1+p2 ≤ 24, ex. KRYPTOS+PALIMPSEST, 7+10) ; indécidable au-delà |
| **T4** `t4_compound_anyalpha.c` | Même chose, **alphabet quelconque** | p1+p2 ≤ 20, p2 non multiple de p1 (54 couples par convention) ; VIG, BEAU, VARB ; validation : reproduit le SAT du dépôt (Vig p 1–26 : aucune ; Beau : 23, 26 compatibles, 16, 19, 24, 25 non tranchés) | **éliminés : VIG 28/54, VARB 28/54, BEAU 18/54** ; compatibles : 20 / 19 / 23 ; non tranchés (limite de nœuds) : 6 / 7 / 13 | 10 chiffrés aléatoires par couple compatible : 30–100 % passent aussi ; **0 couple sévère** (témoin ≤ 1/10) | **éliminé pour les petits couples** (listes ci-dessous) ; les couples compatibles le sont **au niveau du hasard** (indécidables) |
| **T5** `t5_fold.c` | **La pliure** : les cribs sont en miroir autour du centre (i ↔ 96−i, 11 paires) | F1 clé = clair en miroir (partout) ; F2–F5 demi-pliures (clair ou chiffré en miroir, une moitié) ; F6 chiffré réfléchi en tout centre | F1 : **0** ; F2–F5 : 3 cases sur 12 ; F6 : 59 sur 427 | F1 : 0/200 ; F2–F5 : 14–45 % ; F6 : ≈ 20 par chiffré | **F1 éliminé** (test puissant) ; F2–F6 **au niveau du hasard** (indécidables) |
| **T6** `t6_new_sources.c` | **Nouveaux textes** comme clé courante | Journal de fouilles de Carter 1922 (22 084 lettres, ≠ livre déjà testé) ; plaques des 14 directeurs, statue de Donovan, buste de Dulles, *Book of Honor* (relevés NSA 1991) ; tous décalages | **0** partout ; fixés max 8/24 | 10 chiffrés : 0 ; max 7–9/24 | **éliminé** |
| **T7** `t7_cornerstone.c` | Un texte **« enfoui »** ou **fondateur** du site comme clé courante (K2 : « IT'S BURIED OUT THERE SOMEWHERE ») | Directive Truman du 22/01/1946, scellée dans la **boîte de cuivre de la pierre angulaire** de la CIA (`data/truman_1946_directive.txt`, texte FRUS) ; principes du concours artistique CIA 1988 (`data/cia_fine_arts_principles_1988.txt`) ; lettre de Sanborn de 1989 ; concaténation ; endroit et envers, tous décalages | **0** partout ; fixés max 8/24 | 10 chiffrés : 0 ; max 6–8/24 | **éliminé** |
| **T8** `t8_inv563.c` | La **« chaîne de masquage »** proposée sur le groupe (2017), sans explication | Identifiée : développement décimal de **1/563** (période 281). Clé numérique additive : un chiffre par lettre, ou paires mod 26, tous décalages ; σ quelconque et 12 conventions fixées | **0** ; fixés max 6/24 | 10 chiffrés : 0 ; max 5–7/24 | **éliminé** (emploi direct) |
| **T2 (option 7)** `t2_chain.c 7` | Addition en chaîne à amorce de **7 chiffres** (lien 7 / « fonction régénérative ») | base 10, 10⁷ amorces × 4 règles | QIII **0** ; QIV ≈ attendu i.i.d. | 7 témoins : QIII 0 | **éliminé (QIII)** ; `results_t2_amorce7.txt` |
| **T9** `t9_chain_kryptos.c` | Chaîne en base 26 avec l'amorce **KRYPTOS** | rangs A–Z ou KRYPTOS, endroit/envers, 4 règles | **0** ; fixés max 5/24 | — | **éliminé** ; `results_t9.txt` |
| **T10** `t10_girasol.c` | La **maquette GIRASOL de 1988** (« Pre-K », deux versions) comme source de clé | Reconstitution de P. Kiesel (`data/girasol_*.txt`) : tableau GIRASOL 15 et 26 lignes, table ABC inversée, bloc chiffré 7×21 et son clair ; 13 parcours 1D (sources < 40 lettres doublées), 2D avec K4 à sa place physique **et** en lignes de 21 (cylindre, 4 orientations) ; σ quelconque + alphabets fixés A–Z, KA et **GIRASOL** | **54 188 placements : 0** ; fixés max 8/24 | 20 chiffrés : 0 ; max 8–10/24 | **éliminé** ; `results_t10.txt` |

**T4 : couples (p1, p2) éliminés, alphabet quelconque** (aucune permutation σ des 26! n'est compatible avec les 24 lettres connues) :
- **VIG et VARB** (les mêmes 28) : (2,3) (2,5) (2,7) (2,9) (2,11) (2,13) (2,15) (2,17) (3,4) (3,5) (3,7) (3,8) (3,11) (3,13) (3,16) (4,5) (4,6) (4,7) (4,9) (4,10) (4,11) (4,13) (5,7) (5,8) (5,9) (6,8) (8,10) (8,12).
- **BEAU** (18) : (2,3) (2,5) (2,7) (2,9) (2,11) (2,17) (3,4) (3,5) (3,7) (3,8) (3,13) (3,14) (4,5) (4,7) (4,11) (5,7) (5,8) (5,9).
- Non tranchés (arrêt à 2 M nœuds) : VIG et VARB (3,14) (3,17) (4,14) (6,7) (6,9) (6,14), plus (5,14) en VARB ; BEAU (3,16) (3,17) (4,6) (4,9) (4,10) (4,14) (6,7) (6,8) (6,9) (6,10) (6,14) (8,10) (8,12).
- Réserve : le contrôle positif (clé composée (7,10) fabriquée, VIG) a atteint la limite de 20 M nœuds sans conclure ; la validation sur les clés périodiques simples reproduit, elle, exactement le SAT du dépôt. Les éliminations sont des preuves (arbre exploré en entier) ; seuls les « compatibles » et « non tranchés » dépendent de la limite.
- Exemple de lecture : KRYPTOS + PALIMPSEST (7,10) avec un alphabet **quelconque** reste compatible, mais 9 chiffrés aléatoires sur 10 le sont aussi. Les 24 lettres ne suffisent pas à trancher.

## 3. Statistiques revues

`stats_quick.py` et calcul direct :
- **Largeur 21** (11 bigrammes verticaux répétés) : p = 0,0003 **pour la largeur 21 seule**, mais **p ≈ 0,09** si l'on tient compte du balayage de toutes les largeurs 2–48 (5 000 mélanges de K4). La NSA avait relevé un « intervalle 7 » dès 1992 ; l'anomalie est réelle mais faible.
- **IC des 21 premières lettres** (0,0667, « comme l'anglais ») : 2,4 % pour cette fenêtre-là, mais **42 %** des K4 mélangés ont une fenêtre de 21 lettres au moins aussi forte (la meilleure fenêtre de K4 lui-même atteint 0,0857). **Non significatif.**
- **Symétrie des cribs** : positions 23–33 et 73–63 en miroir exact autour de la position 48 (11 paires). Fait géométrique ; les familles « pliure » qui l'exploitent sont testées en T5.
- **M-94** (diapositives NSA) : 25 disques en ordre fixe, génératrice fixe : **impossible** (positions 21 et 71 : E→F et O→F sur le même disque).
- **Lettres doublées** : 5 des 6 doublets de K4 sont en position ≡ 4 (mod 7) ; p ≈ 0,0006 (module 7 seul), **p ≈ 0,02** (modules 3–16 balayés). Relevé par J. Gillogly en 2005 dans l'archive du groupe (base 7 §7.2). Statistique choisie après coup, mais c'est le signal « 7 » le plus net.

## 4. Ce qui reste ouvert (et pourquoi)

- Les familles où **24 lettres ne tranchent pas** : QIV, clés composées longues, demi-pliures, Gromark à deux alphabets. Il faut plus de clair connu : **K5** (outil `audits/k5_depth/`) ou un nouveau crib.
- **Le masque d'abord** (Scheidt 2015) : la famille « masque → substitution » avec un masque **physique** reste non déterminée (bases 2 et 3).
- **Martinsburg** (IRS) : les 17 lignes binaires relevées en 2003 ne se décodent par aucune convention simple (fragments d'alignement inconnu). Si on les décodait, on aurait un exemple réel de « base changée » par Sanborn. Il faudrait une photo complète.
