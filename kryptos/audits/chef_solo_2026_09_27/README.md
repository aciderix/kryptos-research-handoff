# Session conjointe chef ↔ MÉCANISME + solo chef — 2026-09-27

Reprise à deux agents (Claude Agents Mesh). Consigne utilisateur : travailler de concert, **outils en C**, **ne jamais s'arrêter / échec & abandon INTERDITS**, **K5 ET Paradigm EXCLUS** (solution 100 % publique/visible). Aucune solution, aucun clair. Ce dossier consigne les attaques et verdicts pour ne pas les refaire.

## Théorème de clôture (MÉCANISME, rang exact mod 2 & 13)
Toute contrainte tirée d'une **coïncidence du chiffré** (écart-7, doublets, égalité de Bean) est une **tautologie** `τ(c_a)=τ(c_b)` ⇒ **zéro contrainte sur l'alphabet** (rang invariant). Elle ne donne que du **clair gratuit**. ⇒ la voie « structure interne publique → variété réduite → énumération » est FERMÉE. Le verrou-alphabet ne peut être réduit que par de l'information EXTERNE au chiffré.

## Clair connu ÉTENDU (byproduct, vrai dans toute solution sous autoclé-Vig écart-7)
`p8=A, p20=C, p39=N, p58=C(=p72), p83=C` ; égalités `p0=p14, p5=p19, p38=p52, p79=p93`. Utile pour VALIDER/lire une solution, pas pour la trouver.

## k_min mesuré (variety_dim, MÉCANISME)
- Deux alphabets indépendants : dim **21** → il faut fixer **~16** lettres d'alphabet pour énumérer.
- σ=τ (Quagmire III) : dim **9** → **~4-5** lettres suffiraient — MAIS la variété σ=τ ne contient que du **charabia** (base 13), donc sans issue même rendue énumérable.

## Vérification à fort levier (chef, atout unique = modèle 3D)
Relecture indépendante du chiffré K4 sur `figures/kryptos_cipher_panel_flattened.png` (HD 3D) :
`OBKR / UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO / TWTQSJQSSEKZZWATJKLUDIAWINFBNYP / VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR`
= **identique au K4 canonique**. ⇒ **aucune erreur de transcription** (hypothèse « erreur propagée 30 ans » écartée).
Idem tableau (`kryptos_tableau_panel_flattened.png`) = table STANDARD (KRYPTOS keyé + L en trop) → pas d'alphabet caché.

## Attaques menées cette session — TOUTES NÉGATIVES (contrôle positif + témoins)
| Attaque | Outil | Verdict |
|---|---|---|
| 19 alphabets « ordre spatial du chiffré » (dont freq) | visible_alphabets.c + visible_sweep.c | 12/24 = hasard |
| Tableau gravé (lecture HD) | image | standard, pas de source |
| Décimation ×m mod 97 (non-1:1) | decim_perms.c + decim_autokey.c | 10/24 = hasard |
| Alphabets keyés site K0/K1 | keyed_from_text.c | négatif |
| Alphabet ordre-Morse | crossbase | négatif |
| Objectif « mots-codés » (base 13 mal spécifié ?) | ac7.c + qg.bin | RÉFUTÉ (clair mots-codés=-4.78, reste identifiable → mur = RECHERCHE) |
| Recuit depuis germes structurés (Piste A) | autocle_recuit | charabia |
| σ=τ + écart-7 + doublets (Direction 2) | variety_dim + ac7 tie | dim inchangée (théorème), attaque = charabia |
| lag≠7 (5,6,8,9) structuré | s3lag.c | 0 partout |
| σ=τ alphabet connu déterministe (copie 32/73) | autocle_recuit | 14-17 err |
| **2-ERREURS** clé positionnelle alphabet connu (275 couples T5) | pos7_2err.c | 0/1656 ; libre = charabia. Trou comblé |
| **Grille-clé DIÉDRALE** tableau 26×26 (flip-the-chart, 1:1) | dihedral_key.c | 7/24 = hasard |
| **Grille-clé DIÉDRALE clairs K1/K2/K3** (running-key 1:1) | dihedral_plain.c (chef, solo) | 7/24 = hasard |
| Déviations 3D par lettre (misalignment) | devanalyze.c (chef) | artefacts (bord, courbure, bruit) — pas de clé |

## État (offensif, sans abandon)
Sous tout autoclé (tout lag, σ=τ/2 alphabets, structuré/libre) K4 n'est pas déchiffrable depuis public+24 cribs ; seul l'autoclé écart-7 produit le profil. Le manque quantifié = **~16 lettres d'alphabet (2 alphabets)** externes au chiffré. Fronts offensifs restants (repris à 14h avec MÉCANISME) : masque de Scheidt comme vraie 1ʳᵉ étape (fractionnement/recombinaison AVANT substitution) ; Morse F-10 (règle figée) ; transpositions physiques 3D non colonnaires ; toute feature visible fixant plusieurs lettres d'alphabet. **On ne conclut PAS à l'impossibilité.**

Outils C dans ce dossier. Branche chef : claude/stoic-faraday-e8oeoy. Branche MÉCANISME : claude/ecstatic-volta-cn51vq.

## Vérification indépendante (chef solo, 2026-09-27 ~09h20) — les 2 négatifs porteurs confirmés
1. **Contradiction σ=τ (base 9 §6) re-dérivée à la main** : pos 32 (S→S, clé p25=N) ⟹ σ⁻¹(N)=0 ; pos 73 (K→K, clé p66=L) ⟹ σ⁻¹(L)=0 ⟹ N et L à l'index 0 = contradiction exacte. Pas de bug.
2. **Recherche free-alphabet intractable, reproduite indépendamment** : j'ai bâti un scorer quadgramme backoff (corpus repo : Carter+Nova+clairs K1-K3, 94k chars, build_qg.c) et fait tourner ac7 (moteur MÉCANISME).
   - Contrôle positif INDEP (faux K4 anglais planté, 200k×40) : NON retrouvé (20/24 cribs, 33/97, charabia).
   - Contrôle positif TIE σ=τ (300k×60) : NON retrouvé (21/24, 29/97).
   - K4 réel INDEP (400k×80) : best 21/24 cribs, qoff −3.07, charabia (cribs formés par crib-soft, reste = bruit).
   ⇒ la vraie solution est reconnaissable mais introuvable par recherche ; confirmé sur 3 configs. base 13 juste.
Outils : build_qg.c (chef). qg.bin non committé (dérivable du corpus). Fronts NON-autoclé restent ouverts (masque-first, transpo 3D non-colonnaire, feature visible → 14h avec MÉCANISME).
