# Inventaire des données extraites du modèle 3D

Toutes dérivées du modèle Bowen (photos + positions Phillips → **reconstruction**).
Bon pour structure/trame/proportions ; pas un relevé au mm (voir README).

## Fichiers de données
| Fichier | Contenu | n |
|---|---|---|
| `master_cipher_letters.csv` | **Dataset maître** panneau chiffré : `gidx,row,col,letter,section,is_crib,is_q,conf,x_m,y_m,z_m,arc_s` | 869 |
| `letters_cipher_panel.csv` | positions détectées chiffré (3D + m + carte plate) | 870 |
| `letters_tableau_panel.csv` | positions détectées table de Vigenère | 879 |
| `space_and_scale.json` | inventaire spatial de toute la scène + géométrie panneaux + échelle | — |

### Colonnes de `master_cipher_letters.csv`
- `gidx` : index global 0–868 (ordre de lecture continu K1→K4).
- `row,col` : position dans la grille physique gravée.
- `letter` : identité (texte gravé canonique, 28 lignes ; inclut les `?`).
- `section` : `K4` (97 dernières) ou `pre-K4`.
- `is_crib` : True aux positions des cribs connus — EASTNORTHEAST (gidx 793–805)
  et BERLINCLOCK (gidx 835–845). (La lettre montrée = le **chiffré** à cette place.)
- `is_q` : True pour les 4 séparateurs `?` (gidx 100, 226, 288, 862).
- `conf` : `exact` (13/28 lignes : nb détecté = nb réel → identité sûre) ou
  `approx` (±1 possible dans la ligne). **K4 = exact.**
- `x_m,y_m,z_m` : coordonnées 3D en mètres (ancrage 12 ft). `arc_s` : carte plate (unités modèle).

## Faits mesurés
- **Longueurs de ligne gravées** (28) : `32,31,31,30,31,32,31,31,32,31,30,31,31,31,
  32,30,31,30,32,30,32,31,33,29,31,31,31,31` (somme 869). **Varient 29–33** — la
  gravure n'est **pas** un monospace rigide ; mais **K4 = 31,31,31,31**.
- **Pas de lettre** ≈ 3,69 u ≈ **0,108 m** ; **pas de ligne** ≈ 4,45 u ≈ **0,131 m**.
- **Régularité** : CV des écarts ≈ 0,27, mais **gonflé par le bruit de détection**
  (lettres manquées en bord). Résidus vs grille idéale : médiane ≈ 2,6 u (< une
  case). Carte `cipher_grid_residuals.png` : intérieur uniforme, pics = artefacts
  de bord. **Aucun « code » d'espacement systématique visible** à cette fidélité.

## Tests menés (voir RECHERCHE_geometrie_2026_09_26.md)
- « 7 » vs géométrie → horizontal (autoclé), pas d'appareil ; largeur 31 exclut 7/14/21.
- Overlay deux panneaux → co-registrables (rangées identiques, col 4 %).
- Fermeture tube/8 → adjacences déterminées mais **ni crib ni mot ni déchiffrement**.
- Espacement → pas de code visible (bruit-dominé).

## Limites
- Identité `approx` sur 15/28 lignes (bruit ±1) ; K4 sûr.
- Table de Vigenère : positions extraites ; identités = table Kryptos standard
  (clé KRYPTOS) — non ré-étiquetées cellule par cellule ici.
- Reconstruction, pas relevé : revalider sur photogrammétrie calibrée avant
  conclusion forte.

## Maillage portable
- `mesh/copper_sheet_meters.obj` — l'écran de cuivre (les DEUX panneaux, lettres
  découpées incluses) exporté en OBJ, **mis à l'échelle en mètres** (ancrage 12 ft).
  109 183 sommets, 231 770 faces. Chargeable dans n'importe quel outil 3D
  (Blender, MeshLab, trimesh, CloudCompare…). Script : `scripts/export_obj2.py`.
