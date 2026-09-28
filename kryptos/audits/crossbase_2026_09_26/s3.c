/* s3.c — réinterroge le squelette : conv {VIG,BEAU,VAR} x source-clé {PLAIN,CIPHER},
 * écart 7, alphabets σ,τ = affine∘keyée. κ par chaîne dérivé par bruteforce (26) => robuste.
 * decrypt d'une chaîne mod7 : x_i = f(τ(c_i), g_i) ; g_i = κ_r (j=0) sinon PLAIN:x_{prev} / CIPHER:τ(c_{i-7}).
 * f: VIG x=t-g ; BEAU x=g-t ; VAR x=t+g.  pt_i=σ^{-1}(x_i). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 97
#define LAG 7
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
static inline int fx(int conv,int t,int g){ if(conv==0)return md(t-g); if(conv==1)return md(g-t); return md(t+g);}
/* décrypte tout avec conv,src ; κ par chaîne bruteforcé pour min erreurs de crib. renvoie erreurs, remplit pt */
static int decode(int conv,int src,const int*ct,const int*sig,const int*tau,int*pt){
  int siginv[26];for(int i=0;i<26;i++)siginv[sig[i]]=i;
  int errtot=0;
  for(int r=0;r<LAG;r++){
    int bestk=0,beste=1e9,bestx[N];
    for(int kr=0;kr<26;kr++){
      int e=0,xloc[N];
      for(int i=r,j=0;i<N;i+=LAG,j++){
        int t=tau[ct[i]];
        int g = (j==0)? kr : ((src==0)? xloc[i-LAG] : tau[ct[i-LAG]]);
        int x=fx(conv,t,g); xloc[i]=x;
        if(iscrib[i] && siginv[x]!=cribL[i]) e++;
      }
      if(e<beste){beste=e;bestk=kr; for(int i=r;i<N;i+=LAG) bestx[i]=xloc[i];}
    }
    errtot+=beste;
    for(int i=r;i<N;i+=LAG) pt[i]=siginv[bestx[i]];
    (void)bestk;
  }
  return errtot;
}
static uint64_t rs;static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
/* contrôle positif : fabrique un CT avec conv,src,σ,τ,κ connus, clair anglais+cribs */
static void encrypt(int conv,int src,const int*sig,const int*tau,const int*kap,const int*pt,int*ct){
  int tauinv[26];for(int i=0;i<26;i++)tauinv[tau[i]]=i;
  int xv[N];
  for(int i=0;i<N;i++) xv[i]=sig[pt[i]];
  for(int i=0;i<N;i++){
    int g=(i<LAG)?kap[i]:((src==0)?xv[i-LAG]:0);
    /* pour CIPHER, g dépend de τ(c_{i-7}) => on résout séquentiellement */
    int tval;
    if(src==1 && i>=LAG){ g=tau[ct[i-LAG]]; }
    if(conv==0) tval=md(xv[i]+g); else if(conv==1) tval=md(g-xv[i]); else tval=md(xv[i]-g);
    ct[i]=tauinv[tval];
  }
}
int main(int argc,char**argv){
  setcribs();buildKV();
  if(argc>=2&&!strcmp(argv[1],"selftest")){
    /* teste les 6 (conv,src) : fabrique et redécode, doit donner 0 erreur + clair retrouvé */
    for(int conv=0;conv<3;conv++)for(int src=0;src<2;src++){
      rs=100+conv*7+src; int sig[26],tau[26],kap[LAG],pt[N],ct[N];
      mkperm(1,7,4,sig);mkperm(2,3,9,tau);
      for(int i=0;i<LAG;i++)kap[i]=rnd()%26; for(int i=0;i<N;i++)pt[i]=rnd()%26;
      for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
      encrypt(conv,src,sig,tau,kap,pt,ct);
      int p2[N];int e=decode(conv,src,ct,sig,tau,p2);
      int rec=0;for(int i=0;i<N;i++)if(p2[i]==pt[i])rec++;
      printf("  %s/%s: err=%d rec=%d/%d %s\n",CN[conv],SN[src],e,rec,N,(e==0&&rec==N)?"OK":"(control note)");
    }
    return 0;}
  if(argc>=3&&!strcmp(argv[1],"run")){
    const char*cts=argv[2];int ct[N];for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
    int tol=argc>3?atoi(argv[3]):2; long tested=0,surv=0;
    for(int conv=0;conv<3;conv++)for(int src=0;src<2;src++)
      for(int bs=0;bs<4;bs++)for(int ai=0;ai<12;ai++)for(int cs=0;cs<26;cs++){int sig[26];mkperm(bs,MUL[ai],cs,sig);
        for(int bt=0;bt<4;bt++)for(int aj=0;aj<12;aj++)for(int cb=0;cb<26;cb++){int tau[26];mkperm(bt,MUL[aj],cb,tau);
          int pt[N];int e=decode(conv,src,ct,sig,tau,pt);tested++;
          if(e<=tol){surv++;printf("HIT %s/%s err=%d sig=%s*%dx+%d tau=%s*%dx+%d PT=",CN[conv],SN[src],e,BN[bs],MUL[ai],cs,BN[bt],MUL[aj],cb);
            for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');}
        }}
    fprintf(stderr,"tested=%ld survivors(<=%d)=%ld\n",tested,tol,surv);return 0;}
  fprintf(stderr,"usage: selftest | run CT [tol]\n");return 1;}
