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
- **Contrôle du 23/09.**
  - L'effet suit bien **la lettre et non la position**. Sur les 11 petits écarts, 9 tombent sur des lettres de KRYPTOS : p = 0,0004. Leur concentration dans EASTNORTHEAST est moins nette : p = 0,017.
  - Mais seules **13 lettres claires** figurent dans les cribs. L'effet porte en réalité sur **5 lettres (R, S, T, O, K)**, dont plusieurs répétées.
  - Un **ensemble aléatoire de 7 lettres** fait aussi bien dans **0,3 %** des cas au seuil ±2, et dans **5,6 %** au seuil ±1.
  - **Conclusion : suggestif, pas assez solide pour fonder un mécanisme.**
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

---

## Relecture critique : trois hypothèses que tout le monde tient pour acquises (23/09)

### A. « K4 a été exécuté sans faute »
- **Faits (feuille K1/K2 publiée par le NYT en 2010, vérifiée au pixel dans `docs/nyt_k1k2_chart_physical_layout_2026_09_19.md`) :**
  - le mot-clé écrit PALIMPCEST sur la feuille produit IQLUSION, **erreur de clé** fidèlement gravée ;
  - la feuille donne E, le cuivre R : c'est une **erreur de gravure** (UNDERGRUUND) ;
  - une lettre omise sur le cuivre donne IDBYROWS au lieu de LAYER TWO.
- **Au moins 3 erreurs de fabrication sur 432 lettres**, et chacune a égaré les solveurs pendant des années. Sanborn : « il n'y avait personne pour relire » (2020).
- **Conséquence.** Toutes les éliminations de K4 (communauté, dépôt, les nôtres) exigent que les 24 lettres connues soient exactes. Avec un taux d'erreur de cet ordre, un procédé « beaucoup plus difficile » a une probabilité **non négligeable** de contenir une erreur dans la zone des cribs. Le vrai mécanisme aurait alors été écarté à tort.
- **Test** : `audits/one_slip_2026_09_23/` (familles à mot-clé et clés suivies, une erreur tolérée, témoin aléatoire).

### B. « Les anomalies sont des indices »
- IQLUSION et UNDERGRUUND sont des **accidents de fabrication**, prouvés par la feuille : la décision n'existait pas au moment du chiffrement.
- Dans ses notes (IMG_1340, si la lecture est juste), Sanborn présente le L en trop et les « ? » comme des **particularités expliquées** (« not coded »).
- Le discours « mes erreurs sont voulues » (2005) est **postérieur**, et il le nuance lui-même en 2020 (« certaines n'étaient pas voulues »).
- ⇒ **Donner plus de poids aux documents d'époque** (lettre de 1989, discours de 1990, feuilles de travail) qu'aux entretiens ultérieurs. Les programmes de recherche fondés sur Q, U, L, « ? » reposent sur une lecture probablement erronée.

### C. « Les descriptions de Scheidt décrivent K4 »
- Sanborn (2009, 2020) : il a **modifié lui-même** les suggestions de Scheidt, « si bien que même lui ne sait pas ce que ça dit ». Scheidt (2005) : il ne sait pas ce que Sanborn a changé.
- ⇒ « Masquage », « changer la base du langage », « plus d'une étape » décrivent ce que Scheidt a **proposé**, pas nécessairement ce que Sanborn a **exécuté**.
- Un artiste « réfractaire aux maths » qui adapte un chiffre de terrain le **simplifie** plus probablement qu'il ne le complique. Il l'exécute sur le même type de feuille (lignes de 31 : clair, clé, chiffré) et **avec des erreurs**.
- **La source la plus fiable sur la méthode réelle reste Sanborn en 1990** : le tableau pour la plaque du haut ; la plaque du bas « d'une façon beaucoup plus difficile », avec deux systèmes (K3 et K4).
