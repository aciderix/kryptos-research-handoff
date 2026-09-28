/* E03 (solveur) — Polybe 5×5 inconnu + transposition colonnaire à clé LIBRE, grille complète W×H.
 * Chiffré (après retrait éventuel de la colonne 14 imprimée) = colonnes du clair lues dans l'ordre de la clé,
 * concaténées : bloc j = S[j*H .. j*H+H-1] ; clair(r,c) = S[P(c)*H + r].
 * Alternance : (1) carré fixé -> ordre des colonnes OPTIMAL (bigrammes, cycle ligne->ligne) par programmation
 * dynamique sur les sous-ensembles ; (2) clé fixée -> carré par recuit quadgrammes. Redémarrages multiples.
 *   e03_solver qg_joint.bin cipher.txt control N corpus | null N | real
 * Env : W (largeur, défaut 13), GEO (0 : 196 paires ; 1 : colonne 14 retirée -> 182), RESTARTS, ROUNDS, SUBIT, SEED
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

static float *QG; static float BG[26][26];
static uint64_t rs=88172645463325252ULL;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (rnd()>>11)*(1.0/9007199254740992.0);}
static int CT196[196], S[196], N, W=13, H, GEO=1;
static int RESTARTS=20, ROUNDS=6, SUBIT=40000;
static const char EFREQ[]="ETAOINSRHLDCUMFPGWYBVKXQJZ";

static void make_bigrams(void){ double t[26][26]={{0}},tot=0;
  for(int i=0;i<456976;i++){double p=exp(QG[i]); t[i/17576][(i/676)%26]+=p; tot+=p;}
  for(int a=0;a<26;a++)for(int b=0;b<26;b++) BG[a][b]=(float)log(t[a][b]/tot+1e-9); }

static void plain_of(const int*P,int*pl){ for(int r=0;r<H;r++)for(int c=0;c<W;c++) pl[r*W+c]=S[P[c]*H+r]; }
static double qscore(const int*pl,const int*map){ double s=0; for(int k=0;k+3<N;k++) s+=QG[((map[pl[k]]*26+map[pl[k+1]])*26+map[pl[k+2]])*26+map[pl[k+3]]]; return s; }

/* (1) ordre optimal des colonnes à carré fixé : chemin hamiltonien maximal (DP sous-ensembles) */
static float A[14][14], Bw[14][14];
static float *DPv; static signed char *DPp;
static void best_key(const int*map,int*P){
  for(int x=0;x<W;x++)for(int y=0;y<W;y++){ double a=0,b=0; for(int r=0;r<H;r++){ a+=BG[map[S[x*H+r]]][map[S[y*H+r]]]; if(r+1<H) b+=BG[map[S[x*H+r]]][map[S[y*H+r+1]]]; } A[x][y]=a; Bw[x][y]=b; }
  int full=(1<<W)-1; double gb=-1e30; int bestP[14];
  for(int s0=0;s0<W;s0++){
    for(long i=0;i<((long)1<<W)*W;i++) DPv[i]=-1e30f;
    DPv[(1<<s0)*W+s0]=0;
    for(int m=1;m<=full;m++){ if(!(m>>s0&1)) continue;
      for(int e=0;e<W;e++){ float v=DPv[(long)m*W+e]; if(v<-1e29f) continue;
        for(int f=0;f<W;f++){ if(m>>f&1) continue; long idx=(long)(m|1<<f)*W+f; float nv=v+A[e][f]; if(nv>DPv[idx]){DPv[idx]=nv;DPp[idx]=(signed char)e;} } } }
    for(int e=0;e<W;e++){ float v=DPv[(long)full*W+e]; if(v<-1e29f) continue; double tot=v+Bw[e][s0];
      if(tot>gb){ gb=tot; int m=full,cur=e; for(int i=W-1;i>=0;i--){ bestP[i]=cur; int pv=DPp[(long)m*W+cur]; m&=~(1<<cur); cur=pv; } } } }
  memcpy(P,bestP,sizeof(int)*W); }

/* (0) score INVARIANT par substitution : répétitions de bigrammes et trigrammes de symboles du clair */
static double LAM=2.0;
static double inv_score(const int*P){ static int b2[625]; static int b3[15625]; int pl[196]; plain_of(P,pl);
  memset(b2,0,sizeof b2); memset(b3,0,sizeof b3); double s=0;
  for(int k=0;k+1<N;k++){ int i=pl[k]*25+pl[k+1]; s+=b2[i]; b2[i]++; }
  for(int k=0;k+2<N;k++){ int i=(pl[k]*25+pl[k+1])*25+pl[k+2]; s+=LAM*b3[i]; b3[i]++; }
  return s; }
