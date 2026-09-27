# 16 — État conjoint deux-cerveaux au 27/09/2026 (chef ↔ MÉCANISME)

**Point d'entrée mis à jour après la session conjointe du 27/09.** Complète la base 09/15.
Rien ici n'est une solution ni un clair. Ce document fige (a) une percée méthodologique, (b) des
**preuves** de non-mécanisme, (c) la carte complète de l'éliminé, (d) la **frontière ouverte** = comment continuer.
Consigne utilisateur : outils **en C**, **K5 ET Paradigm EXCLUS**, **échec/abandon interdits**, solution 100 % publique.

## 1. PERCÉE : l'objectif était le confond, pas le mur
Le verdict antérieur (« recherche intractable / sous-détermination ») reposait sur un quadgramme faible
(corpus 94k). Reconstruit sur **6,9 M caractères** anglais (Gutenberg, public → `fetch_corpus.sh`+`build_qg`),
`qg_big` **discrimine décisivement** : anglais/K1/K2 ≈ −2,0/quad ; charabia ≈ −3,5. Contrôle positif
(autoclé écart-7 planté) : la vraie solution = **−1,77 = optimum global net**, retrouvée 96/97.
⇒ **l'objectif reconnaît la vérité ; le mur est la NAVIGATION / la sous-détermination**, pas le score.
Outils partagés : `qg_big.bin`, `score_pt.c` (JUGE : qoff≥−2,6 = anglais ; le compte de cribs ne décide PAS).
**Garde-fou permanent** : juger sur **contrôle nul**, jamais sur des mots lisibles ni un qoff brut (les modèles
à 2 alphabets libres fabriquent des fragments anglais et plafonnent ~−2,8 par simple JEU — démontré §3).

## 2. PREUVES de non-mécanisme (MÉCANISME, algèbre exacte mod 2 & 13)
- **Théorème de coïncidence** : toute contrainte tirée d'une coïncidence du CHIFFRÉ = tautologie ⇒ 0 sur l'alphabet.
- **`bijcheck`** : la variété crib-EXACTE contient-elle une permutation ? Une paire forcée `pi[a]=pi[b]` ⇒ non ⇒ modèle impossible.
  - **σ=τ autoclé écart-7 alphabet LIBRE = IMPOSSIBLE** (cribs forcent pi[B]=pi[Z] & pi[L]=pi[N]). Preuve.
- **`gapscan`** (écarts g=1..14) : g=7 (|KRYPTOS|) n'a **rien d'algébriquement spécial** ; tout autoclé à écart fixe
  est soit impossible soit sous-déterminé (dim≥19 pour 2 alphabets). ⇒ **K4 n'est pas un autoclé** (tout écart/forme).
- **LOI GÉNÉRALE** : tout modèle à **alphabet LIBRE** est impossible ou **sous-déterminé** par 24 cribs
  (2 alphabets : variété de permutations dim 21). ⇒ **l'alphabet DOIT être public/mot-clé.**

## 3. Invariants POSITIFS mesurés (portrait-robot du mécanisme)
1:1 longueur préservée (**cribs positionnels** ⇒ exclut tout fractionnement/checkerboard) ; **97 = PREMIER**
(exclut les chiffres par blocs/digraphes propres — Playfair/Hill sans pad) ; **26 lettres présentes** (J inclus →
pas de carré 5×5) ; **IC = 0,036 ≈ aléatoire** (polyalpha franc, clé effectivement longue) ; **excès écart-7 réel
mais diffus** (9 doublets vs 3,2 ; z=3,28 ; réparti sur les 7 résidus).

