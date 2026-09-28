# E13 — Hypothèse « 13 classes » : chaque symbole vaut deux lettres (plusieurs-à-un) — PRÉ-INSCRIPTION

Rédigée et commitée avant le calcul (2026-09-28).

## Motivation
E11-E12 : hors colonne 14, 13 symboles quasi uniformes ; aucun mécanisme injectif ni aucun mécanisme du livre testé
ne le reproduit ; le seul qui approche le **nombre** (≈ 13) est le carré de Wolseley (p. 109-111) lu par
**numéro** (12 numéros couvrant chacun 2 cases symétriques + la case centrale). Calcul indicatif : l'anglais fusionné
en 13 classes « équilibrées » (E seul, puis fréquente + rare : T+Z, A+Q, O+X, …) donne des effectifs attendus sur 182
lettres (22,6 ; 16,7 ; 14,9 ; 14,7 ; 14,6 ; 14,4 ×3 ; 14,1 ; 11,3 ×2 ; 9,5 ×2) proches de l'observé
(20 ; 17 ; 17 ; 16 ; 16 ; 15 ; 14 ; 13 ; 12 ; 12 ; 11 ; 10 ; 9).

## Questions (fermées)
**Q1 — profil.** Le profil trié des 13 symboles (colonnes 1-13, 182 paires) est-il compatible avec un texte naturel
dont les 25 lettres sont fusionnées en 13 classes (12 paires + 1 singleton) ? Partitions testées :
(a) partition **équilibrée optimale** de la langue (glouton fréquente + rare ; borne de planéité) ;
(b) partitions **de Wolseley** : carré à mot-clé (mot-clé puis alphabet, I = J), cases appariées par symétrie
centrale, centre seul — mots-clés = mots du livre (≥ 5 lettres distinctes) ;
(c) partitions **aléatoires** (témoin).
Pour chaque partition : 2 000 extraits de 182 lettres (anglais ; 300 pour fr, de, it, es, la, nl, eo), profil trié
des classes, statistique D (E11) contre le profil observé ; p = rang de D_obs. Pour (b) : distribution des p sur
les mots-clés, et liste des mots-clés dont p > 0,05.
**Q2 — structure séquentielle.** Puissance du test de dépendance des bigrammes (G-test, E11 § 3) sur des suites de
classes anglaises **non transposées** de 182 symboles : fraction des extraits où p < 0,05. Si elle est élevée,
l'absence de dépendance dans le défi (p = 0,71) implique une **transposition** en plus (ou l'absence de langue).
**Q3 — documentation.** Recherche (livre, sources en ligne) d'un système historique « un symbole = deux lettres » à
13 symboles ; consignée avec sources.

## Décision
- Q1 : « compatible » si p > 0,05 pour (a) dans au moins une langue, **ou** pour au moins un mot-clé de Wolseley ;
  la liste des mots-clés compatibles devient l'entrée d'une attaque dédiée (E14, à pré-inscrire).
- « Incompatible » sinon : l'hypothèse « 13 classes » est défavorisée.
- Aucun déchiffrement n'est revendiqué dans E13 (profils seulement).
