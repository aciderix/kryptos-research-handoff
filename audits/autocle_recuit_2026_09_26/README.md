# Audit « autocle_recuit » (26/09/2026) — autoclé écart-7 Vigenère, DEUX alphabets libres (T36b) sur les 97 lettres

**But.** Le seul coin non fermé du procédé désigné par le profil du « 7 » (base 09 §6 pt 1 ; §9 ; base 10 §I) est
l'autoclé sur le clair à l'écart 7, forme Vigenère, avec **deux alphabets libres indépendants** (clair σ, chiffré τ ;
T36b), jugé sur **les 97 lettres** (les 24 cribs seuls ne tranchent pas). Le prérequis bloquant : **réparer le contrôle
positif** de l'attaque par recuit (`../recuit_2026_09_24/`, qui échouait à retrouver un faux K4 fabriqué), AVANT tout
essai sur K4. Cet audit reprend le problème pour CE modèle précis (l'ancien recuit visait des modèles différents :
clé période-7 à mot-clé, clé courante libre).

| Fichier | Rôle |
|---|---|
| `ac7.c` | moteur de référence : chiffrement (fabrique de contrôles), déchiffrement, `selftest`, `score`, recuit conjoint (σ,τ,κ), sonde d'identifiabilité (graine depuis la vérité) |
| `ac7b.c` | attaque rapide : recherche sur (τ,κ) seuls, **σ dérivée par vote des cribs** (σ n'entre pas dans la récurrence), `tune_free` périodique ; graine ancrée-cribs par env `SEED_TAU/SEED_KAPPA` |
| `t36b_recover.py` | ancrage-cribs **exact** : CP-SAT sur les 17 équations de chaîne → (σ_crib, τ) ; puis recuit sur les lettres libres. Mesure la taille de la variété crib-consistante et son pouvoir discriminant |
| `mkplain.py` | segments anglais de 97 lettres (corpus Gutenberg) pour fabriquer les contrôles positifs |
| `witnesses.py` | témoins « K4 mélangé » (permutations aléatoires des 97 lettres de K4, fréquences conservées) |

Corpus/quadrigrammes : `../erreurs_multiples_2026_09_24/corpus_get.sh` (Gutenberg) + `../recuit_2026_09_24/build_qg.c`
(15,5 M quadrigrammes, plancher −9,19).

## 1. Le modèle et sa réduction

VIG, deux codages : `τ(c_i) = σ(p_i) + σ(k_i)` avec `k_i = p_{i−7}` (i ≥ 7) ou l'amorce (i < 7).
**Réduction clé (vérifiée)** : σ **n'entre pas** dans la récurrence. En posant `x_i = σ(p_i)` :

    x_i = τ(c_i) − x_{i−7}   (i ≥ 7),   x_i = τ(c_i) − κ_i   (i < 7),   p_i = σ⁻¹(x_i).

La suite `x` ne dépend que de **(τ, κ)** ; σ n'est qu'un **ré-étiquetage monoalphabétique final**, fixé par les cribs
(vote) et l'anglais. De plus, une fois τ fixé, **κ est déterminé par les cribs** (la récurrence propage l'amorce jusqu'aux
cribs). Round-trip validé (`ac7 selftest` OK ; `decrypt_full` avec les vrais paramètres → 97/97, κ exact).

## 2. Ce qui est établi (contrôle positif fabriqué : anglais + cribs, σ,τ,κ aléatoires ; cible qoff = −3,93)

1. **Identifiabilité — OK.** En partant de la VRAIE solution (basse température), la recherche y **reste** :
   **96/97 lettres** retrouvées, qoff −3,92. Le vrai clair est un **optimum local fort**, et son score hors-cribs
   (−3,93) est **nettement séparé** des solutions crib-consistantes aléatoires (−7,1 à −7,6). **Les 97 lettres
   discriminent** la vraie solution — contrairement aux 24 cribs seuls (T36b : indécidable).

2. **Recuit à froid — échoue.** SA conjointe (σ,τ,κ) 4 M×80 : 11/24 cribs, qoff −4,99, 15/97. Vote-fit sur (τ,κ),
   12 M itérations : 16/24 cribs, qoff −6,1, 20/97. Charabia dans les deux cas. L'espace à deux permutations 26! est
   trop rugueux (cohérent avec base 10 §G : le recuit libre a déjà échoué à son contrôle).

3. **Ancrage-cribs exact (CP-SAT) — les cribs sont satisfaisables, mais insuffisants.** CP-SAT résout les 17 équations
   de chaîne et fournit des (σ_crib, τ) donnant **24/24 cribs**. Mais 8 solutions tirées sont **toutes du charabia
   hors-cribs** (qoff −7,1 à −7,6 ; 28–31/97), et le recuit sur les lettres libres (τ contraint fixé) ne les corrige
   pas : chaque solution est un point ~aléatoire d'une grande variété.

