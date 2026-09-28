# Z32 — état de l'art vérifié, sources primaires et registre d'antériorité (audit du 2026-09-28)

Même règle que pour d'Agapeyeff : **« pas testé chez nous » ≠ « jamais testé »**. Niveaux : `PRIMAIRE`, `VÉRIFIÉ`
(source ouverte et lue), `CITÉ` (référence trouvée mais contenu non consulté), `INTROUVABLE`.

## 1. Sources primaires consultées
| Source | Statut | Apport |
|---|---|---|
| Transcriptions de D. Oranchak (`oranchak.com/zodiac/wiki/cipher_{0..3}.txt`), copiées dans `data/` | VÉRIFIÉ | Z340, Z408, Z13, Z32 en ASCII (Z32 : `C9J|#OktAMf8oORTGX6FDVj%HCELzPW9`, 29 classes). La chaîne `C9J|#Ok[AMf8?ORTG…` du dossier fourni est une autre étiquette ASCII des mêmes glyphes |
| Clairs du Z408 et du Z340 (Hardens ; Blake-Van Eycke-Oranchak) | VÉRIFIÉ ici | alignement Z408 cohérent (2 écarts : symbole `8` ambigu, 1 erreur d'enchiffrement) ; section 1 du Z340 : transposition « cavalier » (ligne +1, colonne +2) vérifiée **sans aucun écart** sur 153 symboles |
| FBI, fichiers Zodiac (7 PDF, 749 p., fournis par le user ; empreintes `sources/fbi/SHA256SUMS_pdfs.txt` ; OCR ici) | PRIMAIRE | voir § 2 |
| Lettres du 26/06/1970 et du 26/07/1970 (Wikisource, zodiackiller.com) | CITÉ | texte de la lettre, « 0 is to be set to Mag. N. », « Radians & # inches along the radians » |
| zodiackiller.com, index des archives de presse | VÉRIFIÉ | une seule coupure datée de juin 1970 (9 juin, Appleton Post-Crescent) ; rien sur le Z32 |
| FBI Vault en ligne | bloqué (Cloudflare) | remplacé par les PDF fournis |

## 2. Découverte dans les fichiers FBI (PRIMAIRE, **non trouvée dans les sources secondaires consultées**)
- `zodiac5 pages 1-249.pdf`, p. 167 et 170 : fiches du laboratoire, demande « Document-Cryptanalysis », reçue le
  **8 juillet 1970** (airtel du 30/06/1970) : enveloppe postée à San Francisco le 26 juin 1970, « accompanying map and hand
  printed letter beginning "This is the…" ».
- p. 168 : **note manuscrite du cryptanalyste** sur la pièce **Q51** (image : `sources/fbi/zodiac5_p168_notes_cryptanalyste_Q51.png`) :
  > « 32 symbols 26 different. No pattern repetitions against previous specimens. Only similarity is 17 length of top row
  > as on previous specimens. Possible words (North, South, East, West, Miles, Yards, Feet, Bomb, Kill) slid through Q51 &
  > previous specimens with no significant "hits". Superimposition — coincidence sum of all specimens to locate possible
  > variants — negative results. »
- Conséquences :
  1. **Antériorité** : l'attaque par mots probables (points cardinaux, unités, BOMB, KILL) et la superposition avec les
     chiffres précédents ont été faites **dès juillet 1970** par le FBI, sans résultat.
  2. **Désaccord de transcription** : le FBI compte **26** symboles différents, et non 29, sur l'original. Si trois paires
     de glyphes que les transcriptions modernes distinguent (candidats visuels : triangle plein 12 / triangles vides 2 et
     32 ; F inversé 11 / F 20 ; quadrilatère plein 5 / quadrilatère à réserve 24 ; cercle centré 19 / réticule 29) sont en
     fait un même symbole, les verrous changent. Hypothèse à instruire, pas un fait établi (le cryptanalyste a pu se
     tromper de compte).
- p. 46 : inventaire des lettres (« One page letter, map and envelope postmarked 26 June 1970 »).
- `zodiac5 pages 250-372.pdf`, p. 123 : en décembre 1991, un particulier remet au FBI un document de 118 pages « Rules of
  Decoding the Mount Diablo Code » (proposition privée ; contenu non publié).

## 3. Propositions de solution — audit (références du dossier fourni vérifiées)
| Proposition | Référence | Vérification | Statut |
|---|---|---|---|
| Penn, « radian theory » (1981) | California Magazine ; Foxon 2023 § 2.1 | VÉRIFIÉ (via Foxon) | ne déchiffre pas les 32 symboles |
| Sundberg & Thelin, lecture chimique (2014) | Cipher Mysteries 2014 ; Foxon § 2.2 | VÉRIFIÉ (via Foxon) | non homophonique ; choix arbitraires |
| Grinell `ESTIMATEFOURRADIANSANDFIVEINCHES` (2020) | zodiacciphers.com | VÉRIFIÉ (via Foxon, wiki) | respecte les verrous ; E01 : compatible |
| DMW `(IN) THREE AND THREE EIGHTHS RADIANS (TEN)` (2019) | zodiackillerciphers.com/zks-attachments/8025.pdf | CITÉ | respecte les verrous ; E01 : compatible |
| Stampher, GeoCSP (2026) | dossier fourni : `github.com/davidstampher/geoCSP` → **INTROUVABLE (404)** ; bon dépôt : `github.com/dstampher/zodiac-z32-cipher` ; SSRN 6266638 | VÉRIFIÉ (code lu et **reproduit en C** : mêmes comptes 2 044 224 / 154 572 / 61) | ne contraint que les 3 égalités ; aucun contrôle nul |
| Foxon (2023) | IACR ePrint 2023/982 | VÉRIFIÉ (lu) | coordonnées (12, 3) ; `TWELVEINCHESALONGTHETHREERADIANS` viole les verrous en lecture directe |
| Ziraoui (2021) | forum zodiackillerciphers | VÉRIFIÉ (via Foxon) | clé Z340 + A1Z26 + anagramme + 2 nulles ; hors champ lettre-à-lettre |
| Case Breakers (2021) | Foxon § 2.5 | VÉRIFIÉ (via Foxon) | anagrammes ; invérifiable |
| Allen, PLSS `TOWNSHIPTHREENORTHRANGETHREEWEST` (2024) | SSRN 4715713 | CITÉ (accès refusé) | viole les verrous en lecture directe (E01) |
| Praetorian (avril 2026) | praetorian.com | VÉRIFIÉ (résumé) | pas de clair ; trigrammes + recherche de paramètres (22 500 / 1 476 combinaisons) |
| Reese (2026) | dossier fourni : `github.com/maddiedreese/zodiac-z32` → **INTROUVABLE (404)** ; bon dépôt : `github.com/maddiedreese/zodiac` | VÉRIFIÉ (README lu) | route affine (8 + 29 i) mod 32 tirée de « 1969 Edition » ; respecte les verrous avec la route ; E01 : compatible |
| Dominik (2026) | dossier fourni : `zenodo.org/` (générique) ; bons liens : Zenodo 22817822, 17966491, 18202334, 19104092 | VÉRIFIÉ (résumés) | routes 17+15, projections P408/P340, test de 10⁶ contrôles ; conclut à la sous-détermination |
| Blake, Van Eycke & Oranchak (2024) | arXiv 2403.17350, § 8.2.1 | VÉRIFIÉ (lu) | « no known test that can scientifically falsify or validate candidate solutions » |
| Cragle `THREERADIANSFROMMOUNTAREATWOINCH` ; forum `USEPIOVERTWO…`, `USETWOPOINTS…` | wiki « Z32 Solutions », Reddit | VÉRIFIÉ (wiki) / CITÉ | respectent les verrous ; E01 : compatibles |

## 4. Registre des approches computationnelles déjà faites (par d'autres)
| Approche | Qui | Couverture |
|---|---|---|
| Clés du Z408 et du Z340 appliquées au Z32 | FBI (1970, clé Z408), Ziraoui, Dominik (P408/P340), Oranchak | faite ; aucun texte |
| Mots probables glissés sur le chiffré | **FBI 1970** (note Q51) | NORTH, SOUTH, EAST, WEST, MILES, YARDS, FEET, BOMB, KILL |
| Génération exhaustive de phrases « nombre + radians + pouces » filtrées par les 3 égalités | Stampher (2 M) ; Dominik (780 840 compositions) ; « pi1 » (lien mort) | faite ; des dizaines à des milliers de survivants |
| Routes 17+15, transpositions (dont cavalier Z340, affine) | Dominik (48 routes-phases), Foxon, Reese | partielles |
| Géométrie polaire depuis le mont Diablo (déclinaison, cadran 12/24 h, pouces) | Grinell, Foxon (tests de sensibilité), Praetorian, Stampher | nombreuses |
| Statistiques de multiplicité, n-grammes, cycles d'homophones du Z32 | Oranchak (table « Cipher comparisons ») | faite (Z32 : 32 / 29, multiplicité 0,906) |
| **Contrainte par les budgets d'homophones du Zodiac (clés Z408/Z340), calibrée** | **ici (E01)** | nouvelle ; résultat négatif (≈ 2 % de puissance) |

## 5. Faits structurels recalculés ici
- 32 positions, 29 classes (transcriptions modernes), égalités 1=26 (C), 2=32 (triangle vide), 6=14 (O).
- Comparaison au système du Z408 : sur les 359 fenêtres de 32 symboles consécutifs du Z408, 33 % ont ≥ 29 symboles
  distincts (moyenne 27,6) ; Z340 : 16 % (moyenne 26,8). **Le nombre de répétitions du Z32 est banal** pour un
  homophonique à la manière du Zodiac : il ne signale ni un autre système, ni une absence de système.
- Distance d'unicité d'un homophonique de cette taille (≈ 36, citée) > 32 : aucune solution ne peut être prouvée par le
  seul chiffré (cf. `dagapeyeff/docs/00_choix_du_defi.md`, écrit avant ce travail).
