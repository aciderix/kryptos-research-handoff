# Recherche sur la géométrie mesurée — K4, le « 7 », overlay (2026-09-26)

Tests **déterminés** menés sur les coordonnées extraites du modèle 3D (Bowen /
positions Phillips, **reconstruction** — voir README). Règles respectées :
texte K4 + géométrie fixés **avant** observation, contrôle positif, shuffle.

## Faits physiques déterminés (nouveaux, tirés de l'objet)
- La gravure du chiffré est une grille **28 lignes × 31 colonnes** (les 3 lignes
  pleines de K4 font exactement 31). **Largeur physique = 31.**
- **K4 = 4 + 31 + 31 + 31** : `?OBKR` en fin de la ligne de queue de K3
  (`OBKR` aux colonnes 27–30), puis `UOXOGH…KSSO`, `TWTQ…FBNYP`, `VTTM…EKCAR`.
- Les **97 lettres de K4 sont placées à des coordonnées 3D mesurées** (x,y,z) —
  fichier `letters_cipher_panel.csv`, lignes détectées 24–27.

## Test 1 — le « 7 » contre la géométrie physique
Contrôle : écart‑7 = **9** coïncidences, shuffle **p = 0,0049** (≈ le 0,004 de la
base 14 → pipeline sain).

- **Coïncidences écart‑7 = relation HORIZONTALE** : 6/9 paires sont *même ligne,
  +7 colonnes* ; les 3 autres débordent d'une ligne (−24 = 31−7). Le « 7 » vit
  **le long de la lecture/gravure**, jamais en vertical.
- **Doublets** (BB, QQ, SS, SS, ZZ, TT) aux **colonnes physiques 14, 21, 28, 7,
  11, 1** → **4/6 sont des multiples de 7**. En **rangée 1**, BB/QQ/SS tombent
  pile aux colonnes **14, 21, 28** (espacées de 7).
- **Mais** comme la largeur physique **31 ≢ 0 (mod 7)**, le motif **ne se propage
  pas verticalement** : le « col ≡ 4 mod 7 » de la base 14 (index linéaire)
  n'est **pas** une colonne physique fixe.

**Verdict (confirme la base 14 sur l'objet réel)** : le « 7 » est un phénomène
**de sens de lecture** (autoclé lag‑7 papier‑crayon), **pas une structure de
grille physique**. La largeur de gravure mesurée (**31**) **exclut 7/14/21 comme
largeur de grille du cuivre** (elles restent possibles comme paramètre *abstrait*
de chiffre, pas comme trame gravée).

## Test 2 — overlay des deux panneaux (idée « orientation inversée » / Cardan)
| | Chiffré | Table Vigenère |
|---|---|---|
| Rayon | 43,83 | 43,90 |
| Pas de colonne | 4,072 | 4,239 (**+4 %**) |
| Pas de ligne | **4,450** | **4,450 (identique)** |
| Grille | 28×31 | 28×31 |

⇒ **rangées identiques, colonnes à 4 %, même rayon** : une grille/masque posée sur
un panneau se **superpose cellule à cellule** sur l'autre. L'overlay type Cardan
est **géométriquement plausible**. **Réserve `frontier`** : l'**opération** (quelle
cellule lue, dans quel sens, quelle transformation) reste **non spécifiée** →
pas encore une expérience admissible, mais la **détermination géométrique** est
désormais disponible (elle manquait).

## Ce que ça change / ne change pas
- **Change** : on a, pour la 1ʳᵉ fois, la trame physique **mesurée** (28×31), la
  place 3D de K4, et une **confirmation objet** que le « 7 » n'est pas physique.
  L'overlay a maintenant une base métrique.
- **Ne change pas** : le verrou reste l'**alphabet libre** (base 13/14). La
  géométrie ne le fixe pas ; elle ferme des interprétations physiques et ouvre un
  overlay *déterminable* (à condition de fixer l'opération).
- **Réserve** : données = **reconstruction** (pas relevé). Solide pour la
  structure/trame ; à revalider sur un vrai relevé avant toute conclusion forte.

## Fichiers
- `k4_physical_grid_annotated.png` — grille physique de K4 annotée (doublets,
  liens écart‑7, colonnes multiples de 7).
- `letters_cipher_panel.csv` — coordonnées 3D/mètres par lettre.
- Scripts : `k4_physical_tests.py`, `k4_figure_overlay.py`.
