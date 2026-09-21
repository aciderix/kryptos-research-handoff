#!/usr/bin/env python3
"""Frozen, small structural exploration; no keys, plaintext search, or tuning."""
from __future__ import annotations

import hashlib
import json
import random
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CT = (ROOT / "data" / "ct.txt").read_text().strip()
CRIBS = ((21, "EASTNORTHEAST"), (63, "BERLINCLOCK"))
CONTROL_SEEDS = tuple(range(1001, 1009))
ALPHABET = set("ABCDEFGHIJKLMNOPQRSTUVWXYZ")


def sha256_text(value: str) -> str:
    return hashlib.sha256(value.encode("ascii")).hexdigest()


def random_ct(seed: int) -> str:
    rng = random.Random(seed)
    return "".join(rng.choice(sorted(ALPHABET)) for _ in range(len(CT)))


def ragged_col7(text: str) -> str:
    rows = [text[i:i + 7] for i in range(0, len(text), 7)]
    return "".join(rows[r][c] for c in range(7) for r in range(len(rows)) if c < len(rows[r]))


def reverse97(text: str) -> str:
    assert len(text) == 97
    return text[::-1]


def mask_even(text: str) -> str:
    return text[0::2]


def random_mask49(text: str, seed: int) -> str:
    rng = random.Random(seed)
    positions = sorted(rng.sample(range(len(text)), 49))
    return "".join(text[i] for i in positions)


def random_permutation(text: str, seed: int) -> str:
    chars = list(text)
    random.Random(seed).shuffle(chars)
    return "".join(chars)


def composed_col7_reverse(text: str) -> str:
    return reverse97(ragged_col7(text))


def composed_random_reverse(text: str, seed: int) -> str:
    return reverse97(random_permutation(text, seed))


def two_band_reverse7(text: str) -> str:
    return text[:49] + text[49:][::-1]


def two_band_random(text: str, seed: int) -> str:
    return text[:49] + random_permutation(text[49:], seed)


def ic(text: str) -> float:
    n = len(text)
    if n < 2:
        return 0.0
    counts = Counter(text)
    return sum(v * (v - 1) for v in counts.values()) / (n * (n - 1))


def metrics(output: str) -> dict:
    crib_occurrences = {crib: sum(output[i:i + len(crib)] == crib for i in range(len(output) - len(crib) + 1)) for _, crib in CRIBS}
    positional = None
    noncrib = None
    if len(output) == len(CT):
        positional = sum(output[pos + j] == crib[j] for pos, crib in CRIBS for j in range(len(crib)))
        crib_positions = {pos + j for pos, crib in CRIBS for j in range(len(crib))}
        noncrib = sum(output[i] == CT[i] for i in range(len(output)) if i not in crib_positions)
    return {
        "length": len(output),
        "alphabet_valid": set(output) <= ALPHABET,
        "output_sha256": sha256_text(output),
        "crib_occurrences": crib_occurrences,
        "public_crib_positional_match": positional,
        "non_crib_identity_match": noncrib,
        "index_of_coincidence": round(ic(output), 12),
        "fixed_points": sum(a == b for a, b in zip(output, CT)),
        "adjacent_pair_preservation": (
            sum(output[i] == CT[i] and output[i + 1] == CT[i + 1] for i in range(len(CT) - 1))
            if len(output) == len(CT)
            else None
        ),
        "output": output,
    }


def make_record(experiment_id: str, family: str, transform: str, null_id: str, control_id: str, output: str, source_kind: str, seed: int | None = None) -> dict:
    return {
        "experiment_id": experiment_id,
        "family": family,
        "transformation": transform,
        "null_id": null_id,
        "control_id": control_id,
        "source_kind": source_kind,
        "seed": seed,
        "status": "EXECUTED",
        "metrics": metrics(output),
    }


def main() -> None:
    assert len(CT) == 97 and set(CT) <= ALPHABET
    records: list[dict] = []
    recipes = [
        ("E01-R-RAGGED-COL7", "R", "ragged_col7", "NULL-LIMITED-RAGGED-COL7", ragged_col7, "EXPLORATION — NULL LIMITED"),
        ("E01-G-ROT180-97", "G", "reverse97", "NULL-LIMITED-GEOM-ROT180", reverse97, "EXPLORATION — NULL LIMITED"),
        ("E01-M-PERIOD2-EVEN", "M", "mask_even", "NULL-LIMITED-MASK-CARD49", mask_even, "EXPLORATION — NULL LIMITED"),
        ("E01-C-COL7-ROT180", "C", "ragged_col7_then_reverse97", "NULL-LIMITED-COMPOSED-RANDOM", composed_col7_reverse, "EXPLORATION — HYPOTHESIS TEST"),
        ("E01-P-TWO-BAND-REVERSE7", "P", "fixed_band_49_48_reverse_second", "NULL-LIMITED-TWO-BAND", two_band_reverse7, "EXPLORATION — HYPOTHESIS TEST"),
    ]
    for exp_id, family, transform, null_id, fn, expected_status in recipes:
        records.append(make_record(exp_id, family, transform, null_id, f"{exp_id}-REAL", fn(CT), "REAL", None))
        for seed in CONTROL_SEEDS:
            if family in {"R", "G"}:
                out = fn(random_ct(seed))
                control_kind = "random_ciphertext_same_length"
            elif family == "M":
                out = random_mask49(CT, seed)
                control_kind = "random_mask_same_cardinality"
            elif family == "C":
                out = composed_random_reverse(CT, seed)
                control_kind = "random_first_layer_then_fixed_reverse"
            else:
                out = two_band_random(CT, seed)
                control_kind = "random_second_band_permutation"
            records.append(make_record(exp_id, family, transform, null_id, f"{exp_id}-C{seed}", out, control_kind, seed))
    summary = {
        "protocol": "Exploration-01-execution-protocol.md",
        "input_sha256": sha256_text(CT),
        "input_length": len(CT),
        "experiment_count": len(recipes),
        "control_count": len(records) - len(recipes),
        "record_count": len(records),
        "records": records,
        "interpretation_status": "EXPLORATION RESULT",
        "provenance_note": "No result was used to alter recipes, parameters, metrics, controls, or seeds.",
    }
    out_path = ROOT / "results" / "exploration_01_structural_raw.json"
    out_path.parent.mkdir(exist_ok=True)
    out_path.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(json.dumps({k: summary[k] for k in ("input_length", "experiment_count", "control_count", "record_count")}, sort_keys=True))


if __name__ == "__main__":
    main()
