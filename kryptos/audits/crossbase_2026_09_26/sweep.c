/* sweep.c — autoclé écart-7 Vigenère (modèle T36b de ac7.c), mais σ et τ chacun
 * PROCHES d'une base CONNUE, éventuellement DIFFÉRENTES (jamais testé : agent-2 a
 * couvert σ=τ même base, et deux alphabets proches de A-Z ; ici on croise les bases,
 * ex. σ=KRYPTOS-keyed (plaintext), τ=A-Z (Quagmire-II motivé par Bean)).
 *
 * Réduction (ac7.c) : x_i = tau[ct_i] - (i<7?kappa_i:x_{i-7}) ; pt_i = siginv[x_i].
 * Les 7 classes mod 7 contiennent toutes >=1 crib -> kappa entièrement DÉTERMINÉ par
 * les cribs pour chaque (sigma,tau). Donc chaque (sigma,tau) => clair déterministe +
 * nombre exact d'erreurs de crib. Filtre : erreurs <= tol. (Pas de quadgrammes requis
 * pour le filtre ; on imprime le clair des survivants pour inspection.)
 *
 * Usage :
 *   sweep selftest
 *   sweep gen  baseSig swapSig baseTau swapTau seed   (fabrique un contrôle positif -> CT + params)
 *   sweep run  CT  k_sig k_tau tol                    (balaye les paires de bases, sigma~base±k, tau~base±k)
 */
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
static uint64_t rs; static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}

/* bases connues (alphabets à mot-clé + variantes) */
static const char *KW[]={"","KRYPTOS","PALIMPSEST","ABSCISSA","KRYPTOSABSCISSA"};
static const char *BNAME[]={"AZ","KRYPTOS","PALIMPSEST","ABSCISSA","KRYPTOSABSCISSA","AZrev","KRYPTOSmirror"};
#define NBASE 7
/* construit base b (0..6) dans out[26] : out[i]=lettre i-ème de l'alphabet */
static void mkbase(int b,int*out){
  int seen[26]={0},n=0;
  if(b<=4){ const char*k=KW[b];
    for(const char*p=k;*p;p++){int L=*p-'A'; if(!seen[L]){seen[L]=1;out[n++]=L;}}
    for(int L=0;L<26;L++) if(!seen[L]) out[n++]=L;
  } else if(b==5){ for(int i=0;i<26;i++) out[i]=25-i; }        /* A-Z inversé */
  else { int kr[26]; mkbase(1,kr); for(int i=0;i<26;i++) out[i]=kr[25-i]; } /* KRYPTOS miroir */
}
/* sigma/tau au format "valeur d'une lettre" : perm[letter]=value.
 * Un alphabet 'alpha' (alpha[pos]=letter) définit perm[alpha[pos]]=pos. */
static void alpha_to_perm(const int*alpha,int*perm){for(int i=0;i<26;i++) perm[alpha[i]]=i;}

/* décryptage + erreurs de crib pour (sig,tau), kappa dérivé des cribs par chaîne */
static int crib_errors(const int*ct,const int*sig,const int*tau,int*ptout){
  /* kappa par chaîne : pour chaque résidu r, on prend la valeur qui satisfait le plus de cribs */
  int kappa[LAG]; int errtot=0;
  for(int r=0;r<LAG;r++){
    /* calcule, pour chaque crib de la chaîne, la valeur kappa_r requise */
    int req[16],nreq=0;
    int A=0; /* A_i alternant, recomputé le long de la chaîne */
    for(int i=r,j=0;i<N;i+=LAG,j++){
      A = md(tau[ct[i]] - A);           /* A_i = tau[ct_i] - A_{i-7} (A démarre à 0) */
      if(iscrib[i]){
        /* x_i = A_i + (-1)^{j+1} kappa_r  et x_i doit = sig[cribL_i] */
        int want=sig[cribL[i]];
        int kr = md( ((j&1)? (want-A) : (A-want)) ); /* (-1)^{j+1}: j pair-> -,(A-want); j impair-> +,(want-A) */
        req[nreq++]=kr;
      }
    }
    /* choisir kappa_r = valeur majoritaire ; erreurs = cribs non satisfaits */
    int best=0,bestc=-1;
    for(int a=0;a<nreq;a++){int c=0;for(int bb=0;bb<nreq;bb++) if(req[bb]==req[a])c++; if(c>bestc){bestc=c;best=req[a];}}
    kappa[r]=best; errtot += (nreq-bestc);
  }
  /* décrypte tout le clair avec kappa dérivé */
  int x[N]; int siginv[26]; for(int i=0;i<26;i++) siginv[sig[i]]=i;
  for(int i=0;i<N;i++){ x[i]=md(tau[ct[i]] - ((i<LAG)?kappa[i]:x[i-LAG])); if(ptout)ptout[i]=siginv[x[i]]; }
  /* recompte les erreurs de crib directement (sécurité) */
  int e=0; for(int i=0;i<N;i++) if(iscrib[i] && ptout && ptout[i]!=cribL[i]) e++;
  return ptout? e : errtot;
}

/* génère les permutations proches d'une base : la base (k=0) puis tous les k swaps.
 * Ici on énumère à la volée dans run(). */

static int CTg[N];

