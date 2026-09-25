# Kryptos K4 Research Handoff

Ce dépôt contient le handoff documentaire pour la recherche sur **Kryptos K4**, le bloc final de 97 caractères de la sculpture de Jim Sanborn.

Le dépôt source complet est [`jcolinpatrick/kryptos`](https://github.com/jcolinpatrick/kryptos). Ce dépôt séparé rassemble uniquement les documents nécessaires pour reprendre le travail : protocole, état de la frontière, audits, registre des cellules ouvertes, provenance documentaire et prompt de démarrage.

## Statut au 21 septembre 2026

> **Le blocage est documentaire, pas expérimental.**

Les cellules F-02, F-07, F-08, F-10 et les mécanismes physiques/procéduraux qui pourraient briser l’alignement direct sont sous-déterminés avec les informations publiques actuellement accessibles. Aucune nouvelle transformation K4 ni aucun calcul n’a été lancé pendant la phase documentaire finale.

Le document principal est [`HANDOFF_NEXT_AGENT.md`](HANDOFF_NEXT_AGENT.md). Il contient le contexte, les règles de preuve, les sources utiles, les corrections du registre, les outils et le prompt complet à donner au prochain agent.

## Mise à jour du 24 septembre 2026

- **Synthèse (à lire en premier)** : `docs/knowledge_base/08_synthese_2026_09_24.md`, qui recoupe l'ensemble des bases.

- Documents versés par l'utilisateur (NSA 1991–1992 et 2014, Scheidt et Sanborn 1991, NOVA 2006, réunion Scheidt 2015…) : `sources/docs_utilisateur_2026_09_24/`, lus et résumés dans `docs/knowledge_base/06_documents_2026_09_24.md`.
- Audit « vision » (liens entre les pistes, tests exacts nouveaux en C, statistiques revues) : `audits/vision_2026_09_24/README.md`. Aucune solution ; plusieurs familles nouvelles éliminées ; la note IMG_1340 est résolue (« ? » non codés).
- Fichiers du groupe kryptos.groups.io (accès membre) : inventaire complet, lectures et tri des dossiers personnels dans `docs/knowledge_base/07_groupsio_2026_09_24.md` (copie intégrale dans `sources/groupsio_membres/` depuis le passage du dépôt en privé, le 24/09 ; **ne pas republier**). Test T7 (textes « enfouis » : directive Truman de la pierre angulaire, cahier des charges CIA 1988, lettre de Sanborn 1989) : négatif. Archive des messages 2003–2018 lue (témoignages directs de rencontres avec Sanborn et Scheidt ; aucune donnée personnelle reprise) : 5 des 6 lettres doublées de K4 sont en position ≡ 4 (mod 7), p ≈ 0,02 après correction ; T8 (« chaîne de masquage » = 1/563) : négatif.
- **Le « 7 » de K4 n'est très probablement pas un hasard** (balayage systématique des dossiers personnels, base 7 §8). Deux anomalies vues séparément, l'écart 7 (NSA 1992) et les doublets mod 7 (connus du groupe depuis 2002–2003 : Stehle, Gillogly ; chiffrés par Matson en 2009), sont indépendantes. Ensemble, avec le module laissé libre : **p ≈ 2 × 10⁻⁴** sur un million de mélanges (`audits/vision_2026_09_24/stats_7b.py`). La largeur 21 n'ajoute pas de preuve indépendante : 3 de ses 11 paires viennent des deux autres signaux (correctif du premier chiffre, 10⁻⁵). Ce n'est ni une substitution périodique (paire 65/72 : P → R puis C), ni une autoclé simple, et aucune famille simulée ne reproduit la concentration des doublets (`sim_7.py`). T10 (maquette GIRASOL de 1988 comme source de clé) : négatif. Trois familles à structure de 7 sans période, testées exactement : disque tourné par bloc de 7 (T11), autoclé mixte (T12), Hill par blocs à petits coefficients (T13), doublets vus comme glissements de copie (T14), Wheatstone à cadran à mot-clé (T15, 704 880 cadrans), disque par bloc de 2 à 30 (T11b), clé transposée à la manière de K3 (T16, 846 720 cas), autoclé « vers l'avant » (T17) : négatives. Contrôle : K1, K2 et K3 n'ont aucun « 7 » ; il est propre au procédé de K4. **L'hypothèse de la NSA de 1992 (autoclé sur le clair à l'intervalle 7) est éliminée pour tout alphabet, s'il n'y a aucune erreur de chiffrement** (T18) : la relation se propage de 7 en 7 à travers le clair inconnu et relie les deux cribs, ce que les tests antérieurs n'exploitaient pas. Une seule lettre mal chiffrée suffit à la rouvrir (T19). Nouvelle observation : en Beaufort A–Z, la clé tirée des cribs se répète localement (p ≈ 0,004 corrigé, `stats_clef.py`).
- **Second balayage des dossiers personnels** (base 7 §9) : rien de caché qui résoudrait K4, mais une pièce oubliée utile. Un petit chiffre de Sanborn, vu dans son atelier en 2005, a **97 lettres**, la méthode de K1 (clé SHADOW), un clair coupé au milieu d'un mot, et **4 erreurs de chiffrement**, dont 3 du même type (glissement de clé). Avec ce taux, les 24 lettres des cribs de K4 n'auraient que 31 % de chances d'être toutes justes. **Rejouées avec une seule erreur (T19–T21), la plupart de nos éliminations tombent au niveau du hasard** (T11, T11b, T13, T18). Seuls le Wheatstone à mot-clé et la clé périodique courte tiennent ; T16 reste sous le hasard. Les deux lettres chiffrées en elles-mêmes, 32 (S → S) et 73 (K → K), portent une grande part de ces éliminations. Aussi : le fragment de Zola (transposition en 16 colonnes, lettres doublées de remplissage) et la « propriété U » de Caveney (p ≈ 2 × 10⁻⁴, après coup, non expliquée).
- **Troisième balayage : la compilation des entretiens** (base 7 §10), 375 pages de 1989 à 2020, conservées dans un dossier personnel. Pas de solution cachée, mais des paroles primaires jamais versées et une correction. **Correction** : « personne n'a retrouvé la matrice d'origine… ni ne l'a fait passer par tous les *shifts* » est de **Sanborn**, celui qui a chiffré K4, et non de Scheidt. **NPR 1999** : les systèmes « **spatiaux** », à motifs, de lumière et d'ombre, ne sont pas dans K1–K3, mais « peut-être » dans K4. **Big Techday 2013** : en montrant ses feuilles, « à peu près la façon dont je l'ai fait » : on **retourne la feuille, la met à l'envers**, on l'éclaire. **Erreurs** : K4 n'a jamais été redéchiffré ni testé par personne (2003). Pour justifier ses lignes au découpage, Sanborn **marquait des lettres omissibles** (2006). D'où un second modèle d'erreur, la lettre retirée, qui décale les rangs suivants. Son protocole est écrit (base 7 §10.3), mais il n'est pas exécuté. Aussi : « cinq ou six » techniques en tout (2005) ; une seconde remise partielle du code à la CIA vers 1991. **Compléments (§10.5)** : Scheidt n'a « pas relu le code lettre à lettre » (2006). Il parle lui-même d'**insérer ou retirer** une lettre (chiffres de contrainte) et dit que le retrait de l'X de K2 était « la décision de Jim ». Son masque est « conçu pour retirer le biais », alors que la NSA trouve dans K4 un biais à l'intervalle 7. La longueur de 97 est confirmée par Sanborn (NPR 2005) ; le « 98 » de Scheidt compte très probablement le « ? ». Les essais de « lettre manquante » des membres (2007–2015) sont tous antérieurs aux cribs complets. **Relecture des 175 affirmations chiffrées de l'archive (§10.6)** : une seule piste n'était pas couverte, la transposition de bits de J. DeSeve (2007). Elle pouvait expliquer des doublets à rang fixe, et elle est absente des familles simulées. Un contrôle à la main par les cribs la ferme presque entièrement ; il reste à le confirmer par le test.
- **Messages 2019–2026 et fichiers de résultats des membres** (base 7 §11). Les 5 900 messages postérieurs à 2018 ont été lus par l'API, avec le compte de l'utilisateur.
  - **Fichiers de résultats** : 48 803 déchiffrements de membres confrontés aux cribs, dont les 4 650 « à analyser » d'Albostoni (2012). Aucun n'approchait les cribs avant leur publication.
  - **Faits nouveaux** :
    - Paradigm, l'acheteur, a fait contrôler par programme le clair saisi par Sanborn : 97 lettres, cribs aux rangs 21 et 63. Il détient une description écrite de la méthode.
    - Sanborn, 2025 : « Scheidt a conçu la méthode, je l'ai changée » ; « It is a solid system, and I fucked with it » ; plusieurs versions du texte, de longueurs différentes, pour que tout tienne sur le cuivre.
    - K5 a BERLINCLOCK au même rang que K4. Premier test immédiat quand K5 sera publié.
    - Dépôt au Copyright Office (2010) : Q et U absents du clair de Sanborn, DESPARATLY présent.
    - Le panneau du bas fait exactement 14 × 31 = 434 caractères.
  - **Conséquence** : le modèle « lettre retirée à la gravure » passe en priorité basse.
