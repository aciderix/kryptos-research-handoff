/* ac7f.c — autoclé écart-7, DEUX alphabets, recherche FACTORISÉE + gate cribs.
 *
 * Constat (this session) : le recuit JOINT (ac7) sur (sigma,tau,kappa) est trop
 * faible — même un contrôle positif KNOWN-autoclé n'est pas retrouvé (34/97 à
 * 450M iters). L'objectif qg_big est PARFAIT (vérité = -1.77, optimum net) : le
 * problème est la NAVIGATION. Ici on factorise :
 *
 *   Modèle : x_i = tau[c_i] - k_i  (k_i = kappa_i si i<7, sinon x_{i-7}) ;  pt_i = siginv[x_i].
 *   => (tau,kappa) DÉTERMINENT la suite de valeurs x. pt = relabel monoalphabétique de x.
 *   => l'intérieur (choisir siginv) = simple substitution mono, RÉSOLUBLE de façon fiable.
 *   => les cribs FIXENT siginv sur {x_i : i crib} -> crib_letter : GATE DUR (conflit => rejet),
 *      ce qui élague massivement l'espace (tau,kappa).
 *
 * Extérieur : recuit sur (tau,kappa), score = qg du meilleur siginv (mono-solve interne),
 *   avec pénalité forte sur les conflits de cribs. Cribs = contrainte DURE, pas soft.
 *
 * Compile: cc -O2 -o ac7f ac7f.c -lm
 * Usage:   ac7f selftest
 *          ac7f attack qg.bin CT outer_iters restarts seed [truePT]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define N 97
#define LAG 7
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N], cribL[N], ncrib;
static void setcribs(void){ const char*e="EASTNORTHEAST",*b="BERLINCLOCK"; ncrib=0;
  for(int i=0;i<13;i++){int p=CE[i]; iscrib[p]=1; cribL[p]=e[i]-'A'; ncrib++;}
  for(int i=0;i<11;i++){int p=CB[i]; iscrib[p]=1; cribL[p]=b[i]-'A'; ncrib++;} }
static inline int md(int x){ x%=26; return x<0?x+26:x; }

static uint64_t rs;
static inline uint64_t rnd(void){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return rs; }
static inline double urand(void){ return (rnd()>>11)*(1.0/9007199254740992.0); }
static void shuffle(int*s,int n){ for(int i=0;i<n;i++)s[i]=i; for(int i=n-1;i>0;i--){int j=rnd()%(i+1);int t=s[i];s[i]=s[j];s[j]=t;} }

static float *QG;
/* fréquence anglaise décroissante (pour amorcer le mono-solve) */
static const char *EFREQ="ETAOINSHRDLCUMWFGYPBVKJXQZ";

/* --- calcule la suite x[] depuis (tau,kappa) --- */
static void calc_x(const int*ct,const int*tau,const int*kappa,int*x){
  for(int i=0;i<N;i++){ int t=tau[ct[i]]; x[i]=md(t-((i<LAG)?kappa[i]:x[i-LAG])); }
}

/* --- cribs GREEDY sans conflit : construit req[value]=letter (sous-ensemble
       cohérent, first-come), renvoie le nb de cribs NON satisfiables par req --- */
static int crib_req(const int*x,int*req){
  int usedL[26]; for(int v=0;v<26;v++)req[v]=-1; for(int l=0;l<26;l++)usedL[l]=-1;
  int skipped=0;
  for(int i=0;i<N;i++) if(iscrib[i]){
    int v=x[i], l=cribL[i];
    if(req[v]==-1 && usedL[l]==-1){ req[v]=l; usedL[l]=v; }
    else if(req[v]==l){ /* déjà cohérent */ }
    else skipped++;  /* ce crib ne peut pas être fixé sans casser req -> soft */
  }
  return skipped;
}

/* qg d'une permutation pt donnée par siginv appliqué à x */
static double qg_of(const int*x,const int*siginv){
  int pt[N]; for(int i=0;i<N;i++)pt[i]=siginv[x[i]];
  double s=0; for(int i=0;i+3<N;i++) s+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];
  return s;
}

/* mono-solve interne : siginv[value]=letter, valeurs crib-fixées gelées.
   amorce par fréquence, puis hill-climb SA sur swaps de valeurs LIBRES. */
