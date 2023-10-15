/* duplicate each line in a file */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *in, *out;
    int c, i, cnt = 0;

    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }

    if ((in = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        exit(2);
    }

    if ((out = fopen(argv[2], "w")) == NULL) {
        perror(argv[2]);
        exit(3);
    }

    while ((c = fgetc(in)) != EOF) {
        if (c != '\n') {
            fputc(c, out);
            cnt++;
        } else {
            /* '\n' */
            fputc(c, out);
            /* copy */
            fseek(in, -(cnt + 1) * sizeof(char), SEEK_CUR);
            for (i = 0; i < cnt + 1; i++) {
                c = fgetc(in);
                fputc(c, out);
            }
            cnt = 0;
        }
    }

    fclose(in);
    fclose(out);

    return 0;
}
