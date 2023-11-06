#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/* ls | cat | wc
 * pipe2()*/
int main(int argc, char **argv)
{
    int fds1[2];
    int fds2[2];

    /* -D_GNU_SOURCE
     * можно оптимизировать и сделать так, 
     * чтобы открыто было всего 3 ф.д. при
     * любом количестве пайпов */
    pipe2(fds1, O_CLOEXEC);
    pipe2(fds2, O_CLOEXEC);

    int pid1 = fork();
    if (!pid1) {
        /* только stdout */
        dup2(fds1[1], 1); 
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    int pid2 = fork();
    if (!pid2) {
        /* и stdout, и stdin */
        dup2(fds1[0], 0);
        dup2(fds2[1], 1);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }

    int pid3 = fork();
    if (!pid3) {
        /* только stdin */
        dup2(fds2[0], 0);
        execlp(argv[3], argv[2], NULL);
        _exit(1);
    }

    close(fds1[1]);
    close(fds1[0]);
    close(fds2[0]);
    close(fds2[1]);

    while (wait(NULL) > 0) {}
}