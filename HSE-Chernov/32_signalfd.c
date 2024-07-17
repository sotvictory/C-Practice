#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/signalfd.h>

int main(void)
{
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigprocmask(SIG_BLOCK, &ss, NULL);

    int sfd = signalfd(-1, &ss, 0);

    while (1) {
        struct signalfd_siginfo sss;
        ssize_t z = read(sfd, &sss, sizeof(sss));
        printf("SIGINT %zu\n", z);
        printf("    signo: %d\n", sss.ssi_signo);
        /* PID = 0 -- сигнал послала ОС */
        printf("    pid: %d\n", sss.ssi_pid);
    }
}