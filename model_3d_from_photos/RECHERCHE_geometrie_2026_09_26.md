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

## Addendum — « refermer en tube / en 8 » (idée utilisateur, 26/09) — TESTÉ, NÉGATIF
Géométrie déterminée (une seule tôle → rangées à même hauteur ; col30 = centre/inflexion, col0 = bord extérieur, pour les DEUX panneaux) :
- **Jonction centrale** (croisement du 8) : cipher‑col30 ↔ tableau‑col30.
- **Nouvelle jonction en fermant** : cipher‑col0 ↔ tableau‑col0.

**(a) Superposition (tube = fold, lettre table = clé Vigenère sur K4)** : cribs, registrations naturelles (col directe/miroir × Vig/Beaufort/Variante, rangées fixées par la hauteur). Meilleur = **3/24** ; hasard = 0,93 moy, max 6/24 → **bruit, aucun signal**. Ne déchiffre pas.

**(b) Adjacence (couture)** : la colonne extérieure de la table = la clé `KRYPTOSABC…` (sa 1ʳᵉ colonne) vient border le chiffré. Colonnes de couture (haut→bas) : bord chiffré `EYVGTQYHEFFEDDECTWTETBAREUTV`, centre `JDEGARIEXFQEPGAEEERBIBTEROPR` — aucun mot, aucun crib.

**Verdict** : adjacences déterminées, mais ni crib ni mot ni déchiffrement. Seul fait notable : la colonne‑clé KRYPTOS borde le chiffré à la fermeture. Élargir au‑delà de ces registrations = fishing (interdit par le frontier). Script : `scripts/tube_eight.py`.

## Addendum 2 — Tests 3D « sculpture » (idées lumière/point de vue/normales) — 26/09
Méthodo : portée d'avance, cribs comme falsificateur, baseline hasard.

