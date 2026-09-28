# Audit documentaire des cellules ouvertes K4

**Date :** 21 septembre 2026
**Décision globale :** **Aucune candidate admissible identifiée.**
**Calcul K4 :** aucun calcul, déchiffrement, score, extraction ou recherche de plaintext n’a été effectué.

## Conclusion générale

Les sources examinées apportent plusieurs contraintes réelles, mais aucune ne transforme une cellule ouverte en recette complète et falsifiable. Elles établissent que Sanborn a utilisé des feuilles de travail, des grilles et un tableau inversé pour K1–K3. Elles établissent également que K4 est un bloc distinct de 97 caractères et que Scheidt a décrit un procédé de masquage de l’anglais. Elles ne fixent toutefois pas le transfert de ces procédés vers K4.

Le résultat important est donc négatif mais précis : **les cellules restent ouvertes comme familles de mécanismes, mais aucune n’est ouverte à l’exécution**. Toutes les candidates ci-dessous échouent avant tout calcul, soit parce qu’un paramètre déterminant manque, soit parce que la transformation proposée est déjà équivalente à une famille recensée.

## F-02 — Réemploi d’un parcours de style K3

**FAITS INDÉPENDANTS.** La procédure K3 est documentée comme une double transposition de grille : une écriture en 8 × 42, une lecture par colonnes de bas en haut et de gauche à droite, une réécriture en 24 × 14, puis une seconde lecture de même type. Des sources secondaires corroborent cette reconstruction. La CIA documente le tableau et les systèmes matriciels pour les trois premiers textes, mais ne dit pas que K4 reprend la route K3. [1] [2] [3]

**MÉCANISME CONTRAINT.** Aucun mécanisme K4 n’est contraint. Le fait établi est seulement : « K3 utilise cette route ». Il ne devient pas : « K4 réutilise cette route ».

**RECETTE COMPLÈTE CANDIDATE A.** Prendre les 97 caractères K4 dans leur ordre public, ajouter une sentinelle finale, remplir une grille 7 × 14 par rangées, puis lire les colonnes de gauche à droite et de bas en haut. Retirer la sentinelle et produire la permutation de 97 positions.

**PARAMÈTRES FIXÉS.** L’ordre public de K4 et le sens du parcours K3 sont documentés pour leurs domaines respectifs. La sentinelle, la grille 7 × 14, le remplissage, l’origine et le transfert K3→K4 ne le sont pas.

**SORTIE ATTENDUE.** Une suite K4 réordonnée et une permutation. Cette recette ne fournit aucune seconde couche ni aucun plaintext.

**ANTÉRIORITÉ ET ÉQUIVALENCE.** Cette candidate est une instance de F-01/F-02 : une route rectangulaire avec remplissage ajouté. Une rotation, une inversion ou un changement de notation ne crée pas une famille nouvelle. La composition avec une consultation de tableau recoupe également l’architecture K1-style/K3-style déjà recensée.

**DEGRÉS DE LIBERTÉ RESTANTS.** Transfert K3→K4, géométrie, remplissage, sentinelle, nombre de passes, sens d’inversion, seconde couche, tableau, clé, alphabet et convention de sortie.

**VERDICT.** **OPEN CELL — NOT ADMISSIBLE.** Il faudrait une feuille K4, un plan ou une déclaration primaire qui fixe à la fois la géométrie K4, le transfert de la route et la couche complémentaire.

## F-07 — Overlay physique ou tableau recto-verso

**FAITS INDÉPENDANTS.** La CIA décrit un écran dont une partie porte le texte chiffré et l’autre un tableau inversé, lisible depuis l’arrière. Une source de presse indique que le tableau utilisé dans les feuilles de travail pouvait différer légèrement de celui de l’écran. Les archives publiques mentionnent une note interprétée comme « Extra L … Bottom chart seeding », mais cette lecture reste ambiguë et ne constitue pas une instruction K4 authentifiée. La construction SolveKryptos est un antécédent revendiqué, mais sa page de vérification reconnaît des éléments ajustés à partir d’un plaintext proposé. [1] [4] [5] [6]

