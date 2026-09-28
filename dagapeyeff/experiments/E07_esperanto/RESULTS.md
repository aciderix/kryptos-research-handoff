# E07 — RÉSULTATS (2026-09-28) : **NÉGATIF** sur les 5 cellules ; revendication « 2×98 espéranto » **non reproduite**

Pré-inscription : `PREREGISTRATION.md` (commit 78236df, avant tout run réel). Modèle : `data/models/qg_eo.bin`.
Exécution : EB, EA, IB, IA ici (`logs/local/`) ; DB par l'autre agent via `run_E07.sh` (commit c00b4bb de sa branche,
journaux copiés dans `logs/run_agent2/` et vérifiés).

| Cellule | Contrôles (espéranto réservé) | min qoff récupéré | Réel (meilleur) | Null (max) | Verdict |
|---|---|---|---|---|---|
| E, B (182) | 6/10 | −10,27 | −12,522 (W = 8) | −12,209 (5) | négatif (sous le null) |
| E, A (196) | 4/5 | −10,62 | −12,839 (W = 2) | −12,519 (5) | négatif (sous le null) |
| I, B | 9/10 | −10,39 | −12,649 (W = 7) | −12,483 (5) | négatif (dans le null) |
| I, A | 5/5 | −10,41 | −12,956 (W = 4) | −12,742 (5) | négatif (dans le null) |
| D, B | 4/5 | −10,05 | −12,373 (6 puis 7) | −12,301 (3) | négatif (sous le null) |

**Largeur 2 (« 2×98 ») en espéranto** : E-B −12,878 ; E-A −12,839 ; I-B −13,244 ; I-A −13,678 — tous au niveau du
hasard, ≈ 2,3 à 3 unités sous les contrôles récupérés. Aucun texte espéranto (clairs dans `logs/`).

## Conclusion
- **RÉSULTAT** : colonnaire (standard, inverse ; largeurs 2-11 ; grilles incomplètes) et double colonnaire (≤ 7 × 7)
  + Polybe (carré quelconque, dont le carré standard) ne produisent **aucun texte espéranto**, alors que le même
  pipeline retrouve 4-9 messages espéranto plantés sur 5-10.
- La revendication de msgtrail (2×98 + Polybe standard → espéranto) **n'est pas reproduite** par un solveur validé
  sur messages plantés, avec null. Son « score au-dessus de l'anglais » est compatible avec un optimum dégénéré
  du modèle de score (cf. E01, amendement 1).
- Avec E04-E05, ces familles sont exclues en **8 langues** (anglais, français, allemand, italien, espagnol, latin,
  néerlandais, espéranto).
