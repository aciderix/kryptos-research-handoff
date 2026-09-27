#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/* quadgram log-prob avec backoff simple: P(d|abc) = (C(abcd)+a)/(C(abc)+a*26).
   QG[abcd] = log(P). ecrit 456976 floats (format ac7). */
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); long L=ftell(f); fseek(f,0,SEEK_SET);
  char*t=malloc(L+1); if(fread(t,1,L,f)!=(size_t)L){return 2;} t[L]=0; fclose(f);
  long *c4=calloc(456976,sizeof(long)); long *c3=calloc(17576,sizeof(long));
  for(long i=0;i+3<L;i++){int a=t[i]-'A',b=t[i+1]-'A',cc=t[i+2]-'A',d=t[i+3]-'A';
    if(a<0||a>25||b<0||b>25||cc<0||cc>25||d<0||d>25)continue;
    c4[((a*26+b)*26+cc)*26+d]++; c3[(a*26+b)*26+cc]++; }
  float*QG=malloc(sizeof(float)*456976); double a=0.5;
  for(int A=0;A<26;A++)for(int B=0;B<26;B++)for(int C=0;C<26;C++){
    long ctx=c3[(A*26+B)*26+C]; double den=ctx+a*26;
    for(int D=0;D<26;D++){ long n=c4[((A*26+B)*26+C)*26+D]; QG[((A*26+B)*26+C)*26+D]=(float)log((n+a)/den);} }
  FILE*o=fopen(argv[2],"wb"); fwrite(QG,sizeof(float),456976,o); fclose(o);
  fprintf(stderr,"qg built from %ld chars\n",L); return 0;
}
