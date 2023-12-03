#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/signalfd.h>

#define WORK_TIME 10

void work(int rfd, int wfd, int sfd)
{
    while (1) {
        struct signalfd_siginfo sss;
        read(sfd, &sss, sizeof(sss));

        /* начало критической секции */
        int val;
        read(rfd, &val, sizeof(val));
        printf("%d %d\n", getpid(), val);
        fflush(stdout);
        val++;
        write(wfd, &val, sizeof(val));
        /* конец критической секции */

        /* условно возвращаем сигнал обратно */
        kill(sss.ssi_pid, SIGUSR1);
    }
}

void alrm_handler(int signum)
{
    kill(0, SIGKILL);
    exit(0);
}

int main(void)
{
    int pfd[2];

    signal(SIGALRM, alrm_handler); // TODO: 2 type of handling
    alarm(WORK_TIME);

    pipe(pfd);

    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);
    sigprocmask(SIG_BLOCK, &ss, NULL);
    int sfd = signalfd(-1, &ss, 0);

    pid_t pid1;
    if (!(pid1 = fork())) {
        work(pfd[0], pfd[1], sfd);
        _exit(0);
    }

    if (!fork()) {
        int val = 0;
        write(pfd[1], &val, sizeof(val));
        /* в родителе нельзя, тк sss.ssi_pid*/
        kill(pid1, SIGUSR1);

        work(pfd[0], pfd[1], sfd);
        _exit(0);
    }

    while (wait(NULL) > 0);
}