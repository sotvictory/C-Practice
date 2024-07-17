#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

/* более-менее полная схема запуска процесса на выполнение */

int main(int argc, char **argv)
{
    int pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(1);
    }

    if (!pid) {
        /* здесь можем производить настройки: */
        execlp(argv[1], argv[1], "1", "2", NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        fflush(stderr);
        /* в сыне желательно использовать _exit(), а не exit():
         * минимально воздействуем на то состояние программы,
         * которое было унаследовано от отца */
        _exit(1);
    }

    int status = 0;
    wait(&status);

    if (WIFEXITED(status)) {
        fprintf(stderr, "exited: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        fprintf(stderr, "signaled: %d\n", WTERMSIG(status));
    }

    return 0;
}