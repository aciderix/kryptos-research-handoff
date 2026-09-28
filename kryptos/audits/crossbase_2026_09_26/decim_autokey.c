/* decim_autokey.c — porte NON-1:1 : decimation de position x m mod 97 AVANT autocle ecart-7.
 * Modele : E (grave) = decim_m( C1 ), ou C1 = autocle-ecart-7(P) en ordre-clair.
 * Donc on DE-transpose : D_k = E[(m*k) mod 97] pour tester m, ou l'inverse ; puis on
 * cherche un autocle ecart-7 structure sur D avec cribs a leurs positions clair 21-33 / 63-73.
 * K3 lui-meme est une decimation (x192 mod 337) ; Sanborn "flip the chart, read a different
 * direction" ; il recule sur le 1:1 (Dunin 2023) => porte legitime, jamais faite hors colonnaire (S4).
 *
 * Alphabets structures : bases keyees {AZ,KRYPTOS,PALIMPSEST,ABSCISSA} (mul=1). Option 'affine'
 * ajoute les 12 multiplicateurs. Conventions VIG/BEAU/VAR x source PLAIN/CIPHER, lag 7.
 * kappa brute-force par residu (26) -> robuste (repris de s3.c).
 *
 * Usage :
 *   decim_autokey selftest
 *   decim_autokey run  [tol] [affine]     (scan m=2..96 fwd+inv sur K4 ; imprime survivants + min)
 *   decim_autokey null [nwit] [seed] [affine]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 97
#define LAG 7
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static const int MUL[12]={1,3,5,7,9,11,15,17,19,21,23,25};
static const char*KW[4]={"","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*BN[4]={"AZ","KRY","PAL","ABS"};
static const char*CN[3]={"VIG","BEAU","VAR"};
static const char*SN[2]={"PLAIN","CIPHER"};
static int KV[4][26];
static void buildKV(void){for(int b=0;b<4;b++){int seen[26]={0},al[26],n=0;
  for(const char*p=KW[b];*p;p++){int L=*p-'A';if(!seen[L]){seen[L]=1;al[n++]=L;}}
  for(int L=0;L<26;L++)if(!seen[L])al[n++]=L; for(int i=0;i<26;i++)KV[b][al[i]]=i;}}
static void mkperm(int base,int a,int b,int*perm){for(int L=0;L<26;L++)perm[L]=md(a*KV[base][L]+b);}
static inline int fx(int conv,int t,int g){if(conv==0)return md(t-g);if(conv==1)return md(g-t);return md(t+g);}
static int invmod(int a,int m){for(int x=1;x<m;x++)if((long)a*x%m==1)return x;return -1;}

/* decode autocle ecart-7 : kappa_r DERIVE analytiquement (x_i = a_i*kappa_r + b_i mod 26,
 * a_i in {0,1,25}) -> majorite sur les cribs de la chaine. Equivaut au brute-force 26x de s3,
 * ~26x plus rapide. Renvoie erreurs de crib, remplit pt (optionnel). */
