# Audit « moteur unique » (25/09/2026) : les six chantiers T0–T6

**Statut :** `EXPLORATION RESULT`. Aucun clair, aucun signal. Chaque test a un contrôle positif qui passe et des témoins « K4 mélangé » (30 sauf mention contraire) calculés dans la même passe.

## 0. Outillage (T0) : tout en C + OpenMP, 4 cœurs

| Fichier | Rôle |
|---|---|
| `genalpha.c` | alphabets à mot-clé sans doublons (un alphabet et son inverse comptent pour un) : L0 (mot + reste), L1 (« suite »), matrices (alphabet écrit en lignes de 2 à 13, relu par colonnes : 4 sens + ordre du mot), K2 (deux mots-clés) |
| `k4x.c` | moteur unique : familles p, b, t, a, c, r, m, g, f (voir l'en-tête) ; 5 types (Q3, Q2, Q1, Q4a, Q4b) × 3 conventions ; option `-2` : Quagmire IV (deux alphabets à mot-clé) |
| `k4s.c` | T3 : clé périodique p = 1..14 (ou 26) avec le modèle d'erreur de Sanborn |
| `k4tp.c` | T4 : transposition à colonnes + substitution périodique (hypothèse NSA 1992) |
| `k4sa.c` | T5 : énumération exacte des clés compatibles avec les cribs (alphabet quelconque), puis recuit sur le texte entier |
| `synth.c`, `tp_synth.c`, `q4_synth.c`, `g_synth.c` | chiffrés synthétiques de contrôle, un par famille |
| `shuf97.c` | K4 mélangé (même générateur que les moteurs) |
| `run_queue*.sh` | files d'attente des balayages |

**Vitesse.** Élagage exact : dans un groupe (phase, bloc, classe), la plus grande multiplicité c vérifie c ≤ 1 + (paires égales), donc on ne calcule exactement que ce qui peut battre le record. Invariances : inverser l'alphabet ou passer de VIG à VARB change la clé en ±K + c ; les familles p, b, t y sont insensibles. Gains mesurés sur 5 000 alphabets × 6 chiffrés : blocs 36 s → 0,3 s ; T16 355 s → 0,9 s ; période 7 53 s → 0,5 s. Toutes les familles sur 101 086 alphabets × 31 chiffrés : **287 s**.

**Contrôles positifs** (`synth.c`, alphabet PALIMPSEST, types et conventions variés) : chaque famille retrouve son chiffré avec e = 0 (témoins : 5 à 11) ; clé courante −3,86 (témoins −5,1 à −5,3). Une erreur de segmentation (lignes du cuivre) de l'ancien `kwsweep` a été corrigée au passage.

## 1. T1 : plus de témoins sur les balayages (30 témoins K4 mélangé)

**Alphabets linéaires** (101 086 : L0, L1, et leurs inverses), `res_T1_lineaires.txt` :

| Famille | K4 | témoins (min / médiane / max) | p |
|---|---|---|---|
| p7 pure | 10 | 9 / 10 / 11 | 0,94 |
| p7 + décalage par ligne du cuivre | 7 | 7 / 8 / 9 | 0,29 |
| p7 + décalage par ligne de 7 | 7 | 5 / 7 / 7 | 1,00 |
| blocs T11/T11b (n = 5–14) | 7 | 7 / 8 / 9 | 0,10 |
| T16 clé transposée (L ≤ 7) | 9 | 8 / 9 / 9 | 1,00 |
| autoclé sur le clair (L ≤ 13) | 4 | 4 / 5 / 5 | 0,19 |
| autoclé sur le chiffré (cribs justes, max) | 10 | 9 / 10 / 10 | 0,65 |
| clé courante anglaise (score) | −4,98 | −5,15 / −5,01 / −4,63 | 0,35 |
| p7, convention par ligne du cuivre / de 7 / alternée | 9 / 9 / 9 | médianes 9 / 8 / 9 | 0,97 / 1,00 / 0,97 |

Les chiffres anciens sont retrouvés (clé courante −4,983, autoclé 4, blocs 7). **Aucune famille ne place K4 hors des témoins.**

**Alphabets en matrice** (1 395 188 : linéaires + matrices de largeur 2 à 13), `res_T1_matrices.txt`, 9 073 s :

| Famille | K4 | témoins (min / médiane / max) | p |
|---|---|---|---|
| p7 pure / + ligne du cuivre / + ligne de 7 | 9 / 7 / 6 | 8–10 / 6–8 / 5–7 | 0,97 / 0,94 / 0,90 |
| blocs / T16 | 7 / 7 | 6–8 / 7–8 | 0,36 / 0,45 |
| **autoclé sur le clair (L ≤ 13)** | **3** | 4 / 4 / 5 | **0,032** |
| autoclé sur le chiffré | 10 | 10 / 10 / 12 | 1,00 |
| clé courante anglaise | −4,72 | −4,86 / −4,67 / −4,32 | 0,58 |
| conventions par ligne (3 variantes) | 8 / 8 / 8 | médianes 8 / 8 / 8 | 0,94–1,00 |

**Le seul écart, examiné et écarté** (`akdec.c`) : un unique cas à 3 erreurs, alphabet `EOSBFHJLNQTWYUDACGIKMPRVXZ` (mot EUODOS, matrice de largeur 2), type Q1, Beaufort, clé lue en A–Z, écart 13. Le déchiffrement complet est du charabia (−7,08) : « …EASTNORTHEASTCLREVRNBAYRNMBAGWYLSLVCGGFDPHQERVISCLOCKKOUMIXA… ». À l'écart 13, EASTNORTHEAST fixe les 13 chaînes ; BERLINCLOCK en retrouve 8 sur 11 par hasard. Onze familles testées : un p de 0,03 est attendu une fois.

## 2. T2 : dictionnaire élargi et deux mots-clés

- Dictionnaire de **430 167 mots** (liste anglaise de 370 000 mots, prénoms et noms, 250 pays, 34 000 villes, 258 termes thématiques : CIA, Berlin, Égypte, cryptographie, fautes de Kryptos…) → 684 730 alphabets. Résultats : §7.
- **Deux mots-clés** (40 mots thématiques de `themes_coeur.txt` × 59 497 mots, dans les deux ordres) → 2 960 415 alphabets. Résultats : §7.

## 3. T3 : le modèle d'erreur observé chez Sanborn (`k4s.c`)

**Modèle.** Sur le petit fragment de 97 lettres (base 7 §9.4), une erreur est un **glissement** (clé de la phase voisine), une **copie** (chiffré = clair), ou un **décalage commun** partagé par au moins deux erreurs. On calcule, pour la clé périodique p = 1..14 : e_min sans restriction, eS0 (toutes les erreurs de ce type, au plus 3) et eS1 (au plus une erreur quelconque en plus).

**Contrôle positif sur le vrai fragment** (KRYPTOS, SHADOW, 24 lettres « crib » prises dans son clair) :
- fenêtres couvrant les erreurs 13–25 et 63–73 (erreur 22, copie) : e = 1, **eS0 = 1** ; témoins 10–12 ;
- fenêtres 0–23 (erreurs 9 et 22) : **eS0 = 2** ;
- fenêtres 0–11 et 85–96 (erreurs 9, 87 glissements ; 91 sans lecture simple) : eS0 impossible, **eS1 = 3**. Le modèle reconnaît les erreurs de Sanborn et refuse celle qui n'en est pas une.
- Un premier essai admettait un « décalage commun » porté par une seule erreur, ce qui expliquait n'importe quoi ; le contrôle l'a révélé et c'est corrigé.

**K4** (101 086 alphabets linéaires, puis 1 395 188 avec les matrices ; 5 types ; VIG/BEAU) : e_min ≥ 5 pour toute période de 1 à 14. **Jamais 3 erreurs ou moins**, de quelque type que ce soit. K4 est au niveau des témoins (p = 0,26 à 1).
⇒ **Éliminé** : K4 n'est pas un chiffre périodique à alphabet à mot-clé (linéaire ou en matrice), période ≤ 14, affecté d'au plus 3 erreurs.

## 4. T4 : familles manquantes

**TABP : transposition à colonnes + substitution périodique** (`k4tp.c`, hypothèse NSA 1992 « alphabets puis transposition », et méthode de K3 combinée à celle de K1).
- Transpositions : toutes les permutations de colonnes pour w = 2..8 ; ordres tirés des 430 167 mots pour w = 9..20 (271 138 ordres) ; lecture haut → bas ou bas → haut (route de K3) ; π et π⁻¹. Substitution p = 1..13 avant ou après ; KRYPTOS et A–Z (4 combinaisons) ; VIG/BEAU.
- **Contrôle** (w = 7, route bas → haut, Quagmire III KRYPTOS p = 5) : e = 0, clair retrouvé (−4,28).
- **K4** : e_min de 15 (p = 1) à 5 (p = 13), au niveau des témoins partout (p = 0,36 à 1). Aucun cas à ≤ 3 erreurs, donc aucun déchiffrement à noter. **Éliminé** dans cette portée.

**Quagmire IV : deux alphabets à mot-clé différents**, familles p7 (P, L, R), blocs, T16 :
- 497 × 497 alphabets thématiques : K4 au niveau des témoins (p = 0,10 à 1) ;
- 76 alphabets thématiques centraux × 101 086 alphabets du dictionnaire (10 témoins) : p = 0,27 à 1.
- Contrôle (GIRASOL / PALIMPSEST, p7 + ligne du cuivre) : e = 0 (témoins 10).

**T16 et conventions mélangées** : dans T1 (§1) ; aucun signal.

## 5. T5 : attaque sur le texte entier à partir des solutions exactes des cribs (`k4sa.c`)

**Pourquoi l'ancienne attaque échouait** (`../recuit_2026_09_24/`) : le recuit sur l'alphabet libre restait bloqué à 22 cribs sur 24.

**Nouvelle méthode.**
1. Chaque lettre des cribs relie σ(chiffré) et σ(clair). Un arbre couvrant exprime chaque lettre comme σ(racine) + combinaison de la clé, et chaque arête de plus donne une équation linéaire mod 26.
2. On énumère **toutes** les clés exactes, en écartant celles qui envoient deux lettres au même rang.
3. On place ensuite par recuit les composantes et les lettres libres. Les cribs sont justes par construction.

**Contrôle positif** (Quagmire III, alphabet quelconque, période 7 + décalage par ligne du cuivre) : 13 152 clés exactes ; **clair retrouvé** en 25 s (score −4,47), à l'exception des 4 lettres de la ligne 25, dont le décalage n'est pas contraint. Même réussite avec 2 lettres de crib retirées (374 940 clés, 2 000 itérations par clé).

**K4.**
- Famille L (décalage par ligne du cuivre), alphabet quelconque : **aucune clé exacte**, ni sans erreur, ni en retirant n'importe laquelle des 24 lettres. CP-SAT confirme e_min = 2 (retrait de 32 et 66). T24 disait « 1 ou 2, non prouvé » ; c'est maintenant 2, prouvé.
- Avec 32 et 66 retirées : 582 924 clés exactes, et le meilleur texte est du charabia (−6,38). Dans les mêmes conditions, le contrôle est résolu. ⇒ **Éliminé** pour ce couple d'erreurs. Les 275 autres couples n'ont pas été parcourus.
- Période 7 pure : aucune clé exacte (déjà connu).
- Famille R (décalage par ligne de 7) : K4 compatible sans erreur (2 844 clés exactes). Mais 75 % des témoins le sont aussi (CP-SAT, `../erreurs_multiples_2026_09_24/res_t24_rangee7.txt`). La famille est trop lâche, et les lignes sans crib ont un décalage libre : pas de conclusion par le texte entier.

## 6. T6 : restriction d'alphabet appliquée aux familles indécidables

Familles de la base 2 déclarées indécidables avec un alphabet quelconque, rejouées avec alphabets à mot-clé (résultats : §7) :
- **clé ligne + colonne** (« ID BY ROWS », largeur 2..48) et **deux mots superposés** (p1 < p2 ≤ 26) : famille `g`. On ne garde que les 190 configurations où les cribs donnent au moins 3 équations redondantes (les autres sont indécidables par construction, quel que soit l'alphabet). On compte les cas compatibles (sans erreur, et avec au plus une erreur). Contrôles : 1 à 11 cas compatibles, contre 0 médiane pour les témoins ;
- **autoclé vers l'avant** (T17), sur le clair et sur le chiffré : famille `f` ; contrôle : e = 0 et 18/18 ;
- **Quagmire I, II, IV à période moyenne** : `k4s` jusqu'à p = 26.

## 7. Résultats des grands balayages

**T2, dictionnaire élargi** (684 730 alphabets, 30 témoins, 2 332 s, `res_T2_dico.txt`) : K4 dans la distribution des témoins pour toutes les familles (p = 0,13 à 1). Meilleurs de K4 : p7 10 / 7 / 6 ; blocs 7 ; T16 8 ; autoclé sur le clair 4 ; autoclé sur le chiffré 10 cribs justes ; clé courante −4,80 (témoins −4,99 à −4,56). **Aucun nom propre, lieu ou terme thématique ne fait sortir K4 du hasard.**

**T3 sur le dictionnaire élargi** (`res_T3_sanborn_dico.txt`) : e_min ≥ 5 pour toute période de 1 à 14 ; aucun cas « à la Sanborn » à ≤ 3 erreurs (p = 0,32 à 1).

