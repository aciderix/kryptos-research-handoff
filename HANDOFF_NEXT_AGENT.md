# Handoff — Kryptos K4 Research

**Version:** 2026-09-21
**Repository source:** `https://github.com/jcolinpatrick/kryptos`
**Branch de référence:** `main`
**Dernier commit connu avant les corrections de cette session:** `4432b47`
**Langue de travail actuelle:** français possible ; les noms de fichiers, identifiants et citations restent en anglais.

> **Mise à jour du 25/09/2026.** Ce document décrit l'état du 21/09. Depuis, plus de 35 tests exacts et une relecture intégrale ont été faits. Pour reprendre, lire d'abord la synthèse complète : [`docs/knowledge_base/09_synthese_complete_2026_09_25.md`](docs/knowledge_base/09_synthese_complete_2026_09_25.md).

## 1. Mission générale

Kryptos est une sculpture de Jim Sanborn installée à la CIA. K1, K2 et K3 sont résolus. K4 est le bloc final de 97 caractères, dont deux fragments sont connus : `EASTNORTHEAST` et `BERLINCLOCK`.

L’objectif n’est pas de produire un plaintext plausible. L’objectif est d’identifier, puis éventuellement de tester, une procédure de déchiffrement qui soit déterminée avant le résultat, réellement distincte de l’antériorité connue, reproductible et falsifiable.

Le projet distingue strictement :

- **fait source** : ce qui est directement établi par un document ou une source primaire ;
- **interprétation** : lecture proposée à partir d’un fait ;
- **hypothèse** : mécanisme possible ;
- **recette** : procédure complète écrite avant tout résultat ;
- **expérience** : exécution avec résultat brut conservé ;
- **preuve** : résultat reproductible et indépendant, avec portée explicitement bornée.

## 2. État scientifique au moment du handoff

La phase documentaire du 21 septembre 2026 est terminée. Elle a confirmé que les cellules ouvertes sont **sous-déterminées avec les informations publiques actuellement accessibles**.

Aucune nouvelle candidate admissible n’a été identifiée. Aucun calcul K4, déchiffrement, score, extraction, recherche de plaintext ou nouvelle micro-variante n’a été exécuté pendant les phases documentaires finales.

Le blocage actuel est donc :

> **documentaire, non expérimental.**

Les informations manquantes sont une liaison authentifiée avec K4, une carte physique, une instruction de parcours, une opération déterminée et/ou la seconde couche cryptographique.

## 3. Cellules ouvertes actuelles

### F-02 — K3-style route reuse

**Établi :** K3 possède une double transposition de grille avec lectures par colonnes, notamment bas-vers-haut.
**Inconnu :** aucune preuve que cette route soit réutilisée pour K4 ; aucune géométrie K4, aucun remplissage, aucune seconde couche.
**Information minimale manquante :** une feuille ou instruction identifiant K4 et fixant grille, origine, route, sens, puis la seconde opération.
**Statut :** `OPEN CELL — NOT ADMISSIBLE`.

### F-07 — Physical overlay / two-sided tableau

**Établi :** l’écran comporte un tableau inversé lisible depuis l’arrière ; les sources évoquent une différence possible entre tableau de travail et tableau de l’écran.
**Inconnu :** repère commun, projection, appariement des glyphes, traitement des collisions, fonction appliquée aux paires.
**Information minimale manquante :** plan coté ou relevé original avec coordonnées et règle de correspondance ; puis opération sur les paires.
**Statut :** `OPEN CELL — NOT ADMISSIBLE`.

### F-08 — Procedural chart operations

**Établi :** les chartes K1–K3 utilisent lignes, marges, débordements, rubans et marqueurs de parcours.
**Inconnu :** aucun document ne relie un pli, une découpe, une marge ou un marqueur à K4.
**Information minimale manquante :** charte K4 authentifiée prescrivant un geste, son ancrage, son sens, son effet sur les 97 positions et la conversion cryptographique.
**Statut :** `OPEN CELL — NOT ADMISSIBLE`.

### F-10 — Morse extra-E masks

