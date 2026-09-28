# Transposition en serpentin (boustrophédon) + Quagmire III périodique (relais Gemini, 23/09)

**Pourquoi** : j'avais écarté le serpentin par un raisonnement (« les paires publiées NYPVTT → BERLIN seraient inversées »). Or Sanborn a reculé en mars 2019
et ne garantit que « BERLIN est du clair qui commence au 64ᵉ caractère ». Test fait proprement : les cribs sont des **positions dans le clair**, et la transposition peut les déplacer.

**Portée** (fixée avant calcul) : clair écrit ligne par ligne en largeur w = 7–31 ; lecture en serpentin par lignes (2 sens) ou par colonnes (2 sens) ;
transposition avant (TS) ou après (ST) la substitution ; Quagmire III, **n'importe quel alphabet**, clé périodique p = 1–26, Vig/Beau ; exact.
Budget de 200 000 conflits par appel au solveur (cas trop durs = « non tranché », 13 cases sur 10 400).
Témoins : (1) 10 chiffrés aléatoires pour chaque case p ≤ 20 où K4 passe ; (2) **témoin global** : le même balayage complet (400 × p 1–20) sur 6 chiffrés aléatoires.

**Résultats**
- Cases compatibles avec K4 : 8 (p ≤ 8), 85 (p ≤ 12), 270 (p ≤ 16), 552 (p ≤ 20).
- Témoin global (6 chiffrés aléatoires) : p ≤ 8 : 4–27 ; p ≤ 12 : 83–178 ; p ≤ 16 : 251–471 ; p ≤ 20 : 501–774.
  **K4 est au niveau du hasard, voire en dessous** : 5 chiffrés aléatoires sur 6 ont autant ou plus de cases compatibles (p ≤ 12, 16, 20).
- Les 159 cases « sévères » (témoins ≤ 1/10) ne sont que l'effet du nombre de cases testées (≈ 8 000) et du bruit de 10 témoins par case.
- **Déchiffrement** des 8 cases les plus frappantes (p = 7 à 10, témoins 0/10) : charabia (`…TFMHDJFPBIXQNSYR…`, `…YWACYUNBRDQHLR…`), aucun mot.
- ⇒ **éliminé en pratique** : aucun serpentin (w 7–31) suivi ou précédé d'une substitution périodique ne se distingue du hasard.
