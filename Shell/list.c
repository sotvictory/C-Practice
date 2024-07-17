#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "list.h"

#define SIZE 16

typedef enum
{
	Start,
	Word,
	Greater,
	Vertical,
	Ampersand,
	SpecialEnd,
	Comment,
	Stop,
	Eof
} vertex;

vertex start();
vertex word();
vertex greater();
vertex vertical();
vertex ampersand();
vertex specialEnd();
vertex comment();

static char *buf;
static int sizebuf;
static int curbuf;
static int curlist;

static int c;
static list lst;
static int sizelist;
static int isQuoting;

list clearlist(list lst, int *sizelistPoint)
{
	int i;
	*sizelistPoint = 0;
	curlist = 0;

	if (lst == NULL)
		return lst;

	for (i = 0; lst[i] != NULL; i++)
		free(lst[i]);

	free(lst);
	lst = NULL;

	return lst;
}

list null_list(list lst, int *sizelistPoint)
{
	*sizelistPoint = 0;
	curlist = 0;
	lst = NULL;
	return lst;
}

list termlist(list lst, int *sizelistPoint)
{
	int sizelist = *sizelistPoint;
	if (lst == NULL)
	{
		*sizelistPoint = sizelist;
		return lst;
	}

	if (curlist > sizelist - 1)
		lst = realloc(lst, (sizelist + 1) * sizeof(*lst));

	lst[curlist] = NULL;

	lst = realloc(lst, (sizelist = curlist + 1) * sizeof(*lst));
	*sizelistPoint = sizelist;

	return lst;
}

void nullbuf()
{
	buf = NULL;
	sizebuf = 0;
	curbuf = 0;
}

void addsym(int c)
{
	if (curbuf > sizebuf - 1)
		buf = realloc(buf, sizebuf += SIZE);
	buf[curbuf++] = c;
}

list addword(list lst, int *sizelistPoint)
{
	int sizelist = *sizelistPoint;

	if (curbuf > sizebuf - 1)
		buf = realloc(buf, sizebuf += 1);
	buf[curbuf++] = '\0';
	buf = realloc(buf, sizebuf = curbuf);

	if (curlist > sizelist - 1)
		lst = realloc(lst, (sizelist += SIZE) * sizeof(*lst));
	lst[curlist++] = buf;
	*sizelistPoint = sizelist;

	return lst;
}

void printlist(list lst, int sizelist)
{
	int i;
	if (lst == NULL)
		return;
	for (i = 0; i < sizelist - 1; i++)
		printf("%s\n", lst[i]);
}

int symset(int c)
{
	return (
			   c != '\n' &&
			   c != ' ' &&
			   c != '\t' &&
			   c != '>' &&
			   c != '|' &&
			   c != '&' &&
			   c != ';' &&
			   c != '<' &&
			   c != '(' &&
			   c != ')' &&
			   c != '#' &&
			   c != EOF) ||
		   isQuoting;
}

vertex start()
{
	if (c == ' ' || c == '\t')
	{
		c = getchar();
		return Start;
	}
	else if (c == EOF)
	{
		lst = termlist(lst, &sizelist);
		lst = clearlist(lst, &sizelist);
		return Eof;
	}
	else if (c == '\n')
	{
		lst = termlist(lst, &sizelist);
		return Stop;
	}
	else if (c == '\"')
	{
		nullbuf();
		c = getchar();
		isQuoting = 1;
		return Word;
	}
	else if (c == '#')
	{
		return Comment;
	}
	else
	{
		char cprev = c;
		nullbuf();
		if (c != '\\')
		{
			addsym(c);
			c = getchar();
		}
		switch (cprev)
		{
		case '>':
			return Greater;
			break;

		case '|':
			return Vertical;
			break;

		case '&':
			return Ampersand;
			break;

		case ';':
		case '<':
		case '(':
		case ')':
			return SpecialEnd;
			break;

		default:
			return Word;
			break;
		}
	}
}

