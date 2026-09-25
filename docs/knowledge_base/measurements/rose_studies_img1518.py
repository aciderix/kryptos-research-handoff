"""Needle bearing on the three hand-drawn compass-rose studies of IMG_1518
(Jim Sanborn papers, Archives of American Art; photo published by kryptosbot.com/archive,
https://www.kryptosbot.com/static/archive/IMG_1518.jpg, 1600x1200).
Same method as compass_rose_bearing.py: homography mapping the four cardinal letters
(N, E, S, W, assumed equidistant from the rose centre) to a unit square, then bearing of
the needle axis in the rose frame. The paper was photographed at an angle; the homography
corrects the perspective. Points read by eye on 2x enlargements with a 20 px grid.
Sensitivity: letters perturbed by N(0, 6 px), needle tips by N(0, 4 px), 2000 draws."""
import numpy as np, math

def homog(src, dst):
    A = []
    for (x, y), (u, v) in zip(src, dst):
        A.append([x, y, 1, 0, 0, 0, -u*x, -u*y, -u])
        A.append([0, 0, 0, x, y, 1, -v*x, -v*y, -v])
    _, _, V = np.linalg.svd(np.array(A, float))
    H = V[-1].reshape(3, 3)
    return H / H[2, 2]

def ap(H, p):
    q = H @ np.array([p[0], p[1], 1.0])
    return q[:2] / q[2]

DST = [(0, 1), (1, 0), (0, -1), (-1, 0)]

# pixel (x, y) in IMG_1518: cardinal letters N, E, S, W, then the needle's NE and SW tips
ROSES = {
    "top left":  dict(N=(397, 122),   E=(602.5, 327.5), S=(372.5, 540),   W=(162.5, 324),
                      ne=(582.5, 231), sw=(180, 416)),
    "top right": dict(N=(1100, 140),  E=(1320, 340),    S=(1116, 547.5),  W=(897.5, 332.5),
                      ne=(1276, 222.5), sw=(939, 452.5)),
    "bottom":    dict(N=(732.5, 600), E=(947.5, 817.5), S=(728.5, 1052.5), W=(507.5, 820),
                      ne=(927.5, 726), sw=(525, 912.5)),
}

def axis_bearing(card, ne, sw):
    H = homog(card, DST)
    v = ap(H, ne) - ap(H, sw)
    return math.degrees(math.atan2(v[0], v[1])) % 360   # clockwise from N

rng = np.random.default_rng(1)
for name, r in ROSES.items():
    card = [r["N"], r["E"], r["S"], r["W"]]
    b = axis_bearing(card, r["ne"], r["sw"])
    sims = []
    for _ in range(2000):
        c = [(x + rng.normal(0, 6), y + rng.normal(0, 6)) for x, y in card]
        ne = (r["ne"][0] + rng.normal(0, 4), r["ne"][1] + rng.normal(0, 4))
        sw = (r["sw"][0] + rng.normal(0, 4), r["sw"][1] + rng.normal(0, 4))
        sims.append(axis_bearing(c, ne, sw))
    lo, hi = np.percentile(sims, [2.5, 97.5])
    print(f"{name:9s}: needle axis {b:5.1f} deg (95% {lo:.1f}-{hi:.1f}), opposite end {(b + 180) % 360:.1f}")
print("reference: NE = 45, ENE = 67.5; engraved Kryptos needle (compass_rose_bearing.py): 66-67 / 246-247")
