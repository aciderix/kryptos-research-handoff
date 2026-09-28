# Provenance des données Z32

| Fichier | Source | Remarque |
|---|---|---|
| `z32_cipher_oranchak.txt`, `z13_…`, `z340_…`, `z408_…` | D. Oranchak, `http://oranchak.com/zodiac/wiki/cipher_{3,2,0,1}.txt` (table « Cipher comparisons » du wiki zodiackillerciphers.com), téléchargés le 2026-09-28 | étiquettes ASCII neutres ; `t` = flèche, `o` = forme Ω, `f` = F inversé, `8` = triangle plein, `9` = triangle vide, `#`/`%` = quadrilatères, `6` = cercle centré, `z` = réticule |
| `z408_plaintext_aligned.txt` | clair des Hardens (1969), orthographe du Zodiac conservée, 390 + 18 lettres | alignement vérifié ici : 2 écarts (symbole `8` : A ×5 / S ×3 ; 1 erreur d'enchiffrement en 106) |
| `z340_plaintext.txt` | Blake, Van Eycke & Oranchak 2020 (arXiv 2403.17350), orthographe du chiffré conservée | section 1 : transposition « ligne +1, colonne +2 » vérifiée sans écart |
| `../sources/fbi/*` | fichiers FBI « The Zodiac Killer » (Vault), 7 PDF fournis par le user (zip Google Drive) ; empreintes dans `SHA256SUMS_pdfs.txt` ; OCR tesseract ici | PDF non versés (76 Mo) ; extraits et images des pages citées seulement |

Comptes d'homophones dérivés (E01) : K408 = E7 I4 T4 O4 N4 A4 S4 L3 R3 H2 F2 D2, autres 1 ;
K340 (sections 1-2, vote majoritaire) = E6 T6 I5 A5 R5 N5 O4 S4 U3 L3 D3 P2 Y2 W2 B2, autres 1.
