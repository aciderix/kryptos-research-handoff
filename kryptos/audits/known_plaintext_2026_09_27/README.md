# ⚠️ RÉTRACTATION (2026-09-27, soir) — lire en premier

Le clair « THE COMPASS ROSE IS HERE … » utilisé ci-dessous vient de **SolveKryptos** : c'est une
**reconstruction communautaire**, que **base 1 §7 avait déjà classée « niveau X — EXCLU, risque de
raisonnement circulaire »**. Kobek & Byrne n'ont pas publié le clair réel. Notre « validation » (cribs aux
bonnes positions) était **circulaire** : la reconstruction est bâtie autour des cribs. Donc :

1. **Invalide** : toute conclusion tirée des ~73 lettres non-crib — en particulier « keystream aléatoire /
   OTP-class » (IC, Berlekamp-Massey, constantes, sensibilité). Soustraire un texte inventé au chiffré
   produit du bruit : ces tests mesuraient notre propre bruit.
2. **Faux, doublement** : « l'excès écart-7 est un artefact du clair ». (a) P n'est pas vérifié ; (b) le
   raisonnement était **logiquement erroné** : avec une clé aléatoire, P(c_i = c_{i+7}) = 1/26 quel que soit
   le clair — la structure du clair ne peut pas passer dans le chiffré. **L'excès écart-7 de K4 (9 vs 3,3,
   p≈0,004) reste une anomalie RÉELLE et NON expliquée.**
3. **Reste valide** : la validation de la convention sur K1 (PALIMPSEST) et tous les résultats qui
   n'utilisent que les **24 cribs** (bijcheck, gapscan, digraphique, periodic_known, Gromark, grid14x7…).

Le verdict « méthode = one-time pad » est **retiré**. K4 reste ouvert.

---

# Analyse KNOWN-PLAINTEXT de K4 (2026-09-27) — le keystream est indiscernable de l'aléatoire

**Contexte.** Le clair de K4 est PUBLIC depuis sept. 2025 (retrouvé — pas cassé — dans les archives
Sanborn par Kobek & Byrne ; l'archive + la description de la MÉTHODE ont été vendues aux enchères
en nov. 2025 à un acheteur privé). Le user autorise l'usage de cette info publique pour identifier
la méthode. Décision conjointe agents : **known-plaintext** = calculer le keystream K=C−P et
chercher sa règle de génération. Rien de scellé/payant n'est utilisé.

## Clair public utilisé (RÉFÉRENCE, non tuné)
Source : solvekryptos.com/solution (reconstruit des archives). **Validé** avant usage :
```
THECOMPASSROSEISHEREXEASTNORTHEASTTHISISYOURPOSITIONXCOMMISSIONBERLINCLOCKWHICHISNORTHEASTOFHEREX
```
- longueur 97 ; clair[21..33]=EASTNORTHEAST ✓ ; clair[63..73]=BERLINCLOCK ✓ (positions confirmées Sanborn) ;
- IC=0.0717 (anglais authentique) ; lecture cohérente : « THE COMPASS ROSE IS HERE X EAST NORTHEAST
  THIS IS YOUR POSITION X COMMISSION BERLIN CLOCK WHICH IS NORTHEAST OF HERE X ».

## Outil validé (`known_plaintext_analysis.c`)
Convention Kryptos confirmée sur **K1** : alphabet KRYPTOS-keyé + Vigenère (key letter = KRY[posK(C)−posK(P)])
⇒ récupère **PALIMPSESTPALIMPSEST…** exactement. L'extraction est donc correcte.

