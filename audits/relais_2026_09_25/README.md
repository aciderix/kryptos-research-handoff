# Relais DeepSeek n° 2 (« Recoupement général : ce que la communauté n'a pas vu », 25/09/2026) : vérification point par point

**Statut :** `EXPLORATION RESULT`. Aucun signal. Le texte reformule surtout la base 7 §9 et §12 (autre session). Ses ajouts sont faux, invérifiables ou relèvent de la numérologie. La seule proposition testable (§3 ci-dessous) est faite, et ne rouvre rien.

**Outil.** Tout le calcul est en C + OpenMP : gcc 13.3, libgomp, 4 threads, AVX-512, compilation `gcc -O3 -march=native -fopenmp`. Le moteur commun de la branche est `../moteur_2026_09_25/k4x.c`.

## Point par point

| n° | Proposition | Vérification | Verdict |
|---|---|---|---|
| 1 | Les 8 signes raturés sous VTTMZFPK correspondent exactement à LINCLOCK (66–73) | C'est le rapprochement de la base 7 §12.2, fait par l'autre session. La table de reconnaissance existe (`../erreurs_multiples_2026_09_24/results_huit_signes.txt`) | **Déjà au dépôt.** Seule action : la photo demandée à Paradigm |
| 2 | Doublets en colonne 4 des blocs de 7 ⇒ clé à pas 7 ; pas de clé 1, 1, 5 = Fibonacci | Mécanisme déjà décrit (base 7 §12.1). **Les « pas de clé » 1, 1, 5 ne viennent pas de la clé.** À un doublet, c_i = c_{i+1} impose k_i − k_{i+1} = σ(p_{i+1}) − σ(p_i) : ce sont les écarts des lettres claires N → O, S → T, I → N dans l'alphabet choisi. En A–Z Vigenère : 1, 1, 5 ; en Beaufort A–Z : 25, 25, 21 ; en KRYPTOS : 12, 24, 4. Les familles « à pas 7 » ont été testées : T11, T11b, T16, T24–T26, alphabets à mot-clé (`../motcle_pas7_2026_09_24/` §1 : au moins 7 erreurs) | **Fibonacci : artefact** du choix de A–Z et des mots des cribs. Le reste est déjà testé |
| 3 | Les auto-chiffrements 32 (S → S) et 73 (K → K) : rejouer T18 et T11b en supposant que l'un est une lettre **recopiée**, les autres contraintes étant maintenues | **Déjà fait**, trois fois : T19 (T18 rouvert par une copie en 73 en Vigenère, en 32 en variante, mais 78 à 97 % des chiffrés aléatoires rouvrent aussi avec une erreur) ; T20 modèle B « copie » (T11b : la plupart des cas rouverts passent par 32, au niveau du hasard). **Complément du jour, T27**, pour la seule forme qui a du pouvoir, c'est-à-dire avec les alphabets à mot-clé (voir plus bas) : à l'écart 7, il faut **au moins 10 erreurs** ; **aucun** cas n'est sauvé par une copie en 32 ou 73, ni pour K4, ni pour les 15 témoins | **Rien ne rouvre** |
| 4 | Le fragment de 97 lettres est un modèle réduit de K4 | Le tableau **mélange deux fragments**. Les lettres doublées de remplissage (SS, LLL, II, EE) et la décimation en 16 colonnes sont ceux du fragment de **Zola** (512 lettres), pas du petit fragment de 97 lettres (base 7 §9.3–9.4). « K4 a 6 doublets comme SHADOW a 6 lettres » : numérologie. La période 6 est éliminée pour tout alphabet, même avec plusieurs erreurs (T22). Ce que le fragment apporte vraiment est déjà noté : le format de 97 lettres, un clair tronqué, environ 4 % d'erreurs | **Rien de nouveau ; une confusion factuelle** |
| 5 | Le « 1-2-3 » de Sanborn = décalages des lignes du tableau ⇒ clé construite par décalages successifs | Une clé qui avance d'un cran à chaque lettre, ou par bloc, est la clé progressive (éliminée pour tout alphabet le 22/09) ou la famille T11 (s·j par bloc). « Le masque change la base » : non formulé, donc non testable | **Déjà couvert** |
| 6 | « Compass cipher » ; les 8 signes seraient 8 directions | Seule une photo des signes peut le dire (base 7 §12.5 : numérologie pour « 66–67° » et « 8 K = 8 directions ») | **Invérifiable** en l'état |
| 7 | Retirer SS en 42 : 5 doublets sur 5 en colonne 4, p = 1/7⁵ ≈ 6 × 10⁻⁵ | **Deux fautes.** (a) La colonne est choisie après coup : la probabilité que 5 doublets donnés tombent dans **une même** classe est 7 × (1/7)⁵ = 1/2401, pas 1/16 807. (b) On ne retire pas l'exception parce qu'elle gêne : la statistique honnête reste « au moins 5 sur 6 dans une classe », déjà calculée (p ≈ 6 × 10⁻⁴ pour le module 7, ≈ 0,02 en balayant le module, base 7 §7.2). Rien ne désigne 42 comme une erreur ; hors cribs, rien ne peut le tester | **Faux** |
| 8 | Chercher si les pas de clé des paires à l'écart 7 forment une suite | Une différence de clé n'est calculable que si les deux positions sont dans les cribs. Parmi les 8 paires, une seule l'est : 65/72 (P → R et P → C). C'est justement la paire qui interdit toute substitution qui ne dépend que de i mod 7. Pour les autres, le clair est inconnu | **Non calculable** |
| 9 | Les 8 K de K4 marqueraient les positions de clé nulle | Il y a bien 8 K (2, 31, 45, 52, 73, 77, 86, 93). Mais en Vigenère ou en variante, quel que soit l'alphabet, clé nulle ⇔ chiffré = clair. **En 31, le chiffré K couvre le clair A** : la clé n'y est pas nulle. Seul 73 est K → K. 4 des 8 K sont ≡ 3 (mod 7), ce qui arrive au hasard avec p ≈ 0,1 | **Réfuté par le crib** |

