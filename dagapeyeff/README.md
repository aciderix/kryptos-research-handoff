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
| E02 | Polybe (carré inconnu) + colonnaire à clé largeur 14, 8 bourrages (clé 6!×8!) | pré-inscrite |
