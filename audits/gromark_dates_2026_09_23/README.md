# Gromark amorcé par des DATES (1986 Égypte, 1989 Berlin) — relais Gemini du 23/09

**Amorces fixées avant calcul** : 1986, 1989, 8689, 8986, 19861, 19891, 19869, 19898, 861989, 891986, 19861989, 19891986.
Clé : chiffres de l'amorce, puis chaîne de Gromark k[i] = k[i−n] + k[i−n+1] mod 10.
Alphabets **entièrement libres** (Quagmire I–IV, n'importe lequel des 26! alphabets), Vigenère et Beaufort.
Cela couvre a fortiori les alphabets KRYPTOS, PALIMPSEST et ABSCISSA proposés par Gemini.

**Contrôle** : le moteur retrouve les trois amorces de 4 chiffres de Bean (3301, 6740, 9903) ; 1986 et 1234 échouent.

**Résultats** (`results.json`) :
- **Exact (24/24) : 0 cas sur 96.** Toutes les amorces de dates sont **éliminées**, quel que soit l'alphabet.
- **Une erreur tolérée (23/24)** : 1 seul cas, QIV (Vig et Beau), amorce 19861989, en abandonnant la position 32 ou 33 (fin de EASTNORTHEAST).
  Hasard avec la même tolérance (`slip_null.py`) : **20/200 = 10 %**. Sur 24 cas QIV, on attend 2 à 3 passages au hasard, pour 1 observé.
  ⇒ **bruit**, rejeté.
- Conclusion : **les dates 1986/1989 ne sont pas des amorces de Gromark pour K4.**
