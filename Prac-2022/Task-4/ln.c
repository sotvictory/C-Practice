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
    }
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

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4) {
        return 1;
    } else {
        // symbolic link
        if ((argc == 4) && !my_strcmp("-s", argv[1])) { 
            // file2 exists
            if (access(argv[3], F_OK) == 0) {
                printstr("ln: failed to create symbolic link '", 1);
                printstr(argv[3], 1);
                printstr("': File exists", 1);  
                my_putchar('\n', 1);
                return 1;
            // create a symbolic link                
            } else {
                symlink(argv[2], argv[3]);
                return 0;
            }
        // hard link
        } else if (argc == 3) {
            // file1 exists
            if (access(argv[1], F_OK) == 0) {
                // file2 exists
                if (access(argv[2], F_OK) == 0) {
                    printstr("ln: failed to create hard link '", 1);
                    printstr(argv[2], 1);
                    printstr("': File exists", 1);  
                    my_putchar('\n', 1);
                    return 1;
                // create a hard link                  
                } else {
                    link(argv[1], argv[2]);
                    return 0;
                }
            // file1 does not exist
            } else {
                printstr("ln: failed to access '", 1);
                printstr(argv[1], 1);
                printstr("': No such file or directory", 1);
                my_putchar('\n', 1);
                return 1;
            }
        }
    }
}