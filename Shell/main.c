#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

#define INV_LEN 100
#define OPEN_ERR 1

void invitation(void)
{
    char s[INV_LEN];
    gethostname(s, INV_LEN);
    fprintf(stderr, "%s", "\x1b[32m");
    fprintf(stderr, "%s@%s:~$ ", getenv("USER"), s);
    fprintf(stderr, "%s", "\x1B[37m");
}

int main(int argc, char **argv)
{
    int i = 0, size_lst = 0, input_fd = 0, output_fd = 1; // + log_fd
    list lst = NULL;
    //tree t = NULL;

    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if ((output_fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
                    perror("open");
                    exit(OPEN_ERR);
                }  
            } else if (strcmp(argv[i], "-i") == 0) {
                if ((input_fd = open(argv[i + 1], O_RDONLY, 0)) < 0) {
                    perror("open");
                    exit(OPEN_ERR);
                }
            }
        }
    }

    /* list */
    while (1) {
        if (input_fd == 0)
            invitation();
        build_list(&lst, &size_lst, input_fd, output_fd);
        if (lst == NULL)
            break;
        if (lst != (list)-1) {
            print_list(lst, size_lst, output_fd);
            clear_list(&lst, &size_lst);
        }
    }

    close(input_fd);
    close(output_fd);

    #if 0
    /* tree */
    t = build_tree(lst, size_lst);
    clear_list(&lst, &size_lst);
    print_tree(t, PRINT_SHIFT);
    clear_tree(t);
    #endif

    return 0;
}