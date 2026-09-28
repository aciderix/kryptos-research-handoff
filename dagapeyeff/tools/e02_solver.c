/* E02 — Polybe 5×5 (carré inconnu) + transposition colonnaire à clé, largeur 14, bourrage final.
 * Modèle : clair (M lettres + 196-M bourrages) écrit ligne par ligne (ou en boustrophédon) dans une grille
 * 14×14 ; colonnes lues de haut en bas dans l'ordre de la clé ; la j-ième colonne lue = j-ième ligne imprimée.
 * Donc symbole du clair en (r,c) = CT[14*P[c] + r], P[c] = ligne imprimée qui porte la colonne c.
 * Contrainte (pré-inscrite) : les lignes imprimées dont la case 14 est un symbole rare (<=3 occurrences)
 * portent les colonnes de bourrage ; M = 196 - (nombre de ces lignes).
 *   e02_solver qg_joint.bin cipher.txt control N corpus   (contraint ; puis N/2 libres)
 *   e02_solver qg_joint.bin cipher.txt null N
 *   e02_solver qg_joint.bin cipher.txt real
 * Env : RESTARTS, ITERS, SEED, BOU=1 (écriture boustrophédon), FREE=1 (clé libre, 14!)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

static float *QG;
static uint64_t rs=88172645463325252ULL;
static inline uint64_t rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
static inline double urand(void){return (rnd()>>11)*(1.0/9007199254740992.0);}

static int CT[196];          /* grille imprimée, ligne par ligne */
static int M;                /* longueur du message (bourrage exclu) */
static int BOU=0, FREE=0;
static int PADROW[14];       /* 1 si la ligne imprimée porte une colonne de bourrage */
static int COLPAD[14];       /* 1 si la colonne c du clair est une colonne de bourrage (dépend de M, BOU) */
static int KPOS[196];        /* k (ordre de lecture du clair) -> indice r*14+c */
static const char EFREQ[]="ETAOINSRHLDCUMFPGWYBVKXQJZ";

static void setup_geometry(void){
  for(int k=0;k<196;k++){int r=k/14,t=k%14,c=(BOU&&(r&1))?13-t:t; KPOS[k]=r*14+c;}
  for(int c=0;c<14;c++)COLPAD[c]=0;
  for(int k=M;k<196;k++) COLPAD[KPOS[k]%14]=1;
}
static void plain_syms(const int*P,int*sym){ for(int k=0;k<196;k++){int rc=KPOS[k],r=rc/14,c=rc%14; sym[k]=CT[14*P[c]+r];} }
static double score(const int*sym,const int*map){
  double s=0; for(int k=0;k+3<M;k++){int a=map[sym[k]],b=map[sym[k+1]],c=map[sym[k+2]],d=map[sym[k+3]]; s+=QG[((a*26+b)*26+c)*26+d];} return s; }

/* clé aléatoire respectant la contrainte (ou libre) */
static void random_key(int*P){
  int pr[14],np=0,mr[14],nm=0; for(int j=0;j<14;j++){ if(FREE||!PADROW[j]) mr[nm++]=j; else pr[np++]=j; }
  for(int i=nm-1;i>0;i--){int j=rnd()%(i+1);int t=mr[i];mr[i]=mr[j];mr[j]=t;}
  for(int i=np-1;i>0;i--){int j=rnd()%(i+1);int t=pr[i];pr[i]=pr[j];pr[j]=t;}
  int a=0,b=0; for(int c=0;c<14;c++){ if(FREE) P[c]=mr[a++]; else P[c]=COLPAD[c]?pr[b++]:mr[a++]; }
}

