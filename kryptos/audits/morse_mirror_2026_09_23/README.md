# « Miroir » du Morse appliqué à K4 (relais Gemini, 23/09)
Miroir = lire chaque lettre Morse à l'envers : A↔N, B↔V, D↔U, F↔L, G↔W, Q↔Y ; lettres palindromes inchangées ; **C, J, Z sans image** (9 lettres de K4, dont le Z en position 70, dans le crib).
Variante (a) : C/J/Z laissés tels quels ; (b) : position 70 retirée. Miroir appliqué au chiffré, au clair, ou aux deux.
**Seules les combinaisons non absorbées par un alphabet libre** : QIII (tous côtés), QI (côté chiffré), QII (côté clair) ; QIV et le côté libre de QI/QII reviennent exactement à changer d'alphabet, déjà testé.
Clé périodique p = 1–26, Vig/Beau, exact ; témoin 20 chiffrés aléatoires par case (`morse_mirror.py`, `results.json`).

**Résultats (728 cas)**
- **Quagmire III (système de K1–K2) : aucune période ≤ 12 compatible**, quel que soit le côté du miroir.
- 388 cases sévères (hasard ≤ 1/20) : K4 compatible dans 7, **toutes à p = 19** (même phénomène « distance 38 entre cribs », déjà mesuré à ≈ 10 %, `../crib_shift_2026_09_23/`), pour 3,4 attendues.
- **Déchiffrement à p = 19** (`p19_mirror_decrypt.py`, 300 solutions par case) : charabia (`…BSQPOYMKOCESLUH…`, `…RGTBWJYGTMSSSJI…`). Quand la position 70 est retirée, BERLINCLOCK devient `BERLINCZOCK` ⇒ rien ne tient.
- 2 cases (QIII Vig p = 11, miroir côté chiffré et côté clair) : K4 **incompatible** ; témoin non calculé (instances SAT trop lentes), inutile pour une élimination.
- ⇒ **éliminé** : le miroir Morse, appliqué avant ou après un Quagmire, ne donne rien.
