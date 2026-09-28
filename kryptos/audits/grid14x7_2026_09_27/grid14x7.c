/* grid14x7.c — HYPOTHÈSE USER : feuille 14×31 = K3 (14×24) + [?+K4] (14×7 = 98 cases).
 * Le « ? » sert de cale (case 0). On place S = '?'+K4 (98) dans une grille 14×7 ou 7×14,
 * on applique les gestes de Sanborn façon K3 (lectures lignes/colonnes, bas→haut, retournements,
 * rotations = 8 transformées diédrales × lecture ligne/colonne × serpentin) + toutes les 5040
 * permutations de colonnes (largeur 7), dans les deux sens (π et π⁻¹). Puis Quagmire/Vigenère
 * PÉRIODIQUE (P=1..14) à alphabet PUBLIC (AZ, KRYPTOS, PALIMPSEST, ABSCISSA), VIG/BEA/VAR.
 * Deux modèles : (a) clé indexée par la position CLAIR ; (b) clé indexée par la position GRAVÉE.
 * Clé dérivée des 24 CRIBS AUTHENTIQUES seulement (majorité par résidu). AUCUN clair externe.
 * Juge : erreurs de crib + qoff qg_big (anglais ≥ -2.6). Null : même pipeline sur K4 mélangé.
 * Usage : grid14x7 qg_big.bin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
#define M 98
static const char *K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static int iscrib[N],cribL[N];
static float*QG;
static inline int md(int x){x%=26;return x<0?x+26:x;}
static const char*KW[4]={"","KRYPTOS","PALIMPSEST","ABSCISSA"};
static int perm[4][26],inv[4][26];
static void mk(void){for(int b=0;b<4;b++){int s[26]={0},a[26],n=0;for(const char*p=KW[b];*p;p++){int L=*p-'A';if(!s[L]){s[L]=1;a[n++]=L;}}
  for(int L=0;L<26;L++)if(!s[L])a[n++]=L;for(int i=0;i<26;i++){perm[b][a[i]]=i;inv[b][i]=a[i];}}}
static double qoff(const int*p){double q=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}}return no?q/no:0;}
/* ct[] = lettres gravées (0..25) ; m[j] = index K4 de la lettre qui donne le clair j */
static int CT[N];
typedef struct{int err;double q;int P,b,c,model;} Res;
static Res eval_map(const int*m){
  Res best={99,-9,0,0,0,0};
  for(int model=0;model<2;model++)for(int b=0;b<4;b++)for(int c=0;c<3;c++)for(int P=1;P<=14;P++){
    int cnt[14][26];memset(cnt,0,sizeof cnt);
    for(int j=0;j<N;j++)if(iscrib[j]){int idx=model?m[j]:j;int cv=perm[b][CT[m[j]]],pv=perm[b][cribL[j]];
      int k=c==0?md(cv-pv):c==1?md(cv+pv):md(pv-cv);cnt[idx%P][k]++;}
    int key[14],has[14];for(int r=0;r<P;r++){int bk=0,bc=0;for(int v=0;v<26;v++)if(cnt[r][v]>bc){bc=cnt[r][v];bk=v;}key[r]=bk;has[r]=bc>0;}
    int pt[N],err=0,ok=1;
    for(int j=0;j<N;j++){int idx=model?m[j]:j;if(!has[idx%P]){ok=0;break;}int cv=perm[b][CT[m[j]]],kv=key[idx%P];
      int pv=c==0?md(cv-kv):c==1?md(kv-cv):md(cv+kv);pt[j]=inv[b][pv];if(iscrib[j]&&pt[j]!=cribL[j])err++;}
    if(!ok)continue;
    double q=qoff(pt);
    /* critère : peu d'erreurs, puis qoff */
    if(err<best.err||(err==best.err&&q>best.q)){best.err=err;best.q=q;best.P=P;best.b=b;best.c=c;best.model=model;}
  }
  return best;
}
/* construit m[] à partir d'un ordre de lecture ord[0..97] sur S (S[0]='?') ; U[k]=S[ord[k]] ; clair = U sans '?' */
static void map_from_order(const int*ord,int*m){int j=0;for(int k=0;k<M;k++){int s=ord[k];if(s==0)continue;m[j++]=s-1;}}
static int NORD=0; static int ORD[20000][M]; static char NAME[20000][40];
static void add(const int*o,const char*nm){memcpy(ORD[NORD],o,sizeof(int)*M);snprintf(NAME[NORD],40,"%s",nm);NORD++;
  /* sens inverse */ int iv[M];for(int k=0;k<M;k++)iv[o[k]]=k;memcpy(ORD[NORD],iv,sizeof(int)*M);snprintf(NAME[NORD],40,"%s^-1",nm);NORD++;}
