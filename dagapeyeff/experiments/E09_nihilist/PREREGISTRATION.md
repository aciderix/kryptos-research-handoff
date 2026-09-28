# E09 — Transposition « nihiliste » 14×14 (lignes et colonnes permutées par la même clé) + Polybe — PRÉ-INSCRIPTION

Rédigée et commitée avant tout run de E09 sur le vrai chiffré (2026-09-28).

## Mécanisme et fondement
Transposition nihiliste (classique des manuels des années 1930, p. ex. Gaines) : clair écrit par lignes dans un
carré N×N, **colonnes puis lignes permutées par la même clé**, lecture **par lignes** ou **par colonnes**. N = 14
s'accorde avec la grille 14×14.
- Lecture **par colonnes** : la dernière ligne du clair (6 lettres + 8 bourrages rares) tombe dans **une seule
  colonne imprimée** — compatible avec l'anomalie de la colonne 14.
- Chaque ligne du clair est une colonne (resp. une ligne) imprimée dont les cases sont permutées par la clé ; la
  permutation des lignes du clair ne touche que 13 jonctions sur 196, donc un solveur « cases permutées, ordre
  des lignes fixé » (clé libre, 14!) détecte le mécanisme même si l'ordre des lignes est faux.

## Statut de nouveauté
- Lecture par colonnes en **anglais** : déjà exclue (E02 contraint, E03-A14 libre, contrôles 8/10).
- Lecture par colonnes dans les 7 autres langues : **non testée**. Lecture par lignes : **non testée** (aucune
  langue). Non trouvée dans les sources consultées.

## Cellules (`tools/e03_solver.c`, solveur E03 accéléré, amendement 1 d'E03 : RESTARTS 24, ROUNDS 6, SIGIT 20 000, SURR 1)
| Cellule | Lecture | Géométrie | Clé | Langues |
|---|---|---|---|---|
| T1A14 | par lignes (TRANSP=1) | A, 14 × 14 | 14! | en fr de it es la nl eo |
| T1B13 | par lignes (TRANSP=1) | B, colonne 14 = nulles, 14 × 13 | 13! | en fr de it es la nl eo |
| T0A14 | par colonnes (TRANSP=0) | A | 14! | fr de it es la nl eo |
Modèles et textes réservés : `data/models/`, `data/heldout/`.

## Contrôles, null, critères
- Contrôles : 5 textes réservés par cellule ; admissible si ≥ 3/5 (sinon réel non lancé).
- Null : 3 mélanges ; réel : 1 graine (2ᵉ graine si critère 1 franchi).
- Succès (tous requis) : qoff > max(null) + 0,5 **et** ≥ min(contrôles récupérés) − 0,5 ; texte lisible ≥ 80 % ;
  ré-enchiffrement exact ; stabilité sur une 2ᵉ graine. Sinon : **négatif**.
- Exécution : `run_E09.sh` (autonome, applique la règle d'admissibilité) ; partagée avec l'autre agent.
