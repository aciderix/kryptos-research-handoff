"""Algebraic (exact SAT) elimination of hand-feasible K4 mechanisms.

Instead of enumerating keywords, each MECHANISM is encoded as constraints over
UNKNOWN mixed alphabets (all 26! permutations) and unknown key values, and a
SAT solver answers one question: does ANY alphabet/key of this form reproduce
the 24 public anchor letters?  UNSAT = the mechanism is eliminated for every
alphabet and key at once (under the stated alignment hypothesis).

Every family is run with:
  * a positive control: a synthetic ciphertext built WITH that mechanism
    (random alphabet, random key, random plaintext carrying the cribs) must be SAT;
  * a null control: random 97-letter ciphertexts with the same anchors, giving
    the rate at which the family is SAT by chance (the power of the test).

Usage: python3 k4_algebraic.py [family ...]      (no argument = all families)
Output: results.json next to this file, plus a line per family on stdout.
"""
import itertools
import json
import os
import random
import sys

from pysat.card import CardEnc, EncType
from pysat.solvers import Cadical153

AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
KA = "KRYPTOSABCDEFGHIJLMNQUVWXZ"
K4 = ("OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFP"
      "KWGDKZXTJCDIGKUHUAUEKCAR")
CRIBS = {21: "EASTNORTHEAST", 63: "BERLINCLOCK"}          # 0-indexed
ANCH = {s + j: c for s, w in CRIBS.items() for j, c in enumerate(w)}
assert len(K4) == 97 and len(ANCH) == 24

K1PT = "BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION"
K2PT = ("ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLETHEYUSEDTHEEARTHSMAGNETICFIELDX"
        "THEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONX"
        "DOESLANGLEYKNOWABOUTTHISTHEYSHOULDITSBURIEDOUTTHERESOMEWHEREX"
        "WHOKNOWSTHEEXACTLOCATIONONLYWWTHISWASHISLASTMESSAGEX"
        "THIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTH"
        "SEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTXLAYERTWO")
K3PT = ("SLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWER"
        "PARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPER"
        "LEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEERED"
        "INTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLY"
        "DETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ")


# ----------------------------------------------------------------- SAT engine
class Model:
    """Integer variables in Z/26, one-hot encoded; some grouped as permutations."""

    def __init__(self):
        self.nv = 0
        self.lit = {}          # (var, value) -> boolean literal
        self.clauses = []
        self.perms = {}        # perm name -> list of 26 var names
        self.unsat = False

    def var(self, name):
        if (name, 0) in self.lit:
            return name
        lits = []
        for v in range(26):
            self.nv += 1
            self.lit[(name, v)] = self.nv
            lits.append(self.nv)
        enc = CardEnc.equals(lits=lits, bound=1, top_id=self.nv, encoding=EncType.seqcounter)
        self.nv = max(self.nv, enc.nv)
        self.clauses += enc.clauses
        return name

    def perm(self, pname):
        names = [self.var(f"{pname}{c}") for c in AZ]
        for v in range(26):
            lits = [self.lit[(n, v)] for n in names]
            enc = CardEnc.equals(lits=lits, bound=1, top_id=self.nv, encoding=EncType.seqcounter)
            self.nv = max(self.nv, enc.nv)
            self.clauses += enc.clauses
        self.perms[pname] = names

    def fix(self, name, value):
        self.var(name)
        self.clauses.append([self.lit[(name, value % 26)]])

    def lin(self, terms, const=0):
        """Add  sum(coef * var) + const == 0 (mod 26).  terms: [(coef, varname)]"""
        acc = {}
        for c, n in terms:
            self.var(n)
            acc[n] = (acc.get(n, 0) + c) % 26
        terms = [(c, n) for n, c in acc.items() if c]
        const %= 26
        if not terms:
            if const:
                self.unsat = True
            return
        while len(terms) > 3:           # chain through auxiliary variables
            (c1, n1), (c2, n2) = terms[-2], terms[-1]
            self.naux = getattr(self, "naux", 0) + 1
            aux = self.var(f"_aux{self.naux}")
            self.lin([(c1, n1), (c2, n2), (-1, aux)])
            terms = terms[:-2] + [(1, aux)]
        piv = next((t for t in terms if t[0] in (1, 25)), None)
        if piv is None:        # no invertible coefficient: block every violating tuple
            names = [n for _, n in terms]
            for vals in itertools.product(range(26), repeat=len(names)):
                if (sum(c * v for (c, _), v in zip(terms, vals)) + const) % 26:
                    self.clauses.append([-self.lit[(n, v)] for n, v in zip(names, vals)])
            return
        rest = [t for t in terms if t is not piv]
        inv = 1 if piv[0] == 1 else 25
        for vals in itertools.product(range(26), repeat=len(rest)):
            s = sum(c * v for (c, _), v in zip(rest, vals)) + const
            forced = (-s * inv) % 26
            self.clauses.append([-self.lit[(n, v)] for (_, n), v in zip(rest, vals)]
                                + [self.lit[(piv[1], forced)]])

    def solve(self):
        if self.unsat:
            return False
        with Cadical153(bootstrap_with=self.clauses) as s:
            return s.solve()


