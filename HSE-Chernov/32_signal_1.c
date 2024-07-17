#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t int_flag = 0, segv_flag = 0;

void sigint_handler(int s)
{
    int_flag = 1;
}

void sigsegv_handler(int s)
{
    segv_flag = 1;
}

/* посмотреть маски: cat /proc/10840/status */
int main(void)
{
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = sigint_handler,\
                .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGSEGV, &(struct sigaction) {.sa_handler = sigsegv_handler,\
                .sa_flags = SA_RESTART}, NULL);

    int x;
    sigset_t ss;
    while(scanf("%d", &x) == 1) {
        printf("%d %d\n", int_flag, segv_flag);
        int_flag = 0;
        segv_flag = 0;
        if (x == 1) {
            sigemptyset(&ss);
            sigaddset(&ss, SIGINT);
            sigprocmask(SIG_BLOCK, &ss, NULL);
            printf("SIGINT blocked\n");
        } else if (x == 2) {
            sigemptyset(&ss);
            sigaddset(&ss, SIGINT);
            sigprocmask(SIG_UNBLOCK, &ss, NULL);
            printf("SIGINT unblocked\n");
        } else if (x == 3) {
            sigemptyset(&ss);
            sigaddset(&ss, SIGSEGV);
            sigprocmask(SIG_BLOCK, &ss, NULL);
            printf("SIGSEGV blocked\n");
        } else if (x == 4) {
            sigemptyset(&ss);
            sigaddset(&ss, SIGSEGV);
            sigprocmask(SIG_UNBLOCK, &ss, NULL);
            printf("SISEGV unblocked\n");
        } else if (x == 5) {
            printf("SIGINT sent\n");
            kill(getpid(), SIGINT);
        } else if (x == 6) {
            /* здесь зациклимся: ошибочная операция рестартуется */
            (*((int *)(intptr_t)x))++;
        } else if (x == 7) {
            printf("SIGSEGV sent\n");
            kill(getpid(), SIGSEGV);
        } else {
            printf("%d\n", x);
        }
    }
}