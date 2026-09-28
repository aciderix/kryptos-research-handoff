# Exploration-01 — Résultats structurels

**Date :** 2026-09-22.

**Statut autorisé :** `EXPLORATION RESULT`.

**Interprétation :** aucun résultat n’est présenté comme solution, preuve ou découverte.

**Calculs exécutés :** 5 transformations réelles + 40 contrôles, soit 45 enregistrements bruts.

**Entrée :** ciphertext public de longueur 97 ; SHA-256 `2797de1451ab97118211735dcef370fc5acd70829f164aa043c47cbf40f0a5a3`.

**Protocole :** `Exploration-01-execution-protocol.md`, SHA-256 `d9f259a70eec3c3b0af112eef9a8b51697c3d751d7f30590e539cc3d84bcaf32`.

**Implémentation exécutée :** `scripts/exploration_01_structural.py`, SHA-256 après correction métrique `724f22847b3349183aa2d9bea9e7295e1ac5edf8dfa99da83d8fd0d34a43a33e`.

## Résumé

Aucune des cinq recettes ne produit une occurrence exacte de `EASTNORTHEAST` ou `BERLINCLOCK`. Les scores de crib positionnels restent dans les plages des contrôles pour les quatre sorties de longueur 97. Le masque M produit une sortie de longueur 49, donc le score positionnel public n’est pas défini ; son indice de coïncidence reste dans la plage des masques contrôles.

Aucun `ANOMALOUS SIGNAL — REQUIRES REPLICATION` n’est déclaré. Le statut global est donc :

> **EXPLORATION RESULT**

## Résultats des cinq transformations réelles

| ID | Famille | Transformation | Longueur | Occurrences cribs | Match positionnel | Match hors-crib identité | IC | Points fixes | Paires adjacentes |
|---|---|---|---:|---|---:|---:|---:|---:|---:|
| `E01-R-RAGGED-COL7` | R | ragged width-7 column read | 97 | 0 / 0 | 3 | 4 | 0.036082 | 4 | 0 |
| `E01-G-ROT180-97` | G | inversion des 97 positions | 97 | 0 / 0 | 1 | 1 | 0.036082 | 1 | 0 |
| `E01-M-PERIOD2-EVEN` | M | conservation des positions paires | 49 | 0 / 0 | non applicable | non applicable | 0.032313 | 3 | non applicable |
| `E01-C-COL7-ROT180` | C | ragged width-7 puis inversion | 97 | 0 / 0 | 2 | 1 | 0.036082 | 1 | 0 |
| `E01-P-TWO-BAND-REVERSE7` | P | bande 0–48 fixe, bande 49–96 inversée | 97 | 0 / 0 | 1 | 40 | 0.036082 | 53 | 48 |

Les deux métriques « points fixes » et « paires adjacentes » de P sont structurellement élevées parce que la première bande est conservée telle quelle. Ce n’est pas un signal cryptographique ; c’est une conséquence directe de la recette.

## Comparaison aux contrôles

| ID | Contrôle positionnel réel | Contrôles positionnels | IC réel | IC contrôles | Conclusion contrôlée |
|---|---:|---:|---:|---:|---|
| R | 3 | 0–3 | 0.036082 | 0.036082–0.046177 | compatible avec les contrôles |
| G | 1 | 0–2 | 0.036082 | 0.036082–0.046177 | compatible avec les contrôles |
| M | n/a | n/a | 0.032313 | 0.028061–0.036565 | compatible avec les contrôles |
| C | 2 | 0–3 | 0.036082 | exactement 0.036082 | compatible avec les contrôles |
| P | 1 | 1–2 | 0.036082 | exactement 0.036082 | compatible avec les contrôles |

Les contrôles ont utilisé les seeds pré-enregistrées 1001 à 1008. Toutes les sorties contrôles et toutes les sorties réelles sont conservées dans le JSON brut.

## Analyse par famille

### R — routes et transpositions

**Fait expérimental :** la recette ragged width-7 donne 3 caractères correspondant aux deux positions de crib publiques, sans occurrence contiguë complète et sans conservation de paires adjacentes.

**Comparaison :** le maximum contrôle est également 3. Le résultat réel n’est donc pas anormal dans cette petite distribution.

**Interprétation :** aucun excès contrôlé observé.

