"""families.py — familles de clés structurées : name -> fonction i -> ([(variable, coef)], constante)."""
QM = [("Q3", "VIG"), ("Q3", "BEAU"), ("Q4", "VIG"), ("Q2", "VIG"), ("Q2", "BEAU"), ("Q1", "VIG")]

def get(name):
    kind, *a = name.split(":")
    a = list(map(int, a))
    if kind == "per":            # clé périodique p
        p, = a; return lambda i: ([(i % p, 1)], 0)
    if kind == "restart":        # mot-clé de période p recommencé à chaque ligne de largeur W ; K4 commence en colonne o
        W, o, p = a; return lambda i: ([(((i + o) % W) % p, 1)], 0)
    if kind == "block":          # disque tourné par bloc de n (phase φ), pas s dans le bloc
        n, phi, s = a; return lambda i: ([(100 + (i - phi + n) // n, 1)], s * ((i - phi) % n))
    if kind == "rowcol":         # clé ligne + colonne, largeur W, K4 commence en colonne o
        W, o = a; return lambda i: ([((i + o) // W, 1), (200 + (i + o) % W, 1)], 0)
    if kind == "prog":           # clé périodique p + progression s par cycle
        p, s = a; return lambda i: ([(i % p, 1)], s * (i // p))
    raise ValueError(name)

def group(g):
    if g == "periodic":
        return [(f"per:{p}", q, m) for p in range(1, 27) for (q, m) in QM]
    raise ValueError(g)

def line31(i, o=27, W=31):
    return (i + o) // W

def get2(name):
    kind, *a = name.split(":")
    a = list(map(int, a))
    if kind == "perline":        # clé de période p + décalage libre par ligne de largeur W (K4 commence en colonne o)
        p, W, o = a
        return lambda i: ([(i % p, 1), (300 + (i + o) // W, 1)], 0)
    return get(name)
