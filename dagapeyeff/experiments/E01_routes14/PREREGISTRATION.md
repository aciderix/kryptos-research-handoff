# E01 — Polybe 5×5 (carré inconnu) + transposition de route sur grille carrée — PRÉ-INSCRIPTION

Rédigée et commitée **avant** tout run sur le vrai chiffré (2026-09-28).

**Statut de nouveauté : REPRODUCTION CONTRÔLÉE d'une famille déjà explorée en partie** (Pelling : 16 diagonales ;
numberworld : lignes↔colonnes ; dagapeyeffresearch : colonnaire). Apport : famille de routes **complète et
explicite**, les deux sens, les deux géométries (A) et (B), **contrôles synthétiques, null calibré, prédiction
« colonne 14 » annoncée d'avance**, ré-enchiffrement. Une issue négative est attendue et utile (base de référence).

## Représentation
- 392 premiers chiffres (les 3 zéros finaux sont exclus comme bourrage) → 196 paires, dans l'ordre imprimé.
- Symbole = paire (ligne ∈ {6,7,8,9,0}, colonne ∈ {1..5}) → indice 0..24. 18 symboles observés.

## Alignement
Paire i = chiffres 2i, 2i+1. Grille imprimée G : 14×14, remplie **ligne par ligne** par les 196 paires.

## Mécanisme (deux géométries, fixées d'avance)
- **(A) grille 14×14 entière** : le clair (196 lettres, bourrage compris) a été écrit dans la grille selon une
  route R, et le chiffré lu ligne par ligne ; ou l'inverse (écrit par lignes, lu selon R). Donc
  clair[k] = G[R(k)] (sens direct) ou clair = G lu selon R⁻¹ (sens inverse).
- **(B) colonne 14 = nulles** : on retire la colonne 14 → grille 14×13 (182 symboles), puis même famille de routes
  sur 14×13, les deux sens.
- Puis substitution simple : symbole → lettre (injective ; 26 lettres disponibles, le carré a 25 cases).

## Espace de recherche (fermé)
Routes sur une grille H×W (H×W = 14×14 pour (A), 14×13 pour (B)) :
- 8 transformées diédrales × {lecture par lignes, par colonnes} × {simple, boustrophédon} = 32 ;
- diagonales : 4 coins × {simple, zigzag} × {anti-diagonales, diagonales} = 16 ;
- spirales : 4 coins × {horaire, anti-horaire} × {vers l'intérieur, vers l'extérieur} = 16.
Soit 64 routes × 2 sens × 2 géométries = **256 cellules**. La substitution est résolue dans chaque cellule par recuit
(quadgrammes anglais `qg_big`, corpus Gutenberg 13,6 M lettres ; 8 redémarrages × 60 000 itérations).

## Transformation finale / convention de sortie
Clair en majuscules A-Z, sans espaces ; score = log-probabilité quadgramme moyenne par quadgramme (qoff).
Pour (A), on rapporte aussi qoff sur les 182 premières lettres (le bourrage attendu en fin est exclu du jugement).

## Prédiction annoncée d'avance
Si (A) est vrai avec bourrage final, la route gagnante doit envoyer la colonne 14 **à la fin** du clair (positions
182-195). Si la route gagnante ne le fait pas, c'est un signal de faux positif.

## Contrôles (obligatoires, exécutés avant le vrai chiffré)
1. **Positif** : 10 textes anglais de 196 lettres (extraits du corpus, I/J fusionnés), carré de Polybe aléatoire,
   route aléatoire de la famille, sens aléatoire → le solveur doit retrouver la route **et** ≥ 90 % des lettres
   dans ≥ 8/10 cas. Idem pour (B) avec colonne de nulles.
2. **Null** : 20 permutations aléatoires des 196 paires réelles → meilleur qoff sur les 256 cellules ; on retient
   le maximum et la distribution.

## Critère de succès (tous requis)
1. qoff de la meilleure cellule réelle > max(null) + 0,4 **et** dans la plage des contrôles positifs ;
2. anglais continu lisible sur ≥ 80 % de la longueur (jugement humain **après** le critère 1, pas avant) ;
3. prédiction « colonne 14 » respectée pour (A) ;
4. ré-enchiffrement exact des 392 chiffres avec le carré et la route trouvés ;
5. stabilité : même clair sur ≥ 6/8 redémarrages.
Sinon : **négatif**, consigné tel quel.

---
## AMENDEMENT 1 (2026-09-28) — décidé à cause des CONTRÔLES, AVANT tout run sur le vrai chiffré

**Constat (contrôles positifs en échec, 1/4) :** avec `qg_big` (modèle conditionnel à repli, repris de
`kryptos/`), le recuit converge vers un **optimum dégénéré ≈ −3,24/quadgramme**, alors que la vraie clé vaut
−1,93. Cause : le repli attribue ln(0,5/13) ≈ −3,26 à tout contexte trigramme **jamais vu** ; une substitution libre
envoie les symboles vers des lettres rares pour fabriquer des contextes inédits, notés mieux qu'un quasi-anglais.

**Changements (seuls ceux-ci) :**
1. Score = log-probabilité **jointe** ln(count(abcd)/N), plancher ln(0,01/N) (`tools/build_qg_joint.c`, même
   corpus Gutenberg 13,6 M lettres). Échelle : anglais ≈ −9,5 par quadgramme.
2. Textes des contrôles tirés d'un livre **hors corpus d'entraînement** (Gutenberg n° 11, *Alice*), pour éviter
   un contrôle optimiste.
3. Critère 1 ré-exprimé dans la nouvelle échelle, relativement aux contrôles et au null :
   **qoff(meilleure cellule réelle) > max(null) + 0,5 ET ≥ min(qoff des contrôles récupérés) − 0,5.**
Recuit : 8 redémarrages × 60 000 itérations, comme prévu. Tout le reste (famille, géométries, sens, prédiction
« colonne 14 », critères 2-5) est inchangé.

**Remarque hors E01 :** les recherches Kryptos à alphabet libre qui ont rapporté des plateaux « charabia ≈ −3,25 »
avec `qg_big` peuvent avoir été piégées par ce même optimum dégénéré. À signaler dans `kryptos/`.
