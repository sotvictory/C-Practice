#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_LEN 256

enum { INP_ERR, OPEN_ERR, FORK_ERR};

/* Написать программу параллельного поиска подстроки в файлах, имена которых заданы в командной строке. 
 * Поиск подстроки в каждом из заданных файлов организовывать в процессе-потомке, создав для каждого файла свой процесс.
 * Программа завершается, как только подстрока будет найдена в одном из файлов. 
 * Все незавершенные процессы должны быть прерваны. */

int main(int argc, char **argv)
{
    char str[MAX_LEN];
    char *substr = "word";
    pid_t pid;
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <file_1> [<file_2>] [<file_3>] ... [<file_n>]\n", argv[0]);
        exit(INP_ERR);
    }

    for (i = 1; i < argc; i++) {
        if ((pid = fork()) == 0) {
            FILE *f = fopen(argv[i], "r");
            if (f == NULL) {
                fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
                _exit(OPEN_ERR);
            }
            while (fgets(str, MAX_LEN, f) != NULL) {
                if (strstr(str, substr) != NULL) {
                    fprintf(stdout, "substring '%s' was found in file %s\n", substr, argv[i]);
                    fflush(stdout);
                    kill(0, SIGTERM);
                }
            }
            fclose(f);
            _exit(0);
        } else if (pid < 0) {
            fprintf(stderr, "fork() failed: %s\n", strerror(errno));
            exit(FORK_ERR);  
        }
    }

    for (i = 1; i < argc - 1; i++)
        wait(NULL); 

    return 0;
}