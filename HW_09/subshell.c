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
    
    int fds1[2], fds2[2];
    int stdin_fd, stdout_fd;
    int pid1, pid2, pid3, pid4;

    if (argc != 5) {
        fprintf(stderr, "usage: %s <pr1> <pr2> <pr3> <pr4>\n", argv[0]);
        exit(INP_ERR);
    }

    stdin_fd = dup(0);
    stdout_fd = dup(1);
    
    pipe(fds1);
    
    if ((pid1 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid1 == 0) {
        dup2(fds1[1], 1);
        close(fds1[1]);
        close(fds1[0]);
        close(stdin_fd);
        close(stdout_fd);
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }
    
    close(fds1[1]);
    pipe(fds2);
    
    if ((pid2 =fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid2 == 0) {
        dup2(fds1[0], 0);
        close(fds1[0]);
        dup2(fds2[1], 1);
        close(fds2[1]);
        close(fds2[0]);
        close(stdin_fd);
        close(stdout_fd);
        execlp(argv[2], argv[2], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }
    
    close(fds1[0]);

    wait(NULL);
    wait(NULL);
    
    if ((pid3 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid3 == 0) {
        dup2(stdin_fd, 0);
        close(stdin_fd);
        close(stdout_fd);
        close(fds2[0]);
        close(fds2[1]);
        execlp(argv[3], argv[3], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }
    
    close(stdin_fd);
    close(fds2[1]);
    
    if ((pid4 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid4 == 0) {
        dup2(stdout_fd, 1);
        close(stdout_fd);
        dup2(fds2[0], 0);
        close(fds2[0]);
        execlp(argv[4], argv[4], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    close(fds2[0]);
    close(stdout_fd);
    
    wait(NULL);

    return 0;
}