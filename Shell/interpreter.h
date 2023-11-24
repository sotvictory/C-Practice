#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

typedef struct process *process_list;

typedef struct process {
    pid_t pid;
    int cnt;
    char *name;
    process_list next;
} process;

void execute(tree cmd, process_list *zombie_list);
void clear_zombie(process_list zombie_list, int level);
process_list wait_zombies(process_list zombie_list);
process_list clear_process_list(process_list list);

#endif /* INTERPRETER_H */