#include <stdio.h>
#include <stdlib.h>

#define CNT 6

void write_random(char *name, unsigned long int len)
{
    FILE *fp;
    unsigned long int i;
    int num;

    if (!(fp = fopen(name, "wb"))) {
        fprintf(stderr, "Cannot open file %s\n", name);
        exit(1);
    }

    for (i = 0; i < len; i++) {
        num = rand() % 201 - 100;
        fwrite(&num, sizeof(int), 1, fp);
    }

    fclose(fp);
}

void exchange_raw(FILE *fp, unsigned long int p1, unsigned long int p2)
{
    int tmp, num_1, num_2;

    fseek(fp, p1 * sizeof(int), SEEK_SET);
    fread(&num_1, sizeof(int), 1, fp);
    tmp = num_1;
    fseek(fp, p2 * sizeof(int), SEEK_SET);
    fread(&num_2, sizeof(int), 1, fp);
    fseek(fp, p1 * sizeof(int), SEEK_SET);
    fwrite(&num_2, sizeof(int), 1, fp);
    fseek(fp, p2 * sizeof(int), SEEK_SET);
    fwrite(&tmp, sizeof(int), 1, fp);
}

void invert_raw(FILE *fp)
{
    unsigned long int len = 0, cnt = 0, i = 0;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    cnt = len / sizeof(int);  

    for (i = 0; i < cnt / 2; i++)
        exchange_raw(fp, i, cnt - i - 1);
}

int main(int argc, char **argv)
{
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }

    write_random(argv[1], CNT);
    getchar(); // pause

    if (!(fp = fopen(argv[1], "rb+"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(1);
    }

    invert_raw(fp);
    fclose(fp);

    return 0;
}
