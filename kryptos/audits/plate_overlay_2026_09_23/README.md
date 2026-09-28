# Plaque du haut posée sur la plaque du bas (23/09)

**Motivation (sources primaires, avant test).**
- Sanborn nomme lui-même « the top plate » et « the bottom plate », et décrit l'œuvre comme des couches qu'on soulève (« pull up one layer, then you can come to the next », 1990).
- Le cuivre est un « papier perforé par le texte » (1989).
- Les projections sont nées de deux plaques de Kryptos superposées devant une lumière (2009).
- Il a dessiné un « Code Breaker » posé sur un « Coded » (IMG_1555).
- Scheidt : les clés sont sur la sculpture et se retrouvent grâce aux indices présents.

**Test** (`plate_overlay.py`, transcription CIA en 28 lignes). La plaque K1–K2 sert de gabarit sur la plaque K3–K4, et la lettre du dessus sert de clé.
- Deux positions : **glissée** (ligne t sur la ligne t+14) ou **rabattue** comme une page (ligne t sur la ligne 29−t).
- Colonnes : par rang de lettre ou à largeur justifiée.
- Relations : lettre directe, Vigenère, Beaufort, Beaufort variant, en AZ et en KA.

**Résultat : 0 à 3 lettres sur 24** (le hasard en donne ≈ 0,9 par cellule ; 28 cellules). **Négatif.**

Antériorité : `scripts/team/e_k3k4_overlay_k1k2.py` (déprécié, marqué « partiel » : sans les vraies fins de ligne). Ce test en est la version déterminée.
