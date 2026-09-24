# Audit « erreurs multiples » (24/09/2026, soir)

**Question.** Le petit fragment de Sanborn (97 lettres, méthode de K1) compte 4 erreurs de chiffrement (base 7 §9.4). T19–T21 ont rejoué les éliminations avec **une** erreur. On mesure ici, pour chaque famille, le **nombre minimal d'erreurs** qu'il faudrait admettre dans les 24 lettres des cribs pour que la famille devienne compatible (e_min), et on le compare au hasard.

**Statut :** `EXPLORATION RESULT`. Aucun clair, aucun signal. Les négatifs sont conservés avec leur portée.

## 0. Outillage

| Fichier | Rôle |
|---|---|
| `emin_cpsat.py` | e_min exact par CP-SAT (OR-Tools 9.15) : alphabets inconnus (valeurs toutes différentes), clé structurée, une variable booléenne « lettre juste » par position de crib, on maximise leur nombre. Types Q3 (σ/σ), Q4 (σ1/σ2), Q2 (A–Z/σ), Q1 (σ/A–Z) ; conventions VIG, BEAU, VARB |
| `imx.h`, `emin.h`, `t22_periodique.c` | même calcul en C (ensembles couvrants implicites). Il concorde avec CP-SAT (p = 5, 7, 12) mais bute sur des cas lâches ; CP-SAT a été retenu |
| `families.py`, `run_families.py` | familles de clés et exécution parallèle avec témoins |
| `t23_autocle_chiffre.py`, `t23_all_L.py` | autoclé sur le chiffré, avec erreurs |
| `t24_perline.py` | clé de période p + décalage de l'alphabet à chaque ligne du cuivre |
| `t25_retournement.py` | « on retourne la feuille » : la convention change selon la ligne |
| `empreinte_doublets.py` | doublets et phases de clé dans les chiffres de Sanborn ; filtre des alphabets |
| `corpus_get.sh` | corpus anglais (26 livres du projet Gutenberg, 22 Mo) pour les bigrammes |

