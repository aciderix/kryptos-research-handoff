# E06 — RÉSULTATS (2026-09-28) : **NÉGATIF** ; l'excès de répétitions en lecture verticale est **expliqué**

Pré-inscription : `PREREGISTRATION.md`. Exécution partagée : anglais B ici (`logs/`), autres langues et anglais A
par l'autre agent via `run_E06.sh` (commit 6e28170 de sa branche ; journaux copiés dans `logs/run_agent2/`,
vérifiés contre son résumé).

## 1. Q2 — déchiffrement (RÉSULTAT)
| Cellule | Contrôles | Réel (meilleur qoff) | Null (max) | Verdict |
|---|---|---|---|---|
| anglais, B | 6/10 | −12,569 / −12,643 (2 graines, clés différentes) | −12,371 (5) | négatif |
| anglais, A | 3/5 | −12,653 | −12,527 (3) | négatif |
| français, B | 4/5 | −12,435 | −12,344 (3) | négatif |
| espagnol, B | 3/5 | −13,032 | −12,667 (3) | négatif |
| allemand, italien, latin, néerlandais (B) | 1/5, 1/5, 2/5, 1/5 | non lancé (règle pré-inscrite) | — | non testé (puissance insuffisante) |

## 2. Q1 — la structure se renforce-t-elle ? (RÉSULTAT)
R max (statistique invariante, indépendante de la langue) par mode :

| Géométrie | Mode | Réel | Mélanges totaux | Mélanges **par colonne** (composition des colonnes préservée) |
|---|---|---|---|---|
| B | P0 | 56 | 48-61 (17) | 50-55 (6) |
| B | P1 | 63 | 49-62 (17) | 48-62 (6) |
| B | P2 | 61 | 48-64 (17) | 52-57 (6) |
| A | P0 | 59 | 43-54 (9) | 45-60 (6) |
| A | P1 | 63 | 46-54 (9) | 50-64 (6) |
| A | P2 | 64 | 47-58 (9) | 50-59 (6) |

- Critère pré-inscrit (P0, B) : **non atteint** (56 ≤ max du null).
- Excès observé en A contre les mélanges totaux (+11 à +13) : **disparaît** contre les mélanges par colonne (seul
  P2 reste à +5, un mode sur six) ⇒ il venait de la **composition des colonnes** (colonne 14 : symboles rares), pas
  d'un ordre de lecture. En B, avec 17 mélanges, le réel n'est plus hors distribution.
- L'excès « largeur 7 » d'E04 (famille de 5 040 clés, R = 46 contre ≤ 38 sur 12 mélanges par colonne) **reste**
  une OBSERVATION non expliquée ; E06 montre qu'il ne se généralise pas aux ordres de lignes indépendants.

## 3. Conclusion
Lecture verticale des colonnes imprimées avec ordres à clé des lignes paires/impaires (3 modes × 7!² clés) +
Polybe : aucun texte en anglais, français, espagnol (A et B) ; non concluant pour 4 langues (contrôles trop
faibles pour cette famille). Q1 négatif.
