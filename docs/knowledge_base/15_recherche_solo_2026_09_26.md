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

## Prochaines hypothèses solo (file)
- S2 : alphabets « géométriques » dérivés du **tableau gravé lu par routes** (colonnes, diagonales, spirale, boustrophédon)
  comme σ et/ou τ — distinct de T1 (qui lisait les textes comme *clé courante*, pas comme *alphabet* de l'autoclé) et de
  H3 (mots-clés). Enumérable (quelques dizaines de routes) → test déterministe.
- S3 : remise en cause contrôlée d'une brique du squelette (ordre autoclé, sens, convention par ligne) sur les 97 lettres.
