# K4 Audit 003 — Width-21 Skip-5/Read-8 Procedure

**Status before execution:** OPEN CELL, not admissible as a complete experiment
**Registry families:** F-01 and F-08
**Question:** Does the `DESPARATLY` anomaly encode a deterministic skip-5/read-8 extraction on the physical width-21 K4 layout, with the separator question mark excluded?

## Candidate procedure

The internal procedural recipe CP-3 proposes:

1. Write K4 into a width-21 grid: four complete rows plus a final 13-character row.
2. Treat the question mark between K3 and K4 as a separator, not part of K4.
3. Derive the extraction rule from `DESPARATLY`: skip every 5th position and read every 8th position.
4. Treat the extracted characters as the real ciphertext.
5. Decrypt the extracted text with a standard method.

## Independent justification

The width-21 anomaly is recorded in the repository as a measured structural observation. `DESPARATLY` is an observed misspelling with positions 5 and 8 available as a possible procedural hint. The question mark is a physical separator whose role is publicly ambiguous. These facts justify the candidate as a research question, not as evidence that the procedure is correct.

## Prior art checked

Internal sources: `docs/procedural_anomaly_recipes.md` CP-3, `docs/research_questions.md`, `docs/two_systems_landscape.md`, width-21 and geometry scripts, and the family registry. The repository records width-21 standard columnar and related grid tests, while CP-3 itself is marked as untested specifically for skip-5/read-8.

External search found discussions of K4 row layouts, `DESPARATLY`, and route methods, but no inspected source supplied the exact skip/read convention plus a public-input-only decryption rule.

## Canonicalization and equivalence

The extraction layer can be canonicalized as a position-selection function on a 21-wide, row-major stream with a separator convention. However, the wording leaves unresolved whether “skip 5, read 8” means a cycle of five skipped then eight retained positions, an 8-step arithmetic progression after each 5-step jump, or a pair of independent strides. These are different transformations, not mere notation changes.

## Missing fixed data

Even after choosing one extraction interpretation, the procedure does not specify:

- the start position;
- direction of traversal;
- treatment of the final partial row;
- whether the selected 73 characters are ciphertext or plaintext;
- the alphabet and cipher variant for step 5;
- the key or key-generation rule;
- whether crib positions are mapped before or after extraction.

## Degrees of freedom

At least seven independent choices remain. The phrase “decrypt with a standard method” is not a parameter; it is an unresolved family of methods. The recipe therefore fails the admissibility rule.

## Pre-registered decision

No decryption test will be executed. Selecting Vigenère, Beaufort, KRYPTOS, a route, or a crib alignment after observing the extracted stream would turn the experiment into a post-hoc search.

## Required next evidence

A source or artifact must independently fix the extraction convention and the second-system decryption rule. Alternatively, a new hypothesis sheet may define one complete recipe with all values fixed before examining the extracted output; it must then be classified as a new proposal rather than as a result implied by `DESPARATLY`.

## Verdict

> **OPEN CELL — NOT ADMISSIBLE AS A COMPLETE HYPOTHESIS.** The anomaly fixes a promising extraction question but not a plaintext-discovery procedure.
