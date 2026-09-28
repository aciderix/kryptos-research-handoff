/* ac7.c — autoclé sur le clair à l'écart 7, forme Vigenère, DEUX alphabets libres (T36b),
 * attaque par recuit sur les 97 lettres + fabrique de contrôles positifs. (audit autocle_recuit, 26/09/2026)
 *
 * Modèle (T36b, VIG) :  tau(c_i) = sigma(p_i) + sigma(k_i) + d   avec
 *   k_i = p_{i-7}          pour i >= 7   (autoclé sur le clair, écart 7)
 *   k_i = amorce_i          pour i < 7    (7 lettres d'amorce)
 * sigma : valeur d'une lettre du CLAIR (permutation).  tau : valeur d'une lettre du CHIFFRÉ (permutation, indépendante).
 * d est absorbé dans tau ; on le fixe à 0 (tau libre le couvre).  Cas T36 (un seul alphabet) : forcer tau = sigma (option -1).
 *
 * Réduction utilisée par l'attaque : x_i = sigma(p_i) ne dépend que de (tau, amorce) :
 *     x_i = tau(c_i) - x_{i-7}   (i>=7),   x_i = tau(c_i) - kappa_i  (i<7),   kappa_i = sigma(amorce_i).
 *   Le clair est p_i = sigma^{-1}(x_i) : sigma n'est qu'un ré-étiquetage monoalphabétique de la suite x.
 *
 * Sous-commandes :
 *   ac7 selftest                          round-trip chiffrement/déchiffrement
 *   ac7 gen  SEED  < clair(97 A-Z)         -> imprime  CT  sigma tau kappa (paramètres vrais)
 *   ac7 attack qg.bin CT iters restarts seed [Wc] [tie=sigma|indep] [truePT]
 *        recuit conjoint sur (sigma, tau, kappa) noté par quadrigrammes + cribs souples.
 *        si truePT donné : imprime aussi le score de la VRAIE solution et le %lettres retrouvées.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#define N 97
#define LAG 7
static const int CE[13] = {21,22,23,24,25,26,27,28,29,30,31,32,33};   /* EASTNORTHEAST */
static const int CB[11] = {63,64,65,66,67,68,69,70,71,72,73};          /* BERLINCLOCK   */
static int iscrib[N], cribL[N], ncrib;
static void setcribs(void){
  const char *e="EASTNORTHEAST", *b="BERLINCLOCK"; ncrib=0;
  for(int i=0;i<13;i++){int p=CE[i]; iscrib[p]=1; cribL[p]=e[i]-'A'; ncrib++;}
  for(int i=0;i<11;i++){int p=CB[i]; iscrib[p]=1; cribL[p]=b[i]-'A'; ncrib++;}
}
static inline int md(int x){ x%=26; return x<0?x+26:x; }

static uint64_t rs;
static inline uint64_t rnd(void){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return rs; }
static inline double urand(void){ return (rnd()>>11)*(1.0/9007199254740992.0); }
static void shuffle(int *s){ for(int i=0;i<26;i++) s[i]=i; for(int i=25;i>0;i--){int j=rnd()%(i+1); int t=s[i]; s[i]=s[j]; s[j]=t;} }

/* ---- chiffrement (génère un contrôle positif) ---- */
/* sigma[letter]=value ; tau[letter]=value ; kappa[0..6]=sigma-values de l'amorce ; pt[0..96] lettres 0..25 */
static void encrypt(const int *pt, const int *sigma, const int *tauinv, const int *kappa, int *ct){
  int x[N];
  for(int i=0;i<N;i++){
    int k = (i<LAG)? kappa[i] : x[i-LAG];
    x[i] = sigma[pt[i]];
    int val = md(x[i] + k);
    ct[i] = tauinv[val];           /* tauinv[value]=letter */
  }
}
/* ---- déchiffrement dans l'espace des valeurs ---- */
static void decrypt_x(const int *ct, const int *tau, const int *kappa, int *x){
  for(int i=0;i<N;i++){
    int t = tau[ct[i]];
    x[i] = md(t - ((i<LAG)? kappa[i] : x[i-LAG]));
  }
}

/* ---- quadrigrammes ---- */
static float *QG;
static double qscore(const int *p){ double s=0; for(int i=0;i+3<N;i++) s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]]; return s; }

/* état d'attaque */
typedef struct { int sig[26], siginv[26], tau[26], kappa[LAG]; } St;
static int TIE=0; /* 1 => tau lié à sigma (cas T36) */

