# E07 — Familles colonnaires + Polybe, clair en ESPÉRANTO — PRÉ-INSCRIPTION

Rédigée et commitée avant tout run de E07 sur le vrai chiffré (2026-09-28).

## Motivation
- msgtrail.com affirme qu'une « colonnaire 2×98 + Polybe standard » donne des scores « au-dessus de l'anglais » et
  que 30 relances retrouvent un vocabulaire espéranto (ESTIS, KAJ, KIEL, TRADUK, KODO, KONTRAU, LANDO), **sans**
  clair complet ni clé (REVENDICATION non vérifiable). dagapeyeffresearch compare espéranto et anglais par des
  scores bruts (échelles non comparables), sans contrôle.
- E04/E05 couvrent ces familles en 7 langues, **pas en espéranto** : c'est le seul trou identifié.

## Modèle
Espéranto : 18 livres Gutenberg (17 pour l'entraînement + 1 réservé ; langue vérifiée, `logs/corpus.txt`), en-têtes retirés, **diacritiques supprimés**
(ĉ→C, ĝ→G, ĥ→H, ĵ→J, ŝ→S, ŭ→U ; convention choisie d'avance, comme pour les autres langues ; l'alphabet
espéranto n'a pas Q, W, X, Y), A-Z ; 2,17 M lettres d'entraînement ; *Robinsono Kruso* (n° 11511) réservé aux
contrôles. Quadgrammes joints (`data/models/qg_eo.bin`).

## Cellules (mêmes outils et réglages qu'E04/E05, `tools/e04_scan.c`)
| Cellule | Famille | Géométrie |
|---|---|---|
| E07-E-B / E07-E-A | colonnaire standard, toutes clés, largeurs 2-11 (**2×98 inclus**), grilles incomplètes | B (182) / A (196) |
| E07-I-B / E07-I-A | colonnaire inverse, idem | B / A |
| E07-D-B | double colonnaire 2-7 × 2-7 | B |
Étage 1 TOP 50 (E, I) / 20 (D) ; étage 2 : 8 départs × 40 000, quadgrammes espéranto.

## Contrôles et null
- Positifs (textes réservés espéranto) : E-B 10, I-B 10, E-A 5, I-A 5, D-B 5 ; admissibilité ≥ 6/10 ou ≥ 3/5.
  Mesure : ≥ 90 % des lettres (décalage ≤ 16 toléré) ; pour I, carré retrouvé (amendement 1 d'E04).
- Null : 5 mélanges (E, I), 3 (D) par cellule admissible.

## Critères de succès (tous requis)
1. qoff(réel) > max(null) + 0,5 **et** ≥ min(qoff des contrôles récupérés) − 0,5 ;
2. texte espéranto continu sur ≥ 80 % (jugé après 1, avec dictionnaire) ;
3. ré-enchiffrement exact ; 4. stabilité (autre graine) ;
5. pour la revendication msgtrail : la largeur 2 doit donner le meilleur qoff et un texte lisible.
Sinon : **négatif**, et la revendication msgtrail est consignée comme non reproduite.
