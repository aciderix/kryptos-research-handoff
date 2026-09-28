# Z32-E01 — RÉSULTATS (2026-09-28) : le test des budgets d'homophones est **presque sans puissance** ; aucun candidat publié n'est rejeté par lui

Pré-inscription : `PREREGISTRATION.md` (commit 21e5e57, avant tout calcul sur les candidats). Outil : `tools/budget.c` ;
sortie : `logs/e01.out`.

## 1. Calibration : juger un vrai chiffré du Zodiac avec son autre clé
| Contrôle | Fenêtres | X = 0 | X ≤ 1 | X ≤ 2 | X ≤ 3 | X ≤ 4 |
|---|---|---|---|---|---|---|
| Z408 (clair réel) jugé avec la clé du Z340 | 316 | 43 % | 84 % | 100 % | | |
| Z340 section 1 (clair réel, ordre de transposition connu) jugé avec la clé du Z408 | 122 | 31 % | 64 % | 86 % | 93 % | 100 % |
⇒ seuil pré-inscrit **v = 4**. (43 fenêtres du Z408 écartées : elles contiennent une erreur d'enchiffrement ou le symbole
`8`, ambigu dans cette transcription, A ×5 / S ×3.)
Constat en passant : juger avec la clé **exacte** du Z340, c'est déclarer impossibles 57 % des vrais passages du Z408 ;
avec la clé exacte du Z408, 69 % des vrais passages du Z340. Le Zodiac n'a pas réutilisé ses comptes
d'homophones d'un chiffre à l'autre : supposer une réutilisation exacte n'est pas légitime.

## 2. Puissance : leurres satisfaisant les 3 verrous du Z32
| Leurres | Satisfont les verrous | Rejetés (X408 > 4 et X340 > 4) |
|---|---|---|
| Grammaire de Stampher (reproduite en C : 2 044 224 phrases, 154 572 de 32 lettres, **61** satisfont les verrous, identique à son dépôt) | 61 | **1** (2 %) |
| Fenêtres de 32 lettres d'anglais réservé (108 111 fenêtres) | 42 | **1** (2 %) |

## 3. Candidats publiés
| Candidat | Lecture | Verrous | X408 | X340 | Verdict |
|---|---|---|---|---|---|
| Grinell 2020 `ESTIMATEFOURRADIANSANDFIVEINCHES` | directe | ✓ | 0 | 1 | compatible |
| DMW / Stampher `INTHREEANDTHREEEIGHTHSRADIANSTEN` | directe | ✓ | 2 | 3 | compatible |
| Reese `THREEANDTHREEEIGHTHSRADIANSSIXIN` | sa route (8 + 29 i) mod 32 | ✓ | 1 | 2 | compatible |
| Cragle `THREERADIANSFROMMOUNTAREATWOINCH` | directe | ✓ | 1 | 1 | compatible |
| forum `USEPIOVERTWOFOURANDTHREEQUARTERS` | directe | ✓ | 3 | 0 | compatible |
| forum `USETWOPOINTSFOURANDTHREEQUARTERS` | directe | ✓ | 2 | 0 | compatible |
| Allen `TOWNSHIPTHREENORTHRANGETHREEWEST` | directe | **✗** (T ≠ R en 1/26…) | — | — | incompatible en lecture directe ; le mode de lecture exact de l'article (SSRN, accès refusé ici) n'a pas pu être vérifié |
| Foxon `TWELVEINCHESALONGTHETHREERADIANS` | directe | **✗** | — | — | attendu : Foxon lui-même passe par une transposition de type Z340 et admet des correspondances imparfaites |
| Reese, même clair | directe | ✗ | — | — | (sans sa route) |

## 4. Conclusion
- **RÉSULTAT (négatif)** : les habitudes de clé que le Zodiac a démontrées (Z408, Z340), calibrées honnêtement (seuil fixé
  sur ses propres chiffrés croisés), ne rejettent que **~2 %** des phrases anglaises compatibles avec les verrous. Le test
  ne peut donc presque rien falsifier, et il ne rejette aucun des candidats publiés qui respectent les verrous.
- Ceci **confirme par une voie nouvelle et quantifiée** le constat de Blake, Van Eycke et Oranchak (« aucun test connu ne
  peut falsifier ») : même en exploitant la façon dont le Zodiac construisait ses clés, les 32 symboles laissent passer
  presque tout.
- Un résultat secondaire utile : deux candidats cités (Allen, Foxon) **violent les verrous en lecture directe**, ce qu'un
  lecteur des résumés ne voit pas.
- Piste suivante (hors E01) : le compte « 26 symboles » de la note du FBI — examinée en E02 : c'est le nombre de symboles
  uniques (29 − 3), pas un désaccord de transcription ; piste close.
