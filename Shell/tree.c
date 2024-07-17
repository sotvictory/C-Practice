#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

#define ERRR (tree)(-1)

void make_shift(int n)
{
	while (n--)
		putc(' ', stderr);
}

void print_argv(char **p, int shift)
{
	char **q = p;

	if (p != NULL)
	{
		while (*p != NULL)
		{
			make_shift(shift);
			fprintf(stderr, "argv[%d]=%s\n", (int)(p - q), *p);
			p++;
		}
	}
}

void print_tree(tree t, int shift)
{
	char **p;

	if ((t == NULL) || (t == ERRR))
		return;

	p = t->argv;

	if (p != NULL)
		print_argv(p, shift);
	else
	{
		make_shift(shift);
		fprintf(stderr, "psubshell\n");
	}
	make_shift(shift);
	if (t->infile == NULL)
		fprintf(stderr, "infile=NULL\n");
	else
		fprintf(stderr, "infile=%s\n", t->infile);
	make_shift(shift);
	if (t->outfile == NULL)
		fprintf(stderr, "outfile=NULL\n");
	else
		fprintf(stderr, "outfile=%s\n", t->outfile);
	make_shift(shift);
	fprintf(stderr, "append=%d\n", t->append);
	make_shift(shift);
	fprintf(stderr, "background=%d\n", t->backgrnd);
	make_shift(shift);
	fprintf(stderr, "type=%s\n", t->type == NXT ? "NXT" : t->type == OR ? "OR"
																		: "AND");
	make_shift(shift);
	if (t->psubcmd == NULL)
		fprintf(stderr, "psubcmd=NULL \n");
	else
	{
		fprintf(stderr, "psubcmd---> \n");
		print_tree(t->psubcmd, shift + 5);
	}
	make_shift(shift);
	if (t->pipe == NULL)
		fprintf(stderr, "pipe=NULL \n");
	else
	{
		fprintf(stderr, "pipe---> \n");
		print_tree(t->pipe, shift + 5);
	}
	make_shift(shift);
	if (t->next == NULL)
		fprintf(stderr, "next=NULL \n");
	else
	{
		fprintf(stderr, "next---> \n");
		print_tree(t->next, shift + 5);
	}
}

tree cleartree(tree t)
{
	if ((t == NULL) || (t == ERRR))
		return NULL;

	cleartree(t->psubcmd);
	cleartree(t->pipe);
	cleartree(t->next);
	free(t->argv);
	free(t);

	return NULL;
}

static int cnt = 0;
static int brackets = 0;

#define _cmdlist_invalid_divider 50
#define _pipe_invalid_divider 49
#define _subshell 48
#define _brackets 47
#define _unexpected_left_bracket 46
#define _unexpected_right_bracket 45
#define _2_infiles 44
#define _2_outfiles 43
#define _0_infile 42
#define _0_outfile 41
#define _empty_command 40
#define _dynamic_mem 39

tree getcmdlist(list lst, int sizelist);
tree getpipe(list lst, int sizelist);
tree getcmd(list lst, int sizelist);
char *getword(list lst, int sizelist);

tree error(tree t, int what)
{
	t = cleartree(t);

	switch (what)
	{
	case _cmdlist_invalid_divider:
		fprintf(stderr, "Invalid command: met unexpected cmdlist divider\n");
		break;
	case _pipe_invalid_divider:
		fprintf(stderr, "Invalid command: met unexpected pipe divider\n");
		break;
	case _subshell:
		fprintf(stderr, "Invalid command: invalid subshell (args in brackets) calling\n");
		break;
	case _brackets:
		fprintf(stderr, "Invalid command: bracket balance is broken\n");
		break;
	case _unexpected_left_bracket:
		fprintf(stderr, "Invalid command: met unexpected \"(\"\n");
		break;
	case _unexpected_right_bracket:
		fprintf(stderr, "Invalid command: met unexpected \")\"\n");
		break;
	case _2_infiles:
		fprintf(stderr, "Invalid command: 2+ input files are stated\n");
		break;
	case _2_outfiles:
		fprintf(stderr, "Invalid command: 2+ output files are stated\n");
		break;
	case _0_infile:
		fprintf(stderr, "Invalid command: no input files are stated, but '<' had place\n");
		break;
	case _0_outfile:
		fprintf(stderr, "Invalid command: no output files are stated, but '>' or \">>\" had place\n");
		break;
	case _empty_command:
		fprintf(stderr, "Invalid command: there is empty command\n");
		break;
	case _dynamic_mem:
		fprintf(stderr, "Dynamic memory error\n");
	default:
		fprintf(stderr, "Invalid command\n");
		break;
	}

	return ERRR;
}

