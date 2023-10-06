#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct cmd_inf {
    list argv;
    char *infile;
    char *outfile;
    int background;
    cmd_inf *psubcmd;
    cmd_inf *pipe;
    cmd_inf *next;
} cmd_inf;

typedef struct cmd_inf *tree;

list plex;

void print_tree(); 
void clear_tree();
tree build_tree(); 

#endif /* PARSER_H */