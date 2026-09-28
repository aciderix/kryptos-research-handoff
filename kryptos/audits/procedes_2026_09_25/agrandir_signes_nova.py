"""agrandir_signes_nova.py — agrandit et contraste la zone des 8 signes sous la dernière ligne de la feuille K3 + K4,
sur la seule image NOVA (2006) disponible dans les dossiers du groupe (`Personal Folders/pi/K4 NOVA.jpg`).
Résultat (25/09/2026) : 8 taches noires de la taille d'une lettre, déjà biffées au feutre ; illisibles.
Usage : python3 agrandir_signes_nova.py sortie.png   (Pillow requis ; les images ne sont pas versionnées, cf. .gitignore)"""
import sys, os
from PIL import Image, ImageOps, ImageEnhance
src = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../sources/groupsio_membres/fichiers/Personal Folders/pi/K4 NOVA.jpg")
im = Image.open(src).convert("L").crop((160, 340, 470, 545))
im = im.resize((im.width * 4, im.height * 4), Image.LANCZOS)
im = ImageEnhance.Contrast(ImageOps.autocontrast(im, cutoff=1)).enhance(1.8)
im.save(sys.argv[1] if len(sys.argv) > 1 else "nova_huit_signes_agrandi.png")
