/* ac7g.c — autoclé σ=τ (UN seul alphabet, dim~9) — recherche FIABLE, multi-formes.
 *
 * Pourquoi ce front (débloqué par qg_big) : la contradiction σ=τ (base 9) qui tuait
 * ce cas ne vaut QUE pour la forme Vigenère (c=p+k). En Beaufort (c=k-p) et variante
 * (c=p-k), positions 32/73 donnent 2σ(S)=σ(N), 2σ(K)=σ(L) : PAS de contradiction.
 * σ=τ = UN alphabet (dim~9) => la recherche est FIABLE ; tout négatif y est CONCLUSIF
 * (contrairement à dim-18 des 2 alphabets). 100% public (aucun K5/Paradigm).
 *
 * Formes (chiffrement) : 0 Vig c=p+k | 1 Beaufort c=k-p | 2 var c=p-k
 * Source autoclé : P = clair p_{i-lag} | C = chiffré c_{i-lag} ; amorce = lag valeurs.
 *
 * Compile: cc -O2 -o ac7g ac7g.c -lm
 * Usage: ac7g selftest
 *        ac7g gen  form src lag seed  < clair(97)      -> CT ...
 *        ac7g attack qg.bin CT form src lag iters R seed [WC] [truePT]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define N 97
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N],ncrib;
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";ncrib=0;
 for(int i=0;i<13;i++){int p=CE[i];iscrib[p]=1;cribL[p]=e[i]-'A';ncrib++;}
 for(int i=0;i<11;i++){int p=CB[i];iscrib[p]=1;cribL[p]=b[i]-'A';ncrib++;}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static uint64_t rs;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (rnd()>>11)*(1.0/9007199254740992.0);}
static void shuffle(int*s,int n){for(int i=0;i<n;i++)s[i]=i;for(int i=n-1;i>0;i--){int j=rnd()%(i+1);int t=s[i];s[i]=s[j];s[j]=t;}}
static float*QG;

/* encrypt: renvoie ct (valeurs) ; sv=perm valeur, svinv=inverse ; pt valeurs ; seeds valeurs amorce */
static void enc(const int*pt,const int*sv,const int*svinv,const int*seeds,int form,int src,int lag,int*ctv){
  for(int i=0;i<N;i++){
    int kval;
    if(i<lag) kval=seeds[i];
    else kval = (src==0)? sv[pt[i-lag]] : sv[ctv[i-lag]]; /* ctv stores LETTER-values? we store ct letter idx */
    int pv=sv[pt[i]], cv;
    if(form==0) cv=md(pv+kval); else if(form==1) cv=md(kval-pv); else cv=md(pv-kval);
    ctv[i]=svinv[cv]; /* ct letter index */
  }
}
/* decrypt: given sv (letter->value), svinv, seeds(values), ct(letter idx) -> pt(letter idx) */
static void dec(const int*ct,const int*sv,const int*svinv,const int*seeds,int form,int src,int lag,int*pt){
  int pvv[N];
  for(int i=0;i<N;i++){
    int kval;
    if(i<lag) kval=seeds[i];
    else kval = (src==0)? pvv[i-lag] : sv[ct[i-lag]];
    int cv=sv[ct[i]], pv;
    if(form==0) pv=md(cv-kval); else if(form==1) pv=md(kval-cv); else pv=md(cv+kval);
    pvv[i]=pv; pt[i]=svinv[pv];
  }
}
static double qg_of(const int*pt){double s=0;for(int i=0;i+3<N;i++)s+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];return s;}

