# Matrice des mécanismes documentés dans *Codes and Ciphers* (d'Agapeyeff, 1939)

Source : scan de l'édition originale (archive.org `codesciphers0000daga`, fourni par le user, non versé au dépôt).
Pages = pagination du livre. « Effet sur les comptes » = effet sur le **profil de fréquences des paires**, seule
grandeur que mesure E11 (invariante par toute transposition des paires et par tout carré).

| # | Mécanisme (pages) | Représentation intermédiaire | Effet sur les comptes des paires | Compatible avec l'alternance {6789 0}/{12345} ? | Testé | Résultat |
|---|---|---|---|---|---|---|
| 1 | Carré de Polybe 5×5 numéroté (p. 16) | lettre → (ligne, colonne) | profil = profil de la langue (relabellisé) | oui | E01-E09 (+ E11) | rejeté par E11 (profil) |
| 2 | Carré à **mot-clé** : mot-clé puis reste de l'alphabet, J omis/fusionné (CLIQUE p. 123 ; MANCHESTER p. 117-118, 124) | idem | idem (injectif) | oui | E11 (tout carré) ; E10 suspendu | rejeté par E11 (profil) |
| 3 | « Figure ciphers » : lettre → nombre à 2 chiffres arbitraire 10-99 ; paires « impaires » = syllabes ou code (p. 127) | lettre → 2 chiffres | injectif ⇒ profil de la langue ; avec syllabes : profil de syllabes | oui si les nombres sont choisis ainsi | E11 (cas injectif) | cas injectif rejeté ; **syllabes non testé** |
| 4 | Carré de **Wolseley** (MAJESTY), cases numérotées 1-12 par paires symétriques, centre sans numéro (p. 109-111) | lettre ↔ lettre partenaire (réciproque) | injectif ⇒ profil de la langue. Variante « on note le **numéro** » (plusieurs-à-un) : **13 classes** | oui | E11, E12 | standard rejeté ; variante « numéro » : ≈ 13 symboles mais pas uniformes (E12) |
| 5 | **Nulles** : bourrage final (p. 51 « abcd », 110, 118 « EE = Z », 125 « D », 130 « WA, WE, W ») | + quelques symboles | négligeable (≤ quelques %) | oui | E02, E09 (bourrage final) | — |
| 6 | **Nulles « une lettre sur 3, 4 ou 5 », insérées après chiffrement** (p. 111) | mélange clair + nulles | mélange ; **ne peut qu'augmenter** le nombre de symboles distincts | oui | E12 | ne reproduit pas le profil (P conjointe = 0 ; p ≤ 0,003) |
| 7 | Playfair (CLIQUE, p. 122-124) ; nulle entre lettres doublées | digrammes → digrammes | profil aplati, ~24-25 symboles | oui (si recodé en coordonnées) | E12 | ne reproduit pas le profil |
| 8 | Chiffre combiné substitution-transposition (**fractionnation** : coordonnées transposées une par une, MANCHESTER, p. 124-125) | coordonnées séparées | paires = (coord. d'une lettre, coord. d'une autre) ⇒ table ≈ produit ligne × colonne | **non** en général (démontré pour les colonnaires) | E11, E12 | rejeté : indépendance ligne × colonne rejetée (p = 0,0002) ; alternance respectée dans 0,3 % des simulations |
| 9 | Transposition nihiliste (même clé lignes/colonnes, nulles « abcd », p. 50-51) | transposition de lettres | aucun | oui | E09 | rejeté |
| 10 | Colonnaire ordinaire (MANCHESTER, p. 118) ; alphabet transposé (p. 117) ; route « chinoise » (p. 126) | transposition / substitution | aucun / injectif | oui | E01-E05 | rejeté |
| 11 | Vigenère, glissière de St-Cyr, roue à chiffrer (p. 119-122, 146-150) | polyalphabétique | aplatit, ~25 symboles | oui (si recodé) | E12 | ne reproduit pas le profil |
| 12 | Homophones de voyelles + lettres factices entre les mots (p. 37-38) | plusieurs symboles par voyelle | augmente les symboles distincts | oui | E12 | ne reproduit pas le profil |
| 13 | Codes de dictionnaire (page + rang du mot ⇒ chiffres), puis paires de chiffres → lettres par table (p. 129-130) ; Mansfield (p. 153-155) ; syllabes de Selenus (p. 127) | nombres ⇒ chiffres | chiffres ≈ uniformes sur 0-9 dans **les deux** positions | **non** tel quel (les deux positions utiliseraient 0-9) | **non** | à examiner |
| 14 | Grille de Cardan (p. 113-115) | message caché dans un texte anodin | — | — | non | hors sujet (pas un chiffre à paires) |
| 15 | **Erreurs de l'encodeur** (lettres omises, fautes : exemple résolu p. 141-146) | — | omissions : **ne réduisent pas** le nombre de symboles | — | E12 | ne reproduit pas le profil |

## Témoin historique (p. 141-142)
Cryptogramme « d'un ami », chiffré en coordonnées A-E (carré mélangé), cassé par l'auteur par fréquences : 89 paires,
**22 symboles distincts**, profil de type anglais (10, 9, 9, 9, 7, 6, 5, 4, 3, 3, 3, …), avec erreurs d'encodage.
C'est ce à quoi ressemble, dans le livre même, un chiffre par coordonnées d'un texte anglais.

## Ce que la source primaire permet d'inférer (sans calcul)
- L'auteur enseigne et pratique le carré à mot-clé, les coordonnées, les nulles (finales et « une lettre sur k »),
  la fractionnation, le nihiliste, les codes numériques ; il insiste sur les **erreurs d'encodage** et affirme plus
  tard avoir **oublié** sa méthode.
- Argument structurel (à vérifier numériquement en E12) : parmi ces mécanismes, **seuls** un codage plusieurs-à-un
  (#4 variante « numéro », #3 syllabes, #13 code) ou un clair à alphabet restreint peuvent **diminuer** le nombre de
  symboles distincts ; nulles (#5, #6), homophones (#12), polyalphabétiques (#11), Playfair (#7), transpositions
  (#9, #10) et omissions (#15) le laissent inchangé ou l'augmentent.
