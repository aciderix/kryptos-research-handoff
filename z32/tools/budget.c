/* Z32-E01 — besoin en homophones d'un clair candidat sous le motif du Z32, jugé avec les clés connues du Zodiac.
 * Voir experiments/E01_budget_homophones/PREREGISTRATION.md.
 * Usage (depuis z32/) :
 *   budget ctrl                      calibration : fenêtres de 32 du Z408 jugées avec K340, du Z340 (section 1) avec K408
 *   budget cand FICHIER              candidats : une ligne = CLAIR [a b]  (route optionnelle : position(i) = (a + b*i) mod 32)
 *   budget corpus FICHIER            leurres : fenêtres de 32 lettres d'un texte satisfaisant les verrous du Z32
 *   budget stampher                  leurres : grammaire de Stampher (reproduite de dstampher/zodiac-z32-cipher, z32.py)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* comptes d'homophones (A..Z) */
static int K408[26], K340[26];
static void set_keys(void){
  for(int i=0;i<26;i++){K408[i]=1;K340[i]=1;}
  const char*a="E7I4T4O4N4A4S4L3R3H2F2D2", *b="E6T6I5A5R5N5O4S4U3L3D3P2Y2W2B2";
  for(const char*q=a;*q;q+=2) K408[q[0]-'A']=q[1]-'0';
  for(const char*q=b;*q;q+=2) K340[q[0]-'A']=q[1]-'0'; }

static int readfile(const char*p,char*buf,int max){ FILE*f=fopen(p,"r"); if(!f){fprintf(stderr,"absent : %s\n",p); exit(2);} int n=0,ch; while((ch=fgetc(f))!=EOF&&n<max) if(ch>' ') buf[n++]=ch; buf[n]=0; fclose(f); return n; }

/* classes de symboles d'une suite de symboles quelconques */
static void classes_of(const char*sym,int n,int*cls){ int k=0; for(int i=0;i<n;i++){ cls[i]=-1; for(int j=0;j<i;j++) if(sym[j]==sym[i]){cls[i]=cls[j];break;} if(cls[i]<0) cls[i]=k++; } }

typedef struct{ int locks; int k[26], m[26]; int X408, X340; double l408, l340; } Score;
/* lettres let[i] (0..25) aux positions i, classes cls[i] */
static Score score(const int*let,const int*cls,int n){
  Score s; memset(&s,0,sizeof s); s.locks=1; int seen[64][26]; memset(seen,0,sizeof seen); int clet[64]; for(int c=0;c<64;c++) clet[c]=-1;
  for(int i=0;i<n;i++){ int c=cls[i],L=let[i]; if(clet[c]>=0&&clet[c]!=L) s.locks=0; clet[c]=L; s.m[L]++; if(!seen[c][L]){seen[c][L]=1; s.k[L]++;} }
  if(!s.locks) return s;
  for(int L=0;L<26;L++){ if(!s.m[L]) continue;
    s.X408+= s.k[L]>K408[L]?s.k[L]-K408[L]:0; s.X340+= s.k[L]>K340[L]?s.k[L]-K340[L]:0;
    for(int t=0;t<2;t++){ int nn=t?K340[L]:K408[L]; double v; if(s.k[L]>nn) v=-INFINITY; else { v=-s.m[L]*log(nn); for(int j=0;j<s.k[L];j++) v+=log(nn-j); } if(t) s.l340+=v; else s.l408+=v; } }
  return s; }
static void kvec(const Score*s,char*out){ int o=0; for(int L=0;L<26;L++) if(s->m[L]&&s->k[L]>1) o+=sprintf(out+o,"%c%d ",'A'+L,s->k[L]); if(!o) strcpy(out,"-"); }

static int Z32CLS[32];
static void load_z32(void){ char b[64]; int n=readfile("data/z32_cipher_oranchak.txt",b,63); if(n!=32){fprintf(stderr,"Z32 : %d symboles\n",n); exit(3);} classes_of(b,32,Z32CLS); }