**MÉCANISME CONTRAINT.** Les sources contraignent l’existence d’un dispositif physique recto-verso et d’un tableau inversé. Elles ne contraignent ni une carte de correspondance entre les faces, ni une projection, ni la fonction appliquée à une paire de lettres.

**RECETTE COMPLÈTE CANDIDATE A.** Relever les deux faces à l’échelle, les déplier dans un repère commun, apparier les glyphes selon leur projection normale et produire 97 paires ordonnées ou cases vides.

**RECETTE COMPLÈTE CANDIDATE B.** Prendre la ligne contenant l’extra-L, conserver l’appariement avant le L et décaler l’indice du tableau après le L. Produire un masque binaire de 97 positions, sans appliquer de déchiffrement.

**PARAMÈTRES FIXÉS.** Aucun relevé métrique public ne fixe le repère, l’axe de retournement, la projection, la tolérance, l’ancrage de K4, la position de l’extra-L ou son effet.

**SORTIE ATTENDUE.** Candidate A : une liste de paires physiques. Candidate B : un masque binaire. Dans les deux cas, aucune opération de conversion vers plaintext n’est documentée.

**ANTÉRIORITÉ ET ÉQUIVALENCE.** A relève directement de F-07/F-06. B recoupe le noyau extra-L de l’audit 002 et, si le décalage est une simple translation, une sous-classe de F-01. Le changement de repère ou l’inversion du tableau n’est pas une nouveauté transformationnelle.

**DEGRÉS DE LIBERTÉ RESTANTS.** Relevé, repère, miroir, projection, tolérance, frontière K4, position de l’extra-L, propagation du décalage, gestion des collisions, rôle de la paire, alphabet, opération finale et ordre des couches.

**VERDICT.** **OPEN CELL — NOT ADMISSIBLE.** Une photographie ou un plan coté pourrait fermer cette cellule seulement s’il fournit aussi la règle de correspondance et la fonction appliquée aux lettres appariées.

## F-08 — Opérations procédurales sur chartes

**FAITS INDÉPENDANTS.** Les chartes K1–K2 montrent des lignes, des marges, des débordements et des supports de travail. K3 possède une route de grille documentée et des marques de tenue de route. Ces éléments concernent les procédures de travail connues pour K1–K3. Aucune source ouverte ne les relie à un geste de pliage, découpe, marge ou marqueur appliqué à K4. [1] [2] [3]

**MÉCANISME CONTRAINT.** Aucun geste matériel K4 n’est fixé. Un terme comme « bottom chart seeding » décrit au mieux un indice lexical ambigu ; il ne fixe ni un objet, ni une opération, ni une sortie.

**RECETTE COMPLÈTE CANDIDATE A.** Représenter K4 en bandes de 31 caractères, retourner les bandes paires et produire la permutation induite.

**RECETTE COMPLÈTE CANDIDATE B.** Ajouter une sentinelle, remplir une grille 7 × 14, lire les colonnes de bas en haut et retirer la sentinelle.

**PARAMÈTRES FIXÉS.** La longueur K4 est connue. La largeur 31, les bandes, la sentinelle, la grille, l’origine et le transfert des marques K3 ne le sont pas.

**SORTIE ATTENDUE.** Une permutation ou une suite de 97 caractères réordonnés, sans plaintext.

**ANTÉRIORITÉ ET ÉQUIVALENCE.** A est une route serpentine de F-01. B est une route de F-01/F-02. Décrire une permutation comme un pli, une coupe ou un retour de bande ne crée pas une transformation nouvelle.

**DEGRÉS DE LIBERTÉ RESTANTS.** Support K4, largeur, bandes, marges, origine, orientation, geste exact, rôle des marques, carte de positions, seconde couche, alphabet et convention de sortie.

