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
| K01 structure | même passage transposé 7 fois ? même passage sous 7 substitutions ? mots allemands anagrammés ? | **les trois rejetées** (contrôles réussis) ; sections homogènes comme des morceaux d'un même texte (voir K02 pour la nature du texte) |
| K02 contacts | les lettres voisines sont-elles liées (toute langue) ? grille lue en colonnes ? espaces réels ? | **aucun contact** (z = 0,5 contre 37 pour une langue même substituée) ⇒ langue naturelle + substitution exclue ; pas de grille sans clé ; espaces liés aux lettres (« i » isolé, n/t finaux) |
| K03 grille à clé | les sections carrées (169 = 13², 144 = 12²) sont-elles de l'allemand transposé par une grille à clé ? | **non** (contrôles 95-100 % de réussite, même avec 5 % de lettres altérées ; réel p = 0,14 à 0,73 contre 200 nuls) |
| K04 répétitions de mots | les mots du chiffré sont-ils ceux du clair (toute substitution, avec ou sans anagramme) ? | **non** : 182 formes distinctes sur 194 mots, jamais atteint dans 13 langues en prose (russe, polonais, finnois… compris) ; espaces posés après coup sur des lettres mélangées |
| K06 routes | colonnes, zigzags, diagonales, spirales, toutes largeurs, par section et texte entier ? | **non** (contrôles 29/30 ; aucune suite crédible, meilleur score −13,7 contre −9 pour de l'allemand) |
| K05 grille tournante | blocs carrés = allemand sous grille de Fleissner ? | **non** (contrôles 19/20 et 17/20 ; réel au niveau des nuls, p ≥ 0,14) |
| K07 transposition sans langue | même question avec un score insensible à la substitution et à la langue (texte entier) | **non** pour les routes et les colonnes à clé 5-20 (MI réel 0,38-0,44 = nuls ; une vraie remise en ordre ≈ 0,9-1,1) ; blocs : sans puissance |
| K08 double transposition | Würfel (deux clés de colonnes 3-9), avec ou sans substitution, toute langue ? | **non** (contrôles 8/10 ; MI réel 0,408 = nuls ; clés ≥ 10 colonnes : hors de portée, déclaré) |
| K09 transpositions simples | barrière (2-100 rails) ou décimation, blocs et texte entier (+ substitution sur le texte entier) ? | **non** (contrôles 10/10 ; réel au niveau des nuls, textes illisibles) |

Synthèse d'étape : [`docs/02_synthese.md`](docs/02_synthese.md).