int main(int argc,char**argv){
  setcribs();
  if(argc>=2&&!strcmp(argv[1],"selftest")){
    rs=7; int sv[26],svinv[26],pt[N],ct[N],seeds[7],lag=7;
    for(int form=0;form<3;form++)for(int src=0;src<2;src++){
      shuffle(sv,26);for(int i=0;i<26;i++)svinv[sv[i]]=i;
      for(int i=0;i<lag;i++)seeds[i]=rnd()%26; for(int i=0;i<N;i++)pt[i]=rnd()%26;
      enc(pt,sv,svinv,seeds,form,src,lag,ct);
      int pt2[N]; dec(ct,sv,svinv,seeds,form,src,lag,pt2);
      int ok=1;for(int i=0;i<N;i++)if(pt2[i]!=pt[i])ok=0;
      printf("selftest form%d src%d %s\n",form,src,ok?"OK":"FAIL");
    } return 0;
  }
  if(argc>=6&&!strcmp(argv[1],"gen")){
    int form=atoi(argv[2]),src=atoi(argv[3]),lag=atoi(argv[4]); rs=0x9E3779B97F4A7C15ULL*(atol(argv[5])+1);
    char buf[256]; if(!fgets(buf,sizeof buf,stdin))return 1;
    int pt[N],n=0;for(char*c=buf;*c&&n<N;c++){int u=*c;if(u>='a'&&u<='z')u-=32;if(u>='A'&&u<='Z')pt[n++]=u-'A';}
    if(n<N){fprintf(stderr,"short %d\n",n);return 1;}
    for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt[p]=cribL[p];}
    int sv[26],svinv[26],seeds[16]; const char*FS=getenv("FIXSIGMA");
    if(FS&&strlen(FS)>=26){ for(int i=0;i<26;i++) sv[FS[i]-'A']=i; } else shuffle(sv,26);
    for(int i=0;i<26;i++)svinv[sv[i]]=i;
    for(int i=0;i<lag;i++)seeds[i]=rnd()%26; int ct[N]; enc(pt,sv,svinv,seeds,form,src,lag,ct);
    for(int i=0;i<N;i++)putchar('A'+ct[i]); printf(" PT="); for(int i=0;i<N;i++)putchar('A'+pt[i]); putchar('\n'); return 0;
  }
  if(argc>=9&&!strcmp(argv[1],"attack")){
    FILE*f=fopen(argv[2],"rb");QG=malloc(sizeof(float)*456976);if(fread(QG,sizeof(float),456976,f)!=456976)return 2;fclose(f);
    const char*cts=argv[3];int ct[N];for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
    int form=atoi(argv[4]),src=atoi(argv[5]),lag=atoi(argv[6]); long IT=atol(argv[7]); int R=atoi(argv[8]);
    rs=0x9E3779B97F4A7C15ULL*(atol(argv[9])+1);
    double WC=argc>10?atof(argv[10]):6.0;
    int truePT[N],hasT=0; if(argc>11){const char*tp=argv[11];for(int i=0;i<N;i++)truePT[i]=tp[i]-'A';hasT=1;}
    double T0=5.0,T1=0.08;
    /* FIXSIGMA : alphabet σ figé (26 lettres) -> on n'anneal QUE les seeds (test feature visible) */
    int fixsig=0, FSV[26]; const char*FS=getenv("FIXSIGMA");
    if(FS && strlen(FS)>=26){ fixsig=1; for(int i=0;i<26;i++) FSV[FS[i]-'A']=i; }
    int bestsv[26],bestseed[16];double best=-1e18;int bestm=0;
    for(int r=0;r<R;r++){
      int sv[26],svinv[26],seeds[16];
      if(fixsig){ memcpy(sv,FSV,sizeof sv); } else shuffle(sv,26);
      for(int i=0;i<26;i++)svinv[sv[i]]=i;
      for(int i=0;i<lag;i++)seeds[i]=rnd()%26;
      int pt[N];dec(ct,sv,svinv,seeds,form,src,lag,pt);
      int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
      double cur=qg_of(pt)+WC*m;
      int bsv[26],bseed[16];memcpy(bsv,sv,sizeof bsv);memcpy(bseed,seeds,sizeof bseed);double bsc=cur;int bm=m;
      for(long it=0;it<IT;it++){
        double T=T0*pow(T1/T0,(double)it/IT);
        int mv=rnd()%10; int su=0,sv2=0,sd=-1,sod=0;
        if(fixsig) mv=9; /* n'annealer que les seeds */
        if(mv<8){ su=rnd()%26; sv2=rnd()%26; if(su==sv2)continue;
          int t=sv[su];sv[su]=sv[sv2];sv[sv2]=t; svinv[sv[su]]=su; svinv[sv[sv2]]=sv2; }
        else { sd=rnd()%lag; sod=seeds[sd]; seeds[sd]=rnd()%26; }
        dec(ct,sv,svinv,seeds,form,src,lag,pt);
        int mm=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])mm++;
        double sc=qg_of(pt)+WC*mm;
        if(sc>=cur||urand()<exp((sc-cur)/T)){cur=sc; if(cur>bsc){bsc=cur;memcpy(bsv,sv,sizeof bsv);memcpy(bseed,seeds,sizeof bseed);bm=mm;}}
        else { if(mv<8){int t=sv[su];sv[su]=sv[sv2];sv[sv2]=t;svinv[sv[su]]=su;svinv[sv[sv2]]=sv2;} else seeds[sd]=sod; }
      }
      if(bsc>best){best=bsc;memcpy(bestsv,bsv,sizeof bestsv);memcpy(bestseed,bseed,sizeof bestseed);bestm=bm;}
    }
    int svinv[26];for(int i=0;i<26;i++)svinv[bestsv[i]]=i; int pt[N];dec(ct,bestsv,svinv,bestseed,form,src,lag,pt);
    double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
    printf("BEST f%d s%d L%d qg=%.1f cribs=%d/%d qoff=%.4f PT=",form,src,lag,best,bestm,ncrib,no?qo/no:0);
    for(int i=0;i<N;i++)putchar('A'+pt[i]);
    if(hasT){int rec=0;for(int i=0;i<N;i++)if(pt[i]==truePT[i])rec++;printf(" recovered=%d/%d",rec,N);}
    putchar('\n'); return 0;
  }
  fprintf(stderr,"usage: selftest | gen form src lag seed | attack qg CT form src lag iters R seed [WC] [truePT]\n");return 1;
}
