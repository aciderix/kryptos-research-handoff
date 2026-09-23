# Texte CHIFFRÉ de K3 comme clé de K4 (relais Gemini, 23/09)

Portée fixée avant calcul (voir l'en-tête de `k3ct_key.py`) :
- **alignement linéaire** : la clé est K3CT[i + o] (336 lettres), pour tout décalage gardant les 24 lettres des cribs ;
- **alignement par lignes** (« colonne par colonne », même plaque) : la clé de la lettre de K4 en (ligne r, colonne c) est la lettre chiffrée en (r − d, c), avec d = 1 à 11.
- 289 alignements valides × 9 modèles :
  - (A) Quagmire III, alphabet quelconque, lettre-clé lue dans le même alphabet ;
  - (B) lettre-clé convertie en nombre (rang A–Z ou KRYPTOS), alphabets QI–QIV libres ;
  - en Vigenère et en Beaufort.

**Résultats** : **K4 : 0 cas compatible.**
Témoin : 5 chiffrés aléatoires, 0 cas compatible chacun (le test est sévère : un passage aurait été significatif).
⇒ **éliminé** : le chiffré de K3 n'est pas la clé de K4, ni en alignement linéaire, ni ligne par ligne.
