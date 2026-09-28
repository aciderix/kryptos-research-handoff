# Recalibration de l'excès de coïncidences à l'écart 7 dans K4 (2026-09-28)

Motif : leçon de `dagapeyeff/experiments/E08_w7_calibration` (un excès jugé sur peu de mélanges et remarqué après coup
peut retomber au niveau du hasard une fois calibré et corrigé pour comparaisons multiples).
Script : `gap_calibration.py` (20 000 permutations de K4, fréquences conservées) ; sortie : `gap_calibration.out`.

| Mesure | Valeur |
|---|---|
| Coïncidences à l'écart 7 | 9 observées ; 3,23 attendues (σ 1,75) ; z = 3,29 |
| p local (écart 7 seul) | **0,0048** |
| p global (plus grand z sur les écarts 1-48) | **0,146** |
| Autres écarts à z ≥ 2 | 38 (5 coïncidences, z = 2,03) |

**Lecture.** Si l'écart 7 est examiné **après coup** parmi 48 : compatible avec le hasard (p ≈ 0,15). S'il existe une
raison **a priori** de l'examiner (mot-clé KRYPTOS de 7 lettres, période 7) : p ≈ 0,005, indice intéressant ; un
Vigenère/Quagmire de période 7 sur de l'anglais attendrait ≈ 5,9 coïncidences à l'écart 7 (IC anglais × 90), l'observé
(9) est au-dessus. Statut : OBSERVATION, dont le poids dépend du caractère a priori de l'hypothèse « période 7 ».
