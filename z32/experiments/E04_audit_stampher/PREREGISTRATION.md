# Z32-E04 — Audit d'une affirmation publiée : le « biais structurel » du chiffre vers 8 h et 10 h (Stampher) — PRÉ-INSCRIPTION

Rédigée avant le calcul (2026-09-28).

## Affirmation auditée (dstampher/zodiac-z32-cipher, README et verify.py V8)
« The cipher itself is mechanically biased toward the Zodiac's crime zones. 87 % of the 54 survivors point to clock hours 8 or
10, representing a 5.2× enrichment over random expectation. » L'espérance utilisée est 2/12 (heures uniformes).

## Question
Cet enrichissement vient-il du **chiffre** (les 3 verrous), ou de la **grammaire** et du **filtre de carte** seuls ?
Référence juste : la même grammaire et le même filtre de carte **sans** les verrous.

## Calcul (reproduction exacte en C de z32.py et de ses constantes data.py/geo.py)
Distribution des heures pour : (a) phrases de 32 lettres ; (b) (a) + limites de carte ; (c) (a) + verrous ; (d) (a) + verrous
+ carte (= les 54 survivants). Enrichissement attribuable aux verrous = part(8, 10) en (d) / part(8, 10) en (b).

## Décision
- Si part(8, 10) en (b) est déjà nettement supérieure à 2/12, l'espérance « aléatoire » de Stampher est **fausse** et
  l'enrichissement est au moins en partie un effet de carte.
- L'effet propre des verrous est testé par tirage : 54 phrases prises au hasard dans (b), 100 000 fois ; p = fraction des
  tirages avec part(8, 10) ≥ celle observée en (d). Si p > 0,01, « biais structurel du chiffre » **non soutenu**.

## AMENDEMENT 1 (après le calcul principal, avant ce calcul supplémentaire)
Résultat principal : l'enrichissement vient des verrous (52 des 61 phrases à verrous sont à 8 h ou 10 h, avant toute carte).
Question supplémentaire : ce « ciblage » est-il propre aux répétitions **réelles** du Z32, ou un effet d'orthographe que
produiraient aussi des répétitions **quelconques** ? Test : 100 000 motifs aléatoires de 3 paires de positions disjointes
(parmi les 32), même grammaire et même carte. Pour chaque motif avec ≥ 10 survivants : (i) part des 2 heures les plus
fréquentes ; (ii) ces 2 heures sont-elles toutes deux des « heures de crime » C = {heures, arrondies, des 4 scènes de
Stampher vues du mont Diablo} ? p1 = fraction des motifs avec part ≥ 0,87 ; p2 = fraction des motifs dont les 2 heures
dominantes sont dans C et de part ≥ 0,87. « Ciblage des crimes » **non soutenu** si p2 > 0,01.
