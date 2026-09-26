# 14 — Comprendre la pièce, pas casser la serrure : le « 7 » comme signature de DEUX étapes (26/09/2026)

**Nature de ce document.** Analyse **conjointe** chef ↔ agent MÉCANISME (échange Mesh du 26/09), en réponse à la
consigne : cesser de tester des familles (« casser la serrure ») et **comprendre** la pièce comme un tableau — chercher
un recoupement que ni nous ni la communauté n'aurions fait. Aucune solution, aucun clair. Ce n'est pas un test de clé :
c'est une **hypothèse de compréhension**, avec ses appuis, ses garde-fous et son point faible. Statut du test décisif :
**en cours** (§5).

---

## 1. Le point de départ (regard global)

Deux constats qu'on relie rarement :
- **Sanborn n'est pas cryptographe, c'est un sculpteur** dont la signature, sur toute son œuvre, est de **projeter un
  texte codé perforé avec une source de lumière ponctuelle** : *Cyrillic Projector*, *Code Room* / *Covert Obsolescence*
  (« Medusa »), *Lux*, *Radiance*, *Meridian*, *Antipodes*. NPR 1999 : K4 utilise des « systèmes **spatiaux, de lumière
  et d'ombre**, **absents de K1–K3** ». Big Techday 2013 : « on **retourne la feuille**, on la met à l'envers, on
  l'**éclaire** ».
- **Scheidt** insiste à l'inverse : système **classique, papier-crayon, résoluble à la main**. Et : « **plus d'une
  étape** » (2015).

La tension se résout si K4 est fait de **deux étapes de natures différentes** : une cryptographique (Scheidt) et une
**spatiale** (Sanborn, « **I fucked with it** »).

## 2. La clé de voûte : les deux moitiés du « 7 » sont INDÉPENDANTES

Le seul signal robuste de K4 est le « 7 », et il a **deux moitiés statistiquement indépendantes** (base 09 §4 ; base 04) :
1. l'**excès à l'écart 7** (9 coïncidences c[i]=c[i+7] pour 3,3 attendues) ;
2. les **doublets alignés** (5 des 6 doublets en position ≡ 4 mod 7).

Leur **indépendance** (corrélation < 0,02) est le levier : **une seule** opération produirait les deux de façon
**corrélée**. Leur indépendance **impose deux mécanismes distincts** — ce qui est exactement « plus d'une étape ».

## 3. Mesures géométriques sur le chiffré seul (agent MÉCANISME, sans aucune clé, 5000 témoins « K4 mélangé »)

Test purement géométrique (aucune hypothèse de clé) :
- **Écart-k, k = 1…14 : seul l'écart 7 déborde** (K4 = 9, P(témoin ≥ K4) = 0,004). Tous les autres au hasard, y compris
  14 (P = 0,81) et 21 (P = 0,52).
- **Sur largeur 7 : l'adjacence VERTICALE (+7) déborde** (P = 0,004) ; les **diagonales (+6/+8) sont au hasard**
  (P = 0,68). **Largeurs 5, 6, 8, 9, 10, 14, 21 : aucun débordement vertical** (P = 0,20–0,85).
- ⇒ **largeur 7 spécifiquement, vertical spécifiquement.**

Trois faits neufs pour une lecture physique :
- **(a) Voisin immédiat seulement.** L'effet est écart-7 pur ; écart-14/21 au hasard ⇒ **un pli en deux ou une
  superposition de rangées éloignées est EXCLU**. Ce qui survit = adjacence verticale **locale** (rang r ↔ r+1).
