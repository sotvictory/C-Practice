#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_BUF_SIZE 128

enum { INP_ERR, OPEN_ERR, MEM_ERR };

void decode_str(char *encoded_str, int code_word_len)
{
    int i = strlen(encoded_str) - 1;

    while (i >= code_word_len) {
        encoded_str[i] = encoded_str[i] ^ encoded_str[i - code_word_len];
        i--;
    }

    return;
}

int main(int argc, char **argv)
{
    FILE *in = NULL, *out = NULL;
    int buf_size = INIT_BUF_SIZE, len = 0, i = 0, c = 0;
    char *buf = NULL, *encoded_str = NULL;
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s <input_file> <output_file>\n", argv[0]);
        exit(INP_ERR);
    }

    if ((in = fopen(argv[1], "r")) == NULL) {
        perror("open");
        exit(OPEN_ERR);
    }

    if ((out = fopen(argv[2], "w")) == NULL) {
        perror("open");
        exit(OPEN_ERR);
    }

    if ((buf = (char *)malloc(buf_size * sizeof(char))) == NULL) {
        perror("malloc");
        exit(MEM_ERR);
    }

    while (fread(&c, 1, 1, in) == 1) {
        if (len == buf_size - 1) {
            buf_size *= 2;
            if ((buf = (char *)realloc(buf, buf_size * sizeof(char))) == NULL) {
                perror("realloc");
                exit(MEM_ERR);
            }
        }
        buf[len++] = c;
    }
    
    if ((buf = (char *)realloc(buf, (len + 1) * sizeof(char))) == NULL) {
        perror("realloc");
        exit(MEM_ERR);
    }
    buf[len] = '\0';

    for (i = 1; i <= len / 2; i++) {
        if (len % i == 0) {
            if ((encoded_str = strdup(buf)) == NULL) {
                perror("strdup");
                exit(MEM_ERR);
            }
            decode_str(encoded_str, i);
            fprintf(out, "%s\n", encoded_str);
            free(encoded_str);
        }
    }

    fprintf(stdout, "decoded file: %s\n", argv[2]);

    free(buf);
    fclose(in);
    fclose(out);

    return 0;
}