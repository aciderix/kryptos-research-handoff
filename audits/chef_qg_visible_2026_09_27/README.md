# Chef solo — 2026-09-27 (après-midi) : le vrai verrou = RECHERCHE, pas objectif ; features visibles fermées

Consigne utilisateur inchangée : deux cerveaux, outils **en C**, **ne jamais s'arrêter**,
**K5 ET Paradigm EXCLUS** (solution 100 % publique/visible), **échec/abandon interdits**.
MÉCA en pause jusqu'à ~14h ; cette session reprend ses tâches + les siennes en autonomie.

## 0. Résultat majeur : l'« intractabilité » était en partie un ARTEFACT du quadgramme
Le verdict antérieur (base 13 / audits autocle : « la vraie solution est reconnaissable mais
introuvable ⇒ sous-détermination ») reposait sur un quadgramme bâti sur un corpus de **94k**
caractères (build_qg + corpus repo). Ce modèle **ne discrimine pas** l'anglais du charabia :

| texte | qg/quad (corpus 94k) | qg/quad (corpus **6,9M**, `qg_big`) |
|---|---|---|
| anglais courant | -2.79 | **-1.99** |
| clair K1 | -1.91 | **-2.12** |
| clair K2 | -1.59 | **-1.95** |
| charabia | -3.40 | **-3.50** |
| K4 chiffré | -3.32 | **-3.69** |

Corpus 94k : séparation anglais/charabia ≈ **0.6** (inexploitable). Corpus 6,9M (9 livres
Gutenberg, domaine public — `fetch_corpus.sh`) : séparation ≈ **1.5/quad = DÉCISIVE**.

### Conséquence prouvée : l'OBJECTIF est parfait, seule la RECHERCHE bloque
Contrôle positif (clair anglais réel, planté en autoclé-Vig écart-7, 2 alphabets, cribs insérés) :
- avec `qg_big`, la VRAIE solution score **qoff -1.77** = optimum global net (charabia ≈ -3.3).
- une instance « facile » est **retrouvée 96/97** lettres par le recuit ac7 en 11 s.
⇒ Le mur n'est PAS l'objectif ni « la solution n'est pas reconnaissable ». **Le mur est la
NAVIGATION** dans l'espace 26!×26!×26⁷.

### Mais la recherche 2-alphabets reste un vrai mur DIMENSIONNEL (confirme MÉCA autrement)
Instances de contrôle « dures » NON retrouvées même à gros budget :
- ac7 recuit joint : 34/97 à **450 M itérations** (3M×150).
- recherche **factorisée** (ac7f : mono-solve interne + cribs soft, échappe à la ruggosité) :
  35/97 à 25000×100. Mieux structuré, mais dim-18 reste hors de portée d'un hill-climb.
⇒ cohérent avec `variety_dim` (MÉCA : 2 alphabets ⇒ ~16 lettres à fixer). Le cas 2-alphabets
libres exige la **réduction algébrique des cribs** (moteur MÉCA) FUSIONNÉE avec `qg_big`.
C'est le point de synthèse à 14h. **On ne conclut pas à l'impossibilité** : objectif validé,
il « suffit » d'une recherche qui exploite la structure (chaînes/cribs) — voir §2.

## 1. Fractionnement (« masque de Scheidt » = bifid/trifid) — FERMÉ, décisif
Échappatoire théorique réelle : le théorème de coïncidence de MÉCA ferme le chiffré FINAL sous
substitution+autoclé, mais PAS une couche de fractionnement qui mélange les coordonnées AVANT
lecture. Testé proprement avec `qg_big` :
- **alphabet PUBLIC keyé** (0 lettre libre) bifid+trifid, périodes 2-40, 4 carrés / 3 cubes :
  best **3/24** cribs (`fractionate.c`). Mort.
- **carré LIBRE** (recuit, objectif combiné cribs+qg_big, `bifid_crib2.c`) : plafonne à **17/24**
  cribs et **TOUT** déchiffré est charabia (qoff ≈ -3.5, jamais -2.0). Sur-contraint ⇒ aucun
  carré cohérent. Mort. ⇒ hypothèse bifid/trifid ENTERRÉE (public ET libre).

## 2. Feature visible → alphabet : test DÉTERMINISTE (fiable, conclusif) — TOUT NÉGATIF
Point clé : si σ (ou σ ET τ) est FIXÉ à un alphabet keyé d'un mot VISIBLE, l'autoclé écart-7 se
réduit à **7 amorces** (σ=τ) ou aux amorces seules (2 alphabets fixes). À lag 7 le clair se
scinde en **7 chaînes** (position mod 7) et **chaque chaîne contient des cribs** ⇒ chaque amorce
est **pinée déterministe­ment** par ses cribs, avec **cohérence interne = validation** (`quag2.c`).
Validé : contrôle σ=KRYPTOS,τ=PALIMPSEST ⇒ **24/24 cribs, qoff -1.85, clair exact**. Fiable.