**VERDICT.** **OPEN CELL — NOT ADMISSIBLE.** Il faudrait une charte K4 authentifiée qui relie explicitement un geste à K4 et indique comment son résultat devient une entrée cryptographique déterminée.

## F-10 — Masques Morse extra-E

**FAITS INDÉPENDANTS.** La CIA confirme l’existence de plaques Morse à l’entrée et d’un écran K4 distinct. Scheidt parle d’un procédé qui « masque » l’anglais. Les transcriptions publiques des E Morse comportent toutefois des ambiguïtés de lecture et de segmentation. Le Smithsonian conserve les papiers de Sanborn, mais la notice ouverte ne publie aucune table de correspondance Morse→K4. Les scripts historiques F-10 déclarent `Last run: never` et aucun résultat correspondant n’a été retrouvé. [1] [7] [8]

**MÉCANISME CONTRAINT.** Les sources contraignent seulement une possibilité générale de masquage. Elles ne fixent pas l’ensemble des E, leur ordre, leur projection vers K4, leur sémantique ou le second système.

**RECETTE COMPLÈTE CANDIDATE A.** Utiliser un relevé coté des plaques Morse et du panneau K4, projeter chaque E selon une transformation géométrique définie et supprimer les positions touchées.

**RECETTE COMPLÈTE CANDIDATE B.** Construire un ruban binaire E/non-E, le consommer une fois après une route K3 transférée à K4 et retirer les cellules marquées.

**PARAMÈTRES FIXÉS.** Aucun paramètre opératoire n’est fixé indépendamment. Même le cardinal exact des E dépend de la transcription et du statut de `DIGETAL`.

**SORTIE ATTENDUE.** Un masque positionnel et une sous-séquence K4. Aucun système final n’est spécifié.

**ANTÉRIORITÉ ET ÉQUIVALENCE.** A recoupe F-10/F-07 et les recettes P-C1-1/CP-5. B recoupe F-10/F-02/F-01 et les scripts historiques non exécutés. L’absence de résultat archivé interdit une conclusion négative, mais ne rend pas ces variantes nouvelles.

**DEGRÉS DE LIBERTÉ RESTANTS.** Transcription, ensemble des E, orientation, projection, repère, échelle, collisions, polarité, domaine cible, grille K4, route, consommation du ruban, sémantique keep/drop, second système et convention de sortie.

**VERDICT.** **OPEN CELL — NOT ADMISSIBLE.** Le prochain apport utile serait un document primaire qui relie explicitement les marques Morse au panneau K4 et prescrit leur opération.

## PHYSICAL-PROCEDURAL — mécanismes qui brisent H1

**FAITS INDÉPENDANTS.** La structure physique de Kryptos rend possible une opération qui réordonne ou sélectionne les caractères avant le chiffrement analysé. Les deux faces, le tableau inversé, les plaques Morse et la note ambiguë sur l’extra-L sont des faits ou des observations documentaires. Ils ne fournissent pas de repère commun, de projection, de carte de positions ou d’opération.

**MÉCANISME CONTRAINT.** Aucun mécanisme physique général n’est contraint. « Briser H1 » décrit une conséquence architecturale possible, pas une recette.

**RECETTE COMPLÈTE CANDIDATE A.** Relever les deux faces, établir une transformation géométrique unique, appliquer la carte de correspondance et émettre une séquence déterminée.

**RECETTE COMPLÈTE CANDIDATE B.** Relever les E Morse et K4, établir une projection unique, appliquer une seule règle de sélection et émettre une séquence déterminée.

**PARAMÈTRES FIXÉS.** Aucun repère, métrique, projection, fonction de paire, règle de sélection ou sortie n’est fixé.

**SORTIE ATTENDUE.** Une séquence ou un masque dont la longueur, l’ordre, l’alphabet et le statut CT intermédiaire/PT seraient fixés avant calcul.

