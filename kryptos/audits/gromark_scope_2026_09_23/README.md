# Gromark : portée exacte (23/09)

Question : pourquoi le carnet trouve 0 amorce Gromark compatible et Bean (2021) en trouve 39 ?

1. **Reproduction de Bean** : `gt.c` (https://github.com/RichardBean/k4testing), `./gt 10 5` → **39 amorces** (liste dans `bean_gt_10_5_primers.txt`). Modèle : base 10, amorce de 5 chiffres, **alphabets clair et chiffré tous deux libres**.
2. **Pourquoi le carnet trouve 0** : il fixe l'alphabet clair à A–Z (Gromark ACA standard). Preuve à la main : le clair R (position 27) et le clair C (position 72) donnent tous deux P. Il faut donc k72 − k27 ≡ 17 − 2 = 15 (mod 26), impossible avec des chiffres de 0 à 9 (et plus généralement en base ≤ 11). Bean le signale lui-même (« C and R… more than 10 places apart »). **Les deux résultats sont justes : leurs portées diffèrent.**
3. **Test fermé, défini avant exécution** (`gromark_fixed_side.py`, `gromark_keyword_alphabets.py`) : toutes les amorces 00000–99999, un côté fixé par un alphabet de l'œuvre, l'autre libre. Alphabets essayés :
   - l'alphabet normal A–Z et l'alphabet KRYPTOS (KA) ;
   - les alphabets à mot-clé PALIMPSEST et ABSCISSA ;
   - les inverses de tous ces alphabets.
   Tous les couples AZ/KA sont aussi testés. **Résultat : 0 partout.** Contrôles positifs : 5/5.
4. **Contrôle nul** (`gromark_null_random_alphabets.py`) : avec un côté fixé par un alphabet **aléatoire**, 0 amorce compatible sur 200 alphabets. Fixer un côté est donc si contraignant que le zéro est attendu. **Portée :** le Gromark (base 10, amorce de 5) n'est possible qu'avec **deux alphabets tous deux étrangers à l'œuvre connue**. Dans ce cas, les 24 lettres ne suffisent pas à le trancher (classe « non réfutable », comme Quagmire IV à période moyenne).
5. **Registre du dépôt** : `docs/two_systems_landscape.md` indique « Vimark / Gromark : STRUCTURAL, zero consistent primers ». Cette élimination ne vaut que pour des **alphabets fixés** (classe additive). Avec deux alphabets libres, elle est **fausse** (39 amorces, reproduites).
