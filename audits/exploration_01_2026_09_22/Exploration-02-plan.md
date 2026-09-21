# Exploration-02 — Plan borné et non exécuté

**Date :** 2026-09-22.

**Statut :** plan proposé, aucune exécution.

**Maximum prévu :** 3 expériences, 11 variantes réelles au total, contrôles associés pré-enregistrés par variante.

**Règle :** aucune variante ne sera ajoutée, supprimée ou reparamétrée après observation. Les expériences ne seront lancées qu’après validation de ce plan.

## Justification générale

Exploration-01 a fermé sous scope cinq recettes précises, mais n’a pas fermé les familles R, M, C ou P. Les résultats justifient uniquement des tests de limites déjà documentées : périodes de masque prévues dans le plan, dépendance de P à la frontière de bande, et ordre des couches de C. Ils ne justifient pas une optimisation autour des scores observés.

Les trois expériences ci-dessous restent exploratoires. Un null incomplet ou mécanisme-spécifique non historique sera étiqueté `EXPLORATION — NULL LIMITED`. Aucun résultat ne pourra devenir directement `PROOF CANDIDATE`.

## Exploration-02-A — périodes de masque pré-déclarées

**FAMILY:** M — masques positionnels périodiques.

**QUESTION EXPÉRIMENTALE:** Les périodes déjà énumérées dans le plan — 3, 5, 7, 11 et 13 — produisent-elles des distributions d’IC ou de longueur sélectionnée qui diffèrent systématiquement d’un masque aléatoire de même cardinalité ?

**JUSTIFICATION:** Le plan original fixe explicitement l’ensemble `{2,3,5,7,11,13}`. Exploration-01 n’a testé que la période 2. Le présent plan teste les cinq autres valeurs sans choisir une période selon un résultat.

**ANTÉRIORITÉ:** Les familles de masques périodiques sont déjà couvertes comme antériorité. Aucune nouveauté n’est revendiquée.

**CE QUI EST DISTINGUÉ D’EXPLORATION-01:** périodes 3, 5, 7, 11 et 13 au lieu de la seule période 2 ; phase 0 et polarité keep restent fixes.

**TRANSFORMATION EXACTE:** pour chaque `p` dans `{3,5,7,11,13}`, conserver les positions `i` telles que `i mod p = 0`, dans l’ordre croissant ; supprimer toutes les autres positions ; aucune seconde couche.

**PARAMÈTRES:** CT97 public ; période p ; phase 0 ; polarité keep ; indexation zéro ; sortie variable ; aucun crib reprojeté.

**NOMBRE DE VARIANTES:** 5 réelles. Pour chaque variante, 8 masques aléatoires de même cardinalité, tirés avec seeds 2001–2008.

**NULL / CONTRÔLE:** `NULL-LIMITED-MASK-CARDINALITY`; positions uniformes sans remise avec cardinalité exactement égale à la sortie de la période correspondante ; même métrique et même pipeline.

**MÉTRIQUES:** longueur de sortie ; IC ; occurrences complètes des cribs ; distribution des caractères ; cardinalité et longueur comparées au null apparié.

**RÉSULTAT RÉELLEMENT INFORMATIF:** une période pré-déclarée montre un effet distributionnel reproductible au-delà de tous ses contrôles appariés, avec direction fixée avant l’exécution et sans sélection de la meilleure période.

**RÉSULTAT QUI SERAIT DU BRUIT:** une période isolée a un IC proche d’un contrôle, un crib partiel, ou une différence expliquée par la cardinalité.

**STATUT ATTENDU:** `EXPLORATION — NULL LIMITED`.

## Exploration-02-B — sensibilité de frontière des bandes

**FAMILY:** P — opération abstraite de bande, sans revendication physique.

**QUESTION EXPÉRIMENTALE:** L’effet observé en E01-P dépend-il uniquement du choix de frontière 49/48, ou reste-t-il présent pour les largeurs explicitement mentionnées dans le plan ?

**JUSTIFICATION:** Le plan P énumère les largeurs `{7,14,21,31}` sous condition de justification interne. Cette expérience ne choisit pas une largeur selon un résultat ; elle teste la liste fermée documentée comme diagnostic de sensibilité.

**ANTÉRIORITÉ:** bandes, pliages et retours sont des hypothèses antérieures et sous-déterminées. Aucune carte physique n’est revendiquée.

**CE QUI EST DISTINGUÉ D’EXPLORATION-01:** E01-P a testé une frontière 49/48 ; E02-B teste les quatre largeurs documentées et conserve le préfixe au lieu d’inventer une projection physique.

**TRANSFORMATION EXACTE:** pour chaque `w` dans `{7,14,21,31}`, conserver `CT[0:w]` dans son ordre et inverser `CT[w:97]`; concaténer les deux segments.

**PARAMÈTRES:** frontières w ; orientation fixe ; action reverse sur le suffixe ; aucun repère physique ; aucune autre largeur.

