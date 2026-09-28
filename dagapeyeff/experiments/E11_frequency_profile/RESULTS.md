# E11 — Profil de fréquences : **incompatible** avec une substitution monoalphabétique d'une langue naturelle

Statut : **OBSERVATION calibrée** (tests choisis après avoir remarqué l'anomalie, mais p-valeurs calculées par
simulation sur des textes réels ; plusieurs statistiques indépendantes concordent). Script : `tools/profile_tests.py` ;
sortie : `profile_tests.out`. Aucun modèle de transposition ni de carré n'intervient : ces tests portent sur les
**comptes** de symboles, invariants par toute transposition et par tout carré de Polybe.

## 1. Constats
| Mesure | Défi | Anglais (4 000 échantillons de même longueur) | Autres langues (fr, de, it, es, la, nl, eo) |
|---|---|---|---|
| Symboles distincts, 196 paires | **18** | 22,0 en moyenne ; P(≤ 18) = 0,0015 | 19,4-21,2 ; P(≤ 18) = 0,003-0,06 |
| Symboles distincts, 182 (sans colonne 14) | **13** | P(≤ 15) = 0,0004 | P(≤ 15) = 0 sur 5 000 |
| Symboles d'effectif ≥ 11 | **12** | 8,1 en moyenne ; P(≥ 12) = 0 | P(≥ 12) ≤ 0,0003 |
| Symboles d'effectif 3 à 8 | **1** | 8,6 en moyenne ; P(≤ 1) ≈ 0,001 | P ≤ 0,001 |

(La mention courante « 3 paires absentes » est inexacte : 7 des 25 cases sont vides — 61, 73, 95, 01, 02, 03, 05.)

## 2. Test du profil rang-fréquence (substitution inconnue ⇒ seul le profil trié compte)
D = Σ_r (obs_r − E_r)² / (E_r + 1), p par 3 000 simulations :
| Modèle | Géométrie A (196) | Géométrie B (182, colonne 14 retirée) |
|---|---|---|
| 8 langues naturelles | **rejetées** (p = 0,0003-0,0027) | **rejetées** (p = 0,0003-0,0020) |
| uniforme sur 13 symboles | rejeté | **accepté** (D = 0,3 ; p = 0,98) |
| uniforme sur 14, 15, 18 symboles | rejetés | rejetés |

## 3. Structure séquentielle des colonnes 1-13 (182 symboles), contre 4 000 mélanges
Dépendance des bigrammes (G-test) p = 0,71 ; doublons 19 (mélanges 13,6 ; p = 0,085) ; coïncidences verticales
15 (12,9) ; bigrammes répétés 67 (68,5). ⇒ **aucune structure séquentielle détectable**.
Les 5 symboles rares (04, 71, 92, 93, 94) n'apparaissent **que** dans la colonne 14.

## 4. Portée — séparée en quatre niveaux (révisée le 2026-09-28 après relecture critique)
1. **Observation** : 18 symboles distincts sur 196 ; 13 hors colonne 14 ; les 5 symboles rares seulement en
   colonne 14 ; 12 symboles d'effectif ≥ 11, un seul entre 3 et 8.
2. **Résultat statistique** : le profil rang-fréquence est rejeté pour 8 langues naturelles (p ≤ 0,003), **en
   géométrie A (aucune colonne retirée) comme en B** ; hors colonne 14, il est compatible avec un tirage uniforme
   sur 13 symboles (p = 0,98) et sans structure séquentielle détectable.
3. **Portée exacte** : sont rejetés tous les mécanismes où **chaque paire du chiffré correspond une-à-une à une
   lettre du clair** dans l'une de ces 8 langues — quel que soit le carré et **quelle que soit la transposition des
   paires** (les comptes y sont invariants), colonne 14 comprise ou exclue. **Non couverts** : nulles insérées
   ailleurs (enseignées par l'auteur), homophones, couche polyalphabétique, fractionnation (bornée par
   l'alternance parfaite, à tester formellement), clair non linguistique.
4. **Conclusion** : **non déterminée**. « Le clair n'est pas linguistique » n'est **pas** démontré tant que la
   capacité des mécanismes documentés (notamment langue + nulles) à produire ce profil n'a pas été calibrée sur des
   textes réels (→ E12). L'idée que l'IC « anglais » soit une coïncidence reste une **interprétation**.

## 5. Pistes (HYPOTHÈSES, à pré-inscrire)
(a) langue naturelle + nulles selon une règle de l'auteur (à calibrer en premier) ; (b) colonnes 1-13 = remplissage,
message ailleurs (colonne 14 : sous la distance d'unicité) ; (c) couche polyalphabétique sur alphabet réduit ;
(d) clair non linguistique (code numérique, alphabet fusionné) ; (e) erreur ou canular.
