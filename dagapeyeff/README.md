# Chiffre de d'Agapeyeff (1939) — recherche cryptanalytique

- [`docs/00_choix_du_defi.md`](docs/00_choix_du_defi.md) — pourquoi d'Agapeyeff plutôt que Zodiac Z32
- [`docs/01_etat_de_lart.md`](docs/01_etat_de_lart.md) — sources, faits structurels, solutions revendiquées auditées, registre des familles déjà testées
- [`docs/PROTOCOLE.md`](docs/PROTOCOLE.md) — règles obligatoires
- [`data/`](data/) — chiffré + provenance
- [`tools/`](tools/) — outils C (mesures, solveurs)
- [`experiments/`](experiments/) — une cellule par dossier ; `PREREGISTRATION.md` commité **avant** le résultat

| Cellule | Famille | Statut |
|---|---|---|
| E01 | Polybe (carré inconnu) + route sur grille 14×14 / 14×13 (256 cellules) | **négatif** — contrôles 20/20, réel sous le max du null ([résultats](experiments/E01_routes14/RESULTS.md)) |
| E02 | Polybe (carré inconnu) + colonnaire à clé largeur 14, 8 bourrages (clé 6!×8!) | **négatif** — contrôles 8/10, réel au niveau de la médiane du null, 3 graines → 3 clés ([résultats](experiments/E02_columnar14/RESULTS.md)) |
| E03 | colonnaire à clé **libre** + carré inconnu, largeurs 7/13/14 (colonne 14 = nulles) et 14 (196) | **négatif** sur les 4 cellules — contrôles 8-10/10, réel ≤ max du null ; OBSERVATION : clés récurrentes en B13 (structure diagonale ?), sans anglais ([résultats](experiments/E03_freecolumnar/RESULTS.md)) |
| E04 | balayage **exhaustif** invariant par substitution : colonnaire standard/inverse largeurs 2-11 (grilles incomplètes), double colonnaire ≤ 7×7, + résolution du carré | **négatif** sur 6 cellules — contrôles 8-10/10, réel au niveau du null ([résultats](experiments/E04_invariant_scan/RESULTS.md)) ; la récurrence B13 d'E03 est un optimum de bigrammes, pas de l'anglais |
| E05 | familles E04 (colonnaire 2-11, double ≤ 7×7) en **français, allemand, italien, espagnol, latin, néerlandais** | **négatif** sur 11 cellules admissibles ([résultats](experiments/E05_languages/RESULTS.md)) |
| E06 | lecture verticale des colonnes, ordres à clé lignes paires/impaires (7!² × 3 modes) | **négatif** (anglais A/B, français, espagnol ; 4 langues non admissibles) ; l'excès de répétitions en lecture verticale s'explique par la composition des colonnes ([résultats](experiments/E06_parity_read/RESULTS.md)) |
