import random, json
from multiprocessing import Pool
from visible_words import scan, AZ
def one(s):
    r = random.Random(1000 + s); ct = "".join(r.choice(AZ) for _ in range(97)); return int(scan(ct)[0])
if __name__ == "__main__":
    with Pool(4) as p: v = p.map(one, range(120))
    print("null best distribution:", {k: v.count(k) for k in sorted(set(v))}, " P(>=10) =", sum(x >= 10 for x in v) / len(v))
    json.dump(v, open("null_120.json", "w"))