**Établi :** les plaques Morse et l’écran K4 sont distincts ; Scheidt parle d’un procédé qui masque l’anglais ; des E sont visibles dans les transcriptions.
**Inconnu :** ensemble exact des E, orientation, correspondance Morse→K4, polarité, fonction, consommation et seconde couche.
**Information minimale manquante :** document primaire définissant les marqueurs, la carte vers K4 et l’action unique.
**Statut :** `INCOMPLETE RECORD / OPEN CELL`; les anciens scripts indiquent `Last run: never` et aucun résultat correspondant n’est archivé.

### PHYSICAL-PROCEDURAL — mécanismes brisant H1

**Établi :** une opération physique peut réordonner ou sélectionner les positions avant la couche analysée et donc briser l’hypothèse directe `CT[i] → PT[i]`.
**Inconnu :** la carte de positions, la géométrie, le parcours, la fonction et la sortie.
**Information minimale manquante :** artefact fournissant une relation positionnelle complète entre un support physique et les 97 caractères K4, ainsi que l’opération appliquée.
**Statut :** `OPEN CELL — NOT ADMISSIBLE`.

## 4. Inconnues communes prioritaires

Les cellules ne doivent pas être traitées comme cinq problèmes indépendants. Elles partagent cinq inconnues structurantes :

1. **Lien documentaire avec K4.** Les scans doivent être rattachés à Kryptos/K4 par une cote, une date, un dossier, une légende primaire ou un contexte de fabrication.
2. **Repère physique commun.** Il faut une origine, une orientation, une échelle, des limites K4 et une numérotation des caractères ou cellules.
3. **Instruction opératoire.** Il faut savoir s’il faut lire, retourner, sélectionner, supprimer, permuter, substituer ou consulter le tableau.
4. **Seconde couche et sortie.** Une route ou un masque intermédiaire ne constitue pas un déchiffrement sans alphabet, clé, ordre et inverse de la seconde couche.
5. **Authenticité et chaîne de conservation.** Il faut distinguer l’original, la photographie, la transcription et l’interprétation du conservateur.

## 5. Sources utiles et niveau de confiance

### Sources institutionnelles ou primaires

- CIA, sculpture et disposition physique :
  `https://www.cia.gov/legacy/headquarters/kryptos-sculpture/`
- Archives of American Art, Jim Sanborn Papers :
  `https://www.aaa.si.edu/collections/jim-sanborn-papers-22298`
- Smithsonian finding aid du fonds Sanborn : source primaire potentielle ; l’accès et les cotes doivent être revérifiés avant toute conclusion.
- New York Times, chartes originales K1/K2 :
  `https://www.nytimes.com/2010/11/21/us/21codecharts.html`

### Interviews et presse

- Wired, entretien avec Ed Scheidt, notamment « masked the English language » :
  `https://www.wired.com/2005/01/inside-info-on-kryptos-codes/`
- Associated Press, archives et distinction plaintext/méthode :
  `https://apnews.com/article/kryptos-jim-sanborn-auction-cia-650c1253d6a96591f29b88a20299c430`
- Scientific American, indices 2025 :
  `https://www.scientificamerican.com/article/cia-kryptos-puzzle-creator-releases-final-clues/`

### Sources secondaires de contexte

- Rumkin, K3 :
  `https://rumkin.com/reference/kryptos/k3/`
- Rumkin, K0 Morse :
  `https://www.rumkin.com/reference/kryptos/k0/`
- Kryptos Beyond K4, K3 Solution #3 :
  `https://kryptosfan.wordpress.com/k3/k3-solution-3/`
- KryptosBot archive page et scans publiés :
  `https://www.kryptosbot.com/archive/`
- SolveKryptos verification page :
  `https://solvekryptos.com/verify`

Les sources secondaires ne doivent pas être utilisées pour transformer une analogie en fait primaire. Les pages d’archive qui transcrivent `shade an area`, `7×88`, `Extra L`, `Bottom chart seeding` et `Code Breaker` sont des pistes documentaires. Elles ne constituent pas actuellement des instructions K4 authentifiées.

## 6. Documents internes essentiels à lire en premier

Ordre recommandé :

