# Protocole obligatoire (fixé par le user, appliqué à chaque expérience)

1. **Audit des connaissances d'abord** (`01_etat_de_lart.md`).
2. **Registre des familles déjà testées** — « pas testé ici » ≠ « jamais testé ».
3. **Pré-inscription avant résultat** : représentation, alignement, mécanisme, paramètres, espace de recherche,
   transformation finale, convention de sortie, critère de succès. Fichier `PREREGISTRATION.md` commité **avant**
   le premier run sur le vrai chiffré.
4. **Contrôles** : tout solveur doit d'abord retrouver des messages synthétiques produits par le même mécanisme.
5. **Pas de sélection a posteriori** : quelques mots anglais ne rendent pas une sortie admissible.
6. **Falsification active** : alternatives, paramètres voisins, null (chiffré mélangé), contrôles négatifs,
   stabilité, solutions concurrentes.
7. **Vérification directe** : toute proposition (clair + clé + mécanisme) est ré-enchiffrée et doit redonner
   **exactement** les 392 chiffres.
8. Séparer : connu / déjà testé / nouveau / observation / hypothèse / résultat / preuve.
9. **Aucune revendication de résolution sans oracle indépendant.** Pour d'Agapeyeff, faute d'oracle externe,
   le seuil minimal est : anglais continu sur ≥ 80 % du texte, mécanisme historiquement plausible sans paramètre
   ajusté, score hors de la distribution nulle, ré-enchiffrement exact, stabilité — et même alors on parle de
   « candidat », pas de « solution ».
