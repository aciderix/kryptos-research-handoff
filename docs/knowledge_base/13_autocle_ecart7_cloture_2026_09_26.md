# 13 — Le dernier coin du « 7 » fermé par le calcul : autoclé écart-7 à alphabet(s) libre(s), et pourquoi les 97 lettres ne suffisent pas (26/09/2026)

**Statut :** aucune solution, aucun clair. Ce document règle le sort du **seul coin non fermé** du procédé désigné par
le profil du « 7 » (base 09 §6 pt 1, §9 ; base 10 §I, ligne « Autoclé Vigenère à l'écart 7 avec deux alphabets libres »).
Il complète l'audit `audits/autocle_recuit_2026_09_26/` (code, contrôles, mesures) et corrige le point ouvert laissé par
`audits/recuit_2026_09_24/`.

## 1. Le coin visé

Le simulateur (base 10 F, `simulateur_2026_09_25`) désigne **un seul** procédé manuel qui rend naturel l'excès à
l'écart 7 : l'**autoclé sur le clair à l'écart 7, forme Vigenère**. Il est éliminé avec les alphabets **à mot-clé**
(T27, T34) et, pour un alphabet commun quelconque, **sans erreur** (T18, preuve algébrique). Restait ouvert, faute de
pouvoir juger sur les seules 24 lettres des cribs (T19, T36b) :
- (a) un **alphabet libre** (non construit sur un mot-clé) avec **≥ 1 erreur** de chiffrement ;
- (b) **deux alphabets libres** indépendants (clair σ, chiffré τ ; T36b), même sans erreur.

La seule façon de trancher est une attaque sur **les 97 lettres** (noter le clair entier), à condition qu'elle **passe
un contrôle positif** : retrouver un faux K4 fabriqué avec le procédé. C'est ce prérequis que `recuit_2026_09_24`
n'avait jamais franchi.

## 2. Ce qui a été fait (portée écrite d'avance)

Moteur dédié à CE procédé (round-trip validé ; `κ` déterminé par les cribs ; σ ne participe pas à la récurrence, donc
la recherche porte sur (τ, κ), σ étant votée sur les cribs). Contrôles positifs = anglais (Gutenberg) + cribs insérés
+ (σ, τ, κ) aléatoires. Témoins = « K4 mélangé ». Quatre familles d'attaque, chacune avec sa cible (qoff anglais
≈ −3,9/−4,2 ; charabia ≈ −7 à −8) :

1. recuit conjoint (σ, τ, κ) à froid ;
2. recuit sur (τ, κ), σ votée (vote-fit) ;
3. **ancrage-cribs exact** par CP-SAT (17 équations de chaîne) + recuit sur les lettres libres ;
4. recuit **ancré dans la variété** (germe CP-SAT à 24/24 cribs, forte pénalité de crib).

## 3. Résultat

- **Identifiabilité acquise.** La vraie solution d'un faux K4 est un **optimum fort** : en partant d'elle, la recherche
  y reste (96–97/97 lettres ; qoff −3,9/−4,2). Son score hors-cribs est **nettement séparé** des solutions
  crib-consistantes aléatoires (−7 à −8). **Les 97 lettres discriminent** la vérité — ce que les 24 cribs ne font pas.
- **Le contrôle positif ne passe pas.** Aucune des quatre attaques ne retrouve le faux clair (au mieux 24/24 cribs mais
  qoff −7,x, 26–34/97). L'ancrage-cribs atteint 24/24 cribs mais reste charabia hors-cribs ; le recuit ancré-variété
  aussi.
