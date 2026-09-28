# E15 — « 13 classes » + double transposition colonnaire : attaque complète en deux étages — PRÉ-INSCRIPTION

Rédigée et commitée avant tout run de E15 sur le vrai chiffré (2026-09-28).

## Contexte
E13 : l'hypothèse H13 (anglais, lettres fusionnées en 13 classes équilibrées ; exige une transposition) est
compatible avec le profil. E14 : H13 + colonnaire simple (largeurs 2-11, deux sens) **exclue** ; H13 + double
colonnaire (2-7 × 2-7) **non concluant** (puissance du seul étage 1 : 73 %). E15 attaque ce dernier cas.

## Méthode (`tools/e04_scan.c`, FAM=D, PAIR=1)
1. **Étage 1** (exhaustif, invariant par les paires) : toutes les paires de clés, largeurs 2-7 × 2-7, géométrie B
   (182 symboles, colonne 14 exclue) ; on garde les **10** meilleures clés par couple de largeurs (TOP=10).
2. **Étage 2 « paires »** : pour chaque clé retenue, affectation lettres → 13 symboles (12 paires + 1 singleton)
   par recuit (échanges de lettres et de paires entières ; 6 départs × 150 000 itérations), score = meilleure
   lecture anglaise (Viterbi, 2 lettres par symbole, quadgrammes joints). On garde le meilleur qoff.
Mise au point (contrôles seuls) : sans transposition, 6/6 lus (171-178/182 lettres justes) ; colonnaire largeurs
5-9, TOP 10, 2/2 lus (178 et 168/182).

## Contrôles, null, critères
- Contrôles : 10 textes anglais réservés, fusion en 13 classes équilibrées (E ; TZ AQ OX IK NV SB HP RY DG LF CW
  UM), symboles aléatoires, double transposition aléatoire dans la famille ; succès si ≥ 80 % des lettres lues
  justes (décalage ≤ 16 toléré). Admissibilité ≥ 6/10.
- Null : 5 mélanges des 182 symboles réels, même pipeline.
- Succès (tous requis) : qoff > max(null) + 0,5 **et** ≥ min(qoff des contrôles lus) − 0,5 ; lecture anglaise
  continue ≥ 80 % (jugée après) ; stabilité (2ᵉ graine) ; ré-enchiffrement exact (clé + appariement → chiffré).
Sinon : **négatif** (H13 + double ≤ 7×7 exclue à la puissance mesurée).
- Exécution : `run_E15.sh` (autonome) ; contrôles possiblement confiés à l'autre agent.
