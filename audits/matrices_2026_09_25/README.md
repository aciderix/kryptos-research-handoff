# Alphabets « en matrice » (25/09/2026)

**Idée.** Sanborn parle de matrices « qu'on tourne et retourne » (2013, 2019) et de « la matrice d'origine » passée par « tous les shifts » (2003). Traduction concrète classique : l'alphabet à mot-clé est écrit en lignes dans une grille de largeur w, puis relu par colonnes (de gauche à droite ou de droite à gauche, de haut en bas ou de bas en haut, ou dans l'ordre alphabétique des lettres du mot-clé). Ces alphabets n'avaient jamais été testés systématiquement ; les balayages du 24/09 n'employaient que des alphabets à mot-clé linéaires.

**Alphabets** (`gen_alphabets.py`, à partir des 59 497 mots de `../motcle_pas7_2026_09_24/words.txt`) : w = 2 à 13, 5 lectures, doublons retirés : **1 294 102 alphabets distincts** (plus leurs inverses). Exemple : KRYPTOS en 7 colonnes donne KAHURBIVYCJWPDLXTEMZOFNSGQ. Le fichier (35 Mo) n'est pas versé : `python3 gen_alphabets.py ../motcle_pas7_2026_09_24/words.txt` le régénère.

**Tests** : les mêmes programmes que l'audit « mot-clé » (copiés ici avec l'option ONLYSTD et des témoins « K4 mélangé » partout). Types Q3, Q2, Q1, Q4a, Q4b ; Vigenère, Beaufort, variante.

| Test | K4 | Témoin(s) « K4 mélangé » | Verdict |
|---|---|---|---|
| Clé courante tirée d'un texte anglais (`kwrunkey`, 233 M cas) | −4,72 | 10 graines : −4,83 à −4,26 | aucun signal |
| Autoclé sur le clair, L = 1–13, clair entier (`kwautokey`, 1,3 G cas) | au mieux 3 erreurs (un cas, clair en charabia, −7,24) ; meilleur texte −6,32 | au mieux 3 à 4 erreurs ; −6,27 | aucun signal |
| Autoclé sur le chiffré, L = 1–96 (`kwcak`) | ≤ 10 lettres des cribs sur 24, charabia | idem | éliminé |
| Clé période 7 + décalage (`kwsweep`) : par ligne du cuivre / par ligne de 7 / période pure | 9 / 6 / 9 erreurs | 9 / 6 / 9 | éliminé |

⇒ Les alphabets en matrice ne rouvrent aucune famille : K4 est partout au niveau des témoins.
