#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd;
    long long num, num_2, min, min_pos, n;

    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        exit(2);
    }

    n = read(fd, &min, sizeof(long long));
    min_pos = lseek(fd, 0, SEEK_CUR);

    while ((n = read(fd, &num, sizeof(long long))) > 0) {
        if (num < min) {
            min = num;
            min_pos = lseek(fd, 0, SEEK_CUR);
        }
    }

    lseek(fd, min_pos, SEEK_SET);
    n = read(fd, &num, sizeof(long long));
    while (1) {
        if (n > 0) {
            if ((n = read(fd, &num_2, sizeof(long long))) > 0)
                lseek(fd, -sizeof(long long), SEEK_CUR);
            write(fd, &num, sizeof(long long));
            num = num_2;
        } else {
            break;
        }
    }

    lseek(fd, min_pos, SEEK_SET);
    write(fd, &min, sizeof(long long));

    close(fd);

    return 0;
}