static void build_orders(void){
  int R_[2]={14,7},W_[2]={7,14};
  for(int g=0;g<2;g++){int R=R_[g],W=W_[g];
    for(int d=0;d<8;d++)for(int colread=0;colread<2;colread++)for(int serp=0;serp<2;serp++){
      /* grille R×W remplie ligne par ligne par S ; transformée diédrale d donne grille R'×W' ; lecture */
      int sw=(d==1||d==3||d==6||d==7); int RR=sw?W:R, WW=sw?R:W; int o[M],n=0;
      int outer=colread?WW:RR, inner=colread?RR:WW;
      for(int a=0;a<outer;a++)for(int t=0;t<inner;t++){int bb=(serp&&(a&1))?inner-1-t:t;
        int r2=colread?bb:a, c2=colread?a:bb; /* coords dans grille transformée RR×WW */
        int r,c; /* ramène à la grille d'origine R×W */
        switch(d){case 0:r=r2;c=c2;break;case 1:r=R-1-c2;c=r2;break;case 2:r=R-1-r2;c=W-1-c2;break;case 3:r=c2;c=W-1-r2;break;
          case 4:r=r2;c=W-1-c2;break;case 5:r=R-1-r2;c=c2;break;case 6:r=c2;c=r2;break;default:r=R-1-c2;c=W-1-r2;}
        o[n++]=r*W+c;}
      char nm[40];snprintf(nm,40,"%dx%d d%d %s%s",R,W,d,colread?"col":"row",serp?"S":"");add(o,nm);}
  }
  /* colonnaire largeur 7 (14 lignes) : toutes les 5040 permutations de colonnes */
  int pc[7]={0,1,2,3,4,5,6};
  for(;;){int o[M],n=0;for(int ci=0;ci<7;ci++){int c=pc[ci];for(int r=0;r<14;r++)o[n++]=r*7+c;}
    char nm[40];snprintf(nm,40,"col7 %d%d%d%d%d%d%d",pc[0],pc[1],pc[2],pc[3],pc[4],pc[5],pc[6]);add(o,nm);
    int i=5;while(i>=0&&pc[i]>pc[i+1])i--;if(i<0)break;int j=6;while(pc[j]<pc[i])j--;int t=pc[i];pc[i]=pc[j];pc[j]=t;
    for(int a=i+1,b2=6;a<b2;a++,b2--){t=pc[a];pc[a]=pc[b2];pc[b2]=t;}}
}
static void run(const char*label,int verbose){
  Res gb={99,-9,0,0,0,0};int gi=-1; double bestq_err2=-9;int qi=-1;Res qr;
  int m[N];
  for(int o=0;o<NORD;o++){map_from_order(ORD[o],m);Res r=eval_map(m);
    if(r.err<gb.err||(r.err==gb.err&&r.q>gb.q)){gb=r;gi=o;}
    if(r.err<=2&&r.q>bestq_err2){bestq_err2=r.q;qi=o;qr=r;}}
  static const char*CN[3]={"VIG","BEA","VAR"};
  printf("[%s] min cribErr=%d (%s, P=%d %s %s modèle %c, qoff %.2f)",label,gb.err,NAME[gi],gb.P,KW[gb.b][0]?KW[gb.b]:"AZ",CN[gb.c],gb.model?'b':'a',gb.q);
  if(qi>=0)printf(" | meilleur qoff à ≤2 err = %.2f (%s)",bestq_err2,NAME[qi]);
  printf("\n");(void)verbose;(void)qr;
}
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[21+i]=1;cribL[21+i]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[63+i]=1;cribL[63+i]=b[i]-'A';}
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  mk(); build_orders(); printf("ordres testés : %d (dont inverses)\n",NORD);
  for(int i=0;i<N;i++)CT[i]=K4[i]-'A';
  /* contrôle positif : clair anglais + cribs, Vig KRYPTOS clé KRYPTOS (P=7) en modèle a, puis transposition colonnaire 3620514 */
  {int saved[N];memcpy(saved,CT,sizeof saved);
   const char*eng="WEAREINTHEFIELDNEARTHEEASTNORTHEASTCORNEROFTHECOURTYARDWAITINGFORTHEBERLINCLOCKSIGNALATMIDNIGHTXX";
   int pt[N];for(int i=0;i<N;i++)pt[i]=eng[i]-'A';for(int i=0;i<N;i++)if(iscrib[i])pt[i]=cribL[i];
   const char*key="KRYPTOS";int V[N];for(int j=0;j<N;j++){int kv=perm[1][key[j%7]-'A'];V[j]=inv[1][md(perm[1][pt[j]]+kv)];}
   /* gravure = S où S[ord[k]] = U[k], U = '?'+V ; ord = col7 0362514 */
   int pc[7]={0,3,6,2,5,1,4},o[M],n=0;for(int ci=0;ci<7;ci++){int c=pc[ci];for(int r=0;r<14;r++)o[n++]=r*7+c;}
   int U[M];U[0]=-1;for(int j=0;j<N;j++)U[j+1]=V[j];
   int S[M];for(int k=0;k<M;k++)S[o[k]]=U[k];
   /* le '?' doit être en S[0] pour notre convention : on place la cale là où elle tombe ; ici on vérifie seulement la récupération */
   int qpos=-1;for(int s=0;s<M;s++)if(S[s]<0)qpos=s;
   if(qpos==0){int j=0;for(int s=1;s<M;s++)CT[j++]=S[s];run("CONTROLE POSITIF",0);}
   else printf("(contrôle : cale tombée en %d, variante ignorée)\n",qpos);
   memcpy(CT,saved,sizeof saved);}
  run("K4 RÉEL",1);
  srand(11);for(int t=0;t<5;t++){int sh[N];for(int i=0;i<N;i++)sh[i]=K4[i]-'A';for(int i=N-1;i>0;i--){int j=rand()%(i+1);int x=sh[i];sh[i]=sh[j];sh[j]=x;}
    memcpy(CT,sh,sizeof sh);char l[20];snprintf(l,20,"NULL %d",t);run(l,0);}
  return 0;
}
