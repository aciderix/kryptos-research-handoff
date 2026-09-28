# E04 — Balayage EXHAUSTIF de transpositions par statistique invariante + résolution du carré — PRÉ-INSCRIPTION

Rédigée avant tout run de E04 sur le vrai chiffré (2026-09-28). Réglages fixés sur contrôles synthétiques seuls.

## Motivation (OBSERVATIONS antérieures, sur le chiffré seul, sans hypothèse de mécanisme)
- Dans l'ordre imprimé, le chiffré a la contiguïté d'un texte **mélangé** : 5 trigrammes et 0 quadrigramme de
  symboles répétés (mélanges : 6,2 ± 2,6 et 0,5 ; anglais non transposé de 196 lettres : 28 ± 11 et 12,6 ± 10).
  ⇒ une transposition (ou équivalent) a eu lieu, qui détruit toute contiguïté.
- Le nombre de n-grammes répétés est **invariant par substitution** : on peut juger une transposition candidate
  sans résoudre le carré.
- E03 : la récurrence des clés B13 est un optimum de bigrammes (K1 : 13 trigrammes, 0 quadrigramme répétés),
  pas de l'anglais.

## Statut de nouveauté
- DÉJÀ TESTÉ par d'autres : colonnaire largeur 7 (5040 ordres) ; double transposition (GitHub, partielle, sans
  contrôle). Ici (E02, E03) : colonnaire grilles complètes largeurs 7, 13, 14.
- NOUVEAU : balayage **exhaustif** de toutes les clés colonnaires, grilles **incomplètes** comprises, largeurs
  2 à 11 (≈ 4,4·10⁷ clés par sens), dans les **deux sens** (standard et inverse), et de la **double** colonnaire
  (largeurs 2 à 7 × 2 à 7, ≈ 3,5·10⁷ couples), par une statistique indépendante du carré, avec contrôles et null.

## Représentation
Comme E01. Deux géométries : **A** (196 symboles) et **B** (colonne 14 retirée, 182).

## Mécanismes (fermés)
- **E** (colonnaire standard) : clair écrit par lignes de W (dernière ligne incomplète), colonnes lues dans l'ordre
  de la clé, concaténées ⇒ chiffré.
- **I** (inverse) : clair réparti dans les colonnes prises dans l'ordre de la clé (longueurs de la grille
  incomplète), chiffré lu par lignes.
- **D** (double) : E avec (W1, clé 1) puis E avec (W2, clé 2).
Puis substitution de Polybe (carré inconnu).

## Méthode (`tools/e04_scan.c`)
1. **Étage 1 (exhaustif)** : pour chaque largeur (ou couple), toutes les clés ; statistique
   R = rep3 + 2·rep4 + 3·rep5 (rep_n = Σ (occurrences − 1) des n-grammes de symboles répétés) ; on garde les
   TOP meilleures (TOP = 50 pour E et I, 20 par couple pour D).
2. **Étage 2** : pour chaque clé retenue, carré résolu à clé fixée (recuit quadgrammes joints, 8 départs
   indépendants × 40 000 itérations, score incrémental) ; on garde le meilleur qoff sur toute la famille.
3. Sortie : meilleur qoff, clé, clair ; ré-enchiffrement exact.

## Contrôles (avant le vrai chiffré)
Textes d'*Alice* (hors corpus), carré aléatoire, largeur(s) et clé(s) aléatoires dans la famille ; succès si
≥ 90 % des lettres (décalage de ≤ 16 positions toléré : une clé tournée donne le même texte décalé).
Seuil d'admissibilité d'une famille : **≥ 6/10**.
**Null** : 10 mélanges des N symboles par (famille, géométrie), même pipeline complet ; on retient le max.

## Critères de succès (tous requis)
1. qoff(réel) > max(null) + 0,5 **et** ≥ min(qoff des contrôles récupérés) − 0,5 ;
2. anglais continu lisible sur ≥ 80 % (jugé après 1) ;
3. R(réel, clé gagnante) hors de la distribution des R maximaux du null pour cette largeur ;
4. ré-enchiffrement exact ; 5. même clé gagnante avec une autre graine.
Sinon : **négatif**.

## Mise au point (contrôles seuls, consignée)
- Étage 1 seul (vraie clé en tête) : 18/30 en largeur ≤ 9 ; poids (1,3,6), (0,1,3), (1,4,10) pas meilleurs.
- Rang de la vraie clé : ≤ 50 dans 26/30, ≤ 2000 dans 29/30 ⇒ étage 2 sur une liste courte.
- Étage 2 : un départ réussit ≈ 45 % des fois ; plancher du modèle (0,01 / 1 / 10 / 100 comptes) sans effet ;
  8 départs indépendants : 19/20 sur le vrai clair. Trois fautes de parenthèses dans le code, et une mesure de
  récupération non tolérante au décalage, corrigées pendant la mise au point.

## Contrôles (réglages définitifs : TOP 50 / 20, 8 départs × 40 000)
| Famille | Géométrie | Récupérés | Remarque |
|---|---|---|---|
| E | B (182) | **9/10** | admissible |
| E | A (196) | **4/5** | admissible |
| D | B | **8/10** | admissible |
| I | B | 3/10 (mesure positionnelle) | les 7 « échecs » trouvent de l'anglais au niveau de la vérité (qoff −9,4…−9,8) : en sens inverse, une clé fausse ne fait que **permuter des segments** anglais (colonnes contiguës) |

**AMENDEMENT 1 (avant tout run réel)** : pour I, le succès d'un contrôle est jugé par le **carré retrouvé**
(≥ 90 % des lettres du texte reçoivent la bonne lettre), mesure indépendante de l'ordre des segments ; les
contrôles I sont refaits avec cette mesure (+ I et D en géométrie A). Pour un candidat réel en famille I, le
critère 2 (lisibilité) s'applique aux segments, et l'ordre des segments est alors une question séparée.