static int RESTARTS=8, ITERS=400000;
static int *DBG_P=NULL,*DBG_M=NULL; static int FIXK=0,FIXM=0; static double TKF=8, PK=0.3; /* diagnostics des contrôles seulement */
/* recuit de substitution sur une clé fixée (depuis map/used), température constante ou décroissante */
static double sub_anneal(const int*sym,int*map,int*used,int iters,double T0,double T1){
  double cur=score(sym,map),best=cur; int bm[25],bu[26]; memcpy(bm,map,sizeof bm); memcpy(bu,used,sizeof bu);
  for(int it=0;it<iters;it++){ double T=T0*pow(T1/T0,(double)it/iters);
    int a=CT[rnd()%196]; int newl=rnd()%26; int b=used[newl]; if(b==a) continue; int la=map[a];
    map[a]=newl; if(b>=0)map[b]=la; double ns=score(sym,map);
    if(ns>=cur||urand()<exp((ns-cur)/T)){cur=ns;used[newl]=a;used[la]=b; if(cur>best){best=cur;memcpy(bm,map,sizeof bm);memcpy(bu,used,sizeof bu);}}
    else {map[a]=la; if(b>=0)map[b]=newl;} }
  memcpy(map,bm,sizeof bm); memcpy(used,bu,sizeof bu); return best; }
static int NESTED=1, OUTER=3000, INNER=1500;
/* recherche emboîtée : recuit sur la clé ; à chaque proposition, le carré est ré-optimisé (INNER itérations) */
static double solve_nested(int*bestP,int*bestmap,double*perRestart){
  double gbest=-1e18;
  for(int R=0;R<RESTARTS;R++){
    int P[14],map[25],used[26]; random_key(P);
    int lt[26]; for(int i=0;i<26;i++)lt[i]=i; for(int i=25;i>0;i--){int j=rnd()%(i+1);int t=lt[i];lt[i]=lt[j];lt[j]=t;}
    for(int l=0;l<26;l++)used[l]=-1; for(int q=0;q<25;q++){map[q]=lt[q];used[lt[q]]=q;}
    int sym[196]; plain_syms(P,sym); double cur=sub_anneal(sym,map,used,60000,2.5,0.2),best=cur;
    int bP[14],bm[25]; memcpy(bP,P,sizeof P); memcpy(bm,map,sizeof bm);
    for(int it=0;it<OUTER;it++){ double TK=TKF*2.5*pow(0.02,(double)it/OUTER);
      int c1=rnd()%14,c2=rnd()%14; if(c1==c2){it--;continue;} if(!FREE&&COLPAD[c1]!=COLPAD[c2]){it--;continue;}
      if(c1>c2){int t=c1;c1=c2;c2=t;} int old[14],om[25],ou[26]; memcpy(old,P,sizeof P); memcpy(om,map,sizeof om); memcpy(ou,used,sizeof ou); int op=rnd()%3;
      if(op==0){int t=P[c1];P[c1]=P[c2];P[c2]=t;}
      else if(op==1){ if(rnd()&1){int t=P[c1];for(int c=c1;c<c2;c++)P[c]=P[c+1];P[c2]=t;} else {int t=P[c2];for(int c=c2;c>c1;c--)P[c]=P[c-1];P[c1]=t;} }
      else { for(int a=c1,b=c2;a<b;a++,b--){int t=P[a];P[a]=P[b];P[b]=t;} }
      plain_syms(P,sym); double ns=sub_anneal(sym,map,used,INNER,0.6,0.1);
      if(ns>=cur||urand()<exp((ns-cur)/TK)){cur=ns; if(cur>best){best=cur;memcpy(bP,P,sizeof P);memcpy(bm,map,sizeof bm);}}
      else {memcpy(P,old,sizeof P);memcpy(map,om,sizeof om);memcpy(used,ou,sizeof ou);} }
    /* polissage final du carré sur la meilleure clé */
    for(int l=0;l<26;l++)used[l]=-1; for(int q=0;q<25;q++)used[bm[q]]=q; plain_syms(bP,sym);
    double fin=sub_anneal(sym,bm,used,200000,1.0,0.05); if(fin>best)best=fin;
    if(perRestart) perRestart[R]=best/(M-3);
    if(best>gbest){gbest=best;memcpy(bestP,bP,sizeof bP);memcpy(bestmap,bm,sizeof bm);}
  }
  return gbest/(M-3); }
