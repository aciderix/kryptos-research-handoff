/* periodic_known.c — Vigenère/Beaufort/variante PÉRIODIQUE (clé répétée, PAS autoclé) à alphabet
 * PUBLIC (AZ / KRYPTOS / PALIMPSEST / ABSCISSA keyés). Motivé par l'excès RÉEL de coïncidences à
 * l'écart 7 (signature d'une clé de période 7). Pour chaque période P=1..40, on DÉRIVE la clé des
 * cribs (majorité par résidu i%P), on compte les erreurs de crib, on déchiffre et on score qg_big.
 * Si une (P,alph,forme) a 0-1 erreur ET qoff anglais ⇒ solution. Contrôle positif + null.
 * Usage : periodic_known qg_big.bin
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
static inline int md(int x){x%=26;return x<0?x+26:x;}
static const char*KW[4]={"","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*BN[4]={"AZ","KRYPTOS","PALIMPSEST","ABSCISSA"};
static const char*CN[3]={"VIG","BEA","VAR"};
static void mkperm(int b,int*perm){int seen[26]={0},al[26],n=0;
  for(const char*p=KW[b];*p;p++){int L=*p-'A';if(!seen[L]){seen[L]=1;al[n++]=L;}}
  for(int L=0;L<26;L++)if(!seen[L])al[n++]=L; for(int i=0;i<26;i++)perm[al[i]]=i;}
static float*QG;
static double qoff(const int*p){double q=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}}return no?q/no:0;}
/* clé requise en un crib : forme VIG c=p+k ⇒ k=c-p ; BEA c=k-p ⇒ k=c+p ; VAR c=p-k ⇒ k=p-c */
static int kreq(int m,int cv,int pv){return m==0?md(cv-pv):m==1?md(cv+pv):md(pv-cv);}
static int dec(int m,int cv,int kv){return m==0?md(cv-kv):m==1?md(kv-cv):md(cv+kv);}
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}
  if(argc<2){fprintf(stderr,"usage: periodic_known qg_big.bin\n");return 1;}
  FILE*q=fopen(argv[1],"rb");QG=malloc(sizeof(float)*456976);if(fread(QG,4,456976,q)!=456976){return 2;}fclose(q);
  int ct[N];for(int i=0;i<N;i++)ct[i]=K4[i]-'A';
  /* CONTROLE POSITIF : période 7 Vig AZ, clé plantée, English+cribs -> doit se retrouver 0 err */
  {int perm[26];mkperm(0,perm);int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
   int key[7]={3,14,7,21,4,9,17};
   const char*eng="WEAREINTHEEASTNORTHEASTQUADRANTLOOKINGTOWARDTHEBERLINCLOCKATMIDNIGHTUNDERTHESTARSXYZFILLERWORDSOK";
   int pt0[N];for(int i=0;i<N;i++)pt0[i]=eng[i]-'A';for(int i=0;i<N;i++)if(iscrib[i])pt0[i]=cribL[i];
   int c0[N];for(int i=0;i<N;i++)c0[i]=inv[md(perm[pt0[i]]+key[i%7])];
   /* redérive clé des cribs et déchiffre */
   int kk[7],cnt[7][26];memset(cnt,0,sizeof cnt);
   for(int i=0;i<N;i++)if(iscrib[i]){int r=i%7;cnt[r][kreq(0,perm[c0[i]],perm[cribL[i]])]++;}
   for(int r=0;r<7;r++){int bk=0,bc=-1;for(int v=0;v<26;v++)if(cnt[r][v]>bc){bc=cnt[r][v];bk=v;}kk[r]=bk;}
   int rec=0;for(int i=0;i<N;i++){int pv=dec(0,perm[c0[i]],kk[i%7]);if(inv[pv]==pt0[i])rec++;}
   printf("CONTROLE POSITIF (période 7 Vig AZ): recovered=%d/97 -> %s\n",rec,rec==N?"PASS":"FAIL");}

  double bestq=-1e9;char blab[128];int bpt[N];int bcrib=0; int bestminerr=99;char elab[128];
  for(int P=1;P<=40;P++)for(int bI=0;bI<4;bI++){int perm[26];mkperm(bI,perm);int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
    for(int m=0;m<3;m++){
      /* clé majoritaire par résidu */
      static int cnt[40][26]; memset(cnt,0,sizeof cnt);
      for(int i=0;i<N;i++)if(iscrib[i])cnt[i%P][kreq(m,perm[ct[i]],perm[cribL[i]])]++;
      int key[40],haskey[40];for(int r=0;r<P;r++){int bk=0,bc=-1;for(int v=0;v<26;v++)if(cnt[r][v]>bc){bc=cnt[r][v];bk=v;}key[r]=bk;haskey[r]=(bc>0);}
      int pt[N],err=0,cov=1;
      for(int i=0;i<N;i++){if(!haskey[i%P])cov=0;int pv=dec(m,perm[ct[i]],key[i%P]);pt[i]=inv[pv];if(iscrib[i]&&pt[i]!=cribL[i])err++;}
      if(!cov)continue; /* période avec résidu non couvert par un crib : indéfini, on saute */
      double qq=qoff(pt);
      if(qq>bestq){bestq=qq;memcpy(bpt,pt,sizeof bpt);bcrib=24-err;snprintf(blab,sizeof blab,"P=%d %s %s (cribErr=%d)",P,BN[bI],CN[m],err);}
      if(err<bestminerr){bestminerr=err;snprintf(elab,sizeof elab,"P=%d %s %s qoff=%.2f",P,BN[bI],CN[m],qq);}
    }}
  printf("MEILLEUR qoff : %.3f  [%s]  (cribs %d/24)\n  PT=",bestq,blab,bcrib);for(int i=0;i<N;i++)putchar('A'+bpt[i]);putchar('\n');
  printf("MIN cribErr : %d  [%s]\n",bestminerr,elab);
  printf("=> %s\n",(bestminerr<=1 && bestq>-2.6)?"À EXAMINER":"NÉGATIF (aucune période known-alphabet ne colle cribs+anglais)");
  return 0;
}
