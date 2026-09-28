# Superposition des deux faces de l'écran (23/09)

**Relevé (photos CIA et Gillogly).**
- Côté chiffré : 28 lignes (14 par plaque). **K3 commence en tête de la plaque du bas** (ligne 15, « ENDYAHROH… »), ce qui confirme que « deux systèmes pour le texte du bas » (Sanborn, 1990) désigne K3 + K4.
- K4 occupe les lignes 25 (fin, après « ? ») à 28, chacune de 31 caractères. EASTNORTHEAST tient entièrement dans la ligne 26 ; BERLINCLOCK est coupé entre les lignes 27 et 28.
- Tableau : 28 lignes (en-tête, lignes A–Z avec le L en trop à la ligne N, pied de tableau), gravé pour être lu **de dos**.

**1. Test fixé à l'avance** (`fold_overlay.py`) : replier l'écran comme un livre, pour que le tableau se lise de face derrière le chiffré (position j sur position j). Variante unique : le tableau vu de face en miroir. Relations essayées : lettre directe, ou Vigenère, Beaufort, Beaufort variant, en AZ et en KA. **Résultat : 0 à 2 lettres sur 24** (le hasard en donne ≈ 1). Négatif.

**2. Fermeture générale** (`row_aligned_closure.py`). Le long d'une ligne du tableau, les lettres se suivent dans l'ordre KA (ou à rebours en miroir). Toute superposition où les lignes du tableau restent parallèles à celles du chiffré (quels que soient le décalage horizontal, l'appariement des lignes et le miroir) donne donc, sous EASTNORTHEAST, une clé qui avance de ±1 dans KA. Or aucune convention ne donne cette progression (pas observés : `results_row_aligned_closure.txt`).
⇒ **Toute la classe « tableau posé sur le chiffré, lignes parallèles, lettre du tableau = clé » est éliminée**, sans énumération.

**Hors portée :** une superposition tournée ou en diagonale, un gabarit qui **sélectionne** des lettres au lieu de les combiner (peu compatible avec « BERLINCLOCK correspond un à un à NYPVTTMZFPK », Sanborn 2019), ou un gabarit extérieur à la sculpture.
