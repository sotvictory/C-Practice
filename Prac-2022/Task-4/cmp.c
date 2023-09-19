#include <fcntl.h>
#include <unistd.h>

void my_putchar(char c, int fd)
{
	write(fd, &c, 1);
}

void putnum(int n, int fd)
{
	if (n > 9)
	    putnum(n / 10, fd);
	my_putchar((n % 10) + '0', fd);
}

void printstr(const char *s, int fd)
{
	if (s) {
		while (*s) {
			my_putchar(*s, fd);
			++s;
		}
		my_putchar(' ', fd);
    }
}

void printdiff(char *argv[], int byte, int line)
{
    printstr(argv[1], 1);
    printstr(argv[2], 1);
    printstr("differ: byte", 1);
    putnum(byte, 1);
    printstr(", line", 1);
    putnum(line, 1);
    my_putchar('\n', 1);
}

void printeof(const char *s, int byte, int line)
{
    printstr("cmp: EOF on", 1);
    printstr(s, 1);
    printstr("after byte", 1);
    putnum(byte, 1);
    printstr(", line", 1);
    putnum(line, 1);
    my_putchar('\n', 1);
}

int	main(int argc, char *argv[]) 
{
    int fd1, fd2, byte = 1, line = 1;
    char data1, data2;

    if (argc == 3) {
        if ((fd1 = open(argv[1], O_RDONLY)) < 0 || (fd2 = open(argv[2], O_RDONLY)) < 0)
            return 1;
        while (1) {
            if ((read(fd1, &data1, 1)) > 0) {
                if ((read(fd2, &data2, 1)) > 0) {
                    // not EOF in both
                    if (data1 != data2) {
                        printdiff(argv, byte, line);
                        break;
                    // end of line
                    } else if (data1 == '\n' && data1 == '\n') {
                        ++line;
                        ++byte;
                    } else {
                        ++byte;
                    }
                } else {
                    // EOF in second, but not EOF in first
                    printeof(argv[2], byte - 1, line - 1);
                    break;            
                }
            } else {
                // EOF in first, but not EOF in second
                if ((read(fd2, &data2, 1)) > 0) {
                    printeof(argv[1], byte - 1, line - 1);
                    break;
                // EOF in both
                } else {
                    break;
                }        
            }
        }
    // incorrect input
    } else
        return 1;
    close(fd1);
    close(fd2);
    return 0;
}