static int decode(int conv,int src,const int*ct,const int*sig,const int*tau,int*pt){
  int siginv[26];for(int i=0;i<26;i++)siginv[sig[i]]=i; int errtot=0;
  int aj[N],bj[N];
  for(int r=0;r<LAG;r++){
    int reqk[16],nreq=0,fixederr=0;
    int aprev=0,bprev=0;
    for(int i=r,j=0;i<N;i+=LAG,j++){
      int t=tau[ct[i]]; int a,b;
      if(j==0){ if(conv==0){a=25;b=t;} else if(conv==1){a=1;b=md(-t);} else {a=1;b=t;} }
      else if(src==0){ /* PLAIN: g=x_prev=aprev*k+bprev */
        if(conv==0){a=md(-aprev);b=md(t-bprev);} else if(conv==1){a=aprev;b=md(bprev-t);} else {a=aprev;b=md(t+bprev);}
      } else { int g=tau[ct[i-LAG]]; a=0; b=fx(conv,t,g); }
      aj[i]=a;bj[i]=b; aprev=a;bprev=b;
      if(iscrib[i]){int want=sig[cribL[i]];
        if(a==0){ if(md(b)!=want) fixederr++; }
        else { reqk[nreq++]=md(a*(want-b)); } /* a in {1,25}=+-1, a^{-1}=a */
      }
    }
    int bestk=0,bestc=0;
    for(int q=0;q<nreq;q++){int c=0;for(int p=0;p<nreq;p++)if(reqk[p]==reqk[q])c++; if(c>bestc){bestc=c;bestk=reqk[q];}}
    errtot += fixederr + (nreq-bestc);
    if(pt) for(int i=r;i<N;i+=LAG) pt[i]=siginv[md(aj[i]*bestk+bj[i])];
  }
  return errtot;
}
/* min erreurs de crib sur le set structure, pour un ct donne (deja de-transpose) */
static int sweep_min(const int*ct,int affine,int*best_meta){
  int mn=99,nb=affine?4:4,na=affine?12:1;
  for(int conv=0;conv<3;conv++)for(int src=0;src<2;src++)
    for(int bs=0;bs<nb;bs++)for(int ai=0;ai<na;ai++)for(int cs=0;cs<26;cs++){int sig[26];mkperm(bs,MUL[ai],cs,sig);
      for(int bt=0;bt<nb;bt++)for(int aj=0;aj<na;aj++)for(int cb=0;cb<26;cb++){int tau[26];mkperm(bt,MUL[aj],cb,tau);
        int e=decode(conv,src,ct,sig,tau,NULL);
        if(e<mn){mn=e; if(best_meta){best_meta[0]=conv;best_meta[1]=src;best_meta[2]=bs;best_meta[3]=ai;best_meta[4]=cs;best_meta[5]=bt;best_meta[6]=aj;best_meta[7]=cb;}}
      }}
  return mn;
}
static uint64_t rs;static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static void encrypt(int conv,int src,const int*sig,const int*tau,const int*kap,const int*pt,int*ct){
  int tauinv[26];for(int i=0;i<26;i++)tauinv[tau[i]]=i; int xv[N];for(int i=0;i<N;i++)xv[i]=sig[pt[i]];
  for(int i=0;i<N;i++){int g=(i<LAG)?kap[i]:((src==0)?xv[i-LAG]:0);
    if(src==1&&i>=LAG)g=tau[ct[i-LAG]];
    int tval;if(conv==0)tval=md(xv[i]+g);else if(conv==1)tval=md(g-xv[i]);else tval=md(xv[i]-g);
    ct[i]=tauinv[tval];}
}
int main(int argc,char**argv){
  setcribs();buildKV();
  int E[N];for(int i=0;i<N;i++)E[i]=K4[i]-'A';

  if(argc>=2&&!strcmp(argv[1],"selftest")){
    /* fabrique fake = decim_m( autocle(P+cribs) ) ; de-transpose par inv(m) ; sweep doit trouver 0 err */
    int ok=1;
    int mtest[3]={5,7,11};
    for(int mi=0;mi<3;mi++){int m=mtest[mi];
      rs=7+m; int sig[26],tau[26],kap[LAG],pt[N],C1[N],Eg[N];
      mkperm(1,1,0,sig);mkperm(0,1,0,tau); /* keyed KRYPTOS x AZ */
      for(int i=0;i<LAG;i++)kap[i]=rnd()%26; for(int i=0;i<N;i++)pt[i]=rnd()%26;
      for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
      encrypt(0,0,sig,tau,kap,pt,C1);                 /* VIG/PLAIN */
      for(int k=0;k<N;k++)Eg[k]=C1[(m*k)%N];          /* grave = decim_m(C1) */
      int mv=invmod(m,N); int D[N];for(int k=0;k<N;k++)D[k]=Eg[(mv*k)%N]; /* de-transpose */
      /* D doit == C1 */
      int okC1=1;for(int i=0;i<N;i++)if(D[i]!=C1[i])okC1=0;
      int p2[N];int e=decode(0,0,D,sig,tau,p2);
      int rec=0;for(int i=0;i<N;i++)if(p2[i]==pt[i])rec++;
      int good=(okC1&&e==0&&rec==N); ok&=good;
      printf("  m=%d inv=%d : D==C1 %s err=%d rec=%d/%d %s\n",m,mv,okC1?"OK":"NO",e,rec,N,good?"OK":"FAIL");
    }
    printf("selftest %s\n",ok?"PASS":"FAIL"); return ok?0:1;
  }
  if(argc>=2&&!strcmp(argv[1],"run")){
    int tol=argc>2?atoi(argv[2]):2; int affine=argc>3?atoi(argv[3]):0;
    int gmin=99,gm=0,gdir=0;
    for(int dir=0;dir<2;dir++)for(int m=2;m<N;m++){int mm=(dir==0)?m:invmod(m,N);
      int D[N];for(int k=0;k<N;k++)D[k]=E[(mm*k)%N];
      int meta[8];int mn=sweep_min(D,affine,meta);
      if(mn<gmin){gmin=mn;gm=m;gdir=dir;}
      if(mn<=tol){int sig[26],tau[26];mkperm(meta[2],MUL[meta[3]],meta[4],sig);mkperm(meta[5],MUL[meta[6]],meta[7],tau);
        int pt[N];decode(meta[0],meta[1],D,sig,tau,pt);
        printf("HIT dir=%d m=%d err=%d %s/%s sig=%s*%dx+%d tau=%s*%dx+%d PT=",dir,m,mn,CN[meta[0]],SN[meta[1]],BN[meta[2]],MUL[meta[3]],meta[4],BN[meta[5]],MUL[meta[6]],meta[7]);
        for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');}
    }
    fprintf(stderr,"scan 190 decimations (m=2..96 x fwd/inv), affine=%d. min_crib_errors=%d at dir=%d m=%d\n",affine,gmin,gdir,gm);
    return 0;
  }
  if(argc>=2&&!strcmp(argv[1],"null")){
    int nwit=argc>2?atoi(argv[2]):300; rs=argc>3?(uint64_t)atol(argv[3]):0xBEEF; int affine=argc>4?atoi(argv[4]):0;
    /* K4 : min sur toutes decimations */
    int k4min=99; for(int dir=0;dir<2;dir++)for(int m=2;m<N;m++){int mm=(dir==0)?m:invmod(m,N);int D[N];for(int k=0;k<N;k++)D[k]=E[(mm*k)%N];int mn=sweep_min(D,affine,NULL);if(mn<k4min)k4min=mn;}
    int bag[N];for(int i=0;i<N;i++)bag[i]=E[i];
    int le=0,eq=0,hist[25]={0};
    for(int w=0;w<nwit;w++){int sh[N];for(int i=0;i<N;i++)sh[i]=bag[i];
      for(int i=N-1;i>0;i--){int j=rnd()%(i+1);int t=sh[i];sh[i]=sh[j];sh[j]=t;}
      int wmin=99;for(int dir=0;dir<2;dir++)for(int m=2;m<N;m++){int mm=(dir==0)?m:invmod(m,N);int D[N];for(int k=0;k<N;k++)D[k]=sh[(mm*k)%N];int mn=sweep_min(D,affine,NULL);if(mn<wmin)wmin=mn;}
      if(wmin<25)hist[wmin]++; if(wmin<=k4min)le++; if(wmin==k4min)eq++;
    }
    printf("K4 min_crib_errors (best over 190 decimations, affine=%d) = %d\n",affine,k4min);
    printf("temoins = %d ; P(temoin_min <= K4_min) = %d/%d = %.4f ; dont == %d\n",nwit,le,nwit,(double)le/nwit,eq);
    for(int m=0;m<25;m++)if(hist[m])printf("  %2d : %d\n",m,hist[m]);
    return 0;
  }
  fprintf(stderr,"usage: selftest | run [tol] [affine] | null [nwit] [seed] [affine]\n");return 1;
}
