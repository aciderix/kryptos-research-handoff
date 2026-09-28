#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* "Flip the chart, shine a light" applique aux FEUILLES DE TRAVAIL (clairs K1/K2/K3) et au
   chiffre K1-K3, comme grid-cle running-key 1:1 sur K4. 8 transformees diedrales x largeurs
   {7,14,21,31} x alphabets {AZ,KRYPTOS} x {VIG,BEAU,VAR} x tous offsets. 1:1 (preserve Bean).
   Compare aux 24 cribs (valeur de cle requise par position). Si 24/24 -> lit le clair. */

static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
/* cribs 0-indexes */
static int cpos[24]={21,22,23,24,25,26,27,28,29,30,31,32,33, 63,64,65,66,67,68,69,70,71,72,73};
static char cpt[25]="EASTNORTHEASTBERLINCLOCK"; /* plaintext aligned to cpos */

static const char*AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char*KR="KRYPTOSABCDEFGHIJLMNQUVWXZ";

static const char*srcs[4];
static const char*names[4]={"K1","K2","K3","K1K2K3"};

static int posA(const char*A,char x){for(int i=0;i<26;i++)if(A[i]==x)return i;return -1;}

/* transforme la source (longueur L) dans une grille largeur w, applique op diedral,
   ecrit le keystream (permutation des lettres) dans ks (longueur L). retourne L. */
static int dihedral(const char*s,int L,int w,int op,char*ks){
    int R=(L+w-1)/w;
    /* cell (r,c) r<R c<w ; lettre = s[r*w+c] si <L sinon vide */
    /* new coords selon op ; new grid dims (nr,nc) ; index = nrr*nc+ncc */
    int nc = (op>=4)? R : w; /* transpose/rot90/rot270/anti -> largeur R */
    /* on collecte (newlin, lettre) puis tri par newlin */
    static int lin[4000]; static char let[4000]; int m=0;
    for(int r=0;r<R;r++)for(int c=0;c<w;c++){int idx=r*w+c; if(idx>=L)continue; char ch=s[idx];
        int nr,ncc;
        switch(op){
          case 0: nr=r; ncc=c; break;              /* id */
          case 1: nr=r; ncc=w-1-c; break;          /* flipH */
          case 2: nr=R-1-r; ncc=c; break;          /* flipV */
          case 3: nr=R-1-r; ncc=w-1-c; break;      /* rot180 */
          case 4: nr=c; ncc=r; break;              /* transpose (w<->R) */
          case 5: nr=c; ncc=R-1-r; break;          /* rot90 cw */
          case 6: nr=w-1-c; ncc=r; break;          /* rot270 */
          default: nr=w-1-c; ncc=R-1-r; break;     /* anti-transpose */
        }
        lin[m]=nr*nc+ncc; let[m]=ch; m++;
    }
    /* tri par lin (insertion, m<=~340) */
    for(int a=0;a<m;a++)for(int b=a+1;b<m;b++) if(lin[b]<lin[a]){int t=lin[a];lin[a]=lin[b];lin[b]=t; char u=let[a];let[a]=let[b];let[b]=u;}
    for(int a=0;a<m;a++)ks[a]=let[a]; return m;
}

/* valeur de cle requise a un crib, alphabet A, conv (0=VIG,1=BEAU,2=VAR) */
static char reqkey(const char*A,int conv,char P,char C){
    int p=posA(A,P),c=posA(A,C),k;
    if(conv==0)k=((c-p)%26+26)%26;        /* VIG: c=A[p+k] */
    else if(conv==1)k=((c+p)%26)%26;      /* BEAU: c=A[k-p] -> k=c+p */
    else k=((p-c)%26+26)%26;              /* VAR: c=A[p-k] -> k=p-c */
    return A[k];
}
static char dec(const char*A,int conv,char C,char K){
    int c=posA(A,C),k=posA(A,K),p;
    if(conv==0)p=((c-k)%26+26)%26;
    else if(conv==1)p=((k-c)%26+26)%26;   /* BEAU sym: p=k-c */
    else p=((c+k)%26)%26;                  /* VAR: p=c+k */
    return A[p];
}

int main(){
  srcs[0]="BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION";
  srcs[1]="ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLETHEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHISTHEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATIONONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTXLAYERTWO";
  srcs[2]="SLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWERPARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPERLEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEEREDINTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLYDETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ";
  static char cat[1200]; cat[0]=0; strcat(cat,srcs[0]);strcat(cat,srcs[1]);strcat(cat,srcs[2]); srcs[3]=cat;
  const char*ALPHS[2]={AZ,KR}; const char*AN[2]={"AZ","KR"}; const char*CN[3]={"VIG","BEAU","VAR"};
  int widths[4]={7,14,21,31};
  int best=0; char bestinfo[256]="";
  char ks[4000];
  for(int s=0;s<4;s++){int L=strlen(srcs[s]);
   for(int wi=0;wi<4;wi++){int w=widths[wi];
    for(int op=0;op<8;op++){int m=dihedral(srcs[s],L,w,op,ks);
     for(int ai=0;ai<2;ai++){const char*A=ALPHS[ai];
      for(int cv=0;cv<3;cv++){
       /* required key letters at cribs */
       char rk[24]; for(int t=0;t<24;t++) rk[t]=reqkey(A,cv,cpt[t],K4[cpos[t]]);
       for(int off=0; off+96 < m; off++){
         int match=0; for(int t=0;t<24;t++){ if(ks[cpos[t]+off]==rk[t]) match++; }
         if(match>best){best=match; snprintf(bestinfo,sizeof bestinfo,"src=%s w=%d op=%d A=%s conv=%s off=%d match=%d/24",names[s],w,op,AN[ai],CN[cv],off,match);}
         if(match>=22){ /* lire le clair */
           printf(">> HIT %s w=%d op=%d A=%s conv=%s off=%d match=%d/24\n   PT=",names[s],w,op,AN[ai],CN[cv],off,match);
           for(int i=0;i<97;i++)putchar(dec(A,cv,K4[i],ks[i+off])); printf("\n");
         }
       }
      }}}}}
  printf("MEILLEUR: %s\n",bestinfo);
  return 0;
}