- **Fils de recherche, album de la vente et site de l'acheteur** (base 7 §11.6).
  - **Correction** : Scheidt n'a pas dit en 2015 que le masque venait en premier. La « première étape » est celle de l'analyste (compter les lettres) ; l'ordre du masque n'est pas dit. Corrigé dans les bases 1, 6, 7 et 8.
  - **Brouillons de K3** (photo de la vente) : « 11 Lines | 342 » et « 3 lines 93 », le plan exact de la plaque du bas. Sanborn coupe « slowly » (6 lettres) pour que K3, « ? » et K4 tiennent en 14 × 31 ; K4 = 4 + 93 = 97. Il règle donc les longueurs en coupant des mots du clair.
  - **Feuille K3 + K4 de NOVA** (31 × 14) : 8 signes raturés sous VTTMZFPK, sous correcteur sur les photos de Paradigm. Illisibles ; à demander à Paradigm.
  - Aussi : EAST « not inadvertent » (Sanborn, 2020) ; un « big hint » donné à la CIA vers 1997–1998, jamais publié (Gillogly 1999) ; les seuils d'IoC du hasard de Matson (2022), à utiliser comme témoins.

- **Nombre minimal d'erreurs et nouveaux liens (24/09, soir)** : `audits/erreurs_multiples_2026_09_24/README.md`, base 7 §12.
  - Au-delà d'une erreur : pour chaque famille, combien de lettres des cribs faudrait-il déclarer fausses, comparé au hasard ? Clé périodique (p ≤ 26, Quagmire I–IV) et autoclé sur le chiffré (seconde hypothèse NSA) restent éliminées **quel que soit le nombre plausible d'erreurs**. Période 7 avec décalage ou retournement par ligne : aucun signal.
  - **Empreinte des doublets** : dans les chiffres périodiques de Sanborn (petit fragment, K1, K2), les doublets suivent les phases de la clé. Celle de K4 désigne une différence de clé fixe entre les colonnes 4 et 5 de chaque ligne de 7, mais un alphabet réaliste ne suffit pas à l'expliquer.
  - **Les 8 signes raturés de la feuille NOVA (2006) sont exactement sous LINCLOCK** (66–73). Si ce sont des lettres de clé, elles livrent 8 lettres de clé de K4 : demande prioritaire à Paradigm, table de reconnaissance prête.
  - Attaque par recuit sur le texte entier (`audits/recuit_2026_09_24/`) : l'outil ne passe pas encore son contrôle positif ; rien n'est conclu sur K4.