4. **Taille de la variété crib-consistante.** Les 17 équations portent sur 35 variables (13 σ_crib + 22 τ) ;
   **dimension du noyau = 18** sur Z2 et sur Z13 → ~2¹⁸·13¹⁸ ≈ **10²⁵ solutions linéaires**. [Comptage exact des
   solutions-permutations : voir §3.] À comparer à **T5** (`../moteur_2026_09_25/k4sa.c`), qui **réussit** son contrôle
   parce qu'avec **un seul** alphabet + clé positionnelle, les clés crib-exactes forment un ensemble **fini énumérable** ;
   on énumère puis on recuit. **Le second alphabet libre** transforme cet ensemble fini en variété de dimension 18.

## 3. Verdict du contrôle positif : **il ne passe pas** (les deux sous-cas)

Toutes les attaques échouent à retrouver le clair d'un faux K4 fabriqué, alors même que la vraie solution est
identifiable (§2 pt 1). Récapitulatif sur le contrôle (cible qoff ≈ −3,9 / −4,2 ; charabia ≈ −7 à −8) :

| Attaque | Cribs | qoff hors-cribs | Lettres retrouvées |
|---|---|---|---|
| SA conjointe (σ,τ,κ) à froid, 4 M×80 | 11/24 | −4,99 | 15/97 |
| Vote-fit sur (τ,κ) à froid, 12 M | 16/24 | −6,1 | 20/97 |
| CP-SAT (cribs exacts) + recuit lettres libres, 8 sol. | **24/24** | −7,1 à −7,6 | 28–31/97 |
| SA **ancrée-variété** (germe CP-SAT, Wc=25) | **24/24** | −7,3 à −7,8 | 26–31/97 |
| Mono-alphabet (tie) seedé CP-SAT, basse **et** haute T (10 germes) | 22–24/24 | −6,9 à −8,2 | 25–34/97 |

Aucune n'approche la cible. **Raison structurelle** (mesurée) : l'ensemble des alphabets compatibles avec les 24 cribs
n'est **pas fini** — c'est une **variété** de dimension **10** (un alphabet, tie) ou **18** (deux alphabets) au noyau des
17 équations de chaîne (sur Z2 et Z13), soit ~10¹² à ~10²⁵ solutions ; CP-SAT n'en énumère que ~10 à 40 par seconde,
sans fin. La vraie solution y est un point isolé, discriminable par les 97 lettres mais **introuvable** par recherche :
le gradient des quadrigrammes ne guide pas depuis un point charabia de la variété vers l'aiguille anglaise.

**Contraste avec T5** (`../moteur_2026_09_25/k4sa.c`, qui **réussit** son contrôle) : là, **un seul** alphabet + une clé
**positionnelle** (période 7 + décalage par ligne) donnent des clés crib-exactes en nombre **fini**, qu'on énumère puis
qu'on recuit. Ici, la clé **est le clair** (auto-référence) : les cribs ne contraignent l'alphabet que partiellement et
laissent une variété. Le levier d'énumération finie de T5 **disparaît** dès que l'alphabet est libre et la clé
auto-référente. C'est aussi ce qui distingue ce coin des familles déjà éliminées avec les alphabets **à mot-clé**
(T27, T34 : l'espace des mots-clés est petit et énumérable).

Ce n'est donc **pas** une indécidabilité au sens de l'information (les 97 lettres contiennent la réponse), mais une
**intractabilité de recherche** : la variété crib-consistante est trop grande pour toute méthode essayée (recuit à
froid, énumération CP-SAT, recuit ancré-variété). C'est la mesure directe et la cause de l'échec déjà constaté par
`../recuit_2026_09_24/` et par base 10 §G.

## 4. Conséquence pour K4

Le contrôle positif **n'étant pas franchi**, **aucun essai sur K4 n'est admissible** (un échec sur K4 ne voudrait rien
dire). Le coin ouvert du procédé désigné par le profil du « 7 » — autoclé écart-7 Vigenère, alphabet(s) libre(s),
sous-cas (a) un alphabet + ≥1 erreur et (b) deux alphabets libres (T36b) — **reste ouvert**, et l'est pour une raison
maintenant **explicite** : sur les seules 97 lettres connues, il faudrait localiser une aiguille dans une variété
crib-consistante de dimension 10 à 18, ce qu'aucune recherche tractable ne fait. Il est réellement bloqué sur une
**donnée nouvelle** — le chiffré de **K5** (même rang pour BERLINCLOCK) ou un nouveau clair connu — cohérent avec T18
(éliminé sans erreur), T19 (non concluant à une erreur) et T36b (indécidable sur 24 lettres). L'outil `../k5_depth/`
reste le bon prochain pas, le jour où K5 paraît.

**Portée / limites.** Négatif de capacité, pas preuve d'impossibilité : une recherche plus fine (paramétrage explicite
du noyau, solveur dédié à l'autoclé, bien plus de calcul) pourrait un jour franchir le contrôle. Ce qui est établi :
les méthodes standard (recuit libre, énumération crib-exacte à la T5, recuit ancré-variété) ne le franchissent pas, et
la cause est la dimension de la variété crib-consistante induite par l'alphabet libre auto-référent.

