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

## Azimuts réels mesurés (Option A) + réserve d'ambiguïté
| Élément | Azimut réel |
|---|---|
| Table de Vigenère — côté lecture (concave) | 140,7° (SE) |
| Cryptogramme K1–K4 — côté lecture | 319,8° (NW) |
| Arbre pétrifié (depuis centre) | 241,5° (SW) |
| Axe du S (centroïdes) | 26,8° |

**Test ENE (67,5°)** : aucun élément de la sculpture ni aucune lettre de K4 ne
s'aligne sur l'axe ENE du message. Négatif.

**Réserve d'ambiguïté** : l'« axe du S » est intrinsèquement flou (~24°) — PCA 13,6°,
centroïdes 26,8°, bout-à-bout 50,6° (calé sur le trait utilisateur). Le
géoréférencement est donc fiable à **±~24°**, pas au degré. Un relevé aérien
géoréférencé lèverait cette ambiguïté.
