#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/* ls | cat
 * pipe2()*/
int main(int argc, char **argv)
{
    int fds[2];

    /* gcc 30_pipe_2.c -o 30 -D_GNU_SOURCE  */
    pipe2(fds, O_CLOEXEC);

    int pid1 = fork();
    if (!pid1) {
        dup2(fds[1], 1); 
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    int pid2 = fork();
    if (!pid2) {
        dup2(fds[0], 0);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }

    close(fds[1]);
    close(fds[0]);

    while (wait(NULL) > 0) {}
}