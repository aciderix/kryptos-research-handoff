# E02 — Polybe 5×5 (carré inconnu) + transposition colonnaire à clé, largeur 14, bourrage final — PRÉ-INSCRIPTION

Rédigée et commitée **avant** tout run sur le vrai chiffré (2026-09-28). Les réglages du solveur ont été mis au
point **uniquement sur des contrôles synthétiques** (voir § Mise au point).

## Statut de nouveauté
- DÉJÀ TESTÉ par d'autres : colonnaire en général (dagapeyeffresearch : « columnar keys on Polybius output »,
  5040 ordres de largeur 7, sans détail sur la largeur 14 ni contrôle) ; Pelling 2013 : écriture par lignes,
  extraction par colonnes **avec réordonnancement interne des colonnes**, « 04 » = bourrage final unique.
- NOUVEAU (dans les sources consultées, cf. `docs/01_etat_de_lart.md`) : la contrainte **8 lignes de bourrage /
  6 lignes de message** tirée de la colonne 14, la clé ainsi restreinte à 6!×8! ≈ 2,9·10⁷ ordres, la
  résolution **jointe** clé + carré, avec contrôles, null et prédiction.

## Observation motrice (faite avant E02, sur le chiffré seul)
Les 5 symboles à ≤ 3 occurrences (04, 71, 92, 93, 94 ; le suivant, 72, en a 9) sont en case 14 des lignes
imprimées 3, 4, 7, 8, 9, 10, 12, 14 (8 lignes) ; les lignes 1, 2, 5, 6, 11, 13 finissent par des symboles courants.

## Représentation / alignement
Identiques à E01 : 392 chiffres → 196 paires → symboles 0..24 ; grille imprimée G 14×14 remplie ligne par ligne.

## Mécanisme
1. Clair = M lettres de message + (196 − M) lettres de bourrage, écrit **ligne par ligne** (gauche→droite) dans une
   grille 14×14 [variante secondaire BOU : écriture en boustrophédon].
2. Les 14 colonnes sont lues **de haut en bas** dans l'ordre d'une clé ; la j-ième colonne lue devient la j-ième
   ligne imprimée. D'où : symbole du clair en (r, c) = G[P(c)][r].
3. Substitution : carré de Polybe inconnu (symbole → lettre, injective).

**Règle de bourrage (fixée d'avance)** : les lignes imprimées dont la case 14 porte un symbole à ≤ 3 occurrences
portent les colonnes de bourrage. Donc 8 lignes → M = 188 ; colonnes de bourrage du clair = 6..13 (BOU : 0..7).
La clé P envoie les colonnes de message sur les 6 autres lignes, les colonnes de bourrage sur ces 8 lignes.

Exclu d'avance par les données (non testé) : lecture des colonnes de bas en haut (le bourrage serait en case 1) ;
écriture par colonnes / lecture par lignes sans seconde permutation (le bourrage serait contigu dans une colonne).

## Espace de recherche et méthode
- Clé : 6! × 8! = 29 030 400 ordres (principal). Falsification : clé **libre** (14!, `FREE=1`).
- Recherche emboîtée (`tools/e02_solver.c`) : recuit sur la clé (échange / insertion / inversion d'un segment
  dans une même classe ; température TK = 8 × 2,5 → × 0,02) ; à chaque proposition, le carré est ré-optimisé par
  1 500 itérations de recuit ; 6 000 propositions ; polissage final du carré (200 000 itérations) ; 8 redémarrages.
- Score : quadgrammes **joints** (`qg_joint.bin`, E01 Amendement 1) sur les **188 lettres de message** seulement
  (bourrage exclu) ; qoff = score / 185.

## Contrôles (obligatoires, avant le vrai chiffré)
1. **Positifs** : 10 textes de 188 lettres tirés d'*Alice* (hors corpus) + 8 bourrages pris dans {Q,X,Z,K,V},
   carré aléatoire, clé aléatoire ; le solveur reçoit la vraie partition → succès si ≥ 90 % des lettres dans
   **≥ 8/10** cas.
2. **Null** : 20 chiffrés obtenus en mélangeant les 182 paires hors colonne 14 (colonne 14 fixe ⇒ même
   partition) → même recherche ; on retient max et distribution.

## Critères de succès (tous requis)
1. qoff(réel) > max(null) + 0,5 **et** ≥ min(qoff des contrôles récupérés) − 0,5 ;
2. anglais continu lisible sur ≥ 80 % des 188 lettres (jugé **après** le critère 1) ;
3. **prédiction** : la recherche à clé **libre** (FREE=1, 14!, sans la règle de bourrage) doit retomber sur le
   même clair (≥ 95 % des 188 lettres), donc placer d'elle-même les 8 lignes « rares » en colonnes 6..13 ;
4. ré-enchiffrement exact des 392 chiffres (clair + carré + clé) ;
5. stabilité : même clair (≥ 95 % des lettres) sur ≥ 6/8 recuits indépendants.
Sinon : **négatif**, consigné tel quel. Variante BOU testée ensuite comme cellule distincte, mêmes critères.

Note (critère 3) : les colonnes 6..13 portent 13 lettres de message chacune (lignes 1-13) ; leur ordre est donc
contraint par le texte ; seule la dernière ligne (bourrage) est exclue du score.

## Mise au point (sur contrôles seulement — consignée pour transparence)
- Recuit joint simple (30 % mouvements de clé, échanges seuls, même température) : 0/2 contrôles.
- Diagnostic : **même avec le vrai carré fourni**, les échanges seuls échouaient (0/2) → ajout de l'insertion et
  de l'inversion de segment + température propre à la clé : 3/3 avec vrai carré ; 1/3 en joint.
- Recherche emboîtée (ci-dessus) : 2/3 à effort réduit (4 redémarrages × 3 000), puis réglages finaux ci-dessus.
Aucun de ces essais n'a touché le vrai chiffré.