- **#3 Overlay par NORMALES** (`scripts/normal_overlay.py`) : pour chaque lettre de K4,
  rayon le long de la normale radiale → cherche la table. **6/97 atteignent la table
  (sortant), 0/97 (entrant).** Les deux lobes du S **tournent le dos** l'un à l'autre
  (convexes vers l'extérieur). Overlay-par-normale **géométriquement absent**. NÉGATIF.
- **#2 Anamorphose / POINT DE VUE** (`scripts/viewpoint_overlay.py`) : 12 000 points de
  vue ; projection centrale des deux panneaux ; appariement des lettres qui se
  superposent → clé → cribs. 1 756 vues donnent ≥20 superpositions, mais meilleur
  score **3/24** (hasard : 0,91 moy, max 7/24). NÉGATIF (bruit).
- **#1 Lumière/ombre (version alignement)** = projection centrale depuis un point =
  mathématiquement #2 → NÉGATIF. (La version « ombre parallèle » = le dépliage, déjà
  fait, redonne le texte, pas d'info neuve.)

### Réserve METHODO majeure (vaut pour toute la suite)
Le modèle est une **reconstruction** (photos + police Phillips). Donc :
- **Fiables** (géométrie GROSSIÈRE captée par photos+police) : centroïdes de lettres,
  relation entre panneaux, courbure du S, points de vue. → tests #2/#3/#5/#6/#10 défendables.
- **NON fiables sur ce modèle** (détail FIN = choix du modeleur, pas Sanborn) : angle des
  tunnels, profondeur exacte, orientation sous-lettre, trous « non-lettre ». → idées
  #4/#7 mesureraient Bowen, pas Sanborn : **à ne PAS conclure sur ce modèle** ; exigent
  un relevé réel.
- **Bloqué** faute de géoréférencement (pas de nord connu dans le modèle) : ombres
  solaires datées (#8).

## Addendum 3 — Idées optiques (lumière / cylindre / vecteur) — 26/09
Toutes déterminées, cribs + baseline hasard (0,93 moy). Scripts : `optics_tests.py`, `idee1_conic.py`.

- **IDÉE 5 « machine à laver » (cylindre coaxial)** : enrouler table + K4 sur un même
  cylindre, superposer (direct/retourné), lire la lettre-table comme clé Vigenère/Beaufort.
  cribs 0–2/24 = **bruit. NÉGATIF.**
- **#5 Ordres de lecture géométriques → écart-7** : l'ordre gravé (haut→bas) porte
  l'écart-7 = 9 (z=+3,26) ; **aucun** autre parcours 3D (angle, x, y, arc, distances)
  ne dépasse le bruit (z ≤ +1). ⇒ le signal autoclé vit dans **l'ordre de lecture gravé**,
  pas dans un parcours spatial. **Négatif pour l'ordre spatial ; léger contre « gravé = mélangé ».**
- **IDÉE 1 « lanterne magique » (projection conique)** : lumière au centre d'arc de la
  table → rayons par les trous → panneau chiffré. 38/879 rayons touchent le chiffré (4 %,
  rasants) depuis le centre-table ; **0** depuis centre-chiffré / inflexion / arbre.
  **Géométriquement ABSENTE** (les lobes se tournent le dos).

### Motif fort (conclusion de branche)
**TOUT appariement statique table↔chiffré qu'on peut construire est négatif** : overlay 2D,
normales (#3), projection conique (IDÉE 1), superposition par point de vue (#2), enroulement
cylindre (IDÉE 5), tube/8. + raison théorique : le mécanisme survivant est une **autoclé**
(clé générée par le texte, écart 7), donc **la clé n'est pas sur un panneau statique**. →
La famille « un panneau est la clé optique de l'autre » est **fermée par la preuve**.

### Ce qui reste, et ce qui bloque
- **IDÉE 2 (rayon ENE) / IDÉE 3 (masque solaire)** : **bloquées** — le modèle n'a **pas de
  nord** (pas de géoréférencement). Débloquables si on fournit l'orientation nord du modèle
  (ou la direction de la rose des vents en coordonnées modèle).
- **Détail fin (tunnels #4, profondeur, trous)** : **non fiable** sur une reconstruction
  (mesure Bowen, pas Sanborn) → exige un relevé calibré réel.

## Addendum 4 — Orientation réelle (doc NSA trouvé) — 26/09
Doc NSA localisé : `sources/.../psizx29/KRYPTOS-Statue-NSA.pdf` (FOIA 2014, The Black
Vault, DOCID 4112086, p.4). C'est le « plan/indices de site » NSA cherché.
Faits d'orientation (doc NSA + web) :
- Écran S au **coin nord-ouest** de la cour.
- **Lodestone** magnétique + **boussole gravée** dans une pierre plate → dévie vers
  **WSW ~240°** (texte NSA « South-by-SouthWest » ; annotation manuscrite « 230°–240° » ;
  vue aérienne Elonka « ~220° »).
- **Rose 16 points ; ENE = 67,5°** = axe que « pointe » le message (NORTHEAST) ; ligne
  de visée sur l'axe intercardinal.
- 2 bancs (Nord / Sud) de part et d'autre du bassin.

**Blocage géoréférencement (honnête)** : ces données sont **qualitatives/imprécises**
(éléments boussole ~220–240°, volontairement déviés ; azimut de l'écran non documenté).
Le modèle n'a **pas de nord embarqué**. Déduire le nord des 2 rochers (lodestone vs
pierre-boussole) laisse ~20° d'incertitude + risque de reflet (handedness Blender). Une
simulation d'ombres/rayon (IDÉE 2/3) sur un nord à ±20° serait **garbage-in**.
**Débloquer proprement** = azimut réel mesuré de l'écran (relevé/aérien géoréférencé),
OU sweep du nord 0–360° traité comme paramètre inconnu cherché (avec contrôle), plutôt
qu'un nord fabriqué.

## Addendum 5 — Plan de site NSA trouvé + IDÉE 3 (ombres solaires) — 26/09
**Plan de site NSA** (DOCID 4110824, 14 NOV 91) fourni par l'utilisateur : diagramme de
la cour AVEC flèche Nord. Extraction : Nord = haut ; **table de Vigenère au NORD**,
**cryptogramme K1–K4 au SUD** (axe long du S ≈ N–S) ; lecture côté concave (est) ;
lodestone+boussole à l'**entrée ouest** (hors sculpture → IDÉE 2 non calculable sur le modèle).

**Nord du modèle** dérivé : `Nm = normalize(cA−cB)`. Soleil (NOAA) à Langley :
Inaug 5/11/1990 15h = alt 20°, az 228° (SW) ; Berlin 9/11/1989 ~13h = alt 32°, az 197°.

**IDÉE 3 (ombre arbre+panneaux sur K4)** `scripts/idee3_solar.py` — **NÉGATIF (artefact)** :
le soleil bas ombre **75–96 lettres/97**. Le « crib-overlap 22–24/24 » est un pur effet de
**couverture** (77–99 % ombré → cribs ombrés par force), pas un masque sélectif. Contrôle :
cribs 92 % vs non-cribs 73 % à 15h = effet de **position** (K4 en bas, face détournée du
soleil SW), pas un masque cryptographique. Sweep du nord : « meilleur » = 24/24 mais 97/97
ombré (nuit totale). **Aucun masque stéganographique.** Seul fait thématique (non décodant) :
à 15h l'ombre pointe ENE ~48°.

## Addendum 6 — Géoréférencement corrigé + IDÉE 3 refaite — 26/09
Correction (merci utilisateur) : mon Nord d'IDÉE 3 était faux. Calé sur le plan NSA
(axe S = 50,6° de N, mesuré sur le trait utilisateur). Transform Option A (rotation
pure, det=+1, lettres à l'endroit). Nord modèle = (−0,323 ; 0,947). Détails : GEOREF.md.
**IDÉE 3 refaite à l'orientation correcte** : cribs ombrés 79–96 % = **non-cribs** 84–97 %
→ aucune sélectivité, **négatif définitif** (le 22–24/24 antérieur = artefact de couverture confirmé).

## Addendum 7 — Transposition à la largeur physique mesurée (31) — FERMÉ
Croisement géométrie × porte « non-1:1 ». Testé sur K4 : ordre gravé vs colonnes,
boustrophedon, largeurs 7/14/21/**31** (physique mesurée), lecture par blocs 4+31×3.
Métrique = écart-7 (z vs 20 000 permutations) + concentration des doublets (mod 7).
**Résultat** : seul l'ordre gravé porte le signal (écart-7 z=+3,26 ; conc 0,83).
**Aucune** transposition ne le renforce (toutes → z≤+2,1 et conc≈0,50). → le signal
est intrinsèque à l'ordre de lecture (cohérent autoclé + doublets manuels).
**La porte transposition est fermée jusqu'à la largeur physique 31 incluse.**
Recoupement algébrique confirmé : pos 32 (S→S, P[25]=N) ⇒ σ(N)=0 ; pos 73 (K→K,
P[66]=L) ⇒ σ(L)=0 ⇒ σ(N)=σ(L) contradiction ⇒ autoclé pur impossible sans erreur ;
avec 1 erreur, 0 pouvoir ; 2 alphabets libres, indécidable (T36b). Le verrou = σ libre.

## Addendum 8 — Piste « artiste » : Morse K0 comme clé (F-10, jamais exécutée) — NÉGATIF
Portrait Sanborn (base 01) → clé = motif VISUEL lu par position (gabarit/pochoir, « on
retourne la feuille et on éclaire », dessin IMG_1555 « Code Breaker plate on Coded plate »),
non-mathématique, 1:1. Fil binaire : Morse (dot/dash), Martinsburg anglais→binaire via aimant,
Scheidt « masques binaires 1=0 », « changer la base du langage vers autre chose ».
**Test F-10 (jamais fait) : texte Morse K0 décodé comme clé courante de K4.** Vérif = reproduit-il
les 24 lettres de flux de clé connues aux cribs (σ=AZ & KRYPTOS ; Vig/Beau/Var ; tous décalages,
K0 et K0 inversé) ? **Meilleur = 6/24 (hasard 3,5 moy, max 7) → bruit. NÉGATIF au niveau lettres.**
La version binaire (bits Morse → clé) reste sous-déterminée (convention d'extraction libre = fishing).
Conclusion : les motifs visuels ÉNUMÉRABLES (Morse texte, overlay panneaux, ombres, décalage
boussole, transposition) sont tous négatifs → si clé = gabarit physique « individuel » (Sanborn),
il faut le VOIR, pas le reconstituer (converge avec base 01 §358).

## Addendum 9 — Idée diagonale (utilisateur) : table KEY régulière + diagonales du chiffré
- **Panneau KEY** (table de Vigenère) : opérations diagonales (retirer diagonales uniformes,
  garder extrémités/entre) → n'extraient que la **structure régulière** de la table
  (alphabet droit vs keyed) ; **zéro information** (table = outil, pas cachette). La seule
  déviation gravée = le L en trop (Sanborn: esthétique).
- **Panneau CODE** (chiffré gravé, 28 lignes) : coïncidences directionnelles vs hasard.
  ↘ (bas-droite) déborde : **53 vs 35,4±5,7, z=+3,07**. MAIS localisé à **49/53 dans K1-K3**
  (lignes 16-22), lettres E/F/T/N/D = **fréquences anglaises** (K3 = transposition → garde
  l'anglais). **Sur K4 SEUL : ↘ z=+0,52, ↙ z=+1,13 = rien.** Seul l'écart-7 survit sur K4.
- **Verdict** : le détecteur diagonal marche (s'allume sur l'anglais de K1-K3) mais **K4 est
  plat sur toutes les diagonales**. Idée diagonale fermée pour K4.

## Addendum 10 — Clés dérivées Morse/binaire/rangs (idée utilisateur) — NÉGATIF
Les 7 lettres manquantes de la suite alphabétique de la table = KRYPTOS (rangs 11,15,16,18,19,20,25).
Batterie testée comme clé/masque de K4 (produit-elle les cribs ?) : KRYPTOS répété, rangs comme
décalages (0/1-indexés), différences 4-1-2-1-1-5, Morse des nombres→bits→lettres 5 bits, Morse de
KRYPTOS→lettres, longueurs/bits Morse comme décalages, XOR 5 bits (« changer la base »). ×
Vig/Beau/Var/XOR × alphabets AZ & KRYPTOS × tous décalages. **Meilleur = 4/24 (hasard 3,6 moy,
max 7) → bruit. NÉGATIF.** Aucune clé dérivée simple (Morse/binaire/rangs) ne reproduit K4.

## Addendum 11 — Clés auto-référentielles du site + clairs K1-3 + erreurs (idée utilisateur) — NÉGATIF
Hypothèse (indice Sanborn « K1-3 liés à K4 ») : la clé de flux de K4 vient de la sculpture
elle-même. Protocole : Vigenère (VIG/BEA/VAR × alphabets AZ & KRYPTOS × tous décalages) +
autoclé écart-7 (flux clair et chiffré). **Contrôle positif validé** : K1/PALIMPSEST →
alphabet KRYPTOS, Vigenère, 100 %.
- **Mots gravés (site + Morse + clair K1-3)** : 104 candidats (BETWEEN, SHADING, IQLUSION,
  INVISIBLE, UNDERGRUUND, LANGLEY, WW, WEBSTER, SLOWLY, DESPARATLY, CANYOUSEEANYTHING,
  SHADOWFORCES, LUCIDMEMORY, VIRTUALLYINVISIBLE, TISYOURPOSITION, BERLINCLOCK,
  EASTNORTHEAST, KRYPTOS…). Meilleur = **5/24**.
- **A — clairs ENTIERS K1/K2/K3 comme flux long** (direct ET inversé, + concaténations).
  Meilleur = **5/24**.
- **B — « erreurs » de Sanborn** (IQLUSION, UNDERGRUUND, DESPARATLY, lettres fautives Q/U/A,
  corrections L/O/E, « ? »). Meilleur = **4/24**.
- **Null familial** (max sur 7000 essais aléatoires, répété) : moyenne des max = **5,7/24**,
  95e centile 7. Nos meilleurs (4-5/24) sont **SOUS le plancher du hasard**.
- **Verdict** : NÉGATIF. Aucune clé tirée du site, du clair de K1-3, ou des erreurs — ni en
  Vigenère ni en autoclé écart-7 — ne reproduit les cribs. Ne réfute PAS un lien K1-3↔K4 qui
  passerait par une étape de masquage/transposition (cas indécidable base 09 §6, hors portée
  des 24 cribs). Scripts : scratchpad site_keys_test.py, AB_test.py.

## Addendum 12 — Clé géométrique 1:1-préservée (idée utilisateur « la forme = la clé ») — NÉGATIF
Rappel de contrainte : Sanborn confirme le **1:1** (BERLINCLOCK ↔ NYPVTTMZFPK, positions
64-74 de l'ordre linéaire publié). Donc une **transposition qui réordonne le chiffré est
exclue** (elle casserait ce repère). Version testée, compatible 1:1 : les lettres restent en
place, mais le **décalage** de chaque position vient de sa **géométrie physique** (mapping
propre des 97 lettres K4 → cellules : fin rangée 24 + rangées 25/26/27, 0/97 manquante).
- Familles : col, row, row+col (= cellule du tableau à la position), row-col, 2·col,
  rang d'arc (cylindre), rang de hauteur z, arc+z, lettre-tableau@(row+col). × VIG/BEA/VAR ×
  AZ & KRYPTOS.
- Résultat : **meilleur = 3/24**. Null (keystream aléatoire/position) : moyenne 0,92, **max 6**,
  P(≥6)=0,0004. Le meilleur géométrique est **sous** ce que le hasard produit.
- **Verdict** : NÉGATIF. La disposition physique (colonne, rangée, arc, hauteur) ne fournit pas
  le flux de clé. « La forme = la clé » réfutée sous forme 1:1-préservée. Script : scratchpad
  geom_key_test.py. (Transposition pure du chiffré : exclue par le 1:1, non testée à dessein.)
