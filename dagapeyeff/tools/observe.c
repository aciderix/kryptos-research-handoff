/* observe.c — mesures structurelles reproductibles du chiffré d'Agapeyeff (aucune hypothèse de clair).
 * Usage : observe ../data/ciphertext_1939.txt */
#include <stdio.h>
#include <string.h>
int main(int argc,char**argv){
  FILE*f=fopen(argc>1?argv[1]:"../data/ciphertext_1939.txt","r"); char d[1000];int n=0,c;
  while((c=fgetc(f))!=EOF) if(c>='0'&&c<='9') d[n++]=c; fclose(f);
  printf("chiffres : %d (groupes de 5 : %d)\n",n,n/5);
  int tail=0; while(tail<n && d[n-1-tail]=='0') tail++;
  printf("zéros terminaux : %d\n",tail);
  int L=392; printf("hypothèse : %d chiffres utiles -> %d paires = 14x14 ? %s\n",L,L/2,(L/2==196)?"oui":"non");
  int bad=0; for(int i=0;i<L;i++){int x=d[i]-'0'; int rowset=(x>=6||x==0); if((i%2==0)!=rowset) bad++;}
  printf("alternance {6789 0} (pos paires) / {12345} (pos impaires) sur %d chiffres : %d violations\n",L,bad);
  /* violations au-delà de 392 */
  printf("chiffres 393-395 : %c%c%c\n",d[392],d[393],d[394]);
  int cnt[10][10]={{0}}; for(int i=0;i<L;i+=2) cnt[d[i]-'0'][d[i+1]-'0']++;
  int rows[5]={6,7,8,9,0}; int distinct=0; double s=0;
  printf("\ntable des paires (lignes 6 7 8 9 0 × colonnes 1..5):\n     1   2   3   4   5\n");
  for(int r=0;r<5;r++){printf("%d ",rows[r]);for(int k=1;k<=5;k++){int v=cnt[rows[r]][k];printf("%4d",v);if(v)distinct++;s+=v*(v-1.0);}printf("\n");}
  double ic=s/(196.0*195.0);
  printf("symboles distincts : %d/25 ; IC=%.4f ; IC×25=%.3f (anglais ~1.73×26/25 ; aléatoire 1.00)\n",distinct,ic,ic*25);
  /* répétitions consécutives de paires */
  printf("\npaires consécutives identiques : ");
  for(int i=0;i+3<L;i+=2) if(d[i]==d[i+2]&&d[i+1]==d[i+3]) printf("%c%c@%d ",d[i],d[i+1],i/2);
  printf("\ntriplets : ");
  for(int i=0;i+5<L;i+=2) if(d[i]==d[i+2]&&d[i+1]==d[i+3]&&d[i]==d[i+4]&&d[i+1]==d[i+5]) printf("%c%c@%d ",d[i],d[i+1],i/2);
  printf("\n\ngrille 14x14 des paires (ligne par ligne):\n");
  for(int r=0;r<14;r++){for(int k=0;k<14;k++){int i=2*(r*14+k);printf("%c%c ",d[i],d[i+1]);}printf("\n");}
  return 0;
}
