# 15 — Phase solo (chef seul, agent MÉCANISME en pause quota) : hypothèses neuves, 26/09/2026

**Cadre.** Sur ordre de l'utilisateur : ne pas s'arrêter, K5 **interdit** comme conclusion (l'artiste n'a pas conçu K4
pour être débloqué par K5 → K4 doit être soluble avec l'existant). On teste de vraies hypothèses neuves, en C, en
respectant : antériorité → **contrôle positif AVANT conclusion** → témoins K4-mélangé → tenue 1-2 erreurs. Ce document
accumule les résultats de la phase solo.

Contexte hérité (bases 13/14) : le seul mécanisme survivant pour l'excès à l'écart 7 est l'**autoclé sur le clair,
écart 7, Vigenère** ; le verrou résiduel est l'**alphabet**. L'agent MÉCANISME a fermé (contrôlé) : alphabet **proche
d'une même base connue** (A–Z, KRYPTOS, miroir, PALIMPSEST, ABSCISSA), en mode σ=τ (tie) et deux alphabets proches de
A–Z, ≤4 erreurs. Reste ouvert : des paires de bases **différentes**, et des alphabets non dérivés d'un mot-clé.

---

## S1 — Paires d'alphabets « proches de deux bases connues DIFFÉRENTES » (cross-base) — FERMÉ (contrôlé)

**Idée neuve (jamais croisée).** Bean/Materna mesurent que l'alphabet **chiffré** est proche de A–Z. L'agent a testé
σ=τ. Mais une structure **Quagmire-II** (clair à mot-clé, chiffré A–Z) donne σ ≠ τ, chacun proche d'une base
**différente**. Jamais testé sous l'autoclé écart-7.

**Portée.** Autoclé écart-7 Vigenère (modèle T36b, réduction de `ac7.c`). σ ~ base_S ± k swaps, τ ~ base_T ± k swaps,
pour toutes les paires ordonnées de bases ∈ {A–Z, KRYPTOS, PALIMPSEST, ABSCISSA, KRYPTOSABSCISSA, A–Z inversé,
KRYPTOS miroir}. κ (amorce) **entièrement déterminé par les cribs** (les 7 classes mod 7 contiennent toutes ≥1 crib),
donc chaque (σ,τ) → clair déterministe + nombre d'erreurs exact. Outil : `audits/crossbase_2026_09_26/sweep.c`.

**Contrôle positif : PASSE.** Un faux K4 fabriqué avec σ≈KRYPTOS(+1 swap), τ≈A–Z(+1 swap) est **retrouvé exactement**
(0 erreur, clair anglais + cribs restitués) par le balayage. Le moteur voit la solution quand elle existe.

**K4 réel : 0 survivant** sur **5 207 524** paires testées (k≤1 de chaque côté), à tolérance **2 ET 4 erreurs**.

**Verdict : FERMÉ (contrôlé).** Sous l'autoclé écart-7, l'alphabet de K4 n'est proche d'aucune paire (même croisée) des
bases connues de l'œuvre. Combiné à la fermeture « même base » de l'agent, **toute la classe « alphabet(s) proche(s)
d'un alphabet connu » est éliminée** — le contrôle positif garantit que ce négatif est concluant (contrairement au cas
« alphabet libre » qui, lui, est intractable, pas éliminé).

**Lecture.** Si l'autoclé écart-7 est bien le squelette, son alphabet n'est **ni** un dérivé de mot-clé **ni** une petite
perturbation d'un alphabet connu : il est « loin » de tout ce qu'on lit sur l'œuvre. Cela oriente vers (a) un alphabet
défini par un **objet physique / ordre spatial** (pochoir, ordre des perforations — non dérivable d'un texte), ou (b) une
remise en cause d'une brique du squelette (à instruire en phase solo).

## S2 — Alphabets STRUCTURÉS/mémorisables (affine, affine∘keyed) — FERMÉ (contrôlé)

**Idée.** Entre « mot-clé » (éliminé) et « proche d'A–Z » (fermé S1) et « libre » (intractable), il reste des alphabets
**structurés et mémorisables** (contrainte « papier-crayon » de Scheidt) jamais testés comme alphabet de l'autoclé : les
**affines** (x→a·x+b, a coprime à 26 : 12×26=312 chacun) et les **affine∘keyée** (KRYPTOS/PALIMPSEST/ABSCISSA puis
affine). L'affine standalone est éliminé, mais **pas** comme alphabet interne de l'autoclé écart-7.

**Portée & outil.** `audits/crossbase_2026_09_26/affine.c` (σ,τ affines) et `sweep2.c` (σ,τ = affine∘base keyée, bases
{A–Z, KRYPTOS, PALIMPSEST, ABSCISSA} — subsume affine pur et keyed pur). κ dérivé des cribs, filtre exact.

**Contrôle positif : PASSE** (une paire fabriquée affine, et affine∘keyée, est retrouvée 97/97, 0 erreur).

**K4 réel : 0 survivant** — affine σ,τ : 0/97 344 ; affine∘keyée σ,τ : 0/1 557 504 ; aux tolérances **2 ET 4 erreurs**.

**Verdict : FERMÉ (contrôlé).**

## Bilan intermédiaire de la phase solo
Sous l'autoclé écart-7 Vigenère, l'alphabet de K4 n'est : ni proche d'un alphabet connu (S1, même base ET croisé), ni
affine, ni affine∘keyée (S2) — **tous contrôlés**. Le cas « alphabet libre » reste, lui, **intractable** (base 13), pas
éliminé. Donc : soit l'alphabet est **arbitraire/externe** (pochoir — l'utilisateur refuse cette issue « donnée
externe »), soit **une brique du squelette autoclé-écart-7-Vigenère est à réinterroger** → priorité S3.

