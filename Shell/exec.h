#ifndef EXEC_H
#define EXEC_H

#include "tree.h"

typedef struct backgrndList *intlist;

struct backgrndList
{
    pid_t pid;
    int cnt;
    struct backgrndList *next;
    char *name;
};

void exe(tree t, intlist *zlist, list lst, int *sizelist);
intlist waitzombies(intlist zlist);
intlist clearintlist(intlist zlist);
void killall(intlist zlist, int level);

#endif /* EXEC_H */