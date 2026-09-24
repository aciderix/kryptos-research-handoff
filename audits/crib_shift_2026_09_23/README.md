# Crib décalé par une lettre sautée ou ajoutée entre les deux cribs (relais Gemini, piste 4, 23/09)

Hypothèse : une lettre sautée ou ajoutée entre EASTNORTHEAST et BERLINCLOCK (feuille ≠ cuivre) décale la **phase de la clé** du second crib de s = −3…+3.
L'appariement lettre chiffrée ↔ lettre claire, lui, est sûr : Sanborn a publié les lettres chiffrées elles-mêmes.
Portée : Quagmire I–IV, **tout alphabet**, clé périodique p = 1–26, Vig/Beau, exact ; témoin 20 chiffrés aléatoires par case (`crib_shift.py`, `results.json`).

## Résultats
- **Quagmire III (système de K1–K2)** : pour **tout décalage s ∈ [−3, +3], toutes les périodes ≤ 12 restent éliminées**, en Vigenère comme en Beaufort
  (plus petite période compatible : 13 à s = −2 en Vig. ; 15 en Beau.). Toutes les cases compatibles sont au niveau du hasard (témoin ≥ 3/20).
  Complément du test `incrib` du 22/09 (décalage **libre**) : Vig. p ≤ 11 éliminé quel que soit le décalage.
- **QI, QII, QIV** : 24 cases « sévères » compatibles (hasard ≤ 1/20) sur 773, pour 4 attendues. **Mais elles relèvent toutes d'un même phénomène** :
  la contrainte « même clé pour les lettres des cribs distantes de 38 » (p = 19 à s = 0, 18 à −2, 20 à +2, 13 à +1, 8 et 10 à +2…).
- Mesure du phénomène seul (`distance_scan.py`, 200 témoins par distance, d = 30–52) :
  seules 4 distances sont sévères (34 : 4 % ; **38 : 4 %** ; 43 : 0 % ; 45 : 2,5 %). K4 passe **uniquement d = 38**.
  Probabilité qu'un chiffré aléatoire passe au moins une de ces distances ≈ **10 %** ⇒ **non significatif**.
  Le déchiffrement à cette distance (p = 19, `../keyword_alphabet_2026_09_23/p19_decrypt.py`) donnait déjà du charabia.
- **Conclusion** : une erreur de position de ±1 à ±3 entre les cribs ne ressuscite aucune clé périodique courte. Les éliminations antérieures restent valables.
