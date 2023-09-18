#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *read_line(void) 
{
    int line_size = 128, cnt = 0, c, i;
    char *buffer, *line;

    buffer = (char *) malloc(line_size * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "OS did not give memory. Exit...\n");
        exit(1);
    }

    while (((c = getchar()) != EOF)) {
        buffer[cnt++] = (char)c;
        if (cnt == line_size) {
            line_size *= 1.5;
            buffer = (char *) realloc(buffer, line_size * sizeof(char));
            if (buffer == NULL) {
                fprintf(stderr, "OS did not give memory. Exit...\n");
                exit(1);
            }
        }
    }

    if ((line_size - cnt) == 1) {
        buffer[cnt] = '\0';
        return buffer;
    } else {
        if (c == EOF) {
            free(buffer);
            return NULL;
        }
        line = malloc ((cnt + 1) * sizeof(char));
        /* or memcpy(line, buffer, cnt * sizeof(char)) */
        for (i = 0; i < cnt; ++i)
            line[i] = buffer[i];
        line[cnt] = '\0';
        free(buffer);
        return line;
    }
}

char **read_stdin(int *cnt)
{
    int lines_cnt = 16, i;
    char *line;
    char **lines, **new_lines;

    lines = (char**) malloc(lines_cnt * sizeof(char*));
    if (lines == NULL) {
        fprintf(stderr, "OS did not give memory. Exit...\n");
        exit(1);
    }

    while (!feof(stdin)) {
        line = read_line();
        if (line == NULL)
            break;
        lines[(*cnt)++] = line;

        if (*cnt == lines_cnt) {
            lines_cnt *= 1.5;
            lines = (char**) realloc(lines, lines_cnt * sizeof(char*));
            if (lines == NULL) {
                fprintf(stderr, "OS did not give memory. Exit...\n");
                exit(1);
            }
        }
    }

    if (*cnt < lines_cnt) {
        new_lines = (char**) malloc((*cnt) * sizeof(char*));
        if (new_lines == NULL) {
            fprintf(stderr, "OS did not give memory. Exit...\n");
            exit(1);
        }
        for (i = 0; i < *cnt; ++i)
            new_lines[i] = lines[i];
        free(lines);
        return new_lines;
    }
    else
        return lines;
}

void sort(char **strings, int first, int last) 
{
    int i = first, j = last;
    char *x = strings[(first + last) / 2];
    char *k;

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

void print_array(char **mas, int size) 
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

int main (void) 
{
    char **mas;
    int size = 0;
    mas = read_stdin(&size);

    printf("Got %d lines:\n", size);
    print_array(mas, size);

    sort(mas, 0, size-1);
    printf("Sorted array:\n");
    print_array(mas, size);

    mas_free(mas, size);

    return 0;
}