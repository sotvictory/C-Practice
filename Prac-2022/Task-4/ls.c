#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

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
		my_putchar(' ', fd);
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

int	main(int argc, char *argv[])
{
    char *path = NULL;
    DIR *dir = NULL;
    struct dirent *dd = NULL;

    // get the path
	path = getcwd(NULL, 0);
	if (path == NULL)
        return 1;
    if ((dir = opendir(path)) != NULL) {
        // print all the files and directories
        while ((dd = readdir(dir)) != NULL) {
            // skip . and ..
            if (my_strcmp(dd->d_name, ".") && my_strcmp(dd->d_name, ".."))
                printstr(dd->d_name, 1);
        }
        closedir(dir);
    } else {
        free(path);
        return 1;
    }
    my_putchar('\n', 1);
    free(path);
    return 0;
}