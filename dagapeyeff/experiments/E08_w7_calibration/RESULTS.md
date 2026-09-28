# E08 — RÉSULTATS (2026-09-28) : observation « largeur 7 » **compatible avec le hasard** — close

Pré-inscription : `PREREGISTRATION.md` (commitée avant le calcul). Journaux : `logs/` ; calcul du p global :
`global_p.py`.

## 1. p local (famille E, largeur 7, géométrie B ; R réel = 46)
| Null | Mélanges | Moyenne (σ) | Quantile 99 % / 99,9 % | Max | # ≥ 46 | p local |
|---|---|---|---|---|---|---|
| total | 2 000 | 32,3 (4,1) | 44 / 50 | 53 | 11 | **0,006** |
| par colonne | 2 000 | 32,9 (4,1) | 45 / 51 | 54 | 18 | **0,0095** |
Les petits échantillons antérieurs (10-36 mélanges, moyenne ≈ 28-30) avaient **sous-estimé** la queue de la
distribution.

## 2. p global (familles E et I, largeurs 2-9, géométrie B ; 500 mélanges)
z réels : seul E7 (z = 3,23) et, dans une moindre mesure, I3 (z = 2,31) sortent ; z max réel = 3,23.
Dans 56 mélanges sur 500, le z maximal sur les 16 cellules est ≥ 3,23 ⇒ **p global = 0,114**.

## 3. Décision (règle pré-inscrite)
Il fallait p global < 0,01 **et** p local (par colonne) < 0,001. Obtenu : 0,114 et 0,0095 ⇒ **« compatible avec
le hasard après correction »**. L'OBSERVATION d'E04 § 6 est **close** : elle n'indique pas de structure réelle.

## 4. Leçon de méthode (consignée)
Un excès remarqué après coup parmi de nombreuses cellules, jugé sur 10-40 mélanges, paraissait « p ≈ 10⁻³ » ; avec
2 000 mélanges et une correction pour comparaisons multiples, il retombe à p ≈ 0,1. Toute observation future de ce
type sera calibrée ainsi **avant** d'y consacrer une expérience.
