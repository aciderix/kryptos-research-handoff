# Clé « ligne + colonne » en Beaufort (complète le registre ; relais Gemini, 23/09)
clé(i) = a[i // w] + b[i % w] (a, b inconnus), Quagmire III avec **n'importe quel alphabet**, exact sur les 24 lettres des cribs, largeurs 2–48.
L'ancien calcul (SAT/CaDiCaL, `one_width.py`) ne tranchait pas les petites largeurs en 15 min ; refait avec **CP-SAT** (OR-Tools : « toutes différentes » + équations modulo 26), `rowcol_cpsat.py`.
**Validation** : CP-SAT retrouve exactement les résultats Vigenère de l'ancien moteur (w = 5 compatible ; 6, 9, 15, 21, 38 éliminés).

**Résultats (Beaufort)**
- **Éliminé** (K4 incompatible) : largeurs [5, 6, 9, 10, 19, 20, 21, 38, 40, 42].
- Compatible aux autres largeurs, mais les témoins (10 chiffrés aléatoires par largeur) le sont aussi dans 60 à 100 % des cas ⇒ **indécidable, aucun signal** (aucune largeur sévère où K4 passe).
- Pour mémoire, Vigenère (même famille, ancien moteur) : éliminé aux largeurs [6, 9, 15, 19, 21, 38, 42, 45] ; compatible ailleurs.
Fichiers : `results_cpsat.jsonl` (K4), `null_cpsat.jsonl` (témoins), `summary.json`.
