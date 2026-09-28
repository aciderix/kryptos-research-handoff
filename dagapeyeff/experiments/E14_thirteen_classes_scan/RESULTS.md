# E14 — RÉSULTATS (2026-09-28) : « 13 classes » + colonnaire simple (largeurs 2-11) **exclue** ; double ≤ 7×7 non concluant

Pré-inscription : `PREREGISTRATION.md`. Contrôles : `logs/e14_{E,I,D}.out` (30 par famille ; anglais réservé fusionné
en 13 classes équilibrées ; symboles aléatoires ; transposition aléatoire ; étage 1 seul). Référence : R max des
mélanges du chiffré réel à largeur égale (journaux E04 ; + 2 000 mélanges d'E08 en largeur 7).

| Famille | R(vraie clé) > max du null de même largeur | Vraie clé en tête | Décision (seuil ≥ 90 %) |
|---|---|---|---|
| E colonnaire standard, largeurs 2-11 | **28/30** (93 %) | 20/30 | **exclue** |
| I colonnaire inverse, largeurs 2-11 | **29/30** (97 %) | 7/30 | **exclue** |
| D double colonnaire, 2-7 × 2-7 | 22/30 (73 %) | 10/30 | puissance insuffisante — **non concluant** |

Marges typiques : R(vraie clé) 57-173 contre un R max du null de 14-54 (ex. largeur 2 : 94 contre 19).
Le vrai chiffré n'a, dans E04, aucun R hors de la distribution des mélanges ⇒ sous l'hypothèse « 13 classes
équilibrées », la transposition n'est **pas** une colonnaire simple (standard ou inverse) de largeur 2-11.

**Limites** : partition équilibrée fixe dans les contrôles (une autre partition changerait les répétitions) ; nulls
de 10 mélanges par largeur (hors largeur 7), mais marges larges. **Reste ouvert** : double transposition (non
concluant), largeurs ≥ 12, autres familles → solveur complet E15.
