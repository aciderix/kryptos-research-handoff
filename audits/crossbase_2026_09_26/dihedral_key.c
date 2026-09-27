/* dihedral_key.c — "flip the chart, shine a light" (Sanborn, Big Techday 2013) rendu testable.
 * Le TABLEAU KRYPTOS 26x26 (la "chart") subit une transformee DIEDRALE, lu en ligne -> KEYSTREAM
 * (running key), applique en Vigenere/Beaufort/Variante a K4 avec alphabet CONNU (AZ ou KRYPTOS),
 * a tous offsets. C'est 1:1 (preserve Bean : le texte n'est pas transpose, seule la CLE vient de
 * la chart transformee). On verifie les 24 cribs (DUR), et si match eleve on LIT le clair (quadgram
 * + accord clair-etendu p8=A,p20=C,p39=N,p58=C,p83=C). NON couvert par T16 (petits grid-cle mot-cle).
 * Usage : dihedral_key qg.bin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 97
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
static const char*AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char*KRY="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static const char*CN[3]={"VIG","BEAU","VAR"};static const char*DN[8]={"id","rot90","rot180","rot270","flipH","flipV","transpose","antitrans"};
static float *QG;
static double qoff(const int*p){double q=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}}return no?q/no:0;}
/* alphabet connu : perm[letter]=value */
static void mkperm(const char*a,int*perm){for(int i=0;i<26;i++)perm[a[i]-'A']=i;}
/* decrypt running-key : p = decrypt(conv, c, k) en value-space de l'alphabet perm */
static int dec1(int conv,int cv,int kv){if(conv==0)return md(cv-kv);if(conv==1)return md(kv-cv);return md(cv+kv);}
/* tableau 26x26 : G[r][c] = KRY[(c+r)%26] (alphabet keye decale) */
static void build_tab(int G[26][26]){for(int r=0;r<26;r++)for(int c=0;c<26;c++)G[r][c]=KRY[(c+r)%26]-'A';}
static void dihedral(int G[26][26],int d,int H[26][26]){for(int r=0;r<26;r++)for(int c=0;c<26;c++){int v;
  switch(d){case 0:v=G[r][c];break;case 1:v=G[25-c][r];break;case 2:v=G[25-r][25-c];break;case 3:v=G[c][25-r];break;
    case 4:v=G[r][25-c];break;case 5:v=G[25-r][c];break;case 6:v=G[c][r];break;default:v=G[25-c][25-r];}H[r][c]=v;}}

int main(int argc,char**argv){
  setcribs(); if(argc<2){fprintf(stderr,"usage: dihedral_key qg.bin\n");return 1;}
  FILE*q=fopen(argv[1],"rb");QG=malloc(sizeof(float)*456976);
  if(fread(QG,sizeof(float),456976,q)!=456976){fprintf(stderr,"qg\n");return 2;}fclose(q);
  int ct[N];for(int i=0;i<N;i++)ct[i]=K4[i]-'A';
  int azp[26],kryp[26];mkperm(AZ,azp);mkperm(KRY,kryp);

  /* CONTROLE POSITIF : running key connu (tableau id, offset 40) chiffre English+cribs en VIG/AZ, on recupere */
  {int G[26][26],H[26][26];build_tab(G);dihedral(G,0,H);int seq[676];int m=0;for(int r=0;r<26;r++)for(int c=0;c<26;c++)seq[m++]=H[r][c];
   const char*eng="THEQUICKBROWNFOXJUMPEDOVERTHELAZYDOGSWHILEWORDFILLERSREACHNINETYSEVENLETTERSFORACONTROLTESTOKAYNOWXX";
   int pt0[N];for(int i=0;i<N;i++)pt0[i]=eng[i]-'A';for(int i=0;i<24;i++){int p=(i<13?CE[i]:CB[i-13]);pt0[p]=cribL[p];}
   int off=40,c0[N];for(int i=0;i<N;i++){int k=azp[seq[off+i]];c0[i]=azp[pt0[i]];c0[i]=md(c0[i]+k);/*VIG cipher=p+k*/}
   /* recupere */ int okc=0;for(int i=0;i<N;i++){int p=dec1(0,c0[i],azp[seq[off+i]]);if(p==azp[pt0[i]])okc++;}
   printf("CONTROLE POSITIF (running key tableau id off40 VIG/AZ): %d/97 %s\n",okc,okc==N?"PASS":"FAIL");}

  int G[26][26];build_tab(G);
  int bestm=-1; char blab[128]; int bpt[N]; double bq=0; int bex=0;
  for(int d=0;d<8;d++){int H[26][26];dihedral(G,d,H);int seq[676];int m=0;for(int r=0;r<26;r++)for(int c=0;c<26;c++)seq[m++]=H[r][c];
    for(int ai=0;ai<2;ai++){int*perm=ai?kryp:azp; int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
      for(int conv=0;conv<3;conv++)for(int off=0;off+N<=676;off++){
        int nm=0,pt[N];
        for(int i=0;i<N;i++){int cv=perm[ct[i]];int kv=perm[seq[off+i]];int pv=dec1(conv,cv,kv);pt[i]=inv[pv];if(iscrib[i]&&pt[i]==cribL[i])nm++;}
        if(nm>bestm){bestm=nm;memcpy(bpt,pt,sizeof bpt);bq=qoff(pt);int ex=0;for(int k=0;k<5;k++)if(pt[EXP[k]]==EXL[k]-'A')ex++;bex=ex;
          snprintf(blab,sizeof blab,"dih=%s alpha=%s %s off=%d",DN[d],ai?"KRYPTOS":"AZ",CN[conv],off);}
      }}}
  printf("MEILLEUR match cribs : %s | cribs=%d/24 ex=%d/5 qoff=%.2f\nPT=",blab,bestm,bex,bq);
  for(int i=0;i<N;i++)putchar('A'+bpt[i]);putchar('\n');
  return 0;
}
