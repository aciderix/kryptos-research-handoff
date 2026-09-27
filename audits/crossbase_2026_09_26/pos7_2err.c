/* pos7_2err.c — attaque 2-ERREURS sur la cle POSITIONNELLE (modele T5 : k[i]=A[i%7]+B[ligne(i)]),
 * alphabet CONNU (AZ/KRYPTOS), VIG/BEAU/VAR. base 10 §I : "275 couples d'erreurs (T5) JAMAIS FAITS".
 * Regime probable (Sanborn 4/97). Pour chaque PAIRE de cribs declares errones, on resout A,B sur les
 * 22 restants (systeme lineaire mod 26 = mod2 x mod13), on LIT le clair et on le note (quadgram +
 * accord clair-etendu p8=A,p20=C,p39=N,p58=C,p83=C). STOP-WIN = clair lisible.
 * Usage : pos7_2err qg.bin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
#define NV 11   /* A0..A6 (0..6), B0..B3 (7..10) */
static const char *K4=
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N];
static const int EXP[5]={8,20,39,58,83}; static const char EXL[5]={'A','C','N','C','C'};
static void setcribs(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}}
static inline int md(int x){x%=26;return x<0?x+26:x;}
static int rowof(int i){return i<4?0:1+(i-4)/31;}
static const char*AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char*KRY="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static const char*CN[3]={"VIG","BEAU","VAR"};
static void mkperm(const char*a,int*perm){for(int i=0;i<26;i++)perm[a[i]-'A']=i;}
static float *QG;
static double qoff_range(const int*p,int lo){double q=0;int no=0;for(int i=lo;i+3<N;i++){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}return no?q/no:0;}
static int dec1(int conv,int cv,int kv){if(conv==0)return md(cv-kv);if(conv==1)return md(kv-cv);return md(cv+kv);}
static int kreq_of(int conv,int cv,int pv){if(conv==0)return md(cv-pv);if(conv==1)return md(pv+cv);return md(pv-cv);}

/* solveur lineaire mod p sur NV variables : rows[nr][NV+1] (derniere col = rhs). renvoie 1 si
 * consistant, remplit sol[NV] (variables libres = 0). */
static int solve_modp(int rows[][NV+1],int nr,int p,int*sol){
  static int A[64][NV+1]; for(int i=0;i<nr;i++)for(int j=0;j<=NV;j++)A[i][j]=((rows[i][j]%p)+p)%p;
  int piv_col[NV]; for(int i=0;i<NV;i++)piv_col[i]=-1; int rank=0;
  for(int c=0;c<NV && rank<nr;c++){
    int pr=-1; for(int r=rank;r<nr;r++) if(A[r][c]){pr=r;break;}
    if(pr<0) continue;
    for(int j=0;j<=NV;j++){int t=A[rank][j];A[rank][j]=A[pr][j];A[pr][j]=t;}
    int inv=1; for(int x=1;x<p;x++) if(A[rank][c]*x%p==1){inv=x;break;}
    for(int j=0;j<=NV;j++)A[rank][j]=A[rank][j]*inv%p;
    for(int r=0;r<nr;r++) if(r!=rank && A[r][c]){int f=A[r][c];for(int j=0;j<=NV;j++)A[r][j]=((A[r][j]-f*A[rank][j])%p+p)%p;}
    piv_col[rank]=c; rank++;
  }
  for(int r=rank;r<nr;r++) if(A[r][NV]) return 0; /* 0 = rhs => inconsistant */
  for(int i=0;i<NV;i++)sol[i]=0;
  for(int r=0;r<rank;r++) sol[piv_col[r]]=A[r][NV];
  return 1;
}
/* CRT sur chaque variable : x = a mod2, b mod13 -> mod26 */
static int crt(int a2,int b13){for(int x=0;x<26;x++) if(x%2==a2 && x%13==b13) return x; return 0;}

