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

**Contrôles du 24/09** (`audits/vision_2026_09_24/`) :
- Largeur 21 : p = 0,0003 pour cette largeur seule, mais **p ≈ 0,09** une fois compté le balayage de toutes les largeurs 2–48. La NSA avait relevé un « intervalle 7 » dès 1992 (base 6). Réel mais faible.
- IC des positions 0–20 (0,0667, « anglais ») : **non significatif** (42 % des K4 mélangés ont une fenêtre de 21 aussi forte ; la meilleure fenêtre de K4 atteint 0,0857).
- **Symétrie des cribs** : les positions 23–33 et 73–63 sont en miroir exact autour de la position 48 (11 paires dont clair et chiffré sont connus). Seule la « pliure » complète (clé = clair en miroir) est décidable : éliminée.
- **Lettres doublées (ajout du 24/09, base 7 §7.2)** : les 6 doublets de K4 commencent en 18 (BB), 25 (QQ), 32 (SS), 42 (SS), 46 (ZZ) et 67 (TT). **5 sur 6 sont ≡ 4 (mod 7).** Mélanges de K4 : p ≈ 0,0006 pour le module 7 seul, **p ≈ 0,02** en balayant les modules 3–16. La statistique a été choisie après coup (J. Gillogly, 2005, qui les jugeait « causées par le chiffrement »). Trois de ces doublets sont dans les cribs (NO→QQ, ST→SS, IN→TT). C'est le signal « 7 » le plus net relevé à ce jour, plus net que la largeur 21.
- **« Réfléchissant »** : en 2007, Scheidt a qualifié le code de *reflective* en refusant le mot « symétrique » (base 7 §7.1). *Lecture* : à rapprocher du miroir des cribs ci-dessus.
- **Coïncidences à l'écart 7 (ajout du 24/09)** : c[i] = c[i+7] neuf fois, pour 3,3 attendues (p ≈ 0,005 pour cet écart seul). C'est vraisemblablement la « *slight interval 7 property* » de la NSA.
- **Les trois ensemble : le 7 est réel** (`stats_7.py`, base 7 §8.2).
  - Doublets mod 7, écart 7 et largeur 21 sont indépendants sous l'hypothèse nulle.
  - Pris ensemble, avec le module libre de 2 à 24 : **p ≈ 10⁻⁵** (1 mélange de K4 sur 100 000).
  - Ce n'est **pas** une substitution dépendant de i mod 7 : en 65 et 72, P chiffre R puis C. Ce n'est pas non plus une autoclé sur le chiffré à l'écart 7.
  - Aucune des 10 familles simulées (`sim_7.py`) ne concentre les doublets comme K4.
  - **Le mécanisme du 7 est la question ouverte principale.**

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
- **Nuance (manuscrit de Sanborn, AAA 6/9, vers 2009)** : *« only one, very focused guy remained and **cut out K3 and K4 by himself** in just two months with **virtually no errors** »*. Les trois erreurs connues de K1–K2 viennent de la **feuille** (clé, lettre omise), pas seulement du découpage. Pour K3–K4, un seul découpeur, contrôle double au pochoir : **le risque de coquille de découpe baisse ; celui d'une erreur sur la feuille de chiffrement reste entier.** L'audit « one-slip » garde donc son sens, pour une erreur de feuille.

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
- **Nuance par des sources d'époque (AAA, base 5)** :
  - *Washington Post*, 14 janvier 1990 : *« the other half will be encoded in a **modern system created for the project by an expert cryptographer** »* ;
  - GSA, 1993 : *« he encoded the screen **with the assistance of** a cryptographer »* ;
  - manuscrit de Sanborn (vers 2009) : *« I needed contemporary expertise… Enter Edward Scheidt »* ; à la dédicace, il remet *« some of the plain text and a **partial** code key »*.
  - ⇒ En 1990, le second système est présenté comme **conçu par Scheidt pour Kryptos**, pas comme une adaptation d'amateur. Les « modifications » de Sanborn (récit de 2009 et 2020) restent possibles mais **ne sont pas attestées à l'époque**. Les deux lectures (système de Scheidt exécuté fidèlement, ou système simplifié par Sanborn) restent ouvertes ; **aucune n'est privilégiée**.
- **Chronologie établie (manuscrit 6/9)** : le tableau KRYPTOS a été découpé **avant que le clair soit écrit** ; le clair était encore « endlessly revised » mi-1989. L'alphabet KRYPTOS était donc fixé avant K4, **indépendamment** de son clair.

### D. « Le clair de K4 est de l'anglais courant »
- Sanborn : « I wrote the plain text for Kryptos to be **enigmatic** » (IMG_1410) ; « a riddle within a riddle » (2020) ; « K4 pointe vers K5 » (2025).
- Sa liste de procédés : « Beaufort cipher, **Compass cipher**, Morse code, **Alphabet code**, **Cryptonyms** » (IMG_1569), avec sa description des cryptonymes CIA (IMG_1570).
- K5 « partage certains **mots codés** aux mêmes positions » que K4 (2025).
- Les deux fragments connus sont une **direction de boussole** et une **horloge**, deux cadrans qui codent une position par un angle ; l'œuvre contient une rose des vents dont l'aiguille est gravée ENE.
- **Lecture** *(interprétation)* : le clair est probablement **fait de mots codés** (directions, heures, cryptonymes), pas de prose. Il faudrait **l'œuvre elle-même** pour le lire ensuite (d'où « a riddle within a riddle »).
- **Conséquence pratique, et c'est peut-être l'angle mort le plus coûteux :** presque toutes les recherches publiques trient leurs candidats par **ressemblance à l'anglais** (n-grammes). Un clair fait de directions et de mots codés obtiendrait un score médiocre ; le bon candidat a pu être **produit puis écarté** par ce juge. Nos propres tests n'y sont pas exposés : ils ne jugent que la cohérence exacte avec les 24 lettres, jamais l'anglais.
