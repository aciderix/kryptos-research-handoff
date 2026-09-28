# K4 Audit 001 — SolveKryptos 7×14 Physical-Tableau Reconstruction

**Status before execution:** CLAIM ONLY / AUDITED PRIOR ART
**Registry family:** F-06
**Question:** Does the public SolveKryptos reconstruction provide an independently derived K4 plaintext and mechanism, or only an arithmetic reconstruction fitted to a proposed plaintext?

## Hypothesis

The public SolveKryptos proposal claims a 97-character plaintext and a 7×14 physical-tableau mechanism using the KRYPTOS keyword, helper letters, helper cards, and a one-bit gate. The proposal would be materially stronger if the complete plaintext and all mechanism tables could be derived from public inputs and fixed rules without using the non-public plaintext.

## Prior art checked before execution

- SolveKryptos, `solution`: https://solvekryptos.com/solution
- SolveKryptos, `verify`: https://solvekryptos.com/verify
- Internal comparison: `docs/family_registry.md`, F-06; `docs/two_systems_landscape.md`; `docs/research_protocol.md`.

The verification page explicitly states that the helper cards and part of the structure are back-solved from the proposed plaintext. This is prior art and a reproducible consistency claim, not independent discovery.

## Canonical mechanism record

| Field | Fixed record |
|---|---|
| Input | Carved 97-character K4 ciphertext |
| Plaintext | Publicly proposed SolveKryptos string; treated as candidate output, not as an input to a solve |
| Geometry | 7 tiers × 14 lanes, one unused cell |
| Position map | SolveKryptos lane/tier mapping |
| Symbol map | Standard A–Z values; KRYPTOS-keyed tableau in claimed helper layer |
| Layer sequence | Helper-card shift decomposition, one-bit gate, final subtraction |
| Parameters | Proposed plaintext, R-grid, helper cards, f-table, gate rule |
| Independent inputs | Public cribs and claimed physical/tableau interpretations |
| Free choices | At least the candidate plaintext and back-solved helper structures, according to the source |

## Equivalence analysis

This is not treated as a new K4 family. It belongs to the already registered physical-tableau/7×14 class F-06. A 7×14 presentation is not a novelty claim by itself. The proposed helper-card layer is materially distinct only if its values and rules are derived before the plaintext is known.

## Pre-registered tests

1. Verify ciphertext length and exact identity.
2. Verify the candidate plaintext length and exact public crib positions.
3. Recompute `R[i] = (C[i] - P[i]) mod 26` for all 97 positions.
4. Record whether the result demonstrates only arithmetic consistency or an independent forward derivation.
5. Do not tune any parameter, alter any letter, or use non-public plaintext.

## Success criteria

- **Arithmetic consistency:** all 97 shifts are reproduced from the proposed plaintext.
- **Independent solve:** not met merely by arithmetic consistency. Requires public-input-only derivation of the plaintext and mechanism parameters.

## Kill criteria

- Any anchor mismatch, length mismatch, or arithmetic mismatch kills the published reconstruction as stated.
- If all arithmetic checks pass but tables are back-solved from the candidate plaintext, classify as **CLAIM ONLY / CONSISTENCY REPRODUCTION**, not a verified solve.

## Expected interpretation

Because the source provides the candidate plaintext and says that some helper structures are back-solved, the expected result is a consistency reproduction rather than an independent solve. This expectation was recorded before execution.