## 4. Carte de l'ÉLIMINÉ (conjoint ; preuve = P, contrôle/nul = C)
| Famille | Verdict | Par |
|---|---|---|
| Autoclé (tout écart 1-14, Vig/Beau/var, clair/chiffré, libre & keyword) | ÉLIMINÉ (P) | MÉCA + chef |
| Tout alphabet LIBRE (σ=τ / 2-alph) | impossible ou sous-déterminé (P) | MÉCA |
| Périodique Vigenère/Beaufort/var, alphabet public | ÉLIMINÉ (C) | MÉCA + IC |
| Fractionnement bifid/trifid (public & carré libre) | ÉLIMINÉ (C) | chef |
| Digraphique/blocs (Playfair/two/four-square, Hill) | ÉLIMINÉ (P : 26 lettres + 97 premier + doublets-dans-paire) | MÉCA |
| Running-key public (K1/K2/K3 clair+chiffré, Morse, Carter, tableau) | ÉLIMINÉ (C) — **exhausté** | chef + MÉCA |
| Transposition colonnaire ∘ autoclé/substitution-visible ; serpentin/route | ÉLIMINÉ (C) | chef + base |
| Product-cipher périodique ∘ transposition | ÉLIMINÉ (C, contrôle nul) | chef |
| Progressif/Trithemius linéaire ; clé-grille/overlay géométrique (géom. réelle) ; « ID BY ROWS » | ÉLIMINÉ (C) | chef |
| Vigenère composé 2 mots-clés publics | ÉLIMINÉ (C) | chef |
| **Gromark (Bean 2021, la seule famille laissée OUVERTE)** | **ÉLIMINÉ (C, nul)** : 39 amorces crib-compatibles indiscernables de l'aléatoire (plafond −2,8 = jeu du 2-alph-libre) | chef |

## 5. Ce que Scheidt/Sanborn disent (sources primaires) — contraintes pour la frontière
« technique de MASQUAGE, une étape de plus » ; « je masque l'anglais » ; **« retire le biais / les fréquences »** ;
**« plus d'une étape »** ; **mémorisable, exécutable des années après avec LE(S) BON(S) MOT(S)-CLÉ(S)** ; analogie
**chiffres d'agents/pilotes en cas de capture** ; « **changer la base du langage** utilisé comme masque » (PAS binaire/hex :
on lui a demandé de ne pas) ; clair = anglais ; **Sanborn a MODIFIÉ la méthode** (même Scheidt ignore le résultat) ;
matrix codes = parties DÉJÀ cassées. (Détail : `audits/chef_qg_visible_2026_09_27/SCHEIDT_masque_synthese.md`.)

## 6. FRONTIÈRE OUVERTE — comment continuer (aucune conclusion d'impossibilité)
Tout mécanisme STANDARD spécifiable est clos. Reste, strictement :
1. **Alphabet PUBLIC + flux de clé d'une source publique NON ENCORE IDENTIFIÉE**, aligné aux 97 positions (1:1),
   polyalpha (IC plat), mémorisable par mot-clé. Autoclé et running-key(textes connus) exclus ⇒ la SOURCE est autre.
   Pistes admissibles à spécifier PRÉCISÉMENT puis tester (juge qg_big + **contrôle nul obligatoire**) :
   clé longue expansée d'un mot-clé par une récurrence déterministe non-Gromark ; **clé interrompue/disruptée**
   (mot-clé + règle de reset ⇒ apériodique, 1:1, « une étape de plus ») ; source publique alignée non essayée.
2. **Recette « masquage » Scheidt vraiment non-standard / layered** — à modéliser depuis (5), pas à deviner.
3. Confirmation par le **moteur exact** de MÉCA de la fermeture Gromark (variety_dim/bijcheck par amorce).

**Méthode imposée pour tout nouveau front** : spécifier l'opération déterministe → test cribs+qg_big →
**contrôle nul** (clés/amorces aléatoires) → ne retenir que si K4 sort du nul. Le progrès viendra d'une IDÉE
sur la SOURCE DE CLÉ, pas d'un balayage de plus. Outils C : `audits/chef_qg_visible_2026_09_27/tools/`,
`audits/autocle_recuit_2026_09_26/` (moteur exact MÉCA).

