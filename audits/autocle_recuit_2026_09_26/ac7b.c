/* ac7b.c — autoclé écart-7 Vigenère, DEUX alphabets libres (T36b). Version rapide.
 * Correctif clé : sigma n'entre PAS dans la récurrence (ré-étiquetage final).
 *   x_i = tau(c_i) - x_{i-7} (i>=7),  x_i = tau(c_i) - kappa_i (i<7).  p_i = sigma^{-1}(x_i).
 * Recherche sur (tau, kappa) SEULS ; sigma DÉRIVÉE : lettres-crib votées à chaque éval (rapide),
 * lettres hors-crib réglées par quadrigrammes (tune_free) tous les 256 pas. Cribs souples (désaccords tolérés).
 *
 *   ac7b attack qg.bin CT iters restarts seed [Wc] [truePT]
 *   ac7b batch  qg.bin listfile iters restarts seed [Wc]
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
static int clet[16], nclet, cpos_by[16][16], cnt_by[16], iscribletter[26];
static void setcribs(void){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK"; ncrib=0; memset(iscrib,0,sizeof iscrib); memset(iscribletter,0,sizeof iscribletter);
  for(int i=0;i<13;i++){int p=CE[i]; iscrib[p]=1; cribL[p]=e[i]-'A'; ncrib++;}
  for(int i=0;i<11;i++){int p=CB[i]; iscrib[p]=1; cribL[p]=b[i]-'A'; ncrib++;}
  nclet=0;
  for(int p=0;p<N;p++) if(iscrib[p]){ int L=cribL[p],f=-1;
    for(int k=0;k<nclet;k++) if(clet[k]==L){f=k;break;}
    if(f<0){f=nclet; clet[nclet]=L; cnt_by[nclet]=0; nclet++; iscribletter[L]=1;}
    cpos_by[f][cnt_by[f]++]=p; }
}
static inline int md(int x){ x%=26; return x<0?x+26:x; }
static uint64_t rs;
static inline uint64_t rnd(void){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return rs; }
static inline double urand(void){ return (rnd()>>11)*(1.0/9007199254740992.0); }
static void shuffle(int*s){ for(int i=0;i<26;i++) s[i]=i; for(int i=25;i>0;i--){int j=rnd()%(i+1); int t=s[i]; s[i]=s[j]; s[j]=t;} }

static float *QG;
static inline double q4(const int*p){ double s=0; for(int i=0;i+3<N;i++) s+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]]; return s; }

typedef struct { int tau[26], kappa[LAG], sig[26], siginv[26]; } St;

static void decode_x(const St*S,const int*ct,int*x){
  for(int i=0;i<N;i++) x[i]=md(S->tau[ct[i]] - ((i<LAG)?S->kappa[i]:x[i-LAG]));
}
/* re-vote crib letters (priority, bump free letters), keep free letters; returns #agreements */
static int refit(St*S,const int*x){
  /* clear crib-letter assignments */
  for(int k=0;k<nclet;k++){ int L=clet[k]; if(S->sig[L]>=0){ S->siginv[S->sig[L]]=-1; S->sig[L]=-1; } }
  int hist[16][26]; memset(hist,0,sizeof hist);
  for(int k=0;k<nclet;k++) for(int t=0;t<cnt_by[k];t++) hist[k][ x[cpos_by[k][t]] ]++;
  int done[16]={0}, agree=0;
  for(int step=0;step<nclet;step++){
    int bk=-1,bv=-1,bc=-1;
    for(int k=0;k<nclet;k++) if(!done[k]) for(int v=0;v<26;v++){
      int holder=S->siginv[v];
      if(holder>=0 && iscribletter[holder]) continue;   /* value held by an already-placed crib letter: forbidden */
      if(hist[k][v]>bc){ bc=hist[k][v]; bk=k; bv=v; }
    }
    if(bk<0) break;
    int holder=S->siginv[bv]; if(holder>=0){ S->sig[holder]=-1; }   /* bump free letter */
    S->sig[clet[bk]]=bv; S->siginv[bv]=clet[bk]; done[bk]=1; agree += bc>0?bc:0;
  }
  /* reassign any unassigned free letters to unused values (canonical) */
  for(int L=0;L<26;L++) if(S->sig[L]<0){ for(int v=0;v<26;v++) if(S->siginv[v]<0){ S->sig[L]=v; S->siginv[v]=L; break; } }
  return agree;
}
static void tune_free(St*S,const int*x){
  int pt[N];
  for(int L=0;L<26;L++) if(!iscribletter[L]){
    int oldv=S->sig[L]; double best=-1e18; int bv=oldv;
    for(int v=0;v<26;v++){ int h=S->siginv[v]; if(h>=0 && h!=L && iscribletter[h]) continue; /* can't take crib-held */
      /* swap L into v (and whoever holds v, a free letter, into oldv) */
      int h2=S->siginv[v];
      S->sig[L]=v; S->siginv[v]=L; if(h2>=0 && h2!=L){ S->sig[h2]=oldv; S->siginv[oldv]=h2; } else if(v!=oldv){ S->siginv[oldv]=-1; }
      for(int i=0;i<N;i++) pt[i]=S->siginv[x[i]]>=0?S->siginv[x[i]]:0;
      double q=q4(pt);
      /* undo */
      S->sig[L]=oldv; S->siginv[oldv]=L; if(h2>=0 && h2!=L){ S->sig[h2]=v; S->siginv[v]=h2; } else if(v!=oldv){ S->siginv[v]=-1; }
      if(q>best){ best=q; bv=v; }
    }
    if(bv!=oldv){ int h2=S->siginv[bv]; S->sig[L]=bv; S->siginv[bv]=L; if(h2>=0){ S->sig[h2]=oldv; S->siginv[oldv]=h2; } else { S->siginv[oldv]=-1; } }
  }
}
static double eval(St*S,const int*ct,double Wc,int*ncr,int*ptout){
  int x[N]; decode_x(S,ct,x); int ag=refit(S,x);
  int pt[N]; for(int i=0;i<N;i++) pt[i]=S->siginv[x[i]];
  if(ncr)*ncr=ag; if(ptout) memcpy(ptout,pt,sizeof pt);
  return q4(pt)+Wc*ag;
}
static double qoff(const int*pt){ double s=0;int n=0; for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d]; if(!in){s+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];n++;}} return n?s/n:0; }

