# K4 Registry Coherence Audit

**Date:** 2026-09-21
**Scope:** Final consistency check after the prior-art reliability audit
**Execution rule:** No cryptographic experiment, campaign, or K4 scoring script was run.

## Executive verdict

The registry is now usable as a scope-bounded frontier, with one correction required during this audit: **F-10 (Morse extra-E masks) was incorrectly presented as “DEAD UNDER RECORDED SCOPES.”** The cited scripts are deprecated and each states `Last run: never`; no corresponding result artifact was found. The contemporaneous Morse report explicitly states that the structural-selector and post-transposition uses had not been tested. F-10 is therefore now **INCOMPLETE RECORD / OPEN CELL**.

No other family in the seed family registry is marked dead solely because a result is absent or unverifiable. F-01’s combined label **DEAD UNDER SCOPE / OPEN CELL** is intentionally bounded: recorded route classes have evidence, while materially new route rules remain open. F-09 is **RETIRED**, not dead; F-05 and F-06 are **CLAIM ONLY / AUDITED PRIOR ART**; F-07 and F-08 are **OPEN CELL**.

## Checks performed

### 1. Status semantics

The family registry now defines `INCOMPLETE RECORD` explicitly as a non-eliminating status. It means that a script or historical entry exists but the executed scope or result artifact is insufficiently archived. It cannot be cited as proof that a transformation failed.

`MISCLASSIFIED PRIOR ART` remains attached to the predecessor Extra-L campaign in `exhaustion_log.json`. That entry is superseded and is not counted as a valid test of Beaufort. The corrected successor has three distinct formulas but remains `INCOMPLETE RECORD` because its raw output is absent.

### 2. DEAD status check

The family registry contains no unqualified `DEAD` status. The only scope-bounded dead labels are:

- **F-01:** `DEAD UNDER SCOPE / OPEN CELL`; this explicitly separates recorded route classes from new route rules.
- **F-10:** corrected during this audit to `INCOMPLETE RECORD / OPEN CELL`.

The root exhaustion log uses `exhausted` for script-level historical closure, not as a universal family proof. Its current counts are **1,044 entries: 732 active, 310 exhausted, and 2 superseded**. Four archive-informed entries carry the new audit classifications: three `INCOMPLETE RECORD` and one `MISCLASSIFIED PRIOR ART`.

### 3. Reproducible evidence versus historical assertion

The repository distinguishes reproducible artifacts from claims as follows:

| Evidence class | Current treatment |
|---|---|
| Structural proof or bounded campaign with an identified artifact and scope | May support `STRUCTURAL` or bounded `DEAD UNDER SCOPE` language. The H1/model caveat remains mandatory. |
| Script exists, but no raw result or exact run scope is recoverable | `INCOMPLETE RECORD`; not an elimination. |
| Historical/public solution method inspected without independent derivation | `AUDITED PRIOR ART` or `CLAIM ONLY`; not a solution and not a negative. |
| Retired evidence line | `RETIRED`; it is not silently counted as an open or dead cryptographic family. |
| Explicitly untested mechanism or missing procedure | `OPEN CELL` or `STRUCTURALLY-OPEN`. |

The three Morse scripts used to support F-10 are specifically in the second and fifth classes: they are deprecated historical artifacts and declare no run. They cannot support the old dead label.

### 4. Summary and counter consistency

The root `exhaustion_log.json` is the authoritative script-level counter. Historical reports retain their own dated snapshots and are not treated as current counters. For example, the June 6 final report says `302 exhausted` in its dated pre-audit snapshot; the current root log says `310 exhausted`. This is a temporal difference, not a live-register contradiction, provided the older report is cited as historical.

The composition v3 report is internally consistent: its nine campaign rows sum to 5,245, matching its stated total, with maximum score 5/24 and no Bean pass. The two-system landscape is also internally coherent because it labels each cell with a scope and explicitly warns that its synthesis is not a substitute for underlying artifacts.

The archive-informed campaigns are not promoted to fully reproducible status: the corrected Extra-L result, the mixed-tableau result, and the original archive-Beaufort/transposition entry remain bounded but incomplete, as documented in the preceding reliability audit.

### 5. Clean experimental frontier

#### Proven tested or structurally proved, within scope

These are not universal statements about K4. They are bounded claims tied to explicit assumptions and artifacts: direct-positional Bean impossibility results for the covered additive classes; structural exclusions such as pure transposition and the documented fractionation/alphabet constraints; the documented composition campaigns whose ledgers and reports agree; and the bounded two-layer campaign described in `docs/two_systems_landscape.md`.

#### Partially documented

The archive-informed Beaufort/transposition entry, corrected Extra-L campaign, and mixed-tableau campaign are mechanically inspectable but lack complete raw run provenance or have scope/count discrepancies. They must be cited only for their documented subscopes, never as blanket family eliminations.

#### Only claimed or externally reported

The SolveKryptos 7×14 construction and the Berlin Clock/Genie Engine claim remain `CLAIM ONLY / AUDITED PRIOR ART`. Inspection is not independent reproduction.

#### Actually open

F-02, F-07, F-08, and now F-10 contain materially distinct mechanisms that are not closed by the recorded tests. The two-system landscape additionally identifies substitution-as-outer compositions, strongly mixing inner layers, non-columnar three-layer compositions, and H1-breaking physical/procedural mechanisms as open or structurally open. F-09 is retired and must not be revived without independent evidence and a new audit.

## Final disposition

The repository now has a clean stopping point:

> **Proven tested** is limited to scope-bounded structural proofs and campaigns with identifiable evidence. **Partially documented** entries are not eliminations. **Misclassified prior art** is not counted as valid coverage of the advertised operation. **Claims** remain claims. **Open cells** remain open.

No new K4 hypothesis was created, and no cryptographic experiment was executed during this audit.
