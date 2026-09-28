# E03 — Polybe 5×5 (carré inconnu) + transposition colonnaire à clé LIBRE, grilles complètes — PRÉ-INSCRIPTION

Rédigée avant tout run sur le vrai chiffré (2026-09-28). **Figée pour la cellule E03-B7 avant son premier run réel ; les autres cellules restent en validation (chacune sera lancée seulement après ses 10 contrôles).** Solveur mis au point sur contrôles seuls.

## Statut de nouveauté
- DÉJÀ TESTÉ : colonnaire largeur 7 (5040 ordres, dagapeyeffresearch, sans contrôle ; « tous les ordres notés
  à l'identique », ce qui suggère un test sans résolution de la substitution) ; colonnaire 14×14 « testé » sans
  détail ; nulles « testées » sans détail. E01 (ici) : colonne 14 = nulles + routes fixes → négatif.
  E02 (ici) : largeur 14 avec partition de bourrage → négatif.
- NOUVEAU (dans les sources consultées) : clé **libre** (jusqu'à 14! ≈ 8,7·10¹⁰) résolue **conjointement** avec
  un carré inconnu, par un solveur validé sur messages plantés ; combinée à l'hypothèse « colonne 14 = nulles ».

## Représentation / alignement
Comme E01. Géométrie **B** : colonne 14 imprimée retirée (14 nulles) → suite de 182 symboles lue ligne par ligne.
Géométrie **A** : les 196 symboles.

## Mécanisme
Clair écrit ligne par ligne dans une grille complète W colonnes × H lignes (W·H = N) ; colonnes lues de haut en
bas dans l'ordre de la clé, concaténées ⇒ chiffré. D'où clair(r, c) = S[P(c)·H + r]. Puis substitution de Polybe.
Pour (B), le chiffré de 182 symboles a ensuite été imprimé par lignes de 13, suivies chacune d'une nulle.

## Cellules (fermées)
| Cellule | Géométrie | N | W × H | Clés |
|---|---|---|---|---|
| E03-B13 | B | 182 | 13 × 14 | 13! ≈ 6,2·10⁹ |
| E03-B14 | B | 182 | 14 × 13 | 14! |
| E03-B7  | B | 182 | 7 × 26 | 7! = 5040 |
| E03-A14 | A | 196 | 14 × 14 | 14! (généralise E02 sans partition imposée) |

## Méthode (`tools/e03_solver.c`)
1. Recuit sur le **carré** (SIGIT = 5 000 propositions) ; chaque carré candidat est noté par la valeur du
   **meilleur ordre de colonnes calculé exactement** (programmation dynamique sur les sous-ensembles, score
   bigramme par paires de colonnes adjacentes, bigrammes marginalisés de `qg_joint`).
2. Puis ROUNDS = 4 alternances : clé optimale exacte (DP avec liaison fin de ligne → début de ligne suivante) ;
   carré par recuit quadgrammes (40 000 itérations).
3. RESTARTS = 24 départs (rang de fréquence, perturbé). Score final : qoff quadgrammes joints sur N lettres.

## Contrôles (avant le vrai chiffré)
1. **Positifs** : pour chaque (W, N), textes d'*Alice* (hors corpus), carré et clé aléatoires ; succès si ≥ 90 %
   des lettres. Seuil d'admissibilité d'une cellule : **≥ 6/10** (abaissé de 8/10 : clé libre 14! ; le taux
   mesuré est rapporté et fixe la puissance). Une cellule sous 6/10 n'est **pas lancée** sur le réel.
2. **Null** : 10 mélanges des N symboles par cellule (coût : ≈ 2,5 min par résolution). Si le réel dépasse le max
   du null, 20 mélanges supplémentaires **avant** toute autre interprétation.

## Critères de succès (tous requis)
1. qoff(réel) > max(null) + 0,5 **et** ≥ min(qoff des contrôles récupérés) − 0,5 ;
2. anglais continu lisible sur ≥ 80 % (jugé après 1) ;
3. stabilité : même clair (≥ 95 %) sur ≥ 3 graines indépendantes ;
4. ré-enchiffrement exact des 392 chiffres (nulles de la colonne 14 conservées en B) ;
5. mécanisme sans paramètre ajusté au-delà de (géométrie, W) fixés ici.
Sinon : **négatif**, consigné tel quel.

## Mise au point (contrôles seuls)
- Alternance naïve (carré par fréquences → DP → recuit) : 0/3 (W=13) ; la DP seule est correcte (13/13 avec le
  vrai carré), c'est l'amorçage qui échoue.
- Statistique invariante par substitution (répétitions de bigrammes/trigrammes de symboles) : trop faible sur
  182 lettres (vraie clé maximale dans 1 cas sur 6) → abandonnée.
- Recuit sur le carré noté par la clé exacte (ci-dessus) : 2/3 (W=13, 4 départs).
- Même solveur, 6 départs, 5 contrôles par configuration (`logs/mise_au_point/`) : B13 4/10, B14 3/5, A14 2/5
  → 9/20 ≈ 45 % ; les échecs finissent au niveau du hasard (≈ −12,2), donc ce sont des départs ratés, pas de
  faux optimums. Taux par départ ≈ 9,5 % ⇒ 24 départs (≈ 90 % attendu), à **mesurer** sur 10 nouveaux contrôles.

## Contrôles définitifs (24 départs)
- E03-B7 : **10/10** (179-182/182 lettres ; qoff −9,23 … −10,16 ; min récupéré **−10,156**) → admissible.
- E03-B13, B14, A14 avec ce solveur (DP exacte dans le recuit du carré) : trop lent (30-60 min par contrôle,
  B13 1/3, B14 1/1, A14 1/1 au moment de l'arrêt ; `logs/mise_au_point_v2/`).

## AMENDEMENT 1 (avant tout run réel de B13, B14, A14) — solveur accéléré
Pendant le recuit du carré, la DP exacte est remplacée par la **relaxation d'affectation** (algorithme hongrois,
majorant du meilleur chemin, O(W³)) ; la clé finale reste calculée **exactement** par DP à chaque alternance.
Réglages : SIGIT = 20 000, ROUNDS = 6, RESTARTS = 24 (≈ 15 s par résolution en largeur 14). Mêmes cellules,
mêmes critères, même seuil (≥ 6/10 contrôles). B7 (déjà exécutée avec la version exacte) est re-contrôlée avec
cette version pour comparaison, sans nouveau run réel.
