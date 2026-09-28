# E03 — RÉSULTATS (2026-09-28) : **NÉGATIF** sur les 4 cellules (+ une OBSERVATION à expliquer)

Pré-inscription : `PREREGISTRATION.md` (+ Amendement 1 : solveur accéléré, avant les runs B13/B14/A14).
Solveur : `tools/e03_solver.c`. Journaux : `logs/`.

## 1. Puissance (contrôles, RÉSULTAT)
| Cellule | Solveur | Contrôles récupérés | min qoff récupéré |
|---|---|---|---|
| B7 (7×26) | DP exacte | 10/10 | −10,156 |
| B13 (13×14) | accéléré | 9/10 | −10,039 |
| B14 (14×13) | accéléré | 9/10 | −10,168 |
| A14 (14×14, 196) | accéléré | 8/10 | −9,680 |
Premier solveur connu de nous qui résout, **validé sur messages plantés**, une colonnaire à clé **libre**
jusqu'à 14! jointe à un carré inconnu, sur ~190 lettres.

## 2. Vrai chiffré vs null (RÉSULTAT)
| Cellule | Réel : meilleur des 3 graines | Null (10) : médiane / max | Seuil critère 1 (max + 0,5) | Verdict |
|---|---|---|---|---|
| B7  | −12,556 | −12,63 / −12,482 | −11,982 | **ÉCHEC** (sous le max du null) |
| B13 | −11,786 | −11,96 / −11,799 | −11,299 | **ÉCHEC** (= max du null) |
| B14 | −11,720 | −11,74 / −11,560 | −11,060 | **ÉCHEC** (sous le max du null) |
| A14 | −12,066 | −12,01 / −11,814 | −11,314 | **ÉCHEC** (sous la médiane du null) |
Ré-enchiffrement exact dans tous les cas (0/196) — trivial sans les autres critères. Aucun clair lisible.
Critères 2-5 non atteints (le critère 1 conditionne l'examen).

**Conclusion E03 :** Polybe (carré inconnu) + colonnaire à clé libre sur grille complète — largeurs 7, 13, 14
après retrait de la colonne 14, ou largeur 14 sur les 196 — **ne produit pas d'anglais**, à la puissance mesurée
(80-100 % par résolution ; 3 graines par cellule).

## 3. OBSERVATION : clés récurrentes en B13 (pas un résultat de déchiffrement)
- Sur 11 graines indépendantes, la recherche B13 converge vers **deux clés** seulement :
  K1 = `5 3 7 13 8 6 1 4 9 2 11 12 10` (5 fois) et K2 = `4 9 2 11 7 13 8 6 1 12 10 3 5` (4 fois) ; les adjacences
  6→1, 13→8, 9→2, 2→11, 7→13, 12→10, 4→9, 8→6 reviennent dans 9 à 11 graines sur 11.
- Contrôle de cette récurrence : deux chiffrés **mélangés fixes**, 3 graines chacun → 0 à 5 adjacences communes
  (moyenne 2,2) contre 6 à 10 pour le réel ; B14 et A14 réels : 0 à 2. La récurrence est donc propre au vrai
  chiffré **et** à la géométrie B13.
- Test décisif (carré résolu seul, clé imposée, 100 départs × 200 000 itérations) :
  K1 −11,76 ; K2 −11,69 ; clé identité −13,09 ; clé aléatoire −13,14 ; vraies clés des contrôles −9,1 … −10,2.
  ⇒ K1/K2 captent **une structure réelle** (+1,4 sur une clé quelconque) mais **ne donnent pas d'anglais**
  (≈ 2 unités sous les contrôles ; au niveau de ce que la recherche obtient sur des mélanges).
- Lecture géométrique : en B13, deux blocs adjacents comparent des symboles distants de 14 dans la suite de 182
  imprimée par lignes de 13, c.-à-d. des voisins **en diagonale** de la grille 14×13. HYPOTHÈSE (non testée) :
  le chiffré porte une structure diagonale (reste d'une vraie route/transposition), que B13 capte partiellement.
  À instruire dans une cellule dédiée (E04), avec pré-inscription : quels couples de symboles portent K1/K2 ;
  cette structure existe-t-elle dans des chiffrés synthétiques Polybe + route diagonale + autre couche ?
