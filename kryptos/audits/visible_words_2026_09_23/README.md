# Mots VISIBLES sur l'œuvre comme clés (leçon MEDUSA du *Cyrillic Projector*)

## Motivation
Sur le *Cyrillic Projector*, le mot-indice **MEDUSA** est gravé en clair (lettres latines, autre police) ; la clé réelle
ЙБАСГТ = МЕДУЗА lu à travers l'alphabet ТЕНЬ (base 5). Scheidt 2003 : « les clés sont dissimulées sur la sculpture ».
Question : un mot **visible sur Kryptos, hors textes chiffrés**, joue-t-il ce rôle pour K4 ?

## Inventaire (fixé AVANT le calcul) — `inventory.txt`
- Morse K0 (base 1 §K0) : mots isolés et groupes, graphies attestées (DIGETAL, INTERPRETATI, INTERPRETATIT) et corrigées.
- Tableau et cuivre : KRYPTOS, HILL (L en trop), YAR, DYAHR.
- Rose des vents : N, E, S, W, ENE, EASTNORTHEAST, NORTHEAST, WSW.
- Clés connues de K1–K3 (référence) : PALIMPSEST, ABSCISSA.

## Protocole (clé entièrement déterminée : aucun paramètre libre)
- Alphabets possibles (clair et chiffré, indépendamment) : A–Z, KRYPTOS, ou l'alphabet à mot-clé de chaque mot de l'inventaire.
  Cela couvre QI, QII, QIII et QIV, et le montage MEDUSA (mot d'alphabet + mot de clé).
- Clé : chaque mot de l'inventaire répété ; lettre-clé convertie par son rang dans A–Z, dans l'alphabet clair ou dans l'alphabet chiffré ;
  toutes les phases (début de clé décalé).
- Modes : Vigenère C = P + k, Beaufort C = k − P, Beaufort variante C = P − k.
- Score : nombre de lettres des cribs retrouvées (sur 24). **Aucun score d'anglais.**
- Témoin : même balayage sur 20 chiffrés aléatoires ; seuil fixé d'avance = maximum du témoin. K4 n'est « signal » que s'il le dépasse.
- Déjà couvert ailleurs : texte Morse comme clé courante (`e_k0_running_key_01`) ; autoclave (éliminé pour toute amorce).

## Gromark : mots → amorces
Les 5 premières lettres de chaque mot converties en chiffres (rang A=1…Z=26, puis mod 10), comparées à la liste exacte des 39 amorces compatibles de Bean
(`../gromark_scope_2026_09_23/bean_gt_10_5_primers.txt`).

## Résultats (23/09)
- 39 alphabets × 39 alphabets × 302 clés (mot, phase) × 3 conversions × 3 modes ≈ 4,1 millions de montages déterministes.
- **K4 : meilleur score 10/24** (alphabet clair INVISIBLE, chiffré PALIMPSEST, Beaufort, clé DIGITALINTERPRETATION décalée).
  Une vraie clé donnerait 24/24 (ou 23 avec une coquille).
- Témoin : 120 chiffrés aléatoires, meilleur score par chiffré {7 : 1, 8 : 73, 9 : 38, 10 : 8} ; **P(≥ 10) = 7 %**. Le seuil pré-enregistré (9, sur 20 témoins) est dépassé de 1, mais avec 120 témoins ce score est **banal**.
  ⇒ **aucun signal ; famille éliminée** : aucun mot visible de l'inventaire ne sert d'alphabet et/ou de clé périodique à K4, dans aucun montage Quagmire I–IV ni MEDUSA.
- **Gromark** : aucune amorce tirée des mots (3 conversions lettres → chiffres) ne figure parmi les 39 amorces compatibles de Bean ⇒ éliminé.
- Reste hors portée : mots visibles utilisés **autrement** que comme clé répétée ou alphabet (sélection de lettres, gabarit, ordre de lecture), et mots **non encore inventoriés** (plaque de dédicace, inscriptions du site non documentées).
