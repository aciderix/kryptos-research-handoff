# 14 — La « question centrale » tranchée par le calcul : la clé aux cribs n'est PAS un mot-clé écrit dans une grille 7-large + décalages (28/09/2026)

**Statut :** négatif contrôlé. Ce document répond à la **question centrale** posée en base 08 §7 et reprise dans le
round conjoint chef+MÉCA (base 16 §11, « nouvelle direction » après rétractation du verdict OTP circulaire) :
la clé Vigenère **aux 24 positions de crib authentiques** (aveugle : chiffré K4 + EASTNORTHEAST 21-33 + BERLINCLOCK
63-73, **aucun faux clair**) est-elle compatible avec un **mot-clé inscrit dans une grille 7-large** (couche « masque »
Scheidt) modulé par un **décalage par ligne** (couche Sanborn), qui **déterminerait** alors la clé aux 73 autres
positions ? Script : `audits/autocle_recuit_2026_09_26/grid7_keylink.py`.

## 1. Le modèle testé (le plus serré, puis le plus généreux)

Grille 7-large : position `i` → colonne `c = i mod 7`, ligne `r = i div 7`. Deux modèles additifs (Vigenère `k=c−p`,
Beaufort `k=p−c`, et variante `k=c+p`) :

- **M1 — décalage vertical global** : `k[i] = A[c] + s·r`. Un mot-clé de 7 lettres `A[0..6]` + **un** pas vertical `s`
  commun (le chef observe `k[i+7]−k[i] = −3` sur 4/10 paires → `s=−3` candidat). Si vrai, ça donne toute la clé.
- **M2 — décalage vertical propre à chaque colonne** : `k[i] = A[c] + s[c]·r` (le vrai degré de liberté Quagmire III :
  une droite indépendante par colonne). Modèle le **plus généreux** possible sans devenir vide.

**Garde-fou obligatoire (chef + règle d'or)** : tolérance Sanborn (1-2 erreurs) **et surtout CONTRÔLE NUL** — même test
sur (a) flux de clé aléatoires aux mêmes 24 positions, (b) **K4 mélangé** (chiffré permuté, mêmes cribs aux mêmes
places). Sans nul, un ajustement à 13/24 est de la **paréidolie** (cf. Hallström : grille-7→clé a déjà fait 7/24 = hasard).

Couverture des cribs dans la grille 7-large : colonnes 0-3 ont **4 lignes** de crib chacune, colonnes 4-5 en ont 3,
colonne 6 en a 2 → 6 colonnes « contraignantes » (≥3 points, donc une droite `A+s·r` y est réellement testable).

## 2. Résultat (20 000 tirages nuls)

| Modèle | Meilleur ajustement | Contrôle nul | Verdict |
|---|---|---|---|
| **M1** Vigenère, `s=−3` | **13/24** positions | aléatoire moy **10,4**, **max 15** ; p(≥13)=0,006 | dans le plafond du bruit |
| **M1** Beaufort, `s=+3` | 13/24 (mêmes 13, signe inversé) | idem | même unique fait |
| **M1** variante `c+p` | 10/24 | p(≥10)=0,95 | pur bruit |
| **M2** droite/colonne, 0 err/col | **1/6** colonnes | nul moy 0,09, **max 3**, p(≥1)=**0,083** | non significatif |
| **M2** droite/colonne, 1 err/col | 2/6 colonnes | nul moy **2,58**, max 6, p(≥2)=**1,00** | **cœur du bruit** |

## 3. Lecture

- **M1 ne tient pas.** 13/24 au meilleur `s=−3` laisse **11 positions de crib en désaccord**, très au-delà des 1-2
  erreurs Sanborn. Surtout, le **nul atteint 15/24** : 13 est **sous le plafond du hasard**. Le p=0,006 vs aléatoire est
  trompeur (recherche sur 26 `s` × 3 conventions ; et Beaufort 13/24 est le **même** fait). Le pas vertical `−3` du chef
  est une **régularité partielle réelle mais faible**, exactement le régime « 7/24 = hasard » qu'il a lui-même désigné
  comme piège.
- **M2, le plus généreux, ne tient pas non plus.** En autorisant une droite **indépendante par colonne**, une seule des
  6 colonnes contraignantes s'ajuste sans erreur — et le hasard en donne jusqu'à 3 (p=0,08). À 1 erreur/colonne, on
  obtient 2/6 alors que **le hasard en donne 2,6 en moyenne** (p=1,00). Aucun signal.

## 4. Verdict (réponse à la question centrale)

**Non.** La clé aux 24 positions de crib **n'est pas engendrée** par un mot-clé écrit dans une grille 7-large modulé par
un décalage (global ou par colonne), au-dessus du hasard. **Conséquence directe pour l'échange** : on **ne peut pas**
lire un mot-clé sur la grille pour **déterminer** la clé aux 73 positions restantes — cette voie est du bruit. Le demi
qui m'était attribué (base 16 §11 : « le lien de clé entre colonnes voisines d'un bloc de 7 ») est **fermé au calcul,
par le négatif contrôlé**.

**Ce que ça ne tue pas :** ce test porte sur le **lien additif** (clé Vigenère périodique lisible en grille). Il **ne**
réfute **pas** la moitié du chef — une **transposition/route** physique sur la grille *suivie* d'un déchiffrement
Quagmire jugé `qg_big` + contrôle nul — tant que la clé résultante n'est **pas** un mot-clé additif extrapolable. Il
recentre l'effort : la structure pas-7 réelle (5/6 doublets en colonne 4 mod 7 ; 9 coïncidences verticales vs 3,5) est
un fait de **placement**, pas la signature d'un **générateur de clé** en grille.

## 5. Antériorité, limites

- **Négatif de capacité, pas d'impossibilité.** Un modèle non additif (alphabet KRYPTOS-keyé + route non triviale) reste
  à juger par la voie du chef (énumération de routes + `qg_big` + nul). Ce qui est établi : le **générateur mot-clé-en-
  grille additif** ne reproduit pas la clé-crib au-dessus du bruit, sous tolérance Sanborn.
- **Cohérent avec** base 08 §7 (question ouverte), base 13 (la variété crib-consistante est de grande dimension : ici on
  voit *pourquoi* aucune structure basse-dimension simple ne s'y imprime), et l'avertissement Hallström (base 16 §11).
- **Aucun essai sur K4 présenté comme concluant** (règle d'or : contrôle nul d'abord).

*(Base 14, agent MÉCANISME, branche `claude/loving-einstein-fizl93`. Le chef édite la synthèse 16 et y pointera ce
document ; un seul propriétaire édite chaque base.)*
