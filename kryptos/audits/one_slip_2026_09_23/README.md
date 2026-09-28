# Une erreur de Sanborn dans les cribs ? Et le mot-clé recommencé à chaque ligne (23/09)

**Pourquoi.** La feuille de travail K1/K2 (NYT 2010, vérifiée au pixel) montre une erreur de clé gravée (PALIMPCEST → IQLUSION), une erreur de gravure (E sur la feuille, R sur le cuivre → UNDERGRUUND) et une lettre omise (IDBYROWS). Toutes les éliminations de K4 exigent 24/24.

## 1. Une erreur tolérée (`one_slip.py`, `results_one_slip_K4.jsonl`)
- **Quagmire III (système de K1/K2), tout alphabet, périodes 1–13, Vigenère et Beaufort : toujours impossible**, même en retirant n'importe laquelle des 24 lettres.
- **Clé suivie tirée des clairs de K1, K2, K3, tout décalage, tout alphabet : toujours impossible** avec une erreur tolérée.
- Quagmire I et II : seule la période 13 passe, mais au hasard 15 à 25 % des chiffrés aléatoires y sont compatibles **sans** erreur (`results_null_p13.json`). Non informatif.
- ⇒ **Les éliminations classiques résistent à une erreur de Sanborn.** L'hypothèse « K4 exécuté sans faute » ne cachait pas la solution simple.

## 2. Mot-clé recommencé à chaque ligne de 31 (`row_restart.py`)
Habitude plausible sur une feuille de 31 cases. Alignement « feuille » (K4 commence en colonne 0) ou « cuivre » (colonne 27). Quagmire I, II, III, p 1–13.
- Aucune compatibilité exacte. Avec une erreur tolérée : 3 cases sur 26 pour Quagmire III (dont p = 8 Beaufort, comme la longueur d'ABSCISSA). Au hasard : **6 chiffrés aléatoires sur 6** ont au moins une case, **2 sur 26 en moyenne** (`results_row_restart_null_QIII.jsonl`, arrêté après 6 chiffrés complets). **Bruit.**
- Contrôles décisifs de la case p = 8 :
  - avec les alphabets de Sanborn (KA ou AZ), **13 à 15 incohérences sur 24** (`row_restart_ka.py`) ;
  - les lettres imposées hors cribs ne ressemblent pas à de l'anglais (« …EASTNORTHEAST**QR**… », `results_row_restart_forced.json`).
- ⇒ **Mirage** : l'alphabet libre absorbe les contraintes.
