#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void swap_raw(int fd, unsigned long int p_1, unsigned long int p_2)
{
    int tmp, num_1, num_2;

    lseek(fd, p_1 * sizeof(int), SEEK_SET);
    read(fd, &num_1, sizeof(int));
    tmp = num_1;
    lseek(fd, p_2 * sizeof(int), SEEK_SET);
    read(fd, &num_2, sizeof(int));
    lseek(fd, p_1 * sizeof(int), SEEK_SET);
    write(fd, &num_2, sizeof(int));
    lseek(fd, p_2 * sizeof(int), SEEK_SET);
    write(fd, &tmp, sizeof(int));
}

void bubble_sort_raw(int fd)
{
    unsigned long int len = 0, cnt = 0, i, j;
    int num_1, num_2;

    lseek(fd, 0, SEEK_END);
    len = lseek(fd, 0, SEEK_CUR);
    lseek(fd, 0, SEEK_SET);
    cnt = len / sizeof(int);  

    for (i = 0; i < cnt - 1; i++) {
        for (j = i + 1; j < cnt; j++) {
            lseek(fd, i * sizeof(int), SEEK_SET);
            read(fd, &num_1, sizeof(int));
            lseek(fd, j * sizeof(int), SEEK_SET);
            read(fd, &num_2, sizeof(int));
            if (num_1 > num_2)
                swap_raw(fd, i, j);
        }
    }
}

int main(int argc, char **argv)
{
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }

    bubble_sort_raw(fd);

    close(fd);

    return 0;
}