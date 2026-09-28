/* visible_sweep.c — H-VISIBLE-ALPHABET (tache conjointe mesh).
 * Teste les alphabets-permutations derives de l'ORDRE SPATIAL du texte grave
 * (14 candidats du chef) comme sigma et/ou tau de l'autocle ecart-7, dans les
 * 3 conventions x 2 sources de cle, avec CONTROLE POSITIF + temoins K4-melange.
 *
 * Modele (identique a s3.c) : chaine mod 7, x_i = f(tau[ct_i], g_i),
 *   g_i = kappa_r (j==0) sinon PLAIN: x_{i-7} / CIPHER: tau[ct_{i-7}].
 *   f: VIG x=t-g ; BEAU x=g-t ; VAR x=t+g. pt_i = sigma^{-1}[x_i].
 * kappa_r brute-force (26) par residu pour minimiser les erreurs de crib.
 *
 * Alphabet donne en "alpha[pos]=lettre" (chaine 26). perm[lettre]=pos.
 *
 * Usage:
 *   visible_sweep selftest
 *   visible_sweep run   [tol]         (sweep K4, imprime survivants <= tol, + min global)
 *   visible_sweep null  [nwit] [seed] (min-erreurs sur nwit temoins K4-melange vs K4)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 97
#define LAG 7

static const char *K4 =
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";

static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}}
static inline int md(int x){x%=26;return x<0?x+26:x;}

/* 21 alphabets: 19 candidats spatiaux (famille GELEE, chef) + AZ + KRYPTOS (alpha[pos]=lettre) */
#define M 27
#define IAZ 25
#define IKRY 26
static const char *ANAME[M]={
 "read_full_first","read_full_first_REV","read_full_last","read_K4_first",
 "colmajor_first","colmajor_first_REV","colmajor_mirror_first","colmajor_mirror_K4",
 "rows_upsidedown","rows_mirror_first","rows_mirror_K4","arc_first","arc_first_REV",
 "boustrophedon",
 "diag_sum_first","diag_sum_first_REV","antidiag_first","freq_desc","freq_asc",
 "K0keyed","K0keyed_REV","K1keyed","K1keyed_REV",
 "Morse_order","Morse_order_REV",
 "AZ","KRYPTOS"};
static const char *ALPHA[M]={
 "EMUFPHZLRAXYSDJKNGIVQTBWCO","RACKEUHGIDJTXZWPFMVYNBLSQO","OQSLBNYVMFPWZXTJDIGHUEKCAR",
 "OBKRUXGHLSIFWVQPNTJEZADYMC","EYVGTQHFDCWBARUMIZLNPSKOXJ","IRXJPOBTCEFDAVYSKQHNLGUMWZ",
 "JXRIVDEAFPCTBONHKQYSMLUGZW","ROPKSYABNCGFEUIWQHVDLJTXZM","VTMZFPKWGDXJCIUHAERQSLNBYO",
 "JVIFNGHSRKDLZUYXAPMETQBCWO","RKBOSGNPQVLFWIUHXYADJTZECM","EFCUGVYTAQRDBHWNIMLPZSKOXJ",
 "QRTFPEIXBOAKJDNSCYHLVZUMWG","EMUFPHZLRAXYSDJKNGIVTQBWCO",
 "EMYUQVFTGPHJWIZXLDKRBACNSO","RAPCYOKNSEBIUFGHLTWDQMVJXZ","JVIDFENMCGAHRLSYQUKTXZBPWO",
 "ETDFNLRHAUQMIGSPKOZVWYJBCX","XCBJYWVZOKPSGIMQUAHRLNFDTE",
 "SORQLUCIDMEYHAWFTPNGVBJKXZ","ZXKJBVGNPTFWAHYEMDICULQROS","BETWNSULHADIGCOFQJKMPRVXYZ","ZYXVRPMKJQFOCGIDAHLUSNWTEB",
 "ETIANMSURWDKGOHVFLPJBXCYZQ","QZYCXBJPLFVHOGKDWRUSMNAITE",
 "ABCDEFGHIJKLMNOPQRSTUVWXYZ","KRYPTOSABCDEFGHIJLMNQUVWXZ"};

static int PERM[M][26];       /* perm[letter]=pos */
static void build_perms(void){
  for(int a=0;a<M;a++){
    int seen[26]={0},n=0;
    for(int p=0;p<26;p++){int L=ALPHA[a][p]-'A'; PERM[a][L]=p; seen[L]=1; n++;}
    if(n!=26){fprintf(stderr,"ALPHA %d not 26!\n",a);exit(2);}
    for(int L=0;L<26;L++) if(!seen[L]){fprintf(stderr,"ALPHA %s missing %c\n",ANAME[a],'A'+L);exit(2);}
  }
}
static const char*CN[3]={"VIG","BEAU","VAR"};
static const char*SN[2]={"PLAIN","CIPHER"};
static inline int fx(int conv,int t,int g){ if(conv==0)return md(t-g); if(conv==1)return md(g-t); return md(t+g);}

/* decode tout ct avec (conv,src,sig,tau) ; kappa par chaine brute-force ; renvoie erreurs de crib, remplit pt */
static int decode(int conv,int src,const int*ct,const int*sig,const int*tau,int*pt){
  int siginv[26];for(int i=0;i<26;i++)siginv[sig[i]]=i;
  int errtot=0;
  for(int r=0;r<LAG;r++){
    int beste=1e9,bestx[N];
    for(int kr=0;kr<26;kr++){
      int e=0,xloc[N];
      for(int i=r,j=0;i<N;i+=LAG,j++){
        int t=tau[ct[i]];
        int g=(j==0)?kr:((src==0)?xloc[i-LAG]:tau[ct[i-LAG]]);
        int x=fx(conv,t,g); xloc[i]=x;
        if(iscrib[i] && siginv[x]!=cribL[i]) e++;
      }
      if(e<beste){beste=e;for(int i=r;i<N;i+=LAG)bestx[i]=xloc[i];}
    }
    errtot+=beste;
    if(pt) for(int i=r;i<N;i+=LAG) pt[i]=siginv[bestx[i]];
  }
  return errtot;
}

