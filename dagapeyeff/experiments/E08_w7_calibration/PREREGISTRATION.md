# E08 — Calibration de l'OBSERVATION « largeur 7 » (E04 § 6) — PRÉ-INSCRIPTION

Rédigée et commitée avant le calcul (2026-09-28). Aucun modèle de langue : statistique R seule (invariante par
substitution et par langue), `tools/e04_scan.c` (STAGE1ONLY=1).

## Observation à calibrer
Géométrie B, famille E (colonnaire standard), largeur 7 : R max réel = **46** (5 040 clés). Nulls déjà vus :
22-35 (10 mélanges totaux), ≤ 41 (36 mélanges totaux / par colonnes / par lignes). Cette observation a été
**remarquée après coup** parmi ≈ 60 cellules (E, I, D) : il faut une correction pour comparaisons multiples.

## Calculs (fermés)
1. **p local** : 2 000 mélanges totaux et 2 000 mélanges par colonne (composition des colonnes préservée) ;
   R max en largeur 7 (famille E, B) ; p = (1 + #{R_null ≥ 46}) / (N + 1).
2. **p global (look-elsewhere)** : 500 mélanges totaux ; pour chacun, le **plus grand écart standardisé**
   z = (R_max(W) − μ_W) / σ_W sur les familles E et I, largeurs 2-9, géométrie B (μ_W, σ_W estimés sur ces
   mêmes mélanges) ; comparé au z réel maximal. p_global = fraction des mélanges dont le z max ≥ z réel.
   (Largeurs 10-11 et famille D exclues pour le coût ; le choix est fait ici, avant le calcul.)

## Règle de décision
- p_global < 0,01 **et** p local (par colonnes) < 0,001 : « structure non aléatoire confirmée » (reste à
  l'expliquer ; ce n'est pas un déchiffrement).
- Sinon : « compatible avec le hasard après correction » ; l'observation est close.
