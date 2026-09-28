# Z32-E03 — RÉSULTATS (2026-09-28) : test du cycle strict **inadmissible** (le Zodiac ne cyclait pas assez strictement)

Pré-inscription : `PREREGISTRATION.md` (commit c85301c). Outil : `tools/budget.c` mode `cycle` ; sortie : `logs/e03.out`.

| Chiffré de contrôle (clair et symboles réels) | Fenêtres de 32 incohérentes avec un cycle strict |
|---|---|
| Z408 | 117 / 316 = **37 %** |
| Z340, section 1 (ordre de transposition connu) | 117 / 122 = **96 %** |

Seuil d'admissibilité pré-inscrit : ≤ 10 % dans les deux. **Non atteint** ⇒ aucun candidat n'est jugé par ce critère.

## Conclusion
- Le cycle ordonné des homophones, frappant sur le Z408 entier, est trop souvent rompu à l'échelle de 32 symboles
  (37 % des passages du Z408, presque tous ceux du Z340) pour servir à juger un chiffré court.
- En particulier, la rupture de cycle notée à la main sur le candidat DMW/Stampher (déclarée dans la pré-inscription)
  **ne dit rien** : les vrais messages du Zodiac en présentent autant.
- Avec E01, c'est la seconde habitude du Zodiac qui, calibrée honnêtement sur ses propres chiffrés, ne permet pas de
  départager les lectures du Z32.
