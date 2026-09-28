# Exploration-01 — Post-mortem scientifique

**Date :** 2026-09-22

**Statut de la campagne :** `EXPLORATION RESULT`.

**Anomalie déclarée :** aucune.

**Résultats analysés :** 5 transformations réelles et 40 contrôles, soit 45 enregistrements bruts.

**Nouvelle exécution lancée pendant ce post-mortem :** 0.

## 1. Question réellement testée

Exploration-01 n’a pas testé « quelle est la solution de K4 ? ». Elle a testé cinq recettes structurelles fixes et leurs contrôles limités :

- une route ragged de largeur 7 ;
- une inversion complète des 97 positions ;
- un masque périodique des positions paires ;
- une composition route width-7 puis inversion ;
- une opération abstraite conservant la première bande et retournant la seconde.

Les métriques étaient des diagnostics : occurrences exactes de cribs, correspondance aux positions publiques lorsque la longueur restait 97, identité hors crib, indice de coïncidence, points fixes et paires adjacentes. Elles n’étaient pas une fonction de recherche d’anglais ni une preuve.

## 2. Résultat global

Aucune sortie réelle ne contenait `EASTNORTHEAST` ou `BERLINCLOCK` comme occurrence contiguë complète. Les scores positionnels des sorties de longueur 97 se trouvaient dans les plages des contrôles. Pour le masque de longueur 49, le score positionnel public n’était volontairement pas défini ; son indice de coïncidence était dans la plage des masques contrôles.

Aucune anomalie contrôlée n’a été observée.

Cette conclusion est limitée aux cinq recettes et aux contrôles prévus. Elle ne permet pas de conclure que les mécanismes structurels, physiques ou documentaires sont impossibles.

## 3. Ce que chaque famille a réellement appris

### R — route ragged width-7

**Observation :** la sortie réelle a obtenu 3 correspondances positionnelles, avec un maximum contrôle également égal à 3. Aucun crib complet n’apparaît et aucune paire adjacente n’est conservée.

**Information obtenue :** cette recette précise ne produit pas un excès de score public par rapport à des ciphertexts aléatoires soumis à la même route. Le contrôle est informatif parce qu’il montre que le score 3 n’est pas distinctif dans cet espace.

**Hypothèse éliminée sous scope :**

> La recette exacte « remplissage ragged width 7, lecture colonne-major, sans padding » produit un signal crib supérieur à son contrôle randomisé dans la campagne exécutée.

**Hypothèses non éliminées :** autres routes, autres géométries, relations K3-K4, traitements documentés d’un tableau incomplet et mécanismes physiques associés.

**Statut :** recette exacte `DEAD UNDER SCOPE` pour la métrique testée ; famille R ouverte.

### G — inversion complète des 97 positions

**Observation :** 1 correspondance positionnelle, zéro crib complet, et une plage contrôle de 0 à 2.

**Information obtenue :** l’inversion abstraite n’est pas distinguable des contrôles sur ces métriques.

**Hypothèse éliminée sous scope :**

> L’inversion complète des 97 positions, considérée comme permutation géométrique abstraite, produit un signal public distinctif.

**Hypothèses non éliminées :** une carte physique non équivalente, une géométrie avec positions manquantes, un overlay, une orientation documentée ou une transformation dont l’échangeabilité diffère.

**Équivalence :** cette recette est une permutation globale connue sous des descriptions de miroir ou de rotation 180° ; elle ne constitue pas une nouvelle famille cryptanalytique.

**Statut :** recette exacte `DEAD UNDER SCOPE` ; famille G ouverte.

### M — masque périodique paires

**Observation :** sortie de longueur 49, aucun crib complet, IC réel 0.032313 dans la plage contrôle 0.028061–0.036565. Le score positionnel public n’était pas comparable, car aucune projection post-hoc des cribs n’a été introduite.

**Information obtenue :** la sélection paire fixe n’a pas produit de différence dans l’IC par rapport à des sélections aléatoires de même cardinalité.

**Hypothèse éliminée sous scope :**

