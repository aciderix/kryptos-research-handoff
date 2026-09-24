# Outil « en profondeur » K4/K5 (prêt pour la publication de K5)
`k5_depth.py <K5>` : aucun paramètre libre. Voir `docs/veille_k5_et_demande_archives.md` §3.
Autotest (23/09) : avec un faux K5 qui partage le flux de clé de K4 (Vigenère, alphabet KRYPTOS) et un mot commun en positions 40–51,
l'outil trouve le mot (témoin mélangé : 0/1000) et affiche le clair caché `NORTHWESTXXXX` dans la bonne convention.
