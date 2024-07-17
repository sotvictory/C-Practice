#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "list.h"
#include "exec.h"

static int bgcnt = 0;
static int status;
static intlist foreground = NULL;
static tree *cmd = NULL;
static list *cmd_list = NULL;

enum {
	NO_INP_FILE,
	INP_FILE_ERR,
	NO_OUT_FILE_ERR
};

void errorcmd(int what, list lst, int *sizelist, tree t, intlist zombielist);
int execd(tree t);
int exeexit(tree t, intlist *zombielist, list lst, int *sizelist);
void execmd(tree t_orig, tree t, int isinputpiped, list lst, int *sizelist, intlist zombielist);
int exepipe(tree t, intlist *zombielist, list lst, int *sizelist);
int exelist(tree t, intlist *zombielist, list lst, int *sizelist);
void waitfg(intlist *fg, int *ret);

static void clearresources(list lst, int *sizelist, tree t, intlist zombielist)
{
	*cmd = cleartree(*cmd);
	*cmd_list = clearlist(*cmd_list, sizelist);
	clearintlist(zombielist);
}

static intlist addpid(pid_t pid, int cnt, char *name, intlist zombielist)
{
	struct backgrndList laststruct = {0, 0, zombielist, NULL};
	intlist last = &laststruct;

	while (last->next != NULL)
		last = last->next;

	if ((last->next = malloc(sizeof(struct backgrndList))) == NULL)
	{
		fprintf(stderr, "Can't allocate dynamic memory for pid storing. Zombie #%d will exist =(\n", pid);
		return laststruct.next;
	}

	last = last->next;
	last->next = NULL;
	last->pid = pid;
	last->cnt = cnt;

	if (name == NULL)
		last->name = NULL;
	else
	{
		if ((last->name = calloc(strlen(name) + 1, sizeof(char))) == NULL)
			fprintf(stderr, "Can't allocate dynamic memory for [%d]'s name\n", cnt);
		else
			strcpy(last->name, name);
	}

	return laststruct.next;
}

intlist clearintlist(intlist zombielist)
{
	if (zombielist == NULL)
		return NULL;

	clearintlist(zombielist->next);

	if (zombielist->name != NULL)
		free(zombielist->name);
	free(zombielist);

	return NULL;
}

void errorcmd(int what, list lst, int *sizelist, tree t, intlist zombielist)
{
	switch (what)
	{
		case NO_INP_FILE:
			fprintf(stderr, "No such input file exist\n");
			break;

		case INP_FILE_ERR:
			fprintf(stderr, "Error while opening input file\n");
			break;

		case NO_OUT_FILE_ERR:
			fprintf(stderr, "Error while opening output file\n");
			break;

		default:
			fprintf(stderr, "Error in command executing\n");
			break;
	}

	clearresources(lst, sizelist, t, zombielist);
	exit(1);
}

int execd(tree t)
{
	if (t->argv[1] == NULL)
		return chdir(getenv("HOME"));
	else
		return chdir(t->argv[1]);
}

int exeexit(tree t, intlist *zombielist, list lst, int *sizelist)
{
	int code = 0;

	killall(*zombielist, 2);

	if (t->argv[1] != NULL)
		code = atoi(t->argv[1]);
	clearresources(lst, sizelist, t, zombielist ? *zombielist : NULL);

	exit(code);
}

