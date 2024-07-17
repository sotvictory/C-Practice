#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { FORK_ERR, EXEC_ERR, PIPE_ERR };

int main(int argc, char **argv)
{
    pid_t pid;
    int fds[2] = {0}, fd_in, fd_out, fd_next_in, i;

    if (argc == 1) {
        return 0;
    } else if (argc == 2) {
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    if (pipe(fds) < 0) {
        fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
        exit(PIPE_ERR);
    }

    fd_out = fds[1];
    fd_next_in = fds[0];

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid == 0) {
        close(fd_next_in);
        dup2(fd_out, 1);
        close(fd_out);
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    fd_in = fd_next_in;

    for (i = 2; i < argc - 1; i++) {
        close(fd_out);
        if (pipe(fds) < 0) {
            fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
            exit(PIPE_ERR);
        }
        fd_out = fds[1];
        fd_next_in = fds[0];
        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork() failed: %s\n", strerror(errno));
            exit(FORK_ERR);
        } else if (pid == 0) {
            dup2(fd_in, 0);
            close(fd_in);
            dup2(fd_out, 1);
            close(fd_out);
            execlp(argv[i], argv[i], NULL);
            fprintf(stderr, "exec() failed: %s\n", strerror(errno));
            _exit(EXEC_ERR);           
        }
        close(fd_in);
        fd_in = fd_next_in;
    }

    close(fd_out);

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid == 0) {
        dup2(fd_in, 0);
        close(fd_in);
        execlp(argv[argc - 1], argv[argc - 1], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);
    }

    close(fd_in);

    for (i = 1; i < argc - 1; i++)
        wait(NULL);

    return 0;
}