/* recuit joint (clé, substitution) ; renvoie qoff (par quadgramme) */
static double solve(int*bestP,int*bestmap,double*perRestart){
  if(NESTED&&!FIXK&&!FIXM) return solve_nested(bestP,bestmap,perRestart);
  double gbest=-1e18;
  int cnt[25]={0}; for(int i=0;i<196;i++)cnt[CT[i]]++;
  for(int R=0;R<RESTARTS;R++){
    int P[14],map[25],used[26]; random_key(P);
    for(int l=0;l<26;l++)used[l]=-1;
    if(R==0){ int ord[25]; for(int i=0;i<25;i++)ord[i]=i;
      for(int i=0;i<25;i++)for(int j=i+1;j<25;j++)if(cnt[ord[j]]>cnt[ord[i]]){int t=ord[i];ord[i]=ord[j];ord[j]=t;}
      for(int i=0;i<25;i++){map[ord[i]]=EFREQ[i]-'A';used[map[ord[i]]]=ord[i];}
    } else { int lt[26]; for(int i=0;i<26;i++)lt[i]=i; for(int i=25;i>0;i--){int j=rnd()%(i+1);int t=lt[i];lt[i]=lt[j];lt[j]=t;}
      for(int s=0;s<25;s++){map[s]=lt[s];used[lt[s]]=s;} }
    if(DBG_P&&FIXK){memcpy(P,DBG_P,sizeof P);} if(DBG_M&&FIXM){memcpy(map,DBG_M,sizeof map);for(int l=0;l<26;l++)used[l]=-1;for(int q=0;q<25;q++)used[map[q]]=q;}
    int sym[196]; plain_syms(P,sym); double cur=score(sym,map),best=cur; int bP[14],bm[25]; memcpy(bP,P,sizeof P); memcpy(bm,map,sizeof map);
    const double T0=2.5,T1=0.05;
    for(int it=0;it<ITERS;it++){
      double T=T0*pow(T1/T0,(double)it/ITERS);
      int doK=urand()<PK; if(FIXK)doK=0; if(FIXM)doK=1;
      if(doK){ /* clé : échange / insertion / inversion d'un segment, à l'intérieur d'une même classe */
        int c1=rnd()%14,c2=rnd()%14; if(c1==c2) continue; if(!FREE&&COLPAD[c1]!=COLPAD[c2]) continue;
        if(c1>c2){int t=c1;c1=c2;c2=t;} int old[14]; memcpy(old,P,sizeof P); int op=rnd()%3;
        if(op==0){int t=P[c1];P[c1]=P[c2];P[c2]=t;}
        else if(op==1){ if(rnd()&1){int t=P[c1];for(int c=c1;c<c2;c++)P[c]=P[c+1];P[c2]=t;} else {int t=P[c2];for(int c=c2;c>c1;c--)P[c]=P[c-1];P[c1]=t;} }
        else { for(int a=c1,b=c2;a<b;a++,b--){int t=P[a];P[a]=P[b];P[b]=t;} }
        plain_syms(P,sym); double ns=score(sym,map); double TK=TKF*T;
        if(ns>=cur||urand()<exp((ns-cur)/TK)) cur=ns; else {memcpy(P,old,sizeof P); plain_syms(P,sym);}
      } else { /* substitution : a <- newl ; l'ancien détenteur de newl <- ancienne lettre de a */
        int a=CT[rnd()%196]; int newl=rnd()%26; int b=used[newl]; if(b==a) continue; int la=map[a];
        map[a]=newl; if(b>=0)map[b]=la; double ns=score(sym,map);
        if(ns>=cur||urand()<exp((ns-cur)/T)){cur=ns;used[newl]=a;used[la]=b;} else {map[a]=la; if(b>=0)map[b]=newl;}
      }
      if(cur>best){best=cur;memcpy(bP,P,sizeof P);memcpy(bm,map,sizeof map);}
    }
    if(perRestart) perRestart[R]=best/(M-3);
    if(best>gbest){gbest=best;memcpy(bestP,bP,sizeof bP);memcpy(bestmap,bm,sizeof bm);}
  }
  return gbest/(M-3);
}

static void load_cipher(const char*p){FILE*f=fopen(p,"r");char d[1000];int n=0,c;while((c=fgetc(f))!=EOF)if(c>='0'&&c<='9')d[n++]=c;fclose(f);
  for(int i=0;i<196;i++){int a=d[2*i]-'0',b=d[2*i+1]-'0'; int ri=(a==0)?4:a-6; CT[i]=ri*5+(b-1);} }
/* règle pré-inscrite : lignes dont la case 14 porte un symbole à <=3 occurrences */
static void derive_padrows(void){ int cnt[25]={0}; for(int i=0;i<196;i++)cnt[CT[i]]++; int np=0;
  for(int j=0;j<14;j++){PADROW[j]=cnt[CT[14*j+13]]<=3; np+=PADROW[j];} M=196-np; }

