# E04 — RÉSULTATS (2026-09-28) : **NÉGATIF** sur les 6 cellules

Pré-inscription : `PREREGISTRATION.md` (+ amendement 1, avant les runs réels I et D). Outil : `tools/e04_scan.c`.
Journaux : `logs/`.

## 1. Puissance (contrôles, RÉSULTAT)
| Famille | Espace balayé exhaustivement | Géométrie B (182) | Géométrie A (196) |
|---|---|---|---|
| E colonnaire standard | toutes les clés, largeurs 2-11, grilles incomplètes (≈ 4,4·10⁷) | **9/10** | **4/5** |
| I colonnaire inverse | idem | **10/10** (carré retrouvé) | **5/5** |
| D double colonnaire | toutes les paires de clés, largeurs 2-7 × 2-7 (≈ 3,5·10⁷) | **8/10** | **5/5** |
qoff des contrôles récupérés : −8,9 … −10,1.

## 2. Vrai chiffré (RÉSULTAT)
| Famille | Géométrie | Meilleur qoff | Largeur(s) | R (statistique invariante) de la clé gagnante |
|---|---|---|---|---|
| E | B | −12,385 | 5 | 11 |
| E | A | −12,704 | 5 | 11 |
| I | B | −12,731 | 7 | 13 |
| I | A | −12,985 | 6 | 4 |
| D | B | −12,332 | 7 puis 5 | 40 |
| D | A | −12,630 | 3 puis 5 | 19 |
Ré-enchiffrement exact partout (trivial sans les autres critères). Aucun clair lisible.

## 3. Null (RÉSULTAT)
- E, B : 10 mélanges, pipeline complet : −12,095 … −12,567 (max **−12,095**). Réel −12,385 : **sous le max**.
- I, B : 10 mélanges : −12,431 … −12,813 (max **−12,431**). Réel −12,731 : **sous le max**.
- D, B : 10 mélanges : −12,085 … −12,445 (max **−12,085**). Réel −12,332 : **sous le max**.

## 4. Critères
Critère 1 exige qoff ≥ min(contrôles récupérés) − 0,5 ≈ −10,6 **et** > max(null) + 0,5 : **aucune** cellule ne
s'en approche (écart ≈ 2 unités). Critères 2-5 non atteints.

## 5. Conclusion
- **RÉSULTAT** : aucune transposition colonnaire (standard ou inverse, grille complète ou incomplète, largeurs
  2 à 11) ni double colonnaire (largeurs 2 à 7 × 2 à 7), suivie d'un carré de Polybe, ne produit d'anglais — que
  la colonne 14 soit comprise comme nulles (B) ou comme message (A) — à la puissance mesurée (80-100 %).
- **Contiguïté** (OBSERVATION chiffrée) : les valeurs R maximales des familles I (≤ 22) montrent qu'aucune
  lecture par colonnes d'une grille de largeur 2-11 ne contient de segments anglais contigus.
- Couverture cumulée E01-E04 (avec contrôles et null) : routes 14×14 / 14×13 (64 × 2 sens), colonnaire à clé
  libre largeurs 7/13/14 (grilles complètes), toutes colonnaires largeurs 2-11 (deux sens), double colonnaire
  ≤ 7 × 7, colonnaire largeur 14 à bourrage 6/8.
- Restent (non couverts) : double transposition à grandes largeurs, colonnaire + route, Myszkowski, colonnaire
  interrompue, grille tournante 14×14, nulles ailleurs qu'en colonne 14, langue autre que l'anglais, **erreur
  d'enchiffrement**. Limite méthodologique : sur 182 lettres, la statistique invariante ne départage plus des
  espaces de clés au-delà d'environ 10¹⁰ (cf. E03) ; ces familles exigent le solveur joint clé + carré.

## 6. OBSERVATION indépendante de la langue : excès de répétitions en largeur 7 (non expliqué)
L'étage 1 (R, invariant par substitution **et** par langue) a été comparé largeur par largeur au null :
- **E, B, largeur 7** : R max réel **46** ; null E04 (10 mélanges) 22-35 ; 36 mélanges supplémentaires
  (totaux, par colonnes, par lignes : composition des colonnes/lignes préservée) : max 41. Moyenne sur les
  5 040 clés : 9,2 (σ 4,3).
- Meilleure clé `7 3 6 1 4 2 5` : deux **6-grammes** répétés, `85 74 91 82 81 64` (écart 60) et
  `62 75 82 81 62 81` (écart 41), chacun formé de cases d'une **même colonne imprimée** lues dans l'ordre de
  lignes 7, 11, 3, 9, 13, 5, 1 (lignes impaires) puis 8, 12, 4, 10, 14, 6, 2 (paires).
- Excès aussi en D pour une largeur interne 7 (W2 = 4 : 52 vs 36-43 ; W2 = 5 : 52 vs 40-51 ; W2 = 7 : 61 vs
  57-58). Attendu sous H0 : ≈ 1/(n+1) dépassement par cellule ; observé 2/10 (E), 10/36 (D), 1/10 (I).
- L'anglais ne l'explique pas (clé imposée, 60 départs : −12,80). Un recuit sur 14! ordres de lignes crée autant
  de répétitions sur des mélanges ⇒ seule la famille **exhaustive et petite** (5 040) est informative.
- Statut initial : OBSERVATION jugée significative sur 10-40 mélanges. **Mise à jour (E08)** : avec 2 000
  mélanges, p local = 0,006-0,0095 et p global (16 cellules) = 0,11 ⇒ **compatible avec le hasard**, close.
