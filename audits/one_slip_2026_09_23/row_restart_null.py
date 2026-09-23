"""Null for row_restart.py, Quagmire III only (the family with K4 one-slip hits at p=8 and p=13, Beaufort).
Random ciphertexts a..b-1, both alignments, p 1..13, both modes; one line per (ct, align, p) appended to JSONL."""
import json, os, sys
from multiprocessing import Pool
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import row_restart as rr

if __name__ == "__main__":
    a, b = int(sys.argv[1]), int(sys.argv[2])
    tasks = [(i, "III", al, p) for i in range(a, b) for al in rr.ALIGN for p in range(1, 14)]
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "results_row_restart_null_QIII.jsonl")
    with Pool(4) as pool, open(path, "a") as f:
        for c, q, al, p, o in pool.imap_unordered(rr.work, tasks, chunksize=2):
            f.write(json.dumps({"ct": c, "align": al, "p": p, "hits": o}) + "\n"); f.flush()
