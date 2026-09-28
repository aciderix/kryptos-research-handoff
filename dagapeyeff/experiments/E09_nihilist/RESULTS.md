# E09 — RÉSULTATS (2026-09-28) : **NÉGATIF** sur les 20 cellules admissibles

Pré-inscription : `PREREGISTRATION.md` (commit 7fbe9b8, avant tout run réel). Solveur : `tools/e03_solver.c`
(option TRANSP). Exécution partagée : 11 cellules ici (`logs/local/`), 12 par l'autre agent (commit b2f7985 de sa
branche ; journaux copiés dans `logs/run_agent2/` et vérifiés).

| Cellule | Langue | Contrôles | min qoff contrôle | Réel | Null max (3) | Écart réel − null |
|---|---|---|---|---|---|---|
| T1A14 (par lignes, A) | en | 3/5 | −9,55 | −12,004 | −11,964 | −0,04 |
| | fr | 5/5 | −9,56 | −12,124 | −12,171 | +0,05 |
| | de | 5/5 | −9,35 | −12,659 | −12,746 | +0,09 |
| | it | 5/5 | −9,57 | −12,167 | −11,951 | −0,22 |
| | es | 4/5 | −9,82 | −12,072 | −12,357 | **+0,29** |
| | la | 4/5 | −9,73 | −11,658 | −11,584 | −0,07 |
| | eo | 4/5 | −10,19 | −12,031 | −11,877 | −0,15 |
| | nl | 1/5 | — | non lancé | — | — |
| T1B13 (par lignes, B) | en | 4/5 | −9,63 | −11,967 | −11,650 | −0,32 |
| | fr | 5/5 | −9,78 | −11,750 | −11,869 | +0,12 |
| | de | 5/5 | −9,55 | −12,716 | −12,559 | −0,16 |
| | it | 5/5 | −9,56 | −11,871 | −11,579 | −0,29 |
| | es | 5/5 | −10,26 | −12,260 | −11,978 | −0,28 |
| | la | 5/5 | −9,60 | −11,492 | −11,335 | −0,16 |
| | nl | 4/5 | −10,17 | −12,479 | −12,324 | −0,16 |
| | eo | 3/5 | −10,01 | −12,033 | −11,800 | −0,23 |
| T0A14 (par colonnes, A) | fr | 4/5 | −9,39 | −12,205 | −11,948 | −0,26 |
| | de | 5/5 | −9,33 | −12,838 | −12,694 | −0,14 |
| | it | 5/5 | −9,64 | −12,114 | −11,994 | −0,12 |
| | es | 5/5 | −9,98 | −12,446 | −12,352 | −0,09 |
| | la | 4/5 | −9,69 | −11,638 | −11,573 | −0,07 |
| | nl, eo | 1/5, 1/5 | — | non lancés | — | — |

## Critère 1 (qoff > max(null) + 0,5 **et** ≥ min(contrôles) − 0,5)
Aucune cellule. Le plus grand écart au null (+0,29, T1A14 espagnol) reste sous +0,5, et le réel y est à 2,2 unités
sous les contrôles (seuil ≈ −10,3 ; réel −12,07) ; « clair » sans aucun mot (`IONYLUSILUENAUDIALRYSESSEA…`). Avec
20 cellules et 3 nulls chacune, 3 dépassements positifs faibles (+0,05 à +0,29) sont attendus par hasard.

## Conclusion
- **RÉSULTAT** : la transposition nihiliste 14×14 (lignes et colonnes permutées par la même clé), lue par lignes
  (géométries A et B) ou par colonnes (A), suivie d'un Polybe, ne produit de texte dans **aucune** des langues
  testées (8 pour la lecture par lignes ; 6 + anglais via E02/E03 pour la lecture par colonnes). Néerlandais et
  espéranto non admissibles dans 3 cellules (contrôles 1/5).
- Le mécanisme qui expliquait le plus naturellement l'anomalie de la colonne 14 (lecture par colonnes : dernière
  ligne du clair = une colonne imprimée) est donc **réfuté** à la puissance mesurée (4-5 contrôles sur 5).
