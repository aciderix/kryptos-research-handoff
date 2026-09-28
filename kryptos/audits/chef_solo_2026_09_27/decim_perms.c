/* decim_perms.c
 * BATCH 3 (fallback « non-1:1 ») : permutations-DECIMATION de position a la maniere de K3.
 * K3 = transposition = decimation (x192 mod 337, C. Feige/base). Sanborn : « flip the
 * chart, read a different direction ». Sanborn RECULE sur le 1:1 (E. Dunin 2023) => une
 * etape de transposition avant l'autocle structure est une porte ouverte (base 09 §6),
 * et S4 n'a teste QUE la transposition colonnaire 7/14/21, PAS la decimation.
 *
 * 97 est premier => tout m in [1,96] est coprime : perm[i] = (m*i + b) mod 97.
 * On emet, pour chaque m (et offset b=0), la permutation de-transposante a passer a
 * crossbase (de-transpose K4 -> sweep2 autocle structure), + le sens inverse.
 * m=1,b=0 = identite (= pas de transposition, deja teste). A exclure du verdict.
 *
 * Compile: cc -O2 -o decim_perms decim_perms.c ; Usage: ./decim_perms > perms97.txt
 * Format ligne: "m<b> : i0 i1 ... i96"  (perm[k] = position source pour la case k)
 */
#include <stdio.h>
static int egcd_inv(int a,int m){for(int x=1;x<m;x++)if((long)a*x%m==1)return x;return -1;}
int main(void){
    const int P=97;
    printf("# decimation perms mod 97 : case k <- source (m*k+b) mod 97\n");
    for(int m=1;m<P;m++){
        /* perm directe : new[k] = old[(m*k) mod 97] */
        printf("D_m%d_b0 :",m);
        for(int k=0;k<P;k++) printf(" %d",(m*k)%P);
        printf("\n");
    }
    /* sens inverse (utile si l'ecriture se fait par decimation et la lecture lineaire) */
    for(int m=1;m<P;m++){
        int inv=egcd_inv(m,P);
        printf("Dinv_m%d :",m);
        for(int k=0;k<P;k++) printf(" %d",(inv*k)%P);
        printf("\n");
    }
    return 0;
}
