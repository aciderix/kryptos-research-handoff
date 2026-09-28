/* chao.c — CHAOCIPHER (Byrne). 1:1, 2 alphabets dynamiques, graine = 2 alphabets (mots-clés).
 * Fit Scheidt: hand-cipher, keyword, removes bias (IC plat), "more than one step", change of base.
 * Permutation standard (zenith=0, nadir=13).
 * encrypt(p): i=index of p in RIGHT; c=LEFT[i]; then permute LEFT then RIGHT.
 * decrypt(c): i=index of c in LEFT; p=RIGHT[i]; then permute LEFT then RIGHT (same as encrypt after).
 * Compile: cc -O2 -o chao chao.c -lm ; Usage: chao selftest | chao attack qg CT leftAlpha rightAlpha */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void rotate(char*a,int n){char t[26];for(int i=0;i<26;i++)t[i]=a[(i+n)%26];memcpy(a,t,26);}
static void permLEFT(char*L){ /* after ct found at zenith(0): extract pos1, shift 2..13 ->1..12, insert at 13 */
  char x=L[1]; for(int i=1;i<13;i++)L[i]=L[i+1]; L[13]=x; }
static void permRIGHT(char*R){ /* after pt at zenith then +1: extract pos2, shift 3..13->2..12, insert 13 */
  char x=R[2]; for(int i=2;i<13;i++)R[i]=R[i+1]; R[13]=x; }
static void enc_step(char*L,char*R,int pv,int*cvout){
  int i=0;for(int k=0;k<26;k++)if(R[k]==pv){i=k;break;} int cv=L[i]; if(cvout)*cvout=cv;
  rotate(L,i); permLEFT(L); rotate(R,i); rotate(R,1); permRIGHT(R); }
static void dec_step(char*L,char*R,int cv,int*pvout){
  int i=0;for(int k=0;k<26;k++)if(L[k]==cv){i=k;break;} int pv=R[i]; if(pvout)*pvout=pv;
  rotate(L,i); permLEFT(L); rotate(R,i); rotate(R,1); permRIGHT(R); }
#define N 97
static const int CE[13]={21,22,23,24,25,26,27,28,29,30,31,32,33};
static const int CB[11]={63,64,65,66,67,68,69,70,71,72,73};
static int iscrib[N],cribL[N]; static float*QG;
int main(int argc,char**argv){
 const char*e="EASTNORTHEAST",*b="BERLINCLOCK";
 for(int i=0;i<13;i++){iscrib[CE[i]]=1;cribL[CE[i]]=e[i]-'A';}
 for(int i=0;i<11;i++){iscrib[CB[i]]=1;cribL[CB[i]]=b[i]-'A';}
 if(argc>=2&&!strcmp(argv[1],"selftest")){
   char L[26],R[26]; for(int i=0;i<26;i++){L[i]="KRYPTOSABCDEFGHIJLMNQUVWXZ"[i]-'A';R[i]="PALIMSETBCDFGHJKNOQRUVWXYZ"[i]-'A';}
   char L0[26],R0[26];memcpy(L0,L,26);memcpy(R0,R,26);
   const char*pt="WEAREINTHEEASTNORTHEASTLOOKINGATTHEBERLINCLOCKUNDERSTARSXYZ";
   int n=strlen(pt);int ct[128];
   for(int i=0;i<n;i++){int cv;enc_step(L,R,pt[i]-'A',&cv);ct[i]=cv;}
   memcpy(L,L0,26);memcpy(R,R0,26);int ok=1;
   for(int i=0;i<n;i++){int pv;dec_step(L,R,ct[i],&pv); if(pv!=pt[i]-'A')ok=0;}
   printf("chao selftest %s\n",ok?"OK":"FAIL");return ok?0:1;
 }
 if(argc>=6&&!strcmp(argv[1],"attack")){
   FILE*f=fopen(argv[2],"rb");QG=malloc(4*456976);if(fread(QG,4,456976,f)!=456976)return 2;fclose(f);
   const char*cts=argv[3];int ct[N];for(int i=0;i<N;i++)ct[i]=cts[i]-'A';
   const char*la=argv[4],*ra=argv[5]; char L[26],R[26];for(int i=0;i<26;i++){L[i]=la[i]-'A';R[i]=ra[i]-'A';}
   int pt[N];for(int i=0;i<N;i++){int pv;dec_step(L,R,ct[i],&pv);pt[i]=pv;}
   int m=0;for(int i=0;i<N;i++)if(iscrib[i]&&pt[i]==cribL[i])m++;
   double qo=0;int no=0;for(int i=0;i+3<N;i++){int in=0;for(int d=0;d<4;d++)in|=iscrib[i+d];if(!in){qo+=QG[((pt[i]*26+pt[i+1])*26+pt[i+2])*26+pt[i+3]];no++;}}
   printf("cribs=%d/24 qoff=%.4f PT=",m,no?qo/no:0);for(int i=0;i<N;i++)putchar('A'+pt[i]);putchar('\n');return 0;
 }
 return 1;}
