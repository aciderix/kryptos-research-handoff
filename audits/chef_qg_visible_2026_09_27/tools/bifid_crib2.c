/* bifid_crib2.c — free-square bifid, OBJECTIF COMBINÉ cribs + quadgram anglais.
 * Décisif : le vrai clair aurait cribs=24 ET anglais lisible. Un carré qui
 * "game" les 24 positions de crib sans faire d'anglais est rejeté par le terme qg.
 * qg.bin = 456976 floats log P(d|abc) (format ac7, build_qg.c).
 * Compile: cc -O2 -o bifid_crib2 bifid_crib2.c -lm
 * Usage:   ./bifid_crib2 qg.bin [restarts_per_period]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const char *K4 =
 "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
#define N 97
static char CRIBP[N];
static float *QG;

static void set_crib(int start,const char*s){ for(int i=0;s[i];i++){char c=s[i]; if(c=='J')c='I'; CRIBP[start+i]=c;} }

static void bifid_decrypt(const int*posOf,const char*sq,int P,char*out){
    int o=0;
    for(int b=0;b<N;b+=P){
        int L=(b+P<=N)?P:(N-b); int d[64];
        for(int i=0;i<L;i++){char c=K4[b+i]; if(c=='J')c='I'; int p=posOf[c-'A']; d[2*i]=p/5; d[2*i+1]=p%5;}
        for(int i=0;i<L;i++){int r=d[i],cc=d[L+i]; out[o++]=sq[r*5+cc];}
    }
    out[o]=0;
}
static int score_cribs(const char*pt){int h=0;for(int i=0;i<N;i++)if(CRIBP[i]>0&&pt[i]==CRIBP[i])h++;return h;}
static double score_qg(const char*pt){
    double s=0;
    for(int i=0;i+3<N;i++){int a=pt[i]-'A',b=pt[i+1]-'A',c=pt[i+2]-'A',d=pt[i+3]-'A';
        if(a<0||a>25||b<0||b>25||c<0||c>25||d<0||d>25){s-=8;continue;}
        s+=QG[((a*26+b)*26+c)*26+d];}
    return s;
}
static void rebuild_pos(const char*sq,int*posOf){for(int i=0;i<26;i++)posOf[i]=-1;for(int i=0;i<25;i++)posOf[sq[i]-'A']=i;posOf['J'-'A']=posOf['I'-'A'];}

/* objectif combiné */
static double obj(const char*pt){ return 30.0*score_cribs(pt) + score_qg(pt); }

int main(int argc,char**argv){
    if(argc<2){fprintf(stderr,"usage: %s qg.bin [restarts]\n",argv[0]);return 1;}
    int RESTARTS=(argc>2)?atoi(argv[2]):1500;
    FILE*f=fopen(argv[1],"rb"); if(!f){perror("qg");return 1;}
    QG=malloc(sizeof(float)*456976); if(fread(QG,sizeof(float),456976,f)!=456976){return 2;} fclose(f);
    srand(999);
    for(int i=0;i<N;i++)CRIBP[i]=-1; set_crib(21,"EASTNORTHEAST"); set_crib(63,"BERLINCLOCK");
    int ncrib=0;for(int i=0;i<N;i++)if(CRIBP[i]>0)ncrib++;
    printf("# FREE-SQUARE BIFID, objectif COMBINÉ (30*cribs + qg). ncrib=%d\n",ncrib);
    char base[26];{int k=0;for(char c='A';c<='Z';c++){if(c=='J')continue;base[k++]=c;}base[25]=0;}
    char out[N+1];
    double gbest=-1e18; int gP=-1,gcr=-1; char gsq[26]={0}; double gqg=0;
    for(int P=3;P<=25;P++){
        double Pb=-1e18; char Psq[26]; int Pcr=0; double Pqg=0;
        for(int rs=0;rs<RESTARTS;rs++){
            char sq[26]; strcpy(sq,base);
            for(int i=24;i>0;i--){int j=rand()%(i+1);char t=sq[i];sq[i]=sq[j];sq[j]=t;}
            int posOf[26]; rebuild_pos(sq,posOf); bifid_decrypt(posOf,sq,P,out);
            double cur=obj(out); double T=6.0;
            for(int it=0;it<2500;it++){
                int a=rand()%25,b=rand()%25; if(a==b)continue;
                char t=sq[a];sq[a]=sq[b];sq[b]=t; rebuild_pos(sq,posOf); bifid_decrypt(posOf,sq,P,out);
                double nn=obj(out), d=nn-cur;
                if(d>=0||exp(d/T)>(rand()/(double)RAND_MAX)) cur=nn;
                else {char u=sq[a];sq[a]=sq[b];sq[b]=u;}
                T*=0.9975; if(T<0.03)T=0.03;
            }
            rebuild_pos(sq,posOf); bifid_decrypt(posOf,sq,P,out);
            double v=obj(out);
            if(v>Pb){Pb=v; strcpy(Psq,sq); Pcr=score_cribs(out); Pqg=score_qg(out);}
        }
        rebuild_pos(Psq,(int[26]){0}); /* noop */
        int posOf[26]; rebuild_pos(Psq,posOf); bifid_decrypt(posOf,Psq,P,out);
        printf("[P=%2d] obj=%.1f cribs=%d/%d qg=%.1f (qg/quad=%.2f) PT=%s\n",
               P,Pb,Pcr,ncrib,Pqg,Pqg/94.0,out);
        if(Pb>gbest){gbest=Pb;gP=P;gcr=Pcr;gqg=Pqg;strcpy(gsq,Psq);}
    }
    int posOf[26]; rebuild_pos(gsq,posOf); bifid_decrypt(posOf,gsq,gP,out);
    printf("\n=== GLOBAL BEST P=%d obj=%.1f cribs=%d/%d qg=%.1f (qg/quad=%.2f) ===\n",gP,gbest,gcr,ncrib,gqg,gqg/94.0);
    printf(" sq=%s\n PT=%s\n",gsq,out);
    printf("LECTURE: anglais réel ~ qg/quad >= -3.2 ; charabia ~ <= -6. "
           "Vrai hit = cribs=24 ET qg/quad>=-3.5 ET lisible.\n");
    return 0;
}