## S3 — Réinterroger le SQUELETTE (toutes conventions, clé clair/chiffré) — FERMÉ (contrôlé)

**Idée.** Ne pas tenir pour acquis « autoclé écart-7 **Vigenère sur le clair** ». Moteur générique `s3.c` :
convention ∈ {Vigenère, Beaufort, Variante} × source de clé ∈ {clair, chiffré}, écart 7, σ,τ = affine∘keyée,
κ dérivé par chaîne (bruteforce 26 → robuste, sans hypothèse de signe). Contrôle positif validé sur les 6 configs
(clé-clair : 97/97 ; clé-chiffré : 90/97 + 0 erreur de crib, les 7 manquants = amorce non contrainte, normal).

**K4 réel : 0 survivant / 9 345 024 configurations**, à ≤2 **et** ≤4 erreurs.

**Verdict : FERMÉ (contrôlé).** Aucun autoclé écart-7 standard (toute convention, clé sur clair ou chiffré) avec un
alphabet **structuré** (proche-connu, affine, affine∘keyée) ne produit les cribs de K4.

## Bilan de la phase solo (S1+S2+S3) et conclusion honnête

Sous le squelette « écart-7 auto-référent », **tout alphabet non arbitraire est éliminé, contrôlé** (S1 proche-connu,
S2 structuré, S3 toutes conventions/sources). Les seuls espaces restants sont **prouvés indécidables avec 24 lettres**
(alphabet libre : variété dim 19, base 13 ; Quagmire à période moyenne, base 02 §3) — ce n'est pas un manque d'effort
mais un **mur informationnel** : les 97 lettres + 24 cribs ne contiennent pas de quoi fixer un alphabet arbitraire.

**Recadrage (aligné sur « comprendre la pièce comme un tableau »).** Si K4 est soluble **sans K5**, la contrainte
manquante — l'alphabet — doit venir d'une **feature VISIBLE de l'œuvre**, pas d'un texte. Le candidat le plus fort
n'est pas un mot-clé (tous éliminés) mais l'**ordre spatial des lettres/perforations de l'écran** (un « pochoir » que
Sanborn a gravé) : c'est « ce qu'on voit », pas une donnée future comme K5. Le blocage concret devient alors **une
donnée mesurable, pas scellée** : un relevé/photo haute résolution de la géométrie de l'écran K4 (jamais mesurée,
base 02 §3.1) permettrait de dériver l'alphabet candidat et de le tester déterministiquement (moteur prêt : `crossbase/`).

## Prochaines hypothèses solo (file)
- **S3 (priorité)** : remise en cause CONTRÔLÉE du squelette. Tests déterministes via cribs, sans dégénérescence
  d'alphabet libre : (a) autoclé sur le CHIFFRÉ écart-7 avec alphabets structurés (affine/keyed) — l'autoclé-chiffré
  était éliminée pour A–Z/KRYPTOS, pas pour affine ; (b) conventions Beaufort/variante **restreintes aux alphabets
  structurés** (le profil désigne Vigenère mais la vérification directe des cribs est gratuite) ; (c) lag ≠ 7 avec
  alphabet structuré (contrôle de cohérence).
- **Interprétatif** : re-examiner les invariants durs (auto-chiffrements 32→S, 73→K ; égalité de Bean P en 27 et 65)
  comme contraintes directes sur (σ,τ) structurés, pour voir s'ils forcent une structure lisible.
