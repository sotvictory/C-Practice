#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define IS_LITTLE 0
#define IS_BIG 1

#define READING_FAILURE 1

int main(int argc, char **argv)
{
    FILE *in = stdin, *out = stdout;
    uint8_t utf8, utf8_1, utf8_2;
    uint16_t utf16;
    int bom = IS_LITTLE, is_bom_set = 0, correct_input = 0, n, i, j;

    /* check I/O redirection */
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if ((in = fopen(argv[i + 1], "r")) == NULL) {
                perror(argv[i + 1]);
                fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-be | -le]\n", argv[0]);
                exit(READING_FAILURE);                
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if ((out = fopen(argv[i + 1], "w")) == NULL) {
                perror(argv[i + 1]);
                fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-be | -le]\n", argv[0]);
                exit(READING_FAILURE);                
            }            
        }
    }

    /* check the byte order */
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-be") == 0) {
            bom = IS_BIG;
            utf16 = 0xFFFE;
            is_bom_set = 1;
            for (j = i; j < argc; j++) {
                if (strcmp(argv[i], "-le") == 0) {
                    fprintf(stderr, "Error: either -be or -le flag must be specified\n");
                    fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-be | -le]\n", argv[0]);
                    exit(READING_FAILURE);
                }
            }
        } else if (strcmp(argv[i], "-le") == 0) {
            bom = IS_LITTLE;
            utf16 = 0xFEFF;
            is_bom_set = 1;
            for (j = i; j < argc; j++) {
                if (strcmp(argv[i], "-be") == 0) {
                    fprintf(stderr, "Error: either -be or -le flag must be specified\n");
                    fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-be | -le]\n", argv[0]);
                    exit(READING_FAILURE);
                }
            }
        }
    }

    if (is_bom_set == 0) {
        bom = IS_LITTLE;
        utf16 = 0xFEFF; 
        fprintf(stderr, "You have not specified either the -be or -le flag\n");
        fprintf(stderr, "By default, the little endian byte order will be applied\n"); 
        fread(&utf8, 1, 1, in);
        fread(&utf8_1, 1, 1, in);
        fread(&utf8_2, 1, 1, in);
        /* no BOM marker */
        if (!((utf8 == 0xEF) && (utf8_1 == 0xBB) && (utf8_2 == 0xBF)))
            fseek(in, 0, SEEK_SET);   
    }

    /* add a BOM marker to new file */
    fwrite(&utf16, 2, 1, out);

    while (1) {
        n = fread(&utf8, 1, 1, in);
        if (n <= 0) {
            break;
        }
        utf16 = 0;
        if (utf8 >> 7 == 0) {                           /* 0xxxxxxx */
            utf16 |= utf8;                              /* 000000000xxxxxxx */
            correct_input = 1;
        } else if (utf8 >> 5 == 0x6) {                  /* 110xxxxx */
            utf16 = (utf16 | (utf8 & 0x1F)) << 6;       /* 00000xxxxx000000 */
            n = fread(&utf8, 1, 1, in);
            if (utf8 >> 6 == 2) {                       /* 10yyyyyy */
                utf16 |= utf8 & 0x3F;                   /* 00000xxxxxyyyyyy */
                correct_input = 1;
            } else {
                fprintf(stderr, "Third byte of the sequence at position %ld is incorrect\n", ftell(in));
            }
        } else if (utf8 >> 4 == 0xE) {                  /* 1110xxxx */
            utf16 = (utf16 | (utf8 & 0xF)) << 6;        /* 000000xxxx000000 */
            n = fread(&utf8, 1, 1, in);
            if (utf8 >> 6 == 2) {                       /* 10yyyyyy */
                utf16 = (utf16 | (utf8 & 0x3F)) << 6;   /* xxxxyyyyyy000000 */
                n = fread(&utf8, 1, 1, in);
                if ((utf8 >> 6) == 2) {                 /* 10zzzzzz */
                    utf16 |= (utf8 & 0x3F);             /* xxxxyyyyyyzzzzzz */
                    correct_input = 1;
                } else {
                    fprintf(stderr, "Third byte of the sequence at position %ld is incorrect\n", ftell(in));
                }
            } else {
                fprintf(stderr, "Second byte of the sequence at position %ld is incorrect\n", ftell(in));
            }
        } else {
            fprintf(stderr, "First byte of the sequence at position %ld is incorrect\n", ftell(in));
        }
        if (correct_input == 1) {
            if (bom == IS_BIG) {
                utf8 = utf16 >> 8;
                fwrite(&utf8, sizeof(utf8), 1, out);
                utf8 = utf16;
                fwrite(&utf8, sizeof(utf8), 1, out);
            } else {
                fwrite(&utf16, 2, 1, out);
            }
            correct_input = 0;
        }
    }

    fclose(in);
    fclose(out);

    return 0;
}