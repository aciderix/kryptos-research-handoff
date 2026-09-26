# 14 — Comprendre la pièce, pas casser la serrure : ce que le « 7 » est vraiment (26/09/2026)

**Nature.** Analyse **conjointe** chef ↔ agent MÉCANISME (échange Mesh du 26/09), en réponse à la consigne : cesser de
« casser la serrure » et **comprendre** la pièce. Aucune solution, aucun clair. Méthode : une hypothèse a été posée
(lecture spatiale/optique), **testée**, **falsifiée par la mesure**, et remplacée par une conclusion plus sobre et plus
solide. Ce document remplace la piste « appareil physique » que la relecture avait laissée ouverte.

---

## 1. La question de départ (regard global)

Sanborn est un **sculpteur** dont la signature est de **projeter un texte codé perforé avec une lumière ponctuelle**
(*Cyrillic Projector*, *Code Room*, *Lux*, *Radiance*, *Antipodes*…) ; NPR 1999 : K4 utiliserait des « systèmes
**spatiaux, de lumière et d'ombre**, absents de K1–K3 ». Scheidt, lui, dit **classique, papier-crayon**, « **plus d'une
étape** ». Hypothèse initiale (chef) : et si le « 7 » était l'empreinte d'une **manipulation physique** (largeur-7,
lumière/ombre) plutôt qu'une clé ?

## 2. Le fait structurant : les deux moitiés du « 7 » sont indépendantes — statistiquement ET géométriquement

Le seul signal robuste de K4 a **deux moitiés** :
1. l'**excès à l'écart 7** (9 coïncidences c[i]=c[i+7] pour 3,3 attendues) ;
2. la **concentration des doublets** (5 des 6 doublets ≡ 4 mod 7).

- **Indépendance statistique** : corrélation < 0,02 (base 09 §4).
- **Indépendance géométrique** (mesuré, sous-question du 26/09) : l'excès écart-7 est **réparti uniformément** sur les 7
  colonnes (2,1,2,1,1,1,1) ; les doublets sont **localisés en colonne 4** (5/6 : positions 18, 25, 32, 46, 67 ; le 6ᵉ en
  colonne 0, position 42). **Un seul appareil de largeur 7 lierait les deux aux mêmes colonnes.** Ils vivent sur des
  géométries distinctes ⇒ **deux phénomènes de natures différentes**, ce qui est « plus d'une étape ».

## 3. Mesures sur le chiffré seul (agent MÉCANISME, aucune clé, témoins « K4 mélangé »)

- **Écart-k, k=1..14 : seul l'écart 7 déborde** (K4=9, P≈0,004). 14 (P=0,81), 21 (P=0,52) au hasard ⇒ **voisin immédiat
  seulement**, donc **pas de pli global ni de superposition de rangées éloignées**.
- **Largeur 7 : adjacence verticale (+7) déborde ; diagonales (+6/+8) au hasard** ; largeurs 5,6,8,9,10,14,21 : rien.
  ⇒ largeur 7 et vertical spécifiquement.

## 4. L'hypothèse « appareil physique » est FALSIFIÉE

On a étendu le simulateur (base 10 F) avec un procédé à **deux étapes** : étape 1 = autoclé écart-7 (Vigenère) ;
étape 2 = substitution à modificateur de voisin `c[i] = combine(x[i], x[i±k])`, testée en **vertical** (k=7) **et
horizontal** (k=1), formes additive et Beaufort. Critère = reproduire la **signature jointe** (écart7 ≥ 9 **ET** ≥ 5
doublets **ET** concentration ≥ 0,8), 8000 essais par famille. **Contrôle positif du détecteur** : en forçant des
doublets en colonne 4, le détecteur mesure joint = 0,128 et conc = 0,66 (vs 0,50) — il **voit** une concentration quand
elle existe.

| Famille | joint | écart7_moy | conc_moy |
|---|---|---|---|
| Témoins K4 mélangé | 0,0000 | 3,31 | 0,50 |
| **Étape 1 seule (autoclé écart-7)** | 0,0000 | **7,30** | 0,48 |
| 2 ét. vertical additif | 0,0000 | 3,40 | 0,51 |
| 2 ét. vertical Beaufort | 0,0000 | 4,98 | 0,55 |
| 2 ét. horizontal additif | 0,0000 | 5,77 | 0,47 |
| 2 ét. horizontal Beaufort | 0,0000 | 2,38 | 0,48 |

**Deux raisons structurelles :**
1. Une étape-2 **additive à l'écart 7 détruit l'excès** de l'étape 1 : composer deux autoclés écart-7 rend c[i]=c[i+7]
   équivalent à y[i−7]=y[i+7] (répétition écart-14 = hasard). Maths et données concordent (l'excès tombe de 7,3 à 3,4).
2. **Mismatch mécanique** : les doublets sont une adjacence **horizontale** (c[i]=c[i+1]) localisée à une **colonne** ;
   une opération **verticale** (r↔r+1) agit sur les relations verticales, pas sur des doublets horizontaux.

**Résultat : aucune opération positionnelle** — verticale ou horizontale, largeur-7 ou voisin — **ne concentre les
doublets** (conc reste ≈ 0,50). Seule une **main forcée** le fait (contrôle positif). L'idée d'un **appareil physique**
largeur-7 (ma piste initiale) est **falsifiée** : elle serait reproductible et concentrerait par un mécanisme ; elle ne
le fait pas, et la colonne 4 n'est pas « l'endroit d'un appareil » (sinon l'écart-7 y serait aussi localisé — il ne l'est
pas). La colonne 4 est en partie alignée sur les cribs (positions 25, 32, 67 = lettres de crib N, S, I ; 18 et 46 hors
crib).

