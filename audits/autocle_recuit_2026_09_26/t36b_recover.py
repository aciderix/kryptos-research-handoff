#!/usr/bin/env python3
"""Contrôle positif ANCRÉ-CRIBS pour l'autoclé écart-7 Vigenère, deux alphabets libres (T36b).
Étape 1 (CP-SAT) : trouver (sigma_crib, tau) satisfaisant les 17 équations de chaîne des cribs.
Étape 2 (recuit court) : compléter les lettres libres de sigma (et 2 tau libres) pour maximiser
les quadrigrammes sur les 73 lettres hors cribs ; kappa est DÉTERMINÉ par les cribs.
On mesure : la vérité est-elle retrouvée (contrôle réparable) ou un autre anglais surgit-il (indécidable) ?

Usage: t36b_recover.py qg.bin CT [truePT] [nsol] [seed]
"""
import sys, random, struct, time
from ortools.sat.python import cp_model

N, LAG = 97, 7
CE = list(range(21, 34)); CB = list(range(63, 74))
CRIBPOS = CE + CB
CRIBPT = "EASTNORTHEAST" + "BERLINCLOCK"
PT_AT = {p: (ord(CRIBPT[i]) - 65) for i, p in enumerate(CRIBPOS)}

def load_qg(path):
    with open(path, "rb") as f:
        data = f.read()
    return struct.unpack("<%df" % (len(data) // 4), data)

def chains():
    byc = {}
    for p in CRIBPOS:
        byc.setdefault(p % LAG, []).append(p)
    out = []
    for r, ps in byc.items():
        ps.sort()
        for a, b in zip(ps, ps[1:]):
            out.append((a, b))
    return out

def build_eqs(CT):
    """17 équations VIG : sigma(p_b) - (-1)^m sigma(p_a) + sum_j c_j tau(ct[a+7j]) = 0 (mod 26)."""
    eqs = []
    for a, b in chains():
        m = (b - a) // LAG
        s = {}   # sigma coeffs on plaintext letters (crib)
        t = {}   # tau coeffs on ciphertext letters
        s[PT_AT[b]] = s.get(PT_AT[b], 0) + 1
        for j in range(1, m + 1):
            ch = CT[a + LAG * j]
            t[ch] = t.get(ch, 0) - ((-1) ** (m - j))
        s[PT_AT[a]] = s.get(PT_AT[a], 0) - ((-1) ** m)
        eqs.append(({k: v for k, v in s.items() if v}, {k: v for k, v in t.items() if v}))
    return eqs

def solve_crib(CT, seed, want_diff=None):
    eqs = build_eqs(CT)
    M = cp_model.CpModel()
    sl = sorted({k for s, _ in eqs for k in s})            # crib plaintext letters (indices 0..25)
    tl = sorted({c for _, t in eqs for c in t})            # ciphertext letters (chars)
    sig = {L: M.NewIntVar(0, 25, "s%d" % L) for L in sl}
    tau = {c: M.NewIntVar(0, 25, "t%s" % c) for c in tl}
    M.AddAllDifferent(list(sig.values()))
    M.AddAllDifferent(list(tau.values()))
    oks = []
    for e, (s, t) in enumerate(eqs):
        q = M.NewIntVar(-50, 50, "q%d" % e)
        M.Add(sum(c * sig[L] for L, c in s.items()) + sum(c * tau[ch] for ch, c in t.items()) == 26 * q)
    S = cp_model.CpSolver()
    S.parameters.max_time_in_seconds = 20
    S.parameters.random_seed = seed
    S.parameters.num_workers = 4
    # randomize to get diverse feasible solutions
    S.parameters.randomize_search = True
    st = S.Solve(M)
    if st not in (cp_model.OPTIMAL, cp_model.FEASIBLE):
        return None
    return ({L: S.Value(v) for L, v in sig.items()}, {c: S.Value(v) for c, v in tau.items()})

def PT_AT_first_value():
    return 0  # arbitrary anchor value for the smallest-index crib letter

def decrypt_full(CT, sigfull, taufull):
    """sigfull[L]=value (perm), taufull[ct char]=value (perm). kappa determined by cribs."""
    siginv = {v: L for L, v in sigfull.items()}
    # kappa per class from the first crib position in that class
    byc = {}
    for p in CRIBPOS:
        byc.setdefault(p % LAG, []).append(p)
    kappa = {}
    for r, ps in byc.items():
        f = min(ps)
        # x_f = sigma(pt_f). walk back: x_i = tau(ct_i) - x_{i-7}; unroll to position r (< LAG) where x_r = tau(ct_r)-kappa_r
        # compute x_f in terms of kappa_r: x_f = A - (-1)^k kappa_r, where along the way
        idxs = list(range(r, f + 1, LAG))
        # x_{idxs[0]} = tau(ct_r) - kappa_r ; x_{idxs[k]} = tau(ct)-x_prev
        # express x_f = C + s*(-kappa_r) with s=(-1)^(len-1)
        C = 0; sgn = 1;
        # forward: x0 = t0 - k ; x1 = t1 - x0 = t1 - t0 + k ; ...
        # accumulate coefficient of kappa and constant
        coefK = 0; const = 0
        for pos_i, pos in enumerate(idxs):
            tval = taufull[CT[pos]]
            if pos_i == 0:
                const = tval; coefK = -1
            else:
                const = tval - const; coefK = -coefK
        # x_f = const + coefK*kappa_r  => kappa_r = (sigma(pt_f) - const)/coefK ; coefK=±1
        target = sigfull[PT_AT[f]]
        kappa[r] = ((target - const) * (1 if coefK == 1 else -1)) % 26
    # now decrypt
    x = [0] * N
    for i in range(N):
        k = kappa[i % LAG] if i < LAG else x[i - LAG]
        x[i] = (taufull[CT[i]] - k) % 26
    pt = [siginv[x[i]] for i in range(N)]
    return pt, kappa

def qoff(qg, pt):
    iscrib = [1 if i in set(CRIBPOS) else 0 for i in range(N)]
    s = 0.0; n = 0
    for i in range(N - 3):
        if not any(iscrib[i + d] for d in range(4)):
            s += qg[((pt[i] * 26 + pt[i + 1]) * 26 + pt[i + 2]) * 26 + pt[i + 3]]; n += 1
    return s / n if n else 0.0

def hillclimb_free(qg, CT, sigcrib, tau, seed, iters=40000):
    rng = random.Random(seed)
    allv = set(range(26))
    sig = dict(sigcrib)
    usedv = set(sig.values())
    freeL = [L for L in range(26) if L not in sig]
    freev = [v for v in range(26) if v not in usedv]
    rng.shuffle(freev)
    for L, v in zip(freeL, freev):
        sig[L] = v
    # complete tau to full perm (free ct letters)
    taufull = dict(tau)
    usedt = set(taufull.values()); freetv = [v for v in range(26) if v not in usedt]
    freetc = [chr(65 + c) for c in range(26) if chr(65 + c) not in taufull]
    rng.shuffle(freetv)
    for c, v in zip(freetc, freetv):
        taufull[c] = v
    def score():
        pt, _ = decrypt_full(CT, sig, taufull)
        return qoff(qg, pt), pt
    best, bestpt = score()
    T0, T1 = 3.0, 0.05
    for it in range(iters):
        T = T0 * (T1 / T0) ** (it / iters)
        # move: swap two free sigma values, or swap two free tau values
        if rng.random() < 0.6:
            a, b = rng.sample(freeL, 2); sig[a], sig[b] = sig[b], sig[a]
            sc, pt = score()
            if sc >= best or rng.random() < pow(2.718, (sc - best) / T):
                if sc > best: best, bestpt = sc, pt
            else:
                sig[a], sig[b] = sig[b], sig[a]
        else:
            a, b = rng.sample(freetc, 2); taufull[a], taufull[b] = taufull[b], taufull[a]
            sc, pt = score()
            if sc >= best or rng.random() < pow(2.718, (sc - best) / T):
                if sc > best: best, bestpt = sc, pt
            else:
                taufull[a], taufull[b] = taufull[b], taufull[a]
    return best, bestpt

def main():
    qg = load_qg(sys.argv[1]); CT = sys.argv[2]
    truePT = sys.argv[3] if len(sys.argv) > 3 else None
    nsol = int(sys.argv[4]) if len(sys.argv) > 4 else 6
    seed0 = int(sys.argv[5]) if len(sys.argv) > 5 else 1
    if truePT:
        tp = [ord(c) - 65 for c in truePT]
        print("target true qoff = %.4f" % qoff(qg, tp))
    for s in range(nsol):
        t0 = time.time()
        sol = solve_crib(CT, seed0 + s)
        if not sol:
            print("sol %d: CP-SAT infeasible" % s); continue
        sigcrib, tau = sol
        best, bestpt = hillclimb_free(qg, CT, sigcrib, tau, seed0 + s)
        pts = "".join(chr(65 + c) for c in bestpt)
        rec = sum(1 for i in range(N) if truePT and pts[i] == truePT[i]) if truePT else -1
        print("sol %d: qoff=%.4f recovered=%d/%d (%.0fs) PT=%s" % (s, best, rec, N, time.time() - t0, pts))

if __name__ == "__main__":
    main()
