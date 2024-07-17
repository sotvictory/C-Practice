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
    uint8_t utf8_1, utf8_2, utf8_3;
    uint16_t utf16;
    int bom = IS_LITTLE, is_bom_set = 0, n, i, j;

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
    fread(&utf8_1, 1, 1, in);
    fread(&utf8_2, 1, 1, in);
    if ((utf8_1 == 0xFF) && (utf8_2 == 0xFE)) {
        bom = IS_LITTLE;
    } else if ((utf8_1 == 0xFE) && (utf8_2 == 0xFF)) {
        bom = IS_BIG;
    /* the file does not comply with the agreement */
    } else if (utf8_1 == 0xFE) {
        bom = IS_LITTLE;
        is_bom_set = 1;
        fseek(in, 1, SEEK_SET);
        fprintf(stderr, "The file has an incorrect odd number of bytes, which is not compatible with the UTF-16 format\n");
    } else if (utf8_1 == 0xFF) {
        bom = IS_BIG;
        is_bom_set = 1;
        fseek(in, 1, SEEK_SET);
        fprintf(stderr, "The file has an incorrect odd number of bytes, which is not compatible with the UTF-16 format\n");
    } else {
        fprintf(stderr, "The file does not comply with the agreement: it does not contain a BOM marker at the beginning of the file\n");
        fseek(in, 0, SEEK_SET);
        for (i = 0; i < argc; i++) {
            if (strcmp(argv[i], "-be") == 0) {
                bom = IS_BIG;
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
            fprintf(stderr, "You also have not specified either the -be or -le flag\n");
            fprintf(stderr, "By default, the little endian byte order will be applied\n");
        }
    }

    /* add a BOM marker to new file */
    utf8_1 = 0xEF;
    utf8_2 = 0xBB;
    utf8_3 = 0xBF;
    fwrite(&utf8_1, 1, 1, out);
    fwrite(&utf8_2, 1, 1, out);
    fwrite(&utf8_3, 1, 1, out);

    while (1) {
        n = fread(&utf16, 1, 2, in);
        if (n == 2) {
            if (bom == IS_BIG) {
                utf16 = (utf16 << 8) | (utf16 >> 8);
            }
            if (utf16 < 128) {                            /* 000000000xxxxxxx */
                utf8_1 = utf16;                           /* 0xxxxxxx */
                fwrite(&utf8_1, 1, 1, out);
            } else if (utf16 < 2048) {                    /* 00000xxxxxyyyyyy */
                utf8_1 = 0xC0 | (utf16 >> 6);             /* 110xxxxx */
                utf8_2 = 0x80 | (utf16 & 0x3F);           /* 10yyyyyy */
                fwrite(&utf8_1, 1, 1, out);
                fwrite(&utf8_2, 1, 1, out);
            } else {                                      /* xxxxyyyyyyzzzzzz */
                utf8_1 = 0xE0 | (utf16 >> 12);            /* 1110xxxx */
                utf8_2 = 0x80 | ((utf16 >> 6) & 0x3F);    /* 10yyyyyy */
                utf8_3 = 0x80 | (utf16 & 0x3F);           /* 10zzzzzz */
                fwrite(&utf8_1, 1, 1, out);
                fwrite(&utf8_2, 1, 1, out);
                fwrite(&utf8_3, 1, 1, out);
            }
        } else if (n == 1) {
            fprintf(stderr, "The file has an incorrect odd number of bytes, which is not compatible with the UTF-16 format\n");
            fprintf(stderr, "The file cannot be decoded properly. The file needs to be fixed before converter processing\n");
        } else {
            break;
        }
    }

    fclose(in);
    fclose(out);

    return 0;
}