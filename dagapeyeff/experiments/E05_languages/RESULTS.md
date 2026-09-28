# E05 — RÉSULTATS (2026-09-28) : **NÉGATIF** sur les 11 cellules admissibles

Pré-inscription : `PREREGISTRATION.md`. Outil : `tools/e04_scan.c` (mêmes réglages qu'E04), quadgrammes par langue
(`data/models/`). Journaux : `logs/`. Géométrie B (colonne 14 = nulles, 182 symboles).

| Langue | Famille | Contrôles | min qoff contrôle récupéré | Réel | Null (max) | Verdict |
|---|---|---|---|---|---|---|
| français | E | 9/10 | −9,75 | −12,43 | −12,24 | négatif (sous le null) |
| français | D | 5/5 | −9,53 | −12,36 | −12,16 | négatif (sous le null) |
| allemand | E | 7/10 | −9,59 | −13,09 | −12,86 | négatif (sous le null) |
| allemand | D | 5/5 | −9,26 | −12,91 | −12,86 | négatif (dans le null) |
| italien | E | 6/10 | −9,58 | −12,10 | −12,09 | négatif (= max du null) |
| italien | D | 3/5 | −9,55 | −12,24 | −12,03 | négatif (sous le null) |
| espagnol | E | 6/10 | −9,79 | −12,83 | −12,68 | négatif (dans le null) |
| espagnol | D | 4/5 | −9,79 | −12,53 | −12,47 | négatif (dans le null) |
| latin | E | 6/10 | −9,99 | −11,88 | −11,68 | négatif (dans le null) |
| latin | D | 3/5 | −9,90 | −11,76 | −11,72 | négatif (dans le null) |
| néerlandais | E | 8/10 | −10,17 | −12,80 | −12,68 | négatif (dans le null) |
| néerlandais | D | 2/5 | — | non lancé (non admissible) | — | — |

(Échelles propres à chaque langue : le latin paraît « meilleur » uniquement parce que son corpus est plus petit.)

## Conclusion
- **RÉSULTAT** : colonnaire (toutes clés, largeurs 2-11) et double colonnaire (≤ 7 × 7) + Polybe ne produisent de
  texte dans **aucune** des 6 langues testées ; le réel reste au niveau des chiffrés mélangés, 2,1 à 3,7 unités
  sous les contrôles. Avec E04 (anglais), ces familles sont exclues dans 7 langues à alphabet latin.
- L'OBSERVATION « largeur 7 » (E04 § 6) **n'est expliquée par aucune de ces langues** (meilleur qoff en largeur 7 :
  FR −12,68, ES −13,08, etc.). → E06 (lecture verticale généralisée).
