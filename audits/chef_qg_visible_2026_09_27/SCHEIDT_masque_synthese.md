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
