# Veille K5 et demande aux Archives of American Art (23/09/2026)

## 1. État des lieux
- **K5 n'est pas publié.** Paradigm (billet du 12/06/2026, https://paradigm.xyz/2026/06/kryptos) : « We plan to release K5 in the future », sans date.
  Une autre source indique une publication **une fois K4 résolu**. Les deux sont à surveiller.
- Ce qu'on sait de K5 (base 1) : 97 caractères ; **mots codés partagés avec K4, aux mêmes positions** (Sanborn 2025) ;
  « K4 alternatif » de 1988 avec **sa propre charte** ; lié à « it's buried out there somewhere » ; destiné à un « espace public ».
- Règle : on n'utilise **ni** le clair acheté par Paradigm **ni** son vérificateur payant pour ajuster une méthode (une seule soumission, à la toute fin).

## 2. Où surveiller
| Source | Quoi | Fréquence |
|---|---|---|
| https://paradigm.xyz/writing/kryptos et https://paradigm.xyz/2026/06/kryptos | publication du chiffré de K5 | mensuelle |
| https://www.elonka.com/kryptos/ et la liste Kryptos (groups.io) | relais rapide de toute nouveauté | mensuelle |
| Wikipédia « Kryptos » (historique des modifications) | nouvel indice, nouvelle date | mensuelle |
| Presse : Scientific American, NYT, Wired | entretiens de Sanborn | au fil de l'eau |

## 3. Protocole le jour J (chiffré de K5 publié)
1. Recopier le chiffré **exactement** (97 caractères, « ? » compris) depuis la source officielle ; archiver la page (URL + date).
2. Lancer `python3 audits/k5_depth/k5_depth.py <K5>` (outil prêt, autotesté). Il donne :
   - les positions où K4 et K5 ont la **même lettre chiffrée**, et les suites de 3 lettres ou plus (mot partagé avec la même clé et la même charte) ;
   - le **clair de K5 déduit** aux 24 positions connues de K4, sous l'hypothèse d'un flux de clé commun, en A–Z et en KRYPTOS, en Vigenère et en Beaufort.
     Si K5 partage le flux de clé de K4, **l'une des quatre lignes est lisible**.
   - un témoin par mélange aléatoire.
3. Si une ligne est lisible : les 24 lettres de K5 étendent les cribs et relancent **tous** les tests exacts de la base 2 avec 48 contraintes au lieu de 24 (le mur de 112 bits recule d'autant).
4. Si rien n'est lisible (chartes différentes) : utiliser les mots partagés (étape 1) comme **nouveaux cribs de position** pour K4 et K5 conjointement.

## 4. Demande aux Archives of American Art (brouillon, à envoyer via le formulaire « Ask Us » de l'AAA)

> Subject: Jim Sanborn papers — non-digitized Kryptos materials (audio of 1990 dedication; "Stencil Patterns")
>
> Dear Reference Team,
>
> I am researching Jim Sanborn's *Kryptos* (1990) using the digitized folders of the Jim Sanborn papers, circa 1945-2024
> (finding aid revised 2026-05-06), which have been very helpful.
>
> Could you please tell me whether the collection holds the following, and whether they can be consulted or reproduced:
> 1. An **audio cassette of the dedication ceremony of 5 November 1990** at CIA headquarters. A CIA Protocol Branch letter of
>    3 January 1991 to Mr. and Mrs. Herbert Sanborn (Scrapbook, box 16, folder 2) lists "Audio tape of ceremony" as an enclosure.
> 2. The folder titled **"Stencil Patterns, circa 1988"**, listed in Series 3 of the 2025 version of the finding aid but absent
>    from the 2026 revision. Has it been merged into another folder, or restricted?
> 3. Any **photographs of the sculpture** in box 11, folder 34 ("Kryptos, undated") and the 2026 additions in box 18
>    (folders 10–11), which are not digitized.
>
> I understand that some Kryptos materials may be restricted; I am only asking about what is open to researchers.
>
> Thank you very much for your help.
