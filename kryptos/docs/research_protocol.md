# K4 Research Protocol: Equivalence, Prior Art, and Negative Evidence

**Status:** Operational protocol
**Scope:** Any new Kryptos K4 hypothesis, experiment, or literature claim
**Principle:** Exploration is not proof. A result found in this repository is not evidence that the same question was never studied elsewhere.

## 1. Mechanism-determination gate

Before a hypothesis enters detailed prior-art review, it must provide a mechanism of determination, not merely an interesting clue. The observed artefact must constrain, independently and in advance, the representation, traversal, parameters, operation, and any final transformation required to produce plaintext.

A candidate fails this gate immediately if its recipe contains an unresolved step such as “use a standard method,” “try keys derived from,” “test several routes,” “decrypt afterward,” or “choose the coherent output.” Such a candidate is recorded as **OPEN CELL** and is not executed. The missing step must be fixed by a new independent source or by a separately declared hypothesis; it may not be selected from K4 output.

The gate is passed only when a third party could write one deterministic recipe from the hypothesis sheet alone and obtain the same input, alignment, operation, parameters, and output convention without seeing any result.

## 2. Admission rule

Before testing a hypothesis, the researcher must define the precise question, search the internal repository and public sources, record materially similar prior work, and determine whether the proposed mechanism is genuinely distinct. The test may begin only after the procedure and all parameters are fixed.

A new hypothesis is admissible only when it introduces either a historically or materially justified operation, or a non-equivalent composition of known operations whose difference has an independent justification. A parameter variation, renaming, orientation change, or post-result adjustment is not novelty.

## 3. Canonical mechanism representation

Equivalence is determined from the transformation itself, before inspecting its output on K4. Every candidate procedure must be serialized as a canonical mechanism record with these fields:

| Field | Required content |
|---|---|
| Input model | Carved K4, reflowed K4, K1–K3-derived material, or another explicitly named input |
| Preprocessing | Character filtering, separators, padding, omissions, or none |
| Geometry | Grid dimensions, block size, line widths, or physical layout |
| Position map | Exact permutation or selection rule, including origin and direction |
| Symbol map | Alphabet, tableau, pair encoding, bit encoding, or substitution rule |
| Layer sequence | Ordered operations, with forward and inverse conventions |
| Parameters | Complete values and their provenance |
| External information | Every artefact, statement, or public crib used |
| Free choices | Choices left open before execution; target is zero |

Two procedures are equivalent when their canonical records induce the same transformation class under representation changes that do not add information: transposed grid notation, inverse route notation, 0/1-based indexing, renamed variables, or algebraically identical layer order. Different K4 outputs do not make equivalent procedures distinct.

Two procedures are distinct when one uses an additional independent input, a genuinely different position map, a different layer order that is not algebraically reducible, or an operation with different information requirements. The distinction must be stated before looking at the K4 result.

## 4. Mandatory hypothesis sheet

Each experiment must have a short record containing:

- hypothesis and precise research question;
- internal and external prior art;
- canonical mechanism record;
- equivalence analysis;
- independent justification for the difference;
- fixed parameters and provenance;
- whether any non-public plaintext was used (must be **no**);
- degrees of freedom before execution;
- preregistered success and kill criteria;
- control tests and null model;
- raw result and final status.

The four public crib segments may be used. The sealed archive plaintext and any plaintext reconstructed from a candidate mechanism may not be used as input, parameter source, or tuning target.

## 5. Prior-art search

The search must cover the repository, its linked documents, public web pages, research blogs, forums, repositories, videos, and claimed solutions. A source is not accepted merely because its title contains words such as "solution" or "grid". The exact procedure, parameter source, and validation direction must be inspected.

Prior work is classified as **verified reproduction**, **claim with inspectable method**, **claim without sufficient method**, or **search lead only**. An external claim may establish prior art without establishing that the claim is correct.

## 6. Negative evidence

A negative result is an asset. It must record the tested mechanism class, parameter coverage, assumptions, score and null model, implementation revision, and remaining open cells. A negative result does not eliminate a broader family unless the recorded transformation class covers that family.

A family may be marked **dead under scope** only when its admissible variants and assumptions are explicitly enumerated. It may be reopened only by a new independent input, a new operation, a corrected implementation, or a changed scope condition.

## 7. Prohibited reasoning

The following do not establish novelty or confirmation:

- finding a different plaintext from an equivalent transformation;
- changing a parameter after observing K4 output;
- fitting a table, key, gate, or helper sequence from a reconstructed plaintext;
- treating absence from the repository as absence from the literature;
- treating a high crib score as a solution;
- treating a source's solution label as independent validation;
- using a simple representation change as a new hypothesis.

## 8. Required final language

Every report must distinguish these statements:

> This mechanism was not found in the sources searched.

> This mechanism is materially distinct from the prior mechanisms listed here because ...

> This experiment was negative under the following scope.

> This result is a candidate or reconstruction, not a verified solution.

No stronger statement is permitted without independent reproduction and a forward derivation from public inputs alone.

## References

[1]: ../docs/search_policy.md "Short Layered Ciphertext Search Policy"
[2]: ../docs/two_systems_landscape.md "The Two Systems Landscape for Kryptos K4"
[3]: ../docs/README_current_state.md "Current Research State"
[4]: ../docs/REAL_K4_CURRENT_POSITION.md "Real-K4 Current Position"
