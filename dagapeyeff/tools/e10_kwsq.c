/* E10 — empreinte d'un carré « mot-clé + alphabet » dans les comptes par case (invariante par transposition).
 * LL(σ) = Σ_c n_c ln p(σ(c)). LL_max : σ quelconque (appariement par rang). LL_K : max sur les 2^25 ensembles de
 * lettres du mot-clé (ordre du mot-clé optimisé par rang ; lettres restantes en ordre alphabétique) et 8 orientations.
 *   e10_kwsq freq.txt real cipher.txt GEO        -> Δ réel (+ meilleurs ensembles)
 *   e10_kwsq freq.txt control N corpus.txt L MODE SEED words.txt  -> MODE=kw (carré à mot-clé) | any (carré quelconque)
 *   e10_kwsq freq.txt words cipher.txt GEO words.txt -> classement des mots-clés du fichier
 * freq.txt : 25 log-probabilités (A..Z sans J), une par ligne.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
static double LP[25]; static int ORD_LP[25];           /* lettres par ln p décroissant */
static uint64_t rs=88172645463325252ULL; static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
/* orientation o : position s (0..24) dans la suite du carré -> case c (0..24, c = ligne*5+col, lignes 6,7,8,9,0) */
static int orient(int o,int s){ int a=s/5,b=s%5; int r,c; if(o&1){r=b;c=a;} else {r=a;c=b;} if(o&2) r=4-r; if(o&4) c=4-c; return r*5+c; }
static double llmax(const int*n){ int cs[25]; memcpy(cs,n,sizeof cs); for(int i=0;i<25;i++)for(int j=i+1;j<25;j++) if(cs[j]>cs[i]){int t=cs[i];cs[i]=cs[j];cs[j]=t;}
  double s=0; for(int i=0;i<25;i++) s+=cs[i]*LP[ORD_LP[i]]; return s; }
typedef struct{double ll; uint32_t mask; int o;} Best;
static double llkw(const int*n,Best*b){
  double best=-1e300;
  for(int o=0;o<8;o++){
    int pc[25]; for(int s=0;s<25;s++) pc[s]=n[orient(o,s)];
    int sp[26][25]; /* sp[k] = comptes des k premières positions, triés décroissants */
    for(int k=0;k<=25;k++){ for(int i=0;i<k;i++) sp[k][i]=pc[i]; for(int i=0;i<k;i++)for(int j=i+1;j<k;j++) if(sp[k][j]>sp[k][i]){int t=sp[k][i];sp[k][i]=sp[k][j];sp[k][j]=t;} }
    for(uint32_t m=0;m<(1u<<25);m++){ int k=__builtin_popcount(m); double s=0; int r=0;
      for(int i=0;i<25&&r<k;i++){ int L=ORD_LP[i]; if(m>>L&1) s+=sp[k][r++]*LP[L]; }
      int pos=k; for(int L=0;L<25;L++) if(!(m>>L&1)) s+=pc[pos++]*LP[L];
      if(s>best){best=s; if(b){b->ll=s;b->mask=m;b->o=o;}} } }
  return best; }
static void load_counts(const char*p,int geo,int*n){ FILE*f=fopen(p,"r"); char d[1000]; int k=0,ch; while((ch=fgetc(f))!=EOF) if(ch>='0'&&ch<='9') d[k++]=ch; fclose(f);
  memset(n,0,25*sizeof(int)); for(int i=0;i<196;i++){ if(geo==1&&i%14==13) continue; int a=d[2*i]-'0',b=d[2*i+1]-'0'; n[((a==0)?4:a-6)*5+(b-1)]++; } }
static int L25(int ch){ if(ch<'A'||ch>'Z') return -1; if(ch=='J') ch='I'; return ch<'J'?ch-'A':ch-'A'-1; }
static const char AL[]="ABCDEFGHIKLMNOPQRSTUVWXYZ";
/* carré à mot-clé : suite des 25 lettres */
static void kw_seq(const char*w,int*seq){ int used[25]={0},k=0; for(const char*p=w;*p;p++){ int L=L25(*p>='a'&&*p<='z'?*p-32:*p); if(L>=0&&!used[L]){used[L]=1;seq[k++]=L;} } for(int L=0;L<25;L++) if(!used[L]) seq[k++]=L; }
int main(int argc,char**argv){
  setbuf(stdout,NULL); FILE*f=fopen(argv[1],"r"); for(int i=0;i<25;i++) if(fscanf(f,"%lf",&LP[i])!=1) return 2; fclose(f);
  for(int i=0;i<25;i++) ORD_LP[i]=i; for(int i=0;i<25;i++)for(int j=i+1;j<25;j++) if(LP[ORD_LP[j]]>LP[ORD_LP[i]]){int t=ORD_LP[i];ORD_LP[i]=ORD_LP[j];ORD_LP[j]=t;}
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  if(!strcmp(argv[2],"real")){ int n[25]; load_counts(argv[3],atoi(argv[4]),n); Best b; double lm=llmax(n),lk=llkw(n,&b);
    printf("comptes (cases 61..65,71..,81..,91..,01..05) :"); for(int i=0;i<25;i++) printf(" %d",n[i]); printf("\n");
    printf("LL_max=%.3f LL_K=%.3f DELTA=%.3f ; meilleur ensemble (orientation %d) : ",lm,lk,lm-lk,b.o);
    for(int L=0;L<25;L++) if(b.mask>>L&1) putchar(AL[L]); printf(" (k=%d)\n",__builtin_popcount(b.mask)); return 0; }
  if(!strcmp(argv[2],"words")){ int n[25]; load_counts(argv[3],atoi(argv[4]),n); double lm=llmax(n); FILE*w=fopen(argv[5],"r"); char buf[256];
    while(fscanf(w,"%255s",buf)==1){ int seq[25]; kw_seq(buf,seq); for(int o=0;o<8;o++){ double s=0; for(int q=0;q<25;q++) s+=n[orient(o,q)]*LP[seq[q]]; printf("%.3f %s %d\n",lm-s,buf,o);} } fclose(w); return 0; }
  if(!strcmp(argv[2],"control")){ int N=atoi(argv[3]); FILE*g=fopen(argv[4],"rb"); fseek(g,0,SEEK_END); long CL=ftell(g); fseek(g,0,SEEK_SET); char*C=malloc(CL); if(fread(C,1,CL,g)!=(size_t)CL) return 3; fclose(g);
    int len=atoi(argv[5]); int kw=!strcmp(argv[6],"kw");
    char words[20000][32]; int nw=0; if(kw){ FILE*w=fopen(argv[7],"r"); while(nw<20000&&fscanf(w,"%31s",words[nw])==1) nw++; fclose(w); }
    for(int t=0;t<N;t++){ long off=rnd()%(CL-5000); int n[25]={0},k=0,seq[25];
      if(kw){ kw_seq(words[rnd()%nw],seq); } else { for(int i=0;i<25;i++)seq[i]=i; for(int i=24;i>0;i--){int j=rnd()%(i+1);int x=seq[i];seq[i]=seq[j];seq[j]=x;} }
      int o=rnd()%8, cell[25]; for(int q=0;q<25;q++) cell[seq[q]]=orient(o,q);
      while(k<len){ int L=L25(C[off++]); if(L<0) continue; n[cell[L]]++; k++; }
      double lm=llmax(n),lk=llkw(n,NULL); printf("CTRL %s #%d DELTA=%.3f\n",kw?"kw":"any",t,lm-lk); }
    return 0; }
  return 1; }
