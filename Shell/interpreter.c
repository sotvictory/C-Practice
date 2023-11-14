#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"
#include "interpreter.h"

void kitty(void)
{
    printf(" /\\_/\\ \n( o.o )\n > ^ <\n");
}

void cd(tree t)
{
	if (t->argv[1] == NULL)
		chdir(getenv("HOME"));
	else
		chdir(t->argv[1]);
    
    return;
}

void execute_cmd(tree t)
{
    if (strcmp((t->argv)[0], "cd") == 0) {
        cd(t);
        return;
    } else if (strcmp((t->argv)[0], "kitty") == 0) {
        kitty();
        return;
    }

    pid_t pid = fork();

    if (pid == 0) {
        execvp((t->argv)[0], t->argv);
        perror("execvp");
        exit(1);
    } else if (pid < 0) {
        perror("fork");
    } else {
        waitpid(pid, NULL, 0);
    }

    return;
}
