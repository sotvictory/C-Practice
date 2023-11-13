#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lexer.h"
#include "parser.h"

#define OPEN_ERR 1

int main(int argc, char **argv)
{
    int i = 0, size_lst = 0, input_fd = 0, output_fd = 1;
    list lst = NULL;
    tree t = NULL;

    if (argc == 3) {
        if ((input_fd = open(argv[1], O_RDONLY)) < 0) {
            perror("open");
            exit(OPEN_ERR);
        }
        if ((output_fd = open(argv[2], O_WRONLY)) < 0) {
            perror("open");
            exit(OPEN_ERR);
        }
    }

    /* list */
    while (1) {
        build_list(&lst, &size_lst, input_fd, output_fd);
        if (lst == NULL)
            break;
        print_list(lst, size_lst, output_fd);
        clear_list(&lst, &size_lst);
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