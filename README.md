# Kryptos K4 Research Handoff

Ce dépôt contient le handoff documentaire pour la recherche sur **Kryptos K4**, le bloc final de 97 caractères de la sculpture de Jim Sanborn.

Le dépôt source complet est [`jcolinpatrick/kryptos`](https://github.com/jcolinpatrick/kryptos). Ce dépôt séparé rassemble uniquement les documents nécessaires pour reprendre le travail : protocole, état de la frontière, audits, registre des cellules ouvertes, provenance documentaire et prompt de démarrage.

## Statut au 21 septembre 2026

> **Le blocage est documentaire, pas expérimental.**

Les cellules F-02, F-07, F-08, F-10 et les mécanismes physiques/procéduraux qui pourraient briser l’alignement direct sont sous-déterminés avec les informations publiques actuellement accessibles. Aucune nouvelle transformation K4 ni aucun calcul n’a été lancé pendant la phase documentaire finale.

Le document principal est [`HANDOFF_NEXT_AGENT.md`](HANDOFF_NEXT_AGENT.md). Il contient le contexte, les règles de preuve, les sources utiles, les corrections du registre, les outils et le prompt complet à donner au prochain agent.

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
