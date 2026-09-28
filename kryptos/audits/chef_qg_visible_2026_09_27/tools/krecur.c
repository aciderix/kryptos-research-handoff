/* krecur.c — la clé K=C-P satisfait-elle une récurrence linéaire ? Testé sur les 2 BLOCS FIABLES
 * (positions crib 21-33 et 63-73, K connu indépendamment de la reconstruction). */
#include <stdio.h>
static int md(int x){x%=26;return x<0?x+26:x;}
int main(void){
 int B1[13]={1,11,25,2,3,2,24,24,6,2,10,0,25};   /* K[21..33] = BLZCDCYYGCKAZ */
 int B2[11]={12,20,24,10,11,6,10,14,17,13,0};     /* K[63..73] = MUYKLGKORNA */
 /* order-2: K_i=(a*K_{i-1}+b*K_{i-2}+c) mod26 ; fit within each block, want fits ALL in both */
 int best=-1,ba=0,bb=0,bc=0;
 for(int a=0;a<26;a++)for(int b=0;b<26;b++)for(int c=0;c<26;c++){
   int f=0,tot=0;
   for(int i=2;i<13;i++){tot++;if(md(a*B1[i-1]+b*B1[i-2]+c)==B1[i])f++;}
   for(int i=2;i<11;i++){tot++;if(md(a*B2[i-1]+b*B2[i-2]+c)==B2[i])f++;}
   if(f>best){best=f;ba=a;bb=b;bc=c;}
 }
 printf("order-2 best recurrence a=%d b=%d c=%d fits %d/20 (both blocks)\n",ba,bb,bc,best);
 /* order-1: K_i=(a*K_{i-1}+c) */
 int best1=-1,a1=0,c1=0;
 for(int a=0;a<26;a++)for(int c=0;c<26;c++){int f=0;
   for(int i=1;i<13;i++)if(md(a*B1[i-1]+c)==B1[i])f++;
   for(int i=1;i<11;i++)if(md(a*B2[i-1]+c)==B2[i])f++;
   if(f>best1){best1=f;a1=a;c1=c;}}
 printf("order-1 best a=%d c=%d fits %d/22\n",a1,c1,best1);
 /* Gromark-style lag: K_i=(K_{i-n}+K_{i-n+1}) mod26 within blocks, n=1..4 */
 /* differences within block (is K an arithmetic prog? const diff?) */
 printf("B1 diffs:"); for(int i=1;i<13;i++)printf(" %d",md(B1[i]-B1[i-1])); printf("\n");
 printf("B2 diffs:"); for(int i=1;i<11;i++)printf(" %d",md(B2[i]-B2[i-1])); printf("\n");
 return 0;}