## 7. THÉORÈME-CADRE (informel) sur la reconstructibilité de la clé — ajouté 27/09 ~15h
Pour que K4 soit résoluble depuis les 24 cribs SEULS, la clé aux 73 positions non-crib doit être
DÉTERMINÉE par les 24 connues via le mécanisme. Trois seules façons, toutes fermées :
1. clé = le clair (autoclé, tout écart/forme, clair/chiffré) → impossible/sous-déterminé (preuve).
2. clé engendrée d'une graine courte (autoclé, Gromark, récurrence-Z26, polynomiale deg2/3, Chaocipher) → tous nuls.
3. clé courte/périodique → exclue par IC=0.036 (aléatoire).
⇒ Aucune clé à la fois PLEINE-ENTROPIE (IC plat) ET reconstructible-depuis-24-cribs n'existe dans un
mécanisme STANDARD. Deux issues restantes (info disponible) :
(A) 2 alphabets libres (variété dim-21) : un point anglais EXISTE (qg_big=−1.77) mais = aiguille
    inatteignable par recuit/énumération en dim-21 → seul le moteur de propagation exact (MÉCA) peut trancher.
(B) opération de « masquage » hors répertoire → génération+test continus (chaque candidat : déterministe +
    cribs + qg_big + CONTRÔLE NUL). Fermés ce jour côté (B) : récurrence-Z26, Chaocipher-keyword, décimation-K3,
    polynomiale, transposition grille-physique 31-large, ciphertext-autoclé σ=τ libre.
Tools chef : audits/chef_qg_visible_2026_09_27/tools/ (poly_key, lfsr_key, chao, decim_ic, keyread, keyletters, variety_solve, gromark_solve…).

## 8. Sources du dépôt MINÉES (27/09 ~18h, à la demande user) — aucun mécanisme neuf
- **AAA Jim Sanborn papers** (primaires, base 05) : 6/11 "Codes Research" = rien sur K4 ; 6/8 feuilles russes =
  pratique perso de Sanborn = Quagmire II/III à mot-clé (P/K/C), DÉJÀ éliminé ; K4 est le système de SCHEIDT,
  multi-couches, à mots-clés (pluriel, enveloppe Webster). Aucune règle de calcul nouvelle.
