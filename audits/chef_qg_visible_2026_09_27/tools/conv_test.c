/* conv_test.c — RIGOROUS: lock the exact Quagmire convention by recovering PALIMPSEST on K1 myself,
 * then apply the SAME convention to K4's authentic cribs to read the real crib-key AS KEYWORD LETTERS.
 * KRYPTOS keyed alphabet KAL. Test all sensible index/letter conventions; report which yields the
 * periodic keyword PALIMPSEST on K1. No reconstructed plaintext anywhere.
 */
#include <stdio.h>
#include <string.h>
#define AL "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
static const char*KAL="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int sK[26]; /* index of a letter in keyed alphabet */
static int md(int x){x%=26;return x<0?x+26:x;}
/* K1 */
static const char*C1="EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD";
static const char*P1="BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION";
/* K4 */
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";

/* candidate key-letter under convention id, given cipher c and plain p (letters 0..25) */
static int keyletter(int id,int c,int p){
  int Kc=sK[c],Kp=sK[p]; /* keyed indices */
  int Ac=c,Ap=p;         /* straight indices */
  int d;
  switch(id){
    case 0: d=md(Kc-Kp); return AL[d]-'A';            /* keyed diff -> straight letter */
    case 1: d=md(Kc-Kp); return KAL[d]-'A';           /* keyed diff -> keyed letter */
    case 2: d=md(Kp-Kc); return AL[d]-'A';            /* Beaufort keyed -> straight */
    case 3: d=md(Kp-Kc); return KAL[d]-'A';
    case 4: d=md(Ac-Ap); return KAL[d]-'A';           /* straight diff -> keyed letter (Quagmire II) */
    case 5: d=md(Ac-Ap); return AL[d]-'A';            /* straight Vigenere */
    case 6: d=md(Kc+Kp); return KAL[d]-'A';
    case 7: d=md(Kc+Kp); return AL[d]-'A';
    /* Quagmire III proper: key letter K positions the keyed alphabet; cipher = keyed[ (straightpos(plain)+straightpos(key))?]... try: sA(C)=(sK(P)+sK(key))? */
    case 8: d=md(Ac-Kp); return KAL[d]-'A';
    case 9: d=md(Kc-Ap); return KAL[d]-'A';
  }
  return 0;
}
int main(void){
  for(int i=0;i<26;i++)sK[KAL[i]-'A']=i;
  int n1=strlen(C1);
  printf("K1 len=%d. Testing conventions (want repeating PALIMPSEST):\n",n1);
  int best=-1;
  for(int id=0;id<10;id++){
    char key[128];
    for(int i=0;i<n1;i++) key[i]='A'+keyletter(id,C1[i]-'A',P1[i]-'A');
    key[n1]=0;
    /* check periodicity 10 == PALIMPSEST */
    int isPal = strncmp(key,"PALIMPSEST",10)==0;
    printf("  id=%d key[0..19]=%.20s %s\n",id,key, isPal?"  <== PALIMPSEST!":"");
    if(isPal) best=id;
  }
  if(best<0){ printf("no convention recovered PALIMPSEST — inspect above\n"); return 0; }
  printf("\nLOCKED convention id=%d. Applying to K4 authentic cribs:\n",best);
  const char*e="EASTNORTHEAST"; int ep=21;
  const char*b="BERLINCLOCK";   int bp=63;
  printf("EAST crib key (pos21-33): ");
  for(int i=0;i<13;i++) putchar('A'+keyletter(best,K4[ep+i]-'A',e[i]-'A'));
  printf("\nBERLIN crib key (pos63-73): ");
  for(int i=0;i<11;i++) putchar('A'+keyletter(best,K4[bp+i]-'A',b[i]-'A'));
  printf("\n");
  return 0;
}
