# Quagmire à alphabet **à mot-clé** (famille de Sanborn) — test exact

## Pourquoi (liens avec la base documentaire)
- **AAA 6/8** : les feuilles manuscrites de Sanborn (russe, 1990–1991) chiffrent ainsi :
  clair et clé dans l'alphabet normal, chiffré dans un **alphabet à mot-clé** (ТЕНЬ + le reste dans l'ordre).
  C'est un Quagmire II ; vérifié sur 22 lettres sur 23 (base 5).
- **AP 1991** : l'enveloppe de Webster contient « the **keywords** » (au pluriel).
- **Bean 2021** : l'alphabet chiffré de K4 est « proche de A–Z » (p ≈ 1/5 520), ce qui est la signature d'un alphabet à mot-clé.
- **Base 2** : Quagmire II/I/IV avec un alphabet **quelconque** reste « non tranchable » aux longues périodes.
  Un alphabet à mot-clé est une contrainte bien plus forte, **jamais testée exactement** (seulement avec des listes de mots).

## Paramètres fixés AVANT le calcul
- Alphabet à mot-clé de préfixe L : positions 0..L−1 = lettres quelconques distinctes ; positions L..25 = les autres lettres **dans l'ordre alphabétique**.
  Une rotation de l'alphabet se ramène à un décalage constant de la clé, déjà libre : tester l'alphabet non tourné suffit.
- L ≤ 12 lettres distinctes (KRYPTOS = 7, PALIMPSEST = 8, ABSCISSA = 5). La propriété est monotone : si c'est impossible à L = 12, ce l'est pour tout L plus petit.
- Familles : QII (chiffré à mot-clé), QI (clair à mot-clé), QIV (deux alphabets à mot-clé indépendants),
  QIII (même alphabet à mot-clé des deux côtés) pour mémoire.
- Chiffrement : Vigenère et Beaufort. Clé périodique, périodes 1–52. Alignement direct.
- Juge : cohérence exacte avec les 24 lettres des cribs (SAT). **Aucun score d'anglais.**
- Témoins : positif (texte synthétique chiffré par ce procédé : doit être SAT) ; nul (20 chiffrés aléatoires : taux de SAT dû au hasard).
- Interprétation fixée d'avance : UNSAT pour K4 à une période où le témoin nul est SAT dans ≥ 50 % des cas = élimination informative ;
  SAT pour K4 à une période où le nul est presque toujours UNSAT = **signal à examiner**.

## Résultats (23/09)
- 350 cases (QI, QII, QIV × Vigenère/Beaufort × p 1–52 ; QIII partiel : déjà éliminé sans contrainte). **Témoins positifs : 350/350 retrouvés.**
- Périodes restant compatibles : `open_periods.md`. Toutes les autres sont **éliminées** pour tout alphabet à mot-clé (L ≤ 12).
  Gain par rapport à l'alphabet quelconque (QII Vigenère) : les périodes 20, 23, 37, 39, 40 et 46 tombent.
  La plupart des périodes restantes sont celles où le hasard passe aussi (≥ 50 % des témoins) : **24 lettres ne tranchent pas**.
- **Seule case « sévère » compatible : période 19** (et 38 = 2×19), en QII et QIV. Hasard (200 témoins) : 1 % (QII Vig), 0 % (QII Beau), 3 % (QIV).
  Sur 154 cases sévères, on en attend une ou deux au hasard.
  **Vérification décisive** (`p19_decrypt.py`) : les solutions déchiffrent K4 en charabia, et le début d'alphabet imposé n'est pas un mot (`GF?KRMS?NTY`, `RFMZKYTNV`).
  ⇒ **coïncidence, rejetée.**
- **Conclusion** : le procédé que Sanborn employait seul (Quagmire II à mot-clé, clé périodique) **n'est pas celui de K4** pour les périodes ≤ 15 (déjà connu) et pour 20, 23, 37, 39, 40, 46.
  Aux autres périodes longues, il reste indécidable avec les seuls cribs. Aucun signal.
