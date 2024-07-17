#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mem_err(void)
{
    fprintf(stderr, "OS did not give memory. Exit...\n");
    exit(1);
}

char *read_line(void) 
{
    int line_size = 128, cnt = 0, c;
    char *buffer;

    buffer = (char *) malloc(line_size * sizeof(char));
    if (buffer == NULL)
        mem_err();

    while (((c = getchar()) != EOF && c != '\n')) {
        buffer[cnt++] = (char)c;
        if (cnt == line_size) {
            line_size *= 1.5;
            buffer = (char *) realloc(buffer, line_size * sizeof(char));
            if (buffer == NULL)
                mem_err();
        }
    }

    if ((line_size - cnt) != 1) {
        if (c == EOF) {
            free(buffer);
            return NULL;
        }
        buffer = (char *) realloc(buffer, (cnt + 1) * sizeof(char));
        if (buffer == NULL)
            mem_err();
    }
    buffer[cnt] = '\0';

    return buffer;

}

char **read_stdin(int *cnt)
{
    int lines_cnt = 16;
    char *line;
    char **lines;

    lines = (char **) malloc(lines_cnt * sizeof(char *));
    if (lines == NULL)
        mem_err();

    while (!feof(stdin)) {
        line = read_line();
        if (line == NULL)
            break;
        lines[(*cnt)++] = line;

        if (*cnt == lines_cnt) {
            lines_cnt *= 1.5;
            lines = (char **) realloc(lines, lines_cnt * sizeof(char *));
            if (lines == NULL)
                mem_err();
        }
    }

    if (*cnt < lines_cnt) {
        lines = (char **) realloc(lines, (*cnt) * sizeof(char *));
        if (lines == NULL)
            mem_err();
    }

    return lines;
}

void sort(char **strings, int first, int last) 
{
    int i = first, j = last;
    char *x = strings[(first + last) / 2], *k;

    do
    {
        while (strcmp(x, strings[i]) > 0)
            i++;
        while (strcmp(strings[j], x) > 0)
            j--;
        if (i <= j) {
            if (i < j) {
                k = strings[i];
                strings[i] = strings[j];
                strings[j] = k;
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        sort(strings, i, last);
    if (first < j)
        sort(strings, first, j);
}

void mas_print(char **mas, int size) 
{
    int i;

    for (i = 0; i < size; ++i)
        printf("%d: %s\n", i + 1, mas[i]);
}

void mas_free(char **mas, int size)
{
    int i;

    for (i = 0; i < size; ++i)
        free(mas[i]);
    free(mas);
}

int main(void) 
{
    char **mas;
    int size = 0;
    mas = read_stdin(&size);

    printf("Got %d lines:\n", size);
    mas_print(mas, size);

    sort(mas, 0, size-1);
    printf("Sorted array:\n");
    mas_print(mas, size);

    mas_free(mas, size);

    return 0;
}