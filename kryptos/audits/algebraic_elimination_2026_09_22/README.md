# Algebraic elimination — 2026-09-22

Exact-SAT tests of hand-feasible K4 mechanisms with **unknown** mixed alphabets.
Rationale, results and limits: `docs/k4_mechanism_reasoning_2026_09_22.md`.

Requirements: `pip install python-sat` (CaDiCaL backend). Python ≥ 3.9.

| File | Content |
|---|---|
| `structural_fixed_alphabets.py` / `.out` | key-generator tests with fixed AZ/KA alphabets (no solver) |
| `k4_algebraic.py` | SAT engine + all families, K1 real control, positive and null controls |
| `results_<family>.json` | raw per-family output (`K4_sat` = parameters compatible with the 24 anchors) |
| `positive_controls_periodic_fixed.json` | corrected Quagmire I–IV positive controls |
| `autokey_pt_lags_null.py`, `results_autokey_pt_lags_null.json` | per-lag null rates for autokey survivors |
| `qmark98.py`, `qmark98_lib.py`, `qmark98_positive_control.py`, `results_qmark98*.json` | '?'-belongs-to-K4 (98 = 14×7) hypothesis |
| `k4_only_prog_rowcol.py`, `prog_rowcol_targeted.py` | K4-only / targeted runs for the two slow families |
| `logs/` | stdout of every run (the `*.log` files are git-ignored; raw results are in the JSON files) |

Reproduce one family: `python3 k4_algebraic.py periodic_QIII`
