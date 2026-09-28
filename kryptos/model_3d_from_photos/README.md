# Kryptos — géométrie de l'écran extraite du modèle 3D (v2, amélioré)

Données dérivées **du modèle 3D imprimable de Bret L. Bowen** (`K4HasGotMe`),
construit à partir de **photos + positions/police de lettres de Gary Phillips**.
Extraction : ouverture du `.blend` (441 Mo, Blender 2.79) en Blender headless
(`bpy`), dépliage de la tôle en S, ray-casting à travers chaque découpe.

## ⚠️ Statut de la donnée (à garder avec les fichiers)
- **Reconstruction**, pas un relevé de terrain. Positions **cohérentes avec le
  modèle** et fidèles à sa précision ; pas garanties au millimètre sur l'œuvre réelle.
- Bon pour **structure, proportions, alignements** ; à ne pas sur-interpréter en absolu.

## Échelle — réponse à « est-ce à l'échelle ? »
Le modèle est en **unités Blender arbitraires** (dimensionné pour l'impression),
**sans échelle réelle embarquée**. On la cale sur **une** mesure connue.
- Ancrage retenu : **hauteur d'écran de cuivre = 12 ft** → **1 unité ≈ 0,0963 ft ≈ 2,94 cm**
  (soit **0,02935 m/unité**).
- **Contrôle de cohérence fort** : avec cet ancrage, l'**emprise totale** sort à
  **≈ 6,31 × 6,25 × 5,31 m (≈ 20,7 × 20,5 × 17,4 ft)** — ce qui **correspond au
  « 12 ft × 20 ft » documenté** de Kryptos. ⇒ le modèle est **bien aux proportions
  réelles**, l'échelle absolue est fiable à quelques %.
- Pour re-caler : multiplier toute longueur en unités par le facteur ci-dessus
  (les CSV donnent déjà les colonnes en **mètres**).

## Inventaire spatial (toute la scène, à l'échelle 12 ft)
| Élément | Dimensions (m) | Centre (unités) |
|---|---|---|
| Écran de cuivre (les lettres) | 2,82 × 4,31 × **3,66 (haut.)** | (−27,8 ; 21,2 ; 76,7) |
| Tronc pétrifié | 1,01 × 1,07 × **4,90 (haut.)** | (−90,9 ; −44,6 ; 97,5) |
| Plateforme (base) | **6,31 × 6,25** × 0,76 | (−13,8 ; 27,4 ; 13,0) |
| Rocher marbre | 1,55 × 1,05 × 0,51 | (16,3 ; −11,0 ; 22,8) |
| Rocher grès | 0,86 × 2,69 × 0,39 | (64,8 ; 48,0 ; 20,8) |
| **Scène totale** | **6,31 × 6,25 × 5,31** | — |

Coordonnées complètes (min/max/centre) de chaque pièce dans `space_and_scale.json`.
Axe **z = vertical** ; le S vit dans le plan x‑y.

## Géométrie des deux panneaux (le S)
| | Panneau chiffré (K1–K4) | Table de Vigenère |
|---|---|---|
| Centre d'arc (x,y) | (−64,3 ; −7,2) | (3,7 ; 49,6) |
| Rayon | 43,83 u ≈ **1,29 m** | 43,90 u ≈ **1,29 m** |
| Longueur d'arc | ≈ 126 u ≈ **3,70 m** | ≈ 131 u ≈ **3,86 m** |
| Hauteur | 124,6 u ≈ **3,66 m (12 ft)** | idem |
| Grille | **28 lignes × ~31** | 28 × ~31 |
| Lettres détectées | **870** (≈ 869 attendu) | 879 |

- **Rayons quasi égaux** → S symétrique ; centres distants ≈ 2R → arcs **presque
  tangents** (inflexion nette). Les deux cartes plates se superposent proprement
  (test overlay / « orientation inversée » de Phillips possible).
- **K4** = **97 derniers caractères** du panneau chiffré → **~3 dernières lignes**
  (voir `letters_cipher_panel.csv`, `row` ≥ 25).

## Validation
Ligne 1 du chiffré lue sur le pochoir = `EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJ`
(texte canonique). Lignes 2–4 concordent. 28 × ~31 ≈ 868 ≈ total connu (869).

## Fichiers
- `kryptos_cipher_panel_flattened.png` — pochoir plat **chiffré K1–K4** (HD, lisible).
- `kryptos_tableau_panel_flattened.png` — pochoir plat **table de Vigenère**.
- `letters_cipher_panel.csv` / `letters_tableau_panel.csv` — une ligne/lettre :
  `row,col,x,y,z,arc_s_units,x_m,y_m,z_m,arc_s_m,z_from_base_m`.
  - `x,y,z` = **3D monde** (unités modèle) ; `*_m` = **mètres** (ancrage 12 ft).
  - `arc_s_*` = **carte plate** (largeur dépliée) ; `z_from_base_m` = hauteur depuis le bas.
- `space_and_scale.json` — inventaire spatial complet + géométrie des panneaux.

## Méthode (reproductible)
1. `Kryptos.Part.CopperSheet` : maillage watertight, 109 183 sommets ; lettres =
   tunnels traversant la dalle.
2. Empreinte x‑y = deux arcs de cercle ; fit cercle à axe vertical par panneau.
3. Dépliage (angle→arc)×z ; rayon radial à travers l'épaisseur : touche = matière,
   rien = découpe → image pochoir.
4. Lettres : bandes de lignes (projection horizontale) + centres (pics de la
   projection verticale lissée). Report en 3D et déplié, converti en mètres.
