# Z32-E03 — Test du cycle strict des homophones — PRÉ-INSCRIPTION

Rédigée et commitée avant tout calcul de E03 (2026-09-28). **Déclaration** : en explorant (avant E01), j'ai remarqué à la
main que le candidat DMW/Stampher `INTHREEANDTHREEEIGHTHSRADIANSTEN` contredit un cycle strict (E aux occurrences 1 et 3 sur
le même symbole, mais 2 et 4 sur des symboles différents). Aucun autre candidat n'a été examiné sous ce critère.

## Idée
Dans le Z408, le Zodiac utilise souvent les homophones d'une lettre **dans un ordre fixe** (cycle) ; le Z340 le montre
beaucoup moins (arXiv 2403.17350, § 3). Sous un cycle strict de période n pour une lettre, deux occurrences de la lettre
portent le même symbole **si et seulement si** leurs rangs sont congrus modulo n. Les 3 répétitions du Z32 imposent alors
des périodes précises aux lettres concernées, et donc des contraintes sur les autres occurrences de ces lettres.

## Définition
Pour une lettre L d'occurrences 1..m (ordre de lecture, après la transposition éventuelle) et de classes de symboles
s_1..s_m : L est **cohérente avec un cycle strict** s'il existe n ≥ 1 tel que, pour tous j, j', s_j = s_j' ⇔ j ≡ j' (mod n)
(n ≥ m revient à « tous distincts »). Un texte (fenêtre) est cohérent si toutes ses lettres le sont.

## Calibration et admissibilité (règle fixée ici)
Taux de faux rejet = fraction des vraies fenêtres de 32 du Zodiac (clair réel, symboles réels) incohérentes :
Z408 (fenêtres sans symbole ambigu/erreur, comme E01) et Z340 section 1 (ordre de transposition connu).
Le test n'est **admissible** que si ce taux est ≤ 10 % **dans les deux** chiffrés. Sinon : conclusion « le Zodiac ne cyclait
pas assez strictement pour que ce critère juge un chiffré court », **sans** jugement des candidats.

## Si admissible
Puissance sur les mêmes leurres qu'E01 (grammaire de Stampher, 61 ; fenêtres d'anglais satisfaisant les verrous, 42) ;
puis candidats publiés d'E01 (même liste, mêmes lectures).
