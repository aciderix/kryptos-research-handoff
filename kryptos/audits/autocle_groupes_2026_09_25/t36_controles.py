"""T36, supplementary positive controls for groups where no English text closes on 26 letters.
The planted plaintext is built letter by letter: at each position a letter is drawn at random
among those whose encryption lands on one of the 26 letter images (cribs kept at their place;
restart if a crib letter cannot be encrypted). Also measures, for each group, how often an
English text closes (the method has to stay inside the 26 letters). Usage: t36_controles.py [n]"""
import sys, random, os
from t36_autocle_groupes import GROUPS, PT, LAG, AZ, solve, add_el, elements

def enc(x, y, d, g, mode):
    if mode == "VIG":
        return add_el(add_el(x, y, g), d, g)
    if mode == "BEAU":
        return add_el(add_el(y, x, g, 1, -1), d, g)
    return add_el(add_el(x, y, g, 1, -1), d, g)

def planted_free(g, mode, rng):
    els = elements(g)
    for _ in range(5000):
        sig = dict(zip(AZ, rng.sample(els, 26))); inv = {e: c for c, e in sig.items()}
        d = rng.choice(els)
        pt, ct, ok = [], [], True
        for i in range(97):
            if i < LAG:
                pt.append(PT.get(i, rng.choice(AZ))); ct.append(rng.choice(AZ)); continue
            cands = [PT[i]] if i in PT else list(AZ)
            rng.shuffle(cands)
            for c in cands:
                e = enc(sig[c], sig[pt[i - LAG]], d, g, mode)
                if e in inv:
                    pt.append(c); ct.append(inv[e]); break
            else:
                ok = False; break
        if ok:
            return "".join(ct)
    return None

def english_closure(g, mode, rng, corpus, trials=2000):
    els = elements(g); good = 0
    for _ in range(trials):
        sig = dict(zip(AZ, rng.sample(els, 26))); imgs = set(sig.values()); d = rng.choice(els)
        st = rng.randrange(len(corpus) - 200); pt = corpus[st:st + 97]
        if all(enc(sig[pt[i]], sig[pt[i - LAG]], d, g, mode) in imgs for i in range(LAG, 97)):
            good += 1
    return good / trials

if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 3
    corpus = "".join(ch for ch in open(os.environ.get("CORPUS", "corpus_all.txt"), errors="ignore").read(3_000_000).upper() if ch in AZ)
    rng = random.Random(3636)
    for name, g in GROUPS.items():
        exp2 = all(k == 2 for k in g)
        for mode in (["VIG"] if exp2 else ["VIG", "BEAU", "VAR"]):
            res = []
            for _ in range(n):
                ct = planted_free(g, mode, rng)
                res.append(solve(ct, g, mode, 60)[0] if ct else "no text")
            cl = english_closure(g, mode, rng, corpus)
            print(f"{name:18s} {mode:4s} planted controls e_min={res} | English 97-letter texts that stay in the 26 letters: {cl:.2%}", flush=True)
