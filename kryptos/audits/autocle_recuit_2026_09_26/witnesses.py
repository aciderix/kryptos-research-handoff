#!/usr/bin/env python3
"""Génère des témoins « K4 mélangé » : permutations aléatoires des 97 lettres de K4
(fréquences conservées), cribs NON imposés (le témoin est un chiffré de contrôle brut).
Usage: witnesses.py N [seed] -> N lignes de 97 lettres."""
import sys, random
K4 = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
def main():
    n = int(sys.argv[1]); seed = int(sys.argv[2]) if len(sys.argv) > 2 else 100
    rng = random.Random(seed)
    for _ in range(n):
        L = list(K4); rng.shuffle(L); print("".join(L))
if __name__ == "__main__":
    main()
