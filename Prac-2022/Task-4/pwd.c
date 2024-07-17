#include <unistd.h>
#include <stdlib.h>

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

int	main(int argc, char *argv[])
{
	char *path = NULL;

	path = getcwd(NULL, 0);
	if (path == NULL)
        return 1;
	printstr(path, 1);
	free(path);
	return 0;
}