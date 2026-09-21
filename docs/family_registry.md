# K4 Family Registry

**Purpose:** Prevent repeated exploration of the same transformation under different names.
**Status:** Seed registry; entries are scope-bounded and do not claim universal elimination.

## Status vocabulary

- **DEAD UNDER SCOPE:** the recorded admissible class was tested or proved incompatible under explicit assumptions.
- **AUDITED PRIOR ART:** an external method exists and was inspected, but its correctness or independence is not established.
- **OPEN CELL:** the broad family has relevant untested mechanisms that are materially distinct.
- **CLAIM ONLY:** a public solution claim was found, but the method is not independently established.
- **INCOMPLETE RECORD:** a script or historical entry exists, but the executed scope or result artifact is insufficiently archived; this is not evidence of elimination.
- **RETIRED:** the project has formally withdrawn the line as an evidentiary basis.

## Registry

| ID | Family | Covered variants or transformation class | Status | Internal evidence | External prior art | Reopening condition |
|---|---|---|---|---|---|---|
| F-01 | Rectangular grids and classical routes | Column, row, reverse, serpentine, diagonal, spiral, selected 7×14 and related routes under recorded campaigns | **DEAD UNDER SCOPE / OPEN CELL** | `docs/two_systems_landscape.md`; `docs/research_questions.md`; route and composition scripts | Numberworld 2020; multiple community route attempts | A materially new route rule or physical input not reducible to an existing permutation |
| F-02 | K3-style route reuse | K3-derived columnar bottom-to-top route and related grid reconstructions | **AUDITED PRIOR ART / OPEN CELL** | `docs/k3_chart_layout_and_route_2026_09_19.md` | K3 method is documented publicly; reuse on K4 must be tested as a separate mechanism | Independent evidence that Sanborn reused the route for K4, plus a fixed K4 geometry |
| F-03 | Pairwise or digraphic transposition | Pair splitting, 8×13 construction, subsequent 31×3 reflow | **AUDITED PRIOR ART** | Related pairwise scripts and transposition policy | Nash Associates / Medium, 2025 | A new pair rule justified by an independent artefact, not a different starting offset |
| F-04 | Quagmire plus coordinate permutation | Quagmire III with `KRYPTOS`, K1-derived key, CIA-coordinate congruence, and related two-pass variants | **AUDITED PRIOR ART / MOSTLY COVERED** | `docs/two_systems_landscape.md`; composition and polyalphabetic campaigns | Numberworld, 2020 | A non-equivalent physical position map or independently sourced tableau operation |
| F-05 | Berlin Clock route plus substitution | 4–4–11–4 cycle, offset 21, boustrophedon mask, `k mod 3` style layer | **CLAIM ONLY / AUDITED PRIOR ART** | Search-policy and composition references; no acceptance as verified | Figshare Genie Engine, 2025 | Inspectable replication must show parameters fixed before plaintext reconstruction and public-input forward derivation |
| F-06 | 7×14 physical-tableau mechanism | 7 tiers × 14 lanes, helper letters, fitted or back-solved cards, one-bit gate | **CLAIM ONLY / AUDITED PRIOR ART** | No project acceptance; compare against H1 and physical-layer scope | SolveKryptos solution and verification pages | Public-input-only derivation, no fitted helper cards, and independent reproduction |
| F-07 | Physical overlay / two-sided tableau | Cipher side overlaid with tableau side or sculpture geometry | **OPEN CELL** | `docs/procedural_anomaly_recipes.md`; `docs/two_systems_landscape.md` | Community proposals exist; exact procedure varies | Public geometry and a fully specified overlay map |
| F-08 | Procedural chart operations | Manual cut, fold, margin, line-break, marker, or chart-derived operations not expressible as ordinary named ciphers | **OPEN CELL** | `docs/procedural_anomaly_recipes.md`; K1/K2/K3 chart reports | K1/K2 chart evidence and public procedural speculation | Each operation requires an exact recipe and prior-art search |
| F-10 | Morse extra-E masks | E-position masks, binary Morse overlays, finite tape consumption, and transposition→Morse-mask compositions | **INCOMPLETE RECORD / OPEN CELL** | Three deprecated scripts exist, but each says `Last run: never`; no corresponding result artifact was found. The explorer report explicitly says structural-selector tests were not completed. | Public discussions of extra E markers and Morse-derived masks | Require a fixed recipe, prior-art audit, and archived kernel-verified result before any negative status |
| F-09 | Retired palette/null-mask line | Palette `{B,G,I,K,O,W,Z}` and derived null constructions | **RETIRED** | `docs/a1_score_conditioned_null_report.md`; `MEMORY.md` | Historical external speculation | Reopen only with a new independent, pre-registered evidence source and an audit close |

## Entry policy

A new experiment must cite one or more registry IDs. It must state whether it is a covered variant, an open cell, or a proposed new family. If it is claimed to be new, its canonical mechanism record and equivalence analysis must be attached before execution, as required by [`research_protocol.md`](research_protocol.md).

A negative experiment updates the relevant row with its exact scope. It does not silently broaden the row to an entire family. A family is not marked dead because a result was poor, because a different parameter was tried, or because a competing solution claim exists.

## External-source audit notes

The following sources were inspected during the initial registry pass:

- Numberworld, *Kryptos — The Cipher (Part 4)*: Quagmire and coordinate-permutation attempts, including a 7-character second pass.
- Nash Associates, *Kryptos K4 Finally Cracks (a bit) with Digraphic Transposition*: pairwise 8×13 and 31×3 proposal; explicitly incomplete and not independently validated.
- SolveKryptos, *Kryptos K4 Solution* and *Verify Kryptos K4 Yourself*: 7×14 physical-tableau claim; the verification page states that helper cards and parts of the structure are back-solved from the proposed plaintext.
- Alan Glanz, *Kryptos K4: A Mechanism-Based Partial Solution and Statistical Confirmation*: Berlin Clock-inspired Genie Engine claim; replication and independence require separate audit.

These entries establish prior art, not correctness.

## Reliability audit — 2026-09-21

The archive-informed campaign records were audited without rerunning any K4 experiment. The predecessor `e_aaa_extra_l_05` is **MISCLASSIFIED PRIOR ART**: its Beaufort-labeled path implemented Vigenère. Its corrected successor `e_aaa_extra_l_07_corrected` implements distinct Vigenère, Beaufort, and variant-Beaufort formulas, but remains **INCOMPLETE RECORD** because the raw run output is absent. `e_aaa_beaufort_trans_01` is also **INCOMPLETE RECORD**: the current source has distinct formulas, but no archived score or run artifact exists. `e_aaa_tableau_struct_06` is **INCOMPLETE RECORD** because its header and exhaustion-log configuration counts disagree and no raw output is archived.

These labels narrow only the claims about what was previously executed. They do not create a new K4 hypothesis and do not authorize a new experiment. See [`audits/prior_art_reliability_2026_09_21/audit_report.md`](../audits/prior_art_reliability_2026_09_21/audit_report.md).