**ANTÉRIORITÉ ET ÉQUIVALENCE.** A relève de F-07/F-08 ; B relève de F-10/F-08. Une translation devient F-01, une route K3 devient F-02 et une projection Morse devient F-10. Une nouvelle cellule exigerait une carte physique et une opération inédites, toutes deux documentées.

**DEGRÉS DE LIBERTÉ RESTANTS.** Tous les éléments matériels et cryptographiques déterminants : repère, orientation, échelle, ancrage, parcours, appariement, sélection, alphabet, couche finale et sortie.

**VERDICT.** **OPEN CELL — NOT ADMISSIBLE.** La cellule reste importante parce qu’un mécanisme physique pourrait sortir du périmètre H1. Elle ne fournit cependant aucune candidate exécutable tant qu’un plan coté, une instruction K4 ou une fiche de procédé n’a pas fixé toutes les étapes.

## Meilleures contraintes documentaires trouvées

Les informations les plus fortes sont les suivantes :

1. **K3 possède une route réellement documentée**, mais cette route est une propriété de K3 et ne peut pas être transférée à K4 par analogie.
2. **Le tableau est physiquement inversé et lisible depuis l’arrière**, mais aucune source ne donne une correspondance exploitable entre les faces.
3. **Les chartes K1–K3 sont des supports de travail documentés**, mais aucune ne constitue une charte K4 publiée.
4. **Scheidt parle d’un masquage de l’anglais**, ce qui soutient l’existence possible d’une couche de sélection ou de brouillage, sans en préciser la mécanique.
5. **Les E Morse et l’extra-L restent des observations documentaires**, non des instructions. La provenance et la lecture de certains documents doivent encore être renforcées avant de pouvoir déterminer une opération.
6. **Un mécanisme physique pourrait briser H1**, mais cette propriété ouvre une architecture, pas une expérience.

## Conclusion arrêtée

> **Aucune nouvelle candidate admissible identifiée.**

Aucune des cinq cellules ne fournit actuellement une combinaison nouvelle de représentation, alignement, parcours, paramètres, opération et sortie. Toute exécution demanderait encore de choisir au moins un paramètre après interprétation de l’indice, ce qui est interdit par le protocole.

Les seules acquisitions documentaires susceptibles de fermer prochainement une cellule sont :

- une feuille K4 de Sanborn avec géométrie et parcours explicites ;
- un plan coté mettant les deux faces dans un repère commun ;
- une table de correspondance entre les plaques Morse et les caractères K4 ;
- une instruction primaire reliant l’extra-L ou `bottom chart` à K4 et définissant son opération ;
- une fiche de procédé indiquant la seconde couche, son alphabet, son ordre et sa convention de sortie.

En l’absence de l’un de ces éléments, la recherche doit rester documentaire. Aucun calcul K4 n’est autorisé sur la base des candidates recensées ici.

## References

[1]: https://www.cia.gov/legacy/headquarters/kryptos-sculpture/ "CIA — About the Kryptos Sculpture"
[2]: https://rumkin.com/reference/kryptos/k3/ "Rumkin — K3: Two Transposition"
[3]: https://kryptosfan.wordpress.com/k3/k3-solution-3/ "Kryptos Beyond K4 — K3 Solution #3"
[4]: https://www.nytimes.com/2010/11/21/us/21codecharts.html "The New York Times — Original Decoding Charts for Kryptos"
[5]: https://solvekryptos.com/verify "SolveKryptos — Verification page"
[6]: https://www.kryptosbot.com/archive/ "KryptosBot — Sanborn Papers Archive Research Collection"
[7]: https://www.wired.com/2005/01/inside-info-on-kryptos-codes/ "Wired — Inside Info on Kryptos Codes"
[8]: https://www.si.edu/object/archives/sova-aaa-sanbojim "Smithsonian — Jim Sanborn Papers finding aid"
