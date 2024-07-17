#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

/* схема запуска процесса на выполнение c перенаправлением */

int main(int argc, char **argv)
{
    int pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(1);
    }

    if (!pid) {
        /* простейшее перенаправление без dup */
        close(0);
        /* гарантируется, что будет первый свободный дескриптор -- 0 
         * правильная семантика для программы с "отсутствующим" stdin */
        if (open("/dev/null", O_RDONLY, 0) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            fflush(stderr);
            _exit(1);
        }

        close(1);
        if (open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            fflush(stderr);
            _exit(1);            
        }

        close(2);
        if (open("error.txt", O_WRONLY | O_CREAT | O_APPEND, 0600) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            fflush(stderr);
            _exit(1);            
        }

        /* запуск */
        execlp(argv[1], argv[1], "1", "2", NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        fflush(stderr);
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