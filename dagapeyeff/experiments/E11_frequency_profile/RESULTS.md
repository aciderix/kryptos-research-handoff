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

## 4. Conséquences (DÉDUCTIONS, pas des tests)
- L'indice de coïncidence « proche de l'anglais » (IC × 25 = 1,74) est une **coïncidence** : 13 symboles quasi
  uniformes (IC × 25 ≈ 1,92) plus 5 symboles rares donnent ≈ 1,74. L'argument « IC anglais ⇒ lettres anglaises en
  substitution simple », commun à toute la littérature (numberworld, dagapeyeffresearch, et nos E01-E09), est
  **invalidé** : le profil est celui d'un tirage uniforme sur 13 symboles, pas celui d'une langue.
- Cela explique l'échec systématique de E01-E09 (et des travaux antérieurs) : **toute** combinaison
  « transposition + Polybe d'un texte en langue naturelle » est exclue par les seuls comptes, quelle que soit la
  transposition (p ≤ 0,003 dans 8 langues).
- Modèles compatibles restant à examiner (HYPOTHÈSES) : (a) colonnes 1-13 = remplissage aléatoire (nulles) tiré
  dans 13 cases, le message éventuel étant ailleurs (colonne 14 : 14 symboles, sous la distance d'unicité ⇒
  invérifiable seul) ; (b) couche de chiffrement forte (polyalphabétique à longue clé) sur un alphabet réduit ;
  (c) clair lui-même non linguistique (code numérique, alphabet réduit/fusionné) ; (d) erreur ou canular. Chaque
  piste exige une pré-inscription et un critère de preuve ; (a) et (d) peuvent rendre le défi **insoluble**.