## T27 — autoclé sur le clair, alphabets à mot-clé, erreurs comptées correctement (`t27_autocle_motcle_chaines.c`)

**Défaut trouvé dans deux programmes antérieurs.**
- `../motcle_pas7_2026_09_24/kwautokey.c` et la famille `a` de `../moteur_2026_09_25/k4x.c` propagent le clair depuis la **première** lettre de crib de chaque classe, sans ré-ancrage.
- En autoclé, une lettre de chiffré fausse corrompt toute la suite de sa chaîne. **Une seule erreur peut donc compter plusieurs fois.**

**Méthode juste.** On ré-ancre la chaîne à chaque lettre de crib. Entre deux positions de crib consécutives a < b d'une même classe, on propage depuis le clair connu en a et l'on compare en b. Chaque équation (a → b) n'est faussée que par une erreur dans ]a, b], et changer c_b suffit à la réparer. Donc **nombre minimal d'erreurs = nombre d'équations fausses**.
- Contrôle positif n° 4 : une lettre fausse en 42, au milieu d'une chaîne. Le nouveau comptage donne 1 erreur, l'ancien 2.
- `k4x.c` est corrigé en une ligne (ré-ancrage). Après correction, la famille `a` donne pour K4 **4 erreurs**, pour 4 à 5 chez les témoins (p = 0,29). Les conclusions de ces audits ne changent pas.

**Portée.**
- 202 172 alphabets distincts : mots du dictionnaire et mots thématiques, sous 4 formes.
- 5 types (Q3, Q2, Q1, Q4a, Q4b) ; lettre-clé lue en σ, en A–Z ou en KRYPTOS ; VIG, BEAU, VARB.
- Écarts L = 1 à 48, seulement les cas à au moins 8 équations : 228 656 532 cas par chiffré, environ 12 s sur 4 threads.
- Pour les cas à 0 ou 1 erreur avec L ≤ 13, on déchiffre les 97 lettres et on note l'anglais (quadrigrammes).

**Contrôles positifs.**
- PALIMPSEST, écart 7, VIG, **une copie en 73** : retrouvé seul à 1 erreur, marqué « copie », clair anglais −3,99.
- Même contrôle avec une lettre fausse en 42 : retrouvé à 1 erreur, −4,07.

**Résultats** (`results_t27.txt`).

| | Écart 7, erreurs minimales | Tous écarts, minimum | Sauvés par une copie en 32 ou 73 |
|---|---|---|---|
| K4 | **10** (sur 17 équations) | 2 (3 cas) | **0** |
| 10 témoins K4 mélangé | 9 à 10 | 2 à 3 | 0 |
| 5 témoins uniformes | 9 à 10 | 1 à 2 | 0 |

⇒ L'hypothèse de la NSA (autoclé sur le clair à l'écart 7) est **exclue pour les alphabets à mot-clé**. Il faudrait au moins 10 lettres fausses sur 24, et une copie en 32 ou 73 n'y change rien. Sur l'ensemble des écarts, K4 est au niveau du hasard. Avec un alphabet **libre**, T19 reste valable : une seule erreur rouvre la famille, comme pour un texte aléatoire, donc le test y est sans pouvoir.
