#include <stdio.h>
#include <string.h>
/* ~140 mots anglais courants 3-6 lettres (dont ceux repérés par l'utilisateur) */
static const char*W[]={"THE","AND","YOU","WILL","VERY","PAST","GOT","HUMP","EAST","NORTH","WEST","SOUTH",
"MY","HIS","HER","HIM","SHE","WHO","WAS","ARE","FOR","NOT","BUT","ALL","ANY","CAN","HAD","HAS","HAVE",
"THAT","THIS","WITH","FROM","THEY","THEM","WERE","BEEN","MORE","SOME","TIME","THAN","THEN","INTO","ONLY",
"OVER","SUCH","MOST","VERY","WHEN","WHAT","YOUR","WOULD","COULD","THERE","WHICH","THEIR","ABOUT","OTHER",
"LOVE","LIFE","HAND","HOME","GOOD","GREAT","THING","THINK","KNOW","LIKE","MADE","MAKE","MUST","BOAT","LAND",
"COLD","HOLD","STOP","STEP","ROPE","SAFE","FREE","HOME","DUCK","RUN","FLY","HURT","HEAR","SEE","SEEN","LOOK",
"LOST","DARK","LONG","SIDE","DOWN","BACK","AWAY","AGES","OURS","MINE","LADY","BODY","EVER","EVEN","NEXT",
"SENT","SERVE","GONE","DONE","CAME","COME","GOES","WENT","TAKE","GIVE","FIND","KEEP","TURN","MOVE","PART",
"BELL","WELL","TELL","FELL","SELL","CALL","FALL","HALL","WALL","MEMORY","SHADOW","LUCID","CLOCK","BERLIN","VEIL",
"LIE","LIT","LIZ","IZY","SVE","OFANY","AGO","AIR","ICE","OIL","TON","SON","TOP","POT","RAT","CAT","CAR","BAR"};
static int NW=sizeof(W)/sizeof(W[0]);
int main(int argc,char**argv){ const char*s=argv[1]; int L=strlen(s); int hits=0,cov=0; char covered[256]={0};
 for(int i=0;i<L;i++)for(int w=0;w<NW;w++){int lw=strlen(W[w]); if(i+lw<=L && strncmp(s+i,W[w],lw)==0){hits++; for(int j=0;j<lw;j++)covered[i+j]=1;}}
 for(int i=0;i<L;i++)cov+=covered[i];
 printf("hits=%d cov=%d/%d (%.0f%%)\n",hits,cov,L,100.0*cov/L); return 0;}