1. `docs/current_experimental_frontier.md`
2. `docs/family_registry.md`
3. `docs/research_protocol.md`
4. `docs/research_protocol.md` — notamment détermination, équivalence et conservation des négatifs
5. `docs/two_systems_landscape.md`
6. `docs/documentary_research_2026_09_21.md`
7. `audits/registry_coherence_2026_09_21/report.md`
8. `audits/prior_art_reliability_2026_09_21/audit_report.md`
9. `audits/open_cells_2026_09_21/synthesis.md`
10. `audits/open_cells_2026_09_21/unknowns_map.md`
11. `audits/documentary_authentication_2026_09_21/authentication_report.md`
12. `docs/k3_chart_layout_and_route_2026_09_19.md`
13. `docs/procedural_anomaly_recipes.md`
14. `docs/nyt_k1k2_chart_physical_layout_2026_09_19.md`
15. `docs/EVIDENCE_POLICY.md`
16. `docs/claims_ladder.md`
17. `MEMORY.md`
18. `exhaustion_log.json`

## 7. Registre et corrections critiques

Le journal racine `exhaustion_log.json` est la source de vérité au niveau des scripts. Au dernier contrôle il comptait 1 044 entrées : 732 `active`, 310 `exhausted` et 2 `superseded`.

Corrections importantes :

- `e_aaa_extra_l_05` est **MISCLASSIFIED PRIOR ART** : son chemin annoncé comme Beaufort exécutait Vigenère.
- `e_aaa_extra_l_07_corrected` implémente des formules distinctes, mais son résultat brut est absent : **INCOMPLETE RECORD**.
- `e_aaa_beaufort_trans_01` : **INCOMPLETE RECORD**, aucun score ou run artifact archivé.
- `e_aaa_tableau_struct_06` : **INCOMPLETE RECORD**, désaccord entre le nombre de configurations annoncé par l’en-tête et le journal.
- F-10 a été corrigé de `DEAD UNDER RECORDED SCOPES` vers **INCOMPLETE RECORD / OPEN CELL** : les scripts correspondants déclarent `Last run: never`.

Un statut `INCOMPLETE RECORD` n’est jamais une preuve d’élimination. `MISCLASSIFIED PRIOR ART` ne compte pas comme test valide de l’opération annoncée.

## 8. Méthode de recherche obligatoire

Avant toute éventuelle expérience, appliquer cette séquence :

1. **Recherche documentaire ciblée.** Chercher d’abord l’information qui fixe les paramètres ; ne pas chercher une sortie anglaise.
2. **Authentification.** Vérifier l’original, la provenance, la date, l’auteur, le contexte et le lien réel avec K4.
3. **Canonicalisation.** Décrire la transformation sans dépendre de la notation ou du résultat.
4. **Antériorité.** Chercher si la transformation a déjà été testée sous un autre nom.
5. **Équivalence.** Déterminer l’équivalence avant de regarder tout résultat K4.
6. **Recette.** Écrire représentation, alignement, parcours, paramètres, opération, ordre des couches, sortie et critères d’échec.
7. **Degrés de liberté.** Lister explicitement tous les choix restants. Un choix post-résultat rend la piste `OPEN CELL — NOT ADMISSIBLE`.
8. **Pré-enregistrement.** Seulement si la recette est complète et nouvelle.
9. **Résultat brut.** Conserver stdout, paramètres, hash des entrées, version du code et contrôles.
10. **Falsification.** Ne pas optimiser après observation ; reproduire indépendamment et tenter de tuer le résultat.

## 9. Règles de preuve

- Ne jamais écrire « personne n’a testé » ; écrire seulement ce que le registre documente.
- Ne jamais transformer `DEAD UNDER SCOPE` en impossibilité universelle.
- Ne jamais transformer `INCOMPLETE RECORD` en élimination.
- Ne jamais utiliser une solution plaintext proposée pour fixer rétroactivement une recette.
- Ne jamais utiliser « méthode standard », « clé probable », « essayer les variantes » ou « choisir la meilleure sortie » comme étape de recette.
- Une simple association sémantique n’est pas une contrainte cryptographique.
- Les scores linguistiques sont des signaux à falsifier, jamais une preuve.
- Toute hypothèse qui casse H1 doit dire précisément comment elle réindexe ou sélectionne le ciphertext avant la couche analysée.
- La conservation d’un négatif est un résultat utile seulement si sa portée, ses paramètres et ses contrôles sont archivés.

## 10. Outillage

### Inspection et recherche

