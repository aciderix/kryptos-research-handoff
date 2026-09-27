/* digraphic_check.c — PREUVE ÉLÉMENTAIRE que K4 n'est pas un chiffre digraphique/par blocs.
 * (Réponse à la demande du chef d'un "bijcheck digraphique" : inutile de coder Playfair — la
 *  structure même de K4 le tue, avant les cribs, plus fort qu'une variété.)
 *
 * Trois faits mesurés sur le chiffré K4 :
 *  A) K4 contient LES 26 LETTRES (J présent). Un carré 5x5 (Playfair/two-square/four-square) n'a
 *     que 25 cases (une lettre omise) ⇒ son chiffré ne peut PAS contenir la lettre omise. K4 les a
 *     toutes ⇒ AUCUN carré 5x5 standard ne peut produire K4. (four-square standard omet J : idem.)
 *  B) |K4| = 97 = PREMIER (impair). Tout chiffre par blocs de taille n>=2 (Hill n×n, tout
 *     digraphique à blocs pairs) préserve la longueur en multiples de n ⇒ sortie de longueur
 *     multiple de n, jamais 97 (97 impair, et 97 premier ⇒ divisible par aucun n∈[2..96]).
 *     Produire exactement 97 exigerait de DROPPER/PADDER des lettres ⇒ casse le 1:1 confirmé
 *     (BERLINCLOCK ↔ NYPVTTMZFPK, positions exactes).
 *  C) Corroboration : Playfair ne sort JAMAIS un digramme à lettres identiques (c1≠c2 pour toute
 *     paire p1≠p2, et les paires p1=p2 sont scindées par X). Or K4 a des doublets adjacents qui
 *     tombent DANS une paire pour LES DEUX découpages (offset 0 et 1) ⇒ aucun pavage simple ne
 *     les évite. (Caveat : insertions X en milieu de texte peuvent décaler ; A et B suffisent.)
 *
 * Usage : digraphic_check
 */
#include <stdio.h>
#include <string.h>
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int isprime(int n){ if(n<2)return 0; for(int d=2;d*d<=n;d++) if(n%d==0)return 0; return 1; }
int main(void){
  int N=strlen(K4); int seen[26]={0},distinct=0;
  for(int i=0;i<N;i++){int L=K4[i]-'A'; if(!seen[L]){seen[L]=1;distinct++;}}
  printf("=== K4 : impossibilité digraphique / par blocs ===\n");
  printf("A) longueur=%d, lettres distinctes=%d/26",N,distinct);
  if(distinct==26){int miss=-1;for(int L=0;L<26;L++)if(!seen[L])miss=L; printf(" (AUCUNE lettre omise) ⇒ carré 5x5 (25) IMPOSSIBLE\n");(void)miss;}
  else printf(" (omises: présentes<26) ⇒ carré 25 à examiner\n");
  printf("B) 97 premier ? %s ", isprime(N)?"OUI":"non");
  printf("⇒ aucun chiffre par blocs n>=2 ne produit une longueur 97 sans drop/pad ⇒ Hill/blocs IMPOSSIBLE (1:1 confirmé)\n");
  /* C) doublets adjacents et parité */
  printf("C) doublets adjacents : ");
  int off0=0,off1=0;
  for(int i=0;i<N-1;i++) if(K4[i]==K4[i+1]){ printf("%c@%d ",K4[i],i); if(i%2==0)off0++; else off1++; }
  printf("\n   dans-une-paire offset0=%d, offset1=%d (Playfair ne sort jamais de digramme doublé)\n",off0,off1);
  printf("\nCONCLUSION : Playfair/two-square/four-square (A) et Hill/blocs (B) ÉLIMINÉS pour K4.\n");
  return 0;
}
