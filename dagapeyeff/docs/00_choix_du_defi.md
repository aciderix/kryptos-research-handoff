# Choix du défi : d'Agapeyeff (1939) plutôt que Zodiac Z32 (1970)

Décision prise le 2026-09-28, avant toute expérience. Critères demandés par le user, évalués sur les faits
de l'audit (`01_etat_de_lart.md`), pas sur la facilité apparente.

| Critère | d'Agapeyeff | Z32 |
|---|---|---|
| **Corpus** | 395 chiffres, 2 transcriptions indépendantes identiques ; imprimé (1939) | 32 symboles, dessinés à la main (lectures de glyphes discutables) |
| **Contraintes connues** | Très fortes et *mesurables sur le chiffré seul* : alternance parfaite {6789 0}/{12345} sur 392 chiffres ; 196 = 14×14 ; IC×25 = 1,74 (≈ anglais) ; 18 symboles ; symboles rares tous dans la colonne 14 (p≈10⁻⁸) | Quasi aucune interne (3 symboles répétés sur 32) ; le reste est un contexte (carte, « set to Mag. N. ») à interpréter |
| **Espace de recherche** | Grand mais **structuré** : carré de Polybe (substitution simple) × transposition d'une grille 14×14 — familles historiques finies (le livre enseigne Polybe, nulles, transpositions) | Homophonique (Z408/Z340) : l'espace des clés dépasse largement ce que 32 symboles peuvent contraindre |
| **Qualité des contrôles** | Excellente : on génère des messages anglais synthétiques Polybe + transposition 14×14 et on vérifie la récupération | Faible : sur 32 caractères, un solveur « récupère » aussi des messages qui n'y sont pas |
| **Vérification exacte** | Oui : ré-enchiffrement déterministe → les 392 chiffres doivent revenir à l'identique | Non : sans clé homophonique connue, pas de ré-enchiffrement contraignant |
| **Unicité / preuve possible** | 196 lettres ≫ distance d'unicité d'une substitution simple (~25-30 lettres) : un anglais continu de 196 lettres sous un mécanisme simple serait probant sans oracle externe | 32 < distance d'unicité d'un homophonique ⇒ **plusieurs « solutions » plausibles coexistent par construction** ; toute preuve exigerait un oracle externe (inexistant) |
| **Travaux antérieurs** | Modérés et inventoriables (Shulman 1952, Cryptogram 1959, Barker 1978, Pelling 2008, numberworld 2013, Schmeh/MTC3, dagapeyeffresearch.com, GitHub) | Très nombreux et hétérogènes (forums, Praetorian, Foxon IACR 2023, Zenodo 2026…) |
| **Risque de faux positifs** | Modéré, maîtrisable par null + contrôles | **Maximal** : c'est le cas d'école du choix d'interprétation géographique après coup |
| **Réserve** | L'auteur dit avoir oublié sa méthode ; une **erreur d'enchiffrement** est possible (pourrait empêcher toute inversion exacte) | — |

**Choix : d'Agapeyeff.** Ce n'est pas le plus « facile » : l'échec répété depuis 1939 suggère une couche
non triviale ou une erreur. Mais c'est le seul des deux où une résolution peut être **démontrée** (contrôles,
null, ré-enchiffrement exact, unicité statistique). Z32 ne pourrait produire, au mieux, qu'une interprétation
parmi d'autres.
