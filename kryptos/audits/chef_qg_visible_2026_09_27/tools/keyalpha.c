#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc,char**argv){ int seen[26]={0}; char out[27]; int n=0;
 for(int a=1;a<argc;a++) for(char*p=argv[a];*p;p++){int c=toupper((unsigned char)*p); if(c>='A'&&c<='Z'&&!seen[c-'A']){seen[c-'A']=1;out[n++]=c;}}
 for(int c='A';c<='Z';c++) if(!seen[c-'A']) out[n++]=c; out[26]=0; printf("%s\n",out); return 0; }
