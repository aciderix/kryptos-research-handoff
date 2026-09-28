# Experimental Prior-Art Reliability Audit

**Date:** 2026-09-21
**Scope:** Archive-informed campaigns whose status could affect the K4 experimental frontier
**Execution rule:** No campaign was re-run. This audit inspects source code, headers, registry entries, and archived metadata only.

## Executive conclusion

The earlier registry contains three different situations:

1. **The original archive-informed Beaufort campaign is mechanically correct in the current script, but its record is incomplete and the file is explicitly deprecated.** It has no archived score or output. It must not count as a verified negative.
2. **The predecessor Extra-L campaign was genuinely misclassified:** its comments and title included Beaufort, but the predecessor implementation used Vigenère for both labels. The corrected successor implements all three formulas correctly and records a best score of 5/24 over 3,084 tested configurations, but the successor’s output is represented only by the exhaustion log; no raw stdout/result artifact was found.
3. **The mixed-tableau campaign implements distinct Vigenère, Beaufort, and variant-Beaufort formulas, but its recorded configuration count conflicts with the script’s visible loops.** The registry says 11,600; the header says approximately 18,000; the script’s loop structure includes multiple phases and does not provide a saved result artifact. Its “exhausted” label is therefore not sufficient for a clean verified-negative claim.

The reliable frontier must therefore distinguish **mechanical coverage** from **archived, reproducible result evidence**.

## Classification definitions used

| Status | Meaning in this audit |
|---|---|
| **VERIFIED PRIOR ART** | Script and announced operation agree; parameters and result artifact agree; execution scope is reconstructable. |
| **MISCLASSIFIED PRIOR ART** | The script or archived record labels an operation incorrectly; the corrected scope is recorded separately. |
| **INCOMPLETE RECORD** | The script exists and is inspectable, but result, parameter count, or execution provenance is missing or inconsistent. |
| **UNREPRODUCIBLE** | The claimed execution cannot be tied to a stable script/data/result set. |
| **CLAIM ONLY** | A registry or report asserts coverage, but the underlying executed artifact cannot be verified. |

## Campaign A — `e_aaa_beaufort_trans_01`

**Script:** `scripts/archive_evidence/e_aaa_beaufort_trans_01.py`

### Announced operation

The title and docstring announce “Archive-evidenced Beaufort + columnar transposition.” The header lists Beaufort, Vigenère, and variant-Beaufort as three variants, with four archive-derived keywords, four widths, two peel orders, and two ciphertext forms.

### Operation actually implemented

The current file contains three separate functions:

- `decrypt_beaufort`: `PT = (K - CT) mod 26`;
- `decrypt_vigenere`: `PT = (CT - K) mod 26`;
- `decrypt_variant_beaufort`: `PT = (CT + K) mod 26`.

The `DECRYPT` dispatch table maps the labels to the corresponding functions. The columnar inverse is a deterministic width-based column fill/read operation. On the current source, there is no Beaufort/Vigenère label collision.

### Parameters actually visible in source

The source fixes:

- keywords: `ABSCISSA`, `ECLIPSE`, `NORMANDY`, `PALIMPSEST`;
- widths: `4, 8, 10, 26`;
- variants: Beaufort, Vigenère, variant-Beaufort;
- ciphertext forms: full `CT97` and null-removed `CT73`;
- peel orders: substitution-then-detransposition and detransposition-then-substitution.

The visible loops perform 4 × 3 × 4 × 2 × 2 = 192 scored configurations.

### Result verification

The header has blank `Last run:` and `Best score:` fields. The exhaustion log also records `best_score: null` and only repeats the header’s keyspace description. No raw stdout, JSON result, or run manifest tied to this script was located.

### Verdict

**INCOMPLETE RECORD**, not verified negative prior art. The operation is mechanically consistent with its labels in the current file, but the historical execution and result are not archived. The file itself is marked deprecated and says not to cite its results as current.

## Campaign B — predecessor `e_aaa_extra_l_05`

**Script:** `scripts/archive_evidence/e_aaa_extra_l_05.py`

### Announced operation

The predecessor’s description includes “Extra L,” bottom-chart seeding, ABSCISSA, and a three-variant framing that purports to include Beaufort.

### Operation actually implemented

The predecessor is explicitly marked deprecated by the successor. Its successor’s header states:

> “beaufort_decrypt_tableau() performed VIGENERE, not Beaufort. Both ‘Beaufort’ and ‘Vigenere’ labels produced identical Vigenere results. True Beaufort on misaligned row N was NEVER tested.”