- **(b) Vertical, pas diagonal** ⇒ compatible avec une projection/superposition **orthogonale** sur grille étroite (une
  lumière traversant des perforations caste des ombres orthogonales ; un décalage oblique donnerait des diagonales — on
  n'en a aucune).
- **(c) Largeur 7 unique.**

## 4. L'hypothèse de compréhension (à deux étapes)

- **Étape 1 — lettre à lettre, papier-crayon (Scheidt).** Une **autoclé de type Vigenère à l'écart 7** : c'est le **seul**
  procédé qui rend naturel l'**excès à l'écart 7** (simulateur, base 10 F, §6). Identifiable mais intractable sur les
  seules 97 lettres connues (base 13) — d'où le rôle de K5.
- **Étape 2 — PAS lettre à lettre, spatiale (Sanborn).** Une **opération verticale locale de largeur 7** (rang r ↔ r+1)
  qui **concentre les doublets alignés**. Contrainte forte (garde-fou du compte des lettres, §6) : ce **n'est pas** un
  réarrangement (la transposition pure est exclue — les cribs demandent 3 E, K4 n'en a que 2) ; c'est une
  **substitution à modificateur vertical** : `c[i] = combine(x[i], x[i−7])`, où `x` est la sortie de l'étape 1. Modèles
  physiques : « bavure / mauvais recalage » de lumière entre perforations verticalement adjacentes ; superposition de
  deux moitiés de largeur 7 décalées d'un rang.

Ce schéma réconcilie **tout le corpus de paroles** : « plus d'une étape » (Scheidt), « systèmes spatiaux de lumière et
d'ombre absents de K1–K3 » (Sanborn NPR 1999), « on retourne la feuille et on l'éclaire » (2013), « I fucked with it »
(2025), et le recul de Sanborn sur le « 1:1 » (une étape non lettre-à-lettre casse la stricte correspondance).

## 5. Le test qui tranche (falsifiable) — EN COURS

Étendre le simulateur (base 10 F) avec le procédé à **deux étapes** ci-dessus et vérifier s'il reproduit la
**SIGNATURE JOINTE** que **aucun des 52 procédés déjà simulés ne reproduit** : excès à l'écart 7 **ET** concentration
des doublets dans **une seule paire de colonnes de 7** (colonnes 4–5, base 09 §7). Protocole : contrôle positif (faux K4
fabriqué avec étape 1 + étape 2, signature retrouvée) ; témoins « K4 mélangé » ; **look-elsewhere payé**.
- **Si oui** → l'hypothèse à deux étapes gagne un vrai appui de mécanisme (la première « classe de procédé » qui
  reproduit la signature complète de K4).
- **Si non** → l'autoclé reste seule à expliquer l'excès, et l'étape 2 spatiale ne tient pas.

*(Résultat à intégrer ici dès que l'agent MÉCANISME l'a produit.)*

## 6. Garde-fous (pour ne pas « se peindre un tableau »)

1. **Signal modeste.** P = 0,004 brut ; après paiement du choix (largeur × écart ≈ 100 tests) ≈ 10⁻² à quelques % —
   soit **≈ 2,6 σ**, **suggestif, pas décisif**. Même un succès au §5 = « **classe de mécanisme plausible** », **pas** une
   solution ni la clé.
2. **Compte des lettres.** L'étape 2 ne peut pas être un simple déplacement (transposition exclue) : elle **doit changer
   des lettres** (substitution à structure 2D).
3. **Objet largeur-7 jamais vu.** La seule feuille connue fait **31 colonnes** ; le « ? » n'est pas chiffré (98 = 14×7
   mort). La largeur 7 est **inférée du chiffré**, pas d'un objet physique attesté. **C'est le point faible testable de
   l'hypothèse** : elle prédit un support/gabarit de largeur 7 qu'on n'a jamais observé.
4. **Antériorité.** Le modèle de l'étape 2 (`c[i] = combine(x[i], x[i−7])`) **diffère** des familles éliminées voisines :
   « toute substitution qui dépend de i mod 7 » (éliminée par les paires 65/72) dépend de la **position**, pas de la
   **lettre voisine** ; « autoclé sur le chiffré à l'écart 7 » (éliminée par 22/72) est testée en **crib-fit**, ici on
   teste une **signature statistique** après une étape 1. À confirmer explicitement dans l'audit (§5).

## 7. Convergences faibles (notées, non probantes)

- **Les deux cribs nomment les deux ingrédients d'une lecture spatiale** : EASTNORTHEAST = une **direction** (= l'aiguille
  gravée de la rose, ENE, mesurée, 30 ans avant l'indice) ; BERLINCLOCK = une **horloge à champs lumineux** (lumière/temps).
  « La clé est l'algorithme… dissimulée sur la sculpture » (Scheidt) ; « la clé la plus évidente, personne ne l'a
  remarquée » (Sanborn 2005). Convergent avec §4 sans rien prouver.

## 8. Ce que ça change pour la suite

- Si le §5 réussit : la priorité n'est plus « quelle clé ? » mais « **quelle opération spatiale de largeur 7** » — et
  K5 sert à fixer l'étape 1 (autoclé) pendant que l'étape 2 est une classe géométrique restreinte.
- Si le §5 échoue : on revient au diagnostic des bases 09/13 (blocage documentaire, K5).
- Dans les deux cas, l'**indépendance des deux moitiés du « 7 »** (§2) reste le fait structurant à ne pas oublier.

*(Base 14. Analyse conjointe chef ↔ MÉCANISME, branche `claude/zealous-cerf-o7d96b`. Mesures géométriques et test §5 =
agent MÉCANISME. La base 09 sera mise à jour pour pointer vers 12, 13 et 14.)*
