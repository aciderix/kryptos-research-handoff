# E12 — RÉSULTATS (2026-09-28) : aucun mécanisme documenté dans le livre ne reproduit le profil observé

Pré-inscription : `PREREGISTRATION.md` (commitée avant le calcul). Simulateur : `tools/e12_calibration.py`
(2 000 tirages en anglais, 300 par autre langue, textes réservés `data/heldout/`). Sortie complète : `calibration.out`.

## 1. Mécanismes simulés (vrais textes, 8 langues)
| Mécanisme | T1 moyen (réel 18) | T2 moyen (réel 13) | T3 moyen (réel 12) | T4 moyen (réel 1) | P(T1≤18 & T3≥12 & T4≤1) | p profil |
|---|---|---|---|---|---|---|
| M1 substitution injective | 19,5-22,0 | 18,5-21,0 | 6,3-8,4 | 5,6-9,3 | 0 | ≤ 0,003 |
| M6 nulles 1/3, 1/4, 1/5 (uniformes / lettres rares / cases déjà utilisées) | 18,9-25 | 18-24 | 5-9 | 7-14 | 0 | ≤ 0,003 |
| M7 Playfair | ≈ 24 | ≈ 23,5 | ≈ 7 | ≈ 11,5 | 0 | ≤ 0,001 |
| M11 Vigenère (clé 3-8) | ≈ 25 | ≈ 24 | ≈ 6 | ≈ 13 | 0 | ≤ 0,001 |
| M12 homophones | ≈ 24,5 | ≈ 23,7 | ≈ 7 | ≈ 11 | 0 | ≤ 0,001 |
| M15 omissions 5 % | ≈ 22 | ≈ 21 | ≈ 8 | ≈ 8,5 | 0 | ≤ 0,001 |
| M4v Wolseley « numéro » (13 classes) | 12,5-12,9 | 12,1-12,7 | 7,7-8,5 | 2,4-3,4 | 0-0,003 | ≤ 0,003 |
| M8 fractionnation | ≈ 24,5 | ≈ 24 | ≈ 6,5 | ≈ 12 | 0 | ≤ 0,01 |
(Détail par langue : `calibration.out`.) M8 : 30 % des paires seulement respectent l'alternance (ligne, colonne) ;
0,3 % des chiffrés la respectent entièrement.

## 2. Tests structurels sur le chiffré
- S1 indépendance ligne × colonne (table 5×5) : G = 86,8, **p = 0,0002** ⇒ pas un produit ligne × colonne ⇒
  contre la fractionnation.
- S2 : retirer **n'importe laquelle** des colonnes 1-13 laisse 18 symboles ; retirer la **colonne 14** en laisse 13.
  ⇒ la colonne 14 est **unique** (pas un artefact de sélection).
- S3 : dans les colonnes 1-13, le symbole ne dépend ni de la colonne (p = 0,49) ni de la ligne (p = 0,29).

## 3. Décision (règle pré-inscrite) et portée
- **Aucun** mécanisme documenté (tel que simulé) n'atteint le seuil (P conjointe ≥ 0,01 ou p profil > 0,05), dans
  aucune langue. Le plus proche, Wolseley « numéro », reproduit le **nombre** de symboles (≈ 13) mais pas leur
  **uniformité** (T3 ≈ 8 au lieu de 12).
- **Portée** : la conclusion d'E11 se **renforce** pour les mécanismes documentés, **sans être une preuve** :
  (i) les variantes simulées ne sont pas exhaustives (règles de nulles, schémas d'homophones, paramètres) ;
  (ii) des mécanismes non documentés restent possibles ; (iii) la nature du clair reste **non déterminée**.
- Argument structurel confirmé numériquement : nulles, homophones, polyalphabétiques, Playfair, omissions et
  fractionnation **augmentent** ou conservent le nombre de symboles distincts ; seul un codage plusieurs-à-un
  (M4v) le diminue jusqu'à ≈ 13.

## 4. Piste qui en découle (HYPOTHÈSE, à pré-inscrire)
Un codage **plusieurs-à-un en 13 classes à fréquences équilibrées** (chaque symbole valant ≈ 2 lettres, appariées
de façon à égaliser les fréquences) reproduirait T2 et l'uniformité ; il resterait à le trouver documenté (livre,
manuels contemporains) et à évaluer s'il est déchiffrable (ambiguïté 2 lettres par symbole ⇒ lecture par contexte).
