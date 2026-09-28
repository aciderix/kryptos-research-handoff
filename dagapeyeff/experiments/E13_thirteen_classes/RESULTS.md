# E13 — RÉSULTATS (2026-09-28) : hypothèse « 13 classes équilibrées » **compatible** (profil), non documentée

Pré-inscription : `PREREGISTRATION.md`. Script : `tools/e13_classes.py` ; sortie : `e13.out`.

## Q1 — profil (colonnes 1-13, 182 symboles ; observé 20 17 17 16 16 15 14 13 12 12 11 10 9)
| Partition des 25 lettres en 13 classes | Résultat |
|---|---|
| (a) **équilibrée optimale** (E seul, puis fréquente + rare), fréquences de la langue (corpus d'entraînement) | **compatible** en **anglais (p = 0,20)** et **latin (p = 0,19)** ; italien limite (0,0498) ; fr, de, es, nl, eo rejetés (p ≤ 0,02) |
| (b) **Wolseley** (carré à mot-clé, symétrie centrale), 3 921 mots du livre | aucune avec p > 0,05 (meilleure : TOTALLY, p = 0,043) ; 0,5 % avec p > 0,01 |
| (c) aléatoires (200) | 0 % compatibles (médiane p = 0,0005) |
Le profil attendu en (a) n'est **pas** ajusté au chiffré : il est prédit par les seules fréquences de la langue.

## Q2 — structure séquentielle
Sur des suites de classes anglaises **non transposées** (182 symboles), la dépendance des bigrammes est détectée
dans **99,7 %** des extraits. Le défi n'en montre aucune (p = 0,71) ⇒ sous l'hypothèse « 13 classes », il faut
**en plus une transposition** (ou le texte n'est pas linguistique).

## Q3 — documentation
- Livre : aucun système « un symbole = deux lettres à fréquences équilibrées » ; les systèmes numériques décrits
  sont injectifs (ex. chiffre militaire allemand p. 103-104, lettres → nombres 11-38) ; le plus proche reste
  Wolseley lu par numéro (p. 109-111), non compatible sous sa forme standard (Q1 b).
- Recherche en ligne rapide : rien de fiable ; un résumé de moteur de recherche attribuant au chiffre de Porta un
  appariement « fréquente + rare » est **inexact** (Porta indexe 13 alphabets par des paires de lettres de clé,
  sans lien avec la fréquence) — écarté.

## Décision et portée
- Règle pré-inscrite : **compatible** (Q1 a, anglais et latin).
- Portée : le profil ne **distingue pas** cette hypothèse d'un tirage uniforme sur 13 symboles (E11 : p = 0,98) ; il
  la rend seulement **possible**, là où tous les mécanismes injectifs et ceux du livre sont rejetés. Elle exige une
  transposition supplémentaire (Q2) et n'est pas documentée (Q3).
- Suite possible (E14, à pré-inscrire) : attaque jointe « appariement + correspondance des 13 symboles + transposition »
  avec un modèle de langue **au niveau des classes** ; distance d'unicité estimée ≈ 30 symboles (182 disponibles),
  donc déchiffrable en principe, mais la lecture finale resterait ambiguë (2 lettres par symbole).