int main(int argc,char**argv){
  setcribs();
  if(argc>=2&&!strcmp(argv[1],"selftest")){
    /* fabrique params connus, chiffre, vérifie que crib_errors=0 et clair retrouvé */
    rs=999; int alS[26],alT[26],sig[26],tau[26],kap[LAG],pt[N],ct[N];
    mkbase(1,alS); mkbase(0,alT); alpha_to_perm(alS,sig); alpha_to_perm(alT,tau);
    for(int i=0;i<LAG;i++) kap[i]=rnd()%26;
    for(int i=0;i<N;i++) pt[i]=rnd()%26;
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
    /* chiffre: val=sig[pt_i]+ (i<7?kap:val_{i-7}); ct=tauinv[val] */
    int tauinv[26]; for(int i=0;i<26;i++) tauinv[tau[i]]=i;
    int x[N]; for(int i=0;i<N;i++){int k=(i<LAG)?kap[i]:x[i-LAG]; x[i]=sig[pt[i]]; ct[i]=tauinv[md(x[i]+k)];}
    int pt2[N]; int e=crib_errors(ct,sig,tau,pt2);
    int rec=0; for(int i=0;i<N;i++) if(pt2[i]==pt[i]) rec++;
    printf("selftest crib_errors=%d recovered=%d/%d -> %s\n",e,rec,N,(e==0&&rec==N)?"OK":"FAIL");
    return (e==0&&rec==N)?0:1;
  }
  if(argc>=7&&!strcmp(argv[1],"gen")){
    int bS=atoi(argv[2]),sS=atoi(argv[3]),bT=atoi(argv[4]),sT=atoi(argv[5]); rs=0x9E37+atol(argv[6]);
    int alS[26],alT[26]; mkbase(bS,alS); mkbase(bT,alT);
    for(int s=0;s<sS;s++){int u=rnd()%26,v=rnd()%26;int t=alS[u];alS[u]=alS[v];alS[v]=t;}
    for(int s=0;s<sT;s++){int u=rnd()%26,v=rnd()%26;int t=alT[u];alT[u]=alT[v];alT[v]=t;}
    int sig[26],tau[26]; alpha_to_perm(alS,sig); alpha_to_perm(alT,tau);
    int kap[LAG]; for(int i=0;i<LAG;i++) kap[i]=rnd()%26;
    /* clair anglais bidon + cribs */
    const char*eng="THEQUICKBROWNFOXJUMPSOVERTHELAZYDOGANDTHENSOMEMOREFILLERTEXTTOREACHNINETYSEVENLETTERSEXACTLYOKAYNOW";
    int pt[N]; for(int i=0;i<N;i++) pt[i]=eng[i%94? i:0]-'A'; for(int i=0;i<N;i++) pt[i]=eng[i]-'A';
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
    int tauinv[26]; for(int i=0;i<26;i++) tauinv[tau[i]]=i; int x[N],ct[N];
    for(int i=0;i<N;i++){int k=(i<LAG)?kap[i]:x[i-LAG]; x[i]=sig[pt[i]]; ct[i]=tauinv[md(x[i]+k)];}
    for(int i=0;i<N;i++) putchar('A'+ct[i]); putchar('\n'); return 0;
  }
  if(argc>=6&&!strcmp(argv[1],"run")){
    const char*cts=argv[2]; for(int i=0;i<N;i++) CTg[i]=cts[i]-'A';
    int kS=atoi(argv[3]),kT=atoi(argv[4]),tol=atoi(argv[5]);
    long tested=0,surv=0;
    /* pour chaque paire de bases, énumère sigma~baseS (0..kS swaps) x tau~baseT (0..kT swaps) */
    for(int bS=0;bS<NBASE;bS++) for(int bT=0;bT<NBASE;bT++){
      int baseS[26],baseT[26]; mkbase(bS,baseS); mkbase(bT,baseT);
      /* liste des alphabets sigma : k=0 (base) + tous swaps simples (k=1) [+ k=2 si kS>=2] */
      /* on génère à la volée par indices de swap */
      for(int sa=-1; sa<26; sa++) for(int sb=(sa<0?-1:sa+1); sb<26; sb++){
        if(sa<0 && sb>=0) continue;         /* (-1,-1)=k0 ; (a,b)=k1 */
        if(kS<1 && sa>=0) break;
        int alS[26]; memcpy(alS,baseS,sizeof alS);
        if(sa>=0){int t=alS[sa];alS[sa]=alS[sb];alS[sb]=t;}
        int sig[26]; alpha_to_perm(alS,sig);
        for(int ta=-1; ta<26; ta++) for(int tb=(ta<0?-1:ta+1); tb<26; tb++){
          if(ta<0 && tb>=0) continue;
          if(kT<1 && ta>=0) break;
          int alT[26]; memcpy(alT,baseT,sizeof alT);
          if(ta>=0){int t=alT[ta];alT[ta]=alT[tb];alT[tb]=t;}
          int tau[26]; alpha_to_perm(alT,tau);
          int pt[N]; int e=crib_errors(CTg,sig,tau,pt);
          tested++;
          if(e<=tol){ surv++;
            printf("HIT err=%d sig=%s(k%d) tau=%s(k%d) PT=",e,BNAME[bS],sa<0?0:1,BNAME[bT],ta<0?0:1);
            for(int i=0;i<N;i++) putchar('A'+pt[i]); putchar('\n');
          }
        }
      }
    }
    fprintf(stderr,"tested=%ld survivors(<=%d errs)=%ld\n",tested,tol,surv);
    return 0;
  }
  fprintf(stderr,"usage: selftest | gen bS sS bT sT seed | run CT kSig kTau tol\n"); return 1;
}