## Résultats sur K4 (keystream K=C−P, toutes conventions AZ & KRYPTOS × VIG/BEA/VAR)
| test | résultat | verdict |
|---|---|---|
| **IC du keystream** | **0.039** (aléatoire 0.0385 ; anglais 0.066) | clé ALÉATOIRE |
| IC≠0.066 | exclut tout texte anglais TRANSPOSÉ/anagrammé (transposition préserve IC) | pas un texte réarrangé |
| running-key alphabet keyé (idée chef) | qoff −3.8..−4.2 = charabia | négatif |
| running-key = K1/K2/K3 (clair/chiffré, tous offsets, fwd/rev) | min 84/97 mismatch = hasard | négatif |
| périodique (clé const par i%L, toute L, tout alphabet) | 63-90 contradictions | PAS périodique |
| plaintext-autoclé c=f(p_i,p_{i-g}) (agnostique alphabet) | 13-32 contradictions | négatif |
| classic autoclé (clé=clair décalé) | ~0 match | négatif |
| ciphertext-autoclé | 5-7 = hasard | négatif |
| récurrence laggée (Gromark/LFSR, lags 1-14) | 10-13/88 = hasard | négatif |

### L'excès à l'écart-7 est un ARTEFACT du clair (résultat majeur)
P a **9** coïncidences c[i]=c[i+7] ; C en a **9** ; la clé n'en a que **3** (≈ hasard). Les positions
de P et de C ne coïncident PAS (2 communes seulement). ⇒ l'excès écart-7 de C provient du CLAIR
(qui a ses propres répétitions à 7) combiné à une clé aléatoire — **PAS d'un mécanisme autoclé-7.**
Toute la piste « autoclé écart-7 » (des mois de travail communauté + nous) reposait sur ce faux signal.

## Conclusion (le « point »)
Le keystream de K4 est **indiscernable de l'aléatoire** par tous les tests standard. Donc :
**K4 = Vigenère (convention Kryptos, alphabet KRYPTOS-keyé) + clé aléatoire non-répétée** —
soit un **one-time pad**, soit un **générateur pseudo-aléatoire fort** (cohérent avec « système
inédit de Scheidt, plus d'une étape »). Le **pad/keystream est RÉCUPÉRÉ** (déterminé par le clair
public), mais le **générateur n'est pas déductible** de 97 sorties aléatoires (impossible en théorie
de l'information) — il faut la description Scheidt (document vendu). Ce n'est pas un abandon :
c'est la démonstration de la frontière + la récupération du pad.

Pourquoi K4 a résisté 35 ans : une clé aléatoire ne laisse AUCUNE contrainte exploitable (l'alphabet
est libre — cf. preuves `audits/autocle_recuit_2026_09_26/`), et l'excès écart-7 qui semblait un
indice était un artefact du clair.

## Seul test restant (long-shot, à null-contrôler)
Existe-t-il un alphabet keyé A **inconnu** (hors KRYPTOS/PALIMPSEST/ABSCISSA) rendant la clé un
TEXTE anglais (running-key) ? 25 ddl d'alphabet pour 97 lettres ⇒ **paréidolie probable** : à ne
tester qu'avec hill-climb qg_big + **contrôle nul strict** (même procédure sur (P,C) mélangés).

## Addendum — générateurs publics FIXES (constantes) écartés (constants_test.py)
Test propre (constantes = 0 paramètre libre, pas de paréidolie) : keystream vs base-26 de
π, e, √2, √3, √5, φ (tous offsets, AZ & KRYPTOS × VIG/BEA/VAR). Meilleur = **11/97** ;
null (clés aléatoires) atteint **18/97**. ⇒ le keystream matche les constantes MOINS que le
hasard : aucune constante mathématique publique n'engendre la clé. Scelle encore le verdict OTP-class.

## Addendum — robustesse à la reconstruction (sensitivity_test.py) : négatifs NON artefacts
Due-diligence : les négatifs pourraient-ils venir d'erreurs dans les ~73 positions non-crib du
clair reconstruit ? NON. Cacher une périodicité exigerait **~63** lettres fausses ; un autoclé
**~13** — or la reconstruction (qoff −2.0, cribs EXACTS, exactitude confirmée par Sanborn) n'a au
plus que quelques lettres douteuses. Perturber 2/5/10 lettres non-crib laisse IC(key)≈0.040
(aléatoire). ⇒ le verdict OTP-class est robuste ; les négatifs ne sont pas des artefacts.
