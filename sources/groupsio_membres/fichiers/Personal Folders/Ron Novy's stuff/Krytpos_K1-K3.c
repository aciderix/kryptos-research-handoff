/* Kryptos Decoder
 *  Written by Ron S. Novy
 *  Copyright (C) 2008
 *
 *  This software was created to decrypt 'ALL' 4 sections of Kryptos...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* K1 and K2 cypher text */
char k1_ct[] = "EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJ"
               "YQTQUXQBQVYUVLLTREVJYQTMKYRDMFD";
char k2_ct[] = "VFPJUDEEHZWETZYVGWHKKQETGFQJNCE"
               "GGWHKK?DQMCPFQZDQMMIAGPFXHQRLG"
               "TIMVMZJANQLVKQEDAGDVFRPJUNGEUNA"
               "QZGZLECGYUXUEENJTBJLBQCRTBJDFHRR"
               "YIZETKZEMVDUFKSJHKFWHKUWQLSZFTI"
               "HHDDDUVH?DWKBFUFPWNTDFIYCUQZERE"
               "EVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDX"
               "FLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKF"
               "FHQNTGPUAECNUVPDJMQCLQUMUNEDFQ"
               "ELZZVRRGKFFVOEEXBDMVPNFQXEZLGRE"
               "DNQFMPNZGLFLPMRJQYALMGNUVPDXVKP"
               "DQUMEBEDMHDAFMJGZNUPLGESWJLLAETG"; /* <- Added 'S' for translation of 'XLAYERTWO' */

/* K3 cypher text */
char k3_ct[] = "ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIA"
               "CHTNREYULDSLLSLLNOHSNOSMRWXMNE"
               "TPRNGATIHNRARPESLNNELEBLPIIACAE"
               "WMTWNDITEENRAHCTENEUDRETNHAEOE"
               "TFOLSEDTIWENHAEIOYTEYQHEENCTAYCR"
               "EIFTBRSPAMHHEWENATAMATEGYEERLB"
               "TEEFOASFIOTUETUAEOTOARMAEERTNRTI"
               "BSEDDNIAAHTTMSTEWPIEROAGRIEWFEB"
               "AECTDDHILCEIHSITEGOEAOSDDRYDLORIT"
               "RKLMLEHAGTDHARDPNEOHMGFMFEUHE"
               "ECDMRIPFEIMEHNLSSTTRTVDOHW";


/* Some stuff for the polyalphabetic cyphers */
int pa_key_counter = 0;
int pa_key_counter_max = 0;
char pa_alpha[27] = "KRYPTOSABCDEFGHIJLMNQUVWXZ\0"; /* Static keyword of KRYPTOS */
#define _KEYWORD_MAX_ 16
char pa_poly_key[_KEYWORD_MAX_][27]; /* Max keyword length of 16 characters and
                                      * a 26 letter alphabet + 1 NULL character.
                                      */


/* pa_find_char:
 *  Function to find a character in an alphabet and return the index value
 * of that character.
 */
int pa_find_char(char c, char *str)
{
    int i;

    for (i = 0; i < 26; i++)
       if (str[i] == c)
          break;

    return i;
}



/* pa_set:
 *  Sets (or resets) the polyalphabetic cypher code using KEYWORD as the
 * second keyword (KRYPTOS is always the first).
 */
void pa_set(char *keyword)
{
    int x, y, z;

    /* Clear global variables */
    memset(pa_poly_key, 0, sizeof(pa_poly_key));
    pa_key_counter = 0;
    pa_key_counter_max = strlen(keyword);
    if (pa_key_counter_max > _KEYWORD_MAX_)
        pa_key_counter_max = _KEYWORD_MAX_;

    /* Check keyword for errors */
    for (x = 0; x < pa_key_counter_max; x++) {
        /* Check for lower case characters and change them to upper case */
        if ((keyword[x] >= 'a') && (keyword[x] <= 'z'))
            keyword[x] = (z - 'a') + 'A';

        /* Make sure the character is now upper case between 'A' and 'Z' */
        if ((keyword[x] < 'A') || (keyword[x] > 'Z')) {
            printf("Error, line:%d: Keyword must be a letter between A and Z.\n\n", __LINE__);
            exit(0);
        }
    }

    /* Setup polyalphabetic tables */
    printf("\nSetting Polyalphabetic cypher keywords to KRYPTOS, %s\n%s\n", keyword, pa_alpha);
    for (y = 0; y < pa_key_counter_max; y++) {

        /* Check for NULL terminator */
        if ((z = pa_find_char(keyword[y], pa_alpha)) > 27)
            break;

        for (x = 0; x < 26; x++) {
            pa_poly_key[y][x] = pa_alpha[(x + z) % 26];
        }
        printf("%s\n", pa_poly_key[y]);
    }
}



/* pa_translate_char:
 *  Translates a single character.
 */
char pa_translate_char(char c)
{
    int r;

    /* Translate question marks to question marks */
    if (c == '?')
        return '?';

    /* Get the index of the character to lookup */
    r = pa_find_char(c, pa_poly_key[pa_key_counter]) % 26;

    /* Increment the key counter while keeping within bounds */
    pa_key_counter = (pa_key_counter + 1) % pa_key_counter_max;

    /* Return the proper character */
    return pa_alpha[r];
}



/* pa_translate:
 *  Translate an entire string from SRC to DST while translating only
 * up to MAX characters.
 */
void pa_translate(char *src, char *dst, int max)
{
    for ( ; (*src != 0) || (max <= 0); src++, dst++, max--)
        *dst = pa_translate_char(*src);

    *dst = '\0';
}