static char *CORPUS; static long CL;
int main(int argc,char**argv){
  setbuf(stdout,NULL);
  if(argc<4){fprintf(stderr,"usage: e02_solver qg.bin cipher.txt control|null|real [n] [corpus]\n");return 1;}
  FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
  if(getenv("SEED")){rs^=0x9E3779B97F4A7C15ULL*(uint64_t)atoll(getenv("SEED"));for(int i=0;i<10;i++)rnd();}
  if(getenv("RESTARTS"))RESTARTS=atoi(getenv("RESTARTS")); if(getenv("ITERS"))ITERS=atoi(getenv("ITERS"));
  if(getenv("NESTED"))NESTED=atoi(getenv("NESTED")); if(getenv("OUTER"))OUTER=atoi(getenv("OUTER")); if(getenv("INNER"))INNER=atoi(getenv("INNER"));
  if(getenv("TKF"))TKF=atof(getenv("TKF")); if(getenv("PK"))PK=atof(getenv("PK"));
  if(getenv("BOU"))BOU=atoi(getenv("BOU")); if(getenv("FREE"))FREE=atoi(getenv("FREE"));
  load_cipher(argv[2]); int REAL[196]; memcpy(REAL,CT,sizeof CT);
  derive_padrows(); int realM=M; int realPAD[14]; memcpy(realPAD,PADROW,sizeof PADROW);
  printf("règle bourrage : lignes");for(int j=0;j<14;j++)if(PADROW[j])printf(" %d",j+1);printf(" -> M=%d ; BOU=%d FREE=%d R=%d I=%d\n",M,BOU,FREE,RESTARTS,ITERS);
  setup_geometry();

  if(!strcmp(argv[3],"control")){
    int nc=argc>4?atoi(argv[4]):5; const char*cp=argv[5];
    FILE*g=fopen(cp,"rb");fseek(g,0,SEEK_END);CL=ftell(g);fseek(g,0,SEEK_SET);CORPUS=malloc(CL);if(fread(CORPUS,1,CL,g)!=(size_t)CL)return 3;fclose(g);
    int ok=0;
    for(int t=0;t<nc;t++){
      /* texte : M lettres anglaises hors corpus + bourrage en lettres rares */
      long off=rnd()%(CL-5000); int txt[196],k=0;
      while(k<M){int ch=CORPUS[off++]; if(ch<'A'||ch>'Z')continue; if(ch=='J')ch='I'; txt[k++]=ch-'A';}
      const char pads[]="QXZKV"; while(k<196) txt[k++]=pads[rnd()%5]-'A';
      int letters[25],m=0; for(int l=0;l<26;l++) if(l!=9) letters[m++]=l;
      for(int i=24;i>0;i--){int j=rnd()%(i+1);int x=letters[i];letters[i]=letters[j];letters[j]=x;}
      int sy[26]; for(int i=0;i<25;i++) sy[letters[i]]=i;
      int Ptrue[14]; for(int c=0;c<14;c++)Ptrue[c]=c; for(int i=13;i>0;i--){int j=rnd()%(i+1);int x=Ptrue[i];Ptrue[i]=Ptrue[j];Ptrue[j]=x;}
      for(int kk=0;kk<196;kk++){int rc=KPOS[kk],r=rc/14,c=rc%14; CT[14*Ptrue[c]+r]=sy[txt[kk]];}
      for(int j=0;j<14;j++)PADROW[j]=0; for(int c=0;c<14;c++) if(COLPAD[c]) PADROW[Ptrue[c]]=1; /* partition vraie */
      int MT[25]; for(int q=0;q<25;q++)MT[q]=-1; for(int i=0;i<25;i++)MT[i]=letters[i]; DBG_P=Ptrue; DBG_M=MT;
      if(getenv("FIXK"))FIXK=1; if(getenv("FIXM"))FIXM=1;
      int P[14],mp[25]; double q=solve(P,mp,NULL); int sym[196]; plain_syms(P,sym);
      int rec=0; for(int kk=0;kk<M;kk++) if(mp[sym[kk]]==txt[kk]) rec++;
      int ptrue[196]; for(int kk=0;kk<196;kk++)ptrue[kk]=sy[txt[kk]]; int mt[25]={0}; for(int kk=0;kk<196;kk++)mt[ptrue[kk]]=txt[kk];
      double qt=score(ptrue,mt)/(M-3);
      printf("CTRL #%d qoff=%.3f (vraie clé %.3f) récupéré=%d/%d %s\n",t,q,qt,rec,M,rec>=0.9*M?"OK":"ÉCHEC"); if(rec>=0.9*M)ok++;
    }
    printf("==> %d/%d contrôles récupérés (≥90%%)\n",ok,nc); return 0; }

  if(!strcmp(argv[3],"null")){
    int nn=argc>4?atoi(argv[4]):10; double mx=-1e9;
    for(int t=0;t<nn;t++){ memcpy(CT,REAL,sizeof CT);
      int idx[182],ni=0; for(int i=0;i<196;i++) if(i%14!=13) idx[ni++]=i; /* colonne 14 fixe -> même partition */
      for(int i=181;i>0;i--){int j=rnd()%(i+1);int x=CT[idx[i]];CT[idx[i]]=CT[idx[j]];CT[idx[j]]=x;}
      int P[14],mp[25]; double q=solve(P,mp,NULL); if(q>mx)mx=q; printf("NULL #%d qoff=%.3f\n",t,q); }
    printf("==> max null = %.3f\n",mx); return 0; }

  if(!strcmp(argv[3],"real")){
    memcpy(CT,REAL,sizeof CT); M=realM; memcpy(PADROW,realPAD,sizeof PADROW);
    int P[14],mp[25]; double pr[64]; double q=solve(P,mp,pr); int sym[196]; plain_syms(P,sym);
    printf("MEILLEUR qoff=%.3f ; clé (ligne imprimée par colonne du clair) :",q); for(int c=0;c<14;c++)printf(" %d",P[c]+1); printf("\n");
    printf("par redémarrage :"); for(int R=0;R<RESTARTS;R++)printf(" %.3f",pr[R]); printf("\n");
    printf("CLAIR="); for(int k=0;k<196;k++){ if(k==M)putchar('|'); putchar('A'+mp[sym[k]]); } printf("\n");
    { /* vérification directe : clair + carré + clé -> 392 chiffres, comparés au fichier */
      int inv[26]; for(int l=0;l<26;l++)inv[l]=-1; int pres[25]={0}; for(int i=0;i<196;i++)pres[CT[i]]=1;
      int coll=0; for(int s=0;s<25;s++) if(pres[s]){ if(inv[mp[s]]>=0)coll++; inv[mp[s]]=s; }
      int G[196]; for(int k=0;k<196;k++){int rc=KPOS[k],r=rc/14,c=rc%14; G[14*P[c]+r]=inv[mp[sym[k]]];}
      FILE*g=fopen(argv[2],"r");char d[1000];int nd=0,ch;while((ch=fgetc(g))!=EOF)if(ch>='0'&&ch<='9')d[nd++]=ch;fclose(g);
      const char rowd[5]={'6','7','8','9','0'}; int bad=0;
      for(int i=0;i<196;i++){ if(G[i]<0||d[2*i]!=rowd[G[i]/5]||d[2*i+1]!='1'+G[i]%5) bad++; }
      printf("ré-enchiffrement : %d/196 paires différentes, collisions carré=%d %s\n",bad,coll,(bad||coll)?"ÉCHEC":"(exact)"); }
    { /* stabilité : 8 recuits indépendants (1 redémarrage chacun) */
      int keep=RESTARTS; RESTARTS=1; int same=0;
      for(int t=0;t<8;t++){int P2[14],m2[25]; double q2=solve(P2,m2,NULL); int s2[196]; plain_syms(P2,s2); int eq=0;
        for(int k=0;k<M;k++) if(m2[s2[k]]==mp[sym[k]]) eq++; printf("  stabilité #%d qoff=%.3f identique=%d/%d\n",t,q2,eq,M); if(eq>=0.95*M)same++;}
      RESTARTS=keep; printf("stabilité : %d/8 redémarrages redonnent le même clair (≥95%%)\n",same); }
    return 0; }
  return 1;
}