vertex word()
{
	if (c == '\"')
	{
		isQuoting = 0;
		c = getchar();
		return Word;
	}

	if (symset(c))
	{
		if (c != '\\')
			addsym(c);
		else
		{
			c = getchar();
			if (c != '\n')
				addsym(c);
			else
			{
				c = getchar();
				addsym(c);
			}
		}
		c = getchar();
		return Word;
	}
	else
	{
		lst = addword(lst, &sizelist);
		return Start;
	}
}

vertex greater()
{
	if (c == '>')
	{
		addsym(c);
		c = getchar();
		return SpecialEnd;
	}
	else
	{
		lst = addword(lst, &sizelist);
		return Start;
	}
}

vertex vertical()
{
	if (c == '|')
	{
		addsym(c);
		c = getchar();
		return SpecialEnd;
	}
	else
	{
		lst = addword(lst, &sizelist);
		return Start;
	}
}

vertex ampersand()
{
	if (c == '&')
	{
		addsym(c);
		c = getchar();
		return SpecialEnd;
	}
	else
	{
		lst = addword(lst, &sizelist);
		return Start;
	}
}

vertex specialEnd()
{
	lst = addword(lst, &sizelist);
	return Start;
}

vertex comment()
{
	c = getchar();
	if (c == '\n')
	{
		lst = termlist(lst, &sizelist);
		return Stop;
	}
	else
		return Comment;
}

list dolist(int *sizelistPoint)
{
	vertex V = Start;
	isQuoting = 0;
	sizelist = *sizelistPoint;
	c = getchar();
	lst = null_list(lst, &sizelist);
	while (1)
	{
		switch (V)
		{
		case Start:
			V = start();
			break;

		case Word:
			V = word();
			break;

		case Greater:
			V = greater();
			break;

		case Vertical:
			V = vertical();
			break;

		case Ampersand:
			V = ampersand();
			break;

		case SpecialEnd:
			V = specialEnd();
			break;

		case Comment:
			V = comment();
			break;

		case Eof:
		case Stop:
			break;
		}

		if (V == Stop)
			break;
		if (V == Eof)
			return (list)(-1);
	}

	*sizelistPoint = sizelist;

	return lst;
}

char *swapenv(char *lst)
{
	int len;
	if (lst != NULL)
		len = strlen(lst);
	else
		len = 1;
	char *buf = calloc(len + 1, sizeof(char));
	if (lst != NULL)
		strcpy(buf, lst);
	else
		strcpy(buf, "0\0");
	return buf;
}

list setvar(list lst, int sizelist)
{
	int i, j = 1, power10 = 10;

	for (i = 0; i < sizelist - 1; i++)
	{
		if (lst[i][0] == '$')
		{
			if (!strcmp(lst[i], "$HOME"))
			{
				free(lst[i]);
				lst[i] = getenv("HOME");
				lst[i] = swapenv(lst[i]);
			}
			else if (!strcmp(lst[i], "$SHELL"))
			{
				free(lst[i]);
				lst[i] = getenv("SHELL");
				lst[i] = swapenv(lst[i]);
			}
			else if (!strcmp(lst[i], "$USER"))
			{
				free(lst[i]);
				lst[i] = getenv("USER");
				lst[i] = swapenv(lst[i]);
			}
			else if (!strcmp(lst[i], "$?"))
			{
				free(lst[i]);
				lst[i] = getenv("?");
				lst[i] = swapenv(lst[i]);
			}
			else if (!strcmp(lst[i], "$USER"))
			{
				free(lst[i]);
				lst[i] = getenv("USER");
				lst[i] = swapenv(lst[i]);
			}
			else if (!strcmp(lst[i], "$EUID"))
			{
				free(lst[i]);
				int euid = geteuid();
				while (euid / power10 != 0)
				{
					j++;
					power10 *= 10;
				}
				lst[i] = calloc(j + 1, sizeof(char));
				sprintf(lst[i], "%d", euid);
			}
		}
	}

	return lst;
}