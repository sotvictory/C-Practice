#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum next_type {NXT, AND, OR} next_type;

typedef struct cmd_inf {
    list argv;              /* list of command name and its arguments */
    char *infile;           /* redirected standart input file */
    char *outfile;          /* redirected standart output file */
    int append;             /* 1, if append mode enabled */
    int background;         /* 1, if background mode endabled */
    cmd_inf *psubcmd;       /* subshell */
    cmd_inf *pipe;          /* next command after "|" */
    cmd_inf *next;          /* next command after "&" or ";" */
    next_type type;         /* type of neighboring commands connection */
} cmd_inf;

typedef struct cmd_inf *tree;

list plex;

void print_tree(); 
tree clear_tree();
tree build_tree(); 

#endif /* PARSER_H */