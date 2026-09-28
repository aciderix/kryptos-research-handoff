from p19_check import solutions, K4, AZ, ANCH
import collections
for q, mode, L in (("II","vig",11),("II","beau",10)):
    sols = solutions(mode, q, L, maxn=2000)
    print(f"== Q{q} {mode} L={L}: {len(sols)} solutions (capped 2000)")
    # cipher alphabet position -> letter
    s_sgn = 1 if mode == "vig" else -1
    outs = collections.Counter(); prefixes = collections.Counter()
    for v in sols:
        pos = {c: v["C"+c] for c in AZ}                      # letter -> position in keyed alphabet
        seq = "".join(sorted(AZ, key=lambda c: pos[c]))
        prefixes[seq[:L]] += 1
        pt = []
        for i, ch in enumerate(K4):
            k = v.get(f"k{i%19}")
            if k is None: pt.append("?"); continue
            x = (s_sgn * (pos[ch] - k)) % 26
            pt.append(AZ[x])
        outs["".join(pt)] += 1
    for p_, n in prefixes.most_common(5): print(" prefix", p_, n)
    for t, n in outs.most_common(4): print(" ", n, t)