**Limite :** ce test mesure une route fixe déjà apparentée à des routes columnar ; il ne constitue pas une nouvelle route historique.

**Statut :** `EXPLORATION — NULL LIMITED`, négatif pour un signal distinctif.

### G — permutation géométrique

**Fait expérimental :** l’inversion complète produit 1 match positionnel et aucun crib complet.

**Comparaison :** les contrôles vont de 0 à 2. L’observation réelle est dans la plage contrôle.

**Interprétation :** aucune anomalie de la permutation abstraite.

**Limite :** l’opération n’est reliée à aucune carte physique authentifiée.

**Statut :** `EXPLORATION — NULL LIMITED`, négatif.

### M — masque positionnel

**Fait expérimental :** le masque paires conserve 49 caractères et aucun crib complet. Le score positionnel public est volontairement non applicable, car la projection des positions de crib dans la sortie filtrée n’a pas été introduite après observation.

**Comparaison :** l’IC réel 0.032313 est compris entre les IC contrôles 0.028061 et 0.036565.

**Interprétation :** aucun signal structurel distinctif dans les métriques pré-déclarées.

**Limite :** la métrique positionnelle n’est pas comparable entre sortie 97 et sortie 49 ; cette absence est une limite enregistrée, pas une raison de redéfinir la métrique après résultat.

**Statut :** `EXPLORATION — NULL LIMITED`, négatif/inconclusif pour les métriques non applicables.

### C — composition

**Fait expérimental :** la composition donne 2 matches positionnels, sans crib complet.

**Comparaison :** les contrôles vont de 0 à 3. L’IC est identique à celui de chaque contrôle dans cette construction, car la composition conserve la multiensemble des caractères.

**Interprétation :** aucun excès contrôlé.

**Limite :** cette composition est une combinaison fixe de transformations structurelles déjà connues ; elle ne crée pas une revendication de nouveauté.

**Statut :** `EXPLORATION — HYPOTHESIS TEST`, négatif.

### P — mécanisme physique/procédural abstrait

**Fait expérimental :** la recette conserve 53 positions identiques et 48 paires adjacentes, car la première bande de 49 positions est inchangée. Elle donne 1 match positionnel et aucun crib complet.

**Comparaison :** les contrôles de permutation de la seconde bande donnent 1 à 2 matches positionnels et le même IC. Les métriques de conservation sont supérieures dans la recette réelle, mais cette différence est tautologique : la première bande est explicitement laissée intacte.

**Interprétation :** aucun signal cryptographique ou physique. La différence structurelle confirme seulement l’effet prévu de la transformation.

**Limite :** la recette ne représente pas une carte de sculpture ou un mécanisme historique authentifié.

**Statut :** `EXPLORATION — HYPOTHESIS TEST`, négatif pour une anomalie.

## Contrôles de complétude

- configurations réelles prévues : 5 ; exécutées : 5 ;
- contrôles prévus : 40 ; exécutés : 40 ;
- enregistrements bruts : 45 ;
- statuts d’exécution : 45 `EXECUTED` ;
- métriques manquantes dans le JSON brut : 0 ;
- résultats positifs et négatifs conservés : oui ;
- optimisation après observation : aucune ;
- ajout de paramètre : aucun ;
- recherche de plaintext : aucune ;
- recherche de clé : aucune ;
- transformation de substitution : aucune.

Le premier lancement technique a rencontré une erreur de métrique sur la longueur variable du masque avant écriture du résultat. La métrique a été corrigée pour retourner `null` lorsqu’elle n’est pas définie pour une sortie de longueur 49 ; les recettes, paramètres, seeds, contrôles et règles d’interprétation n’ont pas changé. L’exécution complète réussie est celle archivée dans le JSON brut.

## Conclusion épistémique

Cette passe fournit une observation concrète, limitée et falsifiable : les cinq transformations fixes testées ne produisent pas de crib complet et ne dépassent pas leurs contrôles dans les métriques comparables. Elle ne permet pas d’exclure d’autres recettes, de valider un null historique, d’authentifier un mécanisme physique ou de résoudre K4.

Aucun résultat ne mérite le statut `ANOMALOUS SIGNAL — REQUIRES REPLICATION`.

> **EXPLORATION RESULT**
