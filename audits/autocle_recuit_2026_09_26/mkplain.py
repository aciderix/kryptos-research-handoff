#!/usr/bin/env python3
"""Extrait des segments de 97 lettres A-Z d'un corpus anglais, pour fabriquer des contrôles positifs.
Usage: mkplain.py CORPUS_DIR N [seed]  -> N lignes de 97 lettres majuscules sur stdout."""
import sys, os, random, glob

def letters_of(path):
    with open(path, encoding="utf-8", errors="ignore") as f:
        t = f.read()
    s = t.find("*** START")
    if s >= 0:
        s = t.find("\n", s)
    else:
        s = 0
    e = t.find("*** END")
    if e < 0:
        e = len(t)
    return "".join(c for c in t[s:e].upper() if "A" <= c <= "Z")

def main():
    cdir, nseg = sys.argv[1], int(sys.argv[2])
    seed = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    rng = random.Random(seed)
    blobs = [letters_of(p) for p in sorted(glob.glob(os.path.join(cdir, "*.txt")))]
    blobs = [b for b in blobs if len(b) > 5000]
    out = []
    while len(out) < nseg:
        b = rng.choice(blobs)
        i = rng.randrange(0, len(b) - 97)
        out.append(b[i:i + 97])
    print("\n".join(out))

if __name__ == "__main__":
    main()
