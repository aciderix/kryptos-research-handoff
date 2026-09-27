#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static float*QG;
static double sc(const char*s){int n=strlen(s);double t=0;int q=0;for(int i=0;i+3<n;i++){int a=s[i]-'A',b=s[i+1]-'A',c=s[i+2]-'A',d=s[i+3]-'A';if(a<0||a>25||b<0||b>25||c<0||c>25||d<0||d>25){t-=8;q++;continue;}t+=QG[((a*26+b)*26+c)*26+d];q++;}return t/q;}
int main(int argc,char**argv){FILE*f=fopen(argv[1],"rb");QG=malloc(4*456976);fread(QG,4,456976,f);fclose(f);
const char*eng="THEQUICKBROWNFOXJUMPSOVERTHELAZYDOGANDTHENRETURNSHOMEFORDINNERWITHHISFAMILY";
const char*k1="BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION";
const char*k2plain="ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLETHEYUSEDTHEEARTHSMAGNETICFIELD";
const char*char1="QXZKWJVBFYPMGHTQXZKWJVBFYPMGHTQXZKWJVBFYPMGHTQXZKWJVBFYPMGHT";
const char*k4="OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
printf("english  = %.3f\n",sc(eng));
printf("K1 plain = %.3f\n",sc(k1));
printf("K2 plain = %.3f\n",sc(k2plain));
printf("charabia = %.3f\n",sc(char1));
printf("K4 cipher= %.3f\n",sc(k4));
return 0;}
