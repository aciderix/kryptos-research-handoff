# Relecture complète du livre et de nos travaux (E01-E15) : ce qui nous a échappé

Date : 2026-09-28. Source : scan de *Codes and Ciphers* (réimpression Gale 1974 de l'édition OUP 1939, archive.org
`codesciphers0000daga`, non versé au dépôt ; pages citées = pagination du livre). Niveaux : `LIVRE` (dit par
l'auteur), `OBSERVATION` (mesuré ici), `TROU` (hypothèse jamais testée chez nous), `CORRECTION` (erreur de nos docs).

## 1. Où se trouve le défi, et ce que cela implique (LIVRE)
Le défi (p. 158) clôt le chapitre VII « Methods of deciphering », juste après la solution du Vigenère. Ce chapitre
enseigne, dans l'ordre : substitution simple (Rohan, p. 133) ; tables de fréquences ; le **chiffre à deux lettres
d'un ami** (coordonnées A-E, carré mélangé, erreurs d'encodage, p. 141-146) ; Vigenère (p. 146-150) ; transposition
(p. 150-153) ; **codes de dictionnaire numérotés** (Mansfield, groupes de 5 chiffres, p. 153-156). Le lecteur est
« invité à tester son habileté » : le défi est vraisemblablement conçu pour les méthodes de ce chapitre.

La méthode de transposition enseignée (p. 151-152) est littéralement : *compter les lettres ; 36 = 6×6 ⇒ construire
un carré ; y écrire le chiffré **de haut en bas, par colonnes** (« transpositions are usually written this way ») ;
permuter les colonnes jusqu'à lire des mots en travers*. Appliquée au défi : 196 = 14×14.

## 2. Les trous (TROU), par ordre d'importance

