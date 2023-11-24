#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#define INV_LEN 100
#define stream stdout

void clear_resources(list *lst, int *size_lst, tree cmd, process_list zombie_lst)
{
    clear_list(lst, size_lst);
    clear_tree(cmd);
    clear_zombie(zombie_lst, 2);
}

void invitation(void)
{
    char s[INV_LEN];

    getcwd(s, INV_LEN);
    fprintf(stream, "%s", "\033[01;36m");
    fprintf(stream, "%s ", s);

    gethostname(s, INV_LEN);
    fprintf(stream, "%s", "\x1b[32m");
    fprintf(stream, "%s@%s:~$ ", getenv("USER"), s);

    fprintf(stream, "%s", "\x1B[37m");
    fflush(stream);
}

int main(int argc, char **argv)
{
    int size_lst = 0;
    list lst = NULL;
    tree cmd = NULL;
    process_list zombie_lst = NULL;

    while (1) {
        clear_resources(&lst, &size_lst, cmd, zombie_lst);
        invitation();
        lst = build_list(&lst, &size_lst, 0, 1);
        if (lst == NULL) {
            exit(0);
        } else if (lst != LIST_ERR) {
            cmd = build_tree(lst, size_lst);
            //print_tree(cmd, 5);
            if (cmd != TREE_ERR) {
                execute(cmd, &zombie_lst);
                // если с ошибкой -- очистить все
            } else if (cmd == TREE_ERR) {
                clear_resources(&lst, &size_lst, cmd, zombie_lst);
            }
        } else if (lst == LIST_ERR) {
            clear_resources(&lst, &size_lst, cmd, zombie_lst);
        }
        wait_zombies(zombie_lst); // ???
        while (wait(NULL) > 0) {};
    }

    exit(0);
}