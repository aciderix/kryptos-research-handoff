# T11 / T11b rejugés avec les alphabets à mot-clé (25/09/2026)

**Motif.** Avec un alphabet libre, T11 et T11b (disque tourné une fois par bloc de n lettres, pas fixe dans le bloc) tombent au niveau du hasard dès qu'on admet une erreur (`../vision_2026_09_24/` T20). Avec un alphabet à mot-clé fixé, la clé est connue aux 24 positions : chaque bloc exige que K_t − motif(j) y soit constante, et le nombre minimal d'erreurs se calcule exactement.

**Portée** (`kwblocks.c`) : 237 988 alphabets à mot-clé (59 497 mots × 4 formes) ; types Q3, Q2, Q1, Q4a, Q4b ; Vigenère, Beaufort, variante ; blocs n = 5 à 14, toutes phases ; motif s·j (s = 0..25), plus KRYPTOS lu dans A–Z (à l'endroit, à l'envers) pour n = 7. Soit environ 7,5 milliards de cas par chiffré.

**Contrôle positif** (`synth_blk.txt` : PALIMPSEST, n = 7, φ = 3, s = 5, une erreur dans les cribs et une hors cribs) : réglage retrouvé avec e_min = 1.

**Résultat.**

| | meilleur e_min |
|---|---|
| K4 | 7 (un seul réglage, compté 4 fois par symétrie) |
| K4 mélangé, graine 501 | 8 |
| K4 mélangé, graine 502 | 8 |

⇒ Avec les alphabets de Sanborn, le disque tourné par bloc demanderait au moins 7 lettres fausses sur 24 : **éliminé**, bien au-delà du taux d'erreur plausible. K4 est au niveau des témoins.
