# E10 — Empreinte d'un carré « mot-clé + alphabet » dans les fréquences (indépendante de la transposition) — PRÉ-INSCRIPTION

Rédigée et commitée avant le calcul (2026-09-28).

## Source primaire (nouvelle)
d'Agapeyeff, *Codes and Ciphers* (scan archive.org `codesciphers0000daga`, fourni par le user ; **non versé au
dépôt**) : le défi p. 158 est identique, chiffre pour chiffre, à `data/ciphertext_1939.txt` (395/395). Dans tous
ses exemples, le carré de Polybe est construit **mot-clé d'abord (lettres répétées omises), puis le reste de
l'alphabet dans l'ordre**, J omis ou fusionné avec I : CLIQUE (p. 123), MANCHESTER (p. 117-118, 124). Nulles
explicitement enseignées (p. 51 « abcd » pour compléter le carré ; p. 118 « EE = Z = nulls to finish » ; p. 125
« ‘D’ is a ‘null’ to complete the square »).

## Hypothèse H_K
Le carré du défi est un carré à mot-clé : 25 lettres (I = J) dans l'ordre « lettres du mot-clé, puis lettres
restantes par ordre alphabétique », rangées dans les 25 cases selon une orientation parmi 8 (remplissage par
lignes ou par colonnes × ordre des lignes 6-7-8-9-0 ou inverse × colonnes 1-5 ou inverse).

## Statistique (invariante par transposition)
Comptes par case n_c (géométrie A : 196 paires ; B : 182, colonne 14 exclue). LL(σ) = Σ_c n_c · ln p(σ(c)),
p = fréquences des lettres anglaises (corpus `qg_en`, I = J). LL_max = meilleur σ quelconque (appariement par
rang). Pour H_K : LL_K = max sur les 2²⁵ ensembles de lettres du mot-clé (ordre des lettres du mot-clé
optimisé par rang, queue alphabétique imposée) et sur les 8 orientations. **Δ = LL_max − LL_K** (coût de la
contrainte « mot-clé »).

## Contrôles (même calcul)
- **Positifs** : 500 textes anglais de même longueur (*Alice*), chiffrés avec un carré à mot-clé (mot tiré de la
  liste des mots du livre, ≥ 5 lettres distinctes, orientation aléatoire) → distribution de Δ sous H_K.
- **Négatifs** : 500 textes chiffrés avec un carré **quelconque** → distribution de Δ sous non-H_K.
- Idem en français, allemand, italien, espagnol, latin, néerlandais, espéranto (200 textes chacun) pour
  vérifier que la conclusion ne dépend pas de l'anglais (fréquences propres à chaque langue).

## Décision
- Δ réel dans la distribution positive (≤ quantile 95 %) **et** hors de la négative (≤ quantile 5 %) :
  « compatible avec un carré à mot-clé, et discriminant » → on retient les **carrés candidats** (meilleurs
  ensembles/mots) pour E11 (recherche de transposition à carré fixé).
- Δ réel au-delà du quantile 99 % positif : H_K **défavorisée** (le carré n'est probablement pas à mot-clé, ou le
  clair n'est pas anglais / la colonne 14 ou d'autres nulles faussent les comptes).
- Sinon : non concluant. Dans tous les cas, **aucune** revendication de déchiffrement (les fréquences seules ne
  lisent rien).

## Liste de mots-clés (pour le classement des carrés candidats)
Tous les mots distincts (≥ 4 lettres) du texte du livre (OCR du scan), plus les mots-clés de ses exemples
(CLIQUE, MANCHESTER, SCHUVALOF, MONTH, CID) ; chaque mot donne un carré par orientation ; classement par LL.