static void st_decrypt(const St *S, const int *ct, int *pt){
  int x[N]; decrypt_x(ct, S->tau, S->kappa, x);
  for(int i=0;i<N;i++) pt[i]=S->siginv[x[i]];
}
static double st_score(const St *S, const int *ct, double Wc, int *nmatch){
  int pt[N]; st_decrypt(S,ct,pt);
  double q=qscore(pt);
  int m=0; for(int t=0;t<N;t++) if(iscrib[t] && pt[t]==cribL[t]) m++;
  if(nmatch)*nmatch=m;
  return q + Wc*m;
}

static int frac_recovered(const St *S, const int *ct, const int *truePT){
  int pt[N]; st_decrypt(S,ct,pt); int m=0; for(int i=0;i<N;i++) if(pt[i]==truePT[i]) m++; return m;
}

int main(int argc,char**argv){
  setcribs();
  if(argc>=2 && !strcmp(argv[1],"selftest")){
    rs=12345; int sig[26],sigi[26],tau[26],taui[26],kap[LAG],pt[N],ct[N];
    shuffle(sig); for(int i=0;i<26;i++) sigi[sig[i]]=i;
    shuffle(tau); for(int i=0;i<26;i++) taui[tau[i]]=i;
    for(int i=0;i<LAG;i++) kap[i]=rnd()%26;
    for(int i=0;i<N;i++) pt[i]=rnd()%26;
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]); pt[p]=cribL[p];}
    encrypt(pt,sig,taui,kap,ct);
    int x[N]; decrypt_x(ct,tau,kap,x); int ok=1; for(int i=0;i<N;i++) if(sigi[x[i]]!=pt[i]) ok=0;
    printf("selftest %s\n", ok?"OK":"FAIL"); return ok?0:1;
  }
  if(argc>=3 && !strcmp(argv[1],"gen")){
    rs=0x9E3779B97F4A7C15ULL*(atol(argv[2])+1);
    char buf[256]; if(!fgets(buf,sizeof buf,stdin)){fprintf(stderr,"no plaintext\n");return 1;}
    int pt[N]; int n=0; for(char*c=buf;*c&&n<N;c++){int u=*c; if(u>='a'&&u<='z')u-=32; if(u>='A'&&u<='Z') pt[n++]=u-'A';}
    if(n<N){fprintf(stderr,"plaintext too short (%d)\n",n);return 1;}
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]); pt[p]=cribL[p];}
    int sig[26],taui[26],tau[26],kap[LAG];
    shuffle(sig); shuffle(tau);
    if(argc>3 && !strcmp(argv[3],"tie")){ memcpy(tau,sig,sizeof tau); }  /* T36 : un seul alphabet, tau=sigma */
    for(int i=0;i<26;i++) taui[tau[i]]=i;
    for(int i=0;i<LAG;i++) kap[i]=rnd()%26;
    int ct[N]; encrypt(pt,sig,taui,kap,ct);
    for(int i=0;i<N;i++) putchar('A'+ct[i]); putchar(' ');
    printf("sigma="); for(int i=0;i<26;i++) putchar('A'+sig[i]); printf(" tau=");
    for(int i=0;i<26;i++) putchar('A'+tau[i]); printf(" kappa=");
    for(int i=0;i<LAG;i++) printf("%d,",kap[i]);
    printf(" PT="); for(int i=0;i<N;i++) putchar('A'+pt[i]); putchar('\n');
    return 0;
  }
  if(argc>=7 && !strcmp(argv[1],"score")){
    FILE*f=fopen(argv[2],"rb"); QG=malloc(sizeof(float)*456976);
    if(fread(QG,sizeof(float),456976,f)!=456976){fprintf(stderr,"qg read\n");return 2;} fclose(f);
    const char*cts=argv[3]; int ct[N]; for(int i=0;i<N;i++) ct[i]=cts[i]-'A';
    St S; const char*ss=argv[4],*ts=argv[5]; const char*ks=argv[6];
    for(int i=0;i<26;i++){ S.sig[i]=ss[i]-'A'; S.tau[i]=ts[i]-'A'; }
    for(int i=0;i<26;i++) S.siginv[S.sig[i]]=i;
    { int i=0; char kb[128]; strncpy(kb,ks,127); kb[127]=0; for(char*tk=strtok(kb,",");tk&&i<LAG;tk=strtok(NULL,",")) S.kappa[i++]=atoi(tk); }
    int pt[N]; st_decrypt(&S,ct,pt); int m; st_score(&S,ct,0,&m);
    double qo=0; int no=0; for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d]; if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]]; no++;}}
    printf("SCORE cribs=%d/%d qoff=%.4f PT=",m,ncrib,no?qo/no:0); for(int i=0;i<N;i++) putchar('A'+pt[i]); putchar('\n');
    return 0;
  }
  if(argc>=6 && !strcmp(argv[1],"attack")){
    FILE*f=fopen(argv[2],"rb"); QG=malloc(sizeof(float)*456976);
    if(fread(QG,sizeof(float),456976,f)!=456976){fprintf(stderr,"qg read\n");return 2;} fclose(f);
    const char*cts=argv[3]; int ct[N]; for(int i=0;i<N;i++) ct[i]=cts[i]-'A';
    long iters=atol(argv[4]); int R=atoi(argv[5]);
    rs=0x9E3779B97F4A7C15ULL*(atol(argv[6])+1);
    double Wc = argc>7?atof(argv[7]):8.0;
    if(argc>8 && !strcmp(argv[8],"tie")) TIE=1;
    int truePT[N]; int hasTrue=0; if(argc>9){const char*tp=argv[9]; for(int i=0;i<N;i++) truePT[i]=tp[i]-'A'; hasTrue=1;}
    double T0 = getenv("T0")?atof(getenv("T0")):5.0, T1=getenv("T1")?atof(getenv("T1")):0.15;
    const char *SS=getenv("SEED_SIGMA"),*ST=getenv("SEED_TAU"),*SK=getenv("SEED_KAPPA");
    St best; double bestsc=-1e18; int bestm=0;
    for(int r=0;r<R;r++){
      St S; shuffle(S.sig); for(int i=0;i<26;i++) S.siginv[S.sig[i]]=i;
      shuffle(S.tau); if(TIE){ memcpy(S.tau,S.sig,sizeof S.tau);}   /* tie: tau=sigma */
      for(int i=0;i<LAG;i++) S.kappa[i]=rnd()%26;
      if(r==0 && SS && ST && SK){   /* graine depuis la vérité: sonde d'identifiabilité */
        for(int i=0;i<26;i++){ S.sig[i]=SS[i]-'A'; S.tau[i]=ST[i]-'A'; } for(int i=0;i<26;i++) S.siginv[S.sig[i]]=i;
        int i=0; char kb[128]; strncpy(kb,SK,127); kb[127]=0; for(char*tk=strtok(kb,",");tk&&i<LAG;tk=strtok(NULL,",")) S.kappa[i++]=atoi(tk);
        if(TIE){ memcpy(S.tau,S.sig,sizeof S.tau);} }
      int m; double cur=st_score(&S,ct,Wc,&m);
      St bs=S; double bsc=cur;
      for(long it=0; it<iters; it++){
        double T=T0*pow(T1/T0,(double)it/iters);
        St X=S; int mv=rnd()%10;
        if(mv<7){ int u=rnd()%26,v=rnd()%26; if(u==v) continue;   /* swap sigma */
          int t=X.sig[u]; X.sig[u]=X.sig[v]; X.sig[v]=t; X.siginv[X.sig[u]]=u; X.siginv[X.sig[v]]=v;
          if(TIE){ memcpy(X.tau,X.sig,sizeof X.tau);} }
        else if(mv<9 && !TIE){ int u=rnd()%26,v=rnd()%26; if(u==v) continue; /* swap tau */
          int t=X.tau[u]; X.tau[u]=X.tau[v]; X.tau[v]=t; }
        else { X.kappa[rnd()%LAG]=rnd()%26; }  /* tweak kappa */
        double sc=st_score(&X,ct,Wc,&m);
        if(sc>=cur || urand()<exp((sc-cur)/T)){ S=X; cur=sc; if(cur>bsc){bsc=cur; bs=S;} }
      }
      if(bsc>bestsc){ bestsc=bsc; best=bs; st_score(&best,ct,Wc,&bestm); }
    }
    int pt[N]; st_decrypt(&best,ct,pt);
    /* quad score sur les lettres HORS cribs (comparable entre solutions) */
    double qo=0; int no=0;
    for(int i=0;i+3<N;i++){int in=0; for(int d=0;d<4;d++) in|=iscrib[i+d]; if(!in){ qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]]; no++;}}
    printf("BEST score=%.2f cribs=%d/%d qoff=%.4f PT=", bestsc, bestm, ncrib, no?qo/no:0);
    for(int i=0;i<N;i++) putchar('A'+pt[i]);
    if(hasTrue){ int rec=frac_recovered(&best,ct,truePT); printf(" recovered=%d/%d", rec, N); }
    putchar('\n');
    return 0;
  }
  fprintf(stderr,"usage: selftest | gen SEED | attack qg CT iters R seed [Wc] [tie] [truePT]\n");
  return 1;
}
