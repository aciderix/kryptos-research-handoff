"""Pre-registered (23/09): are the classic eliminations robust to ONE Sanborn slip among the 24 crib letters?

Why: Sanborn's own K1/K2 worksheet (NYT 2010, pixel-verified in docs/nyt_k1k2_chart_physical_layout_2026_09_19.md)
shows a keystream slip (PALIMPCEST -> IQLUSION) faithfully carved, a carving slip (worksheet E, copper R ->
UNDERGRUUND), and an omitted letter (IDBYROWS). >= 3 process slips in 432 letters. Every K4 elimination so far
requires all 24 crib letters to fit exactly; one slip in the crib zone would wrongly eliminate the true method.

Test: for each family and parameter, drop ONE crib position (24 choices) and ask the exact SAT engine
(unknown alphabets, audits/algebraic_elimination_2026_09_22/k4_algebraic.py) whether the other 23 fit.
Families (fixed in advance, closest to the primary descriptions: keywords, simple, hand-executable):
  periodic Quagmire I, II, III, periods 1-13, Vigenere and Beaufort conventions;
  running key from K1, K2, K3 plaintexts (all offsets), same conventions.
Null: the same on 20 random ciphertexts carrying the same anchors. A K4 hit only matters if clearly rarer
than the null.
"""
import json, sys, os
from multiprocessing import Pool
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "algebraic_elimination_2026_09_22"))
from k4_algebraic import K4, ANCH, periodic, runkey, rand_ct, K1PT, K2PT, K3PT

TEXTS = {"K1PT": K1PT, "K2PT": K2PT, "K3PT": K3PT}
JOBS = []
for q in ("I", "II", "III"):
    for p in range(1, 14):
        JOBS.append(("periodic_Q" + q, q, p))
for name, t in TEXTS.items():
    for o in range(-21, len(t) - 73):
        JOBS.append(("runkey_" + name, name, o))


def check(ct, fam, a, x, mode):
    if fam.startswith("periodic"):
        return periodic(ct, a, mode, x, a_q(fam))
    return runkey(ct, a, mode, TEXTS[fam.split("_")[1]], x)


def a_q(fam):
    return fam.split("_Q")[1]


def work(args):
    ct_id, fam, _, x = args
    ct = K4 if ct_id == "K4" else rand_ct(9000 + ct_id)
    hits = []
    for mode in ("vig", "beau"):
        if check(ct, fam, ANCH, x, mode):
            hits.append((mode, None)); continue
        for d in ANCH:
            a = {k: v for k, v in ANCH.items() if k != d}
            if check(ct, fam, a, x, mode):
                hits.append((mode, d))
    return ct_id, fam, x, hits


if __name__ == "__main__":
    # usage: one_slip.py K4            -> results_one_slip_K4.jsonl
    #        one_slip.py null a b      -> random ciphertexts a..b-1 -> results_one_slip_null.jsonl (appended)
    which = sys.argv[1]
    if which == "K4":
        ids, fn = ["K4"], "results_one_slip_K4.jsonl"
    else:
        ids, fn = list(range(int(sys.argv[2]), int(sys.argv[3]))), "results_one_slip_null.jsonl"
    tasks = [(c, f, q, x) for c in ids for (f, q, x) in JOBS]
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), fn)
    with Pool(4) as pool, open(path, "a") as out:
        for ct_id, fam, x, hits in pool.imap_unordered(work, tasks, chunksize=4):
            out.write(json.dumps({"ct": ct_id, "fam": fam, "x": x, "hits": hits}) + "\n"); out.flush()