static double key_anneal_inv(int*P,int iters){ double cur=inv_score(P),best=cur; int bP[14]; memcpy(bP,P,sizeof(int)*W);
  for(int it=0;it<iters;it++){ double T=3.0*pow(0.1/3.0,(double)it/iters); int old[14]; memcpy(old,P,sizeof(int)*W);
    int c1=rnd()%W,c2=rnd()%W; if(c1==c2) continue; if(c1>c2){int t=c1;c1=c2;c2=t;} int op=rnd()%3;
    if(op==0){int t=P[c1];P[c1]=P[c2];P[c2]=t;} else if(op==1){int t=P[c1];for(int c=c1;c<c2;c++)P[c]=P[c+1];P[c2]=t;} else {for(int a=c1,b=c2;a<b;a++,b--){int t=P[a];P[a]=P[b];P[b]=t;}}
    double ns=inv_score(P); if(ns>=cur||urand()<exp((ns-cur)/T)){cur=ns; if(cur>best){best=cur;memcpy(bP,P,sizeof(int)*W);}} else memcpy(P,old,sizeof(int)*W); }
  memcpy(P,bP,sizeof(int)*W); return best; }
/* (1b) valeur du meilleur chemin (sans liaison de ligne), toutes origines à la fois : ~W^2 2^W opérations */
static double dp_value(const int*map,int*P){
  for(int x=0;x<W;x++)for(int y=0;y<W;y++){ double a=0; for(int r=0;r<H;r++) a+=BG[map[S[x*H+r]]][map[S[y*H+r]]]; A[x][y]=a; }
  int full=(1<<W)-1; for(long i=0;i<((long)1<<W)*W;i++) DPv[i]=-1e30f; for(int e=0;e<W;e++) DPv[(long)(1<<e)*W+e]=0;
  for(int m=1;m<=full;m++) for(int e=0;e<W;e++){ if(!(m>>e&1)) continue; float v=DPv[(long)m*W+e]; if(v<-1e29f) continue;
    for(int f=0;f<W;f++){ if(m>>f&1) continue; long idx=(long)(m|1<<f)*W+f; float nv=v+A[e][f]; if(nv>DPv[idx]){DPv[idx]=nv;DPp[idx]=(signed char)e;} } }
  double gb=-1e30; int be=0; for(int e=0;e<W;e++) if(DPv[(long)full*W+e]>gb){gb=DPv[(long)full*W+e];be=e;}
  if(P){ int m=full,cur=be; for(int i=W-1;i>=0;i--){ P[i]=cur; int pv=DPp[(long)m*W+cur]; m&=~(1<<cur); cur=pv; } }
  return gb; }
static int SIGIT=5000;
/* recuit sur le carré, chaque carré noté par la meilleure clé exacte (bigrammes) */
static void sigma_anneal_dp(int*map){
  int used[26]; for(int l=0;l<26;l++)used[l]=-1; for(int s=0;s<25;s++) used[map[s]]=s;
  int pres[25]={0},np=0,plist[25]; for(int i=0;i<N;i++) pres[S[i]]=1; for(int s=0;s<25;s++) if(pres[s]) plist[np++]=s;
  double cur=dp_value(map,NULL),best=cur; int bm[25]; memcpy(bm,map,sizeof bm);
  for(int it=0;it<SIGIT;it++){ double T=4.0*pow(0.2/4.0,(double)it/SIGIT);
    int a=plist[rnd()%np]; int nl=rnd()%26; int b=used[nl]; if(b==a) continue; int la=map[a];
    map[a]=nl; if(b>=0)map[b]=la; double ns=dp_value(map,NULL);
    if(ns>=cur||urand()<exp((ns-cur)/T)){cur=ns;used[nl]=a;used[la]=b; if(cur>best){best=cur;memcpy(bm,map,sizeof bm);}}
    else {map[a]=la; if(b>=0)map[b]=nl;} }
  memcpy(map,bm,sizeof bm); }
/* (2) carré à clé fixée : recuit quadgrammes (depuis map) */
static double sub_anneal(const int*pl,int*map,int iters,double T0,double T1){
  int used[26]; for(int l=0;l<26;l++)used[l]=-1; for(int s=0;s<25;s++) used[map[s]]=s;
  double cur=qscore(pl,map),best=cur; int bm[25]; memcpy(bm,map,sizeof bm);
  for(int it=0;it<iters;it++){ double T=T0*pow(T1/T0,(double)it/iters);
    int a=pl[rnd()%N]; int nl=rnd()%26; int b=used[nl]; if(b==a) continue; int la=map[a];
    map[a]=nl; if(b>=0)map[b]=la; double ns=qscore(pl,map);
    if(ns>=cur||urand()<exp((ns-cur)/T)){cur=ns;used[nl]=a;used[la]=b; if(cur>best){best=cur;memcpy(bm,map,sizeof bm);}}
    else {map[a]=la; if(b>=0)map[b]=nl;} }
  memcpy(map,bm,sizeof bm); return best; }

