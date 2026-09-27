#include <stdio.h>
#include <string.h>
/* alphabet keye : lettres uniques du texte (ordre 1re occurrence) puis reste A-Z */
static void keyed(const char*name,const char*t){
    int seen[26]={0}; char out[27]; int m=0;
    for(const char*p=t;*p;p++){ char c=*p; if(c>='a'&&c<='z')c-=32; if(c<'A'||c>'Z')continue;
        if(!seen[c-'A']){seen[c-'A']=1;out[m++]=c;} }
    for(int k=0;k<26;k++) if(!seen[k]) out[m++]='A'+k;
    out[26]=0; printf("%-12s %s\n",name,out);
    /* reverse */
    char r[27]; for(int i=0;i<26;i++)r[i]=out[25-i]; r[26]=0; printf("%-12s %s\n",name,r);
}
int main(void){
    keyed("K0keyed","SOSRQLUCIDMEMORYSHADOWFORCESWHATISYOURPOSITIONDIGETALINTERPRETATIONVIRTUALLYINVISIBLE");
    keyed("K1keyed","BETWEENSUBTLESHADINGANDTHEABSENCEOFLIGHTLIESTHENUANCEOFIQLUSION");
    return 0;
}
