#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void mem_err(void)
{
    fprintf(stderr, "OS did not give memory. Exit.\n");
    exit(3);
}

char *read_line(int fd)
{
    char *line = NULL;
    int line_size = 3, i = 0, r;
    char c;

    line = (char *) malloc(line_size * sizeof(char));
    if (line == NULL)
        mem_err();

    if ((r = read(fd, &c, 1) > 0) == 0)
        return NULL;

    while (1) {
        if (line_size == i) {
            line_size *= 2;
            line = (char *) realloc(line, line_size * sizeof(char));
        }
        line[i++] = c;
        if (c == '\n' || r == 0)
            break;
        r = read(fd, &c, 1);
    }

    if (line_size - 1 != i)
        line = (char *) realloc(line, (i + 1) * sizeof(char));

    line[i] = '\0';

    return line;
}

int main(int argc, char **argv)
{
    int in_1, in_2, out;
    char *line_1 = NULL, *line_2 = NULL;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input_file_1> <input_file_2> <output_file>\n", argv[0]);
        exit(1);
    }

    if ((in_1 = open(argv[1], O_RDONLY)) == -1 || (in_2 = open(argv[2], O_RDONLY)) == -1) {
        perror("open");
        exit(2);
    }

    if ((out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("open");
        exit(2);
    }

    line_1 = read_line(in_1);
    line_2 = read_line(in_2);

    while (1) {
        if (line_1 == NULL && line_2 == NULL) {
            break;
        } else if (line_1 == NULL && line_2 != NULL) {
            write(out, line_2, strlen(line_2));
            line_2 = read_line(in_2);
        } else if (line_1 != NULL && line_2 == NULL) {
            write(out, line_1, strlen(line_1));
            line_1 = read_line(in_1);
        } else {
            if (strcmp(line_1, line_2) < 0) {
                write(out, line_1, strlen(line_1));
                line_1 = read_line(in_1);
            } else {
                write(out, line_2, strlen(line_2));
                line_2 = read_line(in_2);
            }
        }
    }

    close(in_1);
    close(in_2);
    close(out);

    return 0;
}