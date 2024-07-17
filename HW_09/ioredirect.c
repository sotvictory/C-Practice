#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { INP_ERR, FORK_ERR, OPEN_ERR, EXEC_ERR };

/* pr1 < f.dat > f.res */

int main(int argc, char **argv)
{
    pid_t pid;
    int status, fd0, fd1;

    if (argc != 4) {
        fprintf(stderr, "usage: %s <pr1> <input_file> <output_file>\n", argv[0]);
        exit(INP_ERR);
    }

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);
    } else if (pid == 0) {
        /* set up new program: input redirection */
        if ((fd0 = open(argv[2], O_RDONLY | O_CLOEXEC, 0)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            //fflush(stderr); // if stderr is not buffered by default
            _exit(OPEN_ERR); // _exit() instead of exit(): minimal impact on the parent process
        }
        dup2(fd0, 0);
        //close(fd0);

        /* output redirection */
        if ((fd1 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0600)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            //fflush(stderr);
            _exit(OPEN_ERR);            
        }
        dup2(fd1, 1);
        //close(fd1);

        /* execute the program */
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        //fflush(stderr);
        _exit(EXEC_ERR);
    }

    wait(&status);

    if (WIFEXITED(status)) {
        fprintf(stderr, "exited: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        fprintf(stderr, "signaled: %d\n", WTERMSIG(status));
    }

    return 0;
}