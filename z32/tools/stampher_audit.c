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
static long H[4][13]; static char PH[120000][33]; static int PHH[120000]; static long nph=0; static int hb[200000]; static long nb=0; /* heures des phrases de (b) */
static unsigned long long rs=0x9E3779B97F4A7C15ULL; static unsigned long long rnd(void){rs^=rs<<13;rs^=rs>>7;rs^=rs<<17;return rs;}
int main(void){
  const char*INT[13]={"ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN","ELEVEN","TWELVE"};
  const char*FR[27]={"","ANDAHALF","ANDONEHALF","ANDATHIRD","ANDONETHIRD","ANDTWOTHIRDS","ANDAQUARTER","ANDONEQUARTER","ANDAFOURTH","ANDONEFOURTH","ANDTHREEQUARTERS","ANDTHREEFOURTHS","ANDANEIGHTH","ANDONEEIGHTH","ANDTHREEEIGHTHS","ANDFIVEEIGHTHS","ANDSEVENEIGHTHS","ANDASIXTEENTH","ANDONESIXTEENTH","ANDTHREESIXTEENTHS","ANDFIVESIXTEENTHS","ANDSEVENSIXTEENTHS","ANDNINESIXTEENTHS","ANDELEVENSIXTEENTHS","ANDTHIRTEENSIXTEENTHS","ANDFIFTEENSIXTEENTHS",NULL};
  const double FV[26]={0,.5,.5,1./3,1./3,2./3,.25,.25,.25,.25,.75,.75,.125,.125,.375,.625,.875,.0625,.0625,.1875,.3125,.4375,.5625,.6875,.8125,.9375};
  const char*PRE[7]={"","IN","AT","TO","BY","GO","ON"}, *RAD[4]={"RAD","RADS","RADIAN","RADIANS"}, *DU[2]={"INCH","INCHES"};
  char b[256]; long tot=0;
  #define TRY(...) { snprintf(b,256,__VA_ARGS__); tot++; if(strlen(b)==32){ int lk=locks(b), mp=inb(dist,an); H[0][an]++; if(mp){H[1][an]++; hb[nb++]=an; memcpy(PH[nph],b,33); PHH[nph++]=an;} if(lk)H[2][an]++; if(lk&&mp)H[3][an]++; } }
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
  /* AMENDEMENT 1 : motifs de 3 paires aléatoires */
  { const double sc[4][2]={{38.0949,-122.1441},{38.1260,-122.1911},{38.5636,-122.2317},{37.7887,-122.4571}}; int C[13]={0};
    printf("heures des scènes (Stampher, vues du mont Diablo, déclinaison 17°) :");
    for(int k=0;k<4;k++){ double la1=LAT0*M_PI/180,lo1=LON0*M_PI/180,la2=sc[k][0]*M_PI/180,lo2=sc[k][1]*M_PI/180,dl=lo2-lo1;
      double tb=fmod(atan2(sin(dl)*cos(la2),cos(la1)*sin(la2)-sin(la1)*cos(la2)*cos(dl))*180/M_PI+360,360), mb=fmod(tb-DEC+360,360), ck=mb/30; int h=(int)lround(ck); if(h==0)h=12; C[h]=1; printf(" %.2f",ck); }
    printf(" ; C ="); for(int h=1;h<13;h++) if(C[h]) printf(" %d",h); printf("\n");
    /* tri par heure et masques par paire de positions */
    static int ord[120000]; int k=0; int st[14]; for(int h=1;h<=12;h++){ st[h]=k; for(long i=0;i<nph;i++) if(PHH[i]==h) ord[k++]=i; } st[13]=k;
    int NW=(k+63)/64; static unsigned long long *M[32][32];
    for(int i=0;i<32;i++) for(int j=i+1;j<32;j++){ M[i][j]=calloc(NW,8); for(int q=0;q<k;q++){ const char*x=PH[ord[q]]; if(x[i]==x[j]) M[i][j][q>>6]|=1ULL<<(q&63);} }
    long NT=100000, n10=0, c1=0, c2=0; unsigned long long *A=malloc(NW*8);
    for(long t=0;t<NT;t++){ int p[6]; for(;;){ for(int u=0;u<6;u++) p[u]=rnd()%32; int ok=1; for(int u=0;u<6;u++) for(int v=u+1;v<6;v++) if(p[u]==p[v]) ok=0; if(ok) break; }
      int a[3],b2[3]; for(int u=0;u<3;u++){ a[u]=p[2*u]<p[2*u+1]?p[2*u]:p[2*u+1]; b2[u]=p[2*u]<p[2*u+1]?p[2*u+1]:p[2*u]; }
      for(int w=0;w<NW;w++) A[w]=M[a[0]][b2[0]][w]&M[a[1]][b2[1]][w]&M[a[2]][b2[2]][w];
      int cnt[13]={0}, tot2=0; for(int h=1;h<=12;h++){ for(int q=st[h];q<st[h+1];q++) if(A[q>>6]>>(q&63)&1) cnt[h]++; tot2+=cnt[h]; }
      if(tot2<10) continue; n10++; int h1=1,h2=2; if(cnt[h2]>cnt[h1]){h1=2;h2=1;} for(int h=3;h<=12;h++){ if(cnt[h]>cnt[h1]){h2=h1;h1=h;} else if(cnt[h]>cnt[h2]) h2=h; }
      double share=(double)(cnt[h1]+cnt[h2])/tot2; if(share>=0.87){ c1++; if(C[h1]&&C[h2]) c2++; } }
    printf("motifs aléatoires : %ld, dont %ld avec >= 10 survivants\n",NT,n10);
    printf("p1 = P(part des 2 heures dominantes >= 0,87) = %ld/%ld = %.4f\n",c1,n10,(double)c1/n10);
    printf("p2 = P(part >= 0,87 et les 2 heures dans C) = %ld/%ld = %.4f\n",c2,n10,(double)c2/n10); }
  return 0; }