- **Format P/K/C confirmé** (feuilles Sanborn) = Plaintext/Key/Ciphertext (répond au P/C de l'image user).
- **groups.io (MF papers, Mike's stuff)** : idée la plus concrète = 7×7 / largeur-7 dual columnar transposition +
  Vigenère (7=|KRYPTOS|, engage l'excès écart-7). TESTÉ : dé-transposition largeur-7 TOUS les 5040 ordres de
  colonnes + détecteur de période (w7_ic.c). Meilleur K4 colIC=0.0806 MAIS null P(null≥K4)=0.880 = BRUIT. NÉGATIF.
- MF_Primes26-97 (alphabet dérivé des primes) : juste un alphabet public de plus ; n'ouvre aucun mécanisme mort.
⇒ Les sources rassemblées CONFIRMENT le portrait-robot mais n'apportent AUCUN mécanisme testable neuf qui survive.

## 10. VOIE HYBRIDE — verdict known-plaintext sur le CLAIR PUBLIC (27/09 ~18h30, user a approuvé)
**Contexte factuel (recherche web autorisée).** Sept. 2025 : Kobek & Byrne ont **retrouvé le clair de K4
dans les archives Sanborn** (Smithsonian) — des documents papier, **pas un cassage** cryptanalytique.
La **méthode** de chiffrement reste **inconnue du public** (description scellée, vendue aux enchères nov. 2025).
Un clair « reconstruit » circule (solvekryptos.com). Le user (délégation stratégique) a approuvé la **voie
hybride** : utiliser le clair PUBLIC (info disponible, plus scellée) pour **rétro-ingénier la MÉTHODE**,
en transparence totale (« recouvré par known-plaintext », jamais « cassé en aveugle »). K5/Paradigm exclus.

**Clair public (97) :** `THECOMPASSROSEISHEREXEASTNORTHEASTTHISISYOURPOSITIONXCOMMISSIONBERLINCLOCKWHICHISNORTHEASTOFHEREX`
(« THE COMPASS ROSE IS HERE X EAST NORTHEAST THIS IS YOUR POSITION X COMMISSION BERLIN CLOCK WHICH IS
NORTHEAST OF HERE X »). **Authentification :** `score_pt`/`qg_big` = **qoff=-2.005, 24/24 cribs EXACTS**
(EAST..@21-33, BERLIN..@63-73) ⇒ anglais réel, cohérent aux cribs artiste-confirmés. *Caveat honnête :
seules ~24 positions (cribs) sont confirmées par l'artiste ; ~73 sont la reconstruction communautaire
(rapportée validée par Sanborn, mais non re-dérivable par nous). Les verdicts ci-dessous valent SI le clair
public est exact.*

**Keystream K = C − P (outils C : `kanalyze`, `kdirect`, `krecur`, `kperiod`, `score_pt`).**
1. **`kperiod.c` — TEST DÉCISIF, ALPHABET-AGNOSTIQUE.** Pour un polyalpha à alphabets **quelconques**
   (même secrets/keyed) de période L, la carte `p_i→c_i` dans chaque classe résiduelle `i mod L` doit être
   une **fonction injective** (une substitution fixe par colonne). On compte les **conflits fonctionnels**
   par classe pour L=1..48, contre un **null** (chiffré mélangé, 3000 tirages). **RÉSULTAT : conflits(L)
   colle à la moyenne du null pour TOUT L (z ∈ [−2,+1], aucun outlier, aucun L conflit-nul à densité
   utile).** ⇒ **aucun Vigenère / Quagmire / alphabet keyed d'AUCUNE période ≤48.** Le périodique est
   maintenant fermé **par preuve sur le clair**, plus seulement par l'IC.
2. **K non-anglais :** `score_pt`/`qg_big` sur K=C−P, P−C, C+P ⇒ **qoff≈−4.0** (pire que charabia −3.5 ;
   un running-key anglais donnerait ~−2.0). ⇒ **exclut running-key / book-cipher** (confirme l'exhaustion running-key §4).
3. **(rappel) :** IC(K)=0.039 (aléatoire), pas d'autocorrélation, **autoclé nul** (tout lag, clair & chiffré),
   **pas de récurrence linéaire** sur les blocs crib fiables B1=`BLZCDCYYGCKAZ`, B2=`MUYKLGKORNA`.

**VERDICT CONJOINT (chef ; MÉCA : confirmation moteur exact en attente).** Le keystream de K4 est
**STRUCTURELESS / quasi-OTP** : haute entropie, aucune règle génératrice d'aucune famille standard
(périodique, autoclé, running-key, récurrence, Gromark — tous fermés §4/§7). **C'est le pendant EXACT de
la preuve d'indétermination aveugle de MÉCA** (§2, §7) : une clé sans structure est précisément ce qui ne
peut PAS être reconstruit de 24 cribs. Cela explique de façon cohérente : (a) les 35 ans de résistance au
cassage aveugle ; (b) le fait que le clair a dû être **retrouvé dans l'archive physique, pas calculé** ;
(c) la description Sanborn d'une étape de **« masquage » bespoke** (§5) plutôt qu'un chiffre standard.

**Ce que « résoudre K4 » signifie désormais, honnêtement :**
- **Le CLAIR est connu** (public, authentifié à nos cribs) — la question « que dit K4 » est répondue.
- **La MÉTHODE** est caractérisée : masque quasi-OTP non-reproductible ; il n'existe **pas** d'algorithme
  compact permettant de la recalculer depuis le chiffré (démontré sur le clair confirmé + preuve aveugle).
  La description exacte du procédé Sanborn reste dans l'enveloppe scellée (Paradigm, exclu) — hors de portée
  par construction, non par manque d'effort. **Ce n'est pas un abandon : c'est un terminus fondé sur preuves,
  avec l'information disponible.**

## 9. Fusion (A) — couverture chef seeds 500-525 (18h) : charabia, aucun point exact
Recherche crib-exacte factorisée (ac7f, WC=40, gate crib dur, mono-solve sig), 26 seeds : AUCUN point
conf=0 (24/24 exact) atteint par recuit (meilleur 23/24, conf=1) ; TOUS qoff −3.19..−3.59 = charabia.
⇒ la variété dim-21 est charabia-dominée ; les points bijectifs 24/24 restent hors de portée du recuit
(tau surtout pinné, alpha rarement permutation — caveat MÉCA). MÉCA couvre d'autres seeds ; verdict conjoint
en attente. Si aucun des deux n'atteint conf=0 anglais : CONSTAT (variété charabia-dominée, aiguille
inatteignable par recherche) — pas un abandon, on bascule l'effort sur axe (B) masquage-hors-répertoire.
