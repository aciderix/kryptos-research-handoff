# K01 — Structure du cryptogramme : sections, homogénéité, isomorphes, anagrammes — PRÉ-INSCRIPTION

Rédigée avant tout calcul sur le texte (2026-09-28). Seul calcul déjà fait : les longueurs de sections (validation de la
transcription, README).

## Données
`data/transcription_v0.txt`. Unités : lettres a-z et ê ö ü ; variante A = apostrophes et soulignements ignorés, ê→e ö→o ü→u
(alphabet réduit) ; variante B = chaque lettre suivie d'apostrophe est un symbole distinct. Sections S1..S6 = blocs complets entre
lettres soulignées (S7 = 144 lettres, traitée à part ; « eimat » exclu).

## Hypothèses et tests (statistiques fixées ici, p par permutation, 10 000 tirages)
- **H-transp** (même texte, transposé différemment dans chaque section, lettres intactes) : comptes de lettres des sections presque
  identiques. Test T1 : χ² d'homogénéité des comptes S1..S6 ; null = découpage du texte entier (mélangé) en blocs de mêmes tailles.
  Prédiction H-transp : χ² **anormalement petit** (p_bas < 0,01).
- **H-subst-multiple** (même texte, clé de substitution différente par section) : profils de fréquences **triés** identiques, lettres
  non. Test T2 : distance entre profils triés des sections vs null (idem). Prédiction : distance anormalement petite.
- **H-même-clé** (textes différents, même substitution) : les tests T1/T2 au niveau du hasard.
- T3 **isomorphes** : pour deux sections et un décalage d ∈ [−10, 10], proportion de paires de positions (i, j), |i − j| ≤ 20, dont
  l'égalité des symboles coïncide dans les deux sections ; null = mélange. Un même clair sous substitutions différentes donne un
  excès net au bon décalage.
- T4 **anagrammes allemands** : fraction des mots (≥ 4 lettres, variante A) dont la multiensemble de lettres est celle d'un mot du
  lexique allemand (mots de `dagapeyeff/data/heldout/de.txt` et du corpus d'entraînement si disponible) ; comparée à la même
  fraction pour des mots aléatoires de même longueur tirés selon les fréquences du cryptogramme, et pour le néerlandais, l'anglais,
  le latin (autres lexiques disponibles). Prédiction « transposition de l'allemand dans les mots » : fraction allemande ≫ contrôles.

## Contrôles synthétiques (avant le vrai texte)
Texte allemand réservé de 1 000 lettres en 6 blocs : (a) un même bloc transposé 6 fois ; (b) un même bloc sous 6 substitutions ;
(c) 6 blocs différents sous une même substitution ; (d) mots allemands anagrammés. Chaque test doit classer correctement (a)-(d)
(p < 0,01 dans la bonne direction) sinon il est déclaré sans puissance.

## Portée
Tests de structure seulement ; aucune « lecture » ne sera proposée à ce stade.
