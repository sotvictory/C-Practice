#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *f;
    int i, pred, cur, counter = 0, c = argv[1];

    for (i = 2; i < argc; i++) {
        f = fopen(argv[i], "r");
        pred = fgetc(f);
        if (pred != EOF)
            while ((cur = fgetc(f)) != EOF) {
                if (pred == c && cur == '\n')
                    ++counter;
                pred = cur;
            }
        if (pred == c && cur == EOF)
            ++counter;
        fclose(f);
    }
    fprintf(stdout, "%d\n", counter);

    return 0;
}