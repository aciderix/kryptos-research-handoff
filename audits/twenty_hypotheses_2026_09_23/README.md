# Catalogue « 20 hypothèses » (relais du 23/09, nuit) : tri et tests

## Tri (sans calcul)
| # | Piste | Statut | Raison |
|---|---|---|---|
| 1 | Deux pièces indépendantes (lignes 25–26 / 27–28) | **couvert** | chaque crib seul, `incrib` (clés indépendantes), `../cribs_separately_2026_09_23/` |
| 4 | Alphabet FUMEE | **couvert** | tout alphabet à mot-clé ≤ 12 lettres et tout alphabet quelconque déjà testés |
| 5 | DRYAD avec le tableau | **couvert** | lignes du tableau = KRYPTOS décalé ⇒ Quagmire III, clé = choix de ligne (périodique : éliminé ; libre : indécidable) |
| 6 | Polybe 5×5 + horloge base 5 | **impossible** | K4 contient les 26 lettres, dont I **et** J : un système à 25 lettres ne peut pas les produire |
| 7 | DIANA (A+B=Z) interrompue par les E | **couvert** | DIANA = Beaufort à décalage constant ; clés interrompues testées (8 modèles) |
| 9 | Vecteur ENE (+2 col, −1 ligne) sur le tableau | **couvert** | sur le tableau (KRYPTOS décalé d'un cran par ligne), tout pas constant donne une clé progressive (+1 par lettre) : famille progressive éliminée pour tout pas |
| 10 | Strates (blocs + décalage) | **couvert** | clé par paliers, 0/13 650 (`../stepped_key_2026_09_23/`) |
| 11 | Deux bassins (spirale puis miroir) | non testable | règle non définie |
| 12 | Joint (seam) comme réglette | non testable | nécessite la mesure physique |
| 13 | Sentinelle « L / » comme décalage initial | **couvert** | décalage constant absorbé par l'alphabet libre / la clé |
| 16 | Miroir Morse appliqué au mot-clé (KRQPTOS) | **couvert** | alphabet quelconque |
| 17 | Alphabet réordonné par les intervalles des E | **couvert** | alphabet quelconque |
| 18 | Autoclave amorcé par le Q de K3 | **couvert** | autoclave impossible pour toute amorce (base 2) |

## Tests lancés (paramètres fixés avant calcul) — `tests.py`
- **#2** substitution partitionnée par les lettres « à îlot fermé » {A, B, D, O, P, Q, R} : clé = k[groupe][i mod p], QIII tout alphabet, p 1–26.
- **#3** décalage par **ligne physique** du cuivre (4 + 31 + 31 + 31) + clé périodique : clé = r[ligne] + k[i mod p], QIII tout alphabet, p 1–26.
- **#8** chiffres des coordonnées de K2 (3 8 5 7 6 5 7 7 8 4 4) comme clé numérique (et inversée), toutes phases, QI/QII/QIV tout alphabet.
- **#14** permutations modulaires sur 97 : x → a·x (a = 1–96), x → a(x+1) − 1 (forme de K3), x → g^x (32 générateurs) et leurs logarithmes,
  avant ou après une substitution QIII tout alphabet, p 1–26 ; et x → a·x + b (toutes les 9 312) avec alphabets fixés A–Z/KRYPTOS.
- **#15** générateur congruentiel X(n+1) = a·X(n) + c mod 26 comme clé : toutes graines, a, c (alphabets fixés) ; graine L (11) avec tout alphabet (QIII).
- **#19** transposition par colonnes à clé DYAHR / YAR, avant ou après QIII tout alphabet, p 1–26.
- **#20** substitution selon la ligne physique (« ID BY ROWS ») puis lecture en colonnes de bas en haut sur la largeur 31 (geste de K3), QIII tout alphabet.
Témoins : chiffrés aléatoires pour chaque configuration où K4 passe ; verdict final comparé au hasard.
