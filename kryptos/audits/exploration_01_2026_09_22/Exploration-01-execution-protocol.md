# Exploration-01 — Protocole d’exécution structurelle

**Statut :** gelé avant exécution.

**Date :** 2026-09-22.

**Expériences réelles :** 5, exactement une par famille.

**Contrôles :** 8 par expérience, avec seeds fixes 1001–1008.

**Aucun paramètre ne sera ajouté après observation.**

## Règles communes

- Entrée réelle : `data/ct.txt`, longueur 97, alphabet A–Z.
- Les fragments `EASTNORTHEAST` et `BERLINCLOCK` servent uniquement de diagnostics pré-déclarés ; ils ne sont pas utilisés pour modifier une recette.
- Chaque transformation est appliquée à l’entrée réelle et aux huit ciphertexts aléatoires de même longueur.
- Un contrôle de permutation aléatoire utilise la même longueur et le même pipeline.
- Les résultats bruts de chaque essai sont conservés.
- Aucune optimisation, aucun tri pour sélectionner une recette et aucune exécution secondaire ne seront effectués.
- Les nulls sont limités : ils évaluent des artefacts de structure et ne prétendent pas reconstruire l’historique.

## Métriques pré-enregistrées

Pour chaque sortie :

1. longueur et alphabet valide ;
2. SHA-256 de la sortie ;
3. nombre d’occurrences exactes de chaque crib dans la sortie ;
4. score de correspondance à la position publique lorsque la longueur reste 97 ;
5. score hors-crib à la position publique lorsque la longueur reste 97 ;
6. nombre de positions inchangées ;
7. nombre de paires adjacentes conservées ;
8. index de coïncidence de la sortie ;
9. pour le masque, cardinalité conservée et nombre de caractères de crib conservés dans la projection fixe.

Aucun de ces diagnostics ne constitue une preuve de plaintext.

## Expérience R — route et transposition

- **experiment_id:** `E01-R-RAGGED-COL7`
- **Hypothèse:** une lecture par colonnes d’un remplissage ragged de largeur 7 peut produire des artefacts structurels distincts d’une permutation aléatoire comparable.
- **Antériorité:** les routes columnar et les variantes de grille sont déjà couvertes dans le dépôt ; cette expérience est un contrôle exploratoire d’une recette fixe, pas une revendication de nouveauté.
- **Transformation exacte:** écrire les 97 caractères ligne par ligne dans des lignes de largeur 7, sans padding ; lire colonne par colonne de gauche à droite, et de haut en bas, en sautant les cellules absentes de la dernière ligne.
- **Paramètres:** largeur 7 ; remplissage interdit ; lecture colonne-major ; sens gauche-droite ; indexation zéro.
- **Variantes:** 1 réelle + 8 contrôles.
- **Null:** `NULL-LIMITED-RAGGED-COL7`, permutation de contrôle non historique.
- **Contrôle:** `C01-R-CT-RANDOM` à `C08-R-CT-RANDOM`, ciphertexts A–Z de longueur 97, seeds 1001–1008.
- **Statut attendu:** `EXPLORATION — NULL LIMITED`.

## Expérience G — permutation géométrique

- **experiment_id:** `E01-G-ROT180-97`
- **Hypothèse:** une rotation 180° abstraite, représentée par l’inversion complète des 97 positions, peut produire un effet de structure mesurable sans carte physique.
- **Antériorité:** miroir et retournements géométriques sont documentés ; aucun statut de nouveauté n’est revendiqué.
- **Transformation exacte:** `output[i] = input[96-i]` pour `i=0..96`.
- **Paramètres:** longueur 97 ; inversion complète ; aucune géométrie physique ; aucun padding.
- **Variantes:** 1 réelle + 8 contrôles.
- **Null:** `NULL-LIMITED-GEOM-ROT180`, randomisation de position de même longueur.
- **Contrôle:** `C01-G-CT-RANDOM` à `C08-G-CT-RANDOM`, seeds 1001–1008.
- **Statut attendu:** `EXPLORATION — NULL LIMITED`.

## Expérience M — masque positionnel

