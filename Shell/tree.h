#ifndef TREE_H
#define TREE_H

#include "list.h"

typedef enum nexttype
{
	NXT,
	AND,
	OR
} nexttype;

typedef struct cmd_inf node;
typedef struct cmd_inf *tree;
struct cmd_inf
{
	char **argv;   /*List of command name and args*/
	char *infile;  /*reassinged standard input file*/
	char *outfile; /*reassinged standard output file*/
	int append;	   /*defines output mode: > (0) or >> (1)*/
	int backgrnd;  /*=1, if command is to be executed in background mode*/
	tree psubcmd;  /*commands to be run in subshell*/
	tree pipe;	   /*next command after “|”*/
	tree next;	   /*next command after “;” (or after “&”)*/
	nexttype type; /*linking with next command via ; or && or ||*/
};

tree buildtree(list lst, int sizelist);
void printtree(tree t, int shift);
tree cleartree(tree t);

#endif /* TREE_H */