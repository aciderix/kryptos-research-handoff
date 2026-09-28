/* runkey_ct.c — clé courante = CHIFFRÉ K1/K2/K3 (768 lettres, 100% public, gravé) appliqué à K4.
 * Le chef a testé les CLAIRS résolus K1/K2/K3 (négatif) ; les CHIFFRÉS n'avaient pas été testés.
 * On glisse K4 sur toute la clé (tous offsets), avant+arrière, VIG/BEA/VAR, alphabets AZ & KRYPTOS.
 * Score : cribs (EASTNORTHEAST@21, BERLINCLOCK@63) + qoff (qg_big). Null : keystream aléatoire.
 * Usage : runkey_ct k123_ct.txt qg_big.bin
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
static const char*AZ="ABCDEFGHIJKLMNOPQRSTUVWXYZ",*KRY="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static const char*CN[3]={"VIG","BEA","VAR"};
static void mkperm(const char*a,int*p){for(int i=0;i<26;i++)p[a[i]-'A']=i;}
static int dec1(int cv,int kv,int m){return m==0?md(cv-kv):m==1?md(kv-cv):md(cv+kv);}
static float*QG;
static double qoff(const int*p){double q=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){q+=QG[((p[i]*26+p[i+1])*26+p[i+2])*26+p[i+3]];no++;}}return no?q/no:0;}
int main(int argc,char**argv){
  const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
  for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
  for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}
  if(argc<3){fprintf(stderr,"usage: runkey_ct k123_ct.txt qg_big.bin\n");return 1;}
  FILE*f=fopen(argv[1],"r"); static char key[4096]; int K=0; int ch;
  while((ch=fgetc(f))!=EOF) if(ch>='A'&&ch<='Z') key[K++]=ch; fclose(f);
  FILE*q=fopen(argv[2],"rb");QG=malloc(sizeof(float)*456976); if(fread(QG,4,456976,q)!=456976){return 2;}fclose(q);
  int ct[N];for(int i=0;i<N;i++)ct[i]=K4[i]-'A';
  int azp[26],kryp[26];mkperm(AZ,azp);mkperm(KRY,kryp);
  int keyr[4096]; for(int i=0;i<K;i++)keyr[i]=key[K-1-i];  /* reversed */
  int bestc=-1;double bestq=-1e9;char blab[128];int bpt[N];
  double bqc=-1e9; char qlab[128]; int qpt[N]; int qcr=0;
  for(int dir=0;dir<2;dir++){const char*ks=dir?"rev":"fwd"; char*src=dir?(char*)0:0; int*K_=dir?keyr:NULL;
    for(int off=0;off+N<=K;off++){
      for(int ai=0;ai<2;ai++){int*perm=ai?kryp:azp;int inv[26];for(int i=0;i<26;i++)inv[perm[i]]=i;
        for(int m=0;m<3;m++){int pt[N],nm=0;
          for(int i=0;i<N;i++){int kc=dir?keyr[off+i]:(key[off+i]-'A'); int kv=perm[kc]; int pv=dec1(perm[ct[i]],kv,m); pt[i]=inv[pv]; if(iscrib[i]&&pt[i]==cribL[i])nm++;}
          if(nm>bestc){bestc=nm;memcpy(bpt,pt,sizeof bpt);snprintf(blab,sizeof blab,"%s off=%d %s %s",ks,off,ai?"KRYP":"AZ",CN[m]);}
          double qq=qoff(pt); if(qq>bqc){bqc=qq;memcpy(qpt,pt,sizeof qpt);qcr=nm;snprintf(qlab,sizeof qlab,"%s off=%d %s %s",ks,off,ai?"KRYP":"AZ",CN[m]);}
        }}}
  }
  printf("=== running-key = CHIFFRÉ K1K2K3 (%d lettres) sur K4 ===\n",K);
  printf("MEILLEUR cribs : %d/24  [%s]\n",bestc,blab);
  printf("MEILLEUR qoff  : %.3f (cribs %d/24) [%s]\n  PT=",bqc,qcr,qlab); for(int i=0;i<N;i++)putchar('A'+qpt[i]);putchar('\n');
  /* null : keystreams aléatoires, meme nb de configs ~ prendre max cribs sur 20000 essais */
  unsigned long rs=12345; int nullmax=0,ge=0,T=20000;
  for(int t=0;t<T;t++){int pt,nm=0; for(int i=0;i<N;i++){rs=rs*6364136223846793005UL+1; int kv=(rs>>33)%26; int pv=md(azp[ct[i]]-kv); (void)pt; if(iscrib[i]&&pv==azp[cribL[i]])nm++;} if(nm>nullmax)nullmax=nm; if(nm>=bestc)ge++;}
  printf("null (keystream aléatoire, VIG/AZ): max %d/24 sur %d, P(>=%d)=%.4f\n",nullmax,T,bestc,(double)ge/T);
  printf("=> %s\n", (bestc>nullmax+2 && bqc>-2.6)?"À EXAMINER":"NÉGATIF (dans le hasard)");
  return 0;
}
