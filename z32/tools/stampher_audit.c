/* Z32-E04 — audit du « biais structurel » de Stampher : reproduction exacte de z32.py (dstampher/zodiac-z32-cipher)
 * avec ses constantes (data.py) et sa projection (geo.py), puis distribution des heures avec / sans verrous / carte. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static const double R=3958.8, DEC=17.0, SCALE=6.4, LAT0=37.881628, LON0=-121.914382, S_=37.3, N_=38.8, W_=-123.0, E_=-121.0;
static int inb(double d,int hr){ double brg=fmod((hr%12)*30+DEC,360)*M_PI/180, dd=d*SCALE/R, la1=LAT0*M_PI/180, lo1=LON0*M_PI/180;
  double la2=asin(sin(la1)*cos(dd)+cos(la1)*sin(dd)*cos(brg)); double lo2=lo1+atan2(sin(brg)*sin(dd)*cos(la1),cos(dd)-sin(la1)*sin(la2));
  double la=la2*180/M_PI, lo=lo2*180/M_PI; return la>=S_&&la<=N_&&lo>=W_&&lo<=E_; }
static int locks(const char*b){ return b[0]==b[25]&&b[1]==b[31]&&b[5]==b[13]; }
static long H[4][13]; static int hb[200000]; static long nb=0; /* heures des phrases de (b) */
static unsigned long long rs=0x9E3779B97F4A7C15ULL; static unsigned long long rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
int main(void){
  const char*INT[13]={"ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN","ELEVEN","TWELVE"};
  const char*FR[27]={"","ANDAHALF","ANDONEHALF","ANDATHIRD","ANDONETHIRD","ANDTWOTHIRDS","ANDAQUARTER","ANDONEQUARTER","ANDAFOURTH","ANDONEFOURTH","ANDTHREEQUARTERS","ANDTHREEFOURTHS","ANDANEIGHTH","ANDONEEIGHTH","ANDTHREEEIGHTHS","ANDFIVEEIGHTHS","ANDSEVENEIGHTHS","ANDASIXTEENTH","ANDONESIXTEENTH","ANDTHREESIXTEENTHS","ANDFIVESIXTEENTHS","ANDSEVENSIXTEENTHS","ANDNINESIXTEENTHS","ANDELEVENSIXTEENTHS","ANDTHIRTEENSIXTEENTHS","ANDFIFTEENSIXTEENTHS",NULL};
  const double FV[26]={0,.5,.5,1./3,1./3,2./3,.25,.25,.25,.25,.75,.75,.125,.125,.375,.625,.875,.0625,.0625,.1875,.3125,.4375,.5625,.6875,.8125,.9375};
  const char*PRE[7]={"","IN","AT","TO","BY","GO","ON"}, *RAD[4]={"RAD","RADS","RADIAN","RADIANS"}, *DU[2]={"INCH","INCHES"};
  char b[256]; long tot=0;
  #define TRY(...) { snprintf(b,256,__VA_ARGS__); tot++; if(strlen(b)==32){ int lk=locks(b), mp=inb(dist,an); H[0][an]++; if(mp){H[1][an]++; hb[nb++]=an;} if(lk)H[2][an]++; if(lk&&mp)H[3][an]++; } }
  for(int d=0;d<13;d++) for(int an=1;an<13;an++) for(int fi=0;FR[fi];fi++){ const char*D=INT[d],*A=INT[an],*F=FR[fi]; double dist=d+FV[fi];
    for(int r=0;r<4;r++){ for(int p=0;p<7;p++){ const char*P=PRE[p],*Rd=RAD[r];
        TRY("%s%s%s%s%s",P,D,F,Rd,A) TRY("%s%s%s%s%s",P,A,Rd,D,F) TRY("%s%s%s%s%s",P,D,F,A,Rd) TRY("%s%s%s%s%s",P,A,D,F,Rd) TRY("%s%s%s%s%s",P,Rd,A,D,F) TRY("%s%s%s%s%s",P,Rd,D,F,A) }
      for(int u=0;u<2;u++) for(int p=0;p<7;p++){ const char*P=PRE[p],*Rd=RAD[r],*U=DU[u];
        TRY("%s%s%s%s%s%s",P,D,F,U,Rd,A) TRY("%s%s%s%s%s%s",P,D,F,U,A,Rd) TRY("%s%s%s%s%s%s",P,A,Rd,D,F,U) TRY("%s%s%s%s%s%s",P,A,D,F,U,Rd) TRY("%s%s%s%s%s%s",P,U,D,F,Rd,A) TRY("%s%s%s%s%s%s",P,Rd,A,U,D,F) } } }
  const char*nm[4]={"(a) 32 lettres","(b) + carte","(c) + verrous","(d) + verrous + carte"};
  printf("phrases générées : %ld\n",tot);
  double part[4];
  for(int k=0;k<4;k++){ long s=0; for(int h=1;h<13;h++) s+=H[k][h]; part[k]=(double)(H[k][8]+H[k][10])/s;
    printf("%-24s n=%7ld  8h+10h = %.3f  | par heure :",nm[k],s,part[k]); for(int h=1;h<13;h++) printf(" %d:%ld",h,H[k][h]); printf("\n"); }
  long s3=0; for(int h=1;h<13;h++) s3+=H[3][h]; int obs=H[3][8]+H[3][10]; long ge=0, NT=100000;
  for(long t=0;t<NT;t++){ int c=0; for(int i=0;i<s3;i++){ int h=hb[rnd()%nb]; c+=(h==8||h==10);} ge+=c>=obs; }
  printf("enrichissement annoncé (vs 2/12) : %.2f ; enrichissement réel dû aux verrous (d)/(b) : %.2f\n",part[3]/(2.0/12),part[3]/part[1]);
  printf("tirage de %ld phrases dans (b), %ld fois : p(part 8h+10h >= %d/%ld) = %.5f\n",s3,NT,obs,s3,(double)(ge+1)/(NT+1));
  return 0; }
