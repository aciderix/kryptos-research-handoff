# Empreinte du mécanisme K4 — ce qu'il DOIT être (synthèse MÉCA+chef, 2026-09-27)

Non pas un « mur » : un **portrait-robot**. En éliminant rigoureusement (algébre + qg_big décisif +
contrôles/null) toutes les familles standard, on a resserré la cible. Ce document dit ce que le
mécanisme doit satisfaire — pour orienter le front « masquage non-standard » (Scheidt).

## Contraintes POSITIVES mesurées sur K4 (invariants)
1. **1:1, longueur préservée** : 97 lettres, cribs à positions exactes (Sanborn confirme
   BERLINCLOCK↔NYPVTTMZFPK). ⇒ pas de transposition qui réordonne, pas de drop/pad.
2. **97 est PREMIER** ⇒ aucun découpage en blocs de taille n≥2. (tue Hill/blocs)
3. **Les 26 lettres présentes** (J inclus), aucune omise ⇒ pas de carré 5×5 (25). (tue Playfair/two/four-square)
4. **IC = 0,0361** (≈ aléatoire 0,0385 ; anglais mono 0,066) ⇒ **polyalphabétique franc**, PAS
   monoalphabétique ⇒ pas de (transposition ∘ substitution simple).
5. **Excès de coïncidences à l'écart 7** réel mais DIFFUS (9 vs 3,2 ; z=3,28 ; p≈0,005 ; réparti
   sur les 7 résidus, tiré par O/K). Une **propriété à reproduire**, pas une preuve d'autoclé.

## Familles ÉLIMINÉES (avec preuve/contrôle)
| famille | par | verdict |
|---|---|---|
| autoclé écart fixe, alphabet mot-clé visible | chef (qg_big) | négatif |
| autoclé écart fixe, alphabet LIBRE (σ=τ et 2-alph), tout écart | MÉCA (bijcheck/gapscan) | impossible ou sous-déterminé (dim≥19) |
| Vigenère/Quagmire périodique + alphabet visible | chef | négatif |
| fractionnement bifid/trifid (public ET libre) | chef | négatif |
| running-key clairs K1/K2/K3 | chef | négatif |
| running-key CHIFFRÉS K1/K2/K3 | MÉCA (runkey_ct) | négatif |
| clé progressive/Trithemius linéaire (a·i+b) | chef | négatif |
| clé-grille/overlay 2-panneaux (a·row+b·col+c, +4%) | chef | négatif |
| géométrie intra-panneau (col/row/arc/z) 1:1 | MÉCA (Addendum 12) | négatif |
| courbure→alphabet ; rose→clé (transcript) | MÉCA (Addendum 13) | négatif |
| digraphique/blocs (Playfair, Hill) | MÉCA (digraphic_check) | impossible (structure) |
| transpo∘autoclé ; périodique∘transpo (Friedman) | chef | négatif |

## LOI dégagée
Tout modèle à **alphabet libre** est, sous 24 cribs, soit **impossible** soit **sous-déterminé**
⇒ **l'alphabet est PUBLIC** (dérivable du site). Or tous les schémas de clé standard sur alphabet
public sont éliminés. ⇒ **Le reste est nécessairement un « masquage » non-standard (Scheidt,
« changer la base vers autre chose »)** : polyalphabétique, 1:1, alphabet public, avec une règle de
clé/masque qui n'est PAS un chiffre de manuel.

## Cible pour le front masquage (chef) — critères d'admissibilité d'une hypothèse
Une hypothèse « masque » testable doit : (a) produire un flux de clé/opération **déterministe** à
partir d'une source **100% publique** alignée sur les **97 positions** (1:1) ; (b) être
polyalphabétique (aplatir l'IC) ; (c) idéalement reproduire l'**excès diffus écart-7**. Dès qu'une
telle règle est posée, MÉCA la teste (cribs + qg_big + null) immédiatement — le moteur exact
(variété/permutation) et le juge qg_big sont prêts.
