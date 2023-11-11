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

/* cat > c.txt 
 * sleep 10000 
 * chmod +x c.txt 
 * ./prog c.txt 
 * ps ax 
 * ls -l /proc/<PID>/fd -- посмотреть дескрипторы */

int main(int argc, char **argv)
{
    int pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(1);
    }

    if (!pid) {
        /* перенаправление с использованием dup2() */
        int fd0;
        if ((fd0 = open("/dev/null", O_RDONLY, 0)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            fflush(stderr);
            _exit(1);
        }
        dup2(fd0, 0);
        close(fd0);

        int fd1;
        if ((fd1 = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            fflush(stderr);
            _exit(1);            
        }
        dup2(fd1, 1);
        close(fd1);

        int fd2;
        if ((fd2 = open("error.txt", O_WRONLY | O_CREAT | O_APPEND, 0600)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            fflush(stderr);
            _exit(1);            
        }
        dup2(fd2, 2);
        close(fd2);

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