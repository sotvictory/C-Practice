#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define MEMORY_FAILURE -1       /* memory allocation error code */

typedef enum {
    Begin, 
    Pipeline, 
    End, 
    Background, 
    In, 
    Out, 
    Out1, 
    Conv1, 
    ConvP
} vertex;

void mem_error(void)
{
    fprintf(stderr, "Memory allocation did not occur. Exit\n");
    exit(MEMORY_FAILURE);
}

void make_cmd(tree t)
{ 
    t = (tree) malloc(sizeof(tree));
    t->argv = NULL;
    t->infile = NULL;
    t->outfile = NULL;
    t->background = 0;
    t->psubcmd = NULL;
    t->pipe = NULL;
    t->next = NULL;
}

tree build_tree(tree *t)
{
    tree beg_cmd = NULL, cur_cmd = NULL, prev_cmd = NULL;
    vertex V = Begin;

    while (1) {
        switch (V) {

            case Begin:
                break;

            case Pipeline:
                break;

            case End:
                break;

            case Background:
                break;

            case In:
                break;

            case Out:
                break;
            
            case Out1:
                break;
            
            case Conv1:
                break;

            case ConvP:
                break;

        }
    }
}
