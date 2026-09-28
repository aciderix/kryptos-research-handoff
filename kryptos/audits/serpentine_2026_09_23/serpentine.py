"""Boustrophedon ('serpentine') transposition + periodic Quagmire III (relay Gemini, 23/09).
Cribs are taken as PLAINTEXT positions (Sanborn, March 2019: 'BERLIN is plaintext that starts at exactly the 64th
character'); the transposition may move them anywhere in the ciphertext.
Plaintext written row by row in a grid of width w = 7..31; read out along a snake:
  rowsLR (row 0 left->right, row 1 right->left, ...), rowsRL, colsDown (col 0 top->bottom, col 1 bottom->top, ...), colsUp.
Orders: TS = transpose then substitute (key follows ciphertext order), ST = substitute then transpose.
Quagmire III, ANY alphabet, periodic key p = 1..26, Vig/Beau, exact.  Null (10 random ciphertexts) only where K4 is SAT."""
import sys, json
sys.path.insert(0, "../algebraic_elimination_2026_09_22")
from k4_algebraic import trans_periodic, K4, ANCH, rand_ct
N = 97
def snake(w, kind):
    rows = (N + w - 1) // w
    order = []
    if kind.startswith("rows"):
        for r in range(rows):
            cols = list(range(w))
            if (r % 2 == 1) == (kind == "rowsLR"): cols.reverse()
            order += [r * w + c for c in cols if r * w + c < N]
    else:
        for c in range(w):
            rs = list(range(rows))
            if (c % 2 == 1) == (kind == "colsDown"): rs.reverse()
            order += [r * w + c for r in rs if r * w + c < N]
    assert sorted(order) == list(range(N))
    return order                      # out[t] = in[order[t]]
if __name__ == "__main__":
    out = open("results.jsonl", "w")
    for w in range(7, 32):
        for kind in ("rowsLR", "rowsRL", "colsDown", "colsUp"):
            perm = snake(w, kind)
            for order in ("TS", "ST"):
                for mode in ("vig", "beau"):
                    sat_p = [p for p in range(1, 27) if trans_periodic(K4, ANCH, mode, p, perm, order)]
                    nul = {p: sum(trans_periodic(rand_ct(4000 + s), ANCH, mode, p, perm, order) for s in range(10)) for p in sat_p}
                    r = dict(w=w, kind=kind, order=order, mode=mode, K4_sat_p=sat_p, null_of_10=nul)
                    out.write(json.dumps(r) + "\n"); out.flush()
    out.write("DONE\n")