int main(int argc,char**argv){
  setcribs(); if(argc<2){fprintf(stderr,"usage: pos7_2err qg.bin\n");return 1;}
  FILE*q=fopen(argv[1],"rb");QG=malloc(sizeof(float)*456976);
  if(fread(QG,sizeof(float),456976,q)!=456976){fprintf(stderr,"qg\n");return 2;}fclose(q);
  int ct[N];for(int i=0;i<N;i++)ct[i]=K4[i]-'A';
  int cribpos[24],cl[24],nc=0; for(int i=0;i<13;i++){cribpos[nc]=CE[i];cl[nc]=cribL[CE[i]];nc++;} for(int i=0;i<11;i++){cribpos[nc]=CB[i];cl[nc]=cribL[CB[i]];nc++;}
  int azp[26],kryp[26];mkperm(AZ,azp);mkperm(KRY,kryp);

  /* CONTROLE POSITIF : cle positionnelle connue, 2 erreurs plantees, doit se retrouver */
  {int*perm=azp;int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
   int A[7]={1,7,3,20,11,5,14},B[4]={0,4,9,2};
   const char*eng="XXXXTHEQUICKBROWNFOXJUMPEDOVERTHELAZYDOGSWHILEFILLERWORDSREACHNINETYSEVENFORACONTROLTESTNOWOKAYX";
   int pt0[N];for(int i=0;i<N;i++)pt0[i]=eng[i]-'A';for(int i=0;i<24;i++)pt0[cribpos[i]]=cl[i];
   int c0[N];for(int i=0;i<N;i++){int k=md(A[i%7]+B[rowof(i)]);c0[i]=inv[md(perm[pt0[i]]+k)];}
   /* plante 2 erreurs sur des positions de crib (24,31) */
   int p2[N];int ok=0; /* on droppe (24,31) et on resout */
   int di=-1,dj=-1; for(int a=0;a<24;a++)if(cribpos[a]==24)di=a; for(int a=0;a<24;a++)if(cribpos[a]==31)dj=a;
   int rows2[64][NV+1],r13[64][NV+1],nr=0;
   for(int a=0;a<24;a++){if(a==di||a==dj)continue;int i=cribpos[a];int cv=perm[c0[i]];int pv=perm[cl[a]];int kr=kreq_of(0,cv,pv);
     int row[NV+1];memset(row,0,sizeof row);row[i%7]=1;row[7+rowof(i)]=1;row[NV]=kr;memcpy(rows2[nr],row,sizeof row);memcpy(r13[nr],row,sizeof row);nr++;}
   int s2[NV],s13[NV];int ok2=solve_modp(rows2,nr,2,s2),ok13=solve_modp(r13,nr,13,s13);
   if(ok2&&ok13){int sol[NV];for(int v=0;v<NV;v++)sol[v]=crt(s2[v],s13[v]);
     for(int i=0;i<N;i++){int k=md(sol[i%7]+sol[7+rowof(i)]);p2[i]=inv[dec1(0,perm[c0[i]],k)];}
     int rec=0;for(int i=4;i<N;i++)if(p2[i]==pt0[i])rec++; ok=(rec>=90);}
   printf("CONTROLE POSITIF (cle positionnelle, drop 2, VIG/AZ): rec(4..96)=%s\n",ok?"PASS":"FAIL");}

  double bq=-1e9;char blab[128];int bpt[N],bex=0,bmiss=0; long nconsist=0;
  for(int ai=0;ai<2;ai++){int*perm=ai?kryp:azp;int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
    for(int conv=0;conv<3;conv++){
      /* kreq pour chaque crib */
      int kr[24]; for(int a=0;a<24;a++){int i=cribpos[a];kr[a]=kreq_of(conv,perm[ct[i]],perm[cl[a]]);}
      for(int di=0;di<24;di++)for(int dj=di+1;dj<24;dj++){
        int rows2[64][NV+1],r13[64][NV+1],nr=0;
        for(int a=0;a<24;a++){if(a==di||a==dj)continue;int i=cribpos[a];int row[NV+1];memset(row,0,sizeof row);
          row[i%7]=1;row[7+rowof(i)]=1;row[NV]=kr[a];memcpy(rows2[nr],row,sizeof row);memcpy(r13[nr],row,sizeof row);nr++;}
        int s2[NV],s13[NV];
        if(!solve_modp(rows2,nr,2,s2)) continue; if(!solve_modp(r13,nr,13,s13)) continue;
        nconsist++;
        int sol[NV];for(int v=0;v<NV;v++)sol[v]=crt(s2[v],s13[v]);
        int pt[N];int miss=0;
        for(int i=0;i<N;i++){int R=rowof(i); if(R==0){pt[i]=0;miss++;continue;} int k=md(sol[i%7]+sol[7+R]);pt[i]=inv[dec1(conv,perm[ct[i]],k)];}
        double qq=qoff_range(pt,4); int ex=0;for(int k2=0;k2<5;k2++)if(EXP[k2]>=4&&pt[EXP[k2]]==EXL[k2]-'A')ex++;
        if(qq>bq){bq=qq;memcpy(bpt,pt,sizeof bpt);bex=ex;bmiss=miss;
          snprintf(blab,sizeof blab,"alpha=%s %s drop(%d,%d)",ai?"KRYPTOS":"AZ",CN[conv],cribpos[di],cribpos[dj]);}
      }
    }}
  printf("paires (di,dj) CONSISTANTES (22 cribs OK) : %ld / %d\n",nconsist,276*6);
  if(nconsist==0){printf("=> AUCUNE solution a exactement 2 erreurs pour alphabet connu (il en faut >2). NEGATIF.\n");return 0;}
  printf("MEILLEUR (max qoff, 2 cribs errones) : %s | ex=%d/5 qoff=%.2f (OBKR indetermine)\nPT=",blab,bex,bq);
  for(int i=0;i<N;i++)putchar(i<4?'?':('A'+bpt[i]));putchar('\n');
  return 0;
}
