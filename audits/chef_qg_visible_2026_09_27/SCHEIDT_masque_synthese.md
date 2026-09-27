# Synthèse « masquage » de Scheidt depuis les sources PRIMAIRES (chef, 2026-09-27 ~13h30)

But : transformer le « masquage » vague en CONTRAINTES précises sur le mécanisme, pour cibler
la box restante (cf README §11). Sources = citations Scheidt/Sanborn du corpus (base 6/7).

## Contraintes DURES tirées des sources
1. **1:1 positionnel CONFIRMÉ** : les cribs sont donnés en POSITION (EASTNORTHEAST@21-33, BERLIN@64).
   ⇒ chaque position chiffré ↔ une position clair. **⇒ EXCLUT tout fractionnement** (straddling
   checkerboard/VIC, bifid… changent la longueur/alignement). Le « removes bias » N'EST donc PAS un
   checkerboard à codes de longueur variable. (Note : Sanborn a parfois relativisé le 1:1, mais les
   cribs positionnels le rétablissent.)
2. **« Retire le biais / les fréquences »** (Scheidt 2007, 2015) + **IC mesuré = 0,036 ≈ aléatoire**
   ⇒ clé EFFECTIVEMENT LONGUE (pas de période courte résiduelle). Un Vigenère à mot-clé COURT
   laisserait une période (Kasiski) ; ici l'IC est plat ⇒ clé ~ longueur du texte.
3. **« Plus d'une étape »** (2015) ; **« changer la base du langage… utilisé comme masque »** (2020) ;
   ordre du masque (avant/après substitution) **non dit** (« je diffère »).
4. **NON-binaire/hex** : Scheidt a envisagé base 2/16 mais **on lui a demandé de NE PAS** le faire.
5. **Hand-cipher, mémorisable, exécutable des années après avec LE(S) BON(S) MOT(S)-CLÉ(S)** (2011).
   Analogie : **chiffres d'agents/pilotes en cas de capture** (escape & evasion).
6. **Clair = anglais** ; **Sanborn a MODIFIÉ la méthode de Scheidt** (même Scheidt ignore le résultat).
7. **matrix codes = parties DÉJÀ cassées** (⇒ pas la piste Hill/matrice pour K4).

## Ce que ça implique (croisé avec nos preuves)
- 1:1 + IC≈aléatoire + retire-biais ⇒ **polyalphabétique à clé quasi-longueur-texte** (running-key-like),
  PAS un mot-clé court. Or : autoclé = PROUVÉ impossible (MÉCA) ; running-key des textes publics
  (K1/K2/K3 clair+chiffré, Morse, Carter, tableau) = TOUS négatifs (chef+MÉCA).
- ⇒ Si la clé est longue-comme-le-texte ET publique ET déterministe ET pas un de ces textes, la SOURCE
  de clé reste à identifier — c'est LE trou. Candidats « mémorisables par mot-clé » générant un flux long :
  (i) **clé étendue depuis un mot-clé** (Gromark/chaîne numérique, expansion type-Playfair, « quagmire
      progressif ») — flux long DÉTERMINÉ par un court mot-clé (colle à « mémorisable + retire biais »).
  (ii) **produit substitution ∘ transposition à mot-clé** (la transposition = le « masque » qui casse
      la période ⇒ Kasiski échoue, mais 1:1). Testé partiellement (colonnaire id/rev = nul) ; **manque :
      transposition à ORDRE-mot-clé (ex. colonnes ordonnées par KRYPTOS), largeur 7.**
- « changer la base du langage » : PAS binaire (contrainte 4). Reste : re-représenter l'anglais dans une
  autre **base symbolique publique** (ex. via le tableau/omot-clé) — à spécifier.

## Prochaines cibles PRÉCISES (admissibles pour le moteur de MÉCA)
A. **Clé longue expansée d'un mot-clé** (Gromark chain, ou mot-clé → suite pseudo par récurrence
   déterministe) appliquée en Vigenère/Beaufort, alphabet public. Mémorisable + IC plat + 1:1. → à coder.
B. **Vigenère(mot-clé) ∘ transposition-colonnaire-à-ordre-mot-clé** (largeurs 7/‹mot›, ordres = rang des
   lettres du mot-clé), détecteur de période après dé-transposition (déjà outillé, ic_detect) + solve qg_big.
C. Écarter définitivement le fractionnement (fait, contrainte 1).

