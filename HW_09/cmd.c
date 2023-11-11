#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { INP_ERR, FORK_ERR, OPEN_ERR, EXEC_ERR, PIPE_ERR };

/* pr1 arg1 arg2 | pr2; pr3 >> f1.dat */

int main(int argc, char **argv)
{
    pid_t pid1, pid2, pid3;
    int fds[2], fd;

    if (argc != 6) {
        fprintf(stderr, "usage: %s <pr1> <arg1> <arg2> <pr2> <pr3>\n", argv[0]);
        exit(INP_ERR);
    }    

    if (pipe(fds) < 0) {
        fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
        exit(PIPE_ERR);        
    }

    /* pr1 arg1 arg2 */
    if ((pid1 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);         
    } else if (pid1 == 0) {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        execlp(argv[1], argv[1], argv[2], argv[3], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    close(fds[1]);

    /* pr2 */
    if ((pid2 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);         
    } else if (pid2 == 0) {
        dup2(fds[0], 0);
        close(fds[0]);
        execlp(argv[4], argv[4], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    close(fds[0]);

    while (wait(NULL) > 0) {}

    /* pr3 >> f1.dat */
    if ((pid3 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);         
    } else if (pid3 == 0){
        if ((fd = open("f1.dat", O_WRONLY | O_CREAT | O_APPEND, 0600)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            _exit(OPEN_ERR);
        }
        dup2(fd, 1);
        close(fd);
        execlp(argv[5], argv[5], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    wait(NULL);

    return 0;
}