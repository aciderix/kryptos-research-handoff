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
- I, B et D, B : voir `logs/null_*` (complétés en fin de run) ; valeurs disponibles au moment de la rédaction :
  I −12,56 / −12,58 ; D −12,28 … −12,45. Les réels I-B (−12,73) et D-B (−12,33) sont **dans** cette plage.

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