## 5. La conclusion (convergence finale)

Le « 7 » est **deux choses de natures différentes**, et une seule est un mécanisme :

- **L'excès à l'écart 7 = mécanique, lettre-à-lettre : une autoclé Vigenère à l'écart 7** (le « système classique »
  papier-crayon de Scheidt). C'est le **seul** procédé qui le reproduit (7,3 vs 3,3). Solide. Reste **intractable** sur
  les seules 97 lettres connues, alphabet libre (base 13) — d'où le rôle de K5, ou d'une contrainte documentaire (§6).
- **La concentration des doublets ≠ mécanisme.** Aucun procédé positionnel testé ne la produit. Les **trois** explications
  de la base 09 §6 se réduisent donc à **deux** :
  - **(a) le hasard** — signal faible (p ≈ 0,02 corrigé, 5/6 sur seulement 6 doublets) ;
  - **(b) un geste manuel de Sanborn** — quelques lettres choisies en regardant le clair pour faire un motif ≡ 4 mod 7,
    soit « **I fucked with it** » au sens **littéral** : la main de l'auteur *dans* le chiffré, une marque d'auteur, **pas
    une serrure** ni un appareil reproductible.

**Lecture « comme un tableau » :** une moitié du « 7 » est du **chiffre** (l'autoclé), l'autre moitié est
vraisemblablement **la main de l'artiste**. Le « spatial/lumière » de Sanborn colore peut-être l'atelier ou la mise en
page, mais **la signature des doublets ne porte aucune empreinte d'appareil physique**.

## 6. Le « 7 » comme longueur de clé — et un levier documentaire gratuit (ouvert)

Recoupement neuf des deux agents, convergé indépendamment :
- Un autoclé de décalage L produit son excès **pile à l'écart L** (vérifié L = 5..9). L'écart observé **= 7 = |KRYPTOS|**
  (7 lettres). Cela relie le **seul mécanisme survivant** à « **la clé la plus évidente de la sculpture, personne ne l'a
  remarquée** » (Sanborn, WSJ 2005) et à « **KRYPTOS joue un rôle intégral** » (NSA 1992).
- **Antériorité stricte** : une clé **périodique**-7 KRYPTOS est **éliminée** (T22, `motcle_pas7`). L'**autoclé** écart-7
  est le survivant. « 7 = |KRYPTOS| » est donc une **réinterprétation du survivant**, pas une résurrection de l'éliminé.
- **Levier actionnable (en cours d'évaluation)** : l'autoclé écart-7 a aussi une **amorce de 7 lettres**
  (keystream[0..6] = clé externe). La base 13 montre que le coin est intractable parce que l'alphabet libre laisse une
  **variété de dimension 10/18**. **Fixer l'amorce = KRYPTOS** ajoute 7 équations : si cela fait tomber la dimension au
  point de rendre la variété **énumérable**, c'est la « contrainte manquante » qu'on attribuait à K5 — mais **gratuite,
  tirée de la sculpture**. Mesure demandée à l'agent MÉCANISME. Variantes d'amorce : PALIMPSEST/ABSCISSA tronqués à 7.

## 7. Garde-fous (honnêteté)

1. **Signal modeste** : écart-7 P ≈ 0,004 brut, ≈ 2,6 σ après look-elsewhere ; concentration p ≈ 0,02 sur 6 doublets.
   Rien de décisif. On bâtit peu sur peu.
2. **Négatif de capacité, pas d'impossibilité** pour l'étape 1 (base 13).
3. **Pas d'objet largeur-7 attesté** : la piste physique reposait sur une largeur inférée du chiffré ; elle est
   maintenant falsifiée comme mécanisme, indépendamment de cette faiblesse.

## 8. Ce que ça change

- La piste « lecture optique / appareil largeur-7 » est **close** (falsifiée), ce qui évite à la communauté et à nous de
  la rechasser.
- Le seul levier de calcul restant qui **ne dépend pas de K5** est le §6 : **amorce KRYPTOS pour l'autoclé écart-7 à
  alphabet libre**. S'il n'énumère pas, on revient au diagnostic des bases 09/13 (K5 / 8 signes / méthode Paradigm).
- Le fait structurant à retenir : les deux moitiés du « 7 » sont indépendantes (stat. **et** géométriquement) — l'une est
  un mécanisme (autoclé), l'autre est la main de l'auteur ou le hasard.

*(Base 14. Analyse conjointe chef ↔ MÉCANISME, branche `claude/zealous-cerf-o7d96b`. Mesures, simulations et test =
agent MÉCANISME (`audits/autocle_recuit_2026_09_26/` + sim2step). Rédaction et arbitrage = chef.)*