- **experiment_id:** `E01-M-PERIOD2-EVEN`
- **Hypothèse:** une sélection périodique fixe peut modifier la densité et les diagnostics de crib plus souvent qu’une sélection positionnelle aléatoire de même cardinalité.
- **Antériorité:** les masques périodiques et extra-E sont déjà recensés ; la présente passe teste une recette abstraite fixe et ne réhabilite aucune palette retirée.
- **Transformation exacte:** conserver exactement les positions zéro-indexées paires `{0,2,4,...,96}` ; supprimer les positions impaires ; conserver l’ordre original.
- **Paramètres:** période 2 ; phase 0 ; polarité keep-even ; longueur de sortie 49 ; aucune seconde couche.
- **Variantes:** 1 réelle + 8 contrôles de masque de cardinalité 49, seeds 1001–1008.
- **Null:** `NULL-LIMITED-MASK-CARD49`, positions uniformes sans remise de cardinalité 49.
- **Contrôle:** `C01-M-MASK-RANDOM` à `C08-M-MASK-RANDOM`, seeds 1001–1008.
- **Statut attendu:** `EXPLORATION — NULL LIMITED`.

## Expérience C — composition de deux systèmes

- **experiment_id:** `E01-C-COL7-ROT180`
- **Hypothèse:** la composition fixe d’une route ragged width 7 puis d’une inversion complète peut modifier les diagnostics autrement que chacune des transformations isolées.
- **Antériorité:** les compositions de routes et retournements sont antérieures ; l’équivalence sera signalée si elle est triviale, sans revendication de nouveauté.
- **Transformation exacte:** appliquer `E01-R-RAGGED-COL7`, puis inverser complètement la chaîne résultante.
- **Paramètres:** couche A route width 7 ; couche B reverse 97 ; ordre A→B ; aucun choix intermédiaire.
- **Variantes:** 1 réelle + 8 contrôles composés avec les mêmes deux classes et les mêmes seeds.
- **Null:** `NULL-LIMITED-COMPOSED-RANDOM`, première couche random permutation comparable puis inversion.
- **Contrôle:** `C01-C-COMPOSED-RANDOM` à `C08-C-COMPOSED-RANDOM`.
- **Statut attendu:** `EXPLORATION — HYPOTHESIS TEST` si l’effet structurel diffère des isolées ; sinon `EXPLORATION — NULL LIMITED`.

## Expérience P — mécanisme physique/procédural abstrait

- **experiment_id:** `E01-P-TWO-BAND-REVERSE7`
- **Hypothèse:** retourner la seconde bande contiguë d’une partition fixe de 49/48 positions peut créer des artefacts de parcours distincts d’une permutation aléatoire comparable.
- **Antériorité:** bandes, retours et overlays sont documentés comme hypothèses sous-déterminées ; aucune carte physique n’est revendiquée.
- **Transformation exacte:** partitionner la chaîne en bande A positions 0–48 et bande B positions 49–96 ; conserver A dans son ordre ; inverser B ; concaténer A+B inversée.
- **Paramètres:** frontière fixe 49/48 ; action reverse sur B ; orientation unique ; aucun repère physique ; aucune largeur choisie après résultat.
- **Variantes:** 1 réelle + 8 contrôles.
- **Null:** `NULL-LIMITED-TWO-BAND`, permutation de la seule bande B de longueur 48, avec A fixe.
- **Contrôle:** `C01-P-BAND-RANDOM` à `C08-P-BAND-RANDOM`.
- **Statut attendu:** `EXPLORATION — HYPOTHESIS TEST`, jamais preuve de mécanisme physique.

## Critères de conservation

Chaque sortie, y compris les huit contrôles par expérience, sera conservée. Un effet inhabituel sera rapporté comme :

> `ANOMALOUS SIGNAL — REQUIRES REPLICATION`

uniquement s’il est défini par comparaison aux contrôles prévus. Aucune optimisation locale ne suivra.

## Limites communes

- les nulls ne reproduisent pas une politique historique complète ;
- les cribs publics peuvent favoriser les diagnostics ;
- plusieurs transformations sont équivalentes à des routes ou permutations connues ;
- une anomalie structurelle n’est pas une solution ni une découverte ;
- cette passe ne teste aucune substitution ni clé ;
- aucune recette n’est convertie en `PROOF CANDIDATE` directement.
