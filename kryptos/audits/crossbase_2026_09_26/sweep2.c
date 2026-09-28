/* sweep2.c — autoclé écart-7 Vigenère, σ et τ = AFFINE ∘ base_keyée.
 * σ[L] = a*keyedval_base(L) + b (mod26). Bases: AZ, KRYPTOS, PALIMPSEST, ABSCISSA.
 * Subsume l'affine pur (base AZ) et le keyed pur (a=1,b=0). κ dérivé des cribs. */
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
/* keyedval[base][L] = position (value) de la lettre L dans l'alphabet keyé */
static int KV[4][26];
static void buildKV(void){for(int b=0;b<4;b++){int seen[26]={0},al[26],n=0;
  for(const char*p=KW[b];*p;p++){int L=*p-'A'; if(!seen[L]){seen[L]=1;al[n++]=L;}}
  for(int L=0;L<26;L++) if(!seen[L]) al[n++]=L;
  for(int i=0;i<26;i++) KV[b][al[i]]=i; }}
/* perm σ[L]=a*KV[base][L]+b */
static void mkperm(int base,int a,int b,int*perm){for(int L=0;L<26;L++) perm[L]=md(a*KV[base][L]+b);}
static int crib_errors(const int*ct,const int*sig,const int*tau,int*ptout){
  int kappa[LAG];
  for(int r=0;r<LAG;r++){int req[16],nreq=0,A=0;
    for(int i=r,j=0;i<N;i+=LAG,j++){A=md(tau[ct[i]]-A);
      if(iscrib[i]){int want=sig[cribL[i]];int kr=md((j&1)?(want-A):(A-want));req[nreq++]=kr;}}
    int best=0,bc=-1;for(int a=0;a<nreq;a++){int c=0;for(int b=0;b<nreq;b++)if(req[b]==req[a])c++;if(c>bc){bc=c;best=req[a];}}
    kappa[r]=best;}
  int x[N],si[26];for(int i=0;i<26;i++)si[sig[i]]=i;
  for(int i=0;i<N;i++){x[i]=md(tau[ct[i]]-((i<LAG)?kappa[i]:x[i-LAG]));ptout[i]=si[x[i]];}
  int e=0;for(int i=0;i<N;i++)if(iscrib[i]&&ptout[i]!=cribL[i])e++;return e;}
static uint64_t rs;static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
int main(int argc,char**argv){
  setcribs();buildKV();
  if(argc>=2&&!strcmp(argv[1],"selftest")){rs=3;int sig[26],tau[26],kap[LAG],pt[N],ct[N];
    mkperm(1,7,4,sig);mkperm(2,3,9,tau); /* σ=affine∘KRYPTOS, τ=affine∘PALIMPSEST */
    for(int i=0;i<LAG;i++)kap[i]=rnd()%26;for(int i=0;i<N;i++)pt[i]=rnd()%26;
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
    int ti[26];for(int i=0;i<26;i++)ti[tau[i]]=i;int x[N];
    for(int i=0;i<N;i++){int k=(i<LAG)?kap[i]:x[i-LAG];x[i]=sig[pt[i]];ct[i]=ti[md(x[i]+k)];}
    int p2[N];int e=crib_errors(ct,sig,tau,p2);int rec=0;for(int i=0;i<N;i++)if(p2[i]==pt[i])rec++;
    printf("selftest err=%d rec=%d/%d -> %s\n",e,rec,N,(e==0&&rec==N)?"OK":"FAIL");return 0;}
  if(argc>=3&&!strcmp(argv[1],"run")){const char*cts=argv[2];int ct[N];for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
    int tol=argc>3?atoi(argv[3]):2;long tested=0,surv=0;
    for(int bs=0;bs<4;bs++)for(int ai=0;ai<12;ai++)for(int cs=0;cs<26;cs++){int sig[26];mkperm(bs,MUL[ai],cs,sig);
      for(int bt=0;bt<4;bt++)for(int aj=0;aj<12;aj++)for(int ctb=0;ctb<26;ctb++){int tau[26];mkperm(bt,MUL[aj],ctb,tau);
        int pt[N];int e=crib_errors(ct,sig,tau,pt);tested++;
        if(e<=tol){surv++;printf("HIT err=%d sig=%s*%dx+%d tau=%s*%dx+%d PT=",e,BN[bs],MUL[ai],cs,BN[bt],MUL[aj],ctb);
          for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');}}}
    fprintf(stderr,"tested=%ld survivors(<=%d)=%ld\n",tested,tol,surv);return 0;}
  fprintf(stderr,"usage: selftest | run CT [tol]\n");return 1;}
