# Base documentaire 1 — Sanborn, l'œuvre, ce qu'il met à disposition

**Version :** 2026-09-23 (première constitution, à enrichir)
**But :** rassembler tout ce qui est publiquement connu de Jim Sanborn et de Kryptos, pour comprendre le mécanisme **par l'intention de l'artiste** avant tout calcul.
**Règle d'usage :** chaque élément porte un niveau de fiabilité. Une citation obtenue par un résumé automatique de page web est marquée *(à vérifier sur la source)* tant qu'elle n'a pas été relue dans l'original.

| Niveau | Signification |
|---|---|
| **P** | Primaire : Sanborn ou Scheidt eux-mêmes (interview, lettre, discours), ou institution (CIA, Smithsonian) |
| **S** | Secondaire fiable : presse nationale, Wikipédia avec source, chercheurs reconnus (Dunin, Bean, Rumkin) |
| **C** | Communauté : blogs, forums, dépôts ; utile, jamais suffisant seul |
| **X** | **Exclu** : fuite supposée, clair non publié, affirmation invérifiable. Consigné pour ne pas s'y laisser influencer |

---

## 1. L'artiste

- Herbert James Sanborn Jr., né le 14 novembre 1945 à Washington. Son père organisait des expositions à la Bibliothèque du Congrès ; sa mère était pianiste de concert et iconographe. **[S]** [W-JS]
- Licence en paléontologie, beaux-arts et anthropologie sociale (Randolph-Macon, 1968), puis MFA en sculpture (Pratt, 1971). **[S]** [W-JS]
- Le fil de toute son œuvre : **« rendre visible l'invisible »**. Magnétisme, force de Coriolis, réactions atomiques, messages secrets. **[S]** [W-JS]
- Œuvres apparentées :
  - ***Invisible Forces*** : aiguilles de boussole suspendues, pierres et **magnétite** (pierre naturellement aimantée). Le champ terrestre et un champ opposé y travaillent « avec et contre » l'autre. **[S]** [AS]
  - ***Cyrillic Projector*** (années 1990) : texte cyrillique chiffré, dont un extrait de document du KGB.
  - ***Antipodes*** (1997) : reprend une partie du texte de Kryptos, avec de légères différences (par exemple `UNDERGROUND` correctement orthographié).
  - ***Lingua*** (2002), ***Critical Assembly*** (2003, projet Manhattan), ***Terrestrial Physics*** (2010, accélérateur de particules), ***Coastline*** (1993). **[S]** [W-K] [W-JS]
- Rapport revendiqué aux mathématiques : « J'ai eu la chance de ne pas comprendre les mathématiques, probablement dans ma capacité à faire le code. » **[P]** (2025, via Scientific American) [SA-25b]

**Lecture artistique** *(interprétation, pas un fait)* : chez Sanborn, le thème récurrent est une **force invisible qui dévie un repère** (boussole, champ magnétique, rayonnement). Le « secret » y est traité comme un phénomène physique à rendre perceptible, pas seulement comme un problème de calcul.

## 2. Le site : ce que l'artiste met physiquement à disposition

Kryptos n'est pas seulement l'écran de cuivre. Inauguré le 3 novembre 1990 à Langley. **[P]** [CIA] **[S]** [W-K]

| Élément | Description | Niveau |
|---|---|---|
| Écran de cuivre en S | **Côté gauche** : K1–K4 (869 caractères, dont 4 « ? »). **Côté droit** : tableau de Vigenère à alphabet KRYPTOS (867 lettres) | S [W-K] |
| **« L » supplémentaire** dans une ligne du tableau | Il forme « HILL » verticalement dans la colonne de droite (lecture suggérée : chiffre de Hill, Bauer/Link/Molle) | S [W-K] |
| Lettres **« YAR » surélevées** près du bas | Sanborn : « une lettre a été omise… pour des raisons esthétiques » (avril 2006) | S [W-K] |
| Plaques de granit + cuivre à l'entrée (**K0**, Morse) | `SOS`, `RQ`/`YR`, `LUCID MEMORY`, `SHADOW FORCES`, `(WHA)T IS YOUR POSITION`, `DIGETAL INTERPRETATI(U)`, `VIRTUALLY INVISIBLE`, avec de nombreux « E » en surnombre. Certaines erreurs sont dites intentionnelles | C [RK0] |
| **Rose des vents gravée pointant vers une magnétite** | La pierre aimantée dévie l'aiguille : le « nord » local n'est pas là où on l'attend. Direction de déviation rapportée : ouest-sud-ouest *(à vérifier)* | S [W-K] / C [inteltoday] |
| Bois pétrifié, bassin, plantations | Éléments paysagers de l'installation | S [CIA] |

