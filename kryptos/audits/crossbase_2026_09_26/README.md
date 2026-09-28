# crossbase_2026_09_26 — autoclé écart-7 : σ et τ proches de DEUX bases connues différentes

Phase solo (chef), 26/09. Teste le trou jamais couvert : sous l'autoclé écart-7 Vigenère (modèle T36b de
`../autocle_recuit_2026_09_26/ac7.c`), σ ~ base_S ± k swaps et τ ~ base_T ± k swaps, avec base_S ≠ base_T possible
(structure Quagmire-II motivée par Bean : clair à mot-clé, chiffré A–Z).

Réduction : `x_i = tau[ct_i] - (i<7?kappa_i:x_{i-7})`, `pt_i = siginv[x_i]`. Les 7 classes mod 7 contiennent toutes un
crib → **κ entièrement déterminé par les cribs** pour chaque (σ,τ). Donc filtre par cribs exact, sans quadgrammes.

Bases : {A–Z, KRYPTOS, PALIMPSEST, ABSCISSA, KRYPTOSABSCISSA, A–Z inversé, KRYPTOS miroir}.

## Commandes
- `./sweep selftest` — contrôle du moteur (chiffre puis retrouve : 97/97, 0 erreur). **OK.**
- `./sweep gen bS sS bT sT seed` — fabrique un contrôle positif (CT).
- `./sweep run CT kSig kTau tol` — balaye les paires de bases, σ~base±k, τ~base±k, imprime les clairs à ≤ tol erreurs.

## Résultats (K4 réel)
- Contrôle positif croisé (σ≈KRYPTOS+1, τ≈A–Z+1) : **retrouvé exactement** (0 erreur).
- K4, cross-base k≤1, **tol 2 et 4** : **0 survivant / 5 207 524 paires**.
- Verdict : **FERMÉ (contrôlé)**. L'alphabet de K4 n'est proche d'aucune paire de bases connues. Détail : base 15 §S1.
