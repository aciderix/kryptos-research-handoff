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
(a) nulles régulières k = 3, 4, 5, toutes phases, sans transposition ; (b) 64 routes × 2 géométries ; (c) colonnaire
complète largeurs 13 et 14, clé libre, par recuit sur cette statistique ; puis solveur de paires d'E15 sur les
meilleurs candidats. Coût estimé : quelques heures.
