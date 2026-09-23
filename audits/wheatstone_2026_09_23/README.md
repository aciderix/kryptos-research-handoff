# Cryptographe de Wheatstone (cadran à deux aiguilles), test exact (23/09)

**Pourquoi.**
- Sanborn liste de sa main « Beaufort cipher, **Compass cipher**, Morse code, Alphabet code, Cryptonyms » (papiers AAA, IMG_1569).
- BERLIN CLOCK est une horloge.
- Wheatstone est aussi co-inventeur du télégraphe (le Morse de l'entrée, « délivrer un message »).
- Le procédé est lettre à lettre, sans calcul. La clé avance d'un cran à chaque tour de cadran : elle dépend du clair, ce qui échappe à tous les tests de clés périodiques.

**Test** (`wheatstone_exact.py`) :
- cadran extérieur normal ou KRYPTOS (+ un blanc, 27 positions), **alphabet intérieur quelconque** (26!) ;
- sens horaire ou inverse, lettre répétée = un tour complet ou 0 ;
- décalage inconnu entre les deux cribs.

**Résultat : incompatible dans toutes les variantes, même sur EASTNORTHEAST seul.** Contrôle positif : 20 chiffrés sur 20 reconnus (`wheatstone_control.py`).

**Antériorité :** `e_wheatstone_clock_01` (dépôt) : échantillonnage d'alphabets de mots-clés, meilleur score 8/24. Ce test-ci est **exact pour tout alphabet intérieur**.
