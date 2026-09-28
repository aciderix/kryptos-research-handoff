# Reproduire l'extraction

Prérequis : `pip install bpy scipy pillow numpy` (bpy = Blender headless, testé 5.0.1
sur Python 3.11). Décompresser `../KryptosSculpture3D.Bowen.Public.zip` pour obtenir
`KryptosSculpture3DPrintable.Public.blend` (441 Mo) dans le dossier de travail.

Ordre :
1. `dump_verts.py` — sort `copper_verts.npy` (sommets monde de l'écran) + axes.
2. `unroll2.py` — fit des deux arcs, écrit `circles.npy`, `assign.npy`, stencils bruts.
   (fournis ici pour éviter de recalculer ; regénérables.)
3. `improve.py` — rasterisation HD des deux panneaux (ray-casting), écrit
   `hi_cipher.png` / `hi_tableau.png`, `space_and_scale.json`, `grid_*_hi.csv`,
   + inventaire spatial de toute la scène et échelle.
4. `grid_extract.py` — grille par projection (28×31), écrit `grid_*.csv` (a besoin
   de `geo.json`, fourni).
5. `k4_physical_tests.py` — tests déterminés sur K4 (écart-7, doublets, shuffle).
6. `k4_figure_overlay.py` — figure annotée de K4 + stats overlay des deux panneaux.

Note : les positions de lettres viennent de la **reconstruction** de Bret Bowen
(photos + police Gary Phillips), pas d'un relevé de terrain (voir ../README.md).