# ------------------------------------------------------- mechanism encodings
# Encryption convention (Quagmire family):
#   vig : C(ct) = P(pt) + k      beau : C(ct) = k - P(pt)          (mod 26)
# qtype I: C = identity, P unknown | II: P = identity, C unknown
#       III: C = P unknown (K1/K2 system) | IV: C, P independent unknowns

def _alph(m, qtype):
    if qtype in ("I", "III", "IV"):
        m.perm("P")
    if qtype in ("II", "IV"):
        m.perm("C")

    def P(ch):
        if qtype == "II":
            return None, AZ.index(ch)
        return "P" + ch, None

    def C(ch):
        if qtype == "I":
            return None, AZ.index(ch)
        if qtype == "III":
            return "P" + ch, None
        return "C" + ch, None
    return P, C


def _relation(m, P, C, ct, pt, key_terms, mode, key_const=0):
    """Encode C(ct) - s*P(pt) - key == 0 with s=+1 (vig) or -1 (beau)."""
    terms, const = [], -key_const
    cv, cc = C(ct)
    pv, pc = P(pt)
    sgn = 1 if mode == "vig" else -1
    if cv: terms.append((1, cv))
    else: const += cc
    if pv: terms.append((-sgn, pv))
    else: const -= sgn * pc
    terms += [(-c, n) for c, n in key_terms]
    m.lin(terms, const)


def periodic(ct, anch, mode, p, qtype="III", idx=None):
    """Periodic key of period p; idx(i) = key index of plaintext position i."""
    m = Model()
    P, C = _alph(m, qtype)
    for i, pt in anch.items():
        j = i if idx is None else idx(i)
        _relation(m, P, C, ct[i], pt, [(1, f"k{j % p}")], mode)
    return m.solve()


