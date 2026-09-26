/* affine.c — autoclé écart-7 Vigenère (réduction ac7.c) avec σ et τ AFFINES (x->a x+b mod 26).
 * Classe structurée/mémorisable, jamais testée comme ALPHABET de l'autoclé (l'affine standalone
 * est éliminé, mais pas ici). κ déterminé par les cribs (7 classes mod7 toutes couvertes). */
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
static const int INV[26]={0,1,0,9,0,21,0,15,0,3,0,19,0,0,0,7,0,23,0,11,0,5,0,17,0,25}; /*inv mod26*/
/* crib errors : sig[L]=val, tau[L]=val ; kappa dérivé des cribs */
static int crib_errors(const int*ct,const int*sig,const int*tau,int*ptout){
  int kappa[LAG];
  for(int r=0;r<LAG;r++){int req[16],nreq=0,A=0;
    for(int i=r,j=0;i<N;i+=LAG,j++){A=md(tau[ct[i]]-A);
      if(iscrib[i]){int want=sig[cribL[i]]; int kr=md((j&1)?(want-A):(A-want)); req[nreq++]=kr;}}
    int best=0,bestc=-1; for(int a=0;a<nreq;a++){int c=0;for(int b=0;b<nreq;b++)if(req[b]==req[a])c++; if(c>bestc){bestc=c;best=req[a];}}
    kappa[r]=best;}
  int x[N],siginv[26]; for(int i=0;i<26;i++) siginv[sig[i]]=i;
  for(int i=0;i<N;i++){x[i]=md(tau[ct[i]]-((i<LAG)?kappa[i]:x[i-LAG])); ptout[i]=siginv[x[i]];}
  int e=0; for(int i=0;i<N;i++) if(iscrib[i]&&ptout[i]!=cribL[i]) e++; return e;
}
static void affine(int a,int b,int*perm){for(int L=0;L<26;L++) perm[L]=md(a*L+b);}
static uint64_t rs; static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
int main(int argc,char**argv){
  setcribs();
  if(argc>=2&&!strcmp(argv[1],"selftest")){
    rs=7; int sig[26],tau[26],kap[LAG],pt[N],ct[N];
    affine(5,8,sig); affine(9,3,tau);
    for(int i=0;i<LAG;i++)kap[i]=rnd()%26; for(int i=0;i<N;i++)pt[i]=rnd()%26;
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
    int tauinv[26];for(int i=0;i<26;i++)tauinv[tau[i]]=i; int x[N];
    for(int i=0;i<N;i++){int k=(i<LAG)?kap[i]:x[i-LAG]; x[i]=sig[pt[i]]; ct[i]=tauinv[md(x[i]+k)];}
    int pt2[N]; int e=crib_errors(ct,sig,tau,pt2); int rec=0;for(int i=0;i<N;i++)if(pt2[i]==pt[i])rec++;
    printf("selftest err=%d rec=%d/%d -> %s\n",e,rec,N,(e==0&&rec==N)?"OK":"FAIL"); return 0;}
  if(argc>=3&&!strcmp(argv[1],"run")){
    const char*cts=argv[2]; int ct[N]; for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
    int tol=argc>3?atoi(argv[3]):2; long tested=0,surv=0;
    for(int ai=0;ai<12;ai++)for(int bs=0;bs<26;bs++){int sig[26];affine(MUL[ai],bs,sig);
      for(int aj=0;aj<12;aj++)for(int bt=0;bt<26;bt++){int tau[26];affine(MUL[aj],bt,tau);
        int pt[N]; int e=crib_errors(ct,sig,tau,pt); tested++;
        if(e<=tol){surv++; printf("HIT err=%d sig=%dx+%d tau=%dx+%d PT=",e,MUL[ai],bs,MUL[aj],bt);
          for(int i=0;i<N;i++)putchar('A'+pt[i]); putchar('\n');}
      }}
    fprintf(stderr,"tested=%ld survivors(<=%d)=%ld\n",tested,tol,surv); return 0;}
  fprintf(stderr,"usage: selftest | run CT [tol]\n"); return 1;
}
