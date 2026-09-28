#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* Pour chaque ligne K4 (row 25,26,27 : 31 lettres col0..30), fit arc_s = a + b*col
   (moindres carres), residu par lettre. Cherche structure : outliers, signe, correlation crib. */
typedef struct{int row,col;char L;double arc;int isc;} R;
static R d[200]; static int n=0;
/* cribs K4 (positions dans le clair 0..96) : EAST..21-33, BERLIN..63-73.
   Ici on repere par (row,col) -> gidx K4. row25 col j -> pos 4+j ; row26 -> 35+j ; row27 -> 66+j.
   OBKR row24 col27..30 -> pos 0..3. */
static int poscrib(int row,int col){
  int p; if(row==24) p=col-27; else if(row==25)p=4+col; else if(row==26)p=35+col; else p=66+col;
  if((p>=21&&p<=33)||(p>=63&&p<=73)) return 1; return 0;
}
int main(){
  FILE*f=fopen("data/master_cipher_letters.csv","r");char line[512];fgets(line,sizeof line,f);
  while(fgets(line,sizeof line,f)){
    int gi,row,col;char sec[16],L,cr[8],q[8],cf[16];double x,y,z,arc;
    if(sscanf(line,"%d,%d,%d,%c,%15[^,],%7[^,],%7[^,],%15[^,],%lf,%lf,%lf,%lf",&gi,&row,&col,&L,sec,cr,q,cf,&x,&y,&z,&arc)<12)continue;
    if(strncmp(sec,"K4",2))continue;
    d[n].row=row;d[n].col=col;d[n].L=L;d[n].arc=arc;d[n].isc=poscrib(row,col);n++;
  }
  fclose(f);
  for(int rr=25;rr<=27;rr++){
    double sx=0,sy=0,sxx=0,sxy=0;int m=0;
    for(int i=0;i<n;i++)if(d[i].row==rr){sx+=d[i].col;sy+=d[i].arc;sxx+=d[i].col*d[i].col;sxy+=d[i].col*d[i].arc;m++;}
    double b=(m*sxy-sx*sy)/(m*sxx-sx*sx),a=(sy-b*sx)/m;
    /* residus + stats */
    double ss=0;int cnt=0;double res[40];int cols[40];char Ls[40];int isc[40];int k=0;
    for(int i=0;i<n;i++)if(d[i].row==rr){double e=d[i].arc-(a+b*d[i].col);res[k]=e;cols[k]=d[i].col;Ls[k]=d[i].L;isc[k]=d[i].isc;ss+=e*e;cnt++;k++;}
    double sd=sqrt(ss/cnt);
    printf("=== ROW %d : pas b=%.3f u/col, sigma_residu=%.3f u ===\n",rr,b,sd);
    printf("col:res(sign)  [*=crib, !=outlier>1.5sigma]\n");
    for(int i=0;i<k;i++){
      char sgn = res[i]>0?'+':'-';
      char out = fabs(res[i])>1.5*sd?'!':' ';
      char cb = isc[i]?'*':' ';
      printf("%2d:%+5.2f%c%c%c %c  ",cols[i],res[i],sgn,out,cb,Ls[i]);
      if((i+1)%6==0)printf("\n");
    }
    printf("\n");
    /* pattern de signe (bit par lettre) */
    printf("signes row%d: ",rr);for(int i=0;i<k;i++)printf("%c",res[i]>0?'1':'0');printf("\n\n");
  }
  return 0;
}
