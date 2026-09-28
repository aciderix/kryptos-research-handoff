# Z32-E01 — Test de compatibilité avec le système de chiffrement démontré du Zodiac (budgets d'homophones) — PRÉ-INSCRIPTION

Rédigée et commitée avant tout calcul sur les candidats Z32 (2026-09-28).

## Motivation
Blake, Van Eycke & Oranchak (arXiv 2403.17350, § 8.2.1) : « for both cryptograms [Z13, Z32], there is **no known test
that can scientifically falsify** or validate candidate solutions ». Les travaux publiés sur le Z32 (Stampher/GeoCSP,
Dominik, Foxon, Reese, Grinell, Allen, Praetorian) ne contraignent un clair que par les **3 égalités** de symboles
(positions 1=26, 2=32, 6=14) : ailleurs, n'importe quelle lettre est admise. Or le Zodiac a montré **deux fois** comment il
construit une clé homophonique (Z408 : 54 symboles ; Z340 : 63) : un **nombre limité d'homophones par lettre**, grand
pour E/T/A/O/I/N/S, souvent **un seul** pour les lettres rares — et H n'en a que 2 (Z408) ou 1 (Z340). Un clair qui, sous
le motif du Z32, exige 4 symboles différents pour H demande une clé que le Zodiac n'a jamais faite. Ce test, fondé sur ses
habitudes documentées et non sur une préférence géographique, n'a été trouvé dans aucune source consultée (voir
`docs/01_etat_de_lart.md`).

## Définitions
- Motif du Z32 : partition des 32 positions en classes de symboles identiques (transcription `data/z32_cipher_oranchak.txt` ;
  29 classes ; égalités 1=26, 2=32, 6=14). Pour un clair candidat P (32 lettres A-Z, éventuellement réordonné par la
  transposition que propose son auteur), **verrou** : deux positions de même symbole ont la même lettre.
- Besoin k_L(P) : nombre de classes de symboles distinctes portées par la lettre L dans P.
- Clés de référence (comptes d'homophones n_L, dérivés ici des clés résolues, `data/`) :
  K408 : E7 I4 T4 O4 N4 A4 S4 L3 R3 H2 F2 D2, autres 1 ; K340 : E6 T6 I5 A5 R5 N5 O4 S4 U3 L3 D3 P2 Y2 W2 B2, autres 1.
- Excès : X_K(P) = Σ_L max(0, k_L − n_L). Vraisemblance secondaire (choix uniforme des homophones) :
  ℓ_K(P) = Σ_L log[ n_L!/(n_L − k_L)! / n_L^{m_L} ] (−∞ si k_L > n_L), m_L = occurrences de L.

## Calibration (avant les candidats) — taux de faux rejet entre clés du Zodiac
La clé du Z32 est inconnue ; on mesure donc ce que coûte de juger un **vrai** chiffré du Zodiac avec **son autre** clé :
- fenêtres de 32 du Z408 (clair connu, symboles réels), jugées avec K340 ;
- fenêtres de 32 du Z340 (section 1, ordre du clair après transposition connue, symboles réels), jugées avec K408.
Seuil **v** = plus petit entier tel que X ≤ v pour ≥ 95 % des fenêtres de contrôle **dans les deux sens** (règle fixée ici).
Un candidat est dit **incompatible avec les clés du Zodiac** si X_K408 > v **et** X_K340 > v.

## Leurres (puissance)
Fenêtres de 32 lettres de textes anglais réservés (`dagapeyeff/data/heldout/en.txt`) qui satisfont les verrous du Z32 ; et
phrases du générateur de Stampher (grammaire « nombre + radians + pouces », 2 044 224 phrases) qui satisfont les verrous.
Puissance = fraction de leurres jugés incompatibles.

## Candidats publiés (liste fermée, chaînes exactes)
Grinell 2020 `ESTIMATEFOURRADIANSANDFIVEINCHES` ; DMW/Stampher `INTHREEANDTHREEEIGHTHSRADIANSTEN` ; Reese
`THREEANDTHREEEIGHTHSRADIANSSIXIN` avec sa route (position(i) = (8 + 29 i) mod 32) ; Allen
`TOWNSHIPTHREENORTHRANGETHREEWEST` ; Foxon `TWELVEINCHESALONGTHETHREERADIANS` (ordre direct ; sa variante à transposition
Z340 est notée à part si elle est explicitée) ; Cragle `THREERADIANSFROMMOUNTAREATWOINCH` ; forum
`USEPIOVERTWOFOURANDTHREEQUARTERS`, `USETWOPOINTSFOURANDTHREEQUARTERS`. (Ziraoui, Sundberg-Thelin, Praetorian, Case
Breakers : pas de clair de 32 lettres lettre-à-lettre ⇒ hors champ, noté.)

## Rapport
Pour chaque candidat : verrous ; vecteur k ; X_K408, X_K340 ; ℓ ; percentile parmi les leurres satisfaisant les verrous.
**Portée annoncée** : un rejet signifie « exigerait une clé construite autrement que les deux clés connues du Zodiac » —
pas une impossibilité logique (il a pu changer de méthode). Une compatibilité ne valide rien (distance d'unicité ≈ 36 > 32).
