# Documentary Authentication Audit — Sanborn Archive Leads

**Date:** 2026-09-21
**Scope:** IMG_1236, IMG_1238, IMG_1555, IMG_1543, IMG_1340, IMG_1569, IMG_1566, IMG_1221
**Execution status:** No K4 calculation performed

## Overall verdict

The published scans are real image files and their visible writing can be inspected directly. Their **archival provenance and Kryptos/K4 attribution cannot be independently authenticated from the public material currently available**.

The archive curator states that the photographs were taken in person at the Smithsonian’s Archives of American Art in March 2026. The underlying Smithsonian finding-aid URLs returned 404 during this audit, and reporting states that Sanborn’s papers were sealed after the 2025 discovery. RR Auction’s catalogue explicitly says that the auctioneer had not independently examined or verified the archive materials. [1] [2] [3]

Accordingly, the correct status is not “Sanborn document proves K4 mechanism.” It is:

> **Published scan with curator attribution; original archive provenance not independently verified.**

No item reaches **DOCUMENT AUTHENTICATED → DETERMINED RECIPE**.

## Provenance scale

| Label | Meaning |
|---|---|
| Published scan | An image is publicly retrievable and visually inspectable. |
| Curator-attributed | The archive page assigns an ID, date, or interpretation. |
| Source-authenticated | The archive or a primary finding aid independently confirms custody, identity, and context. |
| Kryptos-linked | The original document itself identifies Kryptos/K4, or an independent archival record proves the link. |
| Recipe-determining | The document fixes representation, alignment, operation, parameters, and inverse transformation. |

The scanned items reach the first two levels. None reaches all five.

## IMG_1236 — “shade an area”

**Published object:** [local scan](images/IMG_1236.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1236.jpg)

**Visible source content:** The handwritten text reads:

> “encrypted message is included within set of modern day font characters. Could be done to shade an area”

The page also contains a rough drawing of a large outlined form with marks and a central shape. No date, signature, project title, Kryptos label, K4 label, box number, or folder number is visible in the photographed crop.

**Source and provenance:** The curator attributes the image to the Jim Sanborn papers at the Archives of American Art and identifies it as `IMG_1236`. The underlying original is not publicly available for comparison in this audit. The archive page itself says that the project attribution is an interpretation of the curator, not a transcription from a visible document heading. [1]

**Document context:** The image could be a concept note for a sculpture or a general design idea. It does not visibly identify Kryptos or K4.

**Exact fact established:** Sanborn appears to have written the quoted sentence on the photographed page.

**Interpretation not established by the document:** “shade an area” does not itself say Cardan grille, null mask, ciphertext selection, or Kryptos. The page does not specify what is shaded, by what template, on what geometry, or in which direction.

**Prior exploitation:** The repository already used `IMG_1236` in archive-evidence scripts and in `e_anomaly_source_text_inference.py`. Those scripts promoted the phrase to “null masking confirmed” and “Cardan grille candidate”; that is an interpretive layer, not a fact transcribed from the scan. [4] [5]

**Filter result:**

- Determination: **fails**; no complete operation or parameters.
- Novelty: **not demonstrated**; physical overlays and null masks are existing registered families.
- Status: **UNVERIFIED as Kryptos/K4 source; OPEN CELL as a documentary lead.**

## IMG_1238 — purported “7×88” stencil job

**Published object:** [local scan](images/IMG_1238.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1238.jpg)

**Visible source content:** The photographed job sheet visibly includes `JOB 55741-0001-01 JIM SANBORN`, revision/expiration information, and a line referring to a stencil type. The body text visibly discusses the Dupont-Kalorama Museums Consortium, the Phillips Collection, and Anderson House. The small dimension string after the date is partly obscured and difficult to read from the available photograph.

**Source and provenance:** The curator transcribes the dimension as “7x88 on a side,” but the scan does not make that reading secure enough to treat as a verified transcription. The document is a production order, but its visible text points to a museum-text/stencil job and does not identify Kryptos or K4.

