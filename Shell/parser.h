#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define TREE_ERR (tree)-1
#define PRINT_SHIFT 5

typedef enum next_type {
    NEXT, 
    AND, 
    OR
} next_type;

typedef struct cmd_inf *tree;

typedef struct cmd_inf {
    list argv;              /* list of command name and its arguments */
    char *infile;           /* redirected standart input file */
    char *outfile;          /* redirected standart output file */
    int append;             /* 1, if append mode enabled */
    int background;         /* 1, if background mode endabled */
    tree psubcmd;           /* subshell */
    tree pipe;              /* next command after "|" */
    tree next;              /* next command after "&" or ";" */
    next_type type;         /* type of neighboring commands connection */
} cmd_inf;

void print_tree(tree t, int shift);
tree clear_tree(tree cmd);
tree build_tree(list lst, int size_lst);

#endif /* PARSER_H */