**Témoins.** Lettres chiffrées tirées au hasard aux 24 positions des cribs (chiffré complet pour l'autoclé, dont la clé est le chiffré). Colonne « P(témoin ≤ K4) » : part des témoins qui demandent au plus autant d'erreurs que K4. Une valeur petite signale un K4 anormalement proche de la famille.

## 1. Clé périodique, p = 1 à 26, tous types, avec erreurs (T22, `res_periodic.jsonl`)

156 cas (26 périodes × Q3 VIG, Q3 BEAU, Q4, Q2 VIG, Q2 BEAU, Q1), 200 témoins chacun.

- **Aucune période ne devient plausible en admettant des erreurs.** Pour presque toutes, K4 demande autant d'erreurs que les témoins, ou plus : P(témoin ≤ K4) entre 0,5 et 1.
- Seules exceptions : **p = 19** (P = 0,01 à 0,035 selon le type), qui est l'égalité connue de Bean (27 et 65 : R → P, écart 38 = 2 × 19), et Q1 à p = 2 et p = 4 (0,05 et 0,075). Sur 156 essais, c'est ce qu'on attend du hasard.
- **Période 7 : K4 est moins compatible que le hasard.** Q3 VIG : 4 erreurs (retirer 32, 66, 72, 73), contre 2,6 en moyenne pour les témoins (P = 0,92). Q4 : 3 erreurs, P = 0,97.
- Portée : le registre disait « éliminé sans erreur, et avec une erreur pour p ≤ 11 ». Il faut lire désormais : **K4 n'est, pour aucune période de 1 à 26 et dans aucun type Quagmire, un chiffre périodique affecté de quelques erreurs**, avec un alphabet quelconque.

## 2. Autoclé sur le CHIFFRÉ, avec erreurs (T23)

C'était la seconde hypothèse de la NSA (1992) pour l'intervalle 7. Elle était éliminée sans erreur par un seul argument (22 donne A et 72 donne C, qui devraient tous deux valoir « 0 »), donc elle pouvait se rouvrir avec une seule erreur. Tout le chiffré étant gravé, la clé est connue et seul σ est inconnu : le test est puissant.

- **Écart 7** : K4 demande **au moins 5 erreurs** sur 24 dans les 9 variantes (clé lue dans σ, en A–Z ou en KRYPTOS ; VIG, BEAU, VARB). Détail : σ 8 / 6 / 5 ; A–Z 7 / 6 / 7 ; KRYPTOS 6 / 5 / 6.
- **Tous les écarts** (`res_t23_allL.jsonl`, K4 seul ; écarts 1 à 15 au moment de la rédaction, le calcul continue) : jamais moins de 4 erreurs.
- ⇒ **Éliminé de façon robuste** : il faudrait 4 à 5 erreurs dans les 24 lettres des cribs, alors que le taux du fragment (4 %) en prévoit environ une.

## 3. Période 7 avec décalage à chaque ligne du cuivre (T24)

**Motif.** Les deux conflits qui interdisent la période 7 enjambent tous les deux une fin de ligne du cuivre (lignes de 31) : 65 est sur la ligne 27 et 72 sur la ligne 28 ; 24 est sur la ligne 26 et 66 sur la ligne 28. La feuille de K3 + K4 fait 31 × 14. Une clé de période 7 dont l'alignement change à chaque ligne les résout tous les deux.

**Résultat** (K4) : Q4 1 erreur (en 28), Q3 BEAU 1, Q3 VIG 1 ou 2 (non prouvé), Q2 et Q1 3.
**Témoins** (Q3 VIG, 50) : 26 % sont compatibles **sans** erreur, 36 % avec une.
⇒ **Famille trop lâche pour les 24 lettres, et K4 n'y fait pas mieux que le hasard.** Aucun signal.

## 4. « On retourne la feuille » : la convention change selon la ligne (T25)

**Motif.** Sanborn, Big Techday 2013 : on retourne la feuille, on la met à l'envers. Un tableau retourné se lit en Beaufort ou en Beaufort variante. Clé de période 7, alphabet quelconque, convention fixée par ligne.

- Par ligne du cuivre (27 motifs) : **au mieux 2 erreurs**.
- Par ligne de 7, toutes phases (plus de 500 motifs) : au mieux 1 erreur (en 73, 32 ou 31), pour quatre motifs sans régularité. Les témoins de ces motifs atteignent 0 ou 1 erreur dans 20 à 30 % des cas.
- En alternance lettre à lettre (période 2 ou 3) : au mieux 3.
⇒ **Aucun signal.**

## 5. L'empreinte des doublets dans les chiffres de Sanborn (`results_empreinte_doublets.txt`)

**Constat nouveau.** Dans un Quagmire périodique, un doublet c_i = c_{i+1} se produit quand σ(p_{i+1}) − σ(p_i) = k_i − k_{i+1}. Les doublets s'accumulent donc aux phases de la clé où cette différence tombe sur une différence fréquente des bigrammes anglais.

- **Petit fragment de Sanborn** (97 lettres, KRYPTOS, SHADOW, période 6) : **7 doublets** (3,7 attendus), **tous sur 2 des 6 phases**. En phase 2 (clé A → D, différence 23, soit −3 dans l'alphabet KRYPTOS), on trouve TR, SP, AT, AT. En phase 0 (S → H), on trouve NE, NE et ON ; ce dernier doublet est créé par l'erreur en 91.
- **K1 et K2** : la même différence 23 donne de nouveau des doublets (HE dans K1 ; AT, BO, HE dans K2). Dans l'alphabet KRYPTOS, A → T → R et S → P sont à 3 rangs d'écart, d'où la résonance avec AT, TR et SP.
- Le contrôle K1–K3 du 24/09 (base 8 §5) cherchait les doublets **modulo 7**. Il ne pouvait pas voir cet effet, qui se lit modulo la période de chaque section.

**Conséquences pour K4.**
1. Si les 5 doublets de K4 en phase 4 (mod 7) viennent d'une différence de clé fixe d entre les colonnes 4 et 5 de chaque ligne de 7, les trois doublets des cribs imposent à l'alphabet du clair : **σ(N) − σ(I) = σ(O) − σ(N) = σ(T) − σ(S) = d**.
2. **Filtre** : 0,54 % des 118 958 alphabets à mot-clé tirés du corpus le passent. Ni A–Z ni KRYPTOS. Parmi les mots thématiques : **GIRASOL** (alphabet standard, d = 15), l'alphabet de la maquette de 1988, et BERLIN (mode « suite »), qui passe pour une raison banale : le mot finit par IN, que suit le O. Sur environ 80 mots thématiques essayés, en trouver un ou deux est attendu : **p ≈ 0,07, pas un signal**.
3. **La résonance ne suffit pas.** Un alphabet réaliste fait tomber 6 à 7,5 % des bigrammes anglais sur une même différence (A–Z 6,8 %, KRYPTOS 6,2 %, GIRASOL 7,0 %) ; le meilleur alphabet possible, 16 % (14 % avec la contrainte ci-dessus). Avec 7 %, 5 doublets sur les 14 cases de la phase 4 ont une probabilité de **0,002** (0,04 avec l'alphabet optimal ; 0,0001 au hasard). La concentration de K4 demande donc une structure de pas 7 dans la clé elle-même, et pas seulement un alphabet favorable. *Mais* le fragment de Sanborn fait lui-même un tirage d'improbabilité voisine (4 doublets sur 16 cases d'une phase, p ≈ 0,015) : une telle concentration arrive dans ses vrais chiffres.

## 6. Bilan

- **Robustesse étendue** : clé périodique (p ≤ 26, Q1–Q4) et autoclé sur le chiffré (tous écarts testés) restent éliminées **quel que soit le nombre d'erreurs plausible**. K4 n'est jamais anormalement proche de ces familles.
- **Trois modèles « 7 » nouveaux, motivés par la mise en page ou par Sanborn** (décalage par ligne, retournement par ligne du cuivre, retournement par ligne de 7) : aucun signal ; les deux premiers sont trop lâches pour les 24 lettres.
- **Un lien nouveau** : l'empreinte des doublets par phase de clé, visible dans les trois chiffres périodiques de Sanborn en alphabet KRYPTOS. Elle confirme que les doublets alignés de K4 désignent une clé à structure de pas 7. Mais elle montre aussi qu'un alphabet favorable ne suffit pas à les expliquer.
- **Limite** : les familles de pas 7 qui résolvent les conflits (décalages, retournements) sont trop lâches pour les cribs. Seules les 73 lettres hors cribs pourraient trancher. L'attaque par recuit simulé sur le texte entier (`../recuit_2026_09_24/`) ne résout pas encore un contrôle positif de période 7 sur 97 lettres : aucune conclusion n'en est tirée pour K4.
