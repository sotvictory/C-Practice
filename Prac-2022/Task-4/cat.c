#include <fcntl.h>
#include <unistd.h>

void my_putchar(char c, int fd)
{
	write(fd, &c, 1);
}

void printstr(const char *s, int fd)
{
	if (s) {
		while (*s) {
			my_putchar(*s, fd);
			++s;
		}
		my_putchar('\n', fd);
    }
}

void putnum(int n, int fd)
{
	if (n > 9)
	    putnum(n / 10);
	my_putchar((n % 10) + '0', fd);
}

int my_strcmp(const char *s1, const char *s2)
{
	int i = 0;

    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0')
            return 0;
    }
	return (unsigned char)s1[i] - (unsigned char)s2[i];
}

void print_cat(int argc, char *argv[], int flag_n)
{
    int i, fd, flag_num = 1, line = 1;
    char data = 0;

    // file processing
    for (i = 1; i < argc; i++) {
        // skip the flag
        if (argv[i][0] != '-') {
            fd = open(argv[i], O_RDONLY);
            while (read(fd, &data, 1) == 1) {
                // print with number
                if (flag_n == 1) {
                    if (data == '\n') {
                        // empty line
                        if (flag_num)
                            putnum(line, 1);
                        // new line
                        ++line;
                        my_putchar('\n', 1);
                        flag_num = 1;
                    // just next character
                    } else {
                        if (flag_num) {
                            putnum(line, 1);
                            my_putchar(' ', 1);  
                            my_putchar(data, 1);
                            flag_num = 0;                      
                        } else {
                            my_putchar(data, 1);
                        }
                        flag_num = 0;
                    }
                // print without number
                } else
                    my_putchar(data, 1);
            }
            close(fd);          
        }
    }
}

int main(int argc, char *argv[])
{
    int flag_n = 0; //- n number all output lines
 	int i = 0;

    // check flag -n
    for (i = 1; i < argc; i++) {
        if (!my_strcmp("-n", argv[i])) {
            flag_n = 1;
            break;
        }
    }
	print_cat(argc, argv, flag_n);
	return 0;
}