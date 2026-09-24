# Fichiers du groupe kryptos.groups.io (copie intégrale, accès membres)

> **Dépôt privé uniquement.** Ce dossier a été versé le 24/09/2026 à la demande du propriétaire du dépôt, pour un audit parallèle, **après** le passage du dépôt en privé.
> - Ce sont des pièces **réservées aux membres** du groupe. Elles appartiennent à leurs auteurs et contiennent des noms, et, dans les archives `.mbox` brutes, des adresses de courriel.
> - **Ne pas republier.** Si le dépôt redevient public un jour, il faudra d'abord retirer ce dossier de **tout l'historique git**. Un simple commit de suppression ne suffit pas.
> - Les identifiants du compte qui a servi au téléchargement ne sont **pas** versés.

## Contenu

| Dossier | Contenu | Taille |
|---|---|---|
| `fichiers/` | Les fichiers **originaux**, dans l'arborescence du groupe : racine (FAQ, pièces FOIA, bulletins NSA, œuvres de Sanborn…) et `Personal Folders/`, 209 dossiers personnels, 1 609 fichiers | ≈ 630 Mo, 1 654 fichiers |
| `textes/` | Texte extrait de chaque fichier lisible (PDF, Word, tableurs lus en texte brut, `.txt`), même arborescence | ≈ 39 Mo |
| `textes_pdf_dechiffres/` | Texte des PDF protégés de M. Friedrich (mot de passe trivial), extraits après ouverture | < 1 Mo |
| `messages/mbox.json` | Archive des messages 2003–2018 : 20 250 messages, champs `d` (date), `s` (sujet), `a` (auteur, adresse tronquée par groups.io), `t` (texte) | 25 Mo |
| `inventaire/` | `tree.json` (arborescence complète du groupe), `pf_overview.txt` (aperçu des dossiers personnels), `skipped.json` (fichiers non extraits) | — |
| `balayage/` | Sorties et scripts des deux balayages (bases 7 §8 et §9) : phrases d'observation (`obs_candidates.json`, `obs_part2.txt`), affirmations chiffrées (`odds_candidates.txt`), affirmations de solution (`claims.json`), scripts de vérification (`scripts/`) | — |

## Limites connues
- Les scans sans couche texte n'ont pas d'extraction : pas d'OCR dans l'environnement. Ceux qui étaient pertinents ont été regardés en image, page par page.
- Un PDF est corrompu (« Those pesky LY digraphs… », dossier de M. Friedrich).
- Les scripts de `balayage/` pointent vers l'espace temporaire de la session d'origine. Pour les relancer, remplacer le chemin par `sources/groupsio_membres/textes`.
- Les lectures, vérifications et verdicts sont dans `docs/knowledge_base/07_groupsio_2026_09_24.md` (§2–§9).
