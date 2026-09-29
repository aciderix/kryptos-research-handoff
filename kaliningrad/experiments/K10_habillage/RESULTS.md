# K10 — L'« habillage » : les espaces ont été posés **après coup, en regardant les lettres voisines** (2026-09-29)

Statut : **OBSERVATION issue d'une exploration** (non pré-inscrite), mais contrôlée (9 langues réelles), répliquée sur une
**transcription indépendante** et sur des moitiés disjointes du texte, avec un effet (z ≈ −6) qui résiste à toute correction
raisonnable pour les ~30 regards exploratoires de la session. Scripts : reproduits dans `k10_frontieres.py`.

## 1. Le test
Pour chaque frontière entre deux « mots », on regarde la dernière lettre du mot et la première du suivant (voyelle a e i o u,
accents fusionnés, ou consonne). Statistique : nombre de frontières **consonne|voyelle ou voyelle|consonne**. Référence : le même
texte avec **l'ordre des mots mélangé** (20 000 fois). Si les mots sont de vrais mots, ou des mots inventés un par un, l'ordre des
mots ne change pas cette statistique (la première lettre d'un mot ne dépend pas de la dernière du précédent). Si quelqu'un a coupé
une suite de lettres déjà écrite en choisissant où couper selon les lettres de part et d'autre, l'effet apparaît.

## 2. Résultats
| Données | mots | frontières C|V réelles | attendu (mots mélangés) | z |
|---|---|---|---|---|
| v1, texte entier | 193 | **50** | 88,1 | **−6,0** |
| v1, page 1 | 157 | 34 | 72,4 | −6,6 |
| v1, lignes paires / impaires | 99 / 94 | 24 / 25 | 44,7 / 42,9 | −4,6 / −4,0 |
| **Corsair_nv 2015 (lecture indépendante)** | 186 | 45 | 86,1 | **−6,5** |
| v0 (notre première lecture) | 192 | 51 | 88,0 | −5,9 |
| v1, page 2 (feuille pâle) | 36 | 15 | 15,5 | −0,2 (Corsair : −1,3 sur 29 mots) |

Contrôle, textes réels (fenêtres de 194 mots, même test) : allemand (2 livres), néerlandais, français, russe, polonais, tchèque,
finnois, hongrois : z moyen entre −0,4 et +2,1 ; **aucune des 488 fenêtres** ne descend sous −2,8.

Détail (v1) : aux frontières réelles, 43 voyelle|voyelle (attendu 24), 99 consonne|consonne (attendu 80), 25 lettres identiques
(attendu 15). Taux d'espace selon la paire de lettres du flux : consonne-voyelle 10,6 % ; consonne-consonne 23 % ; voyelle-voyelle
31 % ; voyelles doublées 55 % (« ee » coupé 13 fois sur 24).

## 3. Ce que cela établit
- Les « mots » ont été obtenus en **découpant une suite de lettres déjà existante**, et les coupures ont été choisies **selon les
  lettres de part et d'autre** : on coupe entre deux consonnes, entre deux voyelles, entre deux lettres identiques ; on garde
  ensemble consonne + voyelle. Le flux sans espaces, lui, reste au niveau du hasard (alternance voyelle/consonne : 444 contre 442,7
  attendu ; aucun contact, K02). Des mots inventés un par un laisseraient au contraire une trace dans le flux.
- C'est un **habillage** destiné à faire ressembler la suite à une langue (mots « prononçables »), comme les autres traits déjà
  relevés : abréviations formées de grappes de consonnes (27 lettres, aucune voyelle), apostrophes toujours après une consonne,
  « i » isolé, doublets coupés.
- Conséquence pour le déchiffrement : **la vraie donnée est le flux de lettres sans espaces** (variante A de toutes nos cellules),
  et ce flux a été produit **avant** l'habillage, par un procédé sans enchaînement de lettres. Cela va dans le sens d'un **vrai
  chiffré déguisé** (hypothèse A : texte mélangé puis maquillé) plutôt que d'un charabia inventé mot à mot, sans l'exclure tout à
  fait (quelqu'un pourrait écrire un flux au hasard puis le couper).
- Non trouvé dans les sources lues (Cipherbrain 2016-2017 : « perhaps wrong spaces » évoqué sans mesure ; blog russe 2015).

## 4. Réserve : la page 2
La feuille 2 ne montre pas l'effet (z = −0,2 ; Corsair −1,3 sur 4 de ses 5 lignes) ; échantillon petit (36 mots) et page très
pâle (espaces et lettres incertains). À noter aussi : c'est le seul endroit où un léger signal de contacts apparaît (S6 : z = 1,8 à
distance 1 en K02 ; page 2 : z = 2,2 à distance 2, p = 0,015, exploration). **Piste, pas résultat** : la feuille 2 pourrait avoir été
produite ou habillée autrement.