**Observation à garder en tête :** le crib `EASTNORTHEAST` est une **direction de boussole**, et l'installation contient une **boussole volontairement faussée par un aimant**. Chez un artiste dont c'est le sujet central, ce rapprochement est de l'ordre de l'intention, pas du hasard. *(Interprétation. Il ne fixe aucun paramètre.)*

## 3. K1–K3 : ce qu'ils enseignent

| | Méthode | Clé | Clair (extrait) | Niveau |
|---|---|---|---|---|
| K1 | Vigenère, tableau KRYPTOS (Quagmire III) | PALIMPSEST | « BETWEEN SUBTLE SHADING AND THE ABSENCE OF LIGHT LIES THE NUANCE OF IQLUSION » | S [W-K] |
| K2 | idem | ABSCISSA | coordonnées 38°57′6.5″N 77°8′44″W, « IT WAS TOTALLY INVISIBLE », « EARTHS MAGNETIC FIELD », « BURIED OUT THERE SOMEWHERE », fin « X LAYER TWO » (corrigé) / « ID BY ROWS » (gravé) | S [W-K] |
| K3 | Double transposition de route (8×42 puis 24×14, colonnes lues de bas en haut). **Équivaut à une multiplication des positions : ×192 mod 337** | — | récit d'Howard Carter à l'ouverture du tombeau de Toutânkhamon (26 nov. 1922), fin « CAN YOU SEE ANYTHING Q » | S [W-K] + dérivé interne (`docs/k3_chart_layout_and_route_2026_09_19.md`, §2.5 du rapport algébrique) |

**Anomalies volontaires ou revendiquées :**
- Fautes : `IQLUSION` (K1), `UNDERGRUUND` (K2), `DESPARATLY` (K3), `DIGETAL` (K0). Sanborn : « La plupart de mes choses sont truffées d'erreurs, exprès » (2005). **[P]** [W-K]
- K2 : une lettre omise sur le cuivre (« S ») fait lire `IDBYROWS` au lieu de `XLAYERTWO`. Correction orale en 2006, jamais sur le cuivre. **[P]** [W-K]
- Anagramme communautaire : PALIMPSEST + ABSCISSA → « P.S. IT'S AS SIMPLE AS ABC ». **[C]** [W-K]

**Ce que K1–K3 disent de la manière de Sanborn** *(interprétation)* :
- Les clairs parlent de **lumière et d'ombre**, d'**invisibilité**, de **magnétisme terrestre**, d'un **lieu à trouver** (coordonnées, « buried »), et d'une **ouverture** (tombeau, « can you see anything »). Le thème commun : percevoir ce qui est caché.
- Les clés sont des mots qui désignent des **opérations** : *palimpseste* = écriture superposée ; *abscisse* = coordonnée sur un axe.

## 4. Ce que Sanborn et Scheidt ont dit de la méthode (chronologique)

