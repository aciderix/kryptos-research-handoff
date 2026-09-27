# Variété crib-EXACTE de l'autoclé écart-7 : test algébrique de bijectivité + énumération (MÉCA, 2026-09-27)

Fusion avec le tournant `qg_big` du chef (objectif décisif, mur = navigation). Au lieu de recuire
sur 26!×26⁷ à cribs SOFT (ac7f/ac7s : 20-22/24, charabia), on paramètre la **variété affine
crib-EXACTE** (24/24 cribs garantis par construction) et on répond à la question du chef
« pas d'anglais dans le modèle » vs « aiguille manquée » de façon **algébrique, pas par recherche**.

## Modèle
`x_i = tau(c_i) - k_i`, `k_i = kappa_{i%7}` (amorce de chaîne) puis `x_{i-7}` ; `pt_i = sig⁻¹(x_i)`.
Crib (pos i, lettre v) : `sig(v) = x_i` ⇒ équation **homogène** en (tau,kappa,sig), coeffs ±1.
- TIE  : sigma=tau=pi (Quagmire III), 33 variables.
- INDEP : sigma,tau libres (2 alphabets), 59 variables.

## Outils (C, dans tools de ce dossier)
- `variety_dim.c` (déjà présent) : dimension linéaire exacte. **INDEP dim=21, TIE dim=9** (mod2=mod13).
- `bijcheck.c` : **la variété contient-elle une PERMUTATION ?** `pi[a]=pi[b]` (a≠b) est IMPLIQUÉ par
  les cribs ssi ajouter `(e_a−e_b)` ne change ni rang mod2 ni rang mod13. Si UNE paire est forcée
  ⇒ aucune permutation ⇒ modèle impossible. Balaie drop-0/1/2 (régime erreur Sanborn). ~2,5 s.
- `tie_enum2.c` : **énumération EXACTE** de la variété TIE. RREF Z/26 à pivots unités (possible car
  rang2=rang13 ⇒ diviseurs élémentaires inversibles), pivots forcés sur kappa ⇒ 9 variables libres
  = lettres pival ⇒ DFS = énumération d'alphabet à valeurs distinctes (élagage bijection à chaque
  pas). Chaque feuille = permutation + kappa ⇒ 24/24 cribs, décodée et scorée `qg_big`.
  Contrôle positif : message anglais planté ⇒ eq_crib=OUI, decode=OUI, paramétrisation=OUI = **PASS**.

## Résultats (K4 réel, qg_big)

### TIE (σ=τ, alphabet LIBRE) — JAMAIS FAIT (le chef n'avait testé que des alphabets mot-clé VISIBLES)
- **drop 0 (24 cribs EXACTS) : les cribs FORCENT `pi[B]=pi[Z]` ET `pi[L]=pi[N]`.**
  Deux lettres distinctes du clair devraient partager une valeur ⇒ **AUCUNE permutation** ⇒
  **K4 n'est PAS un σ=τ Vigenère autoclé écart-7 à alphabet libre avec les 24 cribs exacts.**
  Preuve **algébrique** (rang), pas une recherche : définitif. (Confirmé indépendamment par
  l'énumérateur : 0 solution, arbre mort à la racine.)
- **drop 1** : retirer le crib en **pos 66** (le « L » de BER**L**IN) supprime LES DEUX collisions
  ⇒ permutations possibles à 23/24 cribs (régime « 1 erreur » évoqué par Sanborn).
  Énumération scorée `qg_big` : ≥ 2202 solutions bijectives distinctes, **meilleur qoff = −3.48**
  (seuil anglais ≥ −2.6) = **charabia**. (Run exhaustif en cours ; plateau net à −3.48.)
- drop 2 (pos 21,66) : permutations possibles aussi, même régime charabia.
⇒ **Famille σ=τ-autoclé-écart-7 (alphabet libre) : FERMÉE** — impossible en cribs exacts,
charabia sous 1 erreur.

### INDEP (2 alphabets libres)
- **drop 0 : 0 collision forcée (tau et sig).** Des permutations EXISTENT dans la variété dim-21.
- ⇒ **sous-détermination confirmée algébriquement** : le modèle 2-alphabets + 24 cribs publics
  laisse une variété de permutations de dimension 21 (≈ le résultat empirique du chef, ac7s :
  crib-cohérent atteignable et massivement charabia). L'énumération exhaustive est hors de portée
  (26²¹), mais la **non-vacuité** + la **haute dimension** disent que ce modèle **ne peut pas
  identifier un clair unique** à partir du public seul ⇒ soit ce n'est pas le mécanisme, soit il
  faut une contrainte externe (qui violerait « 100 % visible »). La recherche `qg_big` intérieure
  reste la seule voie si on garde ce modèle (fusion), mais l'objet est intrinsèquement ambigu ici.

## Portée
Élimine proprement (algébre + qg_big) le cas **σ=τ alphabet libre** (non couvert avant) et
**caractérise** l'ambiguïté du cas 2-alphabets. Reste offensif : cf. `bijcheck` généralisable à
d'autres écarts/formes (Beaufort, variante) et à des couches non-autoclé.