def progressive(ct, anch, mode, p):
    m = Model()
    P, C = _alph(m, "III")
    for i, pt in anch.items():
        _relation(m, P, C, ct[i], pt, [(1, f"k{i % p}"), ((i // p) % 26, "s")], mode)
    return m.solve()


def rowcol(ct, anch, mode, w):
    m = Model()
    P, C = _alph(m, "III")
    for i, pt in anch.items():
        _relation(m, P, C, ct[i], pt, [(1, f"a{i // w}"), (1, f"b{i % w}")], mode)
    return m.solve()


def incrib(ct, anch, mode, p):
    """Periodic key whose phase may jump between the two cribs (nulls/omissions)."""
    m = Model()
    P, C = _alph(m, "III")
    for i, pt in anch.items():
        _relation(m, P, C, ct[i], pt, [(1, f"{'x' if i < 50 else 'y'}{i % p}")], mode)
    return m.solve()


def autokey(ct, anch, mode, L, src):
    m = Model()
    P, C = _alph(m, "III")
    n = 0
    for i, pt in anch.items():
        j = i - L
        if j < 0:
            continue
        if src == "CT":
            kch = ct[j]
        elif j in anch:
            kch = anch[j]
        else:
            continue
        _relation(m, P, C, ct[i], pt, [(1, "P" + kch)], mode)
        n += 1
    return m.solve() if n >= 3 else None


def runkey(ct, anch, mode, R, o):
    m = Model()
    P, C = _alph(m, "III")
    for i, pt in anch.items():
        if not 0 <= i + o < len(R):
            return None
        _relation(m, P, C, ct[i], pt, [(1, "P" + R[i + o])], mode)
    return m.solve()


def columnar_perm(n, key):
    """Encryption permutation of keyed columnar transposition: out[t] = in[perm[t]]."""
    w = len(key)
    order = sorted(range(w), key=lambda c: (key[c], c))
    perm = []
    for c in order:
        perm += list(range(c, n, w))
    return perm


def trans_periodic(ct, anch, mode, p, perm, order):
    """order 'TS': CT = Sub(Trans(PT)) ; 'ST': CT = Trans(Sub(PT)).
    perm: encryption transposition, out[t] = in[perm[t]]."""
    pos_of = {src: t for t, src in enumerate(perm)}   # where plaintext index lands
    m = Model()
    P, C = _alph(m, "III")
    for i, pt in anch.items():
        t = pos_of[i]
        kidx = t if order == "TS" else i
        _relation(m, P, C, ct[t], pt, [(1, f"k{kidx % p}")], mode)
    return m.solve()


# ------------------------------------------------------------ synthetic data
def rand_ct(seed):
    r = random.Random(seed)
    return "".join(r.choice(AZ) for _ in range(97))


def synth(seed, keyfn, mode, qtype="III", perm=None, order=None):
    """Build a ciphertext WITH the mechanism (positive control)."""
    r = random.Random(seed)
    Pa = list(range(26)) if qtype == "II" else r.sample(range(26), 26)
    Ca = {"I": list(range(26)), "III": Pa}.get(qtype) or r.sample(range(26), 26)
    pt = [r.choice(AZ) for _ in range(97)]
    for i, c in ANCH.items():
        pt[i] = c
    inv = {v: k for k, v in enumerate(Ca)}
    sgn = 1 if mode == "vig" else -1

    def enc(i, ch, kidx):
        return AZ[inv[(sgn * Pa[AZ.index(ch)] + keyfn(kidx, Pa, pt)) % 26]]
    if perm is None:
        return "".join(enc(i, pt[i], i) for i in range(97))
    if order == "TS":
        mid = [pt[perm[t]] for t in range(97)]
        return "".join(enc(t, mid[t], t) for t in range(97))
    sub = [enc(i, pt[i], i) for i in range(97)]
    return "".join(sub[perm[t]] for t in range(97))


# ------------------------------------------------------------------- driver
def run_family(name, fn, params, pos_fn, n_null=20):
    if name.startswith("runkey"):
        n_null = 5
    out = {}
    for mode in ("vig", "beau"):
        k4 = [x for x in params if fn(K4, ANCH, mode, x)]
        null_counts = [sum(1 for x in params if fn(rand_ct(9000 + s), ANCH, mode, x))
                       for s in range(n_null)]
        pos = pos_fn(mode)
        out[mode] = {"K4_sat": k4, "n_params": len(params),
                     "null_mean_sat_params": sum(null_counts) / n_null,
                     "null_frac_with_any_sat": sum(1 for c in null_counts if c) / n_null,
                     "positive_control": pos}
        print(name, mode, json.dumps(out[mode]), flush=True)
    return out


def main(selected):
    R = {}
    rk = random.Random(1)
    kwords = [rk.randrange(26) for _ in range(60)]
    s0 = 7

    fams = {}
    for q in ("I", "II", "III", "IV"):
        fams[f"periodic_Q{q}"] = (
            (lambda q: lambda ct, a, md, p: periodic(ct, a, md, p, q))(q), list(range(1, 53)),
            (lambda q: lambda md: periodic(synth(11, lambda j, Pa, pt: kwords[j % 10], md, q),
                                           ANCH, md, 10, q))(q))
    fams["progressive"] = (progressive, list(range(2, 49)),
                           lambda md: progressive(synth(12, lambda j, Pa, pt: kwords[j % 9] + s0 * (j // 9), md),
                                                  ANCH, md, 9))
    fams["rowcol"] = (rowcol, list(range(2, 49)),
                      lambda md: rowcol(synth(13, lambda j, Pa, pt: kwords[j // 21] + kwords[30 + j % 21], md),
                                        ANCH, md, 21))
    fams["incrib"] = (incrib, list(range(1, 13)),
                      lambda md: incrib(synth(14, lambda j, Pa, pt: kwords[(j - (3 if j > 50 else 0)) % 8], md),
                                        ANCH, md, 8))
    fams["autokey_CT"] = (lambda ct, a, md, L: autokey(ct, a, md, L, "CT"), list(range(1, 97)), None)
    fams["autokey_PT"] = (lambda ct, a, md, L: autokey(ct, a, md, L, "PT"), list(range(1, 97)),
                          lambda md: autokey(synth(15, lambda j, Pa, pt: Pa[AZ.index(pt[j - 42])] if j >= 42 else 0, md),
                                             ANCH, md, 42, "PT"))
    for src, text in (("K1PT", K1PT), ("K2PT", K2PT), ("K3PT", K3PT), ("K123PT", K1PT + K2PT + K3PT)):
        offs = list(range(-21, len(text) - 73))
        fams[f"runkey_{src}"] = (
            (lambda text: lambda ct, a, md, o: runkey(ct, a, md, text, o))(text), offs,
            (lambda text, offs: lambda md: runkey(
                synth(16, lambda j, Pa, pt: Pa[AZ.index(text[j + offs[len(offs) // 2]])]
                      if 0 <= j + offs[len(offs) // 2] < len(text) else 0, md),
                ANCH, md, text, offs[len(offs) // 2]))(text, offs))
    # K3 vocabulary: KRYPTOS-keyed columnar (width 7), both layer orders, forward and inverse
    base = columnar_perm(97, "KRYPTOS")
    inv = [0] * 97
    for t, s in enumerate(base):
        inv[s] = t
    for pname, perm in (("fwd", base), ("inv", inv)):
        for order in ("TS", "ST"):
            fams[f"columnarKRYPTOS_{pname}_{order}"] = (
                (lambda perm, order: lambda ct, a, md, p: trans_periodic(ct, a, md, p, perm, order))(perm, order),
                list(range(1, 27)),
                (lambda perm, order: lambda md: trans_periodic(
                    synth(17, lambda j, Pa, pt: kwords[j % 10], md, perm=perm, order=order),
                    ANCH, md, 10, perm, order))(perm, order))

    # autokey_CT positive control needs the ciphertext recursively: build explicitly
    def ak_ct_pos(md):
        r = random.Random(18)
        Pa = r.sample(range(26), 26)
        inv_ = {v: k for k, v in enumerate(Pa)}
        pt = [r.choice(AZ) for _ in range(97)]
        for i, c in ANCH.items():
            pt[i] = c
        sgn = 1 if md == "vig" else -1
        ct = []
        for i in range(97):
            k = Pa[AZ.index(ct[i - 5])] if i >= 5 else 3
            ct.append(AZ[inv_[(sgn * Pa[AZ.index(pt[i])] + k) % 26]])
        return autokey("".join(ct), ANCH, md, 5, "CT")
    fams["autokey_CT"] = (fams["autokey_CT"][0], fams["autokey_CT"][1], ak_ct_pos)

    # Real-K1 positive control for the core periodic Quagmire III encoding
    k1ct = "".join(KA[(KA.index(c) + KA.index("PALIMPSEST"[i % 10])) % 26] for i, c in enumerate(K1PT))
    assert k1ct.startswith("EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJ")
    k1anch = {i: K1PT[i] for i in list(range(21, 34)) + list(range(50, 61))}
    R["K1_real_control"] = {p: periodic(k1ct, k1anch, "vig", p, "III") for p in (7, 8, 9, 10, 11, 20)}
    print("K1 real control (QIII vig, true period 10):", R["K1_real_control"], flush=True)

    for name, (fn, params, pos_fn) in fams.items():
        if selected and name not in selected:
            continue
        R[name] = run_family(name, fn, params, pos_fn)
    here = os.path.dirname(os.path.abspath(__file__))
    tag = "_".join(selected) if selected else "all"
    with open(os.path.join(here, f"results_{tag}.json"), "w") as f:
        json.dump(R, f, indent=1)


if __name__ == "__main__":
    main(sys.argv[1:])