**Exact fact established:** There is a Sanborn-associated job sheet for a stencil-related project, and the photographed body text refers to museum institutions.

**Interpretation not established:** The document does not visibly establish a seven-row K4 layout. The curator’s connection to 97 K4 characters is an inference. The dimension may also be misread because of image angle and small type.

**Prior exploitation:** `IMG_1238` appears in internal anomaly and archive-evidence scripts as “7x88,” and was used to motivate a width-7/row-grid mask hypothesis. [5] This was a hypothesis import, not an independently authenticated K4 manufacturing link.

**Filter result:**

- Determination: **fails**; even the key dimension is not securely transcribed and no K4 relationship is shown.
- Novelty: not reached.
- Status: **UNVERIFIED; not a K4 document on present evidence.**

## IMG_1555 — “Code Breaker” template sketch

**Published object:** [local scan](images/IMG_1555.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1555.jpg)

**Visible source content:** A handwritten sketch shows panel-like rectangles, arrows, and labels that visibly include `Code Breaker` and `Code`/`Coded`. No date, signature, Kryptos title, K4 label, dimensions, or coordinate grid is visible.

**Source and provenance:** The curator assigns `IMG_1555` and describes it as a template laid over a code panel. The image confirms a template-like drawing, but not its project identity.

**Exact fact established:** Sanborn appears to have drawn a template/panel concept and used the phrase `Code Breaker` on the page.

**Interpretation not established:** The sketch does not specify an overlay registration, a selected region, a mask alphabet, or a decoding direction. It cannot be identified as a Kryptos procedure from the image alone.

**Prior exploitation:** The image was used internally as evidence for a “physical overlay” and then folded into Cardan-grille interpretations. [4] [5]

**Filter result:** **UNVERIFIED; OPEN CELL; no recipe.**

## IMG_1543 — mirror-image waterjet invoice

**Published object:** [local scan](images/IMG_1543.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1543.jpg)

**Visible source content:** A document headed `JIM SANBORN`, dated `July 15, 1994`, titled `INVOICE #1`, specifies waterjet cutting of a stainless-steel mirror. It visibly includes:

> `TEXT TO BE MIRROR IMAGE WORDS`

and typed instructions about mirror-image letters, 1/2-inch letter height, line spacing, word spacing, and a one-inch border. Handwriting says:

> `USE THE LETTERS FUMEE ENGLISH ALPHABET`

**Source and provenance:** The date and fabrication instructions are visible. The document does not visibly identify Kryptos, which was dedicated in 1990, and it may concern a later artwork.

**Exact fact established:** Sanborn used or commissioned mirror-reversed text in a July 1994 stainless-steel mirror job.

**Interpretation not established:** This does not establish that K4 uses a mirrored reading, a mirrored alphabet, or the word FUMEE. The four-year date gap and absence of a Kryptos identifier are material.

**Prior exploitation:** Mirror-reading and reversed-tableau families already exist in the internal registry. [5]

**Filter result:** **AUTHENTIC DATED PRODUCTION DOCUMENT; Kryptos/K4 link unverified; no new recipe.**

## IMG_1340 — notebook page with “Extra L” and “Bottom chart seeding”

**Published object:** [local scan](images/IMG_1340.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1340.jpg)

**Visible source content:** The lower part of the page visibly includes notes substantially resembling:

> `Code originally developed to keep things secret in the military`

> `Keywords chosen for their mysterious nature AND how they fit into the aesthetics`

> `Palimpsest is to write over ...` and `ABSCISSA`

> `Extra L ... At end of line, Bottom chart seeding`

A final line visibly begins with `? or like 4, 8, 10, 25 (+) coded`, but the remainder is not reliably legible in this scan.

**Source and provenance:** The photograph is a notebook page attributed to the Sanborn papers. There is no visible date, page number, or explicit K4 heading. Much of the page concerns vendor and fabrication logistics.

