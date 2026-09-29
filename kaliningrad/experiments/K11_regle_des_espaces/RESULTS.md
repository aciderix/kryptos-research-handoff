# K11 — RÉSULTATS (2026-09-29) : la règle des espaces est identifiée — **on coupe quand deux voyelles ou deux consonnes se rencontrent**, rien d'autre

Pré-inscription : `PREREGISTRATION.md` (commit a4ca22a). Outil : `tools/k11_espaces.py` (régression logistique, validation croisée
lignes impaires / paires, nul = ordre des mots mélangé). Sorties : `logs/`.

**Écart de procédure déclaré** : une vérification technique de l'outil, lancée avant les contrôles, a affiché les gains bruts sur le
texte réel (G_int = +15,7, G_fen = +8,6), sans nuls ni p. La règle de décision pré-inscrite n'a pas été modifiée. Après les premiers
contrôles, correction de deux bogues du contrôle russe (lettres cyrilliques ignorées, découpage des fenêtres) et ajout des voyelles
cyrilliques ; les contrôles sur textes réels ont été relancés entièrement avec l'outil corrigé.

## 1. Contrôles
| Contrôle | Attendu | Obtenu |
|---|---|---|
| (a) flux de la bouteille mélangé, espaces posés par une règle **avec** interaction (type M2) | G_int p < 0,01 | **10/10** |
| (a) idem, règle **sans** interaction (effets séparés) | G_int non significatif | **0/10** faux positif |
| (b) textes réels, 10 fenêtres de ~980 lettres : allemand, russe, polonais, néerlandais | G_int au niveau du nul | **40/40** |

## 2. Texte réel (1 000 nuls)
| Données | G_int (M2 − M1) | G'_int (M4 − M3) | G_fen (M5 − M2) | G_pos (M6 − M2) |
|---|---|---|---|---|
| v1 | **+15,7, p = 0,001** | **+16,5, p = 0,001** | +8,6, p = 0,034 | −0,9, p = 0,57 |
| v1, page 1 | +17,7, p = 0,001 | +18,5, p = 0,001 | +7,3, p = 0,022 | −1,1, p = 0,62 |
| **Corsair_nv 2015** | **+16,7, p = 0,001** | **+16,8, p = 0,001** | +9,4, p = 0,028 | −2,4, p = 0,82 |
(p = 0,001 est le minimum atteignable avec 1 000 nuls.) Log-vraisemblances hors échantillon (v1) : M0 −419,4 ; M1 −419,0 ;
**M2 −403,2** ; M3 −428,7 ; M4 −412,1 ; M5 −394,6 ; M6 −404,1.

Règle ajustée (M2, log-cotes) : base −3,11 ; longueur du mot en cours 2 : −0,27, 3 : +0,74, 4 : +0,96, 5 : +1,00, 6 : +1,52,
7 : +1,51, 8+ : +2,26 ; lettre d'avant voyelle +0,15 ; lettre d'après voyelle +0,26 ; **voyelle|voyelle +1,27** ; **consonne|consonne
+0,86** ; lettres identiques +0,31.

## 3. Conclusions
**RÉSULTAT (pré-inscrit, confirmé sur les deux transcriptions)** : la probabilité d'un espace est gouvernée par une **règle locale à
deux lettres** : elle augmente fortement quand deux voyelles ou deux consonnes se rencontrent (et un peu plus si elles sont
identiques), et avec la longueur du mot en cours. L'effet de fenêtre plus large (±2 lettres) est suggestif (p ≈ 0,02-0,03) mais
n'atteint pas le seuil pré-inscrit : non retenu. La position dans la ligne ne joue aucun rôle.

**Point frappant** : les classes séparées (M1) n'apportent rien par rapport à la longueur seule (M0), et **l'identité des lettres
(M3) prédit moins bien que rien**. Exploration de contraste (non pré-inscrite, déclarée) : dans les textes réels, connaître la lettre
qui termine ou commence un mot améliore énormément la prédiction (M3 − M0 = +70 à +115 en moyenne selon la langue, jamais moins de
+48 sur 40 fenêtres) ; bouteille : **−9,2**. Les « mots » n'ont aucune terminaison ni aucun début typiques d'une langue ; seule
compte la rencontre de deux lettres de même classe.

**Portée** : cela décrit précisément le procédé d'**habillage** (faire des « mots » en coupant les rencontres voyelle-voyelle et
consonne-consonne, jusqu'à une longueur de 3 à 8 lettres). Cela ne dit pas d'où vient le flux de lettres (chiffré ou texte
artificiel) et ne donne aucune lecture.