- **Cause structurelle mesurée.** L'ensemble des alphabets compatibles avec les 24 cribs n'est pas fini : c'est une
  **variété** de dimension **10** (un alphabet) ou **18** (deux alphabets) — noyau des 17 équations de chaîne sur Z2 et
  Z13 —, soit ≈ 10¹² à 10²⁵ solutions ; CP-SAT n'en énumère que ~10–40/s, sans fin. La vraie solution y est une aiguille
  isolée : discriminable, mais introuvable par recherche (le gradient des quadrigrammes ne guide pas depuis un point
  charabia de la variété vers l'anglais).

## 4. Pourquoi T5 réussit et pas ici

`audits/moteur_2026_09_25/` **T5** franchit son contrôle (attaque sur le texte entier depuis les clés crib-exactes) —
mais pour **un seul** alphabet sous une clé **positionnelle** (période 7 + décalage par ligne) : les clés crib-exactes
y sont en nombre **fini**, énumérables, puis recuites. Dans l'autoclé, **la clé est le clair** (auto-référence) : les
cribs ne contraignent l'alphabet que **partiellement** et laissent une variété. Le levier d'énumération finie de T5
disparaît dès que l'alphabet est **libre** *et* la clé **auto-référente**. C'est aussi ce qui sépare ce coin des familles
éliminées avec des alphabets **à mot-clé** (T27, T34), dont l'espace est petit et énumérable.

## 5. Verdict

- **Le dernier coin ouvert du procédé désigné par le « 7 » est fermé au calcul, par le négatif :** sur les seules 97
  lettres connues, l'autoclé écart-7 à alphabet(s) libre(s) — (a) un alphabet + ≥ 1 erreur, (b) deux alphabets libres —
  **ne peut pas être testée**, faute d'attaque qui passe son contrôle positif ; et l'on sait maintenant **pourquoi** :
  la variété crib-consistante induite par l'alphabet libre auto-référent est de dimension 10 à 18, hors de portée de
  toute recherche essayée.
- **Ce n'est pas une indécidabilité au sens de l'information** (les 97 lettres contiennent la réponse), mais une
  **intractabilité de recherche**. Le coin est réellement bloqué sur une **donnée nouvelle** : le chiffré de **K5**
  (même rang pour BERLINCLOCK) ou un nouveau clair connu. Cohérent avec T18 / T19 / T36b.
- **Aucun essai sur K4 n'a été présenté comme concluant** (règle d'or : contrôle d'abord).

## 5 bis. Ce que K5 achète (le levier, cadrage vers l'avant)

Cette clôture **n'est pas un cul-de-sac** : elle énonce exactement pourquoi **K5 débloquerait le coin**, et donc à quoi
sert `audits/k5_depth/`.

- K5 est un **second chiffré**, 97 caractères, « codage semblable » (Sanborn, 12/11/2025), avec **BERLINCLOCK au même
  rang** (63–73). Si le même système opère avec **le(s) même(s) alphabet(s)** σ (et τ), alors les cribs de K5 fournissent
  **de nouvelles équations de chaîne sur les mêmes inconnues**.
- Ces équations **réduisent la dimension de la variété** crib-consistante. Le coin bloque aujourd'hui parce que cette
  variété est de dimension 10 à 18 (§3) ; chaque contrainte indépendante de K5 en retranche une part. Une variété
  suffisamment réduite redevient **finie / énumérable** — le régime où l'énumération à la T5 fonctionne.
- Et comme l'objectif (score anglais sur les 97 lettres) **discrimine déjà la vérité** (§2 pt 1), une variété réduite
  suffit : dès que l'espace crib-consistant est petit, on énumère et on **localise** l'aiguille. Autrement dit, le seul
  ingrédient manquant est **de la contrainte**, pas un meilleur objectif — et c'est précisément ce que K5 apporte.
- **Réserve** : ce gain suppose que K4 et K5 **partagent** le(s) alphabet(s) et le procédé (plausible d'après « codage
  semblable » ; à vérifier dès publication). Premier test immédiat, sans paramètre libre : les lettres de K5 en 63–73
  (base 09 §9). Puis l'attaque en profondeur `audits/k5_depth/` (K4 et K5 en même temps).
- **Mesure (26/09, round conjoint chef+MECA)** : ajouter un second jeu de cribs K5 (mêmes σ,τ, autoclé écart-7,
  BERLINCLOCK au même rang) fait tomber le noyau σ,τ de **19 à ~5** (min 3, max 8 ; 40 K5 simulés) — régime **solvable**
  (26⁵ élagué + l'objectif 97-lettres discrimine). À comparer : fixer σ seul (pochoir à deux alphabets) → 8 ; fixer σ
  ET τ (pochoir unique) → ~0. Donc **K5 réduit autant ou plus qu'un demi-pochoir**. Classement par
  (réduction × obtenabilité) : **K5** (fort + peut-être publié) > géométrie de l'écran (obtenable maintenant mais
  spéculative) > dossier scellé « Stencil 1988 » (réduction maximale mais hors d'atteinte). Lecture : « comprendre » la
  pièce = la clé est un **pochoir** (un objet, pas un mot) ; **débloquer** = **K5**. Script : `audits/autocle_recuit_2026_09_26/k5_vs_stencil.py`.
- **Précision « 7 = |KRYPTOS| »** (cf. §5) : cela vaut pour le **décalage / période** de l'autoclé (l'écart 7), **pas**
  pour le **contenu** de l'amorce de 7 lettres. Celle-ci est très probablement la **queue du clair précédent** (K4 est une
  tranche, coupée au milieu d'un mot) — ce que confirme l'échec mesuré de « amorce = KRYPTOS » (n'ôte que 5 dimensions,
  21→16). Le mot gravé KRYPTOS donne la **longueur** du décalage, pas les lettres de l'amorce.

## 6. Portée, limites, antériorité

- **Négatif de capacité, pas preuve d'impossibilité.** Une recherche plus fine (paramétrage explicite du noyau de la
  variété, solveur dédié à l'autoclé, calcul bien supérieur) pourrait un jour franchir le contrôle. Ce qui est établi :
  recuit libre, énumération crib-exacte à la T5 et recuit ancré-variété **ne le franchissent pas**, et la cause est la
  dimension de la variété.
- **Antériorité.** Prolonge et explique l'échec déjà noté (base 10 §G ; `recuit_2026_09_24` ; base 09 §9, coin « ouvert »).
  Rien dans l'archive groups.io ni dans la base 02 ne propose d'attaque sur 97 lettres pour ce procédé à alphabet libre :
  la communauté teste des familles de clés contre les 24 cribs (la « boucle des 35 ans »), ce que l'on n'a pas refait.
- **Prochain pas** : `audits/k5_depth/` le jour de la publication de K5 ; les 8 signes sous LINCLOCK ; la description de
  la méthode détenue par Paradigm.

*(Base 13, écrite par l'agent MÉCANISME sur la branche `claude/loving-einstein-fizl93`. Numérotation arbitrée par le
chef : base 12 = documentaire/liens croisés (chef) ; base 13 = ce document. La base 09 (synthèse) et son guide §10.1
sont mis à jour par le chef, qui y pointera vers 12 et 13 — un seul propriétaire édite 09.)*
