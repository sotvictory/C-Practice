#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lexer.h"
#include "parser.h"

#define OPEN_ERR 1

int main(int argc, char **argv)
{
    int i = 0, size_lst = 0, fd = 0;
    list lst = NULL;
    tree t = NULL;

    if (argc == 2) {
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            perror("open");
            exit(OPEN_ERR);
        }
    }

    /* list */
    build_list(&lst, &size_lst, fd);
    close(fd);

    /* tree */
    t = build_tree(lst, size_lst);
    clear_list(&lst, &size_lst);
    print_tree(t, PRINT_SHIFT);
    clear_tree(t);
    
    return 0;
}