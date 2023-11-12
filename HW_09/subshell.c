#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { INP_ERR, FORK_ERR, EXEC_ERR, PIPE_ERR };

/* ((pr1 | pr2); pr3) | pr4 */

int main(int argc, char **argv)
{
    pid_t pid1, pid2, pid3, pid4;
    int fds[2] = {0}; 

    if (argc != 5) {
        fprintf(stderr, "usage: %s <pr1> <pr2> <pr3> <pr4>\n", argv[0]);
        exit(INP_ERR);
    }

    if (pipe(fds) < 0) {
        fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
        exit(PIPE_ERR);
    }

    /* pr1 */
    if ((pid1 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);        
    } else if (pid1 == 0) {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        execlp(argv[1], argv[1], NULL);
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
        execlp(argv[2], argv[2], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds[0]);

    while (wait(NULL) > 0) {}

    if (pipe(fds) < 0) {
        fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
        exit(PIPE_ERR);
    }

    /* pr3 */
    if ((pid3 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);   
    } else if (pid3 == 0) {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        execlp(argv[3], argv[3], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds[1]);

    /* pr4 */
    if ((pid4 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);   
    } else if (pid4 == 0) {
        dup2(fds[0], 0);
        close(fds[0]);
        execlp(argv[4], argv[4], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds[0]);

    while (wait(NULL) > 0) {}

    return 0;
}