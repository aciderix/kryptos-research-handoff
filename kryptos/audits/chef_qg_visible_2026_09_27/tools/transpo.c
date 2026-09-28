/* transpo.c — dé-transposition colonnaire de K4 (couche transposition AVANT autoclé).
 * Hypothèse : clair -(autoclé/subst)-> intermédiaire -(transposition colonnaire)-> K4.
 * On INVERSE la transposition pour retrouver l'intermédiaire, à passer ensuite à quag2/ac7g
 * (les cribs restent aux positions clair 21-33/63-73, préservées par la transposition).
 *
 * Transposition colonnaire standard : écrire le texte ligne par ligne dans W colonnes
 * (dernière ligne partielle), lire les colonnes dans l'ordre 'order'. Inverse = ce prog.
 * order : "id" (0..W-1), "rev" (W-1..0), ou une permutation explicite "3,1,0,2,..".
 *
 * Compile: cc -O2 -o transpo transpo.c ; Usage: transpo CT W order   (imprime l'intermédiaire)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
int main(int argc,char**argv){
  if(argc<4){fprintf(stderr,"usage: transpo CT W order(id|rev|p0,p1,..)\n");return 1;}
  const char*ct=argv[1]; int L=strlen(ct); int W=atoi(argv[2]);
  if(W<2||W>N){fprintf(stderr,"bad W\n");return 1;}
  int order[64]; const char*os=argv[3];
  if(!strcmp(os,"id")) for(int i=0;i<W;i++)order[i]=i;
  else if(!strcmp(os,"rev")) for(int i=0;i<W;i++)order[i]=W-1-i;
  else { int i=0; char b[256]; strncpy(b,os,255);b[255]=0; for(char*t=strtok(b,",");t&&i<W;t=strtok(NULL,","))order[i++]=atoi(t); }
  int rows=(L+W-1)/W;               /* nb lignes */
  int rem=L%W;                      /* colonnes 'pleines' = rem (si rem>0), sinon toutes pleines */
  /* hauteur de la colonne j (dans l'ordre d'écriture 0..W-1) */
  int height[64]; for(int j=0;j<W;j++) height[j] = (rem==0)? rows : (j<rem? rows : rows-1);
  /* La lecture (chiffrement) a produit ct en lisant les colonnes dans 'order', de haut en bas.
     On reconstruit la grille : on parcourt ct et on remplit colonne order[k] de haut en bas. */
  char grid[64][64]; for(int r=0;r<rows;r++)for(int c=0;c<W;c++)grid[r][c]=0;
  int p=0;
  for(int k=0;k<W;k++){ int col=order[k]; int h=height[col];
    for(int r=0;r<h;r++){ if(p<L) grid[r][col]=ct[p++]; } }
  /* intermédiaire = lecture ligne par ligne */
  char out[N+1]; int o=0;
  for(int r=0;r<rows;r++) for(int c=0;c<W;c++) if(grid[r][c]) out[o++]=grid[r][c];
  out[o]=0;
  printf("%s\n",out);
  return 0;
}
