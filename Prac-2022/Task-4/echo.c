#include <unistd.h>

#define n_flag 0 
#define e_flag 1

void my_putchar(char c, int fd)
{
	write(fd, &c, 1);
}

void my_putstr(const char *s, int *flags, int fd)
{
	if (s)
		while (*s) {
            if (*s == '\\' && flags[e_flag] == 1) {
                ++s;
                switch (*s) {
                    case 'n': 
                        my_putchar('\n', fd);
                        break;
                    case 't':
                        my_putchar('\t', fd);
                        break;
                    case 'v':
                        my_putchar('\v', fd);
                        break;
                    case 'b':
                        my_putchar('\b', fd);
                        break;
                    case 'r':
                        my_putchar('\r', fd);
                        break;
                    default: 
                        my_putchar('\\', fd);
                        my_putchar(*s, fd);
                }
            } else
                my_putchar(*s, fd);
            ++s;
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

void print_echo(char *argv[], int *flags, int i_start, int fd)
{
    int i = i_start;

	while (argv[i]) {
		my_putstr(argv[i], flags, fd);
		if (argv[++i])
			my_putchar(' ', fd);
	}
    if (flags[n_flag] == 0)
        my_putchar('\n', fd);
}

int main(int argc, char *argv[])
{
    //- n do not output the trailing newline
    // -e enable interpretation of backslash escapes
    // -E disable interpretation of backslash escapes (default)

    int flags [2] = {0}; // -n, -e/E
 	int fd = 1, i = 1;

    // no arguments
	if (argc < 2) {
		my_putchar('\n', fd);
		return 0;
	}
    // check flags
    while (argv[i][0] == '-') {
        if (!my_strcmp("-n", argv[i])) {
            flags[n_flag] = 1;
            ++i;
        } else if (!my_strcmp("-e", argv[i])) {
            flags[e_flag] = 1;
            ++i;
        } else if (!my_strcmp("-E", argv[i])) {
            flags[e_flag] = 0;
            ++i;
        } else
            break;
    }
	print_echo(argv, flags, i, fd);
	return 0;
}