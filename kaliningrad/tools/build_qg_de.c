/* build_qg_de.c — German quadrigram model for K16.\n * Independent Gutenberg corpus only; never train on the Kaliningrad ciphertext.
 * Output: 456976 float32 log10 probabilities, A-Z only.
 * Usage: build_qg_de output.bin corpus1.txt corpus2.txt ...
 * Gutenberg headers/footers are ignored.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int main(int argc, char **argv) {
    if (argc < 3) return 2;
    static double cnt[456976];
    double total = 0;
    for (int f = 2; f < argc; f++) {
        FILE *fp = fopen(argv[f], "rb");
        if (!fp) { perror(argv[f]); return 3; }
        fseek(fp, 0, SEEK_END); long n = ftell(fp); rewind(fp);
        char *b = malloc((size_t)n + 1);
        if (!b) return 4;
        if (fread(b,1,(size_t)n,fp)!=(size_t)n) return 5;
        fclose(fp); b[n]=0;
        char *s = strstr(b, "*** START");
        if (s) { s = strchr(s, '\n'); if (!s) s=b; else s++; } else s=b;
        char *e = strstr(s, "*** END"); if (e) *e=0;
        int h[4]={0,0,0,0}, k=0;
        for (char *p=s; *p; ++p) {
            int c=toupper((unsigned char)*p);
            if (c<'A'||c>'Z') continue;
            h[0]=h[1]; h[1]=h[2]; h[2]=h[3]; h[3]=c-'A';
            if (++k>=4) { cnt[((h[0]*26+h[1])*26+h[2])*26+h[3]]++; total++; }
        }
        free(b);
    }
    if (total <= 0) return 6;
    static float out[456976];
    double floorv=log10(0.01/total);
    for (int i=0;i<456976;i++) out[i]=(float)(cnt[i]>0?log10(cnt[i]/total):floorv);
    FILE *o=fopen(argv[1],"wb"); if(!o) return 7;
    fwrite(out,sizeof(out),1,o); fclose(o);
    fprintf(stderr,"German quadrigrams: %.0f; floor %.3f\n",total,floorv);
    return 0;
}
