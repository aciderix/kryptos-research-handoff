#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char**argv){const char*t=argv[1];int L=strlen(t);int W=atoi(argv[2]);int order[64];
 const char*os=argv[3]; if(!strcmp(os,"id"))for(int i=0;i<W;i++)order[i]=i; else if(!strcmp(os,"rev"))for(int i=0;i<W;i++)order[i]=W-1-i;
 else{int i=0;char b[256];strncpy(b,os,255);b[255]=0;for(char*x=strtok(b,",");x&&i<W;x=strtok(NULL,","))order[i++]=atoi(x);}
 int rows=(L+W-1)/W; char g[64][64]; for(int r=0;r<rows;r++)for(int c=0;c<W;c++)g[r][c]=0;
 int p=0; for(int r=0;r<rows;r++)for(int c=0;c<W;c++){if(p<L)g[r][c]=t[p++];}
 for(int k=0;k<W;k++){int col=order[k];for(int r=0;r<rows;r++)if(g[r][col])putchar(g[r][col]);} putchar('\n');return 0;}