- `rg`, `find`, `git`, `gh`
- `manus-config config load` pour examiner les connecteurs avant une opération externe
- `search`/`fetch` pour découvrir et lire des sources publiques
- navigateur Manus uniquement si une page dynamique ou une session connectée est nécessaire

### Validation locale

- `python3 -m json.tool <file.json>` pour valider un JSON
- `pytest` pour les tests explicitement requis par une modification de code
- `git diff`, `git status`, `git log` pour la provenance

Ne jamais exécuter du code exploratoire directement depuis un heredoc ou une commande inline si un script doit être conservé ; écrire d’abord le fichier, puis l’exécuter.

## 11. Prochaine phase proposée par l’utilisateur — à ne pas lancer automatiquement

Le message utilisateur joint propose éventuellement une phase **EXPLORATION**, distincte d’un **PROOF CANDIDATE** :

- `EXPLORATION` peut contenir des paramètres exploratoires, mais aucun score ne devient une preuve ;
- `PROOF CANDIDATE` exige une recette complète, une antériorité auditée, des paramètres indépendants, une reproduction et une validation indépendante.

Avant tout batch exploratoire, le plan doit pré-enregistrer pour chaque famille :

```text
FAMILY:
QUESTION:
ESPACE TESTÉ:
PARAMÈTRES VARIABLES:
PARAMÈTRES FIXES:
MÉTRIQUE:
CONTRÔLE NÉGATIF:
CRITÈRE D’ARRÊT:
RISQUE DE FAUX POSITIF:
```

Contrôles minimaux possibles : ciphertext aléatoire de même longueur, permutations aléatoires, clés aléatoires, contrôle hors-crib, transformation nulle, texte de contrôle. Il ne faut pas lancer la phase Exploration-01 avant qu’un plan soit lui-même revu et accepté.

## 12. Prompt de démarrage recommandé

Le prochain agent peut être démarré avec le prompt suivant :

> Clone `https://github.com/jcolinpatrick/kryptos` et travaille dans le dépôt cloné.
>
> Commence par lire, dans cet ordre :
>
> 1. `docs/current_experimental_frontier.md` ;
> 2. `docs/family_registry.md` ;
> 3. `docs/research_protocol.md` ;
> 4. `audits/registry_coherence_2026_09_21/report.md` ;
> 5. `audits/prior_art_reliability_2026_09_21/audit_report.md` ;
> 6. `audits/open_cells_2026_09_21/synthesis.md` ;
> 7. `audits/open_cells_2026_09_21/unknowns_map.md` ;
> 8. `audits/documentary_authentication_2026_09_21/authentication_report.md` ;
> 9. `MEMORY.md` ;
> 10. `exhaustion_log.json`.
>
> Contexte : les cellules F-02, F-07, F-08, F-10 et les mécanismes physiques/procéduraux qui brisent H1 sont ouvertes mais sous-déterminées. Le blocage est documentaire, pas expérimental. Ne crée pas de nouvelle micro-variante et ne lance aucun calcul K4 avant d’avoir trouvé une information indépendante qui fixe une recette complète.
>
> Pour toute information documentaire, sépare : fait source, interprétation, hypothèse, recette, antériorité, équivalence et degrés de liberté restants. Si un choix pourrait être fait après observation du résultat, écris immédiatement `OPEN CELL — NOT ADMISSIBLE`.
>
> Si une nouvelle phase exploratoire est envisagée, commence uniquement par écrire un plan `Exploration-01` avec `FAMILY`, `QUESTION`, `ESPACE TESTÉ`, `PARAMÈTRES VARIABLES`, `PARAMÈTRES FIXES`, `MÉTRIQUE`, `CONTRÔLE NÉGATIF`, `CRITÈRE D’ARRÊT` et `RISQUE DE FAUX POSITIF`. Ne lance pas le batch avant validation du plan.
>
> Objectif : améliorer la proportion d’expériences déterminées, nouvelles et falsifiables, pas augmenter artificiellement le nombre d’expériences.

## 13. Limites de ce handoff

Ce document ne publie aucune clé, aucun secret, aucune variable d’environnement et aucune authentification. Les fichiers de base de données, logs lourds, images et résultats bruts doivent être examinés séparément avant une publication publique. Les chemins absolus locaux présents dans d’anciens rapports doivent être neutralisés ou laissés explicitement comme références historiques non exécutables dans une copie publique.
