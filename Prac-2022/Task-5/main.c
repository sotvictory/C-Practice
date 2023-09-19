#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "list.h"
#include "tree.h"
#include "exec.h"

#define BUFF_SIZE 10

jmp_buf begin;
list *plst; // текущая лексема
backgrndList *bckgrnd; // список фоновых процессов
int exit_val = 0;

void handler(int s)
{
    signal(SIGINT, handler);
}