| # | Trou | Page | Pourquoi c'est un trou |
|---|---|---|---|
| T1 | **« 13 classes » + colonnaire complète de largeur 13 ou 14 à clé libre** (le carré de la p. 151) | 151-152 | E14 n'a couvert que les largeurs 2-11, E15 les doubles ≤ 7×7. E02/E03 ont bien testé la largeur 14, mais **seulement** pour « une paire = une lettre », que E11 a rejeté ensuite. La largeur que le livre désigne n'a **jamais** été testée avec la seule hypothèse restée compatible. |
| T2 | **Nulles régulières « une sur 3, 4 ou 5 », insérées après chiffrement**, combinées à « 13 classes » | 111 | E12 a simulé ces nulles pour le cas injectif seulement. E13 Q2 conclut qu'il faut une transposition parce que le défi n'a pas de dépendance entre symboles voisins ; or des nulles intercalées cassent aussi cette dépendance. Hypothèse « 13 classes + nulles, **sans** transposition » non exclue. Test peu coûteux (12 motifs de nulles). |
| T3 | **Routes** (dont la route « chinoise » en colonnes alternées depuis la droite, p. 126, et la méthode des cartes p. 69) avec « 13 classes » | 69, 126 | E01 a testé 64 routes × 2 sens × 2 géométries pour le cas injectif seulement. |
| T4 | **Transposition à clés multiples de Richelieu** (clés successives de longueurs différentes 7, 5, 9, 10, appliquées par blocs) | 43 | Famille absente de notre registre (`01_etat_de_lart.md` § 4). Ne change pas les comptes : ne concerne que « 13 classes ». |
| T5 | **Code numérique chiffré** : code de dictionnaire (page + rang du mot) dont les chiffres sont « à nouveau chiffrés » | 129-130, 153 | Le livre le propose explicitement (« these figures … could again be enciphered »). Un clair fait de chiffres est **plat et sans structure séquentielle**, ce qui expliquerait le profil **sans exiger de transposition** ; 13 symboles = 10 chiffres + homophones ou séparateurs. La ligne 13 de `02_mecanismes_du_livre.md` a confondu le code brut (incompatible avec l'alternance) et le code **chiffré** (compatible). Sans le dictionnaire, ce cas serait indéchiffrable : testable seulement en partie. |
| T6 | **Message court caché parmi une majorité de nulles** tirées dans les mêmes 13 symboles (principe de la grille de Cardan « letters or syllables », p. 113) | 30, 113 | E12 n'a simulé que des nulles minoritaires (1/3 à 1/5). Un message de 25-35 lettres anglaises utilise environ 13 lettres ; noyé dans environ 150 nulles uniformes sur ces mêmes symboles, il reproduit profil et absence de structure. Sous la distance d'unicité : indémontrable sans règle de position. |
| T7 | **Carré de Wolseley : la numérotation fait partie de la clé** (« the method of numbering them and the keyword are all that one has to remember ») | 109 | E13 (b) n'a testé que la numérotation de l'exemple (symétrie centrale) avec des mots isolés du livre. Une numérotation libre, ou un mot-clé formé de lettres fréquentes (appariées par symétrie aux lettres rares de fin d'alphabet), donne exactement l'appariement « fréquente + rare » d'E13 (a). Reste non documenté : l'envoi du **numéro** au lieu de la lettre partenaire. |
| T8 | **Carré de Polybe historique sans Q (et non sans J)** | 16 | Nos modèles fusionnent I/J. Sans effet sur les tests de profil ; à corriger pour toute lecture « carré standard ». |

Mécanismes relus et **confirmés hors course** (pas de trou) : chiffre des voyelles de Selenus (p. 37 : consonnes →
5 voyelles, voyelles → 3 homophones, environ 20 à 26 symboles) ; table syllabique de Selenus (p. 127-128, que l'auteur
invite à développer : syllabes ⇒ **plus** de symboles) ; torches de Fortius (p. 53) ; bilitère de Bacon (p. 38) ;
Porta (p. 33) ; nihiliste (p. 50) ; fractionnation MANCHESTER (p. 124-125 : une colonnaire sur coordonnées séparées ne
peut pas conserver l'alternance parfaite, puisque les colonnes de rang impair commencent par une coordonnée de colonne).

## 2 bis. Ce que la relecture complète de E01-E15 corrige dans la liste ci-dessus
(Ajouté après relecture de **tous** les `RESULTS.md` ; la première version de ce document ne s'appuyait que sur
E11-E13, les pré-inscriptions d'E14-E15 et des extraits d'E01/E14.)
- **T1 n'est pas vierge.** E03 a testé la largeur 13 (géométrie B, clé libre) en « une paire = une lettre » et y a
  relevé une **OBSERVATION non résolue** : sur 11 graines, deux clés récurrentes K1/K2, propres au vrai chiffré, qui
  captent une structure de bigrammes réelle (+1,4 sur une clé quelconque) sans donner d'anglais. Elle a été close en
  E04 par un argument de contiguïté (K1 : 13 trigrammes, 0 quadrigramme répétés), mais **jamais examinée sous
  « 13 classes »**. Or ce comportement (structure réelle, pas d'anglais lisible) est ce qu'on attend d'un bon
  ordre de colonnes quand chaque symbole vaut deux lettres. Premier test de E16, presque gratuit : solveur de paires
  d'E15 **à clé imposée** K1 et K2. Réserve : sous « 13 classes », la vraie clé produit beaucoup de répétitions
  (E14 : R = 57-173 aux largeurs 2-11), ce qui plaide plutôt **contre** K1 (R = 13).
- **Limite de puissance connue (E04 § 5-6).** Sur 182 symboles, une statistique invariante (répétitions, et
  vraisemblablement toute dépendance de voisinage) ne départage plus des espaces de clés au-delà d'environ 10¹⁰ :
  un recuit sur 13! ou 14! ordres crée autant de structure sur des mélanges. La partie « largeur 13/14 » de E16
  doit donc d'abord **prouver sa puissance sur contrôles** ; elle peut échouer.
- **T2 est probablement déjà très contraint.** Avec des nulles une sur k et sans transposition, les suites de
  k−1 vrais symboles gardent la contiguïté du clair fusionné, qui répète beaucoup de n-grammes. Or l'ordre imprimé a
  des répétitions au niveau des mélanges (E11 § 3 : 67 bigrammes répétés contre 68,5 ; E04 : 5 trigrammes contre
  6,2 ± 2,6). E16 (a) devrait le confirmer vite.
- **T3** : E01 couvrait les routes pour le cas injectif ; l'espace est petit (64 routes), donc la statistique
  invariante garde sa puissance. Trou entier et peu coûteux.

## 2 ter. Antériorités (vérifiées le 2026-09-28 : nos journaux, `01_etat_de_lart.md`, Cipher Mysteries 2008/2013 et commentaires, dagapeyeffresearch, brendanhiggins.dev)
| Trou | Déjà fait par nous ? | Déjà fait par d'autres ? | Verdict |
|---|---|---|---|
| T1 largeur 13/14 × « 13 classes » | largeur 13/14 en injectif seulement (E02, E03) ; K1/K2 jamais revus sous « 13 classes » | largeur 14 (Pelling, dagapeyeffresearch) en injectif ; « 13 classes » n'existe nulle part ailleurs | **nouveau** (la combinaison) |
| T2 nulles régulières × « 13 classes » | injectif seulement (E12, profil) ; comptes de répétitions d'E04/E11 déjà défavorables | citation du livre connue de tous ; aucun test systématique publié (dagapeyeffresearch : « pending ») | **nouveau mais probablement déjà tranché** par nos données : simple vérification |
| T3 routes × « 13 classes » | routes en injectif (E01) | 16 diagonales (Pelling 2008), injectif | **nouveau** (la combinaison) |
| T4 Richelieu | non | rien trouvé | **nouveau** |
| T5 code numérique chiffré | non | **code de dictionnaire déjà proposé** (Knul 2014 ; brendanhiggins.dev) sous forme brute ; la variante « chiffres re-chiffrés par paires » n'est pas publiée | **redite partielle** ; invérifiable sans le dictionnaire |
| T6 message court dans les nulles | non | idée voisine (« remplissage aléatoire », brendanhiggins.dev ; lettres aléatoires insérées, triggernick.com, non consulté) | **redite partielle** ; invérifiable |
| T7 numérotation libre de Wolseley | numérotation standard seulement (E13 b) | rien trouvé | **nouveau**, mais purement documentaire |
| Flux des seuls 2ᵉ chiffres (§ 3) | ici, descriptif | Melichar 2014 ; brendanhiggins.dev (6-9/0 = nulles) | **redite** |

## 3. Contrôles exploratoires faits pendant la relecture (OBSERVATION, non pré-inscrits, descriptifs)
- **Appariement décalé d'un chiffre** (colonne de la paire i, ligne de la paire i+1) : 20 symboles, profil
  d'allure anglaise (23, 17, 16, 14, 13, …, 3, 1). **Piège** : la table est **indépendante** (G = 16,2 ; p = 0,40), donc
  ce profil est le simple produit des fréquences des deux chiffres. L'appariement standard est au contraire fortement
  dépendant (G = 86,8 ; p = 0,0002) : c'est bien lui l'unité.
- **Aucune structure périodique** : symbole × (position mod p), p = 2-30, en lecture par lignes (182 et 196) et par
  colonnes : pas de p-valeur isolée sous 0,09, sauf 0,007 à la période 12 en géométrie A, non significatif après
  correction (29 périodes). Aucun 4-gramme répété (les mélanges en ont 0 à 3). Pas de code à longueur fixe lisible tel
  quel, pas de mot répété.
- **Place des symboles dans le carré de chiffres** (lu par lignes 6-0 × 1-5, cases 0-24) : les 13 symboles du corps
  occupent les cases 1-15 moins 5 et 7 ; les symboles propres à la colonne 14 occupent les cases 5, 16, 17, 18 et 23.
  Cela ressemble à une **liste d'environ 13 valeurs inscrites dans l'ordre** plutôt qu'à un alphabet de 25 lettres
  (cohérent avec « 13 classes » ou un code numérique). Interprétation après coup : indice, pas preuve.

## 4. Bilan
Les 15 expériences ont exclu, avec contrôles et null, tout ce qui est « une paire = une lettre », puis « 13 classes »
avec les colonnaires de largeur 2-11 et les doubles ≤ 7×7. Mais en passant de E11 à E13-E15, nous avons testé la
nouvelle hypothèse sur les transpositions **faciles à balayer**, pas sur celles **que le livre désigne**. Le trou
principal est T1, le carré 14×14 écrit par colonnes, suivi de T2 (nulles régulières) et T3 (routes).

T5 (code numérique chiffré) et T6 (message court dans les nulles) expliquent le profil sans transposition, mais ne
peuvent être ni confirmés ni exclus par le chiffré seul (pas d'oracle, sous la distance d'unicité) ; ils rejoignent
l'erreur d'encodage et le canular dans la catégorie « compatible, invérifiable ».

Suite proposée (E16, à pré-inscrire) : statistique de dépendance entre symboles voisins, **invariante par relabellisation**
(donc sans connaître l'appariement), calibrée sur contrôles « 13 classes » :
(a) nulles régulières k = 3, 4, 5, toutes phases, sans transposition ; (b) 64 routes × 2 géométries ; (c) clés K1/K2
d'E03 à clé imposée, puis colonnaire complète largeurs 13 et 14 à clé libre **seulement si** la puissance est
démontrée sur contrôles (§ 2 bis) ; puis solveur de paires d'E15 sur les meilleurs candidats. (a), (b) et K1/K2 :
moins d'une heure ; largeurs 13/14 à clé libre : incertain.

## 5. Mise à jour après E16 (2026-09-28)
T2 (nulles régulières), T3 (routes) et les clés K1/K2 d'E03 sous « 13 classes » : **négatifs** ; T1 à clé libre
(largeurs 13/14) : **sans puissance** avec une statistique invariante sur 182 symboles. Voir `experiments/E16_thirteen_classes_gaps/RESULTS.md`.
