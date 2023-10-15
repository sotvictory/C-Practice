/* reverse lines in a file */

#include <stdio.h>
#include <stdlib.h>

void mem_err(void)
{
    fprintf(stderr, "OS did not give memory. Exit.\n");
    exit(1);
}

char *read_line(FILE *fp, int *line_size) 
{
    int buf_size = 128, c;
    char *buf = NULL;

    buf = malloc(buf_size * sizeof(char));
    if (buf == NULL)
        mem_err();

    while ((c = fgetc(fp)) != '\n' && c != EOF) {
        buf[(*line_size)++] = (char)c;
        if ((*line_size) == buf_size) {
            buf_size *= 1.5;
            buf = realloc(buf, buf_size * sizeof(char));
            if (buf == NULL)
                mem_err();
        }
    }

    if ((buf_size - (*line_size)) != 1) {
        if (c == EOF) {
            free(buf);
            return NULL;
        }
        buf = realloc(buf, (*line_size + 1) * sizeof(char));
        if (buf == NULL)
            mem_err();
    }
    buf[(*line_size)++] = '\n';

    return buf;
}

void reverse_line(char *line, int line_size)
{
    int i;
    char tmp;

    for (i = 0; i < line_size / 2; i++) {
        tmp = line[i];
        line[i] = line[line_size - i - 1];
        line[line_size - i - 1] = tmp;
    }
}

void reverse_file(FILE *fp)
{
    long cnt, i;
    char c_1, c_2;

    fseek(fp, -2 * sizeof(char), SEEK_END);
    cnt = ftell(fp) / sizeof(char);

    for (i = 0; i <= cnt / 2; i++) {
        c_1 = fgetc(fp);
        fseek(fp, i * sizeof(char), SEEK_SET);
        c_2 = fgetc(fp);
        fseek(fp, -sizeof(char), SEEK_CUR);
        fputc(c_1, fp);
        fseek(fp, (cnt - i) * sizeof(char), SEEK_SET);
        fputc(c_2, fp);
        fseek(fp, -2 * sizeof(char), SEEK_CUR);
    }
}

int main(int argc, char **argv)
{
    FILE *fp;
    char *line = NULL;
    int line_size = 0, c;
    long file_size = 0; 

    if (argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        exit(2);
    }

    if ((fp = fopen(argv[1], "r+")) == NULL) {
        perror(argv[1]);
        exit(3);
    }

    /* empty file */
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp) / sizeof(char);
    if (file_size == 0)
        return 0;

    /* file without '\n' */
    fseek(fp, -1L, SEEK_END);
    c = fgetc(fp);
    if (c != '\n')
        fputc(c, fp);
        
    rewind(fp);

    /* reverse each line */
    while (1) {
        if ((line = read_line(fp, &line_size)) == NULL)
            break;
        reverse_line(line, line_size - 1);
        fseek(fp, -line_size * sizeof(char), SEEK_CUR);
        fwrite(line, sizeof(char), line_size, fp);
        free(line);
        line_size = 0;
    }

    /* reverse all symbols in file */
    reverse_file(fp);

    fclose(fp);

    return 0;
}