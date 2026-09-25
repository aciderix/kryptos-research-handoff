# Audit « simulateur Sanborn » (25/09/2026) : quel procédé laisse l'empreinte « 7 » de K4 ?

## Pourquoi ce changement de méthode

Depuis T19, on faisait comme tout le monde depuis 35 ans : choisir une famille de clés, la confronter aux 24 lettres des cribs, compter les compatibles. Les cribs sont épuisés : environ 113 bits d'information, trop peu pour trancher une famille souple, et une famille rigide échoue toujours.

Ici, on renverse la question. On ne demande plus « cette clé est-elle compatible ? », mais « **quel procédé fabrique un texte qui ressemble à K4 sur ses 97 lettres ?** ». Pour chaque procédé faisable à la main, on fabrique un million de faux K4 :
- clair anglais tiré de Gutenberg (16,8 M lettres), avec EASTNORTHEAST et BERLINCLOCK insérés à leur place ;
- clés tirées au hasard, alphabet KRYPTOS de Sanborn.

On mesure ensuite la probabilité que le procédé reproduise ce que montre K4 hors de toute hypothèse de clé.

| Fichier | Rôle |
|---|---|
| `sim_sanborn.c` | 52 procédés × 1 000 000 de chiffrés (OpenMP, 13 s au total) |
| `results_sim_cribs.txt` | avec les cribs insérés dans le clair |
| `results_sim_sans_cribs.txt` | anglais seul (témoin de l'effet des cribs) |
| `t34_autocle_deux_alphabets.c` | T34 : test exact du seul procédé que le simulateur désigne |
| `ctl_t34.py`, `ctl_t34_verite.txt` | contrôle positif de T34 (faux K4 du même procédé) |
| `results_t34_F5.txt`, `results_t34.txt` | T34 sur K4 et sur K4 mélangé (F = 5 avec 20 témoins, F = 7 avec 16) |

**Reproduire.**
- Corpus : `../erreurs_multiples_2026_09_24/corpus_get.sh` (Gutenberg), puis concaténation des textes.
- Quadrigrammes : `../recuit_2026_09_24/build_qg.c`.
- Alphabets : `NOMAT=1 ../moteur_2026_09_25/genalpha ../motcle_pas7_2026_09_24/words.txt > alphas_kw.bin`, soit 101 086 alphabets à mot-clé.
- Compilation : `gcc -O3 -march=native -fopenmp`.
- Commandes :
  - `sim_sanborn corpus.txt ../motcle_pas7_2026_09_24/words.txt 1000000` (préfixer `CRIB=0` pour l'anglais sans cribs) ;
  - `t34 alphas_kw.bin qg.bin 20 7`.

## 1. Ce que K4 montre sur ses 97 lettres

| Mesure | K4 | Hasard (lettres de K4) | Remarque |
|---|---|---|---|
| Coïncidences c[i] = c[i+7] | **9** | 3,2 | connu (NSA 1992) |
| Coïncidences aux écarts 14 et 21 | **2 et 3** | 3,0 et 2,7 | **nouveau ici** : au niveau du hasard |
| IC moyen des 7 colonnes | 0,042 | 0,038 | une vraie période 7 donne 0,063 |
| Doublets | 6, dont **5 dans la classe 4 (mod 7)** | 3,7 | connu (Stehle 2002, Gillogly 2003) |
| Doublets aux trois paires de crib 25–26, 32–33, 67–68 | **3 sur 3** (NO → QQ, ST → SS, IN → TT) | 1/26 chacune | les trois paires de la colonne 4–5 qui tombent dans les cribs |

**Constat.** Le « 7 » de K4 ne relie que des **voisins**. L'écart 7 est en excès, les écarts 14, 21, 28, 35, 42 et 49 ne le sont pas (2, 3, 4, 1, 3, 0 coïncidences). Une clé périodique, quelle qu'elle soit (Vigenère, Beaufort, Porta, Quagmire à alphabet quelconque, Gronsfeld, période 14 ou 21), élève **tous** les multiples de 7. Ce n'en est donc pas une, ce que confirmait déjà l'IC par colonne. C'est une dépendance **de proche en proche**, de chaque lettre à celle qui est 7 rangs avant.

## 2. Résultats du simulateur (cribs insérés, 10⁶ chiffrés par procédé)

« Profil voisin » : A7 ≥ 9 **et** A14 + A21 ≤ 5, comme K4. « D » : au moins 5 doublets dans une même classe mod 7.

| Procédé | P(A7 ≥ 9) | P(profil voisin) | P(D) | P(voisin et D) | P(3 doublets de crib) |
|---|---|---|---|---|---|
| hasard uniforme | 0,8 % | 0,29 % | 0,09 % | 4 × 10⁻⁶ | 7 × 10⁻⁵ |
| Vigenère KRYPTOS période 7 (clé ou mot) | 9,4 % | 0,48 % | 0,10 % | 8 × 10⁻⁶ | **0** |
| Quagmire III alphabet quelconque, période 7 | 9,4 % | 0,46 % | 0,20 % | 3 × 10⁻⁶ | 7 × 10⁻⁵ |
| période 7 + erreurs de Sanborn, lettre omise, copies | 7–8 % | 0,5–0,6 % | 0,09 % | ≤ 7 × 10⁻⁶ | 0 |
| **autoclé sur le clair, écart 7, forme Vigenère** | **14 %** | **4,9 %** | 0,06 % | **4 × 10⁻⁵** | 0 (alphabet KRYPTOS) |
| idem, alphabet du chiffré différent (Quagmire IV) | 14 % | 4,8 % | 0,06 % | 4 × 10⁻⁵ | — |
| idem, Beaufort / Variante / lettre-clé dans un autre alphabet | 1,0 % | 0,35–0,38 % | 0,06–0,08 % | ≤ 8 × 10⁻⁶ | — |
| autoclé sur le chiffré, écart 7 | 0,001 % | 0,000 3 % | 0,05 % | 0 | 0 |
| période 7 puis transposition en 7 colonnes | 4,9 % | 1,9 % | 1,3 % | 3 × 10⁻⁴ | 4 × 10⁻⁴ |
| période 7 puis addition en chaîne c[i] += c[i−1] | 1,0 % | 0,38 % | **0,81 %** | 3 × 10⁻⁵ | 0 |
| clé courante, progressive, ligne + colonne, Fibonacci, chaîne, disque par bloc, clé transposée, méthode K3 après la substitution, dents de scie (Matson), Chaocipher | ≈ hasard | ≈ hasard | ≈ hasard | ≤ 2 × 10⁻⁵ | ≈ hasard |

Tableau complet (52 procédés, avec et sans cribs) : `results_sim_cribs.txt`, `results_sim_sans_cribs.txt`.

**Lecture.**
1. **Un seul procédé rend naturel le profil « voisin » de K4 : l'autoclé sur le clair à l'écart 7, forme Vigenère**, où la lettre-clé est lue dans le même alphabet que le clair : τ(c_i) = σ(p_i) + σ(p_{i−7}). Le profil y est 17 fois plus fréquent qu'au hasard, et 10 fois plus qu'avec une vraie période 7.
   - La raison est algébrique : c_i = c_{i+7} ⇔ σp_i + σp_{i−7} = σp_{i+7} + σp_i ⇔ p_{i−7} = p_{i+7}. Deux lettres anglaises à 14 rangs coïncident à 6,6 %. Aux écarts 14 et 21, rien ne se simplifie : le taux reste celui du hasard.
   - En Beaufort, en Variante, ou si la lettre-clé passe par un autre alphabet, le terme commun ne s'annule plus et l'excès disparaît. **La forme est donc imposée** (Vigenère), et c'est un résultat nouveau. En revanche, l'alphabet du chiffré τ reste libre.
   - C'est l'hypothèse de la NSA (1992), qui avait vu la « rugosité à l'intervalle 7 ». La simulation la précise.
2. **Aucun procédé ne concentre les doublets comme K4.** Les transpositions finales montent à 1–2 %, parce qu'elles multiplient les doublets partout. Mais elles détruisent l'alignement des cribs, que Sanborn a confirmé (NYPVTT → BERLIN), et, seules, gardent l'IC de l'anglais. L'addition en chaîne concentre les doublets (0,8 %) sur la colonne dont la lettre « zéro » est fréquente, mais elle exige O = T = N aux trois doublets des cribs : impossible.
3. **Signature complète** (écart 7, doublets en classe 4 et trois doublets de crib) : au plus 10⁻⁵ pour tous les procédés **positionnels**, et 0 sur 10⁶ pour la plupart. Les deux seuls procédés qui dépassent (1 à 3 × 10⁻⁵) sont des transpositions finales, écartées par l'alignement des cribs (point 2).

## 3. T34 : le procédé désigné, testé à fond

**Famille.** Autoclé sur le clair à l'écart 7, forme Vigenère, **deux alphabets à mot-clé indépendants** : σ pour le clair et la clé, τ pour le chiffré, plus un décalage d. C'est un Quagmire IV à autoclé.
- T18 ne couvrait que σ = τ, avec un alphabet quelconque.
- T27 ne couvrait qu'un seul alphabet à mot-clé à la fois, l'autre étant le même, A–Z ou KRYPTOS.
- T34 couvre ces cas **et** toutes les paires de mots-clés.

**Méthode.** Dans chaque classe mod 7, la relation se propage de 7 en 7 à travers le clair inconnu. Entre deux lettres de crib consécutives a et b = a + 7m, on a : σp_b = Σ (−1)^{m−t} (τc_{a+7t} + d) + (−1)^m σp_a. Cela fait 17 équations, dont chacune ne fait intervenir que des lettres connues.
- Balayage : 101 088 σ, 202 176 τ (les alphabets à mot-clé de `genalpha`, plus A–Z et KRYPTOS, τ dans les deux sens) et 26 décalages, soit 5,3 × 10¹¹ paires.
- Recherche **exacte** des paires à au plus F équations fausses, par tiroirs : avec F + 1 blocs, un bloc au moins est juste.
- Pour chaque paire retenue, le **clair entier** est déterminé : on le lit et on le note aux quadrigrammes.

**Contrôle positif.** Faux K4 fabriqué avec σ = MUTINED…, τ = RECONSIDER…, d = 24 : retrouvé en 0,3 s, **0 équation fausse**, clair entier lisible (« …EASTNORTHEASTESTHEEMPRESS… BERLINCLOCKTHECHARITABLE… », score −4,14).

**K4.**
- Aucune paire à moins de **7 équations fausses sur 17** ; 54 paires à 7.
- Clairs illisibles : meilleur score −6,89, pour −4,2 en anglais.

**Témoins (K4 mélangé, 16 tirages).** Minimum de 5 équations fausses (1 fois), 6 (11 fois) ou 7 (4 fois) ; voir `results_t34.txt`. K4 est parmi les moins bons de ses propres mélanges.

**Sens.** Les 17 équations engagent 46 lettres du chiffré, et chacune n'entre que dans une équation (on repart du clair connu à chaque crib). Avec le taux d'erreur de Sanborn (≈ 4 % dans le petit fragment), on attend environ 2 lettres fausses, donc environ 2 équations fausses. En avoir 7 ou plus a une probabilité inférieure à 1 %. Voir les doublets comme des copies ne change rien : leurs secondes lettres (26, 33, 68) sont toutes dans la classe 5, qui ne porte que 2 équations. **Éliminé pour les alphabets de Sanborn**, même avec ses erreurs.

## 4. Bilan

- **Nouveau (méthode et faits).**
  - Le profil « voisin » du 7 : écart 7 en excès, écarts 14 à 49 au niveau du hasard. Cela précise l'IC par colonne (base 8) et exclut toute clé périodique sans passer par les cribs.
  - Le simulateur montre qu'un seul procédé manuel fabrique ce profil, et **seulement en forme Vigenère**.
  - Ce procédé est éliminé pour toutes les paires d'alphabets à mot-clé (T34, nouveau cas Quagmire IV), même avec les erreurs de Sanborn.
- **Conséquence.** Si le « 7 » est réel, son mécanisme est hors de tout procédé manuel classique avec des alphabets de Sanborn. Il reste deux possibilités :
  - soit une dépendance de proche en proche d'une autre nature ;
  - soit un alphabet quelconque (indécidable avec 24 lettres, `../recuit_2026_09_24/`).
- **Rappel honnête.** Le « 7 » n'a qu'une probabilité d'environ 2 × 10⁻⁴ au hasard (module balayé), soit 10⁻³ à 10⁻² en payant le choix après coup des statistiques. Le simulateur montre que le meilleur procédé positionnel ne gagne qu'un facteur 10 sur le hasard pour la signature (écart 7 et doublets). **Il n'est donc pas exclu que le « 7 » soit un hasard.** Aucune famille ne l'explique mieux.
