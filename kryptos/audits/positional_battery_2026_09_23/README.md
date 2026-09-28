# Batterie positionnelle déterministe : position → lettre du tableau au dos → fonction SIMPLE → clé (relais « deux couches », 23/09)

Différence avec `../tableau_lookup_2026_09_23/` : **aucune table libre**. Seules des fonctions simples, écrites d'avance, avec des alphabets **fixés**.
Un vrai mécanisme de cette famille donne 24/24 (23 avec une coquille) ; le hasard donne ~1/24 par case.

## Paramètres fixés AVANT le calcul
**Géométries** (lettre du tableau H sous la lettre i de K4 ; K4 = lignes 25–28 du cuivre, 4 + 31 + 31 + 31) :
- même ligne r, même colonne j (écran replié, FOLD) ; miroir horizontal (colonne len−1−j) ; miroir vertical (ligne 29−r) ; rotation 180° (les deux) ;
- décalage de ligne d = −3…+3 ; pied de tableau avec ou sans blanc initial ;
- **alignements linéaires** (« où commence le repère ») : tableau lu en ordre de lecture, apparié par numéro absolu de caractère depuis le **début** ou depuis la **fin** des deux faces (avec et sans le « ? » compté, avec et sans la ligne d'en-tête du tableau).
**Fonctions** (X = rang dans A–Z ou dans KRYPTOS ; L = lettre-étiquette de la ligne du tableau) :
X(H) ; X(H) − X(L) ; X(L) − X(H) ; X(H) ± colonne ; X(H) ± ligne ; X(H) ± position absolue i ; X(L) ; colonne ; ligne ; i ; colonne + ligne ; distance au bord min(j, 30 − j).
**Chiffrement** : alphabet clair et alphabet chiffré ∈ {A–Z, KRYPTOS} (4 couples, dont celui de K1–K2) × Vigenère C = P + k, Beaufort C = k − P, Beaufort variante C = P − k.
**Juge** : lettres des cribs retrouvées sur 24. Aucun score d'anglais.
**Règle de décision (fixée d'avance)** : signal seulement si K4 atteint **≥ 22/24**. Sinon, comparer le maximum de K4 à la distribution du maximum sur 200 chiffrés aléatoires (même batterie).

## Résultats (23/09)
- 40 géométries valides (32 grilles dont la ligne décalée reste dans le tableau, 8 alignements linéaires) × 15 fonctions × 2 lectures (A–Z, KRYPTOS) = 1 200 clés déterministes, × 12 conventions de chiffrement.
- Contrôle : sous EASTNORTHEAST, l'écran replié donne `HIJLMNQUVWXZK` (suite KRYPTOS du tableau), lettre-étiquette Y.
- **K4 : meilleur score 6/24** (KRYPTOS/KRYPTOS, Vigenère, écran replié d = −1, clé = rang A–Z de H − rang de l'étiquette).
- Hasard (200 chiffrés, même batterie) : maximum {4 : 2, 5 : 86, 6 : 94, 7 : 16, 8 : 2} ⇒ **P(≥ 6) = 56 %**.
- ⇒ **aucun signal** : aucune fonction simple de la position et de la lettre du tableau au dos ne donne la clé de K4, dans aucune des géométries testées.
  Contrairement au test à table libre, ce test **tranche** : une vraie règle de cette famille donnerait 24/24 (ou 23), loin au-dessus du hasard (≤ 8).