void execmd(tree t_orig, tree t, int isinputpiped, list lst, int *sizelist, intlist zombielist)
{
	int fdin = 0, fdout = 0, bg = t->backgrnd;

	if ((t->argv[0] != NULL) && (!strcmp(t->argv[0], "cd")))
		exit(0);
	if ((t->argv[0] != NULL) && (!strcmp(t->argv[0], "exit")))
		exit(0);

	if (t->infile != NULL)
	{
		fdin = open(t->infile, O_RDONLY);
		if (fdin == -1)
		{
			if (errno == ENOENT)
			{
				errorcmd(NO_INP_FILE, lst, sizelist, t, zombielist);
			}
			else
			{
				errorcmd(INP_FILE_ERR, lst, sizelist, t, zombielist);
			}
		}
	}

	if (t->outfile != NULL)
	{
		if (t->append)
			fdout = open(t->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
		else
			fdout = open(t->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fdout == -1)
			errorcmd(NO_OUT_FILE_ERR, lst, sizelist, t, zombielist);
	}

	if (!fdin && !isinputpiped && bg)
		fdin = open("/dev/null", O_RDONLY);

	if (fdin)
	{
		dup2(fdin, 0);
		close(fdin);
	}

	if (fdout)
	{
		dup2(fdout, 1);
		close(fdout);
	}
	signal(SIGINT, bg ? SIG_IGN : SIG_DFL);

	if (t->psubcmd)
	{
		int code;
		foreground = NULL;
		code = exelist(t->psubcmd, NULL, lst, sizelist);
		clearresources(lst, sizelist, t, zombielist);
		exit(code);
	}
	else
	{
		if (t->argv[0] != NULL)
		{
			execvp(t->argv[0], t->argv);
			struct backgrndList *tmp;
			while (foreground != NULL)
			{
				tmp = foreground;
				foreground = foreground->next;
				free(tmp->name);
				free(tmp);
			}
			// killall(foreground, 2);
		}
		else
		{
			exit(0);
		}
		// fprintf(stderr,"before error:\n");
		// print_tree(t, 1);

		fprintf(stderr, "shell: command not found: %s\n", t->argv[0]);
		clearresources(lst, sizelist, t_orig, zombielist);
		exit(1);
	}
}

#define addpidm                                                                            \
	if (t->backgrnd)                                                                       \
	{                                                                                      \
		if (zombielist != NULL)                                                            \
		{                                                                                  \
			printf("[%d] %d\n", ++bgcnt, pid);                                             \
			lastpid = pid;                                                                 \
			*zombielist = addpid(pid, bgcnt, t->argv[0], zombielist ? *zombielist : NULL); \
		}                                                                                  \
	}                                                                                      \
	else                                                                                   \
	{                                                                                      \
		foreground = addpid(pid, 0, t->argv[0], foreground);                               \
	}

int exepipe(tree t, intlist *zombielist, list lst, int *sizelist)
{

	int fd[2], in, out, next_in, ret = 0, codewait = 0;
	pid_t pid, lastpid;
	tree t_orig = t;

	if (t->type != NXT && t->backgrnd)
		codewait = 1;

	if (t->pipe == NULL)
	{
		if ((t->argv[0] != NULL) && (!strcmp(t->argv[0], "cd")))
			return execd(t);
		if ((t->argv[0] != NULL) && (!strcmp(t->argv[0], "exit")))
			return exeexit(t, zombielist, lst, sizelist);
		if ((pid = fork()) == 0)
		{
			execmd(t_orig, t, 0, lst, sizelist, zombielist ? *zombielist : NULL);
		}
		else
		{
			addpidm
		}
	}
	else
	{
		pipe(fd);
		out = fd[1];
		next_in = fd[0];

		/*1st process*/
		if ((pid = fork()) == 0)
		{
			close(next_in);
			dup2(out, 1);
			close(out);
			execmd(t_orig, t, 0, lst, sizelist, zombielist ? *zombielist : NULL);
		}
		addpidm
			in = next_in;

		for (t = t->pipe; t->pipe != NULL; t = t->pipe)
		{
			close(out);
			pipe(fd);
			out = fd[1];
			next_in = fd[0];
			if ((pid = fork()) == 0)
			{
				close(next_in);
				dup2(in, 0);
				close(in);
				dup2(out, 1);
				close(out);
				execmd(t_orig, t, 1, lst, sizelist, zombielist ? *zombielist : NULL);
			}
			addpidm
				close(in);
			in = next_in;
		}
		close(out);

		/*last process*/
		if ((pid = fork()) == 0)
		{
			dup2(in, 0);
			close(in);
			execmd(t_orig, t, 1, lst, sizelist, zombielist ? *zombielist : NULL);
		}
		addpidm
			close(in);
	}
	waitfg(&foreground, &ret);

	if (codewait)
	{
		waitpid(lastpid, &status, 0);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		else
			ret = 1;
	}

	return ret;
}

int exelist(tree t, intlist *zombielist, list lst, int *sizelist)
{
	int code = 0, run = 1;

	while (t != NULL)
	{
		if (run)
			code = exepipe(t, zombielist, lst, sizelist);
		switch (t->type)
		{
		case NXT:
			run = 1;
			break;

		case AND:
			if (code)
				run = 0;
			else
				run = 1;
			break;

		case OR:
			if (!code)
				run = 0;
			else
				run = 1;
			break;
		}
		t = t->next;
	}

	return code;
}

void exe(tree t, intlist *zombielist, list lst, int *sizelist)
{
	int code = 1, i, letters = 1;
	char *buf;
	cmd = &t;
	cmd_list = &lst;

	if (t != NULL)
		code = exelist(t, zombielist, lst, sizelist);
	i = code / 10;
	while (i != 0)
	{
		i /= 10;
		letters++;
	}
	if ((buf = calloc(letters + 1, sizeof(char))) == NULL)
	{
		fprintf(stderr, "Can't allocate dynamic memory for $? storing\n");
		return;
	}
	sprintf(buf, "%d", code);
	setenv("?", buf, 1);
	free(buf);

	printf("exit code: %d\n", code);
}

intlist waitzombies(intlist zombielist)
{
	int i;
	struct backgrndList wstruct = {0, 0, zombielist, NULL};
	intlist del, w = &wstruct;

	while (w->next != NULL)
	{
		if (waitpid(w->next->pid, &status, WNOHANG))
		{
			printf("[%d] %d %s finished\n", w->next->cnt, w->next->pid, w->next->name);
			del = w->next;
			w->next = w->next->next;
			if (del->name != NULL)
				free(del->name);
			free(del);
		}
		else
			w = w->next;
	}
	waitfg(&foreground, &i);

	return wstruct.next;
}

void waitfg(intlist *fg, int *ret)
{
	struct backgrndList wstruct = {0, 0, *fg, NULL};
	intlist del, w = &wstruct;

	while (w->next != NULL)
	{
		if (waitpid(w->next->pid, &status, 0))
		{
			if (w->next->next == NULL)
			{
				if (WIFEXITED(status))
					*ret = WEXITSTATUS(status);
				else
					*ret = 1;
			}
			del = w->next;
			w->next = w->next->next;
			if (del->name != NULL)
				free(del->name);
			free(del);
			*fg = wstruct.next;
		}
		else
			w = w->next;
	}
}

void killall(intlist zombielist, int level)
{
	if (!level)
		return;

	struct backgrndList wstruct = {0, 0, zombielist, NULL};
	intlist del, w = &wstruct;

	while (w->next != NULL)
	{
		kill(w->next->pid, SIGKILL);
		del = w->next;
		w->next = w->next->next;
		if (del->name != NULL)
			free(del->name);
		free(del);
	}

	killall(foreground, level - 1);
}