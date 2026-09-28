"""Semi-automatic reader for the Kryptos entrance Morse plates (K0), from photographs.

For each photo: detect the cut-through holes (dark blobs), classify each as dot or dash from its
length/width ratio, group them into rows, measure the edge-to-edge gaps along each row and
classify every gap in Morse units (1 = inside a letter, 3 = between letters, 7 = between words),
using the median dot width of the row as the local unit (perspective changes it along a row).
Output: the raw element string per row, with '|' for letter gaps and ' / ' for word gaps,
decoded in BOTH reading directions (the photos are not all taken from the reading side).

This is a reading aid: every output must be checked by eye against the photo.
Usage: python3 morse_plate_reader.py photo.jpg [photo2.jpg ...]
"""
import sys

import numpy as np
from PIL import Image
from scipy import ndimage

MORSE = {".-": "A", "-...": "B", "-.-.": "C", "-..": "D", ".": "E", "..-.": "F", "--.": "G",
         "....": "H", "..": "I", ".---": "J", "-.-": "K", ".-..": "L", "--": "M", "-.": "N",
         "---": "O", ".--.": "P", "--.-": "Q", ".-.": "R", "...": "S", "-": "T", "..-": "U",
         "...-": "V", ".--": "W", "-..-": "X", "-.--": "Y", "--..": "Z"}


def blobs(path, dark_q=0.035, min_area=120):
    g = np.asarray(Image.open(path).convert("L"), float)
    bg = ndimage.uniform_filter(g, 61)
    dark = (g - bg) < -np.quantile(np.abs(g - bg), 1 - dark_q)
    dark = ndimage.binary_opening(dark, iterations=2)
    lab, n = ndimage.label(dark)
    out = []
    for i, sl in enumerate(ndimage.find_objects(lab), 1):
        area = int((lab[sl] == i).sum())
        if area < min_area:
            continue
        ys, xs = np.nonzero(lab[sl] == i)
        pts = np.stack([xs + sl[1].start, ys + sl[0].start], 1).astype(float)
        c = pts.mean(0)
        ev, evec = np.linalg.eigh(np.cov((pts - c).T))
        major = 4 * np.sqrt(ev[1]); minor = 4 * np.sqrt(max(ev[0], 1e-6))
        if major > 12 * minor or area > 20000:      # cracks, plate edges
            continue
        out.append(dict(c=c, major=major, minor=minor, axis=evec[:, 1], area=area))
    return out


def rows_of(bl):
    """Group blobs into rows: cluster on the coordinate perpendicular to the dominant axis."""
    if not bl:
        return []
    ax = np.median([b["axis"] * np.sign(b["axis"][0] or 1) for b in bl if b["major"] > 2 * b["minor"]]
                   or [np.array([1.0, 0.0])], axis=0)
    ax = ax / np.linalg.norm(ax)
    nrm = np.array([-ax[1], ax[0]])
    for b in bl:
        b["u"] = float(b["c"] @ ax); b["v"] = float(b["c"] @ nrm)
    bl.sort(key=lambda b: b["v"])
    unit = np.median([b["minor"] for b in bl])
    rows, cur = [], [bl[0]]
    for b in bl[1:]:
        if abs(b["v"] - cur[-1]["v"]) > 1.5 * unit:
            rows.append(cur); cur = [b]
        else:
            cur.append(b)
    rows.append(cur)
    return [sorted(r, key=lambda b: b["u"]) for r in rows if len(r) >= 2]


def read_row(r):
    unit = np.median([b["minor"] for b in r])
    els = ["-" if b["major"] > 2.0 * b["minor"] else "." for b in r]
    s = els[0]
    gaps = []
    for a, b in zip(r, r[1:]):
        gap = (b["u"] - b["major"] / 2) - (a["u"] + a["major"] / 2)
        k = gap / unit
        gaps.append(round(k, 1))
        s += ("" if k < 3.3 else (" | " if k < 7.5 else " / ")) + ("-" if b["major"] > 2.0 * b["minor"] else ".")
    return s, gaps


def decode(s):
    words = []
    for w in s.split(" / "):
        words.append("".join(MORSE.get(l.replace(" ", ""), "?") for l in w.split(" | ")))
    return " ".join(words)


def reverse_reading(s):
    """Same row read from the other side (rotated 180 deg): element order reversed."""
    return s[::-1].replace("| ", "|").replace(" |", "|").replace("|", " | ").replace("/ ", "/").replace(" /", "/").replace("/", " / ")


if __name__ == "__main__":
    for p in sys.argv[1:]:
        print("==", p)
        for i, r in enumerate(rows_of(blobs(p))):
            s, gaps = read_row(r)
            print(f"  row {i} (v={r[0]['v']:.0f}, n={len(r)}): {s}")
            print(f"     gaps(units): {gaps}")
            print(f"     read as shown : {decode(s)}")
            print(f"     read reversed : {decode(reverse_reading(s))}")