char *getword(list lst, int sizelist)
{
	return (cnt < sizelist - 1) ? lst[cnt++] : NULL;
}

tree emptinesscheck(tree t)
{
	tree nxt, pip;
	if (t == NULL)
		return NULL;

	if (t == ERRR)
		return ERRR;

	pip = emptinesscheck(t->pipe);
	if (pip == ERRR)
		return ERRR;
	if (pip == (tree)1)
		return error(t, _empty_command);
	if (emptinesscheck(t->psubcmd) == ERRR)
		return ERRR;
	if ((t->argv[0] == (char *)0) && (t->psubcmd == NULL) && (t->infile == NULL) &&
		(t->outfile == NULL))
	{
		if (t->pipe != NULL)
			return error(t, _empty_command);
		else if (t->next != NULL)
			return error(t, _empty_command);
		else
			return (tree)1;
	}

	nxt = emptinesscheck(t->next);
	if (nxt == (tree)1)
		t->next = cleartree(t->next);
	else if (nxt == ERRR)
		return ERRR;
		
	return t;
}

void setbackground(tree t, int wherecalled)
{
	tree buf = t;
	if (buf == NULL)
		return;
	if (wherecalled)
		while (buf->next != NULL)
		{
			if (buf->type == NXT)
				t = buf->next;
			buf = buf->next;
		}
	t->backgrnd = 1;
	setbackground(t->pipe, 0);
	//setbackground(t->psubcmd, 0);
	if (t->type != NXT)
		setbackground(t->next, 0);
}

