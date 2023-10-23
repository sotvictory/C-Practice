#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CNT 16

void write_random(char *name, unsigned long int len)
{
    FILE *fp;
    unsigned long int i;
    long long num;

    if (!(fp = fopen(name, "wb"))) {
        fprintf(stderr, "Cannot open file %s\n", name);
        exit(1);
    }

    srand(time(NULL));

    for (i = 0; i < len; i++) {
        num = rand() % 201 - 100;
        fwrite(&num, sizeof(long long), 1, fp);
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }

    write_random(argv[1], CNT);

    return 0;
}