| Date | Qui | Déclaration | Niveau | Source |
|---|---|---|---|---|
| 1990 (inauguration) | Sanborn | « Il y a deux systèmes de chiffrement du texte du bas… et c'est un indice majeur en soi. » | P | dépôt `docs/documentary_research_2026_09_21.md` [4] |
| 1990 (CIA) | CIA | Le tableau de Vigenère et des « systèmes matriciels » ont servi aux trois premiers textes ; K4 a été conçu plus difficile | P | [CIA] |
| janv. 2005 | Scheidt | « J'ai masqué la langue anglaise » dans le 4ᵉ procédé ; la technique « peut ne pas être connue » ; il ne sait pas ce que Sanborn a finalement modifié | P | Wired 2005 (via dépôt) |
| janv. 2005 | Sanborn | « J'ai utilisé certains codes matriciels qu'Ed m'a donnés, et j'ai aussi conçu des **systèmes visuels d'encodage**, bien plus durs à casser pour les cryptographes parce qu'**individuels**. » | P *(à vérifier sur la source)* | Wired 2005 (via kryptosfan) [KF-m] |
| janv. 2005 | Sanborn | « En artiste visuel, j'aime m'appuyer sur des systèmes qui incluent du **visuel (matériel)** autant que du numérique déchiffrable par machine. » | P *(à vérifier)* | idem |
| janv. 2005 | Sanborn | « J'ai laissé dans le texte antérieur des **instructions qui renvoient au texte suivant**. » | P *(à vérifier)* | idem |
| 2005 | Sanborn | Ed lui a donné des idées de systèmes « qui ne dépendaient pas nécessairement des mathématiques », à partir de systèmes anciens et contemporains qu'il pouvait « modifier d'une myriade de façons » | P/C | [RK4] |
| — | Scheidt | Changement intentionnel « de méthodologie » pour K4 ; difficulté « environ 9 sur 10 » ; prévu pour être résolu « en 5 à 10 ans » | P (rapporté) | [W-K] |
| — | Sanborn | Interrogé sur le « masking » évoqué par Scheidt, il n'aurait pas compris le terme | C | [KF-m] résumé |
| 2009 | Sanborn | Souhaite que l'énigme dure « un siècle, avec un peu de chance longtemps après ma mort » ; se dit « anathemath » (réfractaire aux maths) à plusieurs reprises | P (via Bean 2021) | AAA Oral History 2009 ; CNN 2010 |
| 8 oct. 2011 (dîner Kryptos, Zola, Washington) | Scheidt | « [La cryptographie de K4] n'est pas mathématique (ce qui n'empêche pas de la modéliser mathématiquement), elle est **simple, peut être mémorisée et exécutée des années plus tard** avec le ou les **bons mots-clés**. » | P (rapporté par E. Hannon, cité par Bean 2021) | kryptos.groups.io msg 12611 |
| 2011 | Scheidt | « La cryptographie de K4 est semblable à ce qu'on fournirait à des **agents ou des pilotes en cas de capture**. » | P (rapporté, idem) | idem |
| 2015 | Scheidt | Il considérerait le chiffrement de K4 comme « **plus d'une étape** » | P (Schmeh, atelier Kryptos 2015, vidéo) | via Bean 2021 |
| 2019 (documentaire CNN) | **Sanborn** | « **BERLINCLOCK en clair correspond directement à NYPVTTMZFPK. C'est une correspondance un-à-un** : on prend le B clair, on lui applique le chiffrement, et il en sort un N chiffré ; puis le E clair est chiffré en Y. » | **P** (Bogart 2019, cité par Bean 2021) | scienceblogs.de, Klausis Krypto Kolumne, 15/03/2019 |
| 2020 | Scheidt | « Si l'on peut **changer la base du langage**, cela devient à mon avantage… quand c'était utilisé comme **masque**… » | P (transcription A.J. Jacobs, citée par Bean) | kryptos.groups.io, EdScheidtKryptosTranscript.pdf |
| 2006 | Sanborn | « Les réponses des trois premiers passages contiennent des indices pour le quatrième. » | P (rapporté) | [W-K] |
| 2010 → 2020 | Sanborn | Indices positionnels (voir §5) | P | presse |
| 2014 | Sanborn | « Il y a plusieurs horloges vraiment intéressantes à Berlin… Vous feriez mieux de vous plonger dans cette horloge-là. » | P | [RK4] [W-K] |
| 2020 | Sanborn | « Le pouvoir réside dans un secret, pas sans lui. » | P | Washington Post (via [W-K]) |
| août 2025 | Sanborn | Lettre ouverte : vente aux enchères, K5 existe, ne sera révélé qu'après la résolution de K4 | P | [OL25] (page non récupérée ici ; à relire) |
| nov. 2025 | Sanborn | « **Qui dit que c'est même une solution mathématique ?** » ; conseille la « créativité » | P | [SA-25b] |
| 2025 | Sanborn | « Ils l'ont découvert. Ils ne l'ont pas déchiffré. Ils n'ont pas la clé. Ils n'ont pas la méthode. » ; « Ce qu'ils ont trouvé, c'est du texte brouillé, pas une résolution du cryptogramme. » ; « K4 a été découvert et il pointe vers K5. » | P | [RR-disc] |

## 5. Indices officiels sur K4

| Date | Indice | Positions (1-indexées) | Niveau |
|---|---|---|---|
| nov. 2010 (NYT) | `NYPVTT` → `BERLIN` | 64–69 | P |
| nov. 2014 | `MZFPK` → `CLOCK` | 70–74 | P |
| 29 janv. 2020 (NYT) | `QQPRNGKSS` → `NORTHEAST` | 26–34 | P |
| août 2020 (communiqué dès avril à quelques personnes) | `FLRV` → `EAST` | 22–25 | P |
| 2025 | `BERLINCLOCK` = l'**horloge universelle (Weltzeituhr) d'Alexanderplatz**, lieu de rassemblement des foules qui ont fait tomber le Mur | — | P [SA-25b] |
| 2025 | Deux événements : un **voyage en Égypte en 1986** et la **chute du Mur de Berlin en 1989** | — | P [SA-25b] |
| 2025 | Le thème des codes de Kryptos : **« délivrer un message »**, du Morse jusqu'à K5 | — | P [SA-25b] |
| 2025 | **K5** : 97 caractères, partage certains mots codés **aux mêmes positions** que K4, lié à « it's buried out there somewhere » (K2), apparaîtra dans un « espace public » | — | P [SA-25b] |

