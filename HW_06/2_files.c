/* if string length <= 7 (including '\n'), print its reverse */

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 7

void reverse_str(char *str, int len)
{
    int i;
    char tmp;

    for (i = 0; i < len / 2; i++) {
        tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
}

int main(int argc, char **argv)
{
    FILE *in, *out;
    char buf[MAX_LEN + 1] = {0};
    int c, cnt = 0;

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
        if (c != '\n' && cnt < 7) {
            buf[cnt] = c;
            cnt++;
        } else {
            if (c == '\n' && cnt < 7) {
                /* reverse and write a buf */
                reverse_str(buf, cnt);
                buf[cnt] = '\n';
                fwrite(buf, sizeof(char), cnt + 1, out);
            } else {
                /* write a buf and write the rest of line */
                fwrite(buf, sizeof(char), cnt, out);
                while (c != EOF && c != '\n') {
                    fputc(c, out);
                    c = fgetc(in);
                }
                if (c != EOF) {
                    fputc(c, out); /* '\n' */
                }
            }
            cnt = 0; 
        }
    } 

    fclose(in);
    fclose(out);

    return 0;
}