- **Alphabets à mot-clé (24/09, nuit)** : `audits/motcle_pas7_2026_09_24/README.md`. On essaie tous les alphabets à mot-clé (237 988, tirés de 59 497 mots), comme ceux qu'employait Sanborn. Pour chacun, la clé est connue en 24 points ; quand elle est déterminée, on déchiffre les 97 lettres. Éliminées, K4 au niveau du hasard et contrôles positifs retrouvés :
  - clés à pas 7 (décalage par ligne du cuivre ou par ligne de 7) ;
  - autoclé sur le clair (écarts 1–13, dont l'hypothèse NSA) et sur le chiffré (tous écarts) ;
  - **clé courante tirée d'un texte anglais inconnu**, famille jusqu'ici réputée irréfutable : les deux fragments de clé devraient être de l'anglais ; ils ne le sont jamais (puissance 99,5 %).

- **Simulateur Sanborn (25/09, fin)** : `audits/simulateur_2026_09_25/README.md`, base 7 §12.10. On ne teste plus une famille contre les cribs. On demande quel procédé manuel fabrique un texte qui ressemble à K4 sur ses 97 lettres : 52 procédés, un million de faux K4 chacun.
- **Moteur unique C/OpenMP (25/09)** : `audits/moteur_2026_09_25/README.md`. Six chantiers T0–T6 : 1,4 à 3 millions d'alphabets à mot-clé, 30 témoins, contrôles positifs.
  - Aucune famille ne place K4 hors des témoins : période et variantes à 7, erreurs de Sanborn, transposition + substitution (NSA 1992), Quagmire IV, ligne + colonne, deux mots superposés.
  - L'attaque sur le texte entier passe désormais son contrôle ; sur K4, charabia.
  - Relais DeepSeek n° 3–4 vérifiés : `audits/relais_2026_09_25/README.md`, 5ᵉ–7ᵉ parties. Les 8 signes étaient déjà masqués en 2006 ; LFSR et « Fibonacci au pas 7 » sont éliminés.
  - Le « 7 » ne relie que des **voisins** : l'écart 7 est en excès, les écarts 14 à 49 sont au hasard. Ce n'est donc pas une période, quelle que soit la clé.
  - Un seul procédé manuel produit ce profil : l'autoclé sur le clair à l'écart 7, **forme Vigenère** (Beaufort et Variante ne le produisent pas). C'est l'hypothèse de la NSA, précisée.
  - T34 la teste avec deux alphabets à mot-clé indépendants (Quagmire IV, 5,3 × 10¹¹ paires, recherche exacte, clair entier lu) : K4 au moins 7 équations fausses sur 17, témoins 5 à 7. **Éliminée**, même avec le taux d'erreur de Sanborn.
  - Aucun procédé positionnel ne concentre les doublets. Le « 7 » reste inexpliqué, et un hasard n'est pas exclu (10⁻³ à 10⁻² après correction).

- **Recherche de procédés et borne des doublets (25/09, soir)** : `audits/procedes_2026_09_25/README.md`, base 7 §12.11.
  - Borne : dans un chiffre déchiffrable, un doublet revient à « prédire » la lettre claire suivante. **Aucune clé indépendante du texte**, quelle qu'elle soit, ne rend probables les doublets alignés de K4 : 0 avec A–Z ou KRYPTOS et les cribs, 6 % au mieux avec des alphabets libres.
  - Recherche automatique parmi environ 20 000 procédés manuels déchiffrables, contrôles positifs réussis : aucun ne produit la concentration. Les meilleurs ne font que forcer SS en 32 à partir du crib.
  - Les doublets alignés sont donc un hasard, une intervention manuelle ou un élément qui n'est pas lettre à lettre, et non l'empreinte d'une clé.

## Principes de travail

- Distinguer fait source, interprétation, hypothèse, recette, expérience et preuve.
- Auditer l’antériorité avant de présenter une piste comme nouvelle.
- Déterminer l’équivalence par la transformation, avant de regarder le résultat.
- Conserver les négatifs et ne jamais confondre `INCOMPLETE RECORD` avec une élimination.
- Ne pas utiliser une sortie anglaise ou un score pour choisir rétroactivement les paramètres.
- Une recette incomplète reste `OPEN CELL — NOT ADMISSIBLE`.

## Contenu

- `HANDOFF_NEXT_AGENT.md` — document de reprise principal et prompt de démarrage.
- `docs/` — protocole, frontière expérimentale, registres, contexte K1–K4 et documentation de recherche.
- `audits/` — audits de fiabilité, authenticité, cohérence et cartographie des cellules ouvertes.
- `MEMORY.md` — mémoire opérationnelle utile à la reprise.
- `docs/knowledge_base/` — **bases documentaires : tout le savoir public sur Sanborn, et tout ce qui a déjà été testé (à lire avant tout calcul)**.
- `docs/k4_mechanism_reasoning_2026_09_22.md` — raisonnement « mécanisme plutôt que clé » et éliminations algébriques (SAT, alphabets inconnus) ; code dans `audits/algebraic_elimination_2026_09_22/`.
- `exhaustion_log.json` — copie documentaire du journal d’antériorité, avec les chemins locaux neutralisés.

Les bases SQLite, logs lourds, clés et configurations locales ne sont pas inclus. **Le dépôt est privé depuis le 24/09/2026** : il contient la copie intégrale des fichiers réservés aux membres de kryptos.groups.io (`sources/groupsio_membres/`). S'il redevient public, ce dossier devra d'abord être retiré de tout l'historique git.

## Reprendre le travail

```bash
git clone https://github.com/aciderix/kryptos-research-handoff.git
cd kryptos-research-handoff
sed -n '1,260p' HANDOFF_NEXT_AGENT.md
```

Pour retrouver le code et les scripts complets :

```bash
git clone https://github.com/jcolinpatrick/kryptos.git
```

## Avertissement méthodologique

Ce dépôt ne présente aucune solution de K4. Il documente ce qui est établi, ce qui est seulement revendiqué, ce qui reste ouvert et l’information minimale qui permettrait éventuellement de rendre une expérience déterminée.