## 6. L'archive de 2025 (ce qu'on sait sans l'avoir vue)

- En 2025, Jarett Kobek et Richard Byrne ont trouvé aux Archives of American Art (Smithsonian) des **fragments de texte brouillé** qui leur ont permis de reconstituer le clair. Ce sont des copies de documents de 1990 destinés à montrer au Department of Historical Intelligence de la CIA que le texte n'était pas offensant. **[S]** [RR-disc]
- Sanborn a confirmé l'authenticité du clair et fait sceller le fonds pour 50 ans (jusqu'en 2075). **[S]** [W-K]
- Kobek : « Il n'y a aucune chance que ce soit une résolution cryptographique. » Byrne : il fallait absolument les indices de l'artiste pour lui donner un sens. **[S]** [RR-disc]
- Le lot vendu 962 500 $ contient notamment : le clair manuscrit de K4 avec une lettre signée de Scheidt, **« le système de codage original de K4 »**, les **clairs manuscrits utilisés pour la découpe de l'écran de K4 (2 pièces)**, les « textes brouillés montrés au Department of Historical Intelligence », un **K1 alternatif de 1988** avec sa charte, et un **« K4 alternatif de 1988 » = K5** avec sa charte. **[P]** [RR-lot]

**Ce qui en ressort** *(interprétation)* :
- Le mot « **brouillé** » (*scrambled*) est employé par Sanborn lui-même pour l'état intermédiaire du texte.
- « Clairs utilisés pour la **découpe de l'écran** » indique que la gravure elle-même est une étape documentée du procédé.
- K5 étant un « K4 alternatif » avec sa propre charte et des mots aux **mêmes positions**, la méthode de K4 est **positionnelle** et réutilisable avec une autre charte.

## 7. Informations exclues (niveau X)

| Affirmation | Où | Pourquoi exclue |
|---|---|---|
| « THE COMPASS ROSE IS HERE » serait le début du clair de K4 | résumé d'un résultat de recherche web (source non identifiée, 2026-09-23) | Clair non publié, source invérifiable. Si c'est une fuite de l'archive scellée, l'utiliser violerait la règle « ne pas ajuster sur un clair privé ». Consigné pour que personne ne s'en serve à son insu |
| « La clé de Kryptos est KOMITET » | message instantané anonyme reçu par E. Dunin | Anonyme, jamais confirmé |

## 8. Ce que l'artiste semble dire, rassemblé *(synthèse interprétative)*

0. **Correspondance lettre à lettre, à la même position** : affirmée par Sanborn lui-même (CNN 2019). C'est la contrainte la plus forte de toute cette base : elle rend improbable une transposition entre le clair et le chiffré gravé. Les tests statistiques de Bean vont dans le même sens.
1. **Deux systèmes**, et le fait qu'il y en ait deux est « un indice majeur » (1990). Scheidt parle de « plus d'une étape » (2015). Compatible avec le point 0 si les deux étapes sont une **fabrication de clé** puis une **substitution lettre à lettre**, comme dans le Gromark.
1 bis. **Simple, mémorisable, exécutable des années plus tard avec le(s) bon(s) mot(s)-clé(s)**, du niveau d'un **chiffre de terrain pour agent ou pilote** (Scheidt 2011). Et **« changer la base du langage »** (Scheidt 2020).
2. L'un des deux peut être **visuel/matériel et individuel**, conçu par l'artiste, et non tiré d'un manuel (2005).
3. Les **textes précédents contiennent des instructions** pour les suivants (2005, 2006).
4. Ce **n'est peut-être pas une solution mathématique** (2025).
5. Le texte passe par un état **« brouillé »** ; la **découpe de l'écran** fait partie du procédé (archive 2025).
6. Le message parle d'un **lieu et d'une direction** (EASTNORTHEAST), d'une **horloge publique** où l'on se rassemble (Weltzeituhr), de **deux voyages dans le temps et l'espace** (Égypte 1986, Berlin 1989), de **transmission** (« délivrer un message », Morse, `WHAT IS YOUR POSITION`).
7. Le site contient une **boussole faussée par un aimant** : le repère « naturel » n'est pas le bon.