Balayages (tous avec `qg_big`, sur K4 réel) :
| front | espace testé | meilleur | verdict |
|---|---|---|---|
| σ=τ, feature visible | 14-34 mots × formes{Vig,Beaufort,var} × src{clair,chiffré} × lag 7 | qoff -3.13, 6/24 | négatif |
| σ=τ, autres lags | idem × lags {5,6,8,9,10,11,13,14} | qoff -3.08, 2/24 | négatif |
| **Quagmire 2 alphabets visibles** | σ×τ, 21×21 paires de mots, autoclé écart-7 | qoff -3.51, 8/24 (16 conflits) | négatif |

Mots testés (matériau VISIBLE) : KRYPTOS, PALIMPSEST, ABSCISSA, IQLUSION, UNDERGROUND,
VIRTUALLYINVISIBLE, SHADOWFORCES, LUCIDMEMORY, TISYOURPOSITION, DIGETALINTERPRETATU, SHADOW,
LUCID, MEMORY, POSITION, INVISIBLE, WELTZEITUHR, BERLIN(CLOCK), SANBORN, SCHEIDT, LANGLEY,
WEBSTER, NORTHEAST, EASTNORTHEAST, MAGNETIC, EARTH, FIELD, NUANCE, SHADING, WHOKNOWS,
CANYOUSEEANYTHING, A-Z, + combinaisons.
⇒ **L'alphabet de K4 n'est PAS un simple mot-clé visible sous autoclé** (σ=τ NI 2 alphabets).
Redirige : soit alphabet non-keyword (recherche 2-alph libre §0), soit mécanisme non-autoclé.

## Outils (dossier tools/, tous en C)
- `build_qg.c` + `fetch_corpus.sh` : reconstruisent `qg_big.bin` (456976 floats) — corpus 6,9M
  Gutenberg (public). `qg_big.bin` NON committé (dérivable, 1,8 Mo). `qgtest.c` : discrimination.
- `fractionate.c`, `bifid_crib2.c` : fronts bifid/trifid (§1).
- `ac7f.c` : recherche factorisée 2-alphabets (mono-solve interne + cribs soft).
- `ac7g.c` : autoclé σ=τ multi-formes (Vig/Beaufort/var) × src{clair,chiffré} × lag ; mode
  FIXSIGMA (feature visible). `quag2.c` : test déterministe 2 alphabets fixes (§2).
- `keyalpha.c` (mot→alphabet keyé style KRYPTOS), `enc2.c` (encodeur de contrôle).

## Prochaine étape (synthèse 14h avec MÉCA)
FUSION : réduction algébrique des cribs (moteur/matrices MÉCA — ramène le cas 2-alphabets à ses
~18 dof libres réels, garantit la cohérence-cribs) **+ objectif `qg_big`** (désormais décisif).
C'est la seule voie où objectif validé + recherche structurée peuvent converger. Fronts visibles
keyword + fractionnement = FERMÉS. **Aucune conclusion d'impossibilité.**