**NOMBRE DE VARIANTES:** 4 réelles. Pour chaque variante, 8 permutations aléatoires du suffixe de longueur `97-w`, avec seeds 2101–2108.

**NULL / CONTRÔLE:** `NULL-LIMITED-BAND-SUFFIX`; préfixe identique, suffixe randomisé de même longueur et même alphabet.

**MÉTRIQUES:** points fixes ; paires adjacentes ; occurrences complètes des cribs ; match positionnel public ; IC ; différence réelle-contrôles.

**RÉSULTAT RÉELLEMENT INFORMATIF:** une propriété qui reste supérieure au contrôle pour toutes les largeurs pré-déclarées et qui n’est pas uniquement due au préfixe conservé.

**RÉSULTAT QUI SERAIT DU BRUIT:** points fixes élevés proportionnels à w, sans excès crib ou hors-crib ; une seule largeur favorable ; résultat expliqué par l’identité du préfixe.

**STATUT ATTENDU:** `EXPLORATION — HYPOTHESIS TEST`, jamais mécanisme physique démontré.

## Exploration-02-C — ordre des couches et équivalence

**FAMILY:** C — composition de deux transformations structurelles.

**QUESTION EXPÉRIMENTALE:** Les deux ordres pré-déclarés `R→G` et `G→R` sont-ils réellement distincts comme permutations, et l’un produit-il un régime contrôlé différent de l’autre ?

**JUSTIFICATION:** Le plan C autorise les deux ordres uniquement lorsqu’ils ne sont pas algébriquement équivalents. E01-C a testé `R→G` avec R=ragged width-7 et G=reverse. Le test inverse est donc une vérification d’ordre et d’équivalence, pas une recherche locale de score.

**ANTÉRIORITÉ:** les routes et retournements sont déjà documentés. La comparaison canonique est prioritaire sur toute revendication de nouveauté.

**CE QUI EST DISTINGUÉ D’EXPLORATION-01:** E01-C a testé `ragged_col7 → reverse`; E02-C ajoute seulement `reverse → ragged_col7`, avec le même couple et aucun paramètre libre.

**TRANSFORMATIONS EXACTES:**

- `C1 = reverse97(ragged_col7(CT))` — reproduction canonique de E01-C ;
- `C2 = ragged_col7(reverse97(CT))` — ordre inverse.

Avant toute métrique, sérialiser les deux permutations de positions et enregistrer si elles sont identiques. Si elles sont identiques, C2 est classée équivalente et aucun statut de nouveauté n’est permis.

**PARAMÈTRES:** R=ragged width-7 ; G=reverse97 ; ordre fixé ; CT97 ; aucun masque, clé ou troisième couche.

**NOMBRE DE VARIANTES:** 2 réelles. Pour chaque ordre, 8 contrôles : première couche permutation aléatoire de même classe, puis seconde couche fixe ; seeds 2201–2208.

**NULL / CONTRÔLE:** `NULL-LIMITED-COMPOSED-ORDER`; même nombre de couches, même longueur, ordre contrôlé, permutation première couche randomisée.

**MÉTRIQUES:** identité de permutation canonique ; occurrences complètes des cribs ; match positionnel ; non-crib identity ; IC ; paires adjacentes ; différence entre les deux ordres.

**RÉSULTAT RÉELLEMENT INFORMATIF:** les deux ordres sont canoniquement distincts et l’un présente une différence distributionnelle pré-déclarée par rapport aux contrôles, sans simple avantage de crib.

**RÉSULTAT QUI SERAIT DU BRUIT:** les deux permutations sont identiques ; ou les différences restent dans les contrôles ; ou un ordre gagne seulement sur un score de crib isolé.

**STATUT ATTENDU:** `EXPLORATION — HYPOTHESIS TEST` ; `OPEN CELL — NOT ADMISSIBLE` pour toute revendication si l’équivalence n’est pas résolue.

## Budgets et arrêt communs

Le budget est fixé à 11 variantes réelles et 88 contrôles, soit 99 exécutions au maximum, sans retries silencieux. Une erreur est une ligne conservée avec son message ; elle ne libère pas un nouvel essai. Un résultat manquant est conservé comme `MISSING`, compté dans le dénominateur de campagne et exclu seulement des métriques qui lui sont inapplicables.

Arrêt avant exécution si :

- une liste ou un seed diffère de ce plan ;
- une équivalence de transformation ne peut pas être sérialisée ;
- un contrôle ne conserve pas la propriété annoncée ;
- une métrique n’est pas définie pour une longueur variable ;
- un choix de paramètre reste dépendant d’une observation.

## Promotion de statut

Aucun résultat de ces trois expériences ne peut devenir `PROOF CANDIDATE`. Un résultat inhabituel serait au maximum :

> `ANOMALOUS SIGNAL — REQUIRES REPLICATION`

après comparaison à tous les contrôles et audit d’équivalence. Ce plan n’est pas exécuté dans le présent tour.