static double mono_solve(const int*x,const int*req,int iters,int*out_siginv){
  /* fréquence des valeurs */
  int cnt[26]={0}; for(int i=0;i<N;i++)cnt[x[i]]++;
  int siginv[26]; int letterUsed[26]={0};
  int freeVals[26],nf=0;
  for(int v=0;v<26;v++){ if(req[v]>=0){ siginv[v]=req[v]; letterUsed[req[v]]=1; } else siginv[v]=-1; }
  /* ordonne valeurs libres par fréquence desc */
  int order[26],no=0; for(int v=0;v<26;v++) if(siginv[v]==-1) order[no++]=v;
  for(int a=0;a<no;a++)for(int b=a+1;b<no;b++) if(cnt[order[b]]>cnt[order[a]]){int t=order[a];order[a]=order[b];order[b]=t;}
  /* lettres libres par fréquence anglaise */
  int freeLet[26],nl=0; for(int k=0;k<26;k++){int l=EFREQ[k]-'A'; if(!letterUsed[l]) freeLet[nl++]=l;}
  for(int a=0;a<no;a++){ siginv[order[a]]=freeLet[a]; }
  for(int v=0;v<26;v++) if(siginv[v]==-1){ /* sécurité */ for(int l=0;l<26;l++){int ok=1;for(int w=0;w<26;w++)if(siginv[w]==l)ok=0; if(ok){siginv[v]=l;break;}} }
  /* liste des valeurs libres (siginv modifiable) */
  for(int v=0;v<26;v++) if(req[v]<0) freeVals[nf++]=v;
  double cur=qg_of(x,siginv), best=cur; int bestsig[26]; memcpy(bestsig,siginv,sizeof bestsig);
  double T0=3.0,T1=0.05;
  for(int it=0; it<iters; it++){
    if(nf<2) break;
    int a=freeVals[rnd()%nf], b=freeVals[rnd()%nf]; if(a==b) continue;
    int t=siginv[a]; siginv[a]=siginv[b]; siginv[b]=t;
    double sc=qg_of(x,siginv);
    double T=T0*pow(T1/T0,(double)it/iters);
    if(sc>=cur || urand()<exp((sc-cur)/T)){ cur=sc; if(cur>best){best=cur; memcpy(bestsig,siginv,sizeof bestsig);} }
    else { int u=siginv[a]; siginv[a]=siginv[b]; siginv[b]=u; }
  }
  if(out_siginv) memcpy(out_siginv,bestsig,sizeof bestsig);
  return best;
}

/* score d'un (tau,kappa) : mono-solve (cribs durs via req) + reward soft cribs.
   La factorisation (mono-solve interne fiable) est TOUJOURS utilisée ;
   les cribs guident en douceur -> paysage lisse, pas de rejet. */
static double WC=6.0;
static double eval(const int*ct,const int*tau,const int*kappa,int inner,int*out_siginv,int*out_conf){
  int x[N]; calc_x(ct,tau,kappa,x);
  int req[26]; int skipped=crib_req(x,req);
  int sig[26]; double q=mono_solve(x,req,inner,sig);
  if(out_siginv) memcpy(out_siginv,sig,sizeof sig);
  int m=0; for(int i=0;i<N;i++) if(iscrib[i] && sig[x[i]]==cribL[i]) m++;
  if(out_conf)*out_conf=ncrib-m;   /* cribs non satisfaits au final */
  return q + WC*m;
}