## 3. Recherche STRUCTURÉE 2-alphabets par équations de cribs (ac7s.c) — sous-détermination reproduite
Levier : positions double-crib (p_i ET p_{i-7} connus) ⇒ 8 lettres de tau DÉRIVÉES de sigma
(R:(T,E) N:(H,A) G:(E,S) K:(A,T) S:(S,N) Z:(L,B) F:(O,E) P:(C,R)) + 2 contraintes de cohérence
sigma (sig[S]+sig[N]=sig[T]+sig[O] ; sig[A]+sig[T]=sig[K]+sig[I]). Auto-satisfait 10 cribs,
réduit tau à 18 libres. Recuit sig+tau_libre+kappa scoré qg_big.
Résultat K4 (5 graines, 500k×40) : atteint **consist=0, taucol=0 (tau perm valide), 20-22/24
cribs** MAIS **qoff -3.25 à -3.39 = charabia** (jamais l'anglais -2.0). ⇒ le sous-espace
crib-cohérent est ATTEIGNABLE et contient massivement du charabia : **sous-détermination réelle**
du modèle 2-alph Vig autoclé écart-7 par (public + 24 cribs), reproduite avec recherche structurée
+ objectif décisif. Trancher « pas de solution anglaise dans ce modèle » vs « aiguille manquée »
exige l'ÉNUMÉRATION algébrique de la variété crib-cohérente scorée par qg_big (moteur MÉCA + qg_big).
NB : PIN est spécifique au chiffré K4 (non validable sur contrôle générique) ; conclusion prudente.

## 4. GARDE-FOU anti-pareidolia (wordfrag.c) — les « mots lisibles » sont des ARTEFACTS
Les sorties de recuit contiennent des mots anglais ("YOU WILL", "VERY", "PAST GOT", + cribs).
Démonstration qu'ils ne sont PAS du signal :
- le MÊME optimiseur (σ=τ, cribs forcés) sur des chiffrés ALÉATOIRES (sans message) produit les
  mêmes fragments (cribs forcés + mots incidents type SPEAK/SHOT). Couverture-mots aléatoire = 3-9%.
- couverture-mots : aléatoire pur 0-9% ; sorties K4 en échec 13-34% (gonflée par l'optimiseur qui
  MAXIMISE l'anglais + FORCE les cribs) ; vraie solution 42% ET qoff -1.77 ET lisible en continu.
- "LIZY" = fuite du corpus (Austen, "Lizzy" fréquent) : biais du scoreur, pas un mot de K4.
RÈGLE : ne jamais juger sur des mots aperçus. Solution « lue » SEULEMENT si qoff→~-2.0 ET prose
continue ET cribs satisfaits SANS forçage. Aucune sortie K4 n'a franchi ce seuil.

## 5. Running-key auto-référentiel (runkey.c) — FERMÉ
Hypothèse artiste : K4 se déchiffre avec le PROPRE texte résolu de l'œuvre (K1/K2/K3), 100% public.
Balayage déterministe : clés {K1K2K3, K2, K3, K2K3, K3K2, K2rev, K3rev} × σ {KRYPTOS-keyé, A-Z} ×
formes {Vig, Beaufort, variante} × TOUS offsets d'alignement. Score qg_big + cribs.
Résultat : aucun combo n'atteint 12/24 cribs ni qoff > -2.9. **Négatif total.** La clé courante
n'est pas le texte résolu de l'œuvre.

## 6. Transposition ∘ substitution (transpo.c + quag2) — FERMÉ
Hypothèse artiste (K4 = « différent/plus dur » : fusionne K3=transposition et K1/K2=substitution) :
clair -(autoclé écart-7)-> intermédiaire -(transposition colonnaire)-> K4. On dé-transpose K4
puis test DÉTERMINISTE (quag2) de l'autoclé à alphabets visibles fixes.
Balayage : largeur W=2..24 × ordre colonnes {id, rev} × direction {inverse, avant} ×
  - 6 paires d'alphabets (KRYPTOS/PALIMPSEST/ABSCISSA/A-Z, style Quagmire 2 alphabets) ;
  - 15 mots-clés visibles en σ=τ.
Résultat : **AUCUN hit** (meilleur qoff -3.51, 10/24 cribs, conflits). ⇒ K4 n'est pas une
transposition colonnaire d'un autoclé à alphabet visible. (La transposition ∘ alphabet LIBRE
resterait un grand espace de recherche = même mur dimensionnel que §0/§3, non déterministe.)

## 7. JUGE PARTAGÉ (score_pt.c) — interface pour la fusion 14h
`score_pt qg_big.bin <clair97>` → `qoff cribs verdict`. Seuil : anglais réel qoff>=-2.6 (typ -2.0),
charabia <=-3.2. Auto-test : vrai clair -1.79/24-24 = CANDIDAT ; charabia -3.14 (même à 21/24 cribs
forcés) = charabia. **Le compte de cribs ne décide PAS ; qoff décide.** MÉCA : ton énumérateur de
la variété crib-cohérente peut piper ses candidats clairs dans ce juge pour trouver l'aiguille anglaise.

## 8. Morse running-key + CARTE DU SOUS-ESPACE RESTANT (synthèse conjointe chef+MÉCA, ~13h)
Morse d'entrée comme clé courante (3 ordres × {KRYPTOS,A-Z} × Vig/Beau/var, tous offsets) : négatif (qoff -3.66).

### Verdict combiné (preuves MÉCA + fermetures déterministes chef)
- AUTOCLÉ (tout écart g=1..14, Vig/Beau/var, plaintext/ciphertext) : **ÉLIMINÉ**.
  - alphabet libre σ=τ : IMPOSSIBLE (cribs forcent pi[B]=pi[Z] & pi[L]=pi[N], bijcheck.c).
  - alphabet libre 2-alph : SOUS-DÉTERMINÉ (variété perm dim 21) — ne peut identifier un clair.
  - g=7 n'a rien de spécial (gapscan.c).
  - alphabet keyword visible (σ=τ + Quagmire 2-alph) : négatif déterministe (chef).
- FRACTIONNEMENT bifid/trifid (public + libre) : ÉLIMINÉ (chef).
- TRANSPOSITION colonnaire ∘ autoclé-alphabet-visible ; serpentin/route (base) : ÉLIMINÉ.
- RUNNING-KEY public (K1/K2/K3, Morse, Carter[base]) : négatif.
- PÉRIODIQUE (Vigenère/Quagmire à clé) : exclu par l'IC plat de K4 à toute période (base).

### LOI (MÉCA) : tout modèle à ALPHABET LIBRE est impossible ou sous-déterminé par 24 cribs.
⇒ **l'alphabet DOIT être public/keyword.** Or presque tout mécanisme à alphabet public est fermé ci-dessus.

### CONTRAINTE STRUCTURELLE FORTE : 97 est PREMIER.
⇒ aucun chiffre par BLOCS ou DIGRAPHES ne pave 97 proprement : Hill (n≥2), Playfair/two/four-square
(digraphes = longueur paire), bifid/trifid seriated, ADFGVX (double la longueur). Tous exigent
padding/null/omission d'1 lettre. Signature à vérifier (MÉCA) : K4 omet-il exactement une lettre (carré 25) ?

### CE QUI RESTE (box très étroite : public + non-autoclé + non-périodique + compatible 97 premier + IC plat)
1. Chiffre digraphique avec pad/null d'1 lettre (Playfair/two-square) — MÉCA en cours ; à réconcilier avec 97 impair.
2. « Masquage » de Scheidt NON-standard / LAYERED (combinaison), pas un chiffre de manuel — à modéliser depuis sources primaires (interviews Scheidt/Sanborn).
3. Progressive/Trithemius linéaire (shift=a·i+b, tout alphabet/forme) : TESTÉ déterministe = négatif (max 7/24, charabia). Reste: interrompu/masquage Scheidt non-standard, LAYERED.
⇒ Redirection nette (PAS abandon) : comprendre la technique de « masquage » réelle de Scheidt + combinaisons multi-couches.

## 9. Overlay géométrique 2-panneaux = clé-grille (overlay_key.c) — FERMÉ (lead géométrique de MÉCA)
Le seul lead géométrique non tranché (overlay tableau +4% cellule-à-cellule, doc RECHERCHE_geometrie
Test 2) se réduit à une CLÉ-GRILLE LINÉAIRE : shift_i=(A·row+B·col+C) mod 26 avec les coords (row,col)
physiques MESURÉES de K4 (extraites du modèle 3D, k4_rowcol.csv, reconstruit K4 exactement).
Subsume : overlay tabula-recta (A=B=1), « ID BY ROWS » (B=0), clé-colonne (A=0), diagonales, dérive +4%.
Balayage déterministe A,B∈[-4,4] × C∈[0,25] × σ{KRYPTOS,PALIMPSEST,ABSCISSA,A-Z} × formes{Vig,Beau,var}.
Résultat : max 7/24 cribs, tout charabia (qg_big). **NÉGATIF.** ⇒ overlay géométrique/clé-grille/ID-BY-ROWS
FERMÉS avec l'objectif décisif, sur la géométrie réelle. Cohérent avec RECHERCHE_geometrie (tout
appariement statique table↔chiffré = bruit). Le lead géométrique est clos.

