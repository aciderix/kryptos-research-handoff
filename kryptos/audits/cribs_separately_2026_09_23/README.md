# Chaque crib SEUL (relais du 23/09 : « deux messages / deux clés indépendants »)
Motivation relayée : « K4 (2 pièces) » dans l'inventaire de la vente (base 1), deux événements (Égypte 1986, Berlin 1989), « keywords » au pluriel (AP 1991).
Déjà couvert : `incrib` (22/09) = une clé périodique **indépendante par crib**, même alphabet quelconque ⇒ Vig p ≤ 11 et Beau p ≤ 7, 9, 10 éliminés ;
clé recommencée à chaque ligne de 31 (base 2, `one_slip`) ⇒ bruit.

**Ici, lien totalement rompu entre les deux cribs :**
1. **Clé nécessaire, alphabet fixé** (système de K1–K2) : entièrement déterminée pour chaque crib (`results.json`). Exemples : EASTNORTHEAST → `BLZCDCYYGCKAZ` (A–Z, Vig),
   `RDUMRIYWOYNKY` (KRYPTOS, Vig) ; BERLINCLOCK → `MUYKLGKORNA` (A–Z, Vig), `ELYOIECBAQK` (KRYPTOS, Vig). **Aucun mot lisible dans les 12 versions** : aucun mot-clé
   (Égypte, Carter, Berlin…) ne chiffre un crib avec un alphabet fixé.
2. **Chaque crib seul, Quagmire III tout alphabet, clé périodique** : Vigenère éliminé pour toute période plus courte que le crib (seule exception : p = 12 pour
   EASTNORTHEAST, où 86 % des témoins passent aussi) ; Beaufort : quelques périodes compatibles, toutes au niveau du hasard (30–90 % des témoins).
⇒ **aucun signal**, même en séparant totalement les deux cribs.
Remarque : ce que Kobek et Byrne ont dit (base 1, [S]) concerne la **lisibilité** du clair, pas le mécanisme ; nos tests n'ont jamais utilisé de score d'anglais.
