# K4 Audit 002 — Extra-L Misaligned Tableau Overlay

**Status before execution:** OPEN CELL, not admissible as a complete experiment
**Registry families:** F-07 and F-08
**Question:** Does the extra L on tableau row N define a physical one-character misalignment that can be used as a deterministic second system for K4?

## Independent justification

The physical tableau contains an extra L on row N. The same physical line is associated with the raised YAR letters on the cipher side. The tableau is intentionally flipped and K2's corrected ending is `X LAYER TWO`. These observations independently justify investigating a two-sided physical alignment operation.

## Prior art checked

Internal sources include `docs/anomaly_registry.md`, `docs/procedural_anomaly_recipes.md`, `scripts/blitz/blitz_rotation_180.py`, `scripts/grille/e_tableau_reflow_grille.py`, `scripts/grille/e_unscramble_01_grille_permutations.py`, and `scripts/grille/e_unscramble_02_correct_mask.py`. These cover full-panel overlays, reflowed tableaux, grille comparisons, and rotation variants, but do not establish that the exact P-B1-3 operation below was run as a public-input-only K4 decoder.

External search found discussion of the extra L, HILL, tableau alignment, and row shifts, but no inspected source supplied a complete fixed recipe that maps the extra-L shift to a 97-character plaintext without post-hoc choices. These sources establish prior art around the object, not a validated decoder.

## Proposed canonical operation

The only independently motivated part is:

1. preserve the physical line layout of the cipher and tableau panels;
2. preserve the extra L on tableau row N;
3. overlay the two panels in their stated orientation;
4. use the one-character row-N displacement as a position map.

The operation is incomplete because it does not independently specify:

- which panel coordinate range corresponds to K4's 97 characters;
- whether the overlay compares, selects, or substitutes letters;
- how non-K4 panel positions are discarded;
- whether the tableau letter is a key, a mask, or a route marker;
- which alphabet operation converts a paired coordinate into plaintext.

## Equivalence analysis

The existence of an overlay is not new: full-panel overlay, reflowed-tableau overlay, and rotation variants are already present internally. The row-N-only shift is potentially distinct as a position map, but distinctness cannot be decided from its output because the downstream operation is unspecified.

## Degrees of freedom

At least five unresolved choices remain. The experiment therefore has non-zero, unbounded researcher freedom and fails the admission rule.

## Pre-registered decision

No K4 decryption test will be executed. Any output produced by selecting a comparison rule, a coordinate range, or a cipher operation now would be a post-hoc construction rather than a test of P-B1-3.

## Required next evidence

To admit this cell, obtain an independent source or artifact that fixes the missing operation, or derive a single complete recipe from a public K1–K3 procedure with no branch selection after seeing K4 output.

## Verdict

> **OPEN CELL — NOT ADMISSIBLE AS A COMPLETE HYPOTHESIS.** The anomaly justifies research, but it does not yet define a reproducible plaintext-discovery procedure.
