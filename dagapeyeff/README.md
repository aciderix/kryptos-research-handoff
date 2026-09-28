# Chiffre de d'Agapeyeff (1939) — recherche cryptanalytique

- [`docs/00_choix_du_defi.md`](docs/00_choix_du_defi.md) — pourquoi d'Agapeyeff plutôt que Zodiac Z32
- [`docs/01_etat_de_lart.md`](docs/01_etat_de_lart.md) — sources, faits structurels, solutions revendiquées auditées, registre des familles déjà testées
- [`docs/02_mecanismes_du_livre.md`](docs/02_mecanismes_du_livre.md) — matrice des mécanismes du livre
- [`docs/03_relecture_du_livre.md`](docs/03_relecture_du_livre.md) — relecture complète du livre et de E01-E15 : trous identifiés
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
| E07 | familles colonnaires (E, I largeurs 2-11 dont 2×98 ; D ≤ 7×7) en **espéranto** | **négatif** sur 5 cellules ; revendication msgtrail « 2×98 espéranto » non reproduite ([résultats](experiments/E07_esperanto/RESULTS.md)) |
| E08 | calibration de l'excès « largeur 7 » (2 000 mélanges ; correction pour comparaisons multiples) | **compatible avec le hasard** (p local 0,006-0,0095 ; p global 0,11) — observation close ([résultats](experiments/E08_w7_calibration/RESULTS.md)) |
| E09 | transposition **nihiliste** 14×14 (lignes et colonnes par la même clé), lecture par lignes / par colonnes, 8 langues | **négatif** sur 20 cellules admissibles ([résultats](experiments/E09_nihilist/RESULTS.md)) |
| E10 | empreinte d'un carré « mot-clé + alphabet » (source primaire : méthode de l'auteur) | **suspendu** : prémisse rejetée par E11 ([note](experiments/E10_keyword_square/RESULTS.md)) |
| **E11** | **profil de fréquences** (invariant par transposition et par carré) | **OBSERVATION calibrée** : rejette, pour 8 langues, tout mécanisme « une paire = une lettre » (toute transposition, tout carré ; p ≤ 0,003) ; hors colonne 14 : compatible avec un uniforme sur 13 symboles. Conclusion sur la nature du clair **non déterminée** (nulles non calibrées → E12) ([résultats](experiments/E11_frequency_profile/RESULTS.md)) |
| E12 | calibration des mécanismes **du livre** (nulles 1/k, Playfair, Vigenère, homophones, fractionnation, Wolseley…) sur vrais textes, 8 langues | **aucun** ne reproduit le profil ; colonne 14 unique ; pas de fractionnation (p = 0,0002) — nature du clair toujours **non déterminée** ([résultats](experiments/E12_mechanism_calibration/RESULTS.md)) |
| E13 | hypothèse **13 classes** (un symbole = deux lettres, appariement fréquente + rare) | **compatible** (profil) en anglais (p = 0,20) et latin ; Wolseley standard et partitions aléatoires rejetés ; exige une transposition ; **non documentée** ([résultats](experiments/E13_thirteen_classes/RESULTS.md)) |
| E14 | « 13 classes » + transpositions d'E04, test rapide (statistique invariante) | colonnaire simple largeurs 2-11 (deux sens) **exclue** ; double ≤ 7×7 **non concluant** ([résultats](experiments/E14_thirteen_classes_scan/RESULTS.md)) |
| E15 | « 13 classes » + **double** transposition ≤ 7×7, attaque complète en deux étages (Viterbi 2 lettres/symbole) | **négatif** — contrôles 8/10, réel −11,06/−11,10 = niveau du null ([résultats](experiments/E15_thirteen_classes_double/RESULTS.md)) |
