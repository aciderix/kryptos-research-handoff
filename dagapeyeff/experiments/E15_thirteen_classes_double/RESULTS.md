# E15 — RÉSULTATS (2026-09-28) : « 13 classes » + double transposition (≤ 7×7) : **NÉGATIF**

Pré-inscription : `PREREGISTRATION.md` (commit aace583, avant tout run réel). Journaux : `logs/local/` (ici) et
`logs/run_agent2/` (8 contrôles par l'autre agent, commit edc561c de sa branche, vérifiés).

## Contrôles (anglais réservé → 13 classes équilibrées → double transposition aléatoire)
Lettres lues justes : ici 169, 178 (+ 117 pour un contrôle supplémentaire) ; autre agent 170, 177, 165, 33, 174, 162,
17, 175. Au seuil **pré-inscrit de 80 %** (≥ 146/182) : **8/10** sur les 10 contrôles prévus (8/11 avec le
supplémentaire) → **admissible**. (Le code affichait « OK » à 90 % ; la pré-inscription fait foi.)
qoff des contrôles lus : −9,007 … −9,712 (min **−9,712**).

## Null (5 mélanges des 182 symboles réels)
−11,067 ; −10,964 ; −11,028 ; −11,005 ; −11,003 (max **−10,964**).

## Vrai chiffré
| Graine | Meilleur qoff | Clés (W1 ; W2) | Appariement trouvé | Vérification directe |
|---|---|---|---|---|
| 1521 | **−11,096** | 7 ; 7 | E seul ; Q/T C/U M/Y D/B H/K W/O N/V F/L R/G S/X P/I Z/A | exacte |
| 1522 | **−11,059** | 7 ; 4 | E seul ; B/T Q/R V/O C/U Y/P H/D A/Z N/W S/X K/L I/G M/F | exacte |

## Critères
1. qoff > max(null) + 0,5 = −10,464 **et** ≥ −9,712 − 0,5 = −10,212 : **non** (−11,06 / −11,10, au niveau du null).
2. Lecture : quelques mots isolés (THAT, FIRST, BEFORE, GOING) — attendus d'un décodage de Viterbi sur du bruit
   (les mélanges produisent le même score) ; pas de texte continu.
3. Stabilité : **non** (clés et appariements différents entre graines).
⇒ **NÉGATIF** : l'hypothèse « 13 classes équilibrées (anglais) + double transposition colonnaire ≤ 7×7 » est
exclue à la puissance mesurée (≈ 80 %).

## Bilan de l'hypothèse « 13 classes » (E13-E15)
- Compatible avec le **profil** des fréquences (E13, anglais/latin) ; exige une transposition (E13 Q2).
- **Exclue** avec colonnaire simple largeurs 2-11, deux sens (E14) et avec double colonnaire ≤ 7×7 (E15).
- Reste ouverte pour d'autres transpositions (largeurs ≥ 12, routes non colonnaires, etc.) et d'autres
  appariements que « fréquente + rare » ; aucune trace documentaire.
