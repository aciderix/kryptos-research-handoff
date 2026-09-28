# Z32-E04 — RÉSULTATS (2026-09-28) : audit du « biais structurel » de Stampher — enrichissement réel, **ciblage des crimes non soutenu**

Pré-inscription : `PREREGISTRATION.md` (+ amendement 1, commité avant le calcul supplémentaire). Outil : `tools/stampher_audit.c`
(reproduction exacte de z32.py : 2 044 224 phrases ; constantes et projection de data.py/geo.py) ; sortie : `logs/e04.out`.

## 1. D'où vient l'enrichissement 8 h + 10 h ?
| Ensemble | Phrases | Part 8 h + 10 h |
|---|---|---|
| (a) 32 lettres | 154 572 | 0,168 |
| (b) + limites de carte | 110 910 | **0,206** (l'espérance « aléatoire » 2/12 = 0,167 de Stampher est donc un peu fausse) |
| (c) + verrous (sans carte) | 61 | **0,852** |
| (d) + verrous + carte (les 54 survivants) | 54 | 0,870 (reproduit : 47/54) |
Enrichissement annoncé 5,2× ; enrichissement réellement dû aux verrous (d)/(b) = **4,2×**, p = 1·10⁻⁵ (tirage de 54 phrases
dans (b)). ⇒ l'effet vient bien des **verrous**, pas de la carte (il existe déjà en (c)).

## 2. Ce « ciblage » est-il propre aux répétitions du Z32 ? (amendement 1)
100 000 motifs de 3 paires de positions tirées au hasard, même grammaire, même carte ; 66 100 motifs laissent ≥ 10 survivants.
- p1 = P(2 heures dominantes ≥ 87 % des survivants) = **0,24** : la concentration sur deux heures est **banale** ; c'est un
  effet d'orthographe (quelques mots-nombres ont les bonnes lettres aux bonnes places, et toute la grammaire les répète).
- Heures des 4 scènes retenues par Stampher, vues du mont Diablo : 10,09 ; 10,04 ; 10,77 ; 8,03 ⇒ C = {8, 10, 11}.
  p2 = P(concentration ≥ 87 % **et** les 2 heures dans C) = **0,0167**.
- Règle pré-inscrite : « ciblage des crimes » non soutenu si p2 > 0,01 ⇒ **non soutenu**. (Et p2 est encore optimiste pour
  l'affirmation : le choix des scènes et du seuil 87 % vient de Stampher, après avoir vu ses survivants.)

## 3. Conclusion
- L'enrichissement existe, mais c'est une propriété de **l'orthographe des mots-nombres face à 3 égalités de positions**, pas
  un indice que le chiffre « vise » les lieux des crimes : environ 1 motif aléatoire sur 60 produit la même coïncidence.
- La « probabilité jointe inférieure à un sur mille milliards » de son README multiplie des indices choisis après coup ; le
  présent audit montre qu'au moins l'un d'eux (le biais structurel, compté 5,2×) est surévalué et n'est pas significatif
  au seuil pré-inscrit une fois le hasard correctement modélisé.
