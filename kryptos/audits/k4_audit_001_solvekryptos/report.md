# Audit 001 — Rapport brut et verdict

## Résultat brut

The candidate has length 97. Both public crib regions match exactly:

- 0-indexed positions 21–33: `EASTNORTHEAST`
- 0-indexed positions 63–73: `BERLINCLOCK`

For every position, the deterministic shift `R[i] = (CT[i] - PT[i]) mod 26` reproduces the proposed candidate. The computed shift vector has 97 entries and checksum 1198.

Reproduction command:

```bash
python3 audits/k4_audit_001_solvekryptos/check_consistency.py
```

Artifact: `audits/k4_audit_001_solvekryptos/result.json`.

## Interpretation

This is an **arithmetic consistency reproduction**, not an independent solve. The plaintext was supplied by the public SolveKryptos claim. Once a candidate plaintext is supplied, its per-position shift sequence is deterministically recoverable. The test therefore confirms only that the candidate is length-compatible, crib-compatible, and arithmetically consistent with the ciphertext.

The source's verification page states that the helper cards and parts of the mechanism are back-solved from the proposed plaintext. That prevents the current test from promoting the claim to a forward public-input-only derivation.

## Classification

- **Family:** F-06, 7×14 physical-tableau mechanism.
- **Status:** `CLAIM ONLY / CONSISTENCY REPRODUCTION ONLY`.
- **Independent solve:** no.
- **New information:** the public claim is internally consistent at the arithmetic level; this does not establish that Sanborn used the claimed mechanism or plaintext.
- **Reopening condition:** obtain or reproduce a decoder whose plaintext and helper structures are generated from public inputs and fixed rules without the candidate plaintext as an input or tuning target.

## Adversarial questions

1. Could the proposed plaintext have been used to build the helper cards? **Yes, according to the source's own verification description.**
2. Does 97/97 arithmetic agreement distinguish the mechanism from any other mechanism that maps the candidate plaintext to the ciphertext? **No.**
3. Were non-public archive materials used in this audit? **No.**
4. Was any parameter changed after seeing the result? **No; the test was pre-registered as a consistency test.**
5. Does this result close the 7×14 family? **No. It classifies one public claim and leaves materially distinct physical-overlay and public-input-only variants open.**

## Final verdict

> **CLAIM ONLY. The reconstruction is arithmetically self-consistent, but the current evidence does not establish an independently derived K4 solution.**