static void hist(const char*name,int*X,int n){ int h[40]={0}; for(int i=0;i<n;i++) h[X[i]<39?X[i]:39]++; printf("%s (n=%d) :",name,n); int cum=0; for(int v=0;v<40;v++) if(h[v]){cum+=h[v]; printf(" X=%d:%d (cum %.3f)",v,h[v],(double)cum/n);} printf("\n"); }

int main(int argc,char**argv){
  if(argc<2) return 1; set_keys(); load_z32();
  if(!strcmp(argv[1],"ctrl")){
    static char c408[500],p408[500],c340[500],p340[500]; readfile("data/z408_cipher_oranchak.txt",c408,499); readfile("data/z408_plaintext_aligned.txt",p408,499);
    readfile("data/z340_cipher_oranchak.txt",c340,499); readfile("data/z340_plaintext.txt",p340,499);
    static int X1[400],X2[400]; int n1=0,n2=0;
    for(int w=0;w+32<=390;w++){ int let[32],cls[32]; classes_of(c408+w,32,cls); for(int i=0;i<32;i++) let[i]=p408[w+i]-'A'; Score s=score(let,cls,32); if(!s.locks){ continue; } X1[n1++]=s.X340; }
    char s1[153]; for(int k=0;k<153;k++) s1[k]=c340[17*(k%9)+(2*k)%17];
    for(int w=0;w+32<=153;w++){ int let[32],cls[32]; classes_of(s1+w,32,cls); for(int i=0;i<32;i++) let[i]=p340[w+i]-'A'; Score s=score(let,cls,32); if(!s.locks) continue; X2[n2++]=s.X408; }
    hist("Z408 (clair réel) jugé avec K340",X1,n1); hist("Z340 section 1 (clair réel) jugé avec K408",X2,n2);
    int v=0; for(;v<40;v++){ int a=0,b=0; for(int i=0;i<n1;i++) a+=X1[i]<=v; for(int i=0;i<n2;i++) b+=X2[i]<=v; if(a>=0.95*n1&&b>=0.95*n2) break; }
    printf("seuil v (X <= v pour >= 95 %% des fenêtres, deux sens) = %d\n",v); return 0; }
  if(!strcmp(argv[1],"cand")){
    FILE*f=fopen(argv[2],"r"); char line[512];
    while(fgets(line,sizeof line,f)){ char txt[128]; int a=0,b=1; int nf=sscanf(line,"%127s %d %d",txt,&a,&b); if(nf<1||txt[0]=='#') continue;
      if((int)strlen(txt)!=32){ printf("%-34s longueur %d : hors champ\n",txt,(int)strlen(txt)); continue; }
      int let[32]; if(nf<3){a=0;b=1;} for(int i=0;i<32;i++) let[(a+b*i)%32]=txt[i]-'A'; /* lettre i du clair -> position chiffrée (a+b i) mod 32 */
      Score s=score(let,Z32CLS,32); char kv[200]; kvec(&s,kv);
      if(!s.locks) printf("%-34s route(%d,%d) VERROUS VIOLÉS\n",txt,a,b);
      else printf("%-34s route(%d,%d) X408=%d X340=%d l408=%.2f l340=%.2f besoins>1 : %s\n",txt,a,b,s.X408,s.X340,s.l408,s.l340,kv); }
    return 0; }
  if(!strcmp(argv[1],"corpus")){
    static char t[4000000]; FILE*f=fopen(argv[2],"r"); int n=0,ch; while((ch=fgetc(f))!=EOF&&n<3999999) if(ch>='A'&&ch<='Z') t[n++]=ch; else if(ch>='a'&&ch<='z') t[n++]=ch-32; fclose(f);
    static int X1[200000],X2[200000]; int nk=0,tot=0,both=0;
    for(int w=0;w+32<=n;w++){ int let[32]; for(int i=0;i<32;i++) let[i]=t[w+i]-'A'; tot++; Score s=score(let,Z32CLS,32); if(!s.locks) continue; X1[nk]=s.X408; X2[nk]=s.X340; nk++; }
    printf("fenêtres %d ; satisfaisant les verrous %d\n",tot,nk); hist("leurres corpus : X408",X1,nk); hist("leurres corpus : X340",X2,nk);
    if(argc>3){ int v=atoi(argv[3]); for(int i=0;i<nk;i++) both+=(X1[i]>v&&X2[i]>v); printf("incompatibles (X408>%d et X340>%d) : %d/%d = %.3f\n",v,v,both,nk,(double)both/(nk?nk:1)); }
    return 0; }
  if(!strcmp(argv[1],"stampher")){
    const char*INT[13]={"ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN","ELEVEN","TWELVE"};
    const char*FR[27]={"","ANDAHALF","ANDONEHALF","ANDATHIRD","ANDONETHIRD","ANDTWOTHIRDS","ANDAQUARTER","ANDONEQUARTER","ANDAFOURTH","ANDONEFOURTH","ANDTHREEQUARTERS","ANDTHREEFOURTHS","ANDANEIGHTH","ANDONEEIGHTH","ANDTHREEEIGHTHS","ANDFIVEEIGHTHS","ANDSEVENEIGHTHS","ANDASIXTEENTH","ANDONESIXTEENTH","ANDTHREESIXTEENTHS","ANDFIVESIXTEENTHS","ANDSEVENSIXTEENTHS","ANDNINESIXTEENTHS","ANDELEVENSIXTEENTHS","ANDTHIRTEENSIXTEENTHS","ANDFIFTEENSIXTEENTHS",NULL};
    const char*PRE[7]={"","IN","AT","TO","BY","GO","ON"}, *RAD[4]={"RAD","RADS","RADIAN","RADIANS"}, *DU[2]={"INCH","INCHES"};
    long tot=0,len32=0; int nk=0; static int X1[100000],X2[100000]; char buf[256]; int v=argc>2?atoi(argv[2]):-1; int both=0;
    #define TRY(...) { snprintf(buf,256,__VA_ARGS__); tot++; if(strlen(buf)==32){ len32++; int let[32]; for(int i=0;i<32;i++) let[i]=buf[i]-'A'; Score s=score(let,Z32CLS,32); if(s.locks){ X1[nk]=s.X408; X2[nk]=s.X340; nk++; if(v>=0&&s.X408>v&&s.X340>v) both++; if(getenv("LIST")) printf("  %s X408=%d X340=%d\n",buf,s.X408,s.X340);} } }
    for(int d=0;d<13;d++) for(int an=1;an<13;an++) for(int fi=0;FR[fi];fi++){ const char*D=INT[d],*A=INT[an],*F=FR[fi];
      for(int r=0;r<4;r++){ for(int p=0;p<7;p++){ const char*P=PRE[p],*R=RAD[r];
          TRY("%s%s%s%s%s",P,D,F,R,A) TRY("%s%s%s%s%s",P,A,R,D,F) TRY("%s%s%s%s%s",P,D,F,A,R) TRY("%s%s%s%s%s",P,A,D,F,R) TRY("%s%s%s%s%s",P,R,A,D,F) TRY("%s%s%s%s%s",P,R,D,F,A) }
        for(int u=0;u<2;u++) for(int p=0;p<7;p++){ const char*P=PRE[p],*R=RAD[r],*U=DU[u];
          TRY("%s%s%s%s%s%s",P,D,F,U,R,A) TRY("%s%s%s%s%s%s",P,D,F,U,A,R) TRY("%s%s%s%s%s%s",P,A,R,D,F,U) TRY("%s%s%s%s%s%s",P,A,D,F,U,R) TRY("%s%s%s%s%s%s",P,U,D,F,R,A) TRY("%s%s%s%s%s%s",P,R,A,U,D,F) } } }
    printf("grammaire de Stampher : %ld phrases, %ld de 32 lettres, %d satisfont les verrous\n",tot,len32,nk);
    hist("leurres Stampher : X408",X1,nk); hist("leurres Stampher : X340",X2,nk); if(v>=0) printf("incompatibles (X408>%d et X340>%d) : %d/%d\n",v,v,both,nk);
    return 0; }
  return 1; }