This is a direct classification failure. The predecessor cannot support the statement that true Beaufort was tested.

### Result verification

The exhaustion log gives the predecessor an approximate keyspace and no score artifact. It is superseded by `e_aaa_extra_l_07_corrected.py`.

### Verdict

**MISCLASSIFIED PRIOR ART.** Preserve historically as a Vigenère-based predecessor only. Remove any implication that it tested Beaufort.

## Campaign C — corrected `e_aaa_extra_l_07_corrected`

**Script:** `scripts/archive_evidence/e_aaa_extra_l_07_corrected.py`

### Operation actually implemented

The current source defines and dispatches distinct formulas:

- Vigenère: `PT = (CT - K) mod 26`;
- Beaufort: `PT = (K - CT) mod 26`;
- variant Beaufort: `PT = (CT + K) mod 26`.

It implements a 27-character misaligned N-row, reversed bottom-chart key use, ordinary KA tableau baselines, width-27 columnar variants, removal of L as a null, divided-chart variants, seeding/autokey variants, variable-width grid handling, and “not-coded” passthrough variants. The visible source contains ten hypothesis blocks and tests both CT97 and CT73 where applicable.

### Parameters actually visible in source

The source fixes 11 keywords, widths `[4, 8, 10, 26, 27]`, and three variants. It explicitly separates the corrected formulas and records that it supersedes the buggy predecessor.

### Result verification

The exhaustion log records:

- date: 2026-04-01;
- 3,084 configurations;
- best score: 5/24;
- all three variants tested;
- status: exhausted;
- predecessor: `e_aaa_extra_l_05`.

No raw stdout or machine-readable result file tied to the corrected script was found. Therefore the existence of the recorded result is supported by the exhaustion log, but not independently reproducible from an archived output artifact.

### Verdict

**INCOMPLETE RECORD with corrected mechanical coverage.** The corrected source genuinely covers Beaufort, unlike its predecessor. The recorded negative is not upgraded to VERIFIED PRIOR ART because the result artifact and exact run environment are absent.

## Campaign D — `e_aaa_tableau_struct_06`

**Script:** `scripts/archive_evidence/e_aaa_tableau_struct_06.py`

### Announced operation

The header calls it an archive-informed mixed-tableau sweep and lists Vigenère, Beaufort, and variant-Beaufort. It tests keyword-mixed alphabets derived from ABSCISSA, PALIMPSEST, ECLIPSE, and NORMANDY, with AZ and KA bases, periodic models, columnar widths, Atbash layers, orientations, and a Quagmire IV phase.

### Operation actually implemented

The source defines a `decrypt_char` dispatch with three formulas corresponding to Vigenère, Beaufort, and variant-Beaufort. The visible code defines `decrypt_periodic`, builds keyword-mixed alphabets, and iterates the listed variants. There is no evidence in the inspected source of the original Beaufort/Vigenère collision found in the Extra-L predecessor.

### Parameter consistency issue

The header says approximately 18,000 configurations. The exhaustion log says 11,600 configurations. The source has multiple phases and prints separate phase counts, but no saved execution output was found to reconcile the discrepancy. The registry therefore cannot safely claim that the 11,600 or 18,000 figure is the exact executed scope.

### Result verification

The exhaustion log records best score 6/24 and status exhausted. No raw stdout, JSON result, or run manifest tied to the file was located.

### Verdict

**INCOMPLETE RECORD.** The formulas appear mechanically distinct, but the exact executed scope and result provenance are not fully recoverable. Do not call the entire mixed-tableau family verified dead.

## Cross-check against current family registry

The family registry currently contains broad statements such as “Periodic models on mixed tableaux: eliminated” in downstream scripts and lists F-07/F-08 as open. The code audit does not justify broadening a negative from these campaigns to all archive-inspired tableaux, all Beaufort constructions, or all overlay mechanisms.

The corrected interpretation is:

- the buggy predecessor covers **Vigenère-like behavior only**, not Beaufort;
- the corrected Extra-L campaign covers the explicitly enumerated 3-variant scope, subject to missing raw output;
- the mixed-tableau campaign covers its visible loop scope, but its exact run count/result artifact is incomplete;
- the original archive-informed Beaufort/transposition script has no verifiable archived result.

## No new experiment authorized

This audit does not reopen or test any K4 hypothesis. It only corrects what “already tested” can honestly mean. A future campaign may cite these entries only with the bounded scopes above and must not inherit the stronger label “family eliminated” without a new, separately authorized audit.
