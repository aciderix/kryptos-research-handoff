# Audit « vision » (24/09/2026) : relier ce qui est dispersé, et tester ce qui ne l'a jamais été

**Demande :** relire tout le dépôt, faire des liens que personne n'a faits, sortir des sentiers battus, tester de l'inédit avec du code rapide.
**Statut :** `EXPLORATION RESULT`. Aucun clair, aucune solution, aucun signal au-dessus du hasard. Les négatifs sont conservés avec leur portée.
**Lectures associées :** `docs/knowledge_base/06_documents_2026_09_24.md` (documents versés le même jour).

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
| **T4** `t4_compound_anyalpha.c` | Même chose, **alphabet quelconque** | p1+p2 ≤ 20 ; validation : reproduit le SAT du dépôt (Vig p 1–26 : aucune ; Beau : 23, 26) | voir `results_t4.txt` | 10 témoins par couple compatible | voir `results_t4.txt` |
| **T5** `t5_fold.c` | **La pliure** : les cribs sont en miroir autour du centre (i ↔ 96−i, 11 paires) | F1 clé = clair en miroir (partout) ; F2–F5 demi-pliures (clair ou chiffré en miroir, une moitié) ; F6 chiffré réfléchi en tout centre | F1 : **0** ; F2–F5 : 3 cases sur 12 ; F6 : 59 sur 427 | F1 : 0/200 ; F2–F5 : 14–45 % ; F6 : ≈ 20 par chiffré | **F1 éliminé** (test puissant) ; F2–F6 **au niveau du hasard** (indécidables) |
| **T6** `t6_new_sources.c` | **Nouveaux textes** comme clé courante | Journal de fouilles de Carter 1922 (22 084 lettres, ≠ livre déjà testé) ; plaques des 14 directeurs, statue de Donovan, buste de Dulles, *Book of Honor* (relevés NSA 1991) ; tous décalages | **0** partout ; fixés max 8/24 | 10 chiffrés : 0 ; max 7–9/24 | **éliminé** |

## 3. Statistiques revues

`stats_quick.py` et calcul direct :
- **Largeur 21** (11 bigrammes verticaux répétés) : p = 0,0003 **pour la largeur 21 seule**, mais **p ≈ 0,09** si l'on tient compte du balayage de toutes les largeurs 2–48 (5 000 mélanges de K4). La NSA avait relevé un « intervalle 7 » dès 1992 ; l'anomalie est réelle mais faible.
- **IC des 21 premières lettres** (0,0667, « comme l'anglais ») : 2,4 % pour cette fenêtre-là, mais **42 %** des K4 mélangés ont une fenêtre de 21 lettres au moins aussi forte (la meilleure fenêtre de K4 lui-même atteint 0,0857). **Non significatif.**
- **Symétrie des cribs** : positions 23–33 et 73–63 en miroir exact autour de la position 48 (11 paires). Fait géométrique ; les familles « pliure » qui l'exploitent sont testées en T5.
- **M-94** (diapositives NSA) : 25 disques en ordre fixe, génératrice fixe : **impossible** (positions 21 et 71 : E→F et O→F sur le même disque).

## 4. Ce qui reste ouvert (et pourquoi)

- Les familles où **24 lettres ne tranchent pas** : QIV, clés composées longues, demi-pliures, Gromark à deux alphabets. Il faut plus de clair connu : **K5** (outil `audits/k5_depth/`) ou un nouveau crib.
- **Le masque d'abord** (Scheidt 2015) : la famille « masque → substitution » avec un masque **physique** reste non déterminée (bases 2 et 3).
- **Martinsburg** (IRS) : les 17 lignes binaires relevées en 2003 ne se décodent par aucune convention simple (fragments d'alignement inconnu). Si on les décodait, on aurait un exemple réel de « base changée » par Sanborn. Il faudrait une photo complète.
