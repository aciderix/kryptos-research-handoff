# E14 — Hypothèse « 13 classes » + transposition : test rapide par la statistique invariante (réutilise E04) — PRÉ-INSCRIPTION

Rédigée et commitée avant le calcul (2026-09-28).

## Idée
Sous H13 (E13 : anglais, 25 lettres fusionnées en 13 classes équilibrées, puis une transposition), la statistique R
d'E04 (répétitions de n-grammes de symboles) est **invariante** par la correspondance classes → symboles, et la
fusion des lettres **augmente** les répétitions. Si la vraie clé de transposition appartient aux familles balayées
exhaustivement par E04 (E, I : largeurs 2-11 ; D : 2-7 × 2-7 ; géométrie B), son R devrait dépasser nettement le
R max des mélanges. Or le R max réel d'E04 est au niveau du null dans toutes ces cellules (E08 : l'excès en
largeur 7 est compatible avec le hasard, p global 0,11).

## Contrôles (seul calcul nouveau)
Textes anglais réservés (*Alice*), 182 lettres, fusion en 13 classes **équilibrées** (E seul ; T+Z, A+Q, O+X, I+K,
N+V, S+B, H+P, R+Y, D+G, L+F, C+W, U+M), classes → 13 symboles aléatoires parmi 25, transposition aléatoire dans la
famille (E, I, D comme E04), `tools/e04_scan.c` option CLS13=1, étage 1 seul. Mesures : R(vraie clé) et rang de la
vraie clé ; 30 contrôles par famille. Référence : distributions du R max des mélanges (journaux E04/E08, par largeur).

## Décision
- Si R(vraie clé) des contrôles dépasse le R max des mélanges de la même largeur dans ≥ 90 % des cas, alors
  **H13 + (familles E, I, D balayées)** est **exclue** (le réel n'a aucun R hors null).
- Sinon, le test est sans puissance et une attaque complète (solveur joint appariement + correspondance +
  transposition, décodage de Viterbi à 2 lettres par symbole) sera pré-inscrite.
- Dans tous les cas, H13 reste ouverte pour les transpositions **hors** de ces familles.
