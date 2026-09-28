import sys, json
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
src = open("gromark_dates.py").read().split("out = []")[0].replace("os.path.dirname(__file__)", '"."')
exec(src)
from multiprocessing import Pool
def one(s):
    ct = rand_ct(50000 + s)
    for mode in ("vig",):
        if sat(ct, "19861989", mode, "IV"): return 1
        for i in ANCH:
            if sat(ct, "19861989", mode, "IV", {j:c for j,c in ANCH.items() if j != i}): return 1
    return 0
with Pool(4) as p: r = p.map(one, range(200))
print("QIV vig 19861989 one-slip null:", sum(r), "/200")