int main(int argc,char**argv){
  setcribs();
  if(argc>=2 && !strcmp(argv[1],"selftest")){
    /* round trip via ac7-compatible encrypt puis decrypt factorisé */
    rs=42; int sig[26],tau[26],taui[26],kap[LAG],pt[N],ct[N],x[N];
    shuffle(sig,26); shuffle(tau,26); for(int i=0;i<26;i++)taui[tau[i]]=i;
    for(int i=0;i<LAG;i++)kap[i]=rnd()%26; for(int i=0;i<N;i++)pt[i]=rnd()%26;
    /* encrypt: x=sig[pt]; val=x+ k; ct=taui[val] */
    for(int i=0;i<N;i++){ int k=(i<LAG)?kap[i]:x[i-LAG]; x[i]=sig[pt[i]]; ct[i]=taui[md(x[i]+k)]; }
    int x2[N]; calc_x(ct,tau,kap,x2); int ok=1; for(int i=0;i<N;i++) if(x2[i]!=sig[pt[i]])ok=0;
    printf("selftest %s\n",ok?"OK":"FAIL"); return ok?0:1;
  }
  if(argc>=6 && !strcmp(argv[1],"attack")){
    FILE*f=fopen(argv[2],"rb"); QG=malloc(sizeof(float)*456976);
    if(fread(QG,sizeof(float),456976,f)!=456976){fprintf(stderr,"qg\n");return 2;} fclose(f);
    const char*cts=argv[3]; int ct[N]; for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
    long OUT=atol(argv[4]); int R=atoi(argv[5]); rs=0x9E3779B97F4A7C15ULL*(atol(argv[6])+1);
    int INNER=getenv("INNER")?atoi(getenv("INNER")):600;
    WC=getenv("WC")?atof(getenv("WC")):6.0;
    int truePT[N],hasTrue=0; if(argc>7){const char*tp=argv[7];for(int i=0;i<N;i++)truePT[i]=tp[i]-'A';hasTrue=1;}
    double T0=getenv("T0")?atof(getenv("T0")):4.0, T1=getenv("T1")?atof(getenv("T1")):0.1;
    int bestsig_g[26],bestc_g=99; double best_g=-1e18; int besttau_g[26],bestkap_g[LAG];
    for(int r=0;r<R;r++){
      int tau[26],kappa[LAG]; shuffle(tau,26); for(int i=0;i<LAG;i++)kappa[i]=rnd()%26;
      int conf; double cur=eval(ct,tau,kappa,INNER,NULL,&conf);
      int btau[26],bkap[LAG]; memcpy(btau,tau,sizeof btau); memcpy(bkap,kappa,sizeof bkap);
      double bcur=cur; int bconf=conf;
      for(long it=0; it<OUT; it++){
        double T=T0*pow(T1/T0,(double)it/OUT);
        int tau2[26],kap2[LAG]; memcpy(tau2,tau,sizeof tau2); memcpy(kap2,kappa,sizeof kap2);
        int mv=rnd()%10;
        if(mv<8){ int u=rnd()%26,v=rnd()%26; if(u==v)continue; int t=tau2[u];tau2[u]=tau2[v];tau2[v]=t; }
        else { kap2[rnd()%LAG]=rnd()%26; }
        int c2; double sc=eval(ct,tau2,kap2,INNER,NULL,&c2);
        if(sc>=cur || urand()<exp((sc-cur)/T)){ memcpy(tau,tau2,sizeof tau); memcpy(kappa,kap2,sizeof kappa); cur=sc; conf=c2;
          if(cur>bcur){bcur=cur; memcpy(btau,tau,sizeof btau); memcpy(bkap,kappa,sizeof bkap); bconf=conf;} }
      }
      /* re-solve interne long sur le meilleur (tau,kappa) de ce restart */
      { int sig[26],cf; double v=eval(ct,btau,bkap,INNER*6,sig,&cf);
        if(v>best_g){ best_g=v; memcpy(bestsig_g,sig,sizeof bestsig_g); bestc_g=cf; memcpy(besttau_g,btau,sizeof besttau_g); memcpy(bestkap_g,bkap,sizeof bestkap_g);} }
    }
    int x[N]; calc_x(ct,besttau_g,bestkap_g,x); int pt[N]; for(int i=0;i<N;i++)pt[i]=bestsig_g[x[i]];
    double qo=0;int no=0; for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d]; if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
    int m=0; for(int i=0;i<N;i++) if(iscrib[i]&&pt[i]==cribL[i])m++;
    printf("BEST qg=%.2f cribs=%d/%d conf=%d qoff=%.4f PT=",best_g,m,ncrib,bestc_g,no?qo/no:0);
    for(int i=0;i<N;i++)putchar('A'+pt[i]);
    if(hasTrue){int rec=0;for(int i=0;i<N;i++)if(pt[i]==truePT[i])rec++; printf(" recovered=%d/%d",rec,N);}
    putchar('\n'); return 0;
  }
  fprintf(stderr,"usage: selftest | attack qg CT outer R seed [truePT]\n"); return 1;
}
