#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define WORK_TIME 10

void work(int rfd, int wfd)
{
    while (1) {
        int buf;

        read(rfd, &buf, sizeof(buf));
        /* начало критической секции */
        printf("%d %d\n", getpid(), buf);
        fflush(stdout);
        buf++;
        /* конец критической секции */
        write(wfd, &buf, sizeof(buf));
    }
}

void alrm_handler(int signum)
{
    kill(0, SIGKILL);
    exit(0);
}
 
int main(int argc, char **argv)
{
    int fds1[2], fds2[2];

    signal(SIGALRM, alrm_handler); // TODO: 2 type of handling
    alarm(WORK_TIME);

    /* -D_GNU_SOURCE */
    pipe2(fds1, O_CLOEXEC);
    pipe2(fds2, O_CLOEXEC);

    int pid1 = fork();
    if (!pid1) {
        work(fds2[0], fds1[1]);
        _exit(1);
    }

    int pid2 = fork();
    if (!pid2) {
        work(fds1[0], fds2[1]);
        _exit(1);
    }

    /* начало работы, выход из deadlock'а */
    int buf = 1;
    write(fds2[1], &buf, sizeof(buf));

    close(fds1[1]);
    close(fds1[0]);
    close(fds2[1]);
    close(fds2[0]);

    while (wait(NULL) > 0);
}