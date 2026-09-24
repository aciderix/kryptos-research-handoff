# Audit « recuit » (24/09/2026, soir) : attaque sur les 97 lettres — outil en cours, aucune conclusion

**But.** Les familles de clé à pas 7 qui résolvent les conflits des cribs (décalage d'alphabet par ligne, par ligne de 7…) sont trop lâches pour être jugées sur les 24 lettres (`../erreurs_multiples_2026_09_24/`). Seules les 73 lettres hors cribs peuvent trancher : il faut une attaque sur le texte entier, notée par un modèle de l'anglais, et validée d'abord sur des chiffrés synthétiques du même modèle.

| Fichier | Rôle |
|---|---|
| `build_qg.c` | table de quadrigrammes anglais (log10) depuis le corpus (`../erreurs_multiples_2026_09_24/corpus_get.sh`, 16,4 M quadrigrammes) |
| `sa.c` | recuit sur alphabet(s) et clé ; cribs « souples » (une lettre fausse coûte au lieu d'être interdite) |
| `sa2.c` | recuit sur l'alphabet seul ; la clé structurée (période p + décalage par segment) est **déduite des cribs par vote** |
| `synth.py` | chiffrés synthétiques du même modèle (texte anglais, cribs insérés, alphabet à mot-clé) |

**Résultat : l'outil ne passe pas encore le contrôle positif.** Sur un Quagmire III synthétique de période 7 (alphabet à mot-clé, 97 lettres, cribs exacts), `sa2` atteint 21 à 22 lettres des cribs sur 24 après 20 millions d'itérations, mais le clair reste faux. Le vrai clair a un score bien meilleur (≈ −116 contre ≈ −281) : c'est la recherche qui échoue, pas le modèle.

**Conséquence.** Aucun essai n'a été fait sur K4 : un échec sur K4 ne voudrait rien dire tant que le contrôle positif échoue. Pistes pour la suite : partir des solutions exactes des cribs (CP-SAT) et ne compléter par recuit que les lettres de l'alphabet qu'ils laissent libres ; mouvements guidés par les colonnes de la période. Le registre (`MEMORY.md`, C-EFRAC54-01) note déjà la difficulté des recherches par quadrigrammes à 97 lettres.
