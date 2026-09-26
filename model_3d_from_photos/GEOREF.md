# Géoréférencement du modèle (calé sur le plan de site NSA)

**Source** : plan NSA DOCID 4110824 (14 NOV 91), avec flèche Nord. Trait utilisateur :
axe de la sculpture mesuré à **50,6° depuis le Nord** (bout NE = table de Vigenère,
bout SW = cryptogramme K1–K4).

**Transformation retenue (Option A)** : rotation pure (det = +1, **pas de miroir** →
lettres à l'endroit, identiques à la vraie sculpture). Alignement : axe long du S
(bout-à-bout) → azimut 50,6°.

**Repère résultant (model → monde)** :
- Nord réel, en coords MODÈLE : **(−0,323 ; 0,947 ; 0)**
- Est réel, en coords MODÈLE : **(0,947 ; 0,323 ; 0)**
- Haut = +z modèle.

**Contrôles croisés (cohérents avec le plan)** :
- Table de Vigenère (lecture/concave) → face SE (vers le whirlpool / Est) ✓
- Cryptogramme K1–K4 → lobe SW ✓
- Arbre pétrifié → SW ✓
- Lettres à l'endroit (rotation pure) ✓

**Réserve** : ancrage sur un plan dessiné à la main → ±quelques degrés. Un relevé
aérien géoréférencé affinerait au degré près.

Figure : `figures/georef_aligned_optionA.png`. Scripts : `scripts/georef_align.py`,
`scripts/idee3_correct.py`.
