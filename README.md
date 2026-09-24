# Kryptos K4 Research Handoff

Ce dépôt contient le handoff documentaire pour la recherche sur **Kryptos K4**, le bloc final de 97 caractères de la sculpture de Jim Sanborn.

Le dépôt source complet est [`jcolinpatrick/kryptos`](https://github.com/jcolinpatrick/kryptos). Ce dépôt séparé rassemble uniquement les documents nécessaires pour reprendre le travail : protocole, état de la frontière, audits, registre des cellules ouvertes, provenance documentaire et prompt de démarrage.

## Statut au 21 septembre 2026

> **Le blocage est documentaire, pas expérimental.**

Les cellules F-02, F-07, F-08, F-10 et les mécanismes physiques/procéduraux qui pourraient briser l’alignement direct sont sous-déterminés avec les informations publiques actuellement accessibles. Aucune nouvelle transformation K4 ni aucun calcul n’a été lancé pendant la phase documentaire finale.

Le document principal est [`HANDOFF_NEXT_AGENT.md`](HANDOFF_NEXT_AGENT.md). Il contient le contexte, les règles de preuve, les sources utiles, les corrections du registre, les outils et le prompt complet à donner au prochain agent.

## Mise à jour du 24 septembre 2026

- **Synthèse (à lire en premier)** : `docs/knowledge_base/08_synthese_2026_09_24.md`, qui recoupe l'ensemble des bases.

- Documents versés par l'utilisateur (NSA 1991–1992 et 2014, Scheidt et Sanborn 1991, NOVA 2006, réunion Scheidt 2015…) : `sources/docs_utilisateur_2026_09_24/`, lus et résumés dans `docs/knowledge_base/06_documents_2026_09_24.md`.
- Audit « vision » (liens entre les pistes, tests exacts nouveaux en C, statistiques revues) : `audits/vision_2026_09_24/README.md`. Aucune solution ; plusieurs familles nouvelles éliminées ; la note IMG_1340 est résolue (« ? » non codés).
- Fichiers du groupe kryptos.groups.io (accès membre) : inventaire complet, lectures et tri des dossiers personnels dans `docs/knowledge_base/07_groupsio_2026_09_24.md` (rien n'est copié dans le dépôt ; références publiques données). Test T7 (textes « enfouis » : directive Truman de la pierre angulaire, cahier des charges CIA 1988, lettre de Sanborn 1989) : négatif. Archive des messages 2003–2018 lue (témoignages directs de rencontres avec Sanborn et Scheidt ; aucune donnée personnelle reprise) : 5 des 6 lettres doublées de K4 sont en position ≡ 4 (mod 7), p ≈ 0,02 après correction ; T8 (« chaîne de masquage » = 1/563) : négatif.
- **Le « 7 » de K4 n'est très probablement pas un hasard** (balayage systématique des dossiers personnels, base 7 §8). Deux anomalies vues séparément, l'écart 7 (NSA 1992) et les doublets mod 7 (Gillogly 2005), sont indépendantes. Ensemble, avec le module laissé libre : **p ≈ 2 × 10⁻⁴** sur un million de mélanges (`audits/vision_2026_09_24/stats_7b.py`). La largeur 21 n'ajoute pas de preuve indépendante : 3 de ses 11 paires viennent des deux autres signaux (correctif du premier chiffre, 10⁻⁵). Ce n'est ni une substitution périodique (paire 65/72 : P → R puis C), ni une autoclé simple, et aucune famille simulée ne reproduit la concentration des doublets (`sim_7.py`). T10 (maquette GIRASOL de 1988 comme source de clé) : négatif. Trois familles à structure de 7 sans période, testées exactement : disque tourné par bloc de 7 (T11), autoclé mixte (T12), Hill par blocs à petits coefficients (T13), doublets vus comme glissements de copie (T14), Wheatstone à cadran à mot-clé (T15, 704 880 cadrans) : négatives. Nouvelle observation : en Beaufort A–Z, la clé tirée des cribs se répète localement (p ≈ 0,004 corrigé, `stats_clef.py`).

## Principes de travail

- Distinguer fait source, interprétation, hypothèse, recette, expérience et preuve.
- Auditer l’antériorité avant de présenter une piste comme nouvelle.
- Déterminer l’équivalence par la transformation, avant de regarder le résultat.
- Conserver les négatifs et ne jamais confondre `INCOMPLETE RECORD` avec une élimination.
- Ne pas utiliser une sortie anglaise ou un score pour choisir rétroactivement les paramètres.
- Une recette incomplète reste `OPEN CELL — NOT ADMISSIBLE`.

## Contenu

- `HANDOFF_NEXT_AGENT.md` — document de reprise principal et prompt de démarrage.
- `docs/` — protocole, frontière expérimentale, registres, contexte K1–K4 et documentation de recherche.
- `audits/` — audits de fiabilité, authenticité, cohérence et cartographie des cellules ouvertes.
- `MEMORY.md` — mémoire opérationnelle utile à la reprise.
- `docs/knowledge_base/` — **bases documentaires : tout le savoir public sur Sanborn, et tout ce qui a déjà été testé (à lire avant tout calcul)**.
- `docs/k4_mechanism_reasoning_2026_09_22.md` — raisonnement « mécanisme plutôt que clé » et éliminations algébriques (SAT, alphabets inconnus) ; code dans `audits/algebraic_elimination_2026_09_22/`.
- `exhaustion_log.json` — copie documentaire du journal d’antériorité, avec les chemins locaux neutralisés.

Les bases SQLite, scans bruts, logs lourds, clés et configurations locales ne sont pas inclus dans ce dépôt public.

## Reprendre le travail

```bash
git clone https://github.com/aciderix/kryptos-research-handoff.git
cd kryptos-research-handoff
sed -n '1,260p' HANDOFF_NEXT_AGENT.md
```

Pour retrouver le code et les scripts complets :

```bash
git clone https://github.com/jcolinpatrick/kryptos.git
```

## Avertissement méthodologique

Ce dépôt ne présente aucune solution de K4. Il documente ce qui est établi, ce qui est seulement revendiqué, ce qui reste ouvert et l’information minimale qui permettrait éventuellement de rendre une expérience déterminée.