**Piste de réflexion qui en découle** *(hypothèse, non testée, sans paramètre fixé)* :
- **Révisée le 23/09 après lecture de Bean.** Le cadre le plus cohérent avec *toutes* les déclarations est : un **chiffre de terrain à mots-clés, en plusieurs étapes, lettre à lettre, dont une étape fabrique une clé non périodique** (d'où l'absence de toute structure arithmétique simple à la même position ; base 2, §1), et où la **« base »** (numérique, ou l'alphabet) a été changée. Le Gromark (amorce de chiffres → clé par addition décalée → substitution à alphabet mélangé) en est l'exemple canonique proposé par Bean et Gillogly. Les lettres surélevées « DYAHR » pourraient désigner une amorce (Bean).
- L'élément manquant serait alors un **ensemble de paramètres mémorisables** : mot(s)-clé(s), base, amorce et règle d'expansion. Il serait indiqué dans l'œuvre (« instructions dans les textes antérieurs »), et non une donnée physique fine.
- La piste « repère physique » (ci-dessous) reste possible mais passe au second plan : Un « nord » déplacé, une origine de lecture, un sens de parcours sur l'objet physique, désignés par l'œuvre elle-même (rose des vents et magnétite, instructions de K2/K3 : « ID BY ROWS », « LAYER TWO », « CAN YOU SEE ANYTHING », « WHAT IS YOUR POSITION »). C'est cohérent avec les points 2, 3 et 7, et avec la conclusion documentaire du dépôt (`current_experimental_frontier.md`). Il manque encore la donnée qui le rendrait testable : la géométrie mesurée de l'écran et du site.

## 9. Questions ouvertes pour enrichir cette base

- Relire l'original de la **lettre ouverte d'août 2025** (page Dunin non récupérée automatiquement).
- Retrouver le **texte exact du discours d'inauguration de 1990** (« deux systèmes »).
- Transcription vérifiée de **K0** (Morse), avec la position physique de chaque plaque et de chaque « E » en surnombre.
- **Direction exacte** de la rose des vents et de la déviation induite par la magnétite (relevé sur site ou photos datées).
- Relevé métrique de l'écran K4 : lignes, retraits, « ? », lettres surélevées « YAR ».
- Différences **Antipodes / Kryptos** lettre à lettre (Antipodes contient K4 ?).
- Interviews vidéo : NOVA 2013 (feuille K3 avec « P/C »), Big Techday 2013, LEMMiNO.
- ~~Article de Bean~~ : intégré le 23/09 (texte fourni par l'utilisateur). À lire ensuite : **l'entretien d'histoire orale de Sanborn aux Archives of American Art (14–16 juillet 2009)**, source primaire longue sur son œuvre : https://www.aaa.si.edu/collections/interviews/oral-history-interview-jim-sanborn-15700 ; la **transcription Scheidt 2020 (A.J. Jacobs)** ; les comptes rendus du **dîner de 2011** (Hannon).

## Sources

- [CIA] https://www.cia.gov/legacy/headquarters/kryptos-sculpture/
- [W-K] Wikipédia, *Kryptos* : https://en.wikipedia.org/wiki/Kryptos
- [W-JS] Wikipédia, *Jim Sanborn* : https://en.wikipedia.org/wiki/Jim_Sanborn
- [AS] Artists Space, *Invisible Forces* : https://artistsspace.org/exhibitions/invisible-forces
- [SA-25b] Scientific American, 12 nov. 2025 : https://www.scientificamerican.com/article/cia-kryptos-puzzle-creator-releases-final-clues/
- [RR-lot] RR Auction, lot 2001 : https://www.rrauction.com/auctions/lot-detail/350761607302001-the-complete-secrets-of-kryptos-jim-sanborns-private-archive/
- [RR-disc] RR Auction, *Kryptos K4: Discovered, Not Solved* : https://content.rrauction.com/kryptos-k4-discovered-not-solved-heres-what-actually-happened/
- [OL25] Lettre ouverte, août 2025 : https://www.elonka.com/kryptos/OpenLetterAug2025.html
- [RK4] Rumkin, K4 : https://rumkin.com/reference/kryptos/k4/
- [RK0] Rumkin, K0 : https://www.rumkin.com/reference/kryptos/k0/
- [KF-m] Kryptos Beyond K4, *matrix codes* : https://kryptosfan.wordpress.com/tag/matrix-codes/
- [Bean21] R. Bean, *Cryptodiagnosis of “Kryptos K4”*, HistoCrypt 2021 (texte fourni par l'utilisateur) ; code : https://github.com/RichardBean/k4testing
- [inteltoday] https://inteltoday.org/2021/08/14/kryptos-the-mystery-of-the-morse-messages/
- Dépôt : `docs/documentary_research_2026_09_21.md`, `docs/k3_chart_layout_and_route_2026_09_19.md`, `docs/nyt_k1k2_chart_physical_layout_2026_09_19.md`