## RÉSOLUTION Gromark (la seule famille laissée OUVERTE par la littérature — Bean 2021)
Gromark = meilleur fit Scheidt (amorce mémorisable → clé longue → retire biais → 1:1 → « plus d'une étape »).
Bean : 39 amorces (base10, 5 chiffres, 2 alphabets LIBRES) crib-compatibles, clairs « proches anglais mais
non convaincants » — c'est le CONFOND du scoring faible que qg_big lève.
TEST (chef, gromark_solve.c, qg_big) : recuit (sig,tv) par amorce. Contrôle positif (vrai primer) plafonne
à qoff -2.81 (dim-2-alphabets dur). Scan des 39 amorces sur K4 : meilleur qoff -2.74, fragments lisibles.
**CONTRÔLE NUL (15 amorces ALÉATOIRES non-crib-compatibles, même budget)** : meilleur qoff -2.84, moyenne -2.99
— **IDENTIQUE** aux 39 de Bean (moyenne -3.03, best -2.74). Les 39 ne gagnent que sur le COMPTE DE CRIBS
(compatibles par construction), PAS sur l'anglais. ⇒ le plafond ~-2.8 est le **plancher de jeu** du Gromark
2-alphabets-libres (sur-déterminé en liberté), pas du signal. **GROMARK FERMÉ** (résout Bean : sous-déterminé,
crib-compatibles indiscernables de l'aléatoire). Confirme empiriquement la LOI de MÉCA (2 alphabets libres = sous-déterminé).
Garde-fou : jugé sur le CONTRÔLE NUL, pas sur les fragments lisibles ni le qoff brut.

## Clé INTERROMPUE (disrupted Vigenère) — variante standard NÉGATIVE
Front frontière #1 (« une étape de plus », apériodique, mémorisable) : mot-clé + reset de l'index sur
lettre-déclencheur (interruptor) dans le CHIFFRÉ ; σ=KRYPTOS-keyé ; 20 mots-clés thématiques × 26 interruptors
× Vig/Beau/var (interrupted.c, déterministe fixe donc pas de jeu). Meilleur : 5/24 cribs, **qoff −4,06** (pire
que l'aléatoire). NÉGATIF net. (Variantes restantes non testées : interruptor sur le CLAIR, autres règles de
reset, autres alphabets — prior faible ; à ne rouvrir qu'avec une raison.)

## Outil de frontière : keyread.c (lecture directe du flux de clé)
Pour un alphabet PUBLIC fixé, les 24 cribs DÉTERMINENT k_i=σ(c_i)∓σ(p_i) aux 24 positions. Si la clé est
« simple/mémorisable » (Scheidt), ces 24 valeurs doivent montrer une structure. Pour σ=KRYPTOS (Vig/Beau/var) :
AUCUNE constance par résidu mod 7, aucune progression arithmétique ⇒ clé non-structurée sous cet alphabet
(cohérent avec périodique-public FERMÉ par MÉCA). **Usage frontière** : dès qu'une source de clé / un alphabet
public candidat est proposé, keyread lit la clé aux 24 positions et teste la structure AVANT tout balayage.

## Attaques info-DISPONIBLE (recadrage user : ne pas invoquer d'info externe)
- **2-alph autoclé écart-7, recherche LOURDE** (ac7s structuré double-crib, 12 graines × 400k×120, qg_big) :
  atteint 21-23/24 cribs consist=0 mais **qoff plafonne −3,13 (charabia)** à tout point crib-cohérent trouvé.
- **Variété crib-EXACTE** (variety_solve.c : système homogène Z26, noyau Gauss mod2 & mod13 → dim mod2=35,
  mod13=35 ; cohérent avec MÉCA 59−rang24). Recherche DANS le noyau : les points BIJECTIFS (chiffre valide)
  ne sont pas atteints par recuit (dim trop grande) ⇒ nécessite propagation-contrainte (moteur MÉCA, faisable
  seulement en dim-9). ⇒ 2-alph autoclé **exhausté avec l'info disponible** : aucun anglais trouvé.
- **Clé = phrase anglaise mémorisée ?** (keyletters.c) : les cribs RÉVÈLENT la clé (k_i=c_i∓p_i). Lues via
  A-Z / KRYPTOS / PALIMPSEST / ABSCISSA × Vig/Beau/var, les lettres de clé aux 24 positions = **non-anglais**
  (ex. KRYPTOS/Vig : RDUMRIYWOYNKY). ⇒ la clé n'est pas une phrase anglaise simple sous alphabet public évident.
