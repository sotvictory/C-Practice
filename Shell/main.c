#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include "exec.h"

#define INV_LEN 100

sigjmp_buf cycle;

void inv(void)
{
    char s[INV_LEN];

    getcwd(s, INV_LEN);
    fprintf(stdout, "%s", "\033[01;36m");
    fprintf(stdout, "%s ", s);

    gethostname(s, INV_LEN);
    fprintf(stdout, "%s", "\x1b[32m");
    fprintf(stdout, "%s@%s:~$ ", getenv("USER"), s);

    fprintf(stdout, "%s", "\x1B[37m");
    fflush(stdout);
}

void siginth(int c)
{
	printf("\n");
	siglongjmp(cycle, 1);
}

int main(int argc, char **argv)
{
	int sizelist = 0;
	list lst = NULL;
	tree t = NULL;
	intlist zombielist = NULL;

	signal(SIGINT, &siginth);

	while (1)
	{
		sigsetjmp(cycle, 1);
		t = cleartree(t);
		lst = clearlist(lst, &sizelist);
		zombielist = waitzombies(zombielist);
		inv();
		lst = dolist(&sizelist);
		if (lst == (list)(-1))
		{
			printf("\n");
			killall(zombielist, 2);
			exit(0);
		}
		lst = setvar(lst, sizelist);
		t = buildtree(lst, sizelist);
		exe(t, &zombielist, lst, &sizelist);
	}
}