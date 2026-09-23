# Base 4 — Les phénomènes de K4 (méthode d'avant l'ordinateur)

**Version :** 2026-09-23
**Principe.** K1–K3 ont été cassés par la méthode classique (Callimahos, *Military Cryptanalytics III* ; Lewis, *Solving Cipher Problems*) : **trouver un phénomène** dans le chiffré, le **mesurer**, l'**expliquer** par un geste manuel, puis l'**exploiter**. Pour K1–K2, c'était la périodicité ; pour K3, des fréquences anglaises. Ici, on n'essaie pas des clés : on cherche **quel geste de Sanborn produit ce qu'on observe**. Un test ne vient qu'après, pour vérifier une explication déjà formulée.

## Phénomène 1 — Les lettres de KRYPTOS restent presque en place (Materna, via Bean 2021)

Écart dans l'alphabet normal entre la lettre claire et la lettre chiffrée, aux 24 positions connues :

| Clair ∈ {K,R,Y,P,T,O,S} | Autres lettres claires |
|---|---|
| S→R −1, T→V +2, O→Q +2, R→P −2, T→R −2, S→S 0, T→S −1, R→P −2, K→K 0, **O→F −9** | E→F +1, A→L +11, N→Q +3, H→N +6, E→G +2, A→K +10, B→N +12, E→Y −6, L→V +10, I→T +11, N→T +6, C→M +10, L→Z −12, C→P +13 |
| **9 sur 10 à ±2** | **2 sur 14 à ±2** (les deux sont des E) |

- Bean évalue ce phénomène à **1 chance sur 5 520** pour une permutation aléatoire. *Réserve :* il a été remarqué **après** avoir regardé les données.
- **Ce qu'il dit, s'il est réel.** L'effet du chiffrement **dépend de la lettre claire elle-même**, et pas seulement de la position. Les lettres du mot KRYPTOS sont traitées à part. Ce n'est pas le comportement d'un simple décalage par position, quel que soit l'alphabet : c'est celui d'un **alphabet construit sur le mot KRYPTOS**, utilisé d'une façon que personne n'a encore identifiée.
- **Ce qui est déjà exclu** (base 2) : un seul décalage par position, avec les alphabets normal ou KRYPTOS.

## Phénomène 2 — Les nombres 7 et 21

- **Paires répétées en largeur 21** : 11 paires de lettres superposées se répètent quand on écrit K4 en lignes de 21 lettres (1 chance sur 6 750, Bean). Le NSA avait déjà relevé 7 et 14 en 1992.
- **Les deux cribs débutent sur des multiples de 21** (positions 21 et 63, en comptant à partir de 0).
- **KRYPTOS a 7 lettres.**

## Phénomène 3 — Une seule lettre claire, plusieurs chiffrées, mais rapprochées

Une même lettre claire donne des lettres chiffrées différentes selon la position. Pourtant, ces positions sont **proches les unes des autres** (distance moyenne 3,6). C'est l'indice statistique d'une correspondance lettre à lettre **sans transposition** (Bean), confirmé par Sanborn en 2019.

## Place des cribs sur le panneau (lignes de 31, K4 commence ligne 25, colonne 27)

- EASTNORTHEAST occupe **la ligne 26 seule**, colonnes 17 à 29.
- BERLINCLOCK est **coupé par la fin de ligne** : « BER » termine la ligne 27, « LINCLOCK » ouvre la ligne 28.

## Programme (raisonnement, pas recherche)

Pour chaque mécanisme manuel plausible en 1989, se demander s'il produit **à la fois** les phénomènes 1, 2 et 3, avec les indices de Sanborn : un mot-clé, un système visuel « individuel », « changer la base du langage », une méthode « simple, au crayon ». On vérifie **à la main sur les 24 paires**, **avant** tout programme. Un mécanisme qui n'explique pas le phénomène 1 est écarté, même s'il « passe » les cribs.