static uint64_t rs;static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}

/* controle positif : fabrique CT avec conv,src,sig,tau,kappa connus (clair anglais + cribs), redecode */
static void encrypt(int conv,int src,const int*sig,const int*tau,const int*kap,const int*pt,int*ct){
  int tauinv[26];for(int i=0;i<26;i++)tauinv[tau[i]]=i;
  int xv[N];for(int i=0;i<N;i++)xv[i]=sig[pt[i]];
  for(int i=0;i<N;i++){
    int g=(i<LAG)?kap[i]:((src==0)?xv[i-LAG]:0);
    if(src==1&&i>=LAG) g=tau[ct[i-LAG]];
    int tval; if(conv==0)tval=md(xv[i]+g); else if(conv==1)tval=md(g-xv[i]); else tval=md(xv[i]-g);
    ct[i]=tauinv[tval];
  }
}

/* min erreurs de crib sur tous les configs (16 sig x 16 tau x 3 conv x 2 src) pour un ct donne */
static int sweep_min(const int*ct){
  int mn=99;
  for(int s=0;s<M;s++)for(int t=0;t<M;t++)for(int c=0;c<3;c++)for(int k=0;k<2;k++){
    int e=decode(c,k,ct,PERM[s],PERM[t],NULL); if(e<mn)mn=e;
  }
  return mn;
}

int main(int argc,char**argv){
  setcribs(); build_perms();
  int ct[N]; for(int i=0;i<N;i++) ct[i]=K4[i]-'A';

  if(argc>=2&&!strcmp(argv[1],"selftest")){
    int ok=1;
    for(int conv=0;conv<3;conv++)for(int src=0;src<2;src++){
      rs=1000+conv*7+src;
      int si=0,ti=IAZ; /* sig=read_full_first, tau=AZ (candidat x connu) */
      int kap[LAG];for(int i=0;i<LAG;i++)kap[i]=rnd()%26;
      int pt[N];for(int i=0;i<N;i++)pt[i]=rnd()%26;
      for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
      int cc[N]; encrypt(conv,src,PERM[si],PERM[ti],kap,pt,cc);
      int p2[N];int e=decode(conv,src,cc,PERM[si],PERM[ti],p2);
      int rec=0;for(int i=0;i<N;i++)if(p2[i]==pt[i])rec++;
      /* PLAIN: recouvrement total attendu. CIPHER: 7 tetes de chaine (amorce) non
       * contraintes par les cribs => rec>=N-7 suffit, avec 0 erreur de crib (base 15 S3). */
      int good=(e==0)&&((src==0)?(rec==N):(rec>=N-7)); ok&=good;
      printf("  %s/%s sig=%s tau=%s: err=%d rec=%d/%d %s\n",
        CN[conv],SN[src],ANAME[si],ANAME[ti],e,rec,N,good?"OK":"FAIL");
    }
    printf("selftest %s\n",ok?"PASS":"FAIL"); return ok?0:1;
  }

  if(argc>=2&&!strcmp(argv[1],"run")){
    int tol=argc>2?atoi(argv[2]):2;
    long tested=0,surv=0; int gmin=99;
    for(int s=0;s<M;s++)for(int t=0;t<M;t++)for(int c=0;c<3;c++)for(int k=0;k<2;k++){
      int pt[N]; int e=decode(c,k,ct,PERM[s],PERM[t],pt); tested++;
      if(e<gmin)gmin=e;
      if(e<=tol){surv++;
        printf("HIT err=%d sig=%s tau=%s %s/%s PT=",e,ANAME[s],ANAME[t],CN[c],SN[k]);
        for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');
      }
    }
    fprintf(stderr,"tested=%ld survivors(<=%d)=%ld  min_crib_errors(K4)=%d\n",tested,tol,surv,gmin);
    return 0;
  }

  if(argc>=2&&!strcmp(argv[1],"null")){
    int nwit=argc>2?atoi(argv[2]):2000; rs=argc>3?(uint64_t)atol(argv[3]):0xC0FFEE;
    int k4min=sweep_min(ct);
    /* multiset de K4 */
    int bag[N];for(int i=0;i<N;i++)bag[i]=ct[i];
    int le=0,eq=0; int hist[25]={0};
    for(int w=0;w<nwit;w++){
      int sh[N];for(int i=0;i<N;i++)sh[i]=bag[i];
      for(int i=N-1;i>0;i--){int j=rnd()%(i+1);int tmp=sh[i];sh[i]=sh[j];sh[j]=tmp;}
      int m=sweep_min(sh); if(m<25)hist[m]++; if(m<=k4min)le++; if(m==k4min)eq++;
    }
    printf("K4 min_crib_errors = %d (sur %dx%dx3x2=%d configs)\n",k4min,M,M,M*M*6);
    printf("temoins K4-melange = %d ; P(temoin_min <= K4_min) = %d/%d = %.4f\n",nwit,le,nwit,(double)le/nwit);
    printf("dont == : %d\n",eq);
    printf("histogramme min-erreurs temoins:\n");
    for(int m=0;m<25;m++) if(hist[m]) printf("  %2d : %d\n",m,hist[m]);
    return 0;
  }

  fprintf(stderr,"usage: selftest | run [tol] | null [nwit] [seed]\n"); return 1;
}