static double run_attack(const int*ct,long iters,int R,double Wc,int*best_ncr,int*best_pt){
  double T0=getenv("T0")?atof(getenv("T0")):5.0, T1=getenv("T1")?atof(getenv("T1")):0.15;
  const char *STAU=getenv("SEED_TAU"),*SKAP=getenv("SEED_KAPPA");
  double bestsc=-1e18; int bpt[N],bncr=0;
  for(int r=0;r<R;r++){
    St S; shuffle(S.tau); for(int i=0;i<LAG;i++) S.kappa[i]=rnd()%26;
    if(STAU && SKAP){   /* graine ancrée-cribs (toutes les reprises repartent près du germe) */
      for(int i=0;i<26;i++) S.tau[i]=STAU[i]-'A';
      int i=0; char kb[128]; strncpy(kb,SKAP,127); kb[127]=0; for(char*tk=strtok(kb,",");tk&&i<LAG;tk=strtok(NULL,",")) S.kappa[i++]=atoi(tk);
    }
    for(int i=0;i<26;i++){ S.sig[i]=-1; S.siginv[i]=-1; }
    int ncr,pt[N]; double cur=eval(&S,ct,Wc,&ncr,pt);
    { int x[N]; decode_x(&S,ct,x); tune_free(&S,x); cur=eval(&S,ct,Wc,&ncr,pt); }
    St bs=S; double bsc=cur; int bspt[N]; memcpy(bspt,pt,sizeof pt); int bsncr=ncr;
    for(long it=0; it<iters; it++){
      double T=T0*pow(T1/T0,(double)it/iters);
      St X=S; int mv=rnd()%10;
      if(mv<8){ int u=rnd()%26,v=rnd()%26; if(u==v) continue; int t=X.tau[u]; X.tau[u]=X.tau[v]; X.tau[v]=t; }
      else { X.kappa[rnd()%LAG]=rnd()%26; }
      if((it&255)==0){ int x[N]; decode_x(&X,ct,x); tune_free(&X,x); }
      double sc=eval(&X,ct,Wc,&ncr,pt);
      if(sc>=cur || urand()<exp((sc-cur)/T)){ S=X; cur=sc; if(cur>bsc){bsc=cur; bs=S; memcpy(bspt,pt,sizeof pt); bsncr=ncr;} }
    }
    { int x[N]; decode_x(&bs,ct,x); tune_free(&bs,x); int ncr2,pt2[N]; double s2=eval(&bs,ct,Wc,&ncr2,pt2); if(s2>bsc){bsc=s2;memcpy(bspt,pt2,sizeof pt2);bsncr=ncr2;} }
    if(bsc>bestsc){ bestsc=bsc; memcpy(bpt,bspt,sizeof bpt); bncr=bsncr; }
  }
  if(best_ncr)*best_ncr=bncr; if(best_pt) memcpy(best_pt,bpt,sizeof bpt);
  return bestsc;
}

