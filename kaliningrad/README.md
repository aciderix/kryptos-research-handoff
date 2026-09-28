# Bouteille de Kaliningrad (Baltiysk, 2015) — recherche cryptanalytique

Même protocole que `dagapeyeff/docs/PROTOCOLE.md`. Choix de la cible : `docs/00_choix.md`.

- [`data/transcription_v1.txt`](data/transcription_v1.txt) — transcription de référence (26 lignes), faite ici d'après les photos de
  K. Schmeh, puis confrontée à la transcription indépendante de Corsair_nv (d3.ru, 2015 ; copie :
  `data/transcription_corsair_2015.txt`, à qui il manque une ligne). Détails : `docs/01_transcription_v1.md`. Contre-épreuve :
  les longueurs de sections entre lettres soulignées (166, 169, 162, 169, 169, 144, 5) sont identiques à celles de Norbert (2017).
  `transcription_v0.txt` = première version (glyphe r en forme de « 2 » noté x), conservée pour traçabilité.
- [`experiments/`](experiments/) — une cellule par dossier, pré-inscription commitée avant le résultat.

| Cellule | Question | Résultat |
|---|---|---|
| K01 structure | même passage transposé 7 fois ? même passage sous 7 substitutions ? mots allemands anagrammés ? | **les trois rejetées** (contrôles réussis) ; le texte se comporte comme un seul texte continu sous une seule règle de lettres |
| K02 contacts | les lettres voisines sont-elles liées (toute langue) ? grille lue en colonnes ? espaces réels ? | **aucun contact** (z = 0,5 contre 37 pour une langue même substituée) ⇒ langue naturelle + substitution exclue ; pas de grille sans clé ; espaces liés aux lettres (« i » isolé, n/t finaux) |
