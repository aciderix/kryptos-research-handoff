/* runkey_crib.c — BLIND: is the REAL crib-key a running key from a known Kryptos text?
 * crib-key computed 2 ways (AZ: k=C-P; KRYPTOS: k=KAL[(posK C-posK P)]). Compare, at ALL offsets and
 * both directions, to K1/K2/K3 plaintext & ciphertext (and the KRYPTOS keyword stream). A running-key
 * match would show ~24/24 at one offset; random ~1/24. Uses only authentic cribs. */
#include <stdio.h>
#include <string.h>
static const char*KAL="KRYPTOSABCDEFGHIJLMNQUVWXZ";
static int sK[26];
static int md(int x){x%=26;return x<0?x+26:x;}
static const char*K4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
/* known texts */
static const char*K1P="BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION";
static const char*K1C="EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJYQTQUXQBQVYUVLLTREVJYQTMKYRDMFD";
static const char*K2P="ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLETHEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHISTHEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATIONONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTXLAYERTWO";
static const char*K3P="SLOWLYDESPARATLYSLOWLYTHEREMAINSOFPASSAGEDEBRISTHATENCUMBEREDTHELOWERPARTOFTHEDOORWAYWASREMOVEDWITHTREMBLINGHANDSIMADEATINYBREACHINTHEUPPERLEFTHANDCORNERANDTHENWIDENINGTHEHOLEALITTLEIINSERTEDTHECANDLEANDPEEREDINTHEHOTAIRESCAPINGFROMTHECHAMBERCAUSEDTHEFLAMETOFLICKERBUTPRESENTLYDETAILSOFTHEROOMWITHINEMERGEDFROMTHEMISTXCANYOUSEEANYTHINGQ";
static int cp[24],cpl[24];
static void setc(void){const char*e="EASTNORTHEAST",*b="BERLINCLOCK";int n=0;
 for(int i=0;i<13;i++){cp[n]=21+i;cpl[n]=e[i]-'A';n++;}for(int i=0;i<11;i++){cp[n]=63+i;cpl[n]=b[i]-'A';n++;}}
static int bestmatch(const int*key,const char*txt,const char*nm,int useAZ){
 int L=strlen(txt);int best=0,boff=0,bdir=0;
 for(int dir=0;dir<2;dir++)for(int off=0;off<L;off++){int m=0;
   for(int q=0;q<24;q++){int p=cp[q]; int idx=dir?(((off-p)%L+L)%L):((off+p)%L); int tv=txt[idx]-'A'; if(tv==key[p])m++;}
   if(m>best){best=m;boff=off;bdir=dir;}
 }
 printf("  %-6s(%s) best=%2d/24 off=%d dir=%d\n",nm,useAZ?"AZ":"KRY",best,boff,bdir);
 return best;
}
int main(){
 for(int i=0;i<26;i++)sK[KAL[i]-'A']=i; setc();
 int kAZ[97],kKR[97];
 for(int q=0;q<24;q++){int p=cp[q];int C=K4[p]-'A';kAZ[p]=md(C-cpl[q]);kKR[p]=md(sK[C]-sK[cpl[q]]);}
 printf("Running-key crib match vs known texts (want ~24/24; random ~1):\n");
 const char*names[]={"K1P","K1C","K2P","K3P"}; const char*txts[]={K1P,K1C,K2P,K3P};
 for(int t=0;t<4;t++){ bestmatch(kAZ,txts[t],names[t],1); bestmatch(kKR,txts[t],names[t],0); }
 return 0;
}
