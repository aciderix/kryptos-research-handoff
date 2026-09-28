# Z32 (Zodiac, 26 juin 1970) — recherche cryptanalytique

Même protocole que `dagapeyeff/docs/PROTOCOLE.md` (audit, registre d'antériorité, pré-inscription, contrôles, pas de
sélection a posteriori, null, vérification exacte, séparation connu / testé / nouveau, aucune « solution » sans oracle
indépendant). Réserve de départ, écrite avant ce travail (`dagapeyeff/docs/00_choix_du_defi.md`) : 32 symboles sont sous
la distance d'unicité d'un homophonique ; on peut **réfuter** des familles, pas **prouver** une lecture.

- [`docs/01_etat_de_lart.md`](docs/01_etat_de_lart.md) — sources primaires (dont les fichiers FBI), audit des propositions et des références, registre
- [`data/`](data/) — transcriptions (Oranchak), clairs Z408/Z340 alignés ; [`data/PROVENANCE.md`](data/PROVENANCE.md)
- [`sources/fbi/`](sources/fbi/) — extraits des fichiers FBI (domaine public fédéral) : fiche du labo de juillet 1970, note manuscrite du cryptanalyste (Q51)
- [`tools/`](tools/) — outils C
- [`experiments/`](experiments/) — une cellule par dossier, pré-inscription commitée avant le résultat

| Cellule | Objet | Statut |
|---|---|---|
| E01 | budgets d'homophones du Zodiac (clés Z408/Z340) comme test de falsification, calibré sur ses propres chiffrés | **négatif** : ≈ 2 % de puissance ; aucun candidat publié qui respecte les verrous n'est rejeté ; Allen et Foxon violent les verrous en lecture directe ([résultats](experiments/E01_budget_homophones/RESULTS.md)) |

**Découverte documentaire** : le cryptanalyste du FBI (juillet 1970) a compté **26** symboles différents, pas 29, et a
déjà essayé les mots probables NORTH, SOUTH, EAST, WEST, MILES, YARDS, FEET, BOMB et KILL ([§ 2](docs/01_etat_de_lart.md)).