**Exact fact established:** The words and phrases appear on the photographed notebook page. The exact punctuation and the unreadable numerical continuation are not established.

**Interpretation not established:** The phrase `Extra L` may refer to the Kryptos tableau, a layout note, or another object. “Bottom chart seeding” is not a complete cryptographic instruction. The curator’s proposed connection to 98 cells and a 14×7 layout is not written visibly on the page.

**Prior exploitation:** This item has been used extensively in `e_aaa_tableau_struct_06.py` and `e_aaa_extra_l_07_corrected.py`, including keyworded tableaux, extra-L offsets, bottom-chart reversal, and several composite hypotheses. The latter script explicitly records that an earlier Beaufort implementation was actually Vigenère and that true Beaufort on the misaligned row had not been tested in that earlier version. [6] [7]

**Filter result:**

- Determination: **fails**; no unique referent or full operation.
- Novelty: the explored extra-L/tableau families are already registered.
- Status: **UNVERIFIED as K4-specific; OPEN CELL.**

## IMG_1569 — method list

**Published object:** [local scan](images/IMG_1569.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1569.jpg)

**Visible source content:** Sanborn’s handwritten list visibly includes `Beaufort cipher`, `Compass cipher`, `Morse code`, `Alphabet code`, `Cryptonyms`, and `Overlord`. The lower part connects `Overlord` to the Normandy invasion.

**Source and provenance:** The image is attributed to the Sanborn papers but shows no date, project title, or K4 reference.

**Exact fact established:** Sanborn wrote down or studied these terms.

**Interpretation not established:** Awareness of Beaufort or Morse does not show use in K4. “Compass cipher” is not defined as a reproducible algorithm. `Overlord` is a historical codename, not automatically a key.

**Prior exploitation:** Internal archive-evidence scripts used Beaufort, compass, and related keyword interpretations as K4 candidates. [6]

**Filter result:** **AUTHENTIC METHOD-LIST IMAGE; K4 use unverified; no determined recipe.**

## IMG_1566 — ECLIPSE and PALIMPSEST definitions

**Published object:** [local scan](images/IMG_1566.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1566.jpg)

**Visible source content:** The page visibly defines `Eclipse` with phrases equivalent to `Blocked shaded from view`, `Shadow of the moon`, and `Loss of Light`. It also contains `Palimpsest` with a phrase about writing over or erasing.

**Source and provenance:** The image shows a notebook vocabulary/materials page with no K4 label or date. Italian stone-supplier information is visible.

**Exact fact established:** Sanborn recorded these thematic definitions.

**Interpretation not established:** The words may be artistic vocabulary, keyword candidates, or general conceptual notes. They do not fix a mask, key, route, or alphabet.

**Prior exploitation:** `ECLIPSE` and `PALIMPSEST` were used as candidate tableau keywords in internal sweeps. [6]

**Filter result:** **AUTHENTIC VOCABULARY NOTE; K4 mechanism unverified; OPEN CELL only.**

## IMG_1221 — transparent overlay/map-like object

