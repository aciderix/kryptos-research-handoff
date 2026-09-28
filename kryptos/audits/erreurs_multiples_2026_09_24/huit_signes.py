"""huit_signes.py — ce qu'on lirait sous VTTMZFPK (positions 66–73) selon l'hypothèse (feuille NOVA K3 + K4, 31 × 14).
Les 8 signes raturés (puis couverts de correcteur sur les photos de Paradigm, 2026) sont à l'aplomb des 8 premières
lettres de la dernière ligne, VTTMZFPK, dont le clair est LINCLOCK (fin de BERLINCLOCK). Tableau de référence pour
reconnaître d'emblée leur nature si l'original est un jour lu (lumière transmise, sous le correcteur)."""
CT = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
PT = "LINCLOCK"; C = CT[66:74]
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
print("chiffré 66–73 :", C, "; clair :", PT)
for n, A in (("A–Z", AZ), ("KRYPTOS", KA)):
    for mode in ("Vigenère", "Beaufort", "variante"):
        k = "".join(A[(A.index(c) - A.index(p)) % 26] if mode == "Vigenère" else A[(A.index(c) + A.index(p)) % 26] if mode == "Beaufort" else A[(A.index(p) - A.index(c)) % 26] for c, p in zip(C, PT))
        print(f"clé {mode:9s} alphabet {n:8s} : {k}")
