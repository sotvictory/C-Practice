#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void)
{
    int size_lst = 0;
    list lst = NULL;
    build_list(&lst, &size_lst);

    tree t = NULL;
    t = build_tree(lst, size_lst);
    print_tree(t, 5);

    return 0;
}