/* transpose by matrix:
 *  Do a matrix style transposition on SRC and place the output in DST.
 * DST must have enough space to hold the final matrix.
 */
void transpose_by_matrix(char *src, int src_cols, int src_rows, char *dst, int dst_cols, int dst_rows)
{
    int src_len = strlen(src);
    int i, x, y;

    /* Error checking */
    if (((src_cols * src_rows) != src_len) ||
        ((dst_cols * dst_rows) != src_len))
        printf("\n\nError: Source and destination matrix must have the same number of chars as the original string.\n");

    printf("\nTranspose from %d x %d matrix to %d x %d matrix\n", src_cols, src_rows, dst_cols, dst_rows);

    /* Do actual transposition */
#if 1 /* 0 or 1 will work... They should both have the same results. */
    for (i = 0; i < src_len; i++) {
        dst[(((dst_rows - 1) - (i % dst_rows)) * dst_cols) + (i / dst_rows)] =
           src[((i % src_rows) * src_cols) + ((src_cols - 1) - (i / src_rows))];
    }
#else
    for (i = 0; i < src_len; i++) {
        dst[((i % dst_rows) * dst_cols) + ((dst_cols - 1) - (i / dst_rows))] =      \
            src[(((src_rows - 1) - (i % src_rows)) * src_cols) + (i / src_rows)];
    }
#endif

    /* Show source matrix */
    for (y = 0; y < src_rows; y++) {
        for (x = 0; x < src_cols; x++) {
            putchar(src[(y * src_cols) + x]);
        }
        putchar('\n');
    }
    putchar('\n');

    /* Show destination matrix */
    for (y = 0; y < dst_rows; y++) {
        for (x = 0; x < dst_cols; x++) {
            putchar(dst[(y * dst_cols) + x]);
        }
        putchar('\n');
    }
    putchar('\n');
}



/* show_analysis:
 *  Displays the number of times each character in the alphabet occors in STR.
 */
void show_analysis(char *str)
{
    int i, j, t, len = strlen(str);
    int count[26];
    char letter[26], l;

    /* Clear the entire count variable to 0 */
    memset(count, 0, sizeof(count));

    /* Set letter to the corresponding letter to start out */
    for (i = 0; i < 26; i++)
        letter[i] = i + 'A';

    /* Scan each character */
    for (i = 0; i < len; i++, str++) {
        /* Make sure the character is upper case */
        if ((*str >= 'a') && (*str <= 'z'))
            *str = (*str - 'a') + 'A';

        /* If the character is not a letter between 'A' and 'Z' then skip it */
        if ((*str < 'A') || (*str > 'Z'))
            continue;

        count[(*str - 'A') % 26]++;
    }

    /* Sort the letters so the ones with the higher count are displayed first. */
    for (i = 0; i < 25; i++) {
        for (j = i + 1; j < 26; j++) {
            if (count[j] > count[i]) {
                t = count[j];
                count[j] = count[i];
                count[i] = t;
                l = letter[j];
                letter[j] = letter[i];
                letter[i] = l;
            }
        }
    }

    /* Now display the count of each letter */
    for (i = 0; i < 26; i++) {
        printf("%c:%d ", letter[i], count[i]);
    }
    putchar('\n');
}



/* wait_for_key:
 *  Displays a message and waits for the user to press a key
 */
void wait_for_key(char *msg)
{
    putchar('\n');
    puts(msg);
#if 0 /* 0 = Don't pause for key, 1 = pause for key */
    puts("Press any key to continue...\n");
    getch();
#endif
}



/* k1_k2_decrypt:
 *  Decrypt section 1.  Keywords: Kryptos, Palimpsest
 */
void k1_decrypt(void)
{
    char k1_pt[512];

    printf("\n--==={ Decyphering K1. Length:%d characters }===--\n%s\n", strlen(k1_ct), k1_ct);
    pa_set("PALIMPSEST");
    pa_translate(k1_ct, k1_pt, sizeof(k1_pt));
    printf("\n%s\n\n", k1_pt);
    show_analysis(k1_pt);
    wait_for_key("K2 Done.\n");
}



/* k1_k2_decrypt:
 *  Decrypt section 2.  Keywords: Kryptos, Abscissa
 */
void k2_decrypt(void)
{
    char k2_pt[512];

    printf("\n--==={ Decyphering K2. Length:%d characters }===--\n%s\n", strlen(k2_ct), k2_ct);
    pa_set("ABSCISSA");
    pa_translate(k2_ct, k2_pt, sizeof(k2_pt));
    printf("\n%s\n\n", k2_pt);
    show_analysis(k2_pt);
    wait_for_key("K2 Done.\n");
}



/* The magic K3 code */
void k3_decrypt(void)
{
    char k3_pt[512];

    printf("\n--==={ Decyphering K3. Length:%d characters }===--\n%s\n", strlen(k3_ct), k3_ct);
    transpose_by_matrix(k3_ct, 24, 14, k3_pt, 42, 8);
    show_analysis(k3_pt);
    wait_for_key("K3 Done.\n");
}



/* Main */
int main(int argc, char *argv[])
{
    printf(
        "\n"
        "Krytos decoder software\n"
        "Written by Ron S. Novy\n"
        "Copyright (C) 2008\n");

    k1_decrypt();
    k2_decrypt();
    k3_decrypt();
    //TODO: k4_decrypt();
    return 0;
}


/*
** -= EOF =-
*/
