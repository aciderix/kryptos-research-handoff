# E16 — RÉSULTATS (2026-09-28) : « 13 classes » + routes / nulles régulières / clés K1-K2 : **NÉGATIF** ; clé libre W = 13/14 : **sans puissance**

Pré-inscription : `PREREGISTRATION.md` (commit 211921d, avant tout calcul réel). Outil : `tools/e16_h13.c` ; journaux : `logs/`.

## Contrôles (anglais réservé → 13 classes équilibrées → mécanisme aléatoire ; succès ≥ 80 % des lettres)
| Cellule | Lus | qoff des lus (min) | Admissible (≥ 6/10) |
|---|---|---|---|
| R routes (128 candidats, 10 retenus par R) | **9/10** (166-176/182) | −9,20 … −9,88 (**−9,878**) | oui |
| N nulles une sur k (k = 3-5, 12 candidats) | **7/10** (106-146 lettres) | −9,12 … −9,50 (**−9,499**) | oui |
| F colonnaire clé libre W = 13/14, conventions E/I (étage 1) | R trouvé > max des mélanges (77) : **4/10** (seuil 8/10) | — | **non** |
F : le recuit atteint R = 62-77 sur les **mélanges** du chiffré réel, autant que sur la plupart des contrôles (70-79) ; seuls les
contrôles à forte répétition (R = 105-187) se détachent. Conformément à la règle pré-inscrite (et à la limite déjà notée en E04
§ 5-6), F est déclarée **sans puissance** : **non concluant**, aucun calcul sur le vrai chiffré.

## Null (5 mélanges des 182 symboles réels, même pipeline)
| Cellule | Null (qoff) | max |
|---|---|---|
| R | −11,450 … −11,299 | **−11,299** |
| N | −10,898 … −10,644 | **−10,644** |
| K | −11,573 … −11,173 | **−11,173** |

## Vrai chiffré (2 graines par cellule)
| Cellule | Graine | qoff | Candidat | Seuil 1a : max(null) + 0,5 | Seuil 1b : min(contrôles) − 0,5 | Stabilité |
|---|---|---|---|---|---|---|
| R | 1631 | −11,328 | diag-main-c2/dir1 (R = 14) | −10,799 | −10,378 | même route, appariements différents |
| R | 1632 | −11,266 | diag-main-c2/dir1 | | | |
| N | 1633 | −11,031 | k = 3, phase 2 | −10,144 | −9,999 | **non** (candidats différents) |
| N | 1634 | −10,792 | k = 4, phase 2 | | | |
| K | 1635 | −10,681 | K2 (R = 13) | −10,673 | (réf. R/N : ≈ −10,4) | **non** (appariements différents) |
| K | 1636 | −10,788 | K2 | | | |
Vérification directe exacte partout (triviale sans les autres critères). Aucun texte continu : mots isolés seulement
(« THERE », « WHOLE », « GOOD »…), attendus du décodage de Viterbi sur du bruit.

## Décision (règles pré-inscrites)
- **R (routes, dont la route « chinoise ») : négatif.** Réel au niveau du null (−11,27 contre max −11,30), 1,4 unité sous
  les contrôles lus.
- **N (nulles une sur 3, 4 ou 5, sans transposition) : négatif.** Réel −10,79 contre max du null −10,64 ; instable.
- **K (clés K1/K2 d'E03) : négatif.** −10,681 reste juste sous le seuil 1a (−10,673), sans lecture ni stabilité. **Biais
  noté** : K1/K2 ont été *sélectionnées sur le vrai chiffré* (E03) pour y maximiser une structure de bigrammes ; les appliquer
  à des mélanges, qui n'ont pas servi à les choisir, avantage mécaniquement le réel. Même avec ce biais favorable, le
  seuil n'est pas atteint et le niveau reste environ 0,8 unité sous les contrôles lus. L'OBSERVATION d'E03 n'est donc pas
  une lecture « 13 classes ».
- **F (largeurs 13/14 à clé libre) : non concluant** (sans puissance, voir plus haut).

## Bilan de l'hypothèse « 13 classes » (E13-E16)
Compatible avec le profil (E13) ; exclue avec colonnaire simple 2-11 (E14), double ≤ 7×7 (E15), 64 routes × 2 sens,
nulles régulières sans transposition et clés K1/K2 (E16). Non testable avec nos outils pour une colonnaire à clé libre de
largeur 13/14 (espace trop grand pour 182 symboles). Restent non testés : autres appariements, latin, Richelieu (T4).