> La classe précise « keep positions paires, phase 0, période 2 » produit naturellement un régime d’IC distinct de son null de cardinalité.

**Hypothèses non éliminées :** autres périodes déjà prévues dans le plan, autres classes de masques, masques provenant d’un artefact primaire, projection déterminée des positions de crib et mécanismes à deux couches.

**Équivalence :** il s’agit d’un masque positionnel périodique, non d’une découverte indépendante ; les familles de masques périodiques sont déjà antérieures dans le registre.

**Statut :** recette exacte `DEAD UNDER SCOPE` pour l’IC et les cribs complets ; famille M ouverte.

### C — composition width-7 puis inversion

**Observation :** 2 correspondances positionnelles, zéro crib complet, contrôles 0–3. L’IC réel est identique à celui des contrôles, ce qui est attendu pour une permutation qui conserve le multiensemble de caractères.

**Information obtenue :** cette composition ne produit pas d’excès contrôlé par rapport à une première couche randomisée suivie de la même inversion.

**Hypothèse éliminée sous scope :**

> La composition exacte route ragged width-7 → inversion complète produit un signal distinctif supérieur à la composition randomisée comparable.

**Hypothèses non éliminées :** autres compositions déjà documentées, compositions avec une carte primaire, ordre de couches non équivalent et mécanismes à deux systèmes non réduits à ces permutations.

**Équivalence :** la composition est une permutation globale résultante ; sa description en deux couches ne suffit pas à la distinguer d’une route unique équivalente. Elle ne doit pas être comptée comme nouvelle sans record canonique.

**Statut :** recette exacte `DEAD UNDER SCOPE` ; famille C ouverte.

### P — bande 0–48 fixe, bande 49–96 inversée

**Observation :** 53 points fixes et 48 paires adjacentes. Cette conservation est imposée par la recette, puisque la première bande de 49 caractères n’est pas modifiée. Le score positionnel est 1, contre 1–2 pour les contrôles ; aucun crib complet n’apparaît.

**Information obtenue :** le contrôle de permutation de la seconde bande montre que le faible score crib n’est pas un signal. Les métriques de conservation sont élevées, mais elles vérifient surtout la définition de l’opération.

**Hypothèse éliminée sous scope :**

> Cette opération abstraite de deux bandes produit un signal textuel public distinctif au-delà de l’effet mécanique de la bande conservée.

**Hypothèses non éliminées :** autres frontières documentées, overlays, cartes physiques, procédures manuelles et opérations dont la conservation n’est pas tautologique.

**Équivalence :** la transformation est une permutation par blocs, apparentée à des routes et retournements déjà couverts. Elle ne constitue pas une carte physique authentifiée.

**Statut :** recette exacte `DEAD UNDER SCOPE` pour les métriques textuelles testées ; famille P ouverte.

## 4. Familles et recettes « dead under scope »

### Recettes suffisamment couvertes sous leur périmètre exact

Les cinq recettes précises peuvent être marquées `DEAD UNDER SCOPE` pour les questions et métriques annoncées :

| Recette | Conclusion limitée |
|---|---|
| R ragged width-7 column read | pas d’excès crib contrôlé |
| G inversion 97 | pas de signal géométrique abstrait distinctif |
| M keep-even period 2 | pas de régime IC distinct du null apparié |
| C width-7 puis reverse | pas d’excès contrôlé de composition |
| P bande 49/48 avec reverse B | conservation tautologique, pas de signal crib |

### Familles qui restent ouvertes

Aucune famille complète R, G, M, C ou P ne doit être classée `DEAD UNDER SCOPE`. Une seule recette par famille ne couvre pas :

- les routes alternatives ;
- les géométries non équivalentes ;
- les masques déterminés par un artefact ;
- les compositions dont les couches ne se réduisent pas à une permutation équivalente ;
- les mécanismes physiques ou procéduraux réels.

## 5. Contrôles les plus informatifs

Le contrôle le plus informatif dépend de la famille :

