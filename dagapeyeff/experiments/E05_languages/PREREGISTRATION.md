# E05 — Familles E04 (colonnaire, double colonnaire) + Polybe, clair dans une AUTRE LANGUE — PRÉ-INSCRIPTION

Rédigée et commitée avant tout run de E05 sur le vrai chiffré (2026-09-28).

## Motivation
1. E01-E04 supposent un clair **anglais** (quadgrammes anglais à l'étage 2). Le livre est en anglais, mais
   d'Agapeyeff était d'origine russe ; dagapeyeffresearch dit avoir essayé RU/FR/DE/eo, sans contrôle publié.
2. OBSERVATION (issue des journaux E04, étage 1 **indépendant de la langue**) : en géométrie B, famille E,
   largeur 7, le maximum de R (répétitions de n-grammes de symboles) vaut **46** pour le vrai chiffré, contre
   22-35 (10 mélanges totaux, journaux E04) et ≤ 41 sur 36 mélanges supplémentaires (totaux, par colonnes, par
   lignes). Meilleure clé `7 3 6 1 4 2 5` : deux 6-grammes répétés (`85 74 91 82 81 64`, `62 75 82 81 62 81`),
   formés de cases d'une même colonne imprimée lues dans l'ordre de lignes 7, 11, 3, 9, 13, 5, 1 (puis paires).
   Excès aussi en D (largeur interne 7 : W2 = 4, 5, 7). L'étage 2 anglais ne donne rien (E03-B7, E04).
   Tenant compte d'≈ 60 cellules examinées, excès significatif (p ≈ 10⁻³), mais **non expliqué**.
   Un recuit sur les 14! ordres de lignes (famille plus grande) produit autant de répétitions sur des mélanges :
   la famille à 14! n'est **pas** informative ; seule l'exhaustivité sur petite famille l'est.

## Langues et modèles
FR, DE, IT, ES, LA, NL. Corpus Gutenberg (identifiants vérifiés par la ligne « Language: » ou le catalogue
officiel), en-têtes retirés, accents supprimés, A-Z ; quadgrammes joints (`tools/build_qg_joint.c`, plancher
0,01 compte). Entraînement / réservé (contrôles) : FR 2,46 M / 0,16 M (Candide) ; DE 1,68 M / 0,10 M (Die
Verwandlung) ; IT 1,91 M / 0,40 M ; ES 1,65 M / 1,39 M (La Regenta) ; LA 0,57 M / 0,08 M ; NL 1,60 M / 0,17 M.
Liste des fichiers : `logs/corpus.txt`.

## Cellules (géométrie B, 182 symboles ; mêmes réglages qu'E04)
Pour chaque langue : **E** (colonnaire standard, toutes clés, largeurs 2-11, grilles incomplètes ; TOP 50) et
**D** (double colonnaire 2-7 × 2-7 ; TOP 20). Étage 2 : 8 départs × 40 000, quadgrammes **de la langue**.
Famille I non reprise : son étage 1 (indépendant de la langue) est au niveau du bruit (R ≤ 22).

## Contrôles et null (par langue)
- Positifs : E 10 textes, D 5 textes, tirés du livre **réservé** de la langue ; admissibilité ≥ 6/10 (E),
  ≥ 3/5 (D). Une langue non admissible n'est pas lancée sur le réel pour cette famille.
- Null : E 5 mélanges, D 3 mélanges par langue, pipeline complet, quadgrammes de la langue.

## Critères de succès (par langue et famille, tous requis)
1. qoff(réel) > max(null) + 0,5 **et** ≥ min(qoff des contrôles récupérés) − 0,5 (échelles propres à chaque
   langue ; aucune comparaison entre langues) ;
2. texte continu lisible dans la langue sur ≥ 80 % (jugé après 1, avec dictionnaire / traduction) ;
3. ré-enchiffrement exact ; 4. stabilité (autre graine, même clé) ; 5. correction de Bonferroni implicite :
   un seul succès parmi 12 cellules doit dépasser le null d'au moins 0,5 (déjà exigé par 1).
Sinon : **négatif**. L'OBSERVATION « largeur 7 » reste alors à expliquer (E06).