**Published object:** [local scan](images/IMG_1221.jpg) · [published image](https://www.kryptosbot.com/static/archive/IMG_1221.jpg)

**Visible source content:** The photograph shows a transparent sheet or overlay over printed material. A letter grid/tableau is visible near the top, while a larger lower image resembles a map or architectural graphic. The angle and reflections prevent exact identification and registration.

**Source and provenance:** The curator attributes it to the Sanborn papers and describes it as a map-like transparency over a tableau. No visible title, date, project identifier, or K4 label is present.

**Exact fact established:** A physical overlay object exists in the photographed material.

**Interpretation not established:** The object may concern a different artwork or a general design exercise. It does not reveal what is aligned with what, which parts are selected, or how a result is decoded.

**Prior exploitation:** The object was used internally as a “physical Cardan grille candidate” and as support for a null-mask interpretation. [4] [5]

**Filter result:** **UNVERIFIED as Kryptos/K4; OPEN CELL; no recipe.**

## Antériorité and methodological audit

The documents were already known within this repository. They have been used in archive-evidence scripts and in an anomaly-inference ledger. The prior use often moved directly from a curator’s description to a cryptographic label such as “null mask,” “Cardan grille,” “7-column grid,” or “Beaufort candidate.” This audit does not accept those labels as source facts.

The previous campaigns also contain at least one documented implementation problem: an earlier script labeled a transformation Beaufort while applying Vigenère, later corrected in a superseding script. This means that even where a document-inspired sweep exists, its result cannot be treated as a clean independent closure without checking the exact implementation and scope.

No external publication located during this audit was found to provide a complete, source-authenticated K4 procedure from these images. The public archive page itself is the principal published description; public reporting confirms the archive discovery and subsequent sealing, but does not expose the documents’ full provenance chain or a new method. [2] [8]

## Final filter

| Document | Authentication | Established fact | Real constraint | Determined recipe | Final status |
|---|---|---|---|---|---|
| IMG_1236 | Scan readable; original custody not independently verified | Phrase about an encrypted message within characters | Possible concealment concept only | No | **UNVERIFIED / OPEN CELL** |
| IMG_1238 | Job sheet readable; exact dimension and K4 link uncertain | Sanborn stencil job involving museum text | None for K4 | No | **UNVERIFIED** |
| IMG_1555 | Sketch readable; project identity absent | Template-like drawing | Physical layering as a general idea | No | **UNVERIFIED / OPEN CELL** |
| IMG_1543 | Dated production document readable | Mirror-image text used in 1994 | General fabrication fact | No K4 recipe | **AUTHENTIC DOCUMENT / K4 LINK UNVERIFIED** |
| IMG_1340 | Notebook scan readable in part; context absent | Extra-L, bottom-chart wording appears | No unique referent | No | **UNVERIFIED / OPEN CELL** |
| IMG_1569 | Method list readable; context absent | Beaufort/Morse/compass terms recorded | Awareness only | No | **AUTHENTIC IMAGE / K4 USE UNVERIFIED** |
| IMG_1566 | Definitions readable; context absent | Eclipse and palimpsest vocabulary | Semantic only | No | **AUTHENTIC IMAGE / OPEN CELL** |
| IMG_1221 | Overlay visible; object identity absent | Physical overlay exists | General technique only | No | **UNVERIFIED / OPEN CELL** |

## Final verdict

No document currently reaches the chain:

```text
DOCUMENT AUTHENTICATED → ESTABLISHED FACT → REAL CONSTRAINT → DETERMINED RECIPE → NOVELTY AUDIT
```

The appropriate research state is therefore:

> **Documentary leads preserved; no new K4 experiment authorized; no K4 calculation performed.**

## References

[1]: https://www.kryptosbot.com/archive/ "KryptosBot, Sanborn Papers — Archive Research Collection"
[2]: https://apnews.com/article/kryptos-jim-sanborn-auction-cia-secret-code-cb8ee8554ca473910cbd0592f8bdb350 "Associated Press, archive discovery and sale of the Kryptos materials"
[3]: https://www.rrauction.com/auctions/lot-detail/350761607302001-the-complete-secrets-of-kryptos-jim-sanborns-private-archive/ "RR Auction, The Complete Secrets of Kryptos"
[4]: ../scripts/analysis/e_anomaly_source_text_inference.py "Internal anomaly-source inference ledger"
[5]: ../scripts/archive_evidence/e_aaa_tableau_struct_06.py "Internal archive-informed tableau sweep"
[6]: ../scripts/archive_evidence/e_aaa_extra_l_07_corrected.py "Internal corrected extra-L and bottom-chart sweep"
[7]: ../scripts/archive_evidence/e_aaa_beaufort_trans_01.py "Internal archive-informed Beaufort/transposition sweep"
[8]: https://www.schneier.com/blog/archives/2025/10/part-four-of-the-kryptos-sculpture.html "Bruce Schneier, Part Four of The Kryptos Sculpture"
