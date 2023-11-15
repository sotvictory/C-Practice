#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#define INV_LEN 100
#define OPEN_ERR 1

void invitation(void)
{
    char s[INV_LEN];

    getcwd(s, 100);
    fprintf(stderr, "%s", "\033[01;36m"); // синий \033[22;34m
    fprintf(stderr, "%s ", s);

    gethostname(s, INV_LEN);
    fprintf(stderr, "%s", "\x1b[32m"); // зеленый
    fprintf(stderr, "%s@%s:~$ ", getenv("USER"), s);

    fprintf(stderr, "%s", "\x1B[37m"); // серый
}

int main(int argc, char **argv)
{
    int i = 0, size_lst = 0, input_fd = 0, output_fd = 1;
    list lst = NULL;
    tree t = NULL;

    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if ((output_fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
                    fprintf(stderr, "open() failed: %s\n", strerror(errno));
                    exit(OPEN_ERR);
                }  
            } else if (strcmp(argv[i], "-i") == 0) {
                if ((input_fd = open(argv[i + 1], O_RDONLY, 0)) < 0) {
                    fprintf(stderr, "open() failed: %s\n", strerror(errno));
                    exit(OPEN_ERR);
                }               
            }
        }
    }

    while (1) {
        if (input_fd == 0)
            invitation();

        build_list(&lst, &size_lst, input_fd, output_fd);
        if (lst == NULL)
            break;

        if (lst != LIST_ERR) {
            //print_list(lst, size_lst, output_fd);
            t = build_tree(lst, size_lst);
            if (t != TREE_ERR) {
                //print_tree(t, PRINT_SHIFT);
                execute_cmd(t);
            }
            clear_list(&lst, &size_lst);
            if (t != TREE_ERR)
                clear_tree(t);
        }
    }

    close(input_fd);
    close(output_fd);

    return 0;
}