tree getcmd(list lst, int sizelist)
{
	tree cmd;
	char *word;
	int isOutput = 0, isInput = 0, isSub = 0;
	int i = 0;

	if ((word = getword(lst, sizelist)) == NULL)
		return NULL;
	if ((cmd = malloc(sizeof(node))) == NULL)
		return error(cmd, _dynamic_mem);
	if ((cmd->argv = calloc(i + 1, sizeof(char *))) == NULL)
		return error(cmd, _dynamic_mem);

	cmd->argv[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->backgrnd = 0;
	cmd->psubcmd = NULL;
	cmd->pipe = NULL;
	cmd->next = NULL;
	cmd->type = NXT;

	do
	{
		if (!strcmp(word, "("))
		{
			if (i != 0)
				return error(cmd, _subshell);
			brackets++;
			cmd->psubcmd = getcmdlist(lst, sizelist);
			if (cmd->psubcmd == NULL)
				return error(cmd, _empty_command);
			if (cmd->psubcmd == ERRR)
				return ERRR;
			if (getword(lst, sizelist) == NULL)
				return error(cmd, _brackets);
			brackets--;
			isSub = 1;
		}
		else if (!strcmp(word, "<"))
			if (!isInput)
			{
				cmd->infile = getword(lst, sizelist);
				if (cmd->infile == NULL)
					return error(cmd, _0_infile);
				isInput = 1;
			}
			else
				return error(cmd, _2_infiles);
		else if (!strcmp(word, ">") || !strcmp(word, ">>"))
			if (!isOutput)
			{
				cmd->outfile = getword(lst, sizelist);
				if (cmd->outfile == NULL)
					return error(cmd, _0_outfile);
				isOutput = 1;
				if (!strcmp(word, ">>"))
					cmd->append = 1;
			}
			else
				return error(cmd, _2_outfiles);
		else if (!strcmp(word, ")"))
		{
			if (brackets - 1 < 0)
				return error(cmd, _brackets);
			cnt--;
			break;
		}
		else if (!strcmp(word, "&") || !strcmp(word, "&&") || !strcmp(word, "||") ||
				 !strcmp(word, ";") || !strcmp(word, "|"))
		{
			cnt--;
			break;
		}
		else
		{
			if (isSub)
				return error(cmd, _subshell);
			i++;
			if ((cmd->argv = realloc(cmd->argv, sizeof(char *) * (i + 1))) == NULL)
				return error(cmd, _dynamic_mem);
			cmd->argv[i - 1] = word;
			cmd->argv[i] = (char *)0;
		}
	} while ((word = getword(lst, sizelist)) != NULL);

	return cmd;
}

tree getpipe(list lst, int sizelist)
{
	tree cmd = getcmd(lst, sizelist), lastcmd = cmd;
	if (cmd == NULL)
		return NULL;
	else if (cmd == ERRR)
		return ERRR;

	char *divider;

	while ((divider = getword(lst, sizelist)) != NULL)
	{
		if (!strcmp(divider, "&"))
		{
			cnt--;
			break;
		}
		else if (!strcmp(divider, "&&"))
		{
			cnt--;
			break;
		}
		else if (!strcmp(divider, "||"))
		{
			cnt--;
			break;
		}
		else if (!strcmp(divider, ";"))
		{
			cnt--;
			break;
		}
		else if (!strcmp(divider, ")"))
		{
			if (brackets - 1 < 0)
				return error(cmd, _brackets);
			cnt--;
			break;
		}
		else if (strcmp(divider, "|"))
			return error(cmd, _pipe_invalid_divider);

		if ((lastcmd->pipe = getcmd(lst, sizelist)) == ERRR)
		{
			cmd = cleartree(cmd);
			return ERRR;
		}
		lastcmd = lastcmd->pipe;
		if (lastcmd == NULL)
			return error(cmd, _empty_command);
	}

	return cmd;
}

tree getcmdlist(list lst, int sizelist)
{
	tree pipe = getpipe(lst, sizelist), lastpipe = pipe;

	if (pipe == NULL)
		return NULL;
	if (pipe == ERRR)
		return ERRR;

	char *divider;

	while ((divider = getword(lst, sizelist)) != NULL)
	{
		if (!strcmp(divider, "&"))
		{
			lastpipe->type = NXT;
			setbackground(pipe, 1);
		}
		else if (!strcmp(divider, "&&"))
			lastpipe->type = AND;
		else if (!strcmp(divider, "||"))
			lastpipe->type = OR;
		else if (!strcmp(divider, ";"))
			lastpipe->type = NXT;
		else if (!strcmp(divider, ")"))
		{
			if (brackets - 1 < 0)
				return error(pipe, _brackets);
			cnt--;
			break;
		}
		else
			return error(pipe, _cmdlist_invalid_divider);
		if ((lastpipe->next = getpipe(lst, sizelist)) == ERRR)
		{
			pipe = cleartree(pipe);
			return ERRR;
		}
		if (lastpipe->next != NULL)
			lastpipe = lastpipe->next;
		else if (!strcmp(divider, "&&") || !strcmp(divider, "||"))
		{
			return error(pipe, _empty_command);
		}
	}

	if (pipe == NULL)
		return error(pipe, _empty_command);
	pipe = emptinesscheck(pipe);

	return pipe == (tree)1 ? error(NULL, _empty_command) : pipe;
}

tree buildtree(list lst, int sizelist)
{
	cnt = 0;
	brackets = 0;

	tree t = getcmdlist(lst, sizelist);

	return t == ERRR ? NULL : t;
}