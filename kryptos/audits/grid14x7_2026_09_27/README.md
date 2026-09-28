# Hypothèse « feuille 14×31 = K3 (14×24) + ?+K4 (14×7) » — testée, NÉGATIVE (2026-09-27)

Proposée par le user. Arithmétique exacte : panneau bas = 14×31 = 434 = K3 (336 = 14×24) + « ? » + K4 (97) ;
31−24 = 7 = |KRYPTOS| ; ?+K4 = 98 = 14×7.

**Test (`grid14x7.c`) — n'utilise AUCUN clair externe, seulement les 24 cribs confirmés + juge qg_big :**
- S = « ? » + K4 (98 cases) en grille 14×7 et 7×14 ; 8 transformées diédrales (retourner, mettre à l'envers,
  pivoter) × lecture lignes/colonnes × serpentin ; + les 5040 permutations de colonnes (largeur 7) ;
  chaque ordre dans les deux sens (π, π⁻¹) → **10 208 ordres**.
- Puis Vigenère/Quagmire **périodique** P=1..14, alphabets AZ/KRYPTOS/PALIMPSEST/ABSCISSA, VIG/BEA/VAR,
  clé indexée par position claire (modèle a) OU gravée (modèle b), dérivée des cribs.

| | min erreurs de crib /24 | qoff |
|---|---|---|
| **Contrôle positif** (message planté 14×7 + Vig KRYPTOS) | **0** | **−2,05 (anglais, retrouvé)** |
| **K4 réel** | 6 | −3,99 (charabia) |
| 5 K4 mélangés (null) | 6 (tous) | −3,5..−3,95 |

⇒ K4 réel = indiscernable du hasard. La feuille 14×7 + gestes de Sanborn + clé périodique à alphabet public
ne reproduit pas les cribs. (Recoupe les négatifs antérieurs : largeur-7 5040 ordres, chef ; ordres 3D, MÉCA.)
