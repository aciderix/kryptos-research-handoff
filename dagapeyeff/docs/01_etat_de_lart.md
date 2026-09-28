# État de l'art et registre des familles déjà testées (audit du 2026-09-28)

Règle : **« pas testé dans notre dépôt » ≠ « jamais testé »**. Chaque ligne indique la source. Les niveaux :
`CONNU` (établi historiquement / publié), `DÉJÀ TESTÉ` (par d'autres), `OBSERVATION` (mesuré ici),
`REVENDICATION` (affirmé sans preuve vérifiable).

## 1. Sources consultées
| Source | Nature | Apport |
|---|---|---|
| d'Agapeyeff, *Codes and Ciphers*, OUP 1939, p. 158 | primaire | le défi ; le livre enseigne le carré de Polybe à alphabet mélangé et l'ajout de nulles (pp. 124-126 selon Pelling). **Non consulté directement** : l'exemplaire archive.org est l'édition 1974 (sans le défi), en prêt contrôlé |
| Wikipédia, « D'Agapeyeff cipher » | secondaire | texte ; citation du livre sur les nulles (« if every third, fourth, or fifth letter… is a dummy ») ; oubli de la méthode par l'auteur |
| Shulman, *The Cryptogram*, avril/mai 1952 ; *The Cryptogram* 1959 ; Barker, *Cryptologia* 2(2):144-147 (1978) | secondaires anciennes | analyses ; Barker : 392 chiffres → 196 paires = 14×14 ⇒ transposition carrée suggérée. **Textes non consultés** (derrière paywall) |
| K. Schmeh, MysteryTwister C3 (PDF) | secondaire | texte (identique) ; hypothèse d'une erreur d'enchiffrement |
| N. Pelling, Cipher Mysteries (2008, 2 billets ; 2013) | analyse | 14×14 ; triplets 75 75 75 et 63 63 63 ; 16 transpositions diagonales testées (programme C++) ; hypothèse Polybe + colonnaire ; 2013 : « 04 » = bourrage final, extraction par colonnes réordonnées |
| numberworld.blog (2013, partie 1) | analyse | alternance ; IC 1,743 ; **symboles rares concentrés dans la dernière colonne** ; « représentation 2 » = transposition lignes↔colonnes |
| dagapeyeffresearch.com | projet computationnel | phases 3-7 : Polybe SA, colonnaire (dont 5040 ordres de largeur 7), ADFGX (faux positif k=7 retiré), two-square, Playfair, langues RU/FR/DE/eo, 116 cribs, 42 mots-clés ; **aucune solution** |
| github.com/ajejfiejof/dagapeyeff-cipher-solver | projet | analyse structurelle, double transposition de Kerckhoffs, blocs 7×14 ; **aucune solution**, pas de contrôle synthétique |
| msgtrail.com, puzzculture.com, cipherfoundation.org, Medium 2022 (403) | secondaires | résumés ; rien de nouveau vérifiable |

## 2. Faits structurels (recalculés ici, `tools/observe.c`)
| Fait | Statut |
|---|---|
| 395 chiffres = 79×5 ; 3 zéros terminaux | CONNU, vérifié |
| 392 chiffres → 196 paires ; 196 = 14×14 | CONNU (Barker 1978), vérifié |
| Alternance parfaite : 1ᵉʳ chiffre ∈ {6,7,8,9,0}, 2ᵉ ∈ {1,…,5} — **0 violation sur 392** | CONNU, vérifié |
| IC des paires = 0,0697 ; ×25 = 1,743 (anglais ≈ 1,7 ; aléatoire 1,0) | CONNU, vérifié |
| **18 symboles distincts sur 25** (absents : 61, 73, 95, 01, 02, 03, 05) ; « 04 » n'apparaît qu'une fois | OBSERVATION (souvent cité comme « 3 absents » : faux si l'on compte la ligne 0) |
| Paires consécutives identiques ×17 ; triplets 75 75 75 (@38) et 63 63 63 (@94) | CONNU (Pelling), vérifié |
| **Les 8 occurrences des 5 symboles les plus rares (04,71,92,93,94) sont toutes en colonne 14** de la grille 14×14 ; P(hasard) ≈ 10⁻⁸ | CONNU qualitativement (numberworld 2013) ; **quantifié ici** |

**Conséquences logiques (déduites, pas testées) :**
- IC ≈ anglais ⇒ les paires sont des **lettres intactes** (substitution monoalphabétique par paire). Une
  fractionation qui transposerait lignes et colonnes indépendamment (bifide/trifide) aplatirait l'IC ⇒ exclue.
- Alternance parfaite ⇒ toute transposition agit sur des **paires entières** (ou préserve la parité).
- Colonne 14 spéciale ⇒ soit (A) remplissage par colonnes / lecture par lignes, la colonne 14 portant la **fin**
  du message (bourrage en lettres rares), soit (B) une **nulle en fin de chaque ligne** (14 nulles, message de
  182 = 14×13), conforme au conseil du livre sur les nulles.

## 3. Solutions revendiquées — audit
| Revendication | Source | Vérification ici | Verdict |
|---|---|---|---|
| « WELCOME TO THE CLUB » | commentaire Cipher Mysteries, 2023 | 16 lettres pour 196 symboles | **Réfutée** (incompatible en longueur) |
| « FATHER CHRISTMAS FILL OUR STOCKINGS … CHOCOLATE » par « Polybe standard » | commentaire, 2026 | ~80 lettres ; un Polybe direct donne 196 lettres ; la lecture directe contient un triplet (75 75 75) absent du texte | **Réfutée** |
| « THE MAP THAT WAS PREPARED FOR THE EXPEDITION… » | commentaire, 2025 | texte complet et mécanisme non fournis | **Non vérifiable** |
| Sommes des groupes mod 26 → noms de fibres | commentaire, 2026 | mécanisme ad hoc ; aucun ré-enchiffrement proposé | **Non admissible** |
| ADFGX k=7 | dagapeyeffresearch | retiré par l'auteur (surajustement, 14 % des positions) | **Retirée** |

## 4. Registre des familles déjà testées (par d'autres), sans solution
| Famille | Qui | Couverture connue |
|---|---|---|
| Polybe seul (substitution simple des 196 paires, ordre imprimé) | nombreux ; dagapeyeffresearch (SA) | exhaustive en pratique |
| Transpositions diagonales 14×14 (16 variantes) | Pelling 2008 | 16 routes |
| Lignes↔colonnes (représentation 2) | numberworld 2013 | 1-2 routes |
| Colonnaire (dont 5040 ordres de largeur 7) | dagapeyeffresearch | largeur 7 exhaustive ; 14×14 « testé » sans détail |
| Écriture par lignes, extraction par colonnes **avec réordonnancement interne** ; « 04 » = X de bourrage final unique (clair de 195 lettres) | Pelling, Cipher Mysteries, 23/12/2013 (hypothèse, non résolue) | hypothèse ; pas de clé publiée. Ne relève pas que 8 lignes (pas 1) finissent par un symbole rare |
| Double transposition (Kerckhoffs) | GitHub ajejfiejof | partielle, sans contrôle |
| ADFGX, two-square, Playfair | dagapeyeffresearch | négatifs |
| Nulles (motifs de suppression) | dagapeyeffresearch (« null-removal patterns ») | détails non publiés |
| Langues autres que l'anglais | dagapeyeffresearch | RU, FR, DE, eo |

**Ce qui manque dans ces travaux (et justifie notre protocole) :** aucun ne publie de **contrôle synthétique**
(message planté puis retrouvé), de **distribution nulle** calibrée, ni de **ré-enchiffrement** exact ; aucun
n'utilise explicitement la contrainte « colonne 14 » comme **prédiction** à satisfaire.