int main(int argc,char**argv){
  setcribs();
  if(argc>=6 && !strcmp(argv[1],"attack")){
    FILE*f=fopen(argv[2],"rb"); QG=malloc(sizeof(float)*456976); if(fread(QG,sizeof(float),456976,f)!=456976) return 2; fclose(f);
    const char*cts=argv[3]; int ct[N]; for(int i=0;i<N;i++) ct[i]=cts[i]-'A';
    long iters=atol(argv[4]); int R=atoi(argv[5]); rs=0x9E3779B97F4A7C15ULL*(atol(argv[6])+1);
    double Wc=argc>7?atof(argv[7]):8.0;
    int truePT[N]; int hasTrue=0; if(argc>8){const char*tp=argv[8]; for(int i=0;i<N;i++) truePT[i]=tp[i]-'A'; hasTrue=1;}
    int ncr,pt[N]; double sc=run_attack(ct,iters,R,Wc,&ncr,pt);
    printf("BEST score=%.2f cribs=%d/%d qoff=%.4f PT=",sc,ncr,ncrib,qoff(pt));
    for(int i=0;i<N;i++) putchar('A'+pt[i]);
    if(hasTrue){int rec=0;for(int i=0;i<N;i++)if(pt[i]==truePT[i])rec++; printf(" recovered=%d/%d",rec,N);}
    putchar('\n'); return 0;
  }
  if(argc>=6 && !strcmp(argv[1],"batch")){
    FILE*f=fopen(argv[2],"rb"); QG=malloc(sizeof(float)*456976); if(fread(QG,sizeof(float),456976,f)!=456976) return 2; fclose(f);
    FILE*lf=fopen(argv[3],"r"); long iters=atol(argv[4]); int R=atoi(argv[5]); long seed0=atol(argv[6]);
    double Wc=argc>7?atof(argv[7]):8.0; char line[256]; int idx=0;
    while(fgets(line,sizeof line,lf)){ int ct[N],ok=1; for(int i=0;i<N;i++){ if(line[i]<'A'||line[i]>'Z'){ok=0;break;} ct[i]=line[i]-'A';} if(!ok) continue;
      rs=0x9E3779B97F4A7C15ULL*(seed0+idx+1); int ncr,pt[N]; double sc=run_attack(ct,iters,R,Wc,&ncr,pt);
      printf("%d score=%.2f cribs=%d/%d qoff=%.4f PT=",idx,sc,ncr,ncrib,qoff(pt)); for(int i=0;i<N;i++) putchar('A'+pt[i]); putchar('\n'); fflush(stdout); idx++; }
    return 0;
  }
  fprintf(stderr,"usage: attack qg CT iters R seed [Wc] [truePT] | batch qg list iters R seed [Wc]\n"); return 1;
}