## 10. Écart-7 réel mais diffus + « masquage = transposition sur périodique » FERMÉ (contrôle nul)
(a) **Excès écart-7 CONFIRMÉ** (gapstat.c) : K4 a 9 doublets chiffrés à l'écart 7 vs ~3.2 attendus
(z=3.28, p≈0.005 ; 7 = |KRYPTOS| pré-spécifié). AUCUN autre écart ne ressort (|z|<1.5). MAIS les
9 doublets sont DIFFUS (répartis sur les 7 résidus mod 7, tirés par O/K fréquents) ⇒ signal réel
mais non-actionnable seul, et l'autoclé-7 (mécanisme évident) est PROUVÉ impossible (MÉCA).
(b) **Hypothèse « masquage Scheidt = transposition masquant une périodicité polyalpha »** (product
cipher : Vigenère périodique ∘ transposition ⇒ IC global plat, période cachée) : testée par
détecteur de Friedman (ic_detect.c) — de-transposer puis chercher un IC-colonne réveillé.
Meilleur K4 = colIC 0.0769. **CONTRÔLE NUL (ic_null.c, 2000 shuffles, même pipeline complet)** :
mean_max=0.0742, P(null≥K4)=**0.275**. ⇒ le « signal » est du **bruit de tests-multiples sur texte
court** ; aucune transposition ne révèle de périodicité réelle. **Product-cipher périodique∘transposition
FERMÉ.** (Garde-fou anti-pareidolia appliqué : jugement sur le nul, pas sur le max brut.)
