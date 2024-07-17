#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGE_SIZE 512

int main(int argc, char **argv)
{
    int fd;
    unsigned short int start_addr, virt_addr, page_offset, virt_page_num, phys_page_num, data;

    if ((fd = open(argv[1], O_RDONLY)) == -1)
       exit(1);

    start_addr = (unsigned short int) strtol(argv[2], NULL, 16);

    while (scanf("%hx", &virt_addr) == 1) {
        virt_page_num = virt_addr / PAGE_SIZE;
        page_offset = virt_addr % PAGE_SIZE;

        lseek(fd, start_addr + 2 * virt_page_num, SEEK_SET);
        read(fd, (void *)(&phys_page_num), sizeof(unsigned short int));
        phys_page_num += page_offset;

        lseek(fd, phys_page_num, SEEK_SET);
        read(fd, (void *)(&data), sizeof(unsigned short int));
        printf("%hu\n", data);
    }

    close(fd);
    exit(0);
}