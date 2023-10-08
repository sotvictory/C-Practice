#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 5

int main(int argc, char **argv)
{
    FILE *in, *out;
    char buf[MAX_LENGTH + 1];
    char c;
    int cnt = 0;

    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }

    if (!(in = fopen(argv[1], "r"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(1);
    }

    if (!(out = fopen(argv[2], "w"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[2]);
        fclose(in);
        exit(1);
    }

    while ((c = fgetc(in)) != EOF) {
        if (c != '\n' && cnt < MAX_LENGTH) {
            fputc(c, out);
            cnt++;
        } else if (c == '\n') {
            fputc('\n', out);
            cnt = 0;
        } else {
            fputc('\n', out);
            cnt = 0;
            while (c != EOF && c != '\n') {
                c = fgetc(in);
            }
        }
    }

    fclose(in);
    fclose(out);

    return 0;
}