static double solve(int*bestP,int*bestmap,double*perR){
  int cnt[25]={0}; for(int i=0;i<N;i++)cnt[S[i]]++; int ord[25]; for(int i=0;i<25;i++)ord[i]=i;
  for(int i=0;i<25;i++)for(int j=i+1;j<25;j++) if(cnt[ord[j]]>cnt[ord[i]]){int t=ord[i];ord[i]=ord[j];ord[j]=t;}
  double gbest=-1e30;
  for(int R=0;R<RESTARTS;R++){
    int map[25]; /* départ : rang de fréquence, perturbé (sauf R=0) */
    int lt[26]; for(int i=0;i<26;i++) lt[i]=EFREQ[i]-'A';
    if(R>0){ int sw=2+R%8; for(int k=0;k<sw;k++){int i=rnd()%12,j=rnd()%26;int t=lt[i];lt[i]=lt[j];lt[j]=t;} }
    for(int i=0;i<25;i++) map[ord[i]]=lt[i];
    int P[14],pl[196]; double sc=-1e30;
    if(SIGIT>0) sigma_anneal_dp(map);
    for(int k=0;k<ROUNDS;k++){ best_key(map,P); plain_of(P,pl); sc=sub_anneal(pl,map,SUBIT,k==0?2.5:1.0,0.05); }
    if(perR) perR[R]=sc/(N-3);
    if(sc>gbest){gbest=sc;memcpy(bestP,P,sizeof(int)*W);memcpy(bestmap,map,sizeof map);} }
  return gbest/(N-3); }

static void load(const char*p){FILE*f=fopen(p,"r");char d[1000];int n=0,c;while((c=fgetc(f))!=EOF)if(c>='0'&&c<='9')d[n++]=c;fclose(f);
  for(int i=0;i<196;i++){int a=d[2*i]-'0',b=d[2*i+1]-'0'; CT196[i]=((a==0)?4:a-6)*5+(b-1);} }
static void set_seq_from(const int*g196){ N=0; for(int i=0;i<196;i++) if(GEO==0||i%14!=13) S[N++]=g196[i]; }