- **R :** ciphertexts aléatoires de même longueur soumis à la même route ; le maximum contrôle égale le score réel.
- **G :** permutation complète de ciphertexts aléatoires ; elle encadre directement le score positionnel de l’inversion.
- **M :** masques aléatoires de cardinalité 49 ; ils rendent l’IC comparable sans confondre cardinalité et signal.
- **C :** première couche randomisée puis inversion fixe ; elle montre que l’IC identique est une propriété de permutation, non un résultat cryptographique.
- **P :** permutation de la seule seconde bande ; elle révèle que le score crib faible est compatible avec la structure conservée et que les points fixes élevés sont tautologiques.

Le contrôle hors crib n’a pas produit de mesure textuelle indépendante suffisante pour une conclusion positive, mais son absence de signal distinctif est cohérente avec les autres contrôles.

## 6. Hypothèses éliminées et compatibles

### Éliminées sous scope

1. La route ragged width-7 testée génère un excès de crib détectable.
2. L’inversion complète des 97 positions constitue un signal géométrique abstrait.
3. Le masque keep-even period 2 produit un régime d’IC distinct à cardinalité fixée.
4. La composition route width-7 → inversion crée un excès supérieur à la composition randomisée.
5. La transformation P fixe produit un signal textuel distinct de l’effet de sa bande conservée.

### Toujours compatibles

1. Une autre route déterminée, en particulier une route directement liée à une source K3, peut différer.
2. Une géométrie ou un overlay non équivalent peut produire un effet absent des cinq recettes.
3. Un masque fixé par un artefact physique ou documentaire peut être différent d’un masque périodique générique.
4. Une composition dont les couches changent réellement les classes de contraintes peut différer d’une permutation équivalente.
5. Une procédure physique réelle peut ne pas être représentée par une opération abstraite unique.
6. Un mécanisme cryptographique utilisant une substitution, une clé ou une relation K1–K3 reste entièrement hors de cette campagne.
7. Un null limité peut manquer un régime de faux positifs ; les résultats ne valident donc pas l’absence de tout signal.

## 7. Équivalence cryptanalytique

Les cinq recettes doivent être conservées dans l’historique, mais elles ne doivent pas être comptées comme cinq familles nouvelles :

- R est une route columnar ragged déjà apparentée aux routes de transposition documentées ;
- G est la permutation inverse globale, équivalente à un miroir ou à une rotation 180° en représentation 1D ;
- M appartient à la classe déjà connue des masques périodiques ;
- C est une permutation composée pouvant être canonisée en une seule permutation résultante ;
- P est une permutation par blocs, apparentée à un retournement de bande.

Une revendication de nouveauté nécessiterait une différence de record canonique, de contraintes ou d’antériorité, pas seulement un nom différent.

## 8. Nouvelle expérience justifiée ou non

Les résultats ne justifient pas une optimisation locale autour des scores 3, 2 ou 1. Ils justifient seulement des tests qui répondent à des limites identifiées avant observation :

- améliorer la comparaison des sorties à longueur variable ;
- vérifier une famille de masques déjà prévue, sans sélectionner la meilleure période ;
- tester une relation structurelle documentée qui distingue réellement une famille d’une permutation équivalente ;
- séparer l’effet mécanique d’une bande conservée de toute métrique textuelle.

Les propositions concrètes, au maximum trois, figurent dans `Exploration-02-plan.md` et ne sont pas exécutées ici.

## Conclusion

Exploration-01 a été informative malgré son résultat négatif : elle a montré que les cinq recettes fixes étudiées ne donnent pas de signal distinctif dans leurs contrôles et que plusieurs métriques structurelles peuvent être tautologiques par construction.

Elle n’a pas fermé les cinq familles. Elle a fermé sous scope cinq recettes précises et a fourni des contrôles de référence pour une éventuelle suite. Toute nouvelle expérience doit être justifiée par une contrainte indépendante ou par une limite méthodologique explicitement identifiée, jamais par la recherche du score le plus élevé.

> **EXPLORATION RESULT — NO ANOMALOUS SIGNAL**
