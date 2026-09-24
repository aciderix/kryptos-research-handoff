"""Pre-registered (23/09): keyword restarted at the start of each 31-cell worksheet row.

Sanborn's K1/K2 worksheet: rows of 31 cells, three lines (plaintext / keyword / ciphertext). A hand encipherer
may restart the keyword at the start of each row. Key index of K4 position i = (column of i) mod p, where the
column is either
  WS : the worksheet column if K4 starts a fresh worksheet at column 0  -> col = i % 31
  PANEL : the copper column (K4 starts at panel column 27, rows of 31)    -> col = (i + 27) % 31
Families: Quagmire I, II, III (unknown alphabets), Vigenere/Beaufort, p = 1..13; exact (24/24) and with one
tolerated slip (23/24). Null: 20 random ciphertexts.
"""
import json, os, sys
from multiprocessing import Pool
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "algebraic_elimination_2026_09_22"))
from k4_algebraic import K4, ANCH, periodic, rand_ct

ALIGN = {"WS": lambda i: i % 31, "PANEL": lambda i: (i + 27) % 31}


def work(args):
    ct_id, q, al, p = args
    ct = K4 if ct_id == "K4" else rand_ct(9000 + ct_id)
    out = {}
    for mode in ("vig", "beau"):
        idx = ALIGN[al]
        if periodic(ct, ANCH, mode, p, q, idx=idx):
            out[mode] = "exact"; continue
        for d in ANCH:
            a = {k: v for k, v in ANCH.items() if k != d}
            if periodic(ct, a, mode, p, q, idx=idx):
                out[mode] = f"slip@{d}"; break
    return ct_id, q, al, p, out


if __name__ == "__main__":
    n_null = 20
    tasks = [(c, q, al, p) for c in ["K4"] + list(range(n_null)) for q in ("I", "II", "III")
             for al in ALIGN for p in range(1, 14)]
    res = {}
    with Pool(4) as pool:
        for c, q, al, p, out in pool.imap_unordered(work, tasks, chunksize=4):
            res.setdefault(f"Q{q}_{al}", {}).setdefault(p, {})[str(c)] = out
    summary = {}
    for fam, byp in res.items():
        summary[fam] = {}
        for p in range(1, 14):
            d = byp[p]
            summary[fam][p] = {"K4": d["K4"],
                               "null_exact": sum(1 for i in range(n_null) if "exact" in d[str(i)].values()) / n_null,
                               "null_any": sum(1 for i in range(n_null) if d[str(i)]) / n_null}
    json.dump(summary, open(os.path.join(os.path.dirname(__file__) or ".", "results_row_restart.json"), "w"), indent=1)
    for fam, s in summary.items():
        print(fam, {p: (v["K4"], v["null_any"]) for p, v in s.items() if v["K4"]})