static char*CORPUS; static long CL;
int main(int argc,char**argv){
  setbuf(stdout,NULL); if(argc<4) return 1;
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f); make_bigrams();
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  if(getenv("W"))W=atoi(getenv("W")); if(getenv("GEO"))GEO=atoi(getenv("GEO"));
  if(getenv("RESTARTS"))RESTARTS=atoi(getenv("RESTARTS")); if(getenv("ROUNDS"))ROUNDS=atoi(getenv("ROUNDS")); if(getenv("SUBIT"))SUBIT=atoi(getenv("SUBIT")); if(getenv("SIGIT"))SIGIT=atoi(getenv("SIGIT"));
  DPv=malloc(sizeof(float)*((long)1<<W)*W); DPp=malloc(((long)1<<W)*W);
  load(argv[2]); set_seq_from(CT196); if(N%W){fprintf(stderr,"N=%d non divisible par W=%d\n",N,W);return 4;} H=N/W;
  printf("GEO=%d N=%d W=%d H=%d R=%d ROUNDS=%d SUBIT=%d\n",GEO,N,W,H,RESTARTS,ROUNDS,SUBIT);
  if(!strcmp(argv[3],"control")){
    int nc=atoi(argv[4]); FILE*g=fopen(argv[5],"rb");fseek(g,0,SEEK_END);CL=ftell(g);fseek(g,0,SEEK_SET);CORPUS=malloc(CL);if(fread(CORPUS,1,CL,g)!=(size_t)CL)return 3;fclose(g);
    int ok=0;
    for(int t=0;t<nc;t++){ long off=rnd()%(CL-5000); int txt[196],k=0; while(k<N){int ch=CORPUS[off++]; if(ch<'A'||ch>'Z')continue; if(ch=='J')ch='I'; txt[k++]=ch-'A';}
      int letters[25],m=0; for(int l=0;l<26;l++) if(l!=9) letters[m++]=l; for(int i=24;i>0;i--){int j=rnd()%(i+1);int x=letters[i];letters[i]=letters[j];letters[j]=x;}
      int sy[26]; for(int i=0;i<25;i++) sy[letters[i]]=i;
      int Pt[14]; for(int c=0;c<W;c++)Pt[c]=c; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int x=Pt[i];Pt[i]=Pt[j];Pt[j]=x;}
      for(int r=0;r<H;r++)for(int c=0;c<W;c++) S[Pt[c]*H+r]=sy[txt[r*W+c]];
      { int mt0[25]; for(int i=0;i<25;i++) mt0[i]=letters[i]; int Pd[14]; best_key(mt0,Pd); int okk=0; for(int c=0;c<W;c++) okk+=(Pd[c]==Pt[c]);
        printf("   [diag] DP avec le vrai carré : %d/%d colonnes à la bonne place\n",okk,W); }
      if(getenv("DIAGINV")){ double ft=inv_score(Pt); double fb=-1; int okb=0; for(int z=0;z<5;z++){ int Pk[14]; for(int c=0;c<W;c++)Pk[c]=c; for(int i=W-1;i>0;i--){int j=rnd()%(i+1);int x=Pk[i];Pk[i]=Pk[j];Pk[j]=x;}
          double fz=key_anneal_inv(Pk,200000); if(fz>fb){fb=fz; okb=0; for(int c=0;c<W;c++){ /* adjacences correctes */ for(int d=0;d+1<W;d++) if(Pk[c]==Pt[d]&&c+1<W&&Pk[c+1]==Pt[d+1]) okb++; }} }
        printf("   [diag] invariant : vraie clé %.0f ; meilleur trouvé %.0f (adjacences justes %d/%d)\n",ft,fb,okb,W-1); }
      int P[14],mp[25],pl[196]; double q=solve(P,mp,NULL); plain_of(P,pl); int rec=0; for(int i=0;i<N;i++) if(mp[pl[i]]==txt[i]) rec++;
      int mt[25]={0}; int tp[196]; for(int i=0;i<N;i++){tp[i]=sy[txt[i]]; mt[tp[i]]=txt[i];}
      printf("CTRL #%d qoff=%.3f (vraie clé %.3f) récupéré=%d/%d %s\n",t,q,qscore(tp,mt)/(N-3),rec,N,rec>=0.9*N?"OK":"ÉCHEC"); if(rec>=0.9*N) ok++; }
    printf("==> %d/%d contrôles récupérés (≥90%%)\n",ok,nc); return 0; }
  if(!strcmp(argv[3],"null")){ int nn=atoi(argv[4]); double mx=-1e9;
    for(int t=0;t<nn;t++){ set_seq_from(CT196); for(int i=N-1;i>0;i--){int j=rnd()%(i+1);int x=S[i];S[i]=S[j];S[j]=x;}
      int P[14],mp[25]; double q=solve(P,mp,NULL); if(q>mx)mx=q; printf("NULL #%d qoff=%.3f\n",t,q); }
    printf("==> max null = %.3f\n",mx); return 0; }
  if(!strcmp(argv[3],"real")){
    int P[14],mp[25],pl[196]; double pr[256]; double q=solve(P,mp,pr); plain_of(P,pl);
    printf("MEILLEUR qoff=%.3f ; clé (bloc lu par colonne du clair) :",q); for(int c=0;c<W;c++)printf(" %d",P[c]+1); printf("\nCLAIR=");
    for(int i=0;i<N;i++)putchar('A'+mp[pl[i]]); printf("\n");
    { /* vérification directe : clair + carré + clé -> chiffré -> grille imprimée (+ nulles conservées en B) -> 392 chiffres */
      int inv[26]; for(int l=0;l<26;l++)inv[l]=-1; int pres[25]={0}; for(int i=0;i<N;i++)pres[S[i]]=1; int coll=0;
      for(int sy=0;sy<25;sy++) if(pres[sy]){ if(inv[mp[sy]]>=0)coll++; inv[mp[sy]]=sy; }
      int S2[196]; for(int r=0;r<H;r++)for(int c=0;c<W;c++) S2[P[c]*H+r]=inv[mp[pl[r*W+c]]];
      int G[196],k=0; for(int i=0;i<196;i++) G[i]=(GEO==0||i%14!=13)?S2[k++]:CT196[i];
      FILE*g=fopen(argv[2],"r");char d[1000];int nd=0,ch;while((ch=fgetc(g))!=EOF)if(ch>='0'&&ch<='9')d[nd++]=ch;fclose(g);
      const char rowd[5]={'6','7','8','9','0'}; int bad=0;
      for(int i=0;i<196;i++) if(G[i]<0||d[2*i]!=rowd[G[i]/5]||d[2*i+1]!='1'+G[i]%5) bad++;
      printf("ré-enchiffrement : %d/196 paires différentes, collisions carré=%d %s\n",bad,coll,(bad||coll)?"ÉCHEC":"(exact)"); }
    int same=0; for(int R=0;R<RESTARTS;R++) if(pr[R]>=q-0.02) same++; printf("redémarrages atteignant le meilleur (±0,02) : %d/%d\n",same,RESTARTS);
    return 0; }
  return 1; }
