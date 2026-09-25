# Audit « alphabets à mot-clé » (24/09/2026, nuit)

**Idée.** Avec un alphabet libre (26! possibilités), beaucoup de familles sont trop lâches pour être jugées sur les 24 lettres des cribs. Mais Sanborn a **toujours** employé des alphabets à mot-clé : KRYPTOS (K1–K2), ТЕНЬ (*Cyrillic Projector*), GIRASOL (maquette de 1988), SHADOW (petit fragment). Si l'on se limite à ces alphabets, on peut **tous les essayer**. Pour chacun, la clé est connue en 24 points, et des familles lâches deviennent très discriminantes. Quand la clé est entièrement déterminée, on déchiffre même les 97 lettres et on juge l'anglais.

**Statut :** `EXPLORATION RESULT`. Aucun signal ; plusieurs familles réputées intestables sont maintenant fermées pour les alphabets à mot-clé.

## 0. Portée commune

- **Alphabets** : 59 497 mots (corpus anglais de 26 livres, mots de 3 à 15 lettres, plus 60 mots thématiques : KRYPTOS, PALIMPSEST, ABSCISSA, SHADOW, GIRASOL, BERLIN, MEDUSA, WELTZEITUHR…), chacun sous 4 formes : standard (le mot, puis le reste de A–Z), « suite » (le reste repart après la dernière lettre du mot), et leurs inverses. Soit 237 988 alphabets.
- **Types** : Q3 (le même alphabet des deux côtés, comme K1–K2), Q2 (clair A–Z, chiffré à mot-clé, comme le *Cyrillic Projector*), Q1 (clair à mot-clé, chiffré A–Z), Q4a (mot-clé / KRYPTOS), Q4b (KRYPTOS / mot-clé).
- **Conventions** : Vigenère, Beaufort, Beaufort variante.
- **Modèle de l'anglais** : quadrigrammes (log10) tirés du même corpus (`../recuit_2026_09_24/build_qg.c`). L'anglais courant se situe vers −4,1 à −4,3 par quadrigramme, le charabia vers −6 à −7.
- **Témoins** : même balayage complet sur des chiffrés aléatoires de 97 lettres (graines fixées).
- **Contrôles positifs** : un chiffré synthétique de chaque famille, fabriqué avec un alphabet à mot-clé ; chaque programme le retrouve (fichiers `synth_*.txt`).

## 1. Clé à structure de pas 7 (`kwsweep.c`)

Clé(i) = m[i mod 7] + a[segment] : décalage par ligne du cuivre (L), par ligne de 7 (R), ou période 7 pure (P). Pour un alphabet donné, ces modèles imposent 14 à 17 égalités entre les 24 valeurs de clé ; on calcule exactement le plus grand nombre de lettres compatibles.

| | meilleur e_min, K4 | témoin 101 | témoin 202 |
|---|---|---|---|
| L : décalage par ligne du cuivre | 10 | 10 | 9 |
| R : décalage par ligne de 7 | 7 | 7 | 7 |
| P : période 7 pure | 10 | 10 | 10 |

⇒ Sur 3,6 millions de cas, **aucun alphabet à mot-clé** ne rend ces clés plausibles : il faudrait au moins 7 erreurs sur 24. K4 est au niveau du hasard.

## 2. Autoclé sur le CLAIR, écart L = 1 à 13 (`kwautokey.c`)

Pour L ≤ 13, chaque classe modulo L contient une lettre des cribs. On propage le clair depuis les cribs dans les deux sens : **le clair entier est déterminé**. La lettre-clé est lue dans l'alphabet du clair, en A–Z ou en KRYPTOS. Cette famille contient l'hypothèse de la NSA de 1992 (écart 7) restreinte aux alphabets à mot-clé. T18–T19 ne vérifiaient que les cribs ; ici on déchiffre tout.

- **K4** : 120 659 916 cas ; au mieux **4 erreurs** sur 24 (2 cas) ; meilleur texte **−6,38** (charabia).
- **Témoin 303** : au mieux 4 erreurs (8 cas) ; meilleur texte −6,52.
- **Contrôle positif** (PALIMPSEST, écart 7) : 0 erreur, clair retrouvé en entier, −4,29.
⇒ **Éliminé** pour les alphabets à mot-clé.

*Correctif du 25/09 (`../relais_2026_09_25/`, T27).* Ce programme propage depuis la première lettre de crib de chaque classe, sans ré-ancrage : une seule lettre fausse y compte plusieurs fois. Avec le comptage juste, par équations de chaîne, l'écart 7 demande au moins 10 erreurs, et tous les écarts L ≤ 48 au moins 2, comme les témoins. La conclusion ne change pas.

## 3. Clé courante tirée d'un texte anglais inconnu (`kwrunkey.c`)

