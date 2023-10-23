#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LENGTH 20

int main(int argc, char **argv)
{
    int in, out;
    char buf[MAX_LENGTH + 1];
    char c;
    int cnt = 0;

    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }

    if ((in = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        exit(2);
    }

    if ((out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        perror("open");
        exit(2);
    }

    while (read(in, &c, 1) > 0) {
        if (c == '\n') {
            if (cnt <= MAX_LENGTH) {
                lseek(in, -cnt - 1, SEEK_CUR);
                read(in, buf, cnt + 1);
                write(out, buf, cnt + 1);
            }
            cnt = 0;
        } else {
            cnt++;
        }
    }

    close(in);
    close(out);

    return 0;
}