C'était une famille « impossible à réfuter avec 24 lettres » (base 2 §3, n° 4) : avec un alphabet libre, n'importe quel texte convient. Avec un alphabet à mot-clé fixé, les cribs donnent la clé en deux fragments, de 13 lettres (positions 21–33) et de 11 lettres (63–73). Si la clé est un texte anglais, **ces fragments doivent être de l'anglais**. On note les deux fragments (18 quadrigrammes), à l'endroit et à l'envers, avec la lettre-clé lue dans l'alphabet du mot, en A–Z ou en KRYPTOS.

**Un seul alphabet à mot-clé** (21 418 920 cas) :
- **K4** : meilleur fragment −4,98 (« RYRAWSSOLEADB | HUGHSALLSMA »), du pseudo-anglais.
- **Témoins** : chiffrés aléatoires uniformes (graines 11–13) −5,05 à −5,09 ; **K4 mélangé** (30 graines, `null_shuf_1alpha.txt`) −5,16 à −4,58, médiane −4,89. K4 est sous la médiane.
- **Contrôle positif** (clé tirée de *Sherlock Holmes*, SHADOW) : −4,06, loin devant le reste (−6,2).
- **Puissance** (`power_rk.py`, 20 000 vraies fenêtres anglaises) : une vraie clé anglaise dépasse −4,98 dans **99,5 %** des cas sans erreur, 84 % avec une lettre fausse, 57 % avec deux (médiane −4,13).
⇒ **Éliminé** avec une puissance de 99,5 % (sans erreur) à 84 % (une erreur), pour un alphabet à mot-clé et une clé courante en anglais continu.

**Deux alphabets à mot-clé différents** (un mot thématique parmi 25, sous 4 formes, d'un côté ; n'importe quel mot du dictionnaire de l'autre ; lecture de la clé dans l'un ou l'autre alphabet, en A–Z ou en KRYPTOS ; 1 142 342 400 cas) :
- **K4** : meilleur fragment −4,44 (« EIPASTILLATNO | IPINHISHOBS », alphabets PLUMING et GIRASOL), du pseudo-anglais.
- **Témoins, K4 mélangé** (8, graines 201–208) : −4,78 à −4,40. K4 se classe 2ᵉ sur 9 : **aucun signal**.
- Puissance : une vraie clé anglaise dépasserait −4,44 dans environ 80 % des cas sans erreur, 40 % avec une erreur.

**Leçon de méthode.** Les premiers témoins (lettres tirées uniformément) donnaient des maxima systématiquement plus bas que K4 (−5,05 à −5,09 pour un alphabet ; −4,77 à −4,46 pour deux). Les témoins **K4 mélangé**, qui gardent les fréquences de K4, sont plus hauts. Avec eux, K4 est banal : pour un alphabet, 20 témoins sur 30 font mieux que lui (`null_shuf_1alpha.txt`). Pour les tests qui notent un texte, il faut des témoins mélangés.

**Portée.** Clé courante en anglais **continu**. Une clé faite de mots codés, avec des X séparateurs, ou de langue non anglaise, n'est pas couverte, pas plus qu'un alphabet absent de la liste ou construit autrement (deux mots-clés, ordre en colonnes).

## 4. Autoclé sur le CHIFFRÉ, écart L = 1 à 96 (`kwcak.c`)

Avec un alphabet fixé, le clair des positions ≥ L est entièrement déterminé par le chiffré gravé. C'est ce qui manquait aux écarts longs : avec un alphabet libre, T23 y est sans puissance (`../erreurs_multiples_2026_09_24/`, §2).
- **Contrôle positif** (GIRASOL, écart 37) : 11 lettres des cribs sur 11, clair anglais −4,21.
- **K4** : 1 028 108 160 cas ; jamais plus de 10 lettres des cribs sur 24 (témoin : 9) ; aux écarts courts, texte en charabia (−6,7 à −7,1). Les meilleurs scores (−4,78 pour K4, −4,89 pour le témoin) viennent des écarts longs, où l'on ne note qu'une vingtaine de lettres.
⇒ **Éliminé** pour les alphabets à mot-clé, à tous les écarts.

## 5. Bilan

- Avec les alphabets que Sanborn employait réellement, quatre familles tombent, dont deux jusqu'ici indécidables : la **clé courante tirée d'un texte anglais inconnu** (puissance 99,5 % sans erreur, 84 % avec une) et l'**autoclé sur le chiffré aux écarts longs**.
- Les clés à pas 7 (décalage par ligne) et l'autoclé sur le clair (écarts 1–13, dont l'hypothèse de la NSA) sont exclues avec déchiffrement complet, et non plus seulement sur les cribs.
- Rien n'est conclu pour les alphabets **non** construits